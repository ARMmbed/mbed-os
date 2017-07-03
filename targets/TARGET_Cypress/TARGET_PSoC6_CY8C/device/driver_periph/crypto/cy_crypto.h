/***************************************************************************//**
* \file cy_crypto.h
* \version 1.0
*
* \brief
*  This file provides the public interface for the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

/**
* \defgroup group_crypto Cryptography (Crypto)
* \{
* The Crypto driver provides an API to perform
* cryptographic operations on user-designated data.
*
* Crypto supports: DES, TDES, AES (128, 192, 256 bits), SHA, CMAC-AES,
* HMAC, PRNG, TRNG, CRC, the utility functions for string management, and
* the accelerator for arithmetic on very large numbers (up to 4096 bits).
*
* Definitions
*
* <table class="doxtable">
*   <tr>
*     <th>Term</th>
*     <th>Definition</th>
*   </tr>
*
*   <tr>
*     <td>Plaintext</td>
*     <td>An unencrypted message</td>
*   </tr>
*
*   <tr>
*     <td>Ciphertext</td>
*     <td>An encrypted message</td>
*   </tr>
*
*   <tr>
*     <td>Block cipher</td>
*     <td>An encryption function for fixed-size blocks of data.
*     This function takes a fixed-size key and a block of plaintext data from
*     the message and encrypts it to generate ciphertext. Block ciphers are
*     reversible. The function performed on a block of encrypted data will decrypt it.</td>
*   </tr>
*
*   <tr>
*     <td>Block cipher mode</td>
*     <td>A mode of encrypting a message using Block ciphers for messages of arbitrary length.
*     The message is padded so that its length is an integer multiple of the block size.
*     ECB (Electronic Code Book), CBC (Cipher Block Chaining), and CFB (Cipher Feedback) are all
*     modes of using Block ciphers to create an encrypted message of arbitrary length.</td>
*   </tr>
*
*   <tr>
*     <td>Data Encryption Standard (DES)</td>
*     <td>An obsolete Block cipher supported  for legacy
*     reasons. It uses a 56-bit key and a 64-bit message block size.</td>
*   </tr>
*
*   <tr>
*     <td>Triple DES (3DES or TDES)</td>
*     <td>Uses the DES operation and three DES encryptions in sequence: encrypt
*     with DES with one 56-bit key, decrypt with a second 56-bit key, and then encrypt again with either
*     the first key or a third 56-bit key. The block size is 64-bits.</td>
*   </tr>
*
*   <tr>
*     <td>Advanced Encryption Standard (AES)</td>
*     <td>This Block cipher replaces DES and 3DES.
*     It is the current Block cipher standard. Keys can be 128, 192, or 256 bits
*     in length and the message block size is 128 bits. This is the US government standard.
*     AES is also used for message authentication.</td>
*   </tr>
*
*   <tr>
*     <td>Cipher-based Message Authentication Code (CMAC)</td>
*     <td>This method uses a key along with the message
*     to compute the MAC value using the AES Block Cipher algorithm.</td>
*   </tr>
*
*   <tr>
*     <td>Secure Hash Algorithm (SHA)</td>
*     <td>This function takes a message of the arbitrary length
*     and reduces it to a fixed-length residue or message digest after performing a series
*     of mathematically defined operations that practically guarantee that any change in
*     the message will change the hash value. It is used for message authentication by transmitting
*     a message with a hash value appended to it and recalculating the message hash value using
*     the same algorithm at the recipient's end. If the hashes differ, then the message is corrupted.</td>
*   </tr>
*
*   <tr>
*     <td>Message Authentication Code (MAC)</td>
*     <td>MACs are used to verify that a received message has not been altered.
*     This is done by first computing a MAC value at the sender's end and appending it to the transmitted message.
*     When the message is received, the MAC is computed again and checked against the MAC value transmitted with
*     the message. If they do not match, the message has been altered. Either a Hash algorithm (such as SHA)
*     or a Block Cipher (such as AES) can be used to produce the MAC value. Keyed MAC schemes use a Secret Key
*     along with the message, thus the Key value must be known to be able to compute the MAC value.</td>
*   </tr>
*
*   <tr>
*     <td>Hash Message Authentication Code (HMAC)</td>
*     <td>This method uses a Key along with the message to compute
*     the MAC value using a Hash algorithm.</td>
*   </tr>
*
*   <tr>
*     <td>Pseudo Random Number Generator (PRNG)</td>
*     <td>Based on Linear Feedback Shift Registers that generate a sequence
*     starting from a non-zero seed.</td>
*   </tr>
*
*   <tr>
*     <td>True Random Number Generator (TRNG)</td>
*     <td>A block that generates a number that is statistically random and based
*     on some physical random variation. The number cannot be duplicated by running the process again.</td>
*   </tr>
*
*   <tr>
*     <td>Symmetric Key Cryptography</td>
*     <td>Uses a common, known key to encrypt and decrypt messages (a shared secret between sender
*     and receiver). An efficient method used for encrypting and decrypting messages once the authenticity
*     of the other party has been established. DES (now obsolete), 3DES, and AES (currently used)
*     are well known symmetric cryptography methods.</td>
*   </tr>
*
*   <tr>
*     <td>Asymmetric Key Cryptography</td>
*     <td>Also referred to as Public Key encryption. Someone who wishes to receive a message, publishes
*     a very large public key (up to 4096 bits currently), which is one of two prime factors of a very large
*     number. The other prime factor is the private key of the recipient and a secret. Someone wishing
*     to send a message to the publisher of the public key encrypts the message with the public key. This message
*     can now be decrypted only with the private key (the other prime factor held secret by the recipient).
*     The message is now sent over any channel to the recipient who can decrypt it with the private, secret, key.
*     The same process is used to send messages to the sender of the original message. The asymmetric cryptography
*     relies on the mathematical impracticality (usually related to the processing power available at any given time)
*     of factoring the keys. Common, computationally intensive, asymmetric algorithms are RSA and ECC.
*     The public key is described by the pair (n, e) where n is a product of two randomly chosen primes p and q.
*     The exponent e is a random integer 1 < e < Q where Q = (p-1) (q-1). The private key d is uniquely
*     defined by the integer 1 < d < Q such that ed congruent to 1 (mod Q ).</td>
*   </tr>
* </table>
*
* \section group_crypto_configuration Configuration Considerations
*
* Crypto has one configuration structure:
* \ref cy_stc_crypto_config_t
*
* \section group_crypto_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA rule</th>
*     <th>Rule Class (Required/ Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>A</td>
*     <td>A cast should not be performed between a pointer to the void to a pointer to the object type.</td>
*     <td>The cast from the void to pointer and vice versa is used to transmit data via the IPC channel
*         by exchanging the pointer. We exchange only one pointer, so there is no way to avoid this cast.</td>
*   </tr>
*   <tr>
*     <td>14.4</td>
*     <td>R</td>
*     <td>Cast from a pointer to the void to a pointer to the object type.</td>
*     <td>The cast from the void to pointer and vice versa is used to transmit data via the IPC channel
*         by exchanging the pointer. We exchange only one pointer, so there is no way to avoid this cast.</td>
*   </tr>
*   <tr>
*     <td>16.7</td>
*     <td>R</td>
*     <td>The object addressed by the pointer parameter '%s' is not modified
*          and so the pointer can be of type 'pointer to const'.</td>
*     <td>Data addressed by the pointer is modified by Crypto hardware, out of the compiler scope, so
*         it can't be the pointer to const.</td>
*   </tr>
*
* \section group_crypto_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \section group_crypto_memory_usage Memory Usage
*
* Flash and RAM memory usage for the Crypto driver is shown in the table below.
* The usage is measured for GCC 5.4.1 in a release build with "-Os" optimization
* for Cortex-M0+ and Cortex-M4 cores.
*
* <table class="doxtable">
*   <tr><th>Core</th><th>Flash Size</th><th>RAM Size</th></tr>
*
*   <tr> <td>CM0p</td> <td>4294</td> <td>5</td> </tr>
*   <tr> <td>CM4</td>  <td> 692</td> <td>4</td> </tr>
* </table>
*
* \defgroup group_crypto_macro Macro
* \defgroup group_crypto_functions Functions
* \defgroup group_crypto_data_structures Data Structures
* \defgroup group_crypto_enums Enumerated Types
*
*/

