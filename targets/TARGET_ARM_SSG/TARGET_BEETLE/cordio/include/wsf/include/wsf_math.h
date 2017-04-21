/*************************************************************************************************/
/*!
 *  \file   wsf_math.h
 *
 *  \brief  Common math utilities.
 *
 *          $Date: 2016-04-05 14:14:53 -0700 (Tue, 05 Apr 2016) $
 *          $Revision: 6646 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
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
#ifndef WSF_MATH_H
#define WSF_MATH_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Returns the minimum of two values. */
#define WSF_MIN(a,b)        ((a) < (b) ? (a) : (b))

/*! \brief      Returns the maximum of two values. */
#define WSF_MAX(a,b)        ((a) > (b) ? (a) : (b))

/*! \brief      ECC key length. */
#define WSF_MATH_ECC_KEY_LEN    32

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ECC service callback. */
typedef void (*WsfMathEccServiceCback_t)(uint8_t op);

/*! \brief      ECC operations. */
enum
{
  WSF_MATH_ECC_OP_GENERATE_P256_KEY_PAIR,   /*!< Generate P-256 key pair. */
  WSF_MATH_ECC_OP_GENERATE_DH_KEY           /*!< Generate Diffie-Hellman key. */
};


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfMathInit
 *
 *  \brief  Initialize math routines.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathInit(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfRandNum
 *
 *  \brief  Generate random number.
 *
 *  \return 32-bit random number.
 */
/*************************************************************************************************/
uint32_t WsfRandNum(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfAesEcb
 *
 *  \brief  Calculate AES ECB.
 *
 *  \param  pKey        Encryption key.
 *  \param  pOut        Output data.
 *  \param  pIn         Input data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfAesEcb(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn);

/*************************************************************************************************/
/*!
 *  \brief  Set service callback for ECC generation.
 *
 *  \param  cback   Callback to invoke when driver needs servicing.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccSetServiceCback(WsfMathEccServiceCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Start generating P-256 key pair.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccGenerateP256KeyPairStart(void);

/*************************************************************************************************/
/*!
 *  \brief  Start generating P-256 public key with a specified private key.
 *
 *  \param  pPrivKey      Private key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccGenerateP256PublicKeyStart(const uint8_t *pPrivKey);

/*************************************************************************************************/
/*!
 *  \brief  Continue generating P-256 key pair.
 *
 *  \return TRUE if key generation complete.
 */
/*************************************************************************************************/
bool_t WsfMathEccGenerateP256KeyPairContinue(void);

/*************************************************************************************************/
/*!
 *  \brief  Get results from generating P-256 key pair.
 *
 *  \param  pPubKey     Storage for public key.
 *  \param  pPrivKey    Storage for private key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccGenerateP256KeyPairComplete(uint8_t *pPubKey, uint8_t *pPrivKey);

/*************************************************************************************************/
/*!
 *  \brief  Start generating Diffie-Hellman key.
 *
 *  \param  pPublicKey    Public key.
 *  \param  pPrivateKey   Private key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccGenerateDhKeyStart(const uint8_t *pPubKey, const uint8_t *pPrivKey);

/*************************************************************************************************/
/*!
 *  \brief  Continue generating Diffie-Hellman key.
 *
 *  \return TRUE if Diffie-Hellman key generation complete.
 */
/*************************************************************************************************/
bool_t WsfMathEccGenerateDhKeyContinue(void);

/*************************************************************************************************/
/*!
 *  \brief  Get results from generating Diffie-Hellman key.
 *
 *  \param  pDhKey      Storage for Diffie-Hellman key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMathEccGenerateDhKeyComplete(uint8_t *pDhKey);

#ifdef __cplusplus
};
#endif

#endif /* WSF_MATH_H */
