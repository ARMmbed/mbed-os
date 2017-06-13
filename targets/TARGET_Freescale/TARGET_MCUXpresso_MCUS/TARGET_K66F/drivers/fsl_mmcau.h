/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_MMCAU_H_
#define _FSL_MMCAU_H_

#include "fsl_common.h"

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @addtogroup mmcau MMCAU Peripheral Driver Wrapper
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief AES: Performs an AES key expansion.
 *
 * This function performs an AES key expansion.
 *
 * @param key Pointer to input key (128, 192, 256 bits in length).
 * @param keySize Key size in bytes (16, 24, 32)
 * @param[out] keySch Pointer to key schedule output (44, 52, 60 longwords)
 * @note Table below shows the requirements for the MMCAU_AES_SetKey() function when using AES128, AES192, or AES256.@n
 * | [in] Key Size (bits)    |  [out] Key Schedule Size (32 bit data values)  |@n
 * | :---------------------: | :--------------------------------------------: |@n
 * |          128            |                      44                        |@n
 * |          192            |                      52                        |@n
 * |          256            |                      60                        |
 * @return Status of the operation. (kStatus_Success, kStatus_InvalidArgument, kStatus_Fail)
 */
status_t MMCAU_AES_SetKey(const uint8_t *key, const size_t keySize, uint8_t *keySch);

/*!
 * @brief AES: Encrypts a single 16 byte block.
 *
 *  This function encrypts a single 16-byte block for AES128, AES192, and AES256.
 *
 * @param in Pointer to 16-byte block of input plaintext.
 * @param keySch Pointer to key schedule (44, 52, 60 longwords).
 * @param aesRounds Number of AES rounds (10, 12, 14 = f(key_schedule)).
 * @param[out] out Pointer to 16-byte block of output ciphertext.
 * @note   Input and output blocks may overlap.@n
 *         Table below shows the requirements for the MMCAU_AES_EncryptEcb()/MMCAU_AES_DecryptEcb()
 *         function when using AES128, AES192 or AES256.@n
 * | Block Cipher | [in] Key Schedule Size (longwords) | [in] Number of AES rounds |@n
 * | :----------: | :--------------------------------: | :-----------------------: |@n
 * |    AES128    |               44                   |             10            |@n
 * |    AES192    |               52                   |             12            |@n
 * |    AES256    |               60                   |             14            |
 * @return Status of the operation. (kStatus_Success, kStatus_InvalidArgument, kStatus_Fail)
 */
status_t MMCAU_AES_EncryptEcb(const uint8_t *in, const uint8_t *keySch, uint32_t aesRounds, uint8_t *out);

/*!
 * @brief AES: Decrypts a single 16-byte block.
 *
 * This function decrypts a single 16-byte block for AES128, AES192, and AES256.
 *
 * @param in Pointer to 16-byte block of input ciphertext.
 * @param keySch Pointer to key schedule (44, 52, 60 longwords).
 * @param aesRounds Number of AES rounds (10, 12, 14 = f(key_schedule)).
 * @param[out] out Pointer to 16-byte block of output plaintext.
 * @note   Input and output blocks may overlap.@n
 *         Table below shows the requirements for the cau_aes_encrypt()/cau_aes_decrypt().
 *         function when using AES128, AES192 or AES256.@n
 * | Block Cipher | [in] Key Schedule Size (longwords) | [in] Number of AES rounds |@n
 * | :----------: | :--------------------------------: | :-----------------------: |@n
 * |    AES128    |               44                   |             10            |@n
 * |    AES192    |               52                   |             12            |@n
 * |    AES256    |               60                   |             14            |
 * @return Status of the operation. (kStatus_Success, kStatus_InvalidArgument, kStatus_Fail)
 */
status_t MMCAU_AES_DecryptEcb(const uint8_t *in, const uint8_t *keySch, uint32_t aesRounds, uint8_t *out);

/*!
 * @brief DES: Checks the key parity.
 *
 * This function checks the parity of a DES key.
 *
 * @param key 64-bit DES key with parity bits.
 * @return kStatus_Success No error.
 * @return kStatus_Fail Parity error.
 * @return kStatus_InvalidArgument Key argument is NULL.
 */
status_t MMCAU_DES_ChkParity(const uint8_t *key);

/*!
 * @brief DES: Encrypts a single 8-byte block.
 *
 * This function encrypts a single 8-byte block with the DES algorithm.
 *
 * @param in Pointer to 8-byte block of input plaintext.
 * @param key Pointer to 64-bit DES key with parity bits.
 * @param[out] out Pointer to 8-byte block of output ciphertext.
 * @note Input and output blocks may overlap.
 * @return Status of the operation. (kStatus_Success, kStatus_InvalidArgument, kStatus_Fail)
 */
status_t MMCAU_DES_EncryptEcb(const uint8_t *in, const uint8_t *key, uint8_t *out);

/*!
 * @brief DES: Decrypts a single 8-byte block.
 *
 * This function decrypts a single 8-byte block with the DES algorithm.
 *
 * @param in Pointer to 8-byte block of input ciphertext.
 * @param key Pointer to 64-bit DES key with parity bits.
 * @param[out] out Pointer to 8-byte block of output plaintext.
 * @note  Input and output blocks may overlap.
 * @return Status of the operation. (kStatus_Success, kStatus_InvalidArgument, kStatus_Fail)
 */
