/**
 * @file
 * Application layered TCP/TLS connection API (to be used from TCPIP thread)
 *
 * This file contains options for an mbedtls port of the TLS layer.
 */

#ifndef LWIP_HDR_ALTCP_TLS_OPTS_H
#define LWIP_HDR_ALTCP_TLS_OPTS_H

#include "lwip/opt.h"

#if LWIP_ALTCP /* don't build if not configured for use in lwipopts.h */

/** LWIP_ALTCP_TLS_MBEDTLS==1: use mbedTLS for TLS support for altcp API
 * mbedtls include directory must be reachable via include search path
 */
#ifndef LWIP_ALTCP_TLS_MBEDTLS
#define LWIP_ALTCP_TLS_MBEDTLS                        0
#endif

/** Configure debug level of this file */
#ifndef ALTCP_MBEDTLS_DEBUG
#define ALTCP_MBEDTLS_DEBUG                           LWIP_DBG_OFF
#endif

/** Set a session timeout in seconds for the basic session cache
 * ATTENTION: Using a session cache can lower security by reusing keys!
 */
#ifndef ALTCP_MBEDTLS_SESSION_CACHE_TIMEOUT_SECONDS
#define ALTCP_MBEDTLS_SESSION_CACHE_TIMEOUT_SECONDS   0
#endif

#endif /* LWIP_ALTCP */

#endif /* LWIP_HDR_ALTCP_TLS_OPTS_H */
