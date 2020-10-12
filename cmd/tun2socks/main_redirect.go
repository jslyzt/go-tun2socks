package main

import (
	"tun2socks/core"
	"tun2socks/proxy/redirect"
)

func init() {
	registerHandlerCreater("redirect", func() {
		core.RegisterTCPConnHandler(redirect.NewTCPHandler(*args.ProxyServer))
		core.RegisterUDPConnHandler(redirect.NewUDPHandler(*args.ProxyServer, *args.UDPTimeout))
	})
}
