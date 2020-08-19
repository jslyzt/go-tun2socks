/**
 * @file
 * Application layered TCP/TLS connection API (to be used from TCPIP thread)
 *
 * @defgroup altcp_tls TLS layer
 * @ingroup altcp
 * This file contains function prototypes for a TLS layer.
 * A port to ARM mbedtls is provided in the apps/ tree
 * (LWIP_ALTCP_TLS_MBEDTLS option).
 */

#ifndef LWIP_HDR_ALTCP_TLS_H
#define LWIP_HDR_ALTCP_TLS_H

#include "lwip/opt.h"

#if LWIP_ALTCP /* don't build if not configured for use in lwipopts.h */

#if LWIP_ALTCP_TLS

#include "lwip/altcp.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup altcp_tls
 * ALTCP_TLS configuration handle, content depends on port (e.g. mbedtls)
 */
struct altcp_tls_config;

/** @ingroup altcp_tls
 * Create an ALTCP_TLS server configuration handle
 */
struct altcp_tls_config* altcp_tls_create_config_server_privkey_cert(const u8_t* privkey, size_t privkey_len,
        const u8_t* privkey_pass, size_t privkey_pass_len,
        const u8_t* cert, size_t cert_len);

/** @ingroup altcp_tls
 * Create an ALTCP_TLS client configuration handle
 */
struct altcp_tls_config* altcp_tls_create_config_client(const u8_t* cert, size_t cert_len);

/** @ingroup altcp_tls
 * Create an ALTCP_TLS client configuration handle with two-way server/client authentication
 */
struct altcp_tls_config* altcp_tls_create_config_client_2wayauth(const u8_t* ca, size_t ca_len, const u8_t* privkey, size_t privkey_len,
        const u8_t* privkey_pass, size_t privkey_pass_len,
        const u8_t* cert, size_t cert_len);

/** @ingroup altcp_tls
 * Free an ALTCP_TLS configuration handle
 */
void altcp_tls_free_config(struct altcp_tls_config* conf);

/** @ingroup altcp_tls
 * Create new ALTCP_TLS layer wrapping an existing pcb as inner connection (e.g. TLS over TCP)
 */
struct altcp_pcb* altcp_tls_wrap(struct altcp_tls_config* config, struct altcp_pcb* inner_pcb);

/** @ingroup altcp_tls
 * Create new ALTCP_TLS pcb and its inner tcp pcb
 */
struct altcp_pcb* altcp_tls_new(struct altcp_tls_config* config, u8_t ip_type);

/** @ingroup altcp_tls
 * Create new ALTCP_TLS layer pcb and its inner tcp pcb.
 * Same as @ref altcp_tls_new but this allocator function fits to
 * @ref altcp_allocator_t / @ref altcp_new.\n
 'arg' must contain a struct altcp_tls_config *.
 */
struct altcp_pcb* altcp_tls_alloc(void* arg, u8_t ip_type);

/** @ingroup altcp_tls
 * Return pointer to internal TLS context so application can tweak it.
 * Real type depends on port (e.g. mbedtls)
 */
void* altcp_tls_context(struct altcp_pcb* conn);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_ALTCP_TLS */
#endif /* LWIP_ALTCP */
#endif /* LWIP_HDR_ALTCP_TLS_H */
