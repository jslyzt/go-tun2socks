package waterutil

// IsIPv4 是否ipv4
func IsIPv4(packet []byte) bool {
	return 4 == (packet[0] >> 4)
}

// IsIPv6 是否ipv6
func IsIPv6(packet []byte) bool {
	return 6 == (packet[0] >> 4)
}
