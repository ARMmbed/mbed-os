/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Pseudo-random number generator interface.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "util/bstream.h"

#include "util/prand.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Random number generator control block. */
static struct
{
  uint32_t rngW;
  uint32_t rngX;
  uint32_t rngY;
  uint32_t rngZ;
} prandCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize random number generator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PrandInit(void)
{
  /* Seed PRNG. */
  prandCb.rngW = 88675123;
  prandCb.rngX = 123456789;
  prandCb.rngY = 362436069;
  prandCb.rngZ = 521288629;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate random number.
 *
 *  \return     32-bit random number.
 *
 *  This software implementation uses a xorshift random number generator.
 *      George Marsaglia (2003), "Xorshift RNGs", Journal of Statistical Software
 *
 *  \note       This routine is not a cryptographic grade random number generator.
 */
/*************************************************************************************************/
static uint32_t prandNum(void)
{
  uint32_t t;

  t = prandCb.rngX ^ (prandCb.rngX << 11);
  prandCb.rngX = prandCb.rngY;
  prandCb.rngY = prandCb.rngZ;
  prandCb.rngZ = prandCb.rngW;
  prandCb.rngW = prandCb.rngW ^ (prandCb.rngW >> 19) ^ (t ^ (t >> 8));
  return prandCb.rngW;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate random data.
 *
 *  \param      pBuf    Buffer.
 *  \param      len     Length of buffer in bytes.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PrandGen(uint8_t *pBuf, uint16_t len)
{
  while (len > 0)
  {
    uint32_t randNum;

    randNum = prandNum();
    if (len >= 4)
    {
      UINT32_TO_BSTREAM(pBuf, randNum);
      len -= 4;
    }
    else
    {
      while (len > 0)
      {
        UINT8_TO_BSTREAM(pBuf, randNum);
        len -= 1;
        randNum >>= 8;
      }
    }
  }
}
