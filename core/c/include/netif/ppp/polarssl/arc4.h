/**
 * \file arc4.h
 *
 *  Based on XySSL: Copyright (C) 2006-2008  Christophe Devine
 */

#include "netif/ppp/ppp_opts.h"
#if LWIP_INCLUDED_POLARSSL_ARC4

#ifndef LWIP_INCLUDED_POLARSSL_ARC4_H
#define LWIP_INCLUDED_POLARSSL_ARC4_H

/**
 * \brief          ARC4 context structure
 */
typedef struct {
    int x;                      /*!< permutation index */
    int y;                      /*!< permutation index */
    unsigned char m[256];       /*!< permutation table */
}
arc4_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          ARC4 key schedule
 *
 * \param ctx      ARC4 context to be initialized
 * \param key      the secret key
 * \param keylen   length of the key
 */
void arc4_setup(arc4_context* ctx, unsigned char* key, int keylen);

/**
 * \brief          ARC4 cipher function
 *
 * \param ctx      ARC4 context
 * \param buf      buffer to be processed
 * \param buflen   amount of data in buf
 */
void arc4_crypt(arc4_context* ctx, unsigned char* buf, int buflen);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_INCLUDED_POLARSSL_ARC4_H */

#endif /* LWIP_INCLUDED_POLARSSL_ARC4 */
