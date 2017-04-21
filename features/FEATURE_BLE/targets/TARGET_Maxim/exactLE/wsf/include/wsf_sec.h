/*************************************************************************************************/
/*!
 *  \file   wsf_sec.h
 *        
 *  \brief  AES and random number security service API.
 *
 *          $Date: 2011-10-14 21:35:03 -0700 (Fri, 14 Oct 2011) $
 *          $Revision: 191 $
 *  
 *  Copyright (c) 2010-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.

 */
/*************************************************************************************************/
#ifndef WSF_SEC_H
#define WSF_SEC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! AES callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;              /*! header */
  uint8_t       *pCiphertext;     /*! pointer to 16 bytes of ciphertext data */
} wsfSecAes_t;

/*! AES callback function type */
typedef void (*wsfSecAesCback_t)(wsfSecAes_t *pMsg);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfSecInit
 *        
 *  \brief  Initialize the security service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecRandInit
 *        
 *  \brief  Initialize the random number service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecRandInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecAes
 *        
 *  \brief  Execute an AES calculation.  When the calculation completes, a WSF message will be
 *          sent to the specified handler.  This function returns a token value that
 *          the client can use to match calls to this function with messages.
 *
 *  \param  pKey        Pointer to 16 byte key.
 *  \param  pPlaintext  Pointer to 16 byte plaintext.
 *  \param  handlerId   WSF handler ID.
 *  \param  param       Client-defined parameter returned in message.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return Token value.
 */
/*************************************************************************************************/
uint8_t WsfSecAes(uint8_t *pKey, uint8_t *pPlaintext, wsfHandlerId_t handlerId,
                  uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecRand
 *        
 *  \brief  This function returns up to 16 bytes of random data to a buffer provided by the
 *          client.
 *
 *  \param  pRand       Pointer to returned random data.
 *  \param  randLen     Length of random data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecRand(uint8_t *pRand, uint8_t randLen);

#ifdef __cplusplus
};
#endif

#endif /* WSF_SEC_H */
