package tun

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"os/exec"
	"strings"
	"sync"
	"syscall"
	"unsafe"

	"golang.org/x/sys/windows"
	"golang.org/x/sys/windows/registry"
)

// 常量定义
const (
	TapWin32MaxRegSize    = 256
	TuntapComponentID0901 = "tap0901"
	TuntapComponentID0801 = "tap0801"
	NetworkKey            = `SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}`
	AdapterKey            = `SYSTEM\CurrentControlSet\Control\Class\{4D36E972-E325-11CE-BFC1-08002BE10318}`
)

func ctlCode(deviceType, function, method, access uint32) uint32 {
	return (deviceType << 16) | (access << 14) | (function << 2) | method
}

func tapControlCode(request, method uint32) uint32 {
	return ctlCode(34, request, method, 0)
}

// 变量定义
var (
	k32                         = windows.NewLazySystemDLL("kernel32.dll")
	procGetOverlappedResult     = k32.NewProc("GetOverlappedResult")
	TapIoctlGetMtu              = tapControlCode(3, 0)
	TapIoctlSetMediaStatus      = tapControlCode(6, 0)
	TapIoctlConfigTun           = tapControlCode(10, 0)
	TapWinIoctlConfigDHCPMasq   = tapControlCode(7, 0)
	TapWinIoctlConfigDHCPSetOpt = tapControlCode(9, 0)
)

func decodeUTF16(b []byte) string {
	if len(b)%2 != 0 {
		return ""
	}

	l := len(b) / 2
	u16 := make([]uint16, l)
	for i := 0; i < l; i++ {
		u16[i] = uint16(b[2*i]) + (uint16(b[2*i+1]) << 8)
	}
	return windows.UTF16ToString(u16)
}

func getTuntapName(componentID string) (string, error) {
	keyName := fmt.Sprintf(NetworkKey+"\\%s\\Connection", componentID)
	key, err := registry.OpenKey(registry.LOCAL_MACHINE, keyName, registry.READ)
	if err != nil {
		key.Close()
		return "", err
	}
	var bufLength uint32 = TapWin32MaxRegSize
	buf := make([]byte, bufLength)
	name, _ := windows.UTF16FromString("Name")
	var valtype uint32
	err = windows.RegQueryValueEx(
		windows.Handle(key),
		&name[0],
		nil,
		&valtype,
		&buf[0],
		&bufLength,
	)
	if err != nil {
		key.Close()
		return "", err
	}
	s := decodeUTF16(buf)
	return s, nil
}

func getTuntapComponentID(ifaceName string) (string, string, error) {
	adapters, err := registry.OpenKey(registry.LOCAL_MACHINE, AdapterKey, registry.READ)
	if err != nil {
		return "", "", fmt.Errorf("failed to read adapter list: %v", err)
	}
	defer adapters.Close()
	var i uint32
	for i = 0; i < 1000; i++ {
		var nameLength uint32 = TapWin32MaxRegSize
		buf := make([]uint16, nameLength)
		if err = windows.RegEnumKeyEx(
			windows.Handle(adapters),
			i,
			&buf[0],
			&nameLength,
			nil,
			nil,
			nil,
			nil); err != nil {
			return "", "", fmt.Errorf("failed to read name: %v", err)
		}
		keyName := windows.UTF16ToString(buf[:])
		adapter, err := registry.OpenKey(adapters, keyName, registry.READ)
		defer adapter.Close()
		if err != nil {
			continue
		}
		name, _ := windows.UTF16FromString("ComponentId")
		name2, _ := windows.UTF16FromString("NetCfgInstanceId")
		var valtype uint32
		var componentID = make([]byte, TapWin32MaxRegSize)
		var componentLen = uint32(len(componentID))
		if err = windows.RegQueryValueEx(
			windows.Handle(adapter),
			&name[0],
			nil,
			&valtype,
			&componentID[0],
			&componentLen); err != nil {
			continue
		}

		id := decodeUTF16(componentID)
		if id == TuntapComponentID0901 || id == TuntapComponentID0801 {
			var valtype uint32
			var netCfgInstanceID = make([]byte, TapWin32MaxRegSize)
			var netCfgInstanceIDLen = uint32(len(netCfgInstanceID))
			if err = windows.RegQueryValueEx(
				windows.Handle(adapter),
				&name2[0],
				nil,
				&valtype,
				&netCfgInstanceID[0],
				&netCfgInstanceIDLen); err != nil {
				return "", "", fmt.Errorf("failed to read net cfg instance id: %v", err)
			}
			s := decodeUTF16(netCfgInstanceID)
			log.Printf("TAP device component ID: %s", s)

			devName, err := getTuntapName(s)
			if err != nil {
				return "", "", fmt.Errorf("failed to get tun/tap name: %v", err)
			}
			if len(ifaceName) == 0 {
				return s, devName, nil
			} else if devName == ifaceName {
				return s, devName, nil
			}
		}
	}
	return "", "", errors.New("not found component id")
}

