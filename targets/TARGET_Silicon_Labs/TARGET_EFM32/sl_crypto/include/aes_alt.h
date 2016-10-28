/**
 * \file aes_alt.h
 *
 * \brief AES block cipher
 *
 *  Copyright (C) 2015-2016, Silicon Labs, http://www.silabs.com
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
#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/***************************************************************************//**
 * \addtogroup sl_crypto
 * \{
 ******************************************************************************/

/***************************************************************************//**
 * \addtogroup sl_crypto_aes AES block cipher
 * \brief CRYPTO hardware accelerated AES block cipher.
 * \{
 ******************************************************************************/

#if defined( MBEDTLS_SLCL_PLUGINS )
#include "aesdrv.h"
#include "sl_crypto.h"
#endif

#include <stddef.h>
#include <stdint.h>

#define MBEDTLS_AES_ENCRYPT     1
#define MBEDTLS_AES_DECRYPT     0

/* Silicon Labs specific error codes: */
#define MBEDTLS_ERR_AES_NOT_SUPPORTED         ((int)MBEDTLS_ECODE_AESDRV_NOT_SUPPORTED)
#define MBEDTLS_ERR_AES_AUTHENTICATION_FAILED ((int)MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED)
#define MBEDTLS_ERR_AES_OUT_OF_RESOURCES      ((int)MBEDTLS_ECODE_AESDRV_OUT_OF_RESOURCES)
#define MBEDTLS_ERR_AES_INVALID_PARAM         ((int)MBEDTLS_ECODE_AESDRV_INVALID_PARAM)

#if defined(MBEDTLS_AES_ALT)
/* SiliconLabs CRYPTO hardware acceleration implementation */

