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
 *  \brief Byte stream to integer conversion functions.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief  Convert bstream to uint64_t.
 *
 *  \param  p       Bstream pointer.
 *
 *  \return Resulting uint64_t number.
 */
/*************************************************************************************************/
uint64_t BstreamToUint64(const uint8_t *p)
{
  return ((uint64_t)p[0] <<  0) |
         ((uint64_t)p[1] <<  8) |
         ((uint64_t)p[2] << 16) |
         ((uint64_t)p[3] << 24) |
         ((uint64_t)p[4] << 32) |
         ((uint64_t)p[5] << 40) |
         ((uint64_t)p[6] << 48) |
         ((uint64_t)p[7] << 56);
}

/*************************************************************************************************/
/*!
 *  \brief  Convert uint64_t to bstream.
 *
 *  \param  p       Bstream pointer.
 *  \param  n       uint64_t number.
 *
 *  \return None.
 */
/*************************************************************************************************/
void Uint64ToBstream(uint8_t *p, uint64_t n)
{
  p[0] = (uint8_t)(n >>  0);
  p[1] = (uint8_t)(n >>  8);
  p[2] = (uint8_t)(n >> 16);
  p[3] = (uint8_t)(n >> 24);
  p[4] = (uint8_t)(n >> 32);
  p[5] = (uint8_t)(n >> 40);
  p[6] = (uint8_t)(n >> 48);
  p[7] = (uint8_t)(n >> 56);
}

/*************************************************************************************************/
/*!
 *  \brief  Convert bstream to BDA64.
 *
 *  \param  p       Bstream pointer.
 *
 *  \return Resulting BDA64 number.
 */
/*************************************************************************************************/
uint64_t BstreamToBda64(const uint8_t *p)
{
  return (uint64_t)p[0] <<  0 |
         (uint64_t)p[1] <<  8 |
         (uint64_t)p[2] << 16 |
         (uint64_t)p[3] << 24 |
         (uint64_t)p[4] << 32 |
         (uint64_t)p[5] << 40;
}

/*************************************************************************************************/
/*!
 *  \brief  Convert BDA64 to bstream.
 *
 *  \param  p       Bstream pointer.
 *  \param  bda     uint64_t BDA.
 *
 *  \return None.
 */
/*************************************************************************************************/
void Bda64ToBstream(uint8_t *p, uint64_t bda)
{
  p[0] = (uint8_t)(bda >>  0);
  p[1] = (uint8_t)(bda >>  8);
  p[2] = (uint8_t)(bda >> 16);
  p[3] = (uint8_t)(bda >> 24);
  p[4] = (uint8_t)(bda >> 32);
  p[5] = (uint8_t)(bda >> 40);
}
