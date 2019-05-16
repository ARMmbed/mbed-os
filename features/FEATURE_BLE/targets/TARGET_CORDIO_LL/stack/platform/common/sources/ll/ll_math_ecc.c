/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief ECC math utilities implementation file.
 */
/*************************************************************************************************/

#include <string.h>

#include "bb_ble_drv.h"
#include "ll_math.h"
#include "ll_defs.h"
#include "wsf_assert.h"

#include "uECC_ll.h"

/**************************************************************************************************
  Variables
**************************************************************************************************/

/*! \brief      ECC service callback. */
static LlMathEccServiceCback_t llMathEccServiceCback = NULL;

/*************************************************************************************************/
/*!
 *  \brief  Random number generator used by uECC.
 *
 *  \param  pDest       Buffer to hold random number
 *  \param  size        Size of pDest in bytes .
 *
 *  \return TRUE if successful.
 */
/*************************************************************************************************/
static int llMathEccRng(uint8_t *pDest, unsigned size)
{
  BbBleDrvRand(pDest, size);

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Copy octets to another buffer, reversing order.
 *
 *  \param  pDest   Destination buffer.
 *  \param  pSrc    Source buffer.
 *  \param  len     Length of data, in octets.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void llMathEccReverseCopy(uint8_t *pDest, const uint8_t *pSrc, uint16_t len)
{
  pSrc += len - 1;
  while (len-- > 0)
  {
    *pDest++ = *pSrc--;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Reverse order of octets in buffer.
 *
 *  \param  p       Buffer.
 *  \param  len     Length of data, in octets.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void llMathEccReverse(uint8_t *p, uint16_t len)
{
  uint8_t *pStart = p;
  uint8_t *pEnd = p + len - 1;
  uint8_t temp;

  len /= 2;
  while (len-- > 0)
  {
    temp      = *pStart;
    *pStart++ = *pEnd;
    *pEnd--   = temp;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set service callback for ECC generation.
 *
 *  \param  cback   Callback to invoke when driver needs servicing.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathEccSetServiceCback(LlMathEccServiceCback_t cback)
{
  llMathEccServiceCback = cback;
}

/*************************************************************************************************/
/*!
 *  \brief  Start generating P-256 key pair.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathEccGenerateP256KeyPairStart(void)
{
  uint8_t privKey[LL_ECC_KEY_LEN];

  WSF_ASSERT(llMathEccServiceCback != NULL);

  /* Generate private key. */
  llMathEccRng(privKey, sizeof(privKey));

  /* Start public key generation. */
  uECC_set_rng(llMathEccRng);
  uECC_make_key_start(privKey);

  llMathEccServiceCback(LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR);
}

/*************************************************************************************************/
/*!
 *  \brief  Start generating P-256 public key with a specified private key.
 *
 *  \param  pPrivKey      Private key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathEccGenerateP256PublicKeyStart(const uint8_t *pPrivKey)
{
  uint8_t privKey[LL_ECC_KEY_LEN];

  WSF_ASSERT(llMathEccServiceCback != NULL);

  /* Reverse private key (to big endian). */
  llMathEccReverseCopy(privKey, pPrivKey, LL_ECC_KEY_LEN);

  /* Start public key generation. */
  uECC_set_rng(llMathEccRng);
  uECC_make_key_start(privKey);

  llMathEccServiceCback(LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR);
}

/*************************************************************************************************/
/*!
 *  \brief  Continue generating P-256 key pair.
 *
 *  \return TRUE if key generation complete.
 */
/*************************************************************************************************/
bool_t LlMathEccGenerateP256KeyPairContinue(void)
{
  WSF_ASSERT(llMathEccServiceCback != NULL);

  if (uECC_make_key_continue())
  {
    return TRUE;
  }

  llMathEccServiceCback(LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR);
  return FALSE;
}

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
void LlMathEccGenerateP256KeyPairComplete(uint8_t *pPubKey, uint8_t *pPrivKey)
{
  /* Complete key generation. */
  uECC_make_key_complete(pPubKey, pPrivKey);

  /* Reverse keys (to little endian). */
  llMathEccReverse(pPubKey, LL_ECC_KEY_LEN);
  llMathEccReverse(pPubKey + LL_ECC_KEY_LEN, LL_ECC_KEY_LEN);
  llMathEccReverse(pPrivKey, LL_ECC_KEY_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Start generating Diffie-Hellman key.
 *
 *  \param  pPubKey     Public key.
 *  \param  pPrivKey    Private key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathEccGenerateDhKeyStart(const uint8_t *pPubKey, const uint8_t *pPrivKey)
{
  uint8_t pubKey[LL_ECC_KEY_LEN * 2];
  uint8_t privKey[LL_ECC_KEY_LEN];

  WSF_ASSERT(llMathEccServiceCback != NULL);

  /* Reverse keys (to big endian). */
  llMathEccReverseCopy(pubKey, pPubKey, LL_ECC_KEY_LEN);
  llMathEccReverseCopy(pubKey + LL_ECC_KEY_LEN, pPubKey + LL_ECC_KEY_LEN, LL_ECC_KEY_LEN);
  llMathEccReverseCopy(privKey, pPrivKey, LL_ECC_KEY_LEN);

  /* Start shared secret generation. */
  uECC_set_rng(llMathEccRng);
  uECC_shared_secret_start(pubKey, privKey);

  llMathEccServiceCback(LL_MATH_ECC_OP_GENERATE_DH_KEY);
}

/*************************************************************************************************/
/*!
 *  \brief  Continue generating Diffie-Hellman key.
 *
 *  \return TRUE if Diffie-Hellman key generation complete.
 */
/*************************************************************************************************/
bool_t LlMathEccGenerateDhKeyContinue(void)
{
  WSF_ASSERT(llMathEccServiceCback != NULL);

  if (uECC_shared_secret_continue())
  {
    return TRUE;
  }

  llMathEccServiceCback(LL_MATH_ECC_OP_GENERATE_DH_KEY);
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Get results from generating Diffie-Hellman key.
 *
 *  \param  pDhKey      Storage for Diffie-Hellman key.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathEccGenerateDhKeyComplete(uint8_t *pDhKey)
{
  /* Complete shared secret generation. */
  uECC_shared_secret_complete(pDhKey);

  /* Reverse shared secret (to little endian). */
  llMathEccReverse(pDhKey, LL_ECC_KEY_LEN);
}
