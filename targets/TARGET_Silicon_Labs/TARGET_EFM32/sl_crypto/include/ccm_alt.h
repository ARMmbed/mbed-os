/**
 * \file ccm_alt.h
 *
 * \brief Counter with CBC-MAC (CCM) for 128-bit block ciphers
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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
#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H

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
 * \addtogroup sl_crypto_ccm CBC-MAC (CCM) for 128-bit block ciphers
 * \brief CRYPTO hardware accelerated counter with CBC-MAC (CCM) for 128-bit block ciphers.
 * \{
 ******************************************************************************/

#if defined( MBEDTLS_CCM_ALT )
/* SiliconLabs CRYPTO hardware acceleration implementation */

#include "aesdrv.h"
#include "sl_crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
/**
 * \brief          CCM asynchronous context structure
 */
typedef struct
{
    AESDRV_CCM_AsynchContext_t aesdrv_asynch_ctx; /*!< AESDRV CCM asynchronous
                                                    context */
}
mbedtls_ccm_asynch_context;
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */

/**
 * \brief          CCM context structure
 */
typedef struct
{
    AESDRV_Context_t                aesdrv_ctx; /*!< AESDRV context */
    unsigned int                    keybits;    /*!<  size of key */
    uint32_t                        key[8];     /*!<  AES key 128 or 256 bits */
    mbedtls_device_io_mode          io_mode;          /*!< I/O mode */
    mbedtls_device_io_mode_specific io_mode_specific; /*!< IO mode specific
                                                        data. */
}
mbedtls_ccm_context;

/**
 * \brief           Initialize CCM context (just makes references valid)
 *                  Makes the context ready for mbedtls_ccm_setkey() or
 *                  mbedtls_ccm_free().
 *
 * \param ctx       CCM context to initialize
 */
void mbedtls_ccm_init( mbedtls_ccm_context *ctx );

/**
 * \brief
 *   Set the device instance of an CCM context.
 *
 * \details
 *   This function sets the AES/CRYPTO device instance of an CCM context.
 *   Subsequent calls to CCM API functions with this context will use the
 *   new AES/CRYPTO device instance.
 *
 * \param[in] ctx
 *   CCM device context.
 *  
 * \param[in] devno
 *   AES/CRYPTO hardware device instance to use.
 *  
 * \return
 *   0 if success. Error code if failure, see \ref ccm.h.
 ******************************************************************************/
int mbedtls_ccm_set_device_instance(mbedtls_ccm_context* ctx,
                                    unsigned int         devno);

/**
 * \brief
 *   Set the device I/O mode of an CCM context.
 *
 * \details
 *   This function sets the device data I/O mode of an CCM context. The data
 *   can be moved by Core CPU, DMA or BUFC between CRYPTO and RAM.
 *
 * \param[in] ctx
 *   CCM device context.
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
 *   0 if success. Error code if failure, see \ref ccm.h.
 ******************************************************************************/
int mbedtls_ccm_set_device_io_mode
(
 mbedtls_ccm_context*             ctx,
 mbedtls_device_io_mode           mode,
 mbedtls_device_io_mode_specific* specific
 );
  
/**
 * \brief           CCM initialization (encryption and decryption)
 *
 * \param ctx       CCM context to be initialized
 * \param cipher    cipher to use (a 128-bit block cipher)
 * \param key       encryption key
 * \param keybits   key size in bits (must be acceptable by the cipher)
 *
 * \return          0 if successful, or a cipher specific error code
 */
int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits );

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
  
/**
 * \brief          Set an CCM context in asynchronous mode.
 *
 * \details        
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   CCM context. In order to enable, the user must set the
 *   @p asynch_ctx parameter to point to an asynchronous ccm context structure
 *   @ref mbedtls_ccm_asynch_context. Subsequent calls to the CCM API
 *   functions with the specified context will behave asynchronously, i.e.
 *   initiate the hardware to execute the operation and return as soon as
 *   possible. The user may specify a callback function by setting the
 *   @p asynch_callback parameter which will called when the operation has
 *   completed.
 *   In order to disable, the user must set the @p asynch_context parameter
 *   to NULL. All subsequent calls to CCM API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * \param ctx              CCM context
 * \param asynch_ctx       CCM asynchronous context structure
 * \param asynch_callback  Asynchronous callback
 * \param user_arg         User specific argument which will be
 *                         sent to callback.
 *
 * \return         0 if successful, or error code
 */
