/*************************************************************************************************/
/*!
 *  \file   bda.h
 *
 *  \brief  Bluetooth device address utilities.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef BDA_H
#define BDA_H

#ifdef __cplusplus
extern "C" {
#endif

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
 *  \fn     BdaCpy
 *
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
 *  \fn     BdaCmp
 *
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
 *  \fn     BdaClr
 *
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
*  \fn     BdaIsZeros
*
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
 *  \fn     Bda2Str
 *
 *  \brief  Convert a BD address to a string.
 *
 *  \param  pAddr    Pointer to BD address.
 *
 *  \return Pointer to string.
 */
/*************************************************************************************************/
char *Bda2Str(const uint8_t *pAddr);

#ifdef __cplusplus
};
#endif

#endif /* BDA_H */