#if !defined(CY_CRYPTO_H)
#define CY_CRYPTO_H


#include <stddef.h>
#include <stdbool.h>
#include "cy_device_headers.h"
#include "crypto/cy_crypto_common.h"

#if (CPUSS_CRYPTO_PRESENT == 1)


#if defined(__cplusplus)
extern "C" {
#endif


/**
* \addtogroup group_crypto_functions
* \{
*/

cy_en_crypto_status_t Cy_Crypto_Init(cy_stc_crypto_context_t *cryptoContext, cy_stc_crypto_config_t const *configStruct);

cy_en_crypto_status_t Cy_Crypto_DeInit(cy_stc_crypto_context_t *cryptoContext, cy_stc_crypto_config_t const *configStruct);

cy_en_crypto_status_t Cy_Crypto_Enable(cy_stc_crypto_context_t *cryptoContext);

cy_en_crypto_status_t Cy_Crypto_Disable(cy_stc_crypto_context_t *cryptoContext);

cy_en_crypto_status_t Cy_Crypto_Sync(cy_stc_crypto_context_t const *cryptoContext, bool isBlocking);

cy_en_crypto_status_t Cy_Crypto_GetErrorStatus(const cy_stc_crypto_context_t *cryptoContext,
                                               cy_stc_crypto_hw_error_t *hwErrorCause);

#if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))
cy_en_crypto_status_t Cy_Crypto_Prng_Init(cy_stc_crypto_context_t *cryptoContext,
                                          cy_stc_crypto_context_prng_t *cryptoPrngContext,
                                          const uint32_t lfsr32InitState,
                                          const uint32_t lfsr31InitState,
                                          const uint32_t lfsr29InitState);

cy_en_crypto_status_t Cy_Crypto_Prng_Generate(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_prng_t *cryptoPrngContext,
                                               uint32_t const max,
                                              uint32_t *rndNumPtr);
#endif /* #if ((CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1))
cy_en_crypto_status_t Cy_Crypto_Aes_Init(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_aes_t *cryptoAesContext,
                                         uint32_t *keyPtr,
                                         cy_en_crypto_aes_key_length_t keyLength);

#if (CY_CRYPTO_USER_AES_ECB == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Ecb_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t *dstBlockPtr,
                                            uint32_t *srcBlockPtr);
