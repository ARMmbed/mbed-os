/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __WLAN_BASIC_TYPES_H__
#define __WLAN_BASIC_TYPES_H__


/* ================================================
  * Sections (1) rtl8195a and (2) other MCU based wlan driver
  * For 8195a, some of the definitions are already defined in system wise "basic_types.h"
  *================================================ */
#define _SUCCESS		1
#define _PASS		1
#define _FAIL		0

//ERRNO Define
#define  EPERM     1  /* Operation not permitted */
#define  ENOENT     2  /* No such file or directory */
#define  ESRCH     3  /* No such process */
#define  EINTR     4  /* Interrupted system call */
#define  EIO     5  /* I/O error */
#define  ENXIO     6  /* No such device or address */
#define  E2BIG     7  /* Arg list too long */
#define  ENOEXEC     8  /* Exec format error */
#define  EBADF     9  /* Bad file number */
#define  ECHILD    10  /* No child processes */
#define  EAGAIN    11  /* Try again */
#define  ENOMEM    12  /* Out of memory */
#define  EACCES    13  /* Permission denied */
#define  EFAULT    14  /* Bad address */
#define  ENOTBLK    15  /* Block device required */
#define  EBUSY    16  /* Device or resource busy */
#define  EEXIST    17  /* File exists */
#define  EXDEV    18  /* Cross-device link */
#define  ENODEV    19  /* No such device */
#define  ENOTDIR    20  /* Not a directory */
#define  EISDIR    21  /* Is a directory */
#define  EINVAL    22  /* Invalid argument */
#define  ENFILE    23  /* File table overflow */
#define  EMFILE    24  /* Too many open files */
#define  ENOTTY    25  /* Not a typewriter */
#define  ETXTBSY    26  /* Text file busy */
#define  EFBIG    27  /* File too large */
#define  ENOSPC    28  /* No space left on device */
#define  ESPIPE    29  /* Illegal seek */
#define  EROFS    30  /* Read-only file system */
#define  EMLINK    31  /* Too many links */
#define  EPIPE    32  /* Broken pipe */
#define  EDOM    33  /* Math argument out of domain of func */
#define  ERANGE    34  /* Math result not representable */
#define  EDEADLK    35  /* Resource deadlock would occur */
#define  ENAMETOOLONG  36  /* File name too long */
#define  ENOLCK    37  /* No record locks available */
#define  ENOSYS    38  /* Function not implemented */
#define  ENOTEMPTY  39  /* Directory not empty */
#define  ELOOP    40  /* Too many symbolic links encountered */
#define  EWOULDBLOCK  EAGAIN  /* Operation would block */
#define  ENOMSG    42  /* No message of desired type */
#define  EIDRM    43  /* Identifier removed */
#define  ECHRNG    44  /* Channel number out of range */
#define  EL2NSYNC  45  /* Level 2 not synchronized */
#define  EL3HLT    46  /* Level 3 halted */
#define  EL3RST    47  /* Level 3 reset */
#define  ELNRNG    48  /* Link number out of range */
#define  EUNATCH    49  /* Protocol driver not attached */
#define  ENOCSI    50  /* No CSI structure available */
#define  EL2HLT    51  /* Level 2 halted */
#define  EBADE    52  /* Invalid exchange */
#define  EBADR    53  /* Invalid request descriptor */
#define  EXFULL    54  /* Exchange full */
#define  ENOANO    55  /* No anode */
#define  EBADRQC    56  /* Invalid request code */
#define  EBADSLT    57  /* Invalid slot */

#define  EDEADLOCK  EDEADLK

