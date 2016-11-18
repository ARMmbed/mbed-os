/*
 *  Internal interface for AES based functions
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
#ifndef __SILICON_LABS_AESDRV_INTERNAL_H
#define __SILICON_LABS_AESDRV_INTERNAL_H

#include "em_device.h"

#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
  ( defined(AES_COUNT) && (AES_COUNT > 0) )

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aesdrv.h"
#include "ecode.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *   Initialize an AESDRV context.
 *
 * @details
 *   This function initializes an AESDRV context structure to default values.
 *   In case of CRYPTO HW module use CRYPTO(0) as default device (for parts
 *   with multiple CRYPTO instances), and set data I/O mode to
 *   @ref aesdrvIoModeCore (i.e. core cpu moves data, not DMA or BUFC).
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_Init(AESDRV_Context_t*  pAesdrvContext);

/***************************************************************************//**
 * @brief
 *   DeInitializes AESDRV context.
 *
 * @details
 *   This function deinitializes an AESDRV context by clearing the values and
 *   releasing the resources in use. 
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_DeInit(AESDRV_Context_t*  pAesdrvContext);

/***************************************************************************//**
 * @brief
 *   Set the device instance of an AESDRV context.
 *
 * @details
 *   This function sets the AES/CRYPTO device instance of an AESDRV context.
 *   Subsequent calls to AESDRV API functions with this context will use the
 *   new AES/CRYPTO device instance.
 *
 * @param[in] pAesdrvContext
 *   AESDRV device context.
 *  
 * @param[in] devno
 *   AES/CRYPTO hardware device instance to use.
 *  
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_SetDeviceInstance(AESDRV_Context_t*  pAesdrvContext,
                                 unsigned int       devno);
  
/***************************************************************************//**
 * @brief
 *   Set the I/O mode of an AESDRV context.
 *
 * @details
 *   This function sets the data I/O mode of an AESDRV context.
 *
 * @param[in] pAesdrvContext
 *   AESDRV device context.
 *  
 * @param[in] ioMode
 *   I/O mode (Core CPU, DMA or BUFC).
 *  
 * @param[in] ioModeSpecific
 *   I/O mode specific configuration @ref AESDRV_IoModeSpecific_t.
 *  
 * @warning
 *   If BUFC is selected (@ref aesdrvIoModeBufc), this function does not enable
 *   the BUFC clock and does not do any global BUFC initialization. I.e. the
 *   user is responsible for performing BUFC initialization prior to calling
 *   this function.
 *   If DMA is selected (@ref aesdrvIoModeDma), this function performs full
 *   DMA driver initialization by calling DMADRV_Init (non-destructive) and
 *   allocates DMA channel resources to be used by AESDRV.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_SetIoMode
(
 AESDRV_Context_t*        pAesdrvContext,
 AESDRV_IoMode_t          ioMode,
 AESDRV_IoModeSpecific_t* ioModeSpecific
 );

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
/***************************************************************************//**
 * @brief
 *   Setup the asynchronous mode of an AESDRV context.
 *
 * @details
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   AESDRV context.
 *   In order to enable, the user must set the @p pAsynchContext parameter to
 *   point to an asynchronous data context structure, and set the corresponding
 *   @p cipherMode. All subsequent calls to AESDRV API functions with the
 *   specified context will behave asynchronously, i.e. initiate the hardware
 *   to execute the operation and return as soon as possible. The user may
 *   specify a callback function by setting the @p asynchCallback parameter
 *   which will called when the operation has completed.
 *   In order to disable, the user must set the @p pAsynchContext parameter
 *   to NULL. All subsequent calls to AESDRV API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * @param[in] pAesdrvContext
 *   AESDRV device context.
 *  
 * @param[in] cipherMode
 *   Cipher mode corresponding to the intended asynchronous operation.
 *  
 * @param[in] pAsynchContext
 *   Pointer to an asynchronous context structure corresponding to the
 *   cipher mode specified by @p cipherMode, or NULL in order to disable
 *   asynchronous mode. In order to enable asynchronous mode, @p pAsynchContext
 *   must be one of
 *   @li @ref AESDRV_BlockCipherAsynchContext_t
 *   @li @ref AESDRV_CCM_AsynchContext_t
 *   @li @ref AESDRV_GCM_AsynchContext_t
 *   @li @ref AESDRV_CMAC_AsynchContext_t.
 *   For BLE optimized CCM @ref AESDRV_CCMBLE, use
 *   @ref AESDRV_CCM_AsynchContext_t.
 *
 * @param[in]  asynchCallback
 *   If non-NULL, this function will operate in asynchronous mode by starting
 *   the AES operation and return immediately (non-blocking API). When the AES
 *   operation has completed, the ascynchCallback function will be called.
 *   If NULL, this function will operate in synchronous mode, and block until
 *   the AES operation has completed.
 *
 * @param[in]  asynchCallbackArgument
 *   User defined parameter to be sent to callback function.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_SetAsynchMode
(
 AESDRV_Context_t*       pAesdrvContext,
 AESDRV_CipherMode_t     cipherMode,
 void*                   pAsynchContext,
 AESDRV_AsynchCallback_t asynchCallback,
 void*                   asynchCallbackArgument
 );
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

/***************************************************************************//**
 * @brief
 *   Set the AES encryption key.
 *
 * @details
 *   This functions sets up a 128 or 256 bit key to use for encryption and
 *   decryption in subsequent calls to AESDRV.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param[in] pKey
 *   Pointer to buffer including the AES key.
 *
 * @param[in] keyLength
 *   The length (in bytes) of the AES key. I.e. 16 bytes = 128 bits,
 *   32 bytes = 256 bits key length.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_SetKey(AESDRV_Context_t* pAesdrvContext,
                      const uint8_t*    pKey,
                      uint32_t          keyLength);
  
/***************************************************************************//**
 * @brief
 *   Generate 128 bit decryption key from 128 bit encryption key. The decryption
 *   key is used for some cipher modes when decrypting.
 *
 * @details
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param[out] pKeyOut
 *   Buffer to place 128 bit decryption key. Must be at least 16 bytes long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] pKeyIn
 *   Buffer holding 128 bit encryption key. Must be at least 16 bytes long.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_DecryptKey128(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*          pKeyOut,
                             const uint8_t*    pKeyIn);

/***************************************************************************//**
 * @brief
 *   Generate 256 bit decryption key from 256 bit encryption key. The decryption
 *   key is used for some cipher modes when decrypting.
 *
 * @details
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param[out] pKeyOut
 *   Buffer to place 256 bit decryption key. Must be at least 32 bytes long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] pKeyIn
 *   Buffer holding 256 bit encryption key. Must be at least 32 bytes long.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_DecryptKey256(AESDRV_Context_t* pAesdrvContext,
                             uint8_t*          pKeyOut,
                             const uint8_t*    pKeyIn);

/***************************************************************************//**
 * @brief
 *   Cipher-block chaining (CBC) cipher mode encryption/decryption, 128 bit key.
 *
 * @details
 *   Encryption:
 * @verbatim
 *           Plaintext                  Plaintext
 *               |                          |
 *               V                          V
 * InitVector ->XOR        +-------------->XOR
 *               |         |                |
 *               V         |                V
 *       +--------------+  |        +--------------+
 * Key ->| Block cipher |  |  Key ->| Block cipher |
 *       |  encryption  |  |        |  encryption  |
 *       +--------------+  |        +--------------+
 *               |---------+                |
 *               V                          V
 *           Ciphertext                 Ciphertext
 * @endverbatim
 *   Decryption:
 * @verbatim
 *         Ciphertext                 Ciphertext
 *              |----------+                |
 *              V          |                V
 *       +--------------+  |        +--------------+
 * Key ->| Block cipher |  |  Key ->| Block cipher |
 *       |  decryption  |  |        |  decryption  |
 *       +--------------+  |        +--------------+
 *               |         |                |
 *               V         |                V
 * InitVector ->XOR        +-------------->XOR
 *               |                          |
 *               V                          V
 *           Plaintext                  Plaintext
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   When doing encryption, this is the 128 bit encryption key. When doing
 *   decryption, this is the 128 bit decryption key. The decryption key may
 *   be generated from the encryption key with AESDRV_DecryptKey128().
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CBC128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      );
  
/***************************************************************************//**
 * @brief
 *   Cipher-block chaining (CBC) cipher mode encryption/decryption, 256 bit key.
 *
 * @details
 *   Please see AESDRV_CBC128() for CBC figure.
 *
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   When doing encryption, this is the 256 bit encryption key. When doing
 *   decryption, this is the 256 bit decryption key. The decryption key may
 *   be generated from the encryption key with AESDRV_DecryptKey256().
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CBC256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      );
  
/***************************************************************************//**
 * @brief
 *   Cipher feedback (CFB) cipher mode encryption/decryption, 128 bit key.
 *
 * @details
 *   Encryption:
 * @verbatim
 *           InitVector    +----------------+
 *               |         |                |
 *               V         |                V
 *       +--------------+  |        +--------------+
 * Key ->| Block cipher |  |  Key ->| Block cipher |
 *       |  encryption  |  |        |  encryption  |
 *       +--------------+  |        +--------------+
 *               |         |                |
 *               V         |                V
 *  Plaintext ->XOR        |   Plaintext ->XOR
 *               |---------+                |
 *               V                          V
 *           Ciphertext                 Ciphertext
 * @endverbatim
 *   Decryption:
 * @verbatim
 *          InitVector     +----------------+
 *               |         |                |
 *               V         |                V
 *       +--------------+  |        +--------------+
 * Key ->| Block cipher |  |  Key ->| Block cipher |
 *       |  encryption  |  |        |  encryption  |
 *       +--------------+  |        +--------------+
 *               |         |                |
 *               V         |                V
 *              XOR<- Ciphertext           XOR<- Ciphertext
 *               |                          |
 *               V                          V
 *           Plaintext                  Plaintext
 * @endverbatim
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   128 bit encryption key is used for both encryption and decryption modes.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      );
  
/***************************************************************************//**
 * @brief
 *   Cipher feedback (CFB) cipher mode encryption/decryption, 256 bit key.
 *
 * @details
 *   Please see AESDRV_CFB128() for CFB figure.
 *
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   256 bit encryption key is used for both encryption and decryption modes.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv,
                      bool              encrypt
                      );
  
/***************************************************************************//**
 * @brief
 *   Counter (CTR) cipher mode encryption/decryption, 128 bit key.
 *
 * @details
 *   Encryption:
 * @verbatim
 *           Counter                    Counter
 *              |                          |
 *              V                          V
 *       +--------------+           +--------------+
 * Key ->| Block cipher |     Key ->| Block cipher |
 *       |  encryption  |           |  encryption  |
 *       +--------------+           +--------------+
 *              |                          |
 * Plaintext ->XOR            Plaintext ->XOR
 *              |                          |
 *              V                          V
 *         Ciphertext                 Ciphertext
 * @endverbatim
 *   Decryption:
 * @verbatim
 *           Counter                    Counter
 *              |                          |
 *              V                          V
 *       +--------------+           +--------------+
 * Key ->| Block cipher |     Key ->| Block cipher |
 *       |  encryption  |           |  encryption  |
 *       +--------------+           +--------------+
 *               |                          |
 * Ciphertext ->XOR           Ciphertext ->XOR
 *               |                          |
 *               V                          V
 *           Plaintext                  Plaintext
 * @endverbatim
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   128 bit encryption key.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] ctr
 *   128 bit initial counter value.
 *   If supported by the device (currently only EFM32 AES hardware module) the
 *   user can point @p ctrCallback to a function which will be called after each
 *   AES block encoding in order to update the counter.
 *
 * @param[in] ctrCallback
 *   Callback function used to update counter value.
 *   Currently only supported by EFM32 AES hardware module. If NULL then
 *   AES_CTRUpdate32Bit from emlib will be used.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CTR128(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback
                      );
  
/***************************************************************************//**
 * @brief
 *   Counter (CTR) cipher mode encryption/decryption, 256 bit key.
 *
 * @details
 *   Please see AES_CTR128() for CTR figure.
 *
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   256 bit encryption key.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] ctr
 *   128 bit initial counter value.
 *   If supported by the device (currently only EFM32 AES hardware module) the
 *   user can point @p ctrCallback to a function which will be called after each
 *   AES block encoding in order to update the counter.
 *
 * @param[in] ctrCallback
 *   Callback function used to update counter value.
 *   Currently only supported by EFM32 AES hardware module. If NULL then
 *   AES_CTRUpdate32Bit from emlib will be used.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CTR256(AESDRV_Context_t*    pAesdrvContext,
                      uint8_t*             out,
                      const uint8_t*       in,
                      unsigned int         len,
                      const uint8_t*       key,
                      uint8_t*             ctr,
                      AESDRV_CtrCallback_t ctrCallback
                      );

/***************************************************************************//**
 * @brief
 *   Electronic Codebook (ECB) cipher mode encryption/decryption, 128 bit key.
 *
 * @details
 *   Encryption:
 * @verbatim
 *          Plaintext                  Plaintext
 *              |                          |
 *              V                          V
 *       +--------------+           +--------------+
 * Key ->| Block cipher |     Key ->| Block cipher |
 *       |  encryption  |           |  encryption  |
 *       +--------------+           +--------------+
 *              |                          |
 *              V                          V
 *         Ciphertext                 Ciphertext
 * @endverbatim
 *   Decryption:
 * @verbatim
 *         Ciphertext                 Ciphertext
 *              |                          |
 *              V                          V
 *       +--------------+           +--------------+
 * Key ->| Block cipher |     Key ->| Block cipher |
 *       |  decryption  |           |  decryption  |
 *       +--------------+           +--------------+
 *              |                          |
 *              V                          V
 *          Plaintext                  Plaintext
 * @endverbatim
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   When doing encryption, this is the 128 bit encryption key. When doing
 *   decryption, this is the 128 bit decryption key. The decryption key may
 *   be generated from the encryption key with AESDRV_DecryptKey128().
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_ECB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt
                      );

/***************************************************************************//**
 * @brief
 *   Electronic Codebook (ECB) cipher mode encryption/decryption, 256 bit key.
 *
 * @details
 *   Please see AESDRV_ECB128() for ECB figure.
 *
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   When doing encryption, this is the 256 bit encryption key. When doing
 *   decryption, this is the 256 bit decryption key. The decryption key may
 *   be generated from the encryption key with AESDRV_DecryptKey256().
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in] encrypt
 *   Set to true to encrypt, false to decrypt.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_ECB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      bool              encrypt
                      );

/***************************************************************************//**
 * @brief
 *   Output feedback (OFB) cipher mode encryption/decryption, 128 bit key.
 *
 * @details
 *   Encryption:
 * @verbatim
 *          InitVector    +----------------+
 *              |         |                |
 *              V         |                V
 *       +--------------+ |        +--------------+
 * Key ->| Block cipher | |  Key ->| Block cipher |
 *       |  encryption  | |        |  encryption  |
 *       +--------------+ |        +--------------+
 *              |         |                |
 *              |---------+                |
 *              V                          V
 * Plaintext ->XOR            Plaintext ->XOR
 *              |                          |
 *              V                          V
 *         Ciphertext                 Ciphertext
 * @endverbatim
 *   Decryption:
 * @verbatim
 *          InitVector    +----------------+
 *              |         |                |
 *              V         |                V
 *       +--------------+ |        +--------------+
 * Key ->| Block cipher | |  Key ->| Block cipher |
 *       |  encryption  | |        |  encryption  |
 *       +--------------+ |        +--------------+
 *              |         |                |
 *              |---------+                |
 *              V                          V
 * Ciphertext ->XOR           Ciphertext ->XOR
 *              |                          |
 *              V                          V
 *          Plaintext                  Plaintext
 * @endverbatim
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   128 bit encryption key.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_OFB128(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv
                      );

/***************************************************************************//**
 * @brief
 *   Output feedback (OFB) cipher mode encryption/decryption, 256 bit key.
 *
 * @details
 *   Please see AES_OFB128() for OFB figure.
 *
 *   Please refer to general comments on layout and byte ordering of parameters.
 *
 * @param[in] pAesdrvContext
 *   Pointer to AES block cipher context structure.
 *
 * @param[out] out
 *   Buffer to place encrypted/decrypted data. Must be at least @p len long. It
 *   may be set equal to @p in, in which case the input buffer is overwritten.
 *
 * @param[in] in
 *   Buffer holding data to encrypt/decrypt. Must be at least @p len long.
 *
 * @param[in] len
 *   Number of bytes to encrypt/decrypt. Must be a multiple of 16.
 *
 * @param[in] key
 *   256 bit encryption key.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in,out] iv
 *   128 bit initalization vector. The updated vector value for the last block
 *   is returned.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_OFB256(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          out,
                      const uint8_t*    in,
                      unsigned int      len,
                      const uint8_t*    key,
                      uint8_t*          iv
                      );

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
uint8_t AESDRV_CCMStar_LengthOfMIC(uint8_t securityLevel);

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
 * @param[in] pDataInput
 *   If @p encrypt is true, pDataInput is the 'P' (payload) parameter in CCM.
 *   I.e. the Payload data to encrypt. 
 *   If @p encrypt is false, pDataInput is the 'C' (ciphertext) parameter in CCM.
 *   I.e. the ciphertext data to decrypt. 
 *
 * @param[out] pDataOutput
 *   If @p encrypt is true, pOututData is the 'C' (ciphertext) parameter in CCM.
 *   I.e. the Ciphertext data as a result of encrypting the payload data.
 *   If @p encrypt is false, pDataOutput is the 'P' (payload) parameter in CCM.
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
 *   Pointer to security key.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
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
 * @param[in/out] pAuthTag
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
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CCM(AESDRV_Context_t* pAesdrvContext,
                   const uint8_t*    pDataInput,
                         uint8_t*    pDataOutput,
                   const uint32_t    dataLength,
                   const uint8_t*    pHdr,
                   const uint32_t    hdrLength,
                   const uint8_t*    pKey,
                   const uint32_t    keyLength,
                   const uint8_t*    pNonce,
                   const uint32_t    nonceLength,
                         uint8_t*    pAuthTag,
                   const uint8_t     authTagLength,
                   const bool        encrypt
                   );

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
 *   Pointer to security key.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM*.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] pNonce
 *   The 'N' parameter in CCM.
 *   Pointer to the nonce, which has length 13 bytes.
 *
 * @param[in] nonceLength
 *   The length in bytes, of the @p pNonce, i.e. the 'N' parameter in CCM*.
 *
 * @param[in/out] pAuthTag
 *   The 'MIC' parameter in CCM.
 *   Pointer to the MIC buffer, which must have length @p authTagLength.
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
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_CCMStar(AESDRV_Context_t* pAesdrvContext,
                       const uint8_t*    pDataInput,
                             uint8_t*    pDataOutput,
                       const uint32_t    dataLength,
                       const uint8_t*    pHdr,
                       const uint32_t    hdrLength,
                       const uint8_t*    pKey,
                       const uint32_t    keyLength,
                       const uint8_t*    pNonce,
                       const uint32_t    nonceLength,
                             uint8_t*    pAuthTag,
                       const uint8_t     securityLevel,
                       const bool        encrypt
                       );
  
/***************************************************************************//**
 * @brief
 *  CCM optimized for BLE
 *
 * @details
 *  This function is an implementation of CCM optimized for Bluetooth Low Energy
 *  (BLE). This function assumes fixed header size (1 byte),
 *  fixed authentication tag (4bytes), fixed length field size (2 bytes)
 *
 * @param[in] pAesdrvContext
 *   Pointer to AESDRV context structure.
 *
 * @param pData
 *  Pointer to data
 *
 * @param dataLength
 *  length of data (max. 27)
 *
 * @param hdr
 *  1 byte header
 *
 * @param pKey
 *  Pointer to 128 bits (16 byte) Security Key.
 *  On devices supporting key buffering this argument can be NULL, and if so,
 *  the current key will used. The key can be explicitly set with
 *  @ref AESDRV_SetKey.
 *
 * @param pNonce
 *  13 byte nonce
 *
 * @param[in/out] pAuthTag
 *   The 'MIC' parameter in CCM.
 *   Pointer to the MIC buffer, which must have length of 4 bytes.
 *
 * @param encrypt
 *  true - encrypt
 *  false - decrypt
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 */
Ecode_t AESDRV_CCMBLE(AESDRV_Context_t* pAesdrvContext,
                      uint8_t*          pData,
                      const uint32_t    dataLength,
                      uint8_t           hdr,
                      const uint8_t*    pKey,
                      const uint8_t*    pNonce,
                            uint8_t*    pAuthTag,
                      const bool        encrypt
                      );

