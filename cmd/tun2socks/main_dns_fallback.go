package main

import (
	"flag"

	"go-tun2socks/core"
	"go-tun2socks/proxy/dnsfallback"
)

func init() {
	args.DNSFallback = flag.Bool("dnsFallback", false, "Enable DNS fallback over TCP (overrides the UDP proxy handler).")

	registerHandlerCreater("dnsfallback", func() {
		core.RegisterUDPConnHandler(dnsfallback.NewUDPHandler())
	})
}
