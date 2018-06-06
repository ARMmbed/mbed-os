/*************************************************************************************************/
/*!
 *  \file   wsf_types.h
 *
 *  \brief  Platform-independent data types.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef WSF_TYPES_H
#define WSF_TYPES_H

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Integer data types */
#if ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) && \
     (!defined(__ICC8051__) || (__ICC8051__ == 0))) || \
     defined(__CC_ARM) || defined(__IAR_SYSTEMS_ICC__)
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

#include <rtx_core_cm.h>

#endif /* WSF_TYPES_H */
