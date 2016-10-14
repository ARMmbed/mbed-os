/*************************************************************************************************/
/*!
 *  \file   wsf_types.h
 *
 *  \brief  Platform-independent data types.
 *
 *          $Date: 2015-05-14 14:58:23 -0700 (Thu, 14 May 2015) $
 *          $Revision: 2837 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_TYPES_H
#define WSF_TYPES_H

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Integer data types */
#if (defined(__IAR_SYSTEMS_ICC__) && (__VER__ >= 7070001)) || \
    (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 5060000L)) || \
    ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) && \
    (!defined(__ICC8051__) || (__ICC8051__ == 0)))
#include <stdint.h>
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#endif

/* Boolean data type */
typedef uint8_t bool_t;

#endif /* WSF_TYPES_H */