#endif /* #if (CY_CRYPTO_USER_AES_ECB == 1) */

#if (CY_CRYPTO_USER_AES_CBC == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Cbc_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CBC == 1) */

#if (CY_CRYPTO_USER_AES_CFB == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Cfb_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *ivPtr,
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CFB == 1) */

#if (CY_CRYPTO_USER_AES_CTR == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Ctr_Run(cy_stc_crypto_context_t *cryptoContext,
                                            cy_stc_crypto_context_aes_t *cryptoAesContext,
                                            cy_en_crypto_dir_mode_t dirMode,
                                            uint32_t srcSize,
                                            uint32_t *srcOffset,
                                            uint32_t nonceCounter[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t streamBlock[CY_CRYPTO_AES_BLOCK_SIZE / 8u],
                                            uint32_t *dstPtr,
                                            uint32_t *srcPtr);
#endif /* #if (CY_CRYPTO_USER_AES_CTR == 1) */

#if (CY_CRYPTO_USER_CMAC == 1)
cy_en_crypto_status_t Cy_Crypto_Aes_Cmac_Run(cy_stc_crypto_context_t *cryptoContext,
                                             cy_stc_crypto_context_aes_t *cryptoAesContext,
                                             uint32_t *srcPtr,
                                             uint32_t srcSize,
                                             uint32_t *keyPtr,
                                             cy_en_crypto_aes_key_length_t keyLength,
                                             uint32_t *cmacPtr);
#endif /* #if (CY_CRYPTO_USER_CMAC == 1) */
#endif /* #if ((CPUSS_CRYPTO_AES == 1) && (CY_CRYPTO_USER_AES_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1))
cy_en_crypto_status_t Cy_Crypto_Sha_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_sha_t *cryptoShaContext,
                                        uint32_t *messagePtr,
                                        uint32_t  messageSize,
                                        uint32_t *digestPtr,
                                        cy_en_crypto_sha_mode_t mode);
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_SHA_ENABLE == 1)) */

#if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1))
cy_en_crypto_status_t Cy_Crypto_Hmac_Run(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_sha_t *cryptoShaContext,
                                         uint32_t *hmacPtr,
                                         uint32_t *messagePtr,
                                         uint32_t  messageSize,
                                         uint32_t *keyPtr,
                                         uint32_t keyLength,
                                         cy_en_crypto_sha_mode_t mode);
#endif /* #if ((CPUSS_CRYPTO_SHA == 1) && (CY_CRYPTO_USER_HMAC == 1)) */