int mbedtls_ccm_set_asynch( mbedtls_ccm_context *ctx,
                            mbedtls_ccm_asynch_context *asynch_ctx,
                            mbedtls_asynch_callback asynch_callback,
                            void* user_arg );

#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */

/**
 * \brief           Free a CCM context and underlying cipher sub-context
 *
 * \param ctx       CCM context to free
 */
void mbedtls_ccm_free( mbedtls_ccm_context *ctx );

/**
 * \brief           CCM buffer encryption
 *
 * \param ctx       CCM context
 * \param length    length of the input data in bytes
 * \param iv        nonce (initialization vector)
 * \param iv_len    length of IV in bytes
 *                  must be 2, 3, 4, 5, 6, 7 or 8
 * \param add       additional data
 * \param add_len   length of additional data in bytes
 *                  must be less than 2^16 - 2^8
 * \param input     buffer holding the input data
 * \param output    buffer for holding the output data
 *                  must be at least 'length' bytes wide
 * \param tag       buffer for holding the tag
 * \param tag_len   length of the tag to generate in bytes
 *                  must be 4, 6, 8, 10, 14 or 16
 *
 * \note            The tag is written to a separate buffer. To get the tag
 *                  concatenated with the output as in the CCM spec, use
 *                  tag = output + length and make sure the output buffer is
 *                  at least length + tag_len wide.
 *
 * \return          0 if successful
 */
int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len );

/**
 * \brief           CCM buffer authenticated decryption
 *
 * \param ctx       CCM context
 * \param length    length of the input data
 * \param iv        initialization vector
 * \param iv_len    length of IV
 * \param add       additional data
 * \param add_len   length of additional data
 * \param input     buffer holding the input data
 * \param output    buffer for holding the output data
 * \param tag       buffer holding the tag
 * \param tag_len   length of the tag
 *
 * \return         0 if successful and authenticated,
 *                 MBEDTLS_ERR_CCM_AUTH_FAILED if tag does not match
 */
int mbedtls_ccm_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                      const unsigned char *iv, size_t iv_len,
                      const unsigned char *add, size_t add_len,
                      const unsigned char *input, unsigned char *output,
                      const unsigned char *tag, size_t tag_len );

/**
 * \brief           CCM buffer encryption optimized for BLE
 *
 * \param ctx       CCM context
 * \param data      Input/output buffer of payload data of BLE packet
 * \param length    length of input data
 * \param iv        nonce (initialization vector)
 *                  must be 13 bytes
 * \param header    header of BLE packet (1 byte)
 * \param tag       buffer where the BLE packet tag (4 bytes) will be written
 *
 * \return          0 if successful
 */
int mbedtls_ccm_encrypt_and_tag_ble( mbedtls_ccm_context *ctx,
                               unsigned char       *data,
                               size_t               length,
                               const unsigned char *iv,
                               unsigned char        header,
                               unsigned char       *tag );

/**
 * \brief           CCM buffer authenticated decryption optimized for BLE
 *
 * \param ctx       CCM context
 * \param data      Input/output buffer of payload data of BLE packet
 * \param length    length of input data
 * \param iv        nonce (initialization vector)
 *                  must be 13 bytes
 * \param header    header of BLE packet (1 byte)
 * \param tag       authentication tag of BLE packet (4 bytes)
 *
 * \return         0 if successful and authenticated,
 *                 MBEDTLS_ERR_CCM_AUTH_FAILED if tag does not match payload
 */
int mbedtls_ccm_auth_decrypt_ble( mbedtls_ccm_context *ctx,
                             unsigned char       *data,
                             size_t               length,
                             const unsigned char *iv,
                             unsigned char        header,
                             unsigned char       *tag );

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_ccm_self_test( int verbose );
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#ifdef __cplusplus
}
#endif

#endif /* #if defined( MBEDTLS_CCM_ALT ) */

/** \} (end addtogroup sl_crypto) */
/** \} (end addtogroup sl_crypto_ccm) */

#endif /* MBEDTLS_CCM_ALT_H */