#define  EBFONT    59  /* Bad font file format */
#define  ENOSTR    60  /* Device not a stream */
#define  ENODATA    61  /* No data available */
#define  ETIME    62  /* Timer expired */
#define  ENOSR    63  /* Out of streams resources */
#define  ENONET    64  /* Machine is not on the network */
#define  ENOPKG    65  /* Package not installed */
#define  EREMOTE    66  /* Object is remote */
#define  ENOLINK    67  /* Link has been severed */
#define  EADV    68  /* Advertise error */
#define  ESRMNT    69  /* Srmount error */
#define  ECOMM    70  /* Communication error on send */
#define  EPROTO    71  /* Protocol error */
#define  EMULTIHOP  72  /* Multihop attempted */
#define  EDOTDOT    73  /* RFS specific error */
#define  EBADMSG    74  /* Not a data message */
#define  EOVERFLOW  75  /* Value too large for defined data type */
#define  ENOTUNIQ  76  /* Name not unique on network */
#define  EBADFD    77  /* File descriptor in bad state */
#define  EREMCHG    78  /* Remote address changed */
#define  ELIBACC    79  /* Can not access a needed shared library */
#define  ELIBBAD    80  /* Accessing a corrupted shared library */
#define  ELIBSCN    81  /* .lib section in a.out corrupted */
#define  ELIBMAX    82  /* Attempting to link in too many shared libraries */
#define  ELIBEXEC  83  /* Cannot exec a shared library directly */
#define  EILSEQ    84  /* Illegal byte sequence */
#define  ERESTART  85  /* Interrupted system call should be restarted */
#define  ESTRPIPE  86  /* Streams pipe error */
#define  EUSERS    87  /* Too many users */
#define  ENOTSOCK  88  /* Socket operation on non-socket */
#define  EDESTADDRREQ  89  /* Destination address required */
#define  EMSGSIZE  90  /* Message too long */
#define  EPROTOTYPE  91  /* Protocol wrong type for socket */
#define  ENOPROTOOPT  92  /* Protocol not available */
#define  EPROTONOSUPPORT  93  /* Protocol not supported */
#define  ESOCKTNOSUPPORT  94  /* Socket type not supported */
#define  EOPNOTSUPP  95  /* Operation not supported on transport endpoint */
#define  EPFNOSUPPORT  96  /* Protocol family not supported */
#define  EAFNOSUPPORT  97  /* Address family not supported by protocol */
#define  EADDRINUSE  98  /* Address already in use */
#define  EADDRNOTAVAIL  99  /* Cannot assign requested address */
#define  ENETDOWN  100  /* Network is down */
#define  ENETUNREACH  101  /* Network is unreachable */
#define  ENETRESET  102  /* Network dropped connection because of reset */
#define  ECONNABORTED  103  /* Software caused connection abort */
#define  ECONNRESET  104  /* Connection reset by peer */
#define  ENOBUFS    105  /* No buffer space available */
#define  EISCONN    106  /* Transport endpoint is already connected */
#define  ENOTCONN  107  /* Transport endpoint is not connected */
#define  ESHUTDOWN  108  /* Cannot send after transport endpoint shutdown */
#define  ETOOMANYREFS  109  /* Too many references: cannot splice */
#define  ETIMEDOUT  110  /* Connection timed out */
#define  ECONNREFUSED  111  /* Connection refused */
#define  EHOSTDOWN  112  /* Host is down */
#define  EHOSTUNREACH  113  /* No route to host */
#define  EALREADY  114  /* Operation already in progress */
#define  EINPROGRESS  115  /* Operation now in progress */
#define  ESTALE    116  /* Stale NFS file handle */
#define  EUCLEAN    117  /* Structure needs cleaning */
#define  ENOTNAM    118  /* Not a XENIX named type file */
#define  ENAVAIL    119  /* No XENIX semaphores available */
#define  EISNAM    120  /* Is a named type file */
#define  EREMOTEIO  121  /* Remote I/O error */
#define  EDQUOT    122  /* Quota exceeded */

#define  ENOMEDIUM  123  /* No medium found */
#define  EMEDIUMTYPE  124  /* Wrong medium type */


#define ENSROK    0 /* DNS server returned answer with no data */
#define ENSRNODATA  160 /* DNS server returned answer with no data */
#define ENSRFORMERR 161 /* DNS server claims query was misformatted */
#define ENSRSERVFAIL 162  /* DNS server returned general failure */
#define ENSRNOTFOUND 163  /* Domain name not found */
#define ENSRNOTIMP  164 /* DNS server does not implement requested operation */
#define ENSRREFUSED 165 /* DNS server refused query */
#define ENSRBADQUERY 166  /* Misformatted DNS query */
#define ENSRBADNAME 167 /* Misformatted domain name */
#define ENSRBADFAMILY 168 /* Unsupported address family */
#define ENSRBADRESP 169 /* Misformatted DNS reply */
#define ENSRCONNREFUSED 170 /* Could not contact DNS servers */
#define ENSRTIMEOUT 171 /* Timeout while contacting DNS servers */
#define ENSROF    172 /* End of file */
#define ENSRFILE  173 /* Error reading file */
#define ENSRNOMEM 174 /* Out of memory */
#define ENSRDESTRUCTION 175 /* Application terminated lookup */
#define ENSRQUERYDOMAINTOOLONG  176 /* Domain name is too long */
#define ENSRCNAMELOOP 177 /* Domain name is too long */







