/*
 *  Common functions for AES based algorithms for AES and CRYPTO hw modules.
 *
 *  Copyright (C) 2016 Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include "em_device.h"

#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) ||  \
  ( defined(AES_COUNT) && (AES_COUNT > 0) )

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aesdrv_internal.h"
#include "aesdrv_authencr.h"

#if defined(CRYPTO_PRESENT)
#include "cryptodrv_internal.h"
#include "aesdrv_common_crypto.h"
#include "em_crypto.h"
#endif

#include "em_assert.h"

/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Computes the length of the MIC (Message Integrity Code)
 *   for a given security level, as defined in IEEE Std 802.15.4-2006 table 95.
 *
 * @details
 *   The two LSBs of securityLevel encodes a MIC length of 0, 4, 8, or 16.
 *
 * @param[in] securityLevel
 *   Security level to use.
 *
 * @return
 *   The length of the MIC for the given @p securityLevel
 ******************************************************************************/
uint8_t AESDRV_CCMStar_LengthOfMIC(uint8_t securityLevel)
{
  securityLevel &= 3;
  if (securityLevel == 3)
  {
    securityLevel++;
  }
  return securityLevel * 4;
}

/***************************************************************************//**
 * @brief
 *   CCM block cipher mode encryption/decryption based on 128 bit AES.
 *
 * @details
 *   Please see http://en.wikipedia.org/wiki/CCM_mode for a general description
 *   of CCM.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param[in] pInputData
 *   If @p encrypt is true, pInputData is the 'P' (payload) parameter in CCM.
 *   I.e. the Payload data to encrypt. 
 *   If @p encrypt is false, pInputData is the 'C' (ciphertext) parameter in CCM.
 *   I.e. the ciphertext data to decrypt. 
 *
 * @param[out] pOutputData
 *   If @p encrypt is true, pOututData is the 'C' (ciphertext) parameter in CCM.
 *   I.e. the Ciphertext data as a result of encrypting the payload data.
 *   If @p encrypt is false, pOutputData is the 'P' (payload) parameter in CCM.
 *   I.e. the Payload data as a result of decrypting the ciphertext.
 *
 * @param[in] dataLength
 *   Length of data to be encrypted/decrypted, referred to as 'p' in CCM.
 *   Note that this does not include the length of the MIC which is specified
 *   with @p authTagLength.
 *
 * @param[in] pHdr
 *   The 'A' parameter in CCM.
 *   Header is used for MIC calculation.
 *   Must be at least @p hdrLength long.
 *
 * @param[in] hdrLength
 *   The 'a' parameter in CCM.
 *   Length of header.
 *
 * @param[in] pKey
 *   The 'K' parameter in CCM.
 *   Pointer to key buffer.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] pNonce
 *   The 'N' parameter in CCM.
 *   Pointer to the nonce, which must have length 15-authTagLength
 *   See @p authTagLength
 *
 * @param[in] nonceLength
 *   The length in bytes, of the @p pNonce, i.e. the 'N' parameter in CCM.
 *   Currently only nonce size equal to 13 bytes is supported.
 *
 * @param[in] pAuthTag
 *   The 'MIC' parameter in CCM.
 *   Pointer to the MIC buffer, which must have length @p authTagLength.
 *
 * @param[in] authTagLength
 *   The 't' parameter in CCM.
 *   The number of bytes used for the authentication tag.
 *   Possible values are 0, 4, 6, 8, 10, 12, 14, 16.
 *   Note that 0 is not a legal value in CCM, but is used for CCM*.
 *
 * @param[in] encrypt
 *   Set to true to run the generation-encryption process,
 *   false to run the decryption-verification process.
 *
 * @return
 *   ECODE_OK if success. Error code if failure.
 *   Encryption will always succeed.
 *   Decryption may fail if the authentication fails.
 ******************************************************************************/
