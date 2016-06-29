/*************************************************************************************************/
/*!
 *  \file   wsf_math.h
 *
 *  \brief  Common math utilities.
 *
 *          $Date: 2015-08-27 08:31:35 -0400 (Thu, 27 Aug 2015) $
 *          $Revision: 3731 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/
#ifndef WSF_MATH_H
#define WSF_MATH_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Returns the minimum of two values. */
#define WSF_MIN(a,b)        ((a) < (b) ? (a) : (b))

/*! \brief      Returns the maximum of two values. */
#define WSF_MAX(a,b)        ((a) > (b) ? (a) : (b))

/*! \brief      Binary divide with 1,000 divisor. */
#define WSF_MATH_DIV_10E3(n)    (((n) * UINT32_C(1048)) >> 20)

/*! \brief      Binary divide with 1,000,000 divisor. */
#define WSF_MATH_DIV_10E6(n)    ((uint32_t)(((uint64_t)(n) * UINT64_C(4295)) >> 32))

/*! \brief      Binary divide with 10 divisor. */
#define WSF_MATH_DIV_10(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(419431)) >> 22))

/*! \brief      Binary divide with 37 divisor. */
#define WSF_MATH_DIV_37(n)      (((n) * UINT32_C(56680)) >> 21)

/*! \brief      Binary modulo 37. */
#define WSF_MATH_MOD_37(n)      ((n) - (WSF_MATH_DIV_37(n) * 37))

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void WsfMathInit(void);
uint32_t WsfRandNum(void);
void WsfAesEcb(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn);

#ifdef __cplusplus
};
#endif

#endif /* WSF_MATH_H */
