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
#ifndef BDA_H
#define BDA_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      BD address length */
#define BDA_ADDR_LEN                  6

/*! \brief      BD address string length */
#define BDA_ADDR_STR_LEN              (BDA_ADDR_LEN * 2)

/*! \brief      BDA RPA check */
#define BDA_ADDR_IS_RPA(bda)          (((bda)[5] & 0xC0) == 0x40)

/*! \brief      BDA NRPA check */
#define BDA_ADDR_IS_NRPA(bda)         (((bda)[5] & 0xC0) == 0x00)

/*! \brief      BDA static random check */
#define BDA_ADDR_IS_STATIC(bda)       (((bda)[5] & 0xC0) == 0xC0)

/*! \brief      BDA64 RPA check */
#define BDA64_ADDR_IS_RPA(bda64)      ((((bda64) >> 40) & 0xC0) == 0x40)

/*! \brief      BDA64 NRPA check */
#define BDA64_ADDR_IS_NRPA(bda64)     ((((bda64) >> 40) & 0xC0) == 0x00)

/*! \brief      BDA64 static random check */
#define BDA64_ADDR_IS_STATIC(bda64)   ((((bda64) >> 40) & 0xC0) == 0xC0)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      BD address data type */
typedef uint8_t bdAddr_t[BDA_ADDR_LEN];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

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
void BdaCpy(uint8_t *pDst, const uint8_t *pSrc);


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
bool_t BdaCmp(const uint8_t *pAddr1, const uint8_t *pAddr2);

/*************************************************************************************************/
/*!
 *  \brief  Set a BD address to all zeros.
 *
 *  \param  pDst    Pointer to destination.
 *
 *  \return pDst + BDA_ADDR_LEN
 */
/*************************************************************************************************/
uint8_t *BdaClr(uint8_t *pDst);

/*************************************************************************************************/
/*!
*  \brief  Check if a BD address is all zeros.
*
*  \param  pAddr    Pointer to address.
*
*  \return TRUE if address is all zeros, FALSE otherwise.
*/
/*************************************************************************************************/
bool_t BdaIsZeros(const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief  Convert a BD address to a string.
 *
 *  \param  pAddr    Pointer to BD address.
 *
 *  \return Pointer to string.
 */
/*************************************************************************************************/
char *Bda2Str(const uint8_t *pAddr);

/*! \} */    /* WSF_UTIL_API */

#ifdef __cplusplus
};
#endif

#endif /* BDA_H */
