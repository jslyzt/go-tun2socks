#ifndef LWIP_ARCH_CC_H
#define LWIP_ARCH_CC_H

#ifdef _MSC_VER
#pragma warning (disable: 4127) /* conditional expression is constant */
#pragma warning (disable: 4996) /* 'strncpy' was declared deprecated */
#pragma warning (disable: 4103) /* structure packing changed by including file */
#pragma warning (disable: 4820) /* 'x' bytes padding added after data member 'y' */
#pragma warning (disable: 4711) /* The compiler performed inlining on the given function, although it was not marked for inlining */
#endif

#define LWIP_PROVIDE_ERRNO

/* Define platform endianness (might already be defined) */
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif /* BYTE_ORDER */

typedef int sys_prot_t;

#ifdef _MSC_VER
/* define _INTPTR for Win32 MSVC stdint.h */
#define _INTPTR 2

/* Do not use lwIP default definitions for format strings
 * because these do not work with MSVC 2010 compiler (no inttypes.h)
 */
#define LWIP_NO_INTTYPES_H 1

/* Define (sn)printf formatters for these lwIP types */
#define X8_F  "02x"
#define U16_F "hu"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"

#define S16_F "hd"
#define X16_F "hx"
#define SZT_F "lu"
#endif /* _MSC_VER */

/* Compiler hints for packing structures */
#define PACK_STRUCT_USE_INCLUDES

#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
  printf("Assertion \"%s\" failed at line %d in %s\n", message, __LINE__, __FILE__); \
  fflush(NULL);handler;} } while(0)

#ifdef _MSC_VER
/* C runtime functions redefined */
#define snprintf _snprintf
#define strdup   _strdup
#endif

/* Define an example for LWIP_PLATFORM_DIAG: since this uses varargs and the old
 * C standard lwIP targets does not support this in macros, we have extra brackets
 * around the arguments, which are left out in the following macro definition:
 */
#if !defined(LWIP_TESTMODE) || !LWIP_TESTMODE
void lwip_win32_platform_diag(const char* format, ...);
#define LWIP_PLATFORM_DIAG(x) lwip_win32_platform_diag x
#endif

#ifndef LWIP_NORAND
extern unsigned int sys_win_rand(void);
#define LWIP_RAND() (sys_win_rand())
#endif

#define PPP_INCLUDE_SETTINGS_HEADER

#endif /* LWIP_ARCH_CC_H */
