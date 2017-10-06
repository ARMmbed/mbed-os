/*************************************************************************************************/
/*!
 *  \file   sec_api.h
 *
 *  \brief  AES and random number security service API.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2010-2017 ARM Ltd., all rights reserved.
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
#ifndef SEC_API_H
#define SEC_API_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! CMAC algorithm key length */
#define SEC_CMAC_KEY_LEN              16

/*! CMAC algorithm result length */
#define SEC_CMAC_HASH_LEN             16

/*! ECC algorithm key length */
#define SEC_ECC_KEY_LEN               32

/*! Invalid AES Token */
#define SEC_TOKEN_INVALID             0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! AES Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*! header */
  uint8_t       *pCiphertext;        /*! pointer to 16 bytes of ciphertext data */
} secMsg_t;

/*! AES Security callback are the same as secMsg_t */
typedef secMsg_t secAes_t;

/*! CMAC Security callback are the same as secMsg_t */
typedef secMsg_t secCmacMsg_t;

/*! ECC Security public/private key pair */
typedef struct
{
  uint8_t pubKey_x[SEC_ECC_KEY_LEN]; /*! x component of ecc public key */
  uint8_t pubKey_y[SEC_ECC_KEY_LEN]; /*! y component of ecc public key */
  uint8_t privKey[SEC_ECC_KEY_LEN];  /*! ecc private key */
} secEccKey_t;

/*! ECC security DH Key shared secret */
typedef struct
{
  uint8_t secret[SEC_ECC_KEY_LEN];   /*! DH Key Shared secret */
} secEccSharedSec_t;


/*! ECC Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*! header */
  union
  {
    secEccSharedSec_t sharedSecret;  /*! shared secret */
    secEccKey_t key;                 /*! ecc public/private key pair */
  } data;
} secEccMsg_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     SecInit
 *
 *  \brief  Initialize the security service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecInit(void);

/*************************************************************************************************/
/*!
 *  \fn     SecRandInit
 *
 *  \brief  Initialize the random number service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRandInit(void);

/*************************************************************************************************/
/*!
 *  \fn     SecAesInit
 *
 *  \brief  Initialize the AES service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecAesInit(void);

/*************************************************************************************************/
/*!
 *  \fn     SecCmacInit
 *
 *  \brief  Called to initialize CMAC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCmacInit(void);

/*************************************************************************************************/
/*!
 *  \fn     SecEccInit
 *
 *  \brief  Called to initialize ECC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecEccInit(void);

/*************************************************************************************************/
/*!
 *  \fn     SecAes
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
uint8_t SecAes(uint8_t *pKey, uint8_t *pPlaintext, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     SecAesCmac
 *
 *  \brief  Execute the CMAC algorithm.
 *
 *  \param  pKey        Key used in CMAC operation.
 *  \param  pPlaintext  Data to perform CMAC operation over
 *  \param  len         Size of pPlaintext in bytes.
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecCmac(const uint8_t *pKey, uint8_t *pPlaintext, uint8_t textLen, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     SecEccGenKey
 *
 *  \brief  Generate an ECC key.
 *
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecEccGenKey(wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     SecEccGenSharedSecret
 *
 *  \brief  Generate an ECC key.
 *
 *  \param  pKey        ECC Key structure.
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecEccGenSharedSecret(secEccKey_t *pKey, wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     SecRand
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
void SecRand(uint8_t *pRand, uint8_t randLen);

#ifdef __cplusplus
};
#endif

#endif /* SEC_API_H */
