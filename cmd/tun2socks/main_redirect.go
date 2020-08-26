package main

import (
	"go-tun2socks/core"
	"go-tun2socks/proxy/redirect"
)

func init() {
	registerHandlerCreater("redirect", func() {
		core.RegisterTCPConnHandler(redirect.NewTCPHandler(*args.ProxyServer))
		core.RegisterUDPConnHandler(redirect.NewUDPHandler(*args.ProxyServer, *args.UDPTimeout))
	})
}
