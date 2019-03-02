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
 *  \brief String manipulation functions.
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

/*************************************************************************************************/
/*!
 *  \fn     WStrHexToArray
 *
 *  \brief  Convert a hex string to int array, zero out space after the string length.
 *
 *  \param  pStr    Pointer to the string to convert.
 *  \param  pBuf    Pointer to destination buffer.
 *  \param  len     Size of pBuf in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WStrHexToArray(const char *pStr, uint8_t *pBuf, uint16_t len)
{
  uint16_t i;
  uint8_t val = 0;
  uint8_t j = 0;
  bool_t zeroing = FALSE;

  /* Skip white spaces. */
  while (*pStr == ' ' || *pStr == '\t' || *pStr == '\n' || *pStr == '\r')
  {
    ++pStr;
  }

  /* Check for hex value format (0x or 0X). */
  if (WSTR_IS_HEX_FORMAT(pStr))
  {
    pStr += 2;
  }

  for (i = 0; i < len; i++)
  {
    if (!zeroing && *pStr == '\0')
    {
      zeroing = TRUE;
    }

    if (zeroing)
    {
      *pBuf++ = 0;
    }
    else
    {
      while (j < 2)
      {
        if ((*pStr >= '0') && (*pStr <= '9'))
        {
          val = (val << (j << 2)) | (*pStr - '0');
        }
        else if ((*pStr >= 'a') && (*pStr <= 'f'))
        {
          val = (val << (j << 2)) | (10 + (*pStr - 'a'));
        }
        else if ((*pStr >= 'A') && (*pStr <= 'F'))
        {
          val = (val << (j << 2)) | (10 + (*pStr - 'A'));
        }
        else
        {
          break;
        }

        pStr++;

        /* Check if value is computed and needs to be added to destination buffer. */
        if (j++ == 1)
        {
          *pBuf++ = val;
        }
      }

      j = 0;
      val = 0;
    }
  }
}
