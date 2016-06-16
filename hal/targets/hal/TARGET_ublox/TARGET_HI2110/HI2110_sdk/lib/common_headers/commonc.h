/******************************************************************************
 * @brief    This should be #included by every C source file neul protocol development
             board system(s).
			 The file includes typedefs and #defines that are common to many
			 C projects.
			 This file should be changed only infrequently and with great care.
			 This file should contain the minimum necessary to get the job done.
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/

#ifndef COMMONC_H
#define COMMONC_H

#include <stddef.h>

/* If DO_NOT_PACK_STRUCS is defined then strucs are not packed
   Typically this makes sense only when building on a PC. */
#ifdef NEUL_DO_NOT_PACK_STRUCTS
#define NPACKED_STRUCT
#define VAR_ALIGN(x)
#define VAR_SEGMENT(x)
#endif

/* If some of the defines we use for locating variable are not defined then we had better define them */
#ifndef NPACKED_STRUCT
#ifdef __GNUC__
#define NPACKED_STRUCT __attribute((packed))
#else
#error "You need to define NPACKED_STRUCT for your compiler."
#endif
#endif


#ifndef VAR_ALIGN
#ifdef __GNUC__
#define VAR_ALIGN(x) __attribute__ ((aligned (x)))
#else
#error "You need to define VAR_ALIGN for your compiler."
#endif
#endif


#ifndef VAR_SEGMENT
#ifdef __GNUC__
#define VAR_SEGMENT(seg) __attribute__ ((section (seg)))
#else
#error "You need to define VAR_SEGMENT for your compiler."
#endif
#endif

/* Macro to mark specific variables as preserved across a reboot, i.e. are not reset during C runtime setup. */
#ifndef PRESERVE
#define PRESERVE VAR_SEGMENT("preserve")
#endif


#define UNUSED(var) do { (void)var; } while(0)

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#undef  forever
#define forever         for(;;)

#define tobool(v)       ((v) ? true : false)

#define MAKEUINT32(lo, hi)  ((uint32)(((lo) & 0xffff) | (((hi) & 0xffff) << 16)))
#define MAKEUINT16(lo, hi)  ((uint16)(((lo) & 0xff) | (((hi) & 0xff) << 8)))
#define HIUINT16(val)       ((uint16)(((val) >> 16) & 0xffff))
#define LOUINT16(val)       ((uint16)(((val) >>  0) & 0xffff))
#define HIUINT8(val)        ((uint8)(((val) >> 8) & 0xff))
#define LOUINT8(val)        ((uint8)(((val) >> 0) & 0xff))


#define PREPROCESSOR_ARGUMENT_STRING(x)                 #x
#define PREPROCESSOR_ARGUMENT_EVALUATED_STRING(x)       PREPROCESSOR_ARGUMENT_STRING(x)

#endif  /* COMMONC_H */