/* ================================================
  * Sections only for other MCU based wlan driver
  *========================================== ======*/
 #if !defined(CONFIG_PLATFORM_8195A) && !defined(CONFIG_PLATFORM_8711B)
#include <drv_conf.h>

#define SUCCESS	0
#define FAIL	(-1)

#ifndef TRUE
	#define _TRUE	1
#else
	#define _TRUE	TRUE	
#endif
		
#ifndef FALSE		
	#define _FALSE	0
#else
	#define _FALSE	FALSE	
#endif

//
// pack & weak attribute
//
#if defined (__ICCARM__)

#define RTW_PACK_STRUCT_BEGIN
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END
#define RTW_PACK_STRUCT_USE_INCLUDES

#define RTW_WEAK					__weak

#elif defined (__CC_ARM)

#define RTW_PACK_STRUCT_BEGIN	__packed
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END

#define RTW_WEAK					 __weak 

#elif defined (__GNUC__)

#define RTW_PACK_STRUCT_BEGIN
#define RTW_PACK_STRUCT_STRUCT 	__attribute__ ((__packed__))
#define RTW_PACK_STRUCT_END

#define RTW_WEAK					__attribute__ ((weak))

#elif defined(PLATFORM_WINDOWS)

#define RTW_PACK_STRUCT_BEGIN
#define RTW_PACK_STRUCT_STRUCT 
#define RTW_PACK_STRUCT_END
#define RTW_PACK_STRUCT_USE_INCLUDES
#endif

#ifndef BIT
	#define BIT(x)	((u32)1 << (x))
#endif

#if !defined(CONFIG_PLATFORM_8195A) && !defined(CONFIG_PLATFORM_8711B)
#define BIT0	0x00000001
#define BIT1	0x00000002
#define BIT2	0x00000004
#define BIT3	0x00000008
#define BIT4	0x00000010
#define BIT5	0x00000020
#define BIT6	0x00000040
#define BIT7	0x00000080
#define BIT8	0x00000100
#define BIT9	0x00000200
#define BIT10	0x00000400
#define BIT11	0x00000800
#define BIT12	0x00001000
#define BIT13	0x00002000
#define BIT14	0x00004000
#define BIT15	0x00008000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000
#define BIT32	0x0100000000
#define BIT33	0x0200000000
#define BIT34	0x0400000000
#define BIT35	0x0800000000
#define BIT36	0x1000000000
#endif


#ifdef PLATFORM_ECOS

	#define IN
	#define OUT
	#define VOID void
	#define NDIS_OID uint
	#define NDIS_STATUS uint

	typedef unsigned int		uint;
	typedef	signed int		sint;

	#ifndef	PVOID
	typedef void *			PVOID;
	#endif


	typedef unsigned int		__kernel_size_t;
	typedef int			__kernel_ssize_t;

	typedef	__kernel_size_t		SIZE_T;	
	typedef	__kernel_ssize_t	SSIZE_T;
	#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)

#endif


#ifdef PLATFORM_WINDOWS

	typedef signed char s8;
	typedef unsigned char u8;

	typedef signed short s16;
	typedef unsigned short u16;

	typedef signed long s32;
	typedef unsigned long u32;
	
	typedef unsigned int	uint;
	typedef	signed int		sint;


	typedef signed long long s64;
	typedef unsigned long long u64;

	#ifdef NDIS50_MINIPORT
	
		#define NDIS_MAJOR_VERSION       5
		#define NDIS_MINOR_VERSION       0

	#endif

	#ifdef NDIS51_MINIPORT

		#define NDIS_MAJOR_VERSION       5
		#define NDIS_MINOR_VERSION       1

	#endif

	typedef NDIS_PROC proc_t;

	typedef LONG atomic_t;

