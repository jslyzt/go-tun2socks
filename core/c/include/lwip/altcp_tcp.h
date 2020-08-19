/**
 * @file
 * Application layered TCP connection API (to be used from TCPIP thread)\n
 * This interface mimics the tcp callback API to the application while preventing
 * direct linking (much like virtual functions).
 * This way, an application can make use of other application layer protocols
 * on top of TCP without knowing the details (e.g. TLS, proxy connection).
 *
 * This file contains the base implementation calling into tcp.
 */

#ifndef LWIP_HDR_ALTCP_TCP_H
#define LWIP_HDR_ALTCP_TCP_H

#include "lwip/opt.h"

#if LWIP_ALTCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/altcp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct altcp_pcb* altcp_tcp_new_ip_type(u8_t ip_type);

#define altcp_tcp_new() altcp_tcp_new_ip_type(IPADDR_TYPE_V4)
#define altcp_tcp_new_ip6() altcp_tcp_new_ip_type(IPADDR_TYPE_V6)

struct altcp_pcb* altcp_tcp_alloc(void* arg, u8_t ip_type);

struct tcp_pcb;
struct altcp_pcb* altcp_tcp_wrap(struct tcp_pcb* tpcb);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_ALTCP */

#endif /* LWIP_HDR_ALTCP_TCP_H */
