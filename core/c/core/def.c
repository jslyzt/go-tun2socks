/**
 * @file
 * Common functions used throughout the stack.
 *
 * These are reference implementations of the byte swapping functions.
 * Again with the aim of being simple, correct and fully portable.
 * Byte swapping is the second thing you would want to optimize. You will
 * need to port it to your architecture and in your cc.h:
 *
 * \#define lwip_htons(x) your_htons
 * \#define lwip_htonl(x) your_htonl
 *
 * Note lwip_ntohs() and lwip_ntohl() are merely references to the htonx counterparts.
 *
 * If you \#define them to htons() and htonl(), you should
 * \#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS to prevent lwIP from
 * defining htonx/ntohx compatibility macros.

 * @defgroup sys_nonstandard Non-standard functions
 * @ingroup sys_layer
 * lwIP provides default implementations for non-standard functions.
 * These can be mapped to OS functions to reduce code footprint if desired.
 * All defines related to this section must not be placed in lwipopts.h,
 * but in arch/cc.h!
 * These options cannot be \#defined in lwipopts.h since they are not options
 * of lwIP itself, but options of the lwIP port to your system.
 */

#include "lwip/opt.h"
#include "lwip/def.h"

#include <string.h>

#if BYTE_ORDER == LITTLE_ENDIAN

#if !defined(lwip_htons)
/**
 * Convert an u16_t from host- to network byte order.
 *
 * @param n u16_t in host byte order
 * @return n in network byte order
 */
u16_t lwip_htons(u16_t n) {
    return PP_HTONS(n);
}
#endif /* lwip_htons */

#if !defined(lwip_htonl)
/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
u32_t lwip_htonl(u32_t n) {
    return PP_HTONL(n);
}
#endif /* lwip_htonl */

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#ifndef lwip_strnstr
/**
 * @ingroup sys_nonstandard
 * lwIP default implementation for strnstr() non-standard function.
 * This can be \#defined to strnstr() depending on your platform port.
 */
char* lwip_strnstr(const char* buffer, const char* token, size_t n) {
    const char* p;
    size_t tokenlen = strlen(token);
    if (tokenlen == 0) {
        return LWIP_CONST_CAST(char*, buffer);
    }
    for (p = buffer; *p && (p + tokenlen <= buffer + n); p++) {
        if ((*p == *token) && (strncmp(p, token, tokenlen) == 0)) {
            return LWIP_CONST_CAST(char*, p);
        }
    }
    return NULL;
}
#endif

#ifndef lwip_stricmp
/**
 * @ingroup sys_nonstandard
 * lwIP default implementation for stricmp() non-standard function.
 * This can be \#defined to stricmp() depending on your platform port.
 */
int lwip_stricmp(const char* str1, const char* str2) {
    char c1, c2;

    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) {
            char c1_upc = c1 | 0x20;
            if ((c1_upc >= 'a') && (c1_upc <= 'z')) {
                /* characters are not equal an one is in the alphabet range:
                downcase both chars and check again */
                char c2_upc = c2 | 0x20;
                if (c1_upc != c2_upc) {
                    /* still not equal */
                    /* don't care for < or > */
                    return 1;
                }
            } else {
                /* characters are not equal but none is in the alphabet range */
                return 1;
            }
        }
    } while (c1 != 0);
    return 0;
}
#endif

#ifndef lwip_strnicmp
/**
 * @ingroup sys_nonstandard
 * lwIP default implementation for strnicmp() non-standard function.
 * This can be \#defined to strnicmp() depending on your platform port.
 */
int lwip_strnicmp(const char* str1, const char* str2, size_t len) {
    char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) {
            char c1_upc = c1 | 0x20;
            if ((c1_upc >= 'a') && (c1_upc <= 'z')) {
                /* characters are not equal an one is in the alphabet range:
                downcase both chars and check again */
                char c2_upc = c2 | 0x20;
                if (c1_upc != c2_upc) {
                    /* still not equal */
                    /* don't care for < or > */
                    return 1;
                }
            } else {
                /* characters are not equal but none is in the alphabet range */
                return 1;
            }
        }
        len--;
    } while ((len != 0) && (c1 != 0));
    return 0;
}
#endif

#ifndef lwip_itoa
/**
 * @ingroup sys_nonstandard
 * lwIP default implementation for itoa() non-standard function.
 * This can be \#defined to itoa() or snprintf(result, bufsize, "%d", number) depending on your platform port.
 */
void lwip_itoa(char* result, size_t bufsize, int number) {
    char* res = result;
    char* tmp = result + bufsize - 1;
    int n = (number >= 0) ? number : -number;

    /* handle invalid bufsize */
    if (bufsize < 2) {
        if (bufsize == 1) {
            *result = 0;
        }
        return;
    }

    /* First, add sign */
    if (number < 0) {
        *res++ = '-';
    }
    /* Then create the string from the end and stop if buffer full,
       and ensure output string is zero terminated */
    *tmp = 0;
    while ((n != 0) && (tmp > res)) {
        char val = (char)('0' + (n % 10));
        tmp--;
        *tmp = val;
        n = n / 10;
    }
    if (n) {
        /* buffer is too small */
        *result = 0;
        return;
    }
    if (*tmp == 0) {
        /* Nothing added? */
        *res++ = '0';
        *res++ = 0;
        return;
    }
    /* move from temporary buffer to output buffer (sign is not moved) */
    memmove(res, tmp, (size_t)((result + bufsize) - tmp));
}
#endif
