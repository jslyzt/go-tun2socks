/**
 * @file
 * Application layered TCP connection API that executes a proxy-connect.
 *
 * This file provides a starting layer that executes a proxy-connect e.g. to
 * set up TLS connections through a http proxy.
 */

#ifndef LWIP_HDR_APPS_ALTCP_PROXYCONNECT_H
#define LWIP_HDR_APPS_ALTCP_PROXYCONNECT_H

#include "lwip/opt.h"

#if LWIP_ALTCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct altcp_proxyconnect_config {
    ip_addr_t proxy_addr;
    u16_t proxy_port;
};


struct altcp_pcb* altcp_proxyconnect_new(struct altcp_proxyconnect_config* config, struct altcp_pcb* inner_pcb);
struct altcp_pcb* altcp_proxyconnect_new_tcp(struct altcp_proxyconnect_config* config, u8_t ip_type);

struct altcp_pcb* altcp_proxyconnect_alloc(void* arg, u8_t ip_type);

#if LWIP_ALTCP_TLS
struct altcp_proxyconnect_tls_config {
    struct altcp_proxyconnect_config proxy;
    struct altcp_tls_config* tls_config;
};

struct altcp_pcb* altcp_proxyconnect_tls_alloc(void* arg, u8_t ip_type);
#endif /* LWIP_ALTCP_TLS */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_ALTCP */
#endif /* LWIP_HDR_APPS_ALTCP_PROXYCONNECT_H */