// OpenTunDevice open tun
func OpenTunDevice(name, addr, gw, mask string, dns []string, persist bool) (io.ReadWriteCloser, error) {
	componentID, devName, err := getTuntapComponentID(name)
	if err != nil {
		return nil, fmt.Errorf("failed to get component ID: %v", err)
	}
	log.Printf("TAP device name: %s", devName)

	devID, _ := windows.UTF16FromString(fmt.Sprintf(`\\.\Global\%s.tap`, componentID))
	// set dhcp with netsh
	cmd := exec.Command("netsh", "interface", "ip", "set", "address", devName, "dhcp")
	cmd.Run()
	cmd = exec.Command("netsh", "interface", "ip", "set", "dns", devName, "dhcp")
	cmd.Run()
	// open
	fd, err := windows.CreateFile(
		&devID[0],
		windows.GENERIC_READ|windows.GENERIC_WRITE,
		windows.FILE_SHARE_READ|windows.FILE_SHARE_WRITE,
		nil,
		windows.OPEN_EXISTING,
		windows.FILE_ATTRIBUTE_SYSTEM|windows.FILE_FLAG_OVERLAPPED,
		//windows.FILE_ATTRIBUTE_SYSTEM,
		0,
	)
	if err != nil {
		return nil, err
	}
	// set addresses with dhcp
	var returnLen uint32
	tunAddr := net.ParseIP(addr).To4()
	tunMask := net.ParseIP(mask).To4()
	gwAddr := net.ParseIP(gw).To4()
	addrParam := append(tunAddr, tunMask...)
	addrParam = append(addrParam, gwAddr...)
	lease := make([]byte, 4)
	binary.BigEndian.PutUint32(lease[:], 86400)
	addrParam = append(addrParam, lease...)
	err = windows.DeviceIoControl(
		fd,
		TapWinIoctlConfigDHCPMasq,
		&addrParam[0],
		uint32(len(addrParam)),
		&addrParam[0],
		uint32(len(addrParam)),
		&returnLen,
		nil,
	)
	if err != nil {
		windows.Close(fd)
		return nil, err
	}
	log.Printf("Set %s with net/mask: %s/%s through DHCP", devName, addr, mask)

	// set dns with dncp
	dnsParam := []byte{6, 4}
	primaryDNS := net.ParseIP(dns[0]).To4()
	dnsParam = append(dnsParam, primaryDNS...)
	if len(dns) >= 2 {
		secondaryDNS := net.ParseIP(dns[1]).To4()
		dnsParam = append(dnsParam, secondaryDNS...)
		dnsParam[1] += 4
	}
	err = windows.DeviceIoControl(
		fd,
		TapWinIoctlConfigDHCPSetOpt,
		&dnsParam[0],
		uint32(len(dnsParam)),
		&addrParam[0],
		uint32(len(dnsParam)),
		&returnLen,
		nil,
	)
	if err != nil {
		windows.Close(fd)
		return nil, err
	}
	log.Printf("Set %s with DNS: %s through DHCP", devName, strings.Join(dns, ","))

	// set connect.
	inBuffer := []byte("\x01\x00\x00\x00")
	err = windows.DeviceIoControl(
		fd,
		TapIoctlSetMediaStatus,
		&inBuffer[0],
		uint32(len(inBuffer)),
		&inBuffer[0],
		uint32(len(inBuffer)),
		&returnLen,
		nil,
	)
	if err != nil {
		windows.Close(fd)
		return nil, err
	}
	return newWinTapDev(fd, addr, gw), nil
}

