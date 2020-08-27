package water

// PlatformSpecificParams defines parameters in Config that are specific to
// Windows. A zero-value of such type is valid.
type PlatformSpecificParams struct {
	ComponentID         string
	InterfaceName       string
	Network             string
	Addr, GateWay, Mask string
	DNS                 []string
}

func defaultPlatformSpecificParams() PlatformSpecificParams {
	return PlatformSpecificParams{
		ComponentID: "tap0901",
		Network:     "192.168.1.10/24",
	}
}