status_t MMCAU_DES_DecryptEcb(const uint8_t *in, const uint8_t *key, uint8_t *out);

/*!
 * @brief MD5: Initializes the MD5 state variables.
 *
 * This function initializes the MD5 state variables.
 * The output can be used as input to MMCAU_MD5_HashN().
 *
 * @param[out] md5State Pointer to 128-bit block of md5 state variables: a,b,c,d
 */
status_t MMCAU_MD5_InitializeOutput(uint32_t *md5State);

/*!
 * @brief MD5: Updates the MD5 state variables with n message blocks.
 *
 * This function updates the MD5 state variables for one or more input message blocks.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[in,out] md5State Pointer to 128-bit block of MD5 state variables: a, b, c, d.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_MD5_InitializeOutput() function must be called when starting a new hash.
 *        Useful when handling non-contiguous input message blocks.
 */
status_t MMCAU_MD5_HashN(const uint8_t *msgData, uint32_t numBlocks, uint32_t *md5State);

/*!
 * @brief MD5: Updates the MD5 state variables.
 *
 * This function updates the MD5 state variables for one or more input message blocks.
 * It starts a new hash as it internally calls MMCAU_MD5_InitializeOutput() first.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[out] md5State Pointer to 128-bit block of MD5 state variables: a, b, c, d.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_MD5_InitializeOutput() function is not required to be called
 *        as it is called internally to start a new hash.
 *        All input message blocks must be contiguous.
 */
status_t MMCAU_MD5_Update(const uint8_t *msgData, uint32_t numBlocks, uint32_t *md5State);

/*!
 * @brief SHA1: Initializes the SHA1 state variables.
 *
 * This function initializes the SHA1 state variables.
 * The output can be used as input to MMCAU_SHA1_HashN().
 *
 * @param[out] sha1State Pointer to 160-bit block of SHA1 state variables: a, b, c, d, e.
 */
status_t MMCAU_SHA1_InitializeOutput(uint32_t *sha1State);

/*!
 * @brief SHA1: Updates the SHA1 state variables with n message blocks.
 *
 * This function updates the SHA1 state variables for one or more input message blocks.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[in,out] sha1State Pointer to 160-bit block of SHA1 state variables: a, b, c, d, e.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_SHA1_InitializeOutput() function must be called when starting a new hash.
 *        Useful when handling non-contiguous input message blocks.
 */
status_t MMCAU_SHA1_HashN(const uint8_t *msgData, uint32_t numBlocks, uint32_t *sha1State);

/*!
 * @brief SHA1: Updates the SHA1 state variables.
 *
 * This function updates the SHA1 state variables for one or more input message blocks.
 * It starts a new hash as it internally calls MMCAU_SHA1_InitializeOutput() first.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[out] sha1State Pointer to 160-bit block of SHA1 state variables: a, b, c, d, e.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_SHA1_InitializeOutput() function is not required to be called
 *        as it is called internally to start a new hash.
 *        All input message blocks must be contiguous.
 */
status_t MMCAU_SHA1_Update(const uint8_t *msgData, uint32_t numBlocks, uint32_t *sha1State);

/*!
 * @brief SHA256: Initializes the SHA256 state variables.
 *
 * This function initializes the SHA256 state variables.
 * The output can be used as input to MMCAU_SHA256_HashN().
 *
 * @param[out] sha256State Pointer to 256-bit block of SHA2 state variables a, b, c, d, e, f, g, h.
 * @return kStatus_Success No error. CAU hardware support for SHA256 is present.
 * @return kStatus_Fail Error. CAU hardware support for SHA256 is not present.
 * @return kStatus_InvalidArgument Error. sha256State is NULL.
 */
status_t MMCAU_SHA256_InitializeOutput(uint32_t *sha256State);

/*!
 * @brief SHA256: Updates the SHA256 state variables with n message blocks.
 *
 * This function updates SHA256 state variables for one or more input message blocks.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[in,out] sha256State Pointer to 256-bit block of SHA2 state variables: a, b, c, d, e, f, g, h.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_SHA256_InitializeOutput() function must be called when starting a new hash.
 *        Useful when handling non-contiguous input message blocks.
 */
status_t MMCAU_SHA256_HashN(const uint8_t *input, uint32_t numBlocks, uint32_t *sha256State);

/*!
 * @brief SHA256: Updates SHA256 state variables.
 *
 * This function updates the SHA256 state variables for one or more input message blocks.
 * It starts a new hash as it internally calls cau_sha256_initialize_output() first.
 *
 * @param msgData Pointer to start of input message data.
 * @param numBlocks Number of 512-bit blocks to process.
 * @param[out] sha256State Pointer to 256-bit block of SHA2 state variables: a, b, c, d, e, f, g, h.
 * @note  Input message and digest output blocks must not overlap.
 *        The MMCAU_SHA256_InitializeOutput() function is not required to be called.
 *        as it is called internally to start a new hash.
 *        All input message blocks must be contiguous.
 */
status_t MMCAU_SHA256_Update(const uint8_t *input, uint32_t numBlocks, uint32_t *sha256State);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_MMCAU_H_ */