#ifdef __cplusplus
extern "C" {
#endif

#if defined( MBEDTLS_SLCL_PLUGINS )
#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
  
/**
 * \brief          AES asynchronous context structure
 */
typedef struct
{
    AESDRV_BlockCipherAsynchContext_t aesdrv_asynch_ctx;/*!< AESDRV asynchronous
                                                          context */
}
mbedtls_aes_asynch_context;
  
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */
#endif /* #if defined( MBEDTLS_SLCL_PLUGINS ) */  

/**
 * \brief          AES context structure
 */
typedef struct
{
#if defined( MBEDTLS_SLCL_PLUGINS )
    AESDRV_Context_t                aesdrv_ctx;       /*!< AESDRV context */
    mbedtls_device_io_mode          io_mode;          /*!< I/O mode */
    mbedtls_device_io_mode_specific io_mode_specific; /*!< I/O mode specific
                                                        data */
#endif
    unsigned int                    keybits;   /*!<  size of key */
    uint32_t                        key[8];    /*!<  AES key 128 or 256 bits */
}
mbedtls_aes_context;

/**
 * \brief          Initialize AES context
 *
 * \param ctx      AES context to be initialized
 */
void mbedtls_aes_init( mbedtls_aes_context *ctx );

/**
 * \brief          Clear AES context
 *
 * \param ctx      AES context to be cleared
 */
void mbedtls_aes_free( mbedtls_aes_context *ctx );

#if defined( MBEDTLS_SLCL_PLUGINS )
/**
 * \brief
 *   Set the device instance of an AES context.
 *
 * \details
 *   This function sets the AES/CRYPTO device instance of an AES context.
 *   Subsequent calls to AES API functions with this context will use the
 *   new AES/CRYPTO device instance.
 *
 * \param[in] ctx
 *   AES device context.
 *  
 * \param[in] devno
 *   AES/CRYPTO hardware device instance to use.
 *  
 * \return
 *   0 if success. Error code if failure, see \ref aes.h.
 ******************************************************************************/
int mbedtls_aes_set_device_instance(mbedtls_aes_context *ctx,
                                    unsigned int         devno);

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
/**
 * \brief          Set an AES context in asynchronous mode.
 *
 * \details        
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   AES context. In order to enable, the user must set the
 *   @p asynch_ctx parameter to point to an asynchronous aes context structure
 *   @ref mbedtls_aes_asynch_context. Subsequent calls to the AES API
 *   functions with the specified context will behave asynchronously, i.e.
 *   initiate the hardware to execute the operation and return as soon as
 *   possible. The user may specify a callback function by setting the
 *   @p asynch_callback parameter which will called when the operation has
 *   completed.
 *   In order to disable, the user must set the @p asynch_context parameter
 *   to NULL. All subsequent calls to AES API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * \param ctx              AES context
 * \param asynch_ctx       AES asynchronous context structure
 * \param asynch_callback  Asynchronous callback
 * \param user_arg         User specific argument which will be
 *                         sent to callback.
 *
 * \return         0 if successful, or error code
 */
int mbedtls_aes_set_asynch( mbedtls_aes_context *ctx,
                            mbedtls_aes_asynch_context *asynch_ctx,
                            mbedtls_asynch_callback asynch_callback,
                            void* user_arg );

#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */
  
/**
 * \brief
 *   Set the device I/O mode of an AES context.
 *
 * \details
 *   This function sets the device data I/O mode of an AES context. The data
 *   can be moved by Core CPU, DMA or BUFC between CRYPTO and RAM.
 *
 * \param[in] ctx
 *   AES device context.
 *  
 * \param[in] mode
 *   I/O mode (Core CPU, DMA or BUFC).
 *  
 * \param[in] specific
 *   I/O mode specific configuration \ref mbedtls_device_io_mode_specific.
 *  
 * \warning
 *   If BUFC is selected (\ref MBEDTLS_DEVICE_IO_MODE_BUFC), this function does
 *   not enable the BUFC clock and does not do any global BUFC initialization.
 *   I.e. the user is responsible for performing BUFC initialization prior to
 *   calling this function.
 *   If DMA is selected (\ref MBEDTLS_INCLUDE_IO_MODE_DMA), this function
 *   performs full DMA driver initialization by calling DMADRV_Init
 *   (non-destructive) and allocates DMA channel resources to be used by CCM.
 *
 * \return
 *   0 if success. Error code if failure, see \ref aes.h.
 ******************************************************************************/
int mbedtls_aes_set_device_io_mode( mbedtls_aes_context             *ctx,
                                    mbedtls_device_io_mode          mode,
                                    mbedtls_device_io_mode_specific *specific );
                                    
#endif
  
/**
 * \brief          AES key schedule (encryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      encryption key
 * \param keybits  must be 128, 192 or 256
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_KEY_LENGTH
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits );

/**
 * \brief          AES key schedule (decryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      decryption key
 * \param keybits  must be 128, 192 or 256
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_KEY_LENGTH
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits );

/**
 * \brief          AES-ECB block encryption/decryption
 *
 * \details
 *  TODO: doc asynch mode output buffer not ready until completion
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/**
 * \brief          AES-CBC buffer encryption/decryption
 *                 Length should be a multiple of the block
 *                 size (16 bytes)
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/**
 * \brief          AES-CFB128 buffer encryption/decryption.
 *
 * Note: Due to the nature of CFB you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv_off   offset in IV (updated after use)
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output );

/**
 * \brief          AES-CFB8 buffer encryption/decryption.
 *
 * Note: Due to the nature of CFB you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/**
 * \brief               AES-CTR buffer encryption/decryption
 *
 * Warning: You have to keep the maximum use of your counter in mind!
 *
 * Note: Due to the nature of CTR you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \param ctx           AES context
 * \param length        The length of the data
 * \param nc_off        The offset in the current stream_block (for resuming
 *                      within current cipher stream). The offset pointer to
 *                      should be 0 at the start of a stream.
 * \param nonce_counter The 128-bit nonce and counter.
 * \param stream_block  The saved stream-block for resuming. Is overwritten
 *                      by the function.
 * \param input         The input data stream
 * \param output        The output data stream
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_AES_ALT */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_aes_self_test( int verbose );

#ifdef __cplusplus
}
#endif

/** \} (end addtogroup sl_crypto) */
/** \} (end addtogroup sl_crypto_aes) */

#endif /* MBEDTLS_AES_ALT_H */