Ecode_t AESDRV_CCM(AESDRV_Context_t*       pAesdrvContext,
                   const uint8_t*          pDataInput,
                         uint8_t*          pDataOutput,
                   const uint32_t          dataLength,
                   const uint8_t*          pHdr,
                   const uint32_t          hdrLength,
                   const uint8_t*          pKey,
                   const uint32_t          keyLength,
                   const uint8_t*          pNonce,
                   const uint32_t          nonceLength,
                         uint8_t*          pAuthTag,
                   const uint8_t           authTagLength,
                   const bool              encrypt)
{
  return AESDRV_CCM_Generalized( pAesdrvContext,
                                 pDataInput,
                                 pDataOutput,
                                 dataLength,
                                 pHdr,
                                 hdrLength,
                                 pKey,
                                 keyLength,
                                 pNonce,
                                 nonceLength,
                                 pAuthTag,
                                 authTagLength,
                                 encrypt,
                                 true);
}

/***************************************************************************//**
 * @brief
 *   CCM* block cipher mode encryption/decryption based on 128 bit AES.
 *
 * @details
 *   Please see IEEE Std 802.15.4-2006 Annex B for a description of CCM*.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param[in] pDataInput
 *   If @p encrypt is true, pDataInput is the plaintext.
 *   I.e. the payload data to encrypt. 
 *   If @p encrypt is false, pDataInput is the ciphertext.
 *   I.e. the ciphertext data to decrypt. 
 *
 * @param[out] pDataOutput
 *   If @p encrypt is true, pDataOutput is the ciphertext.
 *   I.e. the Ciphertext data as a result of encrypting the payload data.
 *   If @p encrypt is false, pDataOutput is the plaintext.
 *   I.e. the Payload data as a result of decrypting the ciphertext.
 *
 * @param[in] dataLength
 *   Length of data to be encrypted/decrypted, referred to as l(m) in CCM*.
 *   Note that this does not include the length of the MIC,
 *   so for decryption there are
 *   l(c) = @p dataLength + CCM_LengthOfMIC(securityLevel)
 *   bytes available in the buffer.
 *
 * @param[in] pHdr
 *   The 'a' parameter in CCM*.
 *   Header is used for MIC calculation.
 *   Must be at least @p hdrLength long.
 *
 * @param[in] hdrLength
 *   Length of header.
 *   Referred to as l(a) in CCM*
 *
 * @param[in] pKey
 *   The 'K' parameter in CCM*.
 *   Pointer to key to use.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] pNonce
 *   The 'N' parameter in CCM.
 *   Pointer to the nonce, which has length 13 bytes.
 *
 * @param[in] nonceLength
 *   The length in bytes, of the @p pNonce, i.e. the 'N' parameter in CCM*.
 *
 * @param[in] securityLevel
 *   Security level to use. See table 95 in IEEE Std 802.15.4-2006
 *   See also function CCM_LengthOfMIC
 *   Level 0: No encryption, no authentication
 *   Level 1: No encryption, M=4 bytes authentication tag
 *   Level 2: No encryption, M=8 bytes authentication tag
 *   Level 3: No encryption, M=16 bytes authentication tag
 *   Level 4: Encryption, no authentication
 *   Level 5: Encryption, M=4 bytes authentication tag
 *   Level 6: Encryption, M=8 bytes authentication tag
 *   Level 7: Encryption, M=16 bytes authentication tag
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure.
 *   Encryption will always succeed.
 *   Decryption may fail if the authentication fails.
 ******************************************************************************/
Ecode_t AESDRV_CCMStar(AESDRV_Context_t*       pAesdrvContext,
                       const uint8_t*          pDataInput,
                             uint8_t*          pDataOutput,
                       const uint32_t          dataLength,
                       const uint8_t*          pHdr,
                       const uint32_t          hdrLength,
                       const uint8_t*          pKey,
                       const uint32_t          keyLength,
                       const uint8_t*          pNonce,
                       const uint32_t          nonceLength,
                             uint8_t*          pAuthTag,
                       const uint8_t           securityLevel,
                       const bool              encrypt)
{
  return AESDRV_CCM_Generalized( pAesdrvContext,
                                 pDataInput,
                                 pDataOutput,
                                 dataLength,
                                 pHdr,
                                 hdrLength,
                                 pKey,
                                 keyLength,
                                 pNonce,
                                 nonceLength,
                                 pAuthTag,
                                 AESDRV_CCMStar_LengthOfMIC(securityLevel),
                                 encrypt,
                                 (securityLevel > 3));
}

#endif /* #if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) ||  \
          ( defined(AES_COUNT) && (AES_COUNT > 0) ) */