#endif


#ifdef PLATFORM_LINUX

	#include <linux/types.h>
	#define IN
	#define OUT
	#define VOID void
	#define NDIS_OID uint
	#define NDIS_STATUS uint

	typedef	signed int sint;

	#ifndef	PVOID
	typedef void * PVOID;
	//#define PVOID	(void *)
	#endif

	typedef void (*proc_t)(void*);

	typedef 	__kernel_size_t	SIZE_T;	
	typedef	__kernel_ssize_t	SSIZE_T;
	#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
	
#endif


#ifdef PLATFORM_FREEBSD
	typedef signed char s8;
	typedef unsigned char u8;

	typedef signed short s16;
	typedef unsigned short u16;

	typedef signed int s32;
	typedef unsigned int u32;
	
	typedef unsigned int	uint;
	typedef	signed int		sint;
	typedef long atomic_t;

	typedef signed long long s64;
	typedef unsigned long long u64;
	#define IN
	#define OUT
	#define VOID void
	#define NDIS_OID uint
	#define NDIS_STATUS uint
	
	#ifndef	PVOID
	typedef void * PVOID;
	//#define PVOID	(void *)
	#endif
	typedef u32 dma_addr_t;

	typedef void (*proc_t)(void*);
  
  typedef unsigned int __kernel_size_t;
  typedef int __kernel_ssize_t;
  
	typedef 	__kernel_size_t	SIZE_T;	
	typedef	__kernel_ssize_t	SSIZE_T;
	#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
	
#endif


#define MEM_ALIGNMENT_OFFSET	(sizeof (SIZE_T))
#define MEM_ALIGNMENT_PADDING	(sizeof(SIZE_T) - 1)

#define SIZE_PTR SIZE_T
#define SSIZE_PTR SSIZE_T

//port from fw by thomas
// TODO: Belows are Sync from SD7-Driver. It is necessary to check correctness

/*
 *	Call endian free function when
 *		1. Read/write packet content.
 *		2. Before write integer to IO.
 *		3. After read integer from IO.
*/

//
// Byte Swapping routine.
//
#define EF1Byte	
#define EF2Byte 	le16_to_cpu
#define EF4Byte	le32_to_cpu

//
// Read LE format data from memory
//
#define ReadEF1Byte(_ptr)		EF1Byte(*((u8 *)(_ptr)))
#define ReadEF2Byte(_ptr)		EF2Byte(*((u16 *)(_ptr)))
#define ReadEF4Byte(_ptr)		EF4Byte(*((u32 *)(_ptr)))

//
// Write LE data to memory
//
#define WriteEF1Byte(_ptr, _val)	(*((u8 *)(_ptr)))=EF1Byte(_val)
#define WriteEF2Byte(_ptr, _val)	(*((u16 *)(_ptr)))=EF2Byte(_val)
#define WriteEF4Byte(_ptr, _val)	(*((u32 *)(_ptr)))=EF4Byte(_val)									

//
//	Example:
//		BIT_LEN_MASK_32(0) => 0x00000000
//		BIT_LEN_MASK_32(1) => 0x00000001
//		BIT_LEN_MASK_32(2) => 0x00000003
//		BIT_LEN_MASK_32(32) => 0xFFFFFFFF
//
#define BIT_LEN_MASK_32(__BitLen) \
	(0xFFFFFFFF >> (32 - (__BitLen)))
//
//	Example:
//		BIT_OFFSET_LEN_MASK_32(0, 2) => 0x00000003
//		BIT_OFFSET_LEN_MASK_32(16, 2) => 0x00030000
//
#define BIT_OFFSET_LEN_MASK_32(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_32(__BitLen) << (__BitOffset)) 

//
//	Description:
//		Return 4-byte value in host byte ordering from
//		4-byte pointer in litten-endian system.
//
#define LE_P4BYTE_TO_HOST_4BYTE(__pStart) \
	(EF4Byte(*((u32 *)(__pStart))))

//
//	Description:
//		Translate subfield (continuous bits in little-endian) of 4-byte value in litten byte to
//		4-byte value in host byte ordering.
//
#define LE_BITS_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P4BYTE_TO_HOST_4BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_32(__BitLen) \
	)

