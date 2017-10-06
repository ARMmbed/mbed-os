/*************************************************************************************************/
/*!
 *  \file   wsf_types.h
 *        
 *  \brief  Platform-independent data types.
 *
 *          $Date: 2012-04-28 22:02:14 -0700 (Sat, 28 Apr 2012) $
 *          $Revision: 306 $
 *  
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
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
#if ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) && \
     (__ICC8051__ == 0)) || defined(__CC_ARM) || \
     (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || \
     defined(__IAR_SYSTEMS_ICC__)
#include <stdint.h>
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
#endif

/* Boolean data type */
typedef uint8_t bool_t;

#endif /* WSF_TYPES_H */