/***************************************************************************//**
 * @brief
 *   GCM block cipher mode encryption/decryption based on 128 bit AES.
 *
 * @details
 *   This function implements GCM (Galois Counter Mode) block cipher mode
 *   encryption and decryption based on 128 bit AES.
 *   This function expects that pHdr and pData are consecutive.
 *   For a general description please see
 *   http://en.wikipedia.org/wiki/Galois/Counter_Mode
 *   or for detailed specification see
 *   http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf
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
 *   Length of plaintext to be encrypted, referred to as 'n' in GCM.
 *   Note that this does not include the length of the MIC,
 *
 * @param[in] pHdr
 *   The 'A' parameter in GCM.
 *   Header is used for MIC calculation.
 *   Must be @p hdrLength long.
 *
 * @param[in] hdrLength
 *   The 'm' parameter in GCM.
 *   Length of authentication data.
 *
 * @param[in] pKey
 *   The 'K' parameter in GCM.
 *   Pointer to security key buffer.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *   On devices supporting key buffering this argument can be NULL, and if so,
 *   the current key will be used. The key can be set explicitly with
 *   @ref AESDRV_SetKey.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in] pInitialVector
 *   The 'IV' parameter in GCM.
 *   Pointer to the initial vector, which must have length 12 bytes (=96 bits
 *   which is recommended by GCM for efficiency).
 *
 * @param[in/out] pAuthTag
 *   The 'MIC' parameter in CCM.
 *   Pointer to the MIC buffer, which must have length @p authTagLength.
 *
 * @param[in] authTagLength
 *  Length of authentication tag 0-16 bytes.
 *
 * @param[in] encrypt
 *   Set to true to run the generation-encryption process,
 *   false to run the decryption-verification process.
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 ******************************************************************************/
Ecode_t AESDRV_GCM(AESDRV_Context_t* pAesdrvContext,
                   const uint8_t*    pDataInput,
                         uint8_t*    pDataOutput,
                   const uint32_t    dataLength,
                   const uint8_t*    pHdr,
                   const uint32_t    hdrLength,
                   const uint8_t*    pKey,
                   const uint32_t    keyLength,
                   const uint8_t*    pInitialVector,
                   const uint32_t    initialVectorLength,
                   uint8_t*          pAuthTag,
                   const uint8_t     authTagLength,
                   const bool        encrypt);