#if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1))
cy_en_crypto_status_t Cy_Crypto_Str_MemCpy(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void *dstPtr,
                                           void const *srcPtr,
                                           uint16_t size);

cy_en_crypto_status_t Cy_Crypto_Str_MemSet(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void *dstPtr,
                                           uint8_t data,
                                           uint16_t size);

cy_en_crypto_status_t Cy_Crypto_Str_MemCmp(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void const *src0Ptr,
                                           void const *src1Ptr,
                                           uint16_t size,
                                           uint32_t *resultPtr);

cy_en_crypto_status_t Cy_Crypto_Str_MemXor(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_str_t *cryptoMemContext,
                                           void const *src0Ptr,
                                           void const *src1Ptr,
                                           void *dstPtr,
                                           uint16_t size);
#endif /* #if ((CPUSS_CRYPTO_STR == 1) && (CY_CRYPTO_USER_STR == 1)) */

#if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))
cy_en_crypto_status_t Cy_Crypto_Crc_Init(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_crc_t *cryptoCrcContext,
                                         uint32_t polynomial,
                                         uint8_t  dataReverse,
                                         uint8_t  dataXor,
                                         uint8_t  remReverse,
                                         uint32_t remXor);

cy_en_crypto_status_t Cy_Crypto_Crc_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_crc_t *cryptoCrcContext,
                                        void *dataPtr,
                                        uint16_t  dataSize,
                                        uint32_t *crcPtr,
                                        uint32_t  lfsrInitState);
#endif /* #if ((CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))
cy_en_crypto_status_t Cy_Crypto_Trng_Generate(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_trng_t *cryptoTrngContext,
                                              uint32_t  GAROPol,
                                              uint32_t  FIROPol,
                                              uint32_t  max,
                                              uint32_t *rndNumPtr);
#endif /* #if ((CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1))
cy_en_crypto_status_t Cy_Crypto_Des_Run(cy_stc_crypto_context_t *cryptoContext,
                                        cy_stc_crypto_context_des_t *cryptoDesContext,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint32_t *keyPtr,
                                        uint32_t *dstBlockPtr,
                                        uint32_t *srcBlockPtr);

cy_en_crypto_status_t Cy_Crypto_Tdes_Run(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_des_t *cryptoDesContext,
                                         cy_en_crypto_dir_mode_t dirMode,
                                         uint32_t *keyPtr,
                                         uint32_t *dstBlockPtr,
                                         uint32_t *srcBlockPtr);
#endif /* #if ((CPUSS_CRYPTO_DES == 1) && (CY_CRYPTO_USER_DES == 1)) */

#if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))
cy_en_crypto_status_t Cy_Crypto_Rsa_Proc(cy_stc_crypto_context_t *cryptoContext,
                                         cy_stc_crypto_context_rsa_t *cryptoRsaContext,
                                         cy_stc_crypto_rsa_pub_key_t const *pubKeyPtr,
                                         uint32_t const *messageePtr,
                                         uint32_t messageLength,
                                         uint32_t const *processedMessagePtr);

cy_en_crypto_status_t Cy_Crypto_Rsa_CalcCoefs(cy_stc_crypto_context_t *cryptoContext,
                                              cy_stc_crypto_context_rsa_t *cryptoRsaContext,
                                              cy_stc_crypto_rsa_pub_key_t const *pubKeyPtr);
#endif /* #if ((CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#if (CY_CRYPTO_USER_PKCS1_5 == 1)
cy_en_crypto_status_t Cy_Crypto_Rsa_Verify(cy_stc_crypto_context_t *cryptoContext,
                                           cy_stc_crypto_context_rsa_ver_t *cryptoRsaVerContext,
                                           cy_en_crypto_rsa_ver_result_t *verResult,
                                           cy_en_crypto_sha_mode_t digestType,
                                           uint32_t const *digestPtr,
                                           uint32_t const *decryptedSignaturePtr,
                                           uint32_t decryptedSignatureLength);

void Cy_Crypto_Rsa_InvertEndianness(void *inArrPtr, uint32_t byteSize);
#endif /* #if (CY_CRYPTO_USER_PKCS1_5 == 1) */


/** \} group_crypto_functions */


#if defined(__cplusplus)
}
#endif

#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* (CY_CRYPTO_H) */

/** \} group_crypto */

/* [] END OF FILE */
