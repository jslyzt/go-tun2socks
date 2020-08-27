package waterutil

import (
	"net"
)

// IPv4DSCP ipv4 dscp
func IPv4DSCP(packet []byte) byte {
	return packet[1] >> 2
}

// IPv4ECN ipv4 ecn
func IPv4ECN(packet []byte) byte {
	return packet[1] & 0x03
}

// IPv4Identification ipv4鉴定
func IPv4Identification(packet []byte) [2]byte {
	return [2]byte{packet[4], packet[5]}
}

// IPv4TTL ipv4 ttl
func IPv4TTL(packet []byte) byte {
	return packet[8]
}

// IPv4Protocol ipv4 protocol
func IPv4Protocol(packet []byte) IPProtocol {
	return IPProtocol(packet[9])
}

// IPv4Source ipv4来源
func IPv4Source(packet []byte) net.IP {
	return net.IPv4(packet[12], packet[13], packet[14], packet[15])
}

// SetIPv4Source 设置 ipv4来源
func SetIPv4Source(packet []byte, source net.IP) {
	copy(packet[12:16], source.To4())
}

// IPv4Destination ipv4目的地
func IPv4Destination(packet []byte) net.IP {
	return net.IPv4(packet[16], packet[17], packet[18], packet[19])
}

// SetIPv4Destination 设置ipv4地址
func SetIPv4Destination(packet []byte, dest net.IP) {
	copy(packet[16:20], dest.To4())
}

// IPv4Payload ipv4 payload
func IPv4Payload(packet []byte) []byte {
	ihl := packet[0] & 0x0F
	return packet[ihl*4:]
}

// IPv4SourcePort ipv4来源端口
func IPv4SourcePort(packet []byte) uint16 {
	payload := IPv4Payload(packet)
	return (uint16(payload[0]) << 8) | uint16(payload[1])
}

// IPv4DestinationPort ipv4目的端口
func IPv4DestinationPort(packet []byte) uint16 {
	payload := IPv4Payload(packet)
	return (uint16(payload[2]) << 8) | uint16(payload[3])
}

// SetIPv4SourcePort 设置ipv4来源端口
func SetIPv4SourcePort(packet []byte, port uint16) {
	payload := IPv4Payload(packet)
	payload[0] = byte(port >> 8)
	payload[1] = byte(port & 0xFF)
}

// SetIPv4DestinationPort 设置ipv4目的端口
func SetIPv4DestinationPort(packet []byte, port uint16) {
	payload := IPv4Payload(packet)
	payload[2] = byte(port >> 8)
	payload[3] = byte(port & 0xFF)
}
