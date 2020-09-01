package water

// PlatformSpecificParams defines parameters in Config that are specific to
// Windows. A zero-value of such type is valid.
type PlatformSpecificParams struct {
	ComponentID         string
	InterfaceName       string
	Addr, GateWay, Mask string
	DNS                 []string
}

func defaultPlatformSpecificParams() PlatformSpecificParams {
	return PlatformSpecificParams{
		ComponentID: "tap0901",
	}
}
