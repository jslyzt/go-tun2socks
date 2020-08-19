/**
 * \file md5.h
 *
 *  Based on XySSL: Copyright (C) 2006-2008  Christophe Devine
 */

#include "netif/ppp/ppp_opts.h"
#if LWIP_INCLUDED_POLARSSL_MD5

#ifndef LWIP_INCLUDED_POLARSSL_MD5_H
#define LWIP_INCLUDED_POLARSSL_MD5_H

/**
 * \brief          MD5 context structure
 */
typedef struct {
    unsigned long total[2];     /*!< number of bytes processed  */
    unsigned long state[4];     /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
}
md5_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          MD5 context setup
 *
 * \param ctx      context to be initialized
 */
void md5_starts(md5_context* ctx);

/**
 * \brief          MD5 process buffer
 *
 * \param ctx      MD5 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void md5_update(md5_context* ctx, const unsigned char* input, int ilen);

/**
 * \brief          MD5 final digest
 *
 * \param ctx      MD5 context
 * \param output   MD5 checksum result
 */
void md5_finish(md5_context* ctx, unsigned char output[16]);

/**
 * \brief          Output = MD5( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   MD5 checksum result
 */
void md5(unsigned char* input, int ilen, unsigned char output[16]);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_INCLUDED_POLARSSL_MD5_H */

#endif /* LWIP_INCLUDED_POLARSSL_MD5 */