//
//	Description:
//		Mask subfield (continuous bits in little-endian) of 4-byte value in litten byte oredering  
//		and return the result in 4-byte value in host byte ordering.
//
#define LE_BITS_CLEARED_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P4BYTE_TO_HOST_4BYTE(__pStart) \
		& \
		( ~BIT_OFFSET_LEN_MASK_32(__BitOffset, __BitLen) ) \
	)

//
//	Description:
//		Set subfield of little-endian 4-byte value to specified value.	
//
#define SET_BITS_TO_LE_4BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u32 *)(__pStart)) = \
		EF4Byte( \
			LE_BITS_CLEARED_TO_4BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u32)__Value) & BIT_LEN_MASK_32(__BitLen)) << (__BitOffset) ) \
		);

		
#define BIT_LEN_MASK_16(__BitLen) \
		(0xFFFF >> (16 - (__BitLen)))
		
#define BIT_OFFSET_LEN_MASK_16(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_16(__BitLen) << (__BitOffset))
	
#define LE_P2BYTE_TO_HOST_2BYTE(__pStart) \
	(EF2Byte(*((u16 *)(__pStart))))
	
#define LE_BITS_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P2BYTE_TO_HOST_2BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_16(__BitLen) \
	)
	
#define LE_BITS_CLEARED_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P2BYTE_TO_HOST_2BYTE(__pStart) \
		& \
		( ~BIT_OFFSET_LEN_MASK_16(__BitOffset, __BitLen) ) \
	)

#define SET_BITS_TO_LE_2BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u16 *)(__pStart)) = \
		EF2Byte( \
			LE_BITS_CLEARED_TO_2BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u16)__Value) & BIT_LEN_MASK_16(__BitLen)) << (__BitOffset) ) \
		);
			
#define BIT_LEN_MASK_8(__BitLen) \
		(0xFF >> (8 - (__BitLen)))

#define BIT_OFFSET_LEN_MASK_8(__BitOffset, __BitLen) \
	(BIT_LEN_MASK_8(__BitLen) << (__BitOffset))

#define LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
	(EF1Byte(*((u8 *)(__pStart))))

#define LE_BITS_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		( LE_P1BYTE_TO_HOST_1BYTE(__pStart) >> (__BitOffset) ) \
		& \
		BIT_LEN_MASK_8(__BitLen) \
	)

#define LE_BITS_CLEARED_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
		& \
		( ~BIT_OFFSET_LEN_MASK_8(__BitOffset, __BitLen) ) \
	)

#define SET_BITS_TO_LE_1BYTE(__pStart, __BitOffset, __BitLen, __Value) \
	*((u8 *)(__pStart)) = \
		EF1Byte( \
			LE_BITS_CLEARED_TO_1BYTE(__pStart, __BitOffset, __BitLen) \
			| \
			( (((u8)__Value) & BIT_LEN_MASK_8(__BitLen)) << (__BitOffset) ) \
		);

//pclint
#define LE_BITS_CLEARED_TO_1BYTE_8BIT(__pStart, __BitOffset, __BitLen) \
	( \
		LE_P1BYTE_TO_HOST_1BYTE(__pStart) \
	)

//pclint
#define SET_BITS_TO_LE_1BYTE_8BIT(__pStart, __BitOffset, __BitLen, __Value) \
{ \
	*((u8*)(__pStart)) = \
		EF1Byte( \
			LE_BITS_CLEARED_TO_1BYTE_8BIT(__pStart, __BitOffset, __BitLen) \
			| \
			((u8)__Value) \
		); \
}

// Get the N-bytes aligment offset from the current length
#define N_BYTE_ALIGMENT(__Value, __Aligment) ((__Aligment == 1) ? (__Value) : (((__Value + __Aligment - 1) / __Aligment) * __Aligment))

typedef unsigned char	BOOLEAN,*PBOOLEAN;

#define TEST_FLAG(__Flag,__testFlag)		(((__Flag) & (__testFlag)) != 0)



#endif//! defined(CONFIG_PLATFORM_8195A)

#endif //__WLAN_BASIC_TYPES_H__