type winTapDev struct {
	// TODO Not sure if a read lock is needed.
	readLock sync.Mutex
	// Write is not allowed concurrent accessing.
	writeLock sync.Mutex

	fd          windows.Handle
	addr        string
	addrIP      net.IP
	gw          string
	gwIP        net.IP
	rBuf        [2048]byte
	wBuf        [2048]byte
	wInitiated  bool
	rOverlapped windows.Overlapped
	wOverlapped windows.Overlapped
}

func newWinTapDev(fd windows.Handle, addr string, gw string) *winTapDev {
	rOverlapped := windows.Overlapped{}
	rEvent, _ := windows.CreateEvent(nil, 0, 0, nil)
	rOverlapped.HEvent = windows.Handle(rEvent)

	wOverlapped := windows.Overlapped{}
	wEvent, _ := windows.CreateEvent(nil, 0, 0, nil)
	wOverlapped.HEvent = windows.Handle(wEvent)

	dev := &winTapDev{
		fd:          fd,
		rOverlapped: rOverlapped,
		wOverlapped: wOverlapped,
		wInitiated:  false,

		addr:   addr,
		addrIP: net.ParseIP(addr).To4(),
		gw:     gw,
		gwIP:   net.ParseIP(gw).To4(),
	}
	return dev
}

func (dev *winTapDev) Read(data []byte) (int, error) {
	dev.readLock.Lock()
	defer dev.readLock.Unlock()

	for {
		var done uint32
		var nr int

		err := windows.ReadFile(dev.fd, dev.rBuf[:], &done, &dev.rOverlapped)
		if err != nil {
			if err != windows.ERROR_IO_PENDING {
				return 0, err
			}
			windows.WaitForSingleObject(dev.rOverlapped.HEvent, windows.INFINITE)
			nr, err = getOverlappedResult(dev.fd, &dev.rOverlapped)
			if err != nil {
				return 0, err
			}
		} else {
			nr = int(done)
		}
		if nr > 14 {
			if isStopMarker(dev.rBuf[14:nr], dev.addrIP, dev.gwIP) {
				return 0, errors.New("received stop marker")
			}

			// discard IPv6 packets
			if dev.rBuf[14]&0xf0 == 0x60 {
				continue
			} else if dev.rBuf[14]&0xf0 == 0x40 {
				if !dev.wInitiated {
					// copy ether header for writing
					copy(dev.wBuf[:], dev.rBuf[6:12])
					copy(dev.wBuf[6:], dev.rBuf[0:6])
					copy(dev.wBuf[12:], dev.rBuf[12:14])
					dev.wInitiated = true
				}
				copy(data, dev.rBuf[14:nr])
				return nr - 14, nil
			}
		}
	}
}

func (dev *winTapDev) Write(data []byte) (int, error) {
	dev.writeLock.Lock()
	defer dev.writeLock.Unlock()

	var done uint32
	var nw int

	payloadL := copy(dev.wBuf[14:], data)
	packetL := payloadL + 14
	err := windows.WriteFile(dev.fd, dev.wBuf[:packetL], &done, &dev.wOverlapped)
	if err != nil {
		if err != windows.ERROR_IO_PENDING {
			return 0, err
		}
		windows.WaitForSingleObject(dev.wOverlapped.HEvent, windows.INFINITE)
		nw, err = getOverlappedResult(dev.fd, &dev.wOverlapped)
		if err != nil {
			return 0, err
		}
	} else {
		nw = int(done)
	}
	if nw != packetL {
		return 0, fmt.Errorf("write %d packet (%d bytes payload), return %d", packetL, payloadL, nw)
	}
	return payloadL, nil
}

func getOverlappedResult(h windows.Handle, overlapped *windows.Overlapped) (int, error) {
	var n int
	r, _, err := syscall.Syscall6(procGetOverlappedResult.Addr(), 4,
		uintptr(h),
		uintptr(unsafe.Pointer(overlapped)),
		uintptr(unsafe.Pointer(&n)), 1, 0, 0)
	if r == 0 {
		return n, err
	}
	return n, nil
}

func (dev *winTapDev) Close() error {
	log.Printf("close winTap device")
	sendStopMarker(dev.addr, dev.gw)
	return windows.Close(dev.fd)
}
