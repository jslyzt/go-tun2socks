# tun2socks

### 运行参数

    Usage of ./tun2socks:
        -blockOutsideDns
                Prevent DNS leaks by blocking plaintext DNS queries going out through non-TUN interface (may require admin privileges) (Windows only) 
        -dnsFallback
                Enable DNS fallback over TCP (overrides the UDP proxy handler).
        -loglevel string
                Logging level. (debug, info, warn, error, none) (default "info")
        -proxyServer string
                Proxy server address (default "1.2.3.4:1087")
        -proxyType string
                Proxy handler type (default "socks")
        -tunAddr string
                TUN interface address (default "10.255.0.2")
        -tunDns string
                DNS resolvers for TUN interface (only need on Windows) (default "8.8.8.8,8.8.4.4")
        -tunGw string
                TUN interface gateway (default "10.255.0.1")
        -tunMask string
                TUN interface netmask, it should be a prefixlen (a number) for IPv6 address (default "255.255.255.0")
        -tunName string
                TUN interface name (default "tun1")
        -tunPersist
                Persist TUN interface after the program exits or the last open file descriptor is closed (Linux only)
        -udpTimeout duration
                UDP session timeout (default 1m0s)
        -version
                Print version

### 参数示例

    -tunAddr 10.0.0.2 -tunGw 10.0.0.1 -proxyType socks -proxyServer 127.0.0.1:3000 -tunDns 8.8.8.8,4.4.4.4 -tunName vpn
    

### tun创建

    linux:
    sudo ip tuntap add vpn mode tun user jslyzt group jslyzt
    sudo ip link set vpn up
    sudo ip addr add 10.0.0.1/24 dev vpn
    sudo ip a

    sudo ifconfig -a
    sudo netstat -nr

    sudo route add -net 10.0.0.1 netmask 255.255.225.0 10.0.0.2
    sudo route del -net 10.0.0.1 netmask 255.255.255.0

    sudo route add -host 10.0.0.2 dev vpn
    sudo route del -net 10.0.0.2 netmask 255.255.255.255


    sudo ip route add to 192.168.1.18 dev ens33 src 192.168.1.16
    sudo ip route delete default
    sudo ip route add default via "虚拟IP例如10.3.3.3" dev vpn

    windows: