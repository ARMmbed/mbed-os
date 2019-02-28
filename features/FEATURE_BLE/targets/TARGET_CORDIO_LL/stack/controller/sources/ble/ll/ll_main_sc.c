/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) secure connections control interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lmgr_api.h"
#include "lmgr_api_sc.h"
#include "lctr_api.h"
#include "ll_api.h"
#include "lctr_api_sc.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include <string.h>
#if (LL_ENABLE_TESTER)
#include "ll_tester_api.h"
#endif

/*************************************************************************************************/
/*!
 *  \brief      Generate a P-256 public/private key pair.
 *
 *  \return     Status error code.
 *
 *  Generate a P-256 public/private key pair.  If another ECC operation (P-256 key pair generation
 *  or Diffie-Hellman key generation) is ongoing, an error will be returned.
 */
/*************************************************************************************************/
uint8_t LlGenerateP256KeyPair(void)
{
  return LctrGenerateP256KeyPair();
}

/*************************************************************************************************/
/*!
 *  \brief      Generate a Diffie-Hellman key.
 *
 *  \param      pubKey_x  Remote public key x-coordinate.
 *  \param      pubKey_y  Remote public key y-coordinate.
 *
 *  \return     Status error code.
 *
 *  Generate a Diffie-Hellman key from a remote public key and the local private key.  If another
 *  ECC operation (P-256 key pair generation or Diffie-Hellman key generation) is ongoing, an error
 *  will be returned.
 */
/*************************************************************************************************/
uint8_t LlGenerateDhKey(const uint8_t pubKey_x[LL_ECC_KEY_LEN], const uint8_t pubKey_y[LL_ECC_KEY_LEN])
{
  uint8_t pubKey[LL_ECC_KEY_LEN * 2];
  uint8_t privKey[LL_ECC_KEY_LEN];

  memcpy(pubKey, pubKey_x, LL_ECC_KEY_LEN);
  memcpy(pubKey + LL_ECC_KEY_LEN, pubKey_y, LL_ECC_KEY_LEN);
  memcpy(privKey, lmgrScCb.privKey, LL_ECC_KEY_LEN);

  return LctrGenerateDhKey(pubKey, privKey);
}

/*************************************************************************************************/
/*!
 *  \brief      Set P-256 private key for debug purposes.
 *
 *  \param      privKey   Private key, or all zeros to clear set private key.
 *
 *  \return     Status error code.
 *
 *  Set P-256 private key or clear set private key.  The private key will be used for generate key
 *  pairs and Diffie-Hellman keys until cleared.
 */
/*************************************************************************************************/
uint8_t LlSetP256PrivateKey(const uint8_t privKey[LL_ECC_KEY_LEN])
{
  bool_t allZeros = TRUE;
  unsigned int i;

  for (i = 0; i < LL_ECC_KEY_LEN; i++)
  {
    if (privKey[i] != 0)
    {
      allZeros = FALSE;
      break;
    }
  }

  if (allZeros)
  {
    lmgrScCb.privKeySet = FALSE;
  }
  else
  {
    memcpy(lmgrScCb.privKey, privKey, LL_ECC_KEY_LEN);
    lmgrScCb.privKeySet = TRUE;
  }

  return LL_SUCCESS;
}
