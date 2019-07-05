/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer (LL) secure connections control interface file.
 */
/*************************************************************************************************/

#include "lmgr_api.h"
#include "lmgr_api_sc.h"
#include "lctr_api.h"
#include "ll_api.h"
#include "hci_defs.h"
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
 *  \brief      Generate a Diffie-Hellman key.
 *
 *  \param      pubKey_x  Remote public key x-coordinate.
 *  \param      pubKey_y  Remote public key y-coordinate.
 *  \param      keyType   Debug enable
 *
 *  \return     Status error code.
 *
 *  Generate a Diffie-Hellman key from a remote public key and the local private key.  If another
 *  ECC operation (P-256 key pair generation or Diffie-Hellman key generation) is ongoing, an error
 *  will be returned. If keyType == HCI_PRIVATE_KEY_DEBUG, debug keys will be used.
 */
/*************************************************************************************************/
uint8_t LlGenerateDhKeyV2(uint8_t pubKey_x[LL_ECC_KEY_LEN], uint8_t pubKey_y[LL_ECC_KEY_LEN], uint8_t keyType)
{

  if (keyType == HCI_PRIVATE_KEY_DEBUG)
  {
    LL_TRACE_INFO0("Using Debug keys for DHKey generation");
    return LctrGenerateDebugDhKey();
  }
  else
  {
    uint8_t pubKey[LL_ECC_KEY_LEN * 2];
    uint8_t privKey[LL_ECC_KEY_LEN];

    memcpy(pubKey, pubKey_x, LL_ECC_KEY_LEN);
    memcpy(pubKey + LL_ECC_KEY_LEN, pubKey_y, LL_ECC_KEY_LEN);
    memcpy(privKey, lmgrScCb.privKey, LL_ECC_KEY_LEN);

    return LctrGenerateDhKey(pubKey, privKey);
  }
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

/*************************************************************************************************/
/*!
 *  \brief      Set validate public key mode.
 *
 *  \param      validateMode   ALT1 or ALT2.
 *
 *  \return     Status error code.
 *
 */
/*************************************************************************************************/
uint8_t LlSetValidatePublicKeyMode(uint8_t validateMode)
{
  if (validateMode > KEY_VALIDATE_MODE_MAX)
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  return LctrSetValidatePublicKeyMode(validateMode);
}