/***************************************************************************//**
 * @brief
 *  CMAC cipher mode encryption/decryption based on 128 bit AES.
 *
 * @details
 *  This function implements CMAC (Cipher-based Message Authentication Code)
 *  cipher mode encryption/decryption based on 128 bit AES.
 *  For a general description please see
 *  https://en.wikipedia.org/wiki/CMAC
 *  or for detailed specification see
 *  http://csrc.nist.gov/publications/nistpubs/800-38B/SP_800-38B.pdf
 *  http://dl.acm.org/citation.cfm?id=2206249
 *
 * @param[in] pAesdrvContext
 *  Pointer to AESDRV context structure.
 *
 * @param[in] pData
 *  Pointer to data (message) Be careful: this memory should be allocated on
 *  block-size (128-bit) boundaries!
 *
 * @param[in] dataLengthBits
 *  length of actual data in bits
 *
 * @param[in] pKey
 *  Pointer to key buffer for the AES algorithm.
 *  Currently only 128 bit keys (16 bytes) are supported.
 *  On devices supporting key buffering this argument can be NULL, and if so,
 *  the current key will used. The key can be explicitly set with
 *  @ref AESDRV_SetKey.
 *
 * @param[in] keyLength
 *   The length in bytes, of the @p pKey, i.e. the 'K' parameter in CCM.
 *   Currently only 128 bit keys (16 bytes) are supported.
 *
 * @param[in/out] pDigest
 *  128-bit (maximum) digest. If encrypting, the digest will be stored there.
 *  If verifying, the calculated digest will be compared to the one stored in
 *  this place.
 *  Warning: regardless of digestLengthBits, 128 bits will get written here.
 *
 * @param[in] digestLengthBits
 *  Requested length of the message digest in bits. LSB's will be zeroed out.
 *
 * @param[in] encrypt
 *  true - Generate hash
 *  false - Verify hash
 *
 * @return
 *   ECODE_OK if success. Error code if failure, see @ref aesdrv.h.
 */
Ecode_t AESDRV_CMAC(AESDRV_Context_t* pAesdrvContext,
                    const uint8_t*    pData,
                    uint32_t          dataLengthBits,
                    const uint8_t*    pKey,
                    const uint32_t    keyLength,
                    uint8_t*          pDigest,
                    uint16_t          digestLengthBits,
                    const bool        encrypt
                    );

#ifdef __cplusplus
}
#endif

#endif /* #if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
          ( defined(AES_COUNT) && (AES_COUNT > 0) ) */

#endif /* __SILICON_LABS_AESDRV_INTERNAL_H */
