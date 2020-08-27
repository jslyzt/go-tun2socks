package tun

import (
	"go-tun2socks/water"
	"io"
)

// OpenTunDevice open tun
func OpenTunDevice(name, addr, gw, mask string, dns []string, persist bool) (io.ReadWriteCloser, error) {
	cfg := water.Config{
		DeviceType: water.TUN,
	}
	cfg.InterfaceName = name
	cfg.Addr = addr
	cfg.GateWay = gw
	cfg.Mask = mask
	cfg.DNS = dns

	tunDev, err := water.New(cfg)
	if err != nil {
		return nil, err
	}
	return tunDev, nil
}
