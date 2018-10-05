/*************************************************************************************************/
/*!
 *  \file   wstr.c
 *
 *  \brief  String manipulation functions.
 *
 *  Copyright (c) 2014-2018 Arm Ltd. All Rights Reserved.
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

#include <string.h>
#include "wsf_types.h"
#include "util/wstr.h"

/*************************************************************************************************/
/*!
 *  \brief  Copy a string and zero out space after the string length.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WstrnCpy(char *pBuf, const char *pData, uint8_t n)
{
  uint8_t i;
  uint8_t zeroing = FALSE;

  for (i=0; i<n; i++)
  {
    if (!zeroing && pData[i] == '\0')
      zeroing = TRUE;

    if (zeroing)
      *pBuf++ = 0;
    else
      *pBuf++ = pData[i];
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Byte by byte reverse and copy a buffer.
 *
 *  \param  pBuf1   Buffer to hold reversed copy.
 *  \param  pBuf2   Buffer to copy.
 *  \param  len     Size of pBuf1 and pBuf2 in bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WStrReverseCpy(uint8_t *pBuf1, const uint8_t *pBuf2, uint16_t len)
{
  int16_t i;

  for (i=0; i<len; i++)
  {
    pBuf1[len-1-i] = pBuf2[i];
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Byte by byte reverse a buffer.
 *
 *  \param  pBuf   Buffer to reverse.
 *  \param  len    size of pBuf in bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WStrReverse(uint8_t *pBuf, uint8_t len)
{
  uint8_t i, temp;

  for (i=0; i<len/2; i++)
  {
    temp = pBuf[len-i-1];
    pBuf[len-i-1] = pBuf[i];
    pBuf[i] = temp;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Format a hex value.
 *
 *  \param  pBuf    Storage for string representation of value.
 *  \param  val     Value.
 *  \param  len     Length of value, in bits.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void WStrFormatHex(char *pBuf, uint32_t val, uint8_t len)
{
  uint32_t i;
  uint8_t  lenChars = (len + 3) / 4;

  for (i = 0; i < lenChars; i++)
  {
    uint8_t n = val & 0x0F;
    pBuf[(lenChars - 1) - i] = (n < 10) ? ('0' + n) : ('A' + (n - 10));
    val >>= 4;
  }
}
