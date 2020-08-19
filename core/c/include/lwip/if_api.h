/**
 * @file
 * Interface Identification APIs from:
 *              RFC 3493: Basic Socket Interface Extensions for IPv6
 *                  Section 4: Interface Identification
 */

#ifndef LWIP_HDR_IF_H
#define LWIP_HDR_IF_H

#include "lwip/opt.h"

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */

#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IF_NAMESIZE NETIF_NAMESIZE

char* lwip_if_indextoname(unsigned int ifindex, char* ifname);
unsigned int lwip_if_nametoindex(const char* ifname);

#if LWIP_COMPAT_SOCKETS
#define if_indextoname(ifindex, ifname)  lwip_if_indextoname(ifindex,ifname)
#define if_nametoindex(ifname) lwip_if_nametoindex(ifname)
#endif /* LWIP_COMPAT_SOCKETS */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_SOCKET */

#endif /* LWIP_HDR_IF_H */
