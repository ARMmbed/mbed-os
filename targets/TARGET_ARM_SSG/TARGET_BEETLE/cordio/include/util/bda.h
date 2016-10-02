/*************************************************************************************************/
/*!
 *  \file   bda.h
 *
 *  \brief  Bluetooth device address utilities.
 *
 *          $Date: 2015-09-26 13:06:16 -0400 (Sat, 26 Sep 2015) $
 *          $Revision: 4027 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

/*! BD address length */
#define BDA_ADDR_LEN         6

/*! BD address string length */
#define BDA_ADDR_STR_LEN    (BDA_ADDR_LEN * 2)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! BD address data type */
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
