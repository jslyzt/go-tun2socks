/**
 * @file
 * IPv4 protocol definitions
 */

#ifndef LWIP_HDR_PROT_IP4_H
#define LWIP_HDR_PROT_IP4_H

#include "lwip/arch.h"
#include "lwip/ip4_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** This is the packed version of ip4_addr_t,
    used in network headers that are itself packed */
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip4_addr_packed {
    PACK_STRUCT_FIELD(u32_t addr);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

typedef struct ip4_addr_packed ip4_addr_p_t;

/* Size of the IPv4 header. Same as 'sizeof(struct ip_hdr)'. */
#define IP_HLEN 20
/* Maximum size of the IPv4 header with options. */
#define IP_HLEN_MAX 60

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
/* The IPv4 header */
struct ip_hdr {
    /* version / header length */
    PACK_STRUCT_FLD_8(u8_t _v_hl);
    /* type of service */
    PACK_STRUCT_FLD_8(u8_t _tos);
    /* total length */
    PACK_STRUCT_FIELD(u16_t _len);
    /* identification */
    PACK_STRUCT_FIELD(u16_t _id);
    /* fragment offset field */
    PACK_STRUCT_FIELD(u16_t _offset);
#define IP_RF 0x8000U        /* reserved fragment flag */
#define IP_DF 0x4000U        /* don't fragment flag */
#define IP_MF 0x2000U        /* more fragments flag */
#define IP_OFFMASK 0x1fffU   /* mask for fragmenting bits */
    /* time to live */
    PACK_STRUCT_FLD_8(u8_t _ttl);
    /* protocol*/
    PACK_STRUCT_FLD_8(u8_t _proto);
    /* checksum */
    PACK_STRUCT_FIELD(u16_t _chksum);
    /* source and destination IP addresses */
    PACK_STRUCT_FLD_S(ip4_addr_p_t src);
    PACK_STRUCT_FLD_S(ip4_addr_p_t dest);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

/* Macros to get struct ip_hdr fields: */
#define IPH_V(hdr)  ((hdr)->_v_hl >> 4)
#define IPH_HL(hdr) ((hdr)->_v_hl & 0x0f)
#define IPH_HL_BYTES(hdr) ((u8_t)(IPH_HL(hdr) * 4))
#define IPH_TOS(hdr) ((hdr)->_tos)
#define IPH_LEN(hdr) ((hdr)->_len)
#define IPH_ID(hdr) ((hdr)->_id)
#define IPH_OFFSET(hdr) ((hdr)->_offset)
#define IPH_OFFSET_BYTES(hdr) ((u16_t)((lwip_ntohs(IPH_OFFSET(hdr)) & IP_OFFMASK) * 8U))
#define IPH_TTL(hdr) ((hdr)->_ttl)
#define IPH_PROTO(hdr) ((hdr)->_proto)
#define IPH_CHKSUM(hdr) ((hdr)->_chksum)

/* Macros to set struct ip_hdr fields: */
#define IPH_VHL_SET(hdr, v, hl) (hdr)->_v_hl = (u8_t)((((v) << 4) | (hl)))
#define IPH_TOS_SET(hdr, tos) (hdr)->_tos = (tos)
#define IPH_LEN_SET(hdr, len) (hdr)->_len = (len)
#define IPH_ID_SET(hdr, id) (hdr)->_id = (id)
#define IPH_OFFSET_SET(hdr, off) (hdr)->_offset = (off)
#define IPH_TTL_SET(hdr, ttl) (hdr)->_ttl = (u8_t)(ttl)
#define IPH_PROTO_SET(hdr, proto) (hdr)->_proto = (u8_t)(proto)
#define IPH_CHKSUM_SET(hdr, chksum) (hdr)->_chksum = (chksum)


#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_PROT_IP4_H */
