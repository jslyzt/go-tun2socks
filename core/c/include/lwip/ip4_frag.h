/**
 * @file
 * IP fragmentation/reassembly
 */

#ifndef LWIP_HDR_IP4_FRAG_H
#define LWIP_HDR_IP4_FRAG_H

#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"

#if LWIP_IPV4

#ifdef __cplusplus
extern "C" {
#endif

#if IP_REASSEMBLY
/* The IP reassembly timer interval in milliseconds. */
#define IP_TMR_INTERVAL 1000

/** IP reassembly helper struct.
 * This is exported because memp needs to know the size.
 */
struct ip_reassdata {
    struct ip_reassdata* next;
    struct pbuf* p;
    struct ip_hdr iphdr;
    u16_t datagram_len;
    u8_t flags;
    u8_t timer;
};

void ip_reass_init(void);
void ip_reass_tmr(void);
struct pbuf* ip4_reass(struct pbuf* p);
#endif /* IP_REASSEMBLY */

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
#ifndef LWIP_PBUF_CUSTOM_REF_DEFINED
#define LWIP_PBUF_CUSTOM_REF_DEFINED
/** A custom pbuf that holds a reference to another pbuf, which is freed
 * when this custom pbuf is freed. This is used to create a custom PBUF_REF
 * that points into the original pbuf. */
struct pbuf_custom_ref {
    /** 'base class' */
    struct pbuf_custom pc;
    /** pointer to the original pbuf that is referenced */
    struct pbuf* original;
};
#endif /* LWIP_PBUF_CUSTOM_REF_DEFINED */
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */

err_t ip4_frag(struct pbuf* p, struct netif* netif, const ip4_addr_t* dest);
#endif /* IP_FRAG */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV4 */

#endif /* LWIP_HDR_IP4_FRAG_H */
