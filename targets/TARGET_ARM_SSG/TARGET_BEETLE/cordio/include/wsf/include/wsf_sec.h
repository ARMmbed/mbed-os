/*************************************************************************************************/
/*!
 *  \file   wsf_sec.h
 *        
 *  \brief  AES and random number security service API.
 *
 *          $Date: 2015-10-15 11:57:57 -0700 (Thu, 15 Oct 2015) $
 *          $Revision: 4218 $
 *  
 *  Copyright (c) 2010 Wicentric, Inc., all rights reserved.
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
#ifndef WSF_SEC_H
#define WSF_SEC_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! CMAC algorithm key length */
#define WSF_CMAC_KEY_LEN              16

/*! CMAC algorithm result length */
#define WSF_CMAC_HASH_LEN             16

/*! ECC algorithm key length */
#define WSF_ECC_KEY_LEN               32

/*! Invalid AES Token */
#define WSF_TOKEN_INVALID             0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! AES Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;              /*! header */
  uint8_t       *pCiphertext;     /*! pointer to 16 bytes of ciphertext data */
} wsfSecMsg_t;

/*! AES Security callback are the same as wsfSecMsg_t */
typedef wsfSecMsg_t wsfSecAes_t;

/*! CMAC Security callback are the same as wsfSecMsg_t */
typedef wsfSecMsg_t wsfSecCmacMsg_t;

/*! ECC Security public/private key pair */
typedef struct
{
  uint8_t pubKey_x[WSF_ECC_KEY_LEN]; /*! x component of ecc public key */
  uint8_t pubKey_y[WSF_ECC_KEY_LEN]; /*! y component of ecc public key */
  uint8_t privKey[WSF_ECC_KEY_LEN];  /*! ecc private key */
} wsfSecEccKey_t;

/*! ECC security DH Key shared secret */
typedef struct
{
  uint8_t secret[WSF_ECC_KEY_LEN];  /*! DH Key Shared secret */
} wsfSecEccSharedSec_t;


/*! ECC Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;                    /*! header */
  union
  {
    wsfSecEccSharedSec_t sharedSecret;  /*! shared secret */
    wsfSecEccKey_t key;                 /*! ecc public/private key pair */
  } data;
} wsfSecEccMsg_t;


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
 *  \fn     WsfSecAesInit
 *        
 *  \brief  Initialize the AES service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecAesInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecCmacInit
 *        
 *  \brief  Called to initialize CMAC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecCmacInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecEccInit
 *        
 *  \brief  Called to initialize ECC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSecEccInit(void);

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
 *  \fn     WsfSecAesCmac
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
bool_t WsfSecCmac(const uint8_t *pKey, uint8_t *pPlaintext, uint8_t textLen, wsfHandlerId_t handlerId, 
                   uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecEccGenKey
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
bool_t WsfSecEccGenKey(wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfSecEccGenSharedSecret
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
bool_t WsfSecEccGenSharedSecret(wsfSecEccKey_t *pKey, wsfHandlerId_t handlerId, uint16_t param, uint8_t event);

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
