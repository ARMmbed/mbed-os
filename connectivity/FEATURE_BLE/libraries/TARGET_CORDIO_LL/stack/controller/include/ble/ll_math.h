/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link Layer math utilities.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef LL_MATH_H
#define LL_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsf_types.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Binary divide with 1,000 divisor (n[max]=7,999). */
#define LL_MATH_DIV_10E3(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(536871)) >> 29))

/*! \brief      Binary divide with 1,000,000 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_10E6(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(4295)) >> 32))

/*! \brief      Binary divide with 10 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_10(n)       ((uint32_t)(((uint64_t)(n) * UINT64_C(419431)) >> 22))

/*! \brief      Binary divide with 27 divisor (n[max]=55,295). */
#define LL_MATH_DIV_27(n)       (((n) * UINT32_C(77673)) >> 21)

/*! \brief      Binary divide with 37 divisor (n[max]=75,776). */
#define LL_MATH_DIV_37(n)       (((n) * UINT32_C(56680)) >> 21)

/*! \brief      Binary modulo 37. */
#define LL_MATH_MOD_37(n)       ((n) - (LL_MATH_DIV_37(n) * 37))

/*! \brief      Binary divide with 1250 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_1250(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(1717987)) >> 31))

/*! \brief      Binary divide with 625 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_625(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(1717987)) >> 30))

/*! \brief      Binary divide with 30 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_30(n)       ((uint32_t)(((uint64_t)(n) * UINT64_C(286331154)) >> 33))

/*! \brief      Binary divide with 300 divisor (n[max]=0x3FFFFFFF). */
#define LL_MATH_DIV_300(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(14660155038)) >> 42))

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ECC service callback. */
typedef void (*LlMathEccServiceCback_t)(uint8_t op);

/*! \brief      ECC operations. */
enum
{
  LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR,   /*!< Generate P-256 key pair. */
  LL_MATH_ECC_OP_GENERATE_DH_KEY           /*!< Generate Diffie-Hellman key. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Generate random number.
 *
 *  \return 32-bit random number.
 */
/*************************************************************************************************/
uint32_t LlMathRandNum(void);

/*************************************************************************************************/
/*!
 *  \brief  Return the number of bits which are set.
 *
 *  \param  num   Input parameter.
 *
 *  \return Number of bits which are set.
 */
/*************************************************************************************************/
uint8_t LlMathGetNumBitsSet(uint64_t num);

/*************************************************************************************************/
/*!
 *  \brief  Return result of a division.
 *
 *  \param  nu32        Numerator of size 32 bits.
 *  \param  de32        Denominator of size 32 bits.
 *
 *  \return Result of a division.
 */
/*************************************************************************************************/
uint32_t LlMathDivideUint32(uint32_t nu32, uint32_t de32);

/*************************************************************************************************/
/*!
 *  \brief  Return result of a division, rounding up.
 *
 *  \param  nu32        Numerator of size 32 bits.
 *  \param  de32        Denominator of size 32 bits.
 *
 *  \return Result of a division.
 */
/*************************************************************************************************/
uint32_t LlMathDivideUint32RoundUp(uint32_t nu32, uint32_t de32);

#ifdef __cplusplus
};
#endif

#endif /* LL_API_H */
