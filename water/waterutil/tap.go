package waterutil

import (
	"net"
)

// Tagging 标签
type Tagging int

// Indicating whether/how a MAC frame is tagged. The value is number of bytes taken by tagging.
const (
	NotTagged    Tagging = 0
	Tagged       Tagging = 4
	DoubleTagged Tagging = 8
)

// MACDestination mac目的地
func MACDestination(macFrame []byte) net.HardwareAddr {
	return net.HardwareAddr(macFrame[:6])
}

// MACSource mac来源
func MACSource(macFrame []byte) net.HardwareAddr {
	return net.HardwareAddr(macFrame[6:12])
}

// MACTagging mac标签
func MACTagging(macFrame []byte) Tagging {
	if macFrame[12] == 0x81 && macFrame[13] == 0x00 {
		return Tagged
	} else if macFrame[12] == 0x88 && macFrame[13] == 0xa8 {
		return DoubleTagged
	}
	return NotTagged
}

// MACEthertype mac网卡类型
func MACEthertype(macFrame []byte) Ethertype {
	ethertypePos := 12 + MACTagging(macFrame)
	return Ethertype{macFrame[ethertypePos], macFrame[ethertypePos+1]}
}

// MACPayload mac payload
func MACPayload(macFrame []byte) []byte {
	return macFrame[12+MACTagging(macFrame)+2:]
}

// IsBroadcast 是否广播
func IsBroadcast(addr net.HardwareAddr) bool {
	return addr[0] == 0xff && addr[1] == 0xff && addr[2] == 0xff && addr[3] == 0xff && addr[4] == 0xff && addr[5] == 0xff
}

// IsIPv4Multicast 是否ipv4多播
func IsIPv4Multicast(addr net.HardwareAddr) bool {
	return addr[0] == 0x01 && addr[1] == 0x00 && addr[2] == 0x5e
}
