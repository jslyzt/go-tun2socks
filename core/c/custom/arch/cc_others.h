#ifndef LWIP_ARCH_CC_H
#define LWIP_ARCH_CC_H

/* Include some files for defining library routines */
#include <sys/time.h>

#define LWIP_TIMEVAL_PRIVATE 0

/* Define platform endianness */
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif /* BYTE_ORDER */

/* Define generic types used in lwIP */
/*
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int     u32_t;
typedef signed     int     s32_t;

typedef unsigned long mem_ptr_t;
*/

/* Define (sn)printf formatters for these lwIP types */
#define X8_F  "02x"
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"

/* If only we could use C99 and get %zu */
#if defined(__x86_64__)
#define SZT_F "lu"
#else
#define SZT_F "u"
#endif

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

/* prototypes for printf() and abort() */
#include <stdio.h>
#include <stdlib.h>
/* Plaform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(x)	do {printf x;} while(0)

#ifdef LWIP_UNIX_EMPTY_ASSERT
#define LWIP_PLATFORM_ASSERT(x)
#else
#define LWIP_PLATFORM_ASSERT(x) do {printf("Assertion \"%s\" failed at line %d in %s\n", \
x, __LINE__, __FILE__); fflush(NULL); abort();} while(0)
#endif

#define LWIP_RAND() ((u32_t)rand())

struct sio_status_s;
typedef struct sio_status_s sio_status_t;
#define sio_fd_t sio_status_t*
#define __sio_fd_t_defined

#endif /* LWIP_ARCH_CC_H */
