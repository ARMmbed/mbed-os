/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  AES and random number security service API.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
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
#ifndef SEC_API_H
#define SEC_API_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_SECURITY_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief CMAC algorithm key length */
#define SEC_CMAC_KEY_LEN              16

/*! \brief CMAC algorithm result length */
#define SEC_CMAC_HASH_LEN             16

/*! \brief ECC algorithm key length */
#define SEC_ECC_KEY_LEN               32

/*! \brief Invalid AES Token */
#define SEC_TOKEN_INVALID             0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief AES Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< \brief header */
  uint8_t       *pCiphertext;        /*!< \brief pointer to 16 bytes of ciphertext data */
} secMsg_t;

/*! \brief AES Security callback are the same as secMsg_t */
typedef secMsg_t secAes_t;

/*! \brief CMAC Security callback are the same as secMsg_t */
typedef secMsg_t secCmacMsg_t;

/*! \brief ECC Security public/private key pair */
typedef struct
{
  uint8_t pubKey_x[SEC_ECC_KEY_LEN]; /*!< \brief x component of ECC public key */
  uint8_t pubKey_y[SEC_ECC_KEY_LEN]; /*!< \brief y component of ECC public key */
  uint8_t privKey[SEC_ECC_KEY_LEN];  /*!< \brief ecc private key */
} secEccKey_t;

/*! \brief ECC security DH Key shared secret */
typedef struct
{
  uint8_t secret[SEC_ECC_KEY_LEN];   /*!< \brief DH Key Shared secret */
} secEccSharedSec_t;


/*! \brief ECC Security callback parameters structure */
typedef struct
{
  wsfMsgHdr_t   hdr;                 /*!< \brief header */
  union
  {
    secEccSharedSec_t sharedSecret;  /*!< \brief shared secret */
    secEccKey_t key;                 /*!< \brief ECC public/private key pair */
  } data;                            /*!< \brief ECC message data union */
} secEccMsg_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name Security Initialization Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the security service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the random number service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRandInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the AES service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecAesInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CMAC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCmacInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize ECC security.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecEccInit(void);

/**@}*/

/** \name Security AES and CMAC Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
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
 *  \brief  Execute the CMAC algorithm.
 *
 *  \param  pKey        Key used in CMAC operation.
 *  \param  pPlaintext  Data to perform CMAC operation over
 *  \param  textLen     Size of pPlaintext in bytes.
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecCmac(const uint8_t *pKey, uint8_t *pPlaintext, uint8_t textLen, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event);

/**@}*/

/** \name Security ECC Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
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

/**@}*/

/** \name Security Random Number Generator Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
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

/**@}*/

/*! \} */    /* STACK_SECURITY_API */

#ifdef __cplusplus
};
#endif

#endif /* SEC_API_H */
