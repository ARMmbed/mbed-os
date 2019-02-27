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
 *  \brief Bluetooth device address utilities.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "util/bda.h"

/*************************************************************************************************/
/*!
 *  \brief  Copy a BD address from source to destination.
 *
 *  \param  pDst    Pointer to destination.
 *  \param  pSrc    Pointer to source.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BdaCpy(uint8_t *pDst, const uint8_t *pSrc)
{
  memcpy(pDst, pSrc, BDA_ADDR_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Compare two BD addresses.
 *
 *  \param  pAddr1  First address.
 *  \param  pAddr2  Second address.
 *
 *  \return TRUE if addresses match, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BdaCmp(const uint8_t *pAddr1, const uint8_t *pAddr2)
{
  return (memcmp(pAddr1, pAddr2, BDA_ADDR_LEN) == 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Set a BD address to all zeros.
 *
 *  \param  pDst    Pointer to destination.
 *
 *  \return pDst + BDA_ADDR_LEN
 */
/*************************************************************************************************/
uint8_t *BdaClr(uint8_t *pDst)
{
  memset(pDst, 0, BDA_ADDR_LEN);

  return (pDst + BDA_ADDR_LEN);
}

/*************************************************************************************************/
/*!
*  \brief  Check if a BD address is all zeros.
*
*  \param  pAddr    Pointer to address.
*
*  \return TRUE if address is all zeros, FALSE otherwise.
*/
/*************************************************************************************************/
bool_t BdaIsZeros(const uint8_t *pAddr)
{
  uint8_t addrZeros[BDA_ADDR_LEN] = { 0 };

  return (memcmp(pAddr, addrZeros, BDA_ADDR_LEN) == 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Convert a BD address to a string.
 *
 *  \param  pAddr    Pointer to BD address.
 *
 *  \return Pointer to string.
 */
/*************************************************************************************************/
char *Bda2Str(const uint8_t *pAddr)
{
  static const char hex[] = "0123456789ABCDEF";
  static char       str[BDA_ADDR_STR_LEN + 1];
  char              *pStr = str;

  /* address is little endian so copy in reverse */
  pAddr += BDA_ADDR_LEN;

  while (pStr < &str[BDA_ADDR_STR_LEN])
  {
    *pStr++ = hex[*--pAddr >> 4];
    *pStr++ = hex[*pAddr & 0x0F];
  }

  /* null terminate string */
  *pStr = 0;

  return str;
}
