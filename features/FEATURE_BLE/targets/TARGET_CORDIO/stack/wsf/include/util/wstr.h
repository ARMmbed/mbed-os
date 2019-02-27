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

#ifndef WSTR_H
#define WSTR_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! HEX format '0x' or '0X' check */
#define WSTR_IS_HEX_FORMAT(c) (((char)(c)[0] == '0') && \
                               (((char)(c)[1] == 'x') || ((char)(c)[1] == 'X')))

/*! Binary format '0b' check */
#define WSTR_IS_BIN_FORMAT(c)  (((char)(c)[0] == '0') && ((char)(c)[1] == 'b'))

/*************************************************************************************************/
/*!
 *  \brief  Copies a string up to a given length.
 *
 *  \param  pBuf    Pointer to buffer to copy to.
 *  \param  pData   Pointer to the string to copy.
 *  \param  n       Size of pBuf in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WstrnCpy(char *pBuf, const char *pData, uint8_t n);

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
void WStrReverseCpy(uint8_t *pBuf1, const uint8_t *pBuf2, uint16_t len);

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
void WStrReverse(uint8_t *pBuf, uint8_t len);

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
void WStrFormatHex(char *pBuf, uint32_t val, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Convert a formatted string to int array, zero out space after the string length.
 *
 *  \param  pStr    Pointer to the string to convert.
 *  \param  pBuf    Pointer to destination buffer.
 *  \param  len     Size of pBuf in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WStrHexToArray(const char *pStr, uint8_t *pBuf, uint16_t len);

/*! \} */    /* WSF_UTIL_API */

#ifdef __cplusplus
}
#endif

#endif /* WSTR_H */
