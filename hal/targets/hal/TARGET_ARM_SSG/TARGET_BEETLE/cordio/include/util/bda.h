/*************************************************************************************************/
/*!
 *  \file   bda.h
 *
 *  \brief  Bluetooth device address utilities.
 *
 *          $Date: 2016-02-18 16:07:11 -0800 (Thu, 18 Feb 2016) $
 *          $Revision: 5910 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
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
