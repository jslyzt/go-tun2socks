/*
 * Definitions for tcp compression routines.
 *
 */

#include "netif/ppp/ppp_opts.h"
#if PPP_SUPPORT && VJ_SUPPORT /* don't build if not configured for use in lwipopts.h */

#ifndef VJ_H
#define VJ_H

#include "lwip/ip.h"
#include "lwip/priv/tcp_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SLOTS 16 /* must be > 2 and < 256 */
#define MAX_HDR   128

/* packet types */
#define TYPE_IP               0x40
#define TYPE_UNCOMPRESSED_TCP 0x70
#define TYPE_COMPRESSED_TCP   0x80
#define TYPE_ERROR            0x00

/* Bits in first octet of compressed packet */
#define NEW_C 0x40 /* flag bits for what changed in a packet */
#define NEW_I 0x20
#define NEW_S 0x08
#define NEW_A 0x04
#define NEW_W 0x02
#define NEW_U 0x01

/* reserved, special-case values of above */
#define SPECIAL_I (NEW_S|NEW_W|NEW_U) /* echoed interactive traffic */
#define SPECIAL_D (NEW_S|NEW_A|NEW_W|NEW_U) /* unidirectional data */
#define SPECIALS_MASK (NEW_S|NEW_A|NEW_W|NEW_U)

#define TCP_PUSH_BIT 0x10


/*
 * "state" data for each active tcp conversation on the wire.  This is
 * basically a copy of the entire IP/TCP header from the last packet
 * we saw from the conversation together with a small identifier
 * the transmit & receive ends of the line use to locate saved header.
 */
struct cstate {
    struct cstate* cs_next; /* next most recently used state (xmit only) */
    u16_t cs_hlen;        /* size of hdr (receive only) */
    u8_t cs_id;           /* connection # associated with this state */
    u8_t cs_filler;
    union {
        char csu_hdr[MAX_HDR];
        struct ip_hdr csu_ip;     /* ip/tcp hdr from most recent packet */
    } vjcs_u;
};
#define cs_ip vjcs_u.csu_ip
#define cs_hdr vjcs_u.csu_hdr


struct vjstat {
    u32_t vjs_packets;        /* outbound packets */
    u32_t vjs_compressed;     /* outbound compressed packets */
    u32_t vjs_searches;       /* searches for connection state */
    u32_t vjs_misses;         /* times couldn't find conn. state */
    u32_t vjs_uncompressedin; /* inbound uncompressed packets */
    u32_t vjs_compressedin;   /* inbound compressed packets */
    u32_t vjs_errorin;        /* inbound unknown type packets */
    u32_t vjs_tossed;         /* inbound packets tossed because of error */
};

/*
 * all the state data for one serial line (we need one of these per line).
 */
struct vjcompress {
    struct cstate* last_cs;          /* most recently used tstate */
    u8_t last_recv;                /* last rcvd conn. id */
    u8_t last_xmit;                /* last sent conn. id */
    u16_t flags;
    u8_t maxSlotIndex;
    u8_t compressSlot;             /* Flag indicating OK to compress slot ID. */
#if LINK_STATS
    struct vjstat stats;
#endif
    struct cstate tstate[MAX_SLOTS]; /* xmit connection states */
    struct cstate rstate[MAX_SLOTS]; /* receive connection states */
};

/* flag values */
#define VJF_TOSS 1U /* tossing rcvd frames because of input err */

extern void  vj_compress_init(struct vjcompress* comp);
extern u8_t  vj_compress_tcp(struct vjcompress* comp, struct pbuf** pb);
extern void  vj_uncompress_err(struct vjcompress* comp);
extern int   vj_uncompress_uncomp(struct pbuf* nb, struct vjcompress* comp);
extern int   vj_uncompress_tcp(struct pbuf** nb, struct vjcompress* comp);

#ifdef __cplusplus
}
#endif

#endif /* VJ_H */

#endif /* PPP_SUPPORT && VJ_SUPPORT */
