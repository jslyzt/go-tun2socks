package main

import (
	"flag"
	"fmt"
	"io"
	"os"
	"os/signal"
	"runtime"
	"strings"
	"syscall"
	"time"

	"go-tun2socks/common/dns/blocker"
	"go-tun2socks/common/log"
	_ "go-tun2socks/common/log/simple" // Register a simple logger.
	"go-tun2socks/core"
	"go-tun2socks/tun"
)

// 常量定义
const (
	MTU = 1500
)

// 变量定义
var (
	version        = "undefined"
	handlerCreater = make(map[string]func(), 0)
	args           = new(CmdArgs)
	lwipWriter     io.Writer
)

////////////////////////////////////////////////////////////////////////////////////////////////////////////

type (
	// CmdArgs cmd参数
	CmdArgs struct {
		Version         *bool
		TunName         *string
		TunAddr         *string
		TunGw           *string
		TunMask         *string
		TunDNS          *string
		TunPersist      *bool
		BlockOutsideDNS *bool
		ProxyType       *string
		ProxyServer     *string
		ProxyHost       *string
		ProxyPort       *uint16
		UDPTimeout      *time.Duration
		LogLevel        *string
		DNSFallback     *bool
	}
)

func registerHandlerCreater(name string, creater func()) {
	handlerCreater[name] = creater
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

func main() {
	args.Version = flag.Bool("version", false, "Print version")
	args.TunName = flag.String("tunName", "tun1", "TUN interface name")
	args.TunAddr = flag.String("tunAddr", "10.255.0.2", "TUN interface address")
	args.TunGw = flag.String("tunGw", "10.255.0.1", "TUN interface gateway")
	args.TunMask = flag.String("tunMask", "255.255.255.0", "TUN interface netmask, it should be a prefixlen (a number) for IPv6 address")
	args.TunDNS = flag.String("tunDns", "8.8.8.8,8.8.4.4", "DNS resolvers for TUN interface (only need on Windows)")
	args.TunPersist = flag.Bool("tunPersist", false, "Persist TUN interface after the program exits or the last open file descriptor is closed (Linux only)")
	args.BlockOutsideDNS = flag.Bool("blockOutsideDns", false, "Prevent DNS leaks by blocking plaintext DNS queries going out through non-TUN interface (may require admin privileges) (Windows only) ")
	args.ProxyType = flag.String("proxyType", "socks", "Proxy handler type")
	args.LogLevel = flag.String("loglevel", "info", "Logging level. (debug, info, warn, error, none)")
	args.ProxyServer = flag.String("proxyServer", "1.2.3.4:1087", "Proxy server address")
	args.UDPTimeout = flag.Duration("udpTimeout", 1*time.Minute, "UDP session timeout")

	flag.Parse()

	if *args.Version {
		fmt.Println(version)
		os.Exit(0)
	}

	// Set log level.
	switch strings.ToLower(*args.LogLevel) {
	case "debug":
		log.SetLevel(log.DEBUG)
	case "info":
		log.SetLevel(log.INFO)
	case "warn":
		log.SetLevel(log.WARN)
	case "error":
		log.SetLevel(log.ERROR)
	case "none":
		log.SetLevel(log.NONE)
	default:
		panic("unsupport logging level")
	}

	// Open the tun device.
	dnsServers := strings.Split(*args.TunDNS, ",")
	tunDev, err := tun.OpenTunDevice(*args.TunName, *args.TunAddr, *args.TunGw, *args.TunMask, dnsServers, *args.TunPersist)
	if err != nil || tunDev == nil {
		log.Fatalf("failed to open tun device: %v", err)
		return
	}

	if runtime.GOOS == "windows" && *args.BlockOutsideDNS {
		if err := blocker.BlockOutsideDNS(*args.TunName); err != nil {
			log.Fatalf("failed to block outside DNS: %v", err)
			return
		}
	}

	// Setup TCP/IP stack.
	lwipWriter := core.NewLWIPStack().(io.Writer)

	// Register TCP and UDP handlers to handle accepted connections.
	if creater, found := handlerCreater[*args.ProxyType]; found {
		creater()
	} else {
		log.Fatalf("unsupported proxy type")
	}

	if args.DNSFallback != nil && *args.DNSFallback {
		// Override the UDP handler with a DNS-over-TCP (fallback) UDP handler.
		if creater, found := handlerCreater["dnsfallback"]; found {
			creater()
		} else {
			log.Fatalf("DNS fallback connection handler not found, build with `dnsfallback` tag")
		}
	}

	// Register an output callback to write packets output from lwip stack to tun
	// device, output function should be set before input any packets.
	core.RegisterOutputFn(func(data []byte) (int, error) {
		return tunDev.Write(data)
	})

	// Copy packets from tun device to lwip stack, it's the main loop.
	go func() {
		_, err := io.CopyBuffer(lwipWriter, tunDev, make([]byte, MTU))
		if err != nil {
			log.Fatalf("copying data failed: %v", err)
		}
	}()

	log.Infof("Running tun2socks")

	osSignals := make(chan os.Signal, 1)
	signal.Notify(osSignals, os.Interrupt, os.Kill, syscall.SIGTERM, syscall.SIGHUP)
	<-osSignals
}
