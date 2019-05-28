/* Copyright (c) 2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 * \file
 * \brief Common math utilities generic implementation file.
 */
/*************************************************************************************************/

#include "ll_math.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

static uint32_t wsfRngW = 88675123;     /*!< W seed for random number generation. */
static uint32_t wsfRngX = 123456789;    /*!< X seed for random number generation. */
static uint32_t wsfRngY = 362436069;    /*!< Y seed for random number generation. */
static uint32_t wsfRngZ = 521288629;    /*!< Z seed for random number generation. */

/*************************************************************************************************/
/*!
 *  \brief  Initialize random number seeds.
 *
 *  \param seed    Pointer to initial seed for random numbers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathSetSeed(const uint32_t *pSeed)
{
  wsfRngW ^= *pSeed;
  wsfRngX ^= *pSeed;
  wsfRngY ^= *pSeed;
  wsfRngZ ^= *pSeed;
}

/*************************************************************************************************/
/*!
 *  \fn     LlMathRandNum
 *
 *  \brief  Generate random number.
 *
 *  \return 32-bit random number.
 *
 *  This software implementation uses a xorshift random number generator.
 *      George Marsaglia (2003), "Xorshift RNGs", Journal of Statistical Software
 *
 *  \note   This routine is not a cryptographic grade random number generator.
 */
/*************************************************************************************************/
uint32_t LlMathRandNum(void)
{
  uint32_t t;

  t = wsfRngX ^ (wsfRngX << 11);
  wsfRngX = wsfRngY;
  wsfRngY = wsfRngZ;
  wsfRngZ = wsfRngW;
  wsfRngW = wsfRngW ^ (wsfRngW >> 19) ^ (t ^ (t >> 8));
  return wsfRngW;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the number of bits which are set.
 *
 *  \param  num     Input parameter.
 *
 *  \return Number of bits which are set.
 */
/*************************************************************************************************/
uint8_t LlMathGetNumBitsSet(uint64_t num)
{
  num = num - ((num >> 1) & 0x5555555555555555);
  num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
  return (((num + (num >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56;
}

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
uint32_t LlMathDivideUint32(uint32_t nu32, uint32_t de32)
{
  uint32_t temp = 1;
  uint32_t result = 0;
  uint64_t nu = nu32;
  uint64_t de = de32;

  while (de <= nu)
  {
    de <<= 1;
    temp <<= 1;
  }

  while (temp > 1)
  {
    de >>= 1;
    temp >>= 1;

    if (nu >= de)
    {
      nu -= de;
      result += temp;
    }
  }

  return result;
}
