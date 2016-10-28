/**
 * \file cmac_alt.h
 *
 * \brief CMAC cipher mode based on 128 bit AES.
 *
 *  Copyright (C) 2016, Silicon Labs, All Rights Reserved
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
#ifndef MBEDTLS_CMAC_ALT_H
#define MBEDTLS_CMAC_ALT_H

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
 * \addtogroup sl_crypto_cmac CMAC cipher mode based on 128 bit AES
 * \brief CRYPTO hardware accelerated CMAC (Cipher-based Message Authentication Code)
 * \details
 *  Encryption and decryption based on 128-bit AES.
 *  For a general description please see
 *  https://en.wikipedia.org/wiki/CMAC
 *  or for detailed specification see
 *  http://csrc.nist.gov/publications/nistpubs/800-38B/SP_800-38B.pdf
 *  http://dl.acm.org/citation.cfm?id=2206249
 * \{
 ******************************************************************************/

#if defined( MBEDTLS_CMAC_ALT )
/* SiliconLabs CRYPTO hardware acceleration implementation */

#include "aesdrv.h"
#include "sl_crypto.h"
#include "cipher.h"
#include <stddef.h>

#define MBEDTLS_ERR_CMAC_BAD_INPUT      ((int)MBEDTLS_ERR_CMAC_BASE | 0x01)
#define MBEDTLS_ERR_CMAC_AUTH_FAILED    ((int)MBEDTLS_ERR_CMAC_BASE | 0x02)

#ifdef __cplusplus
extern "C" {
#endif

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
/**
 * \brief          CMAC asynchronous context structure
 */
typedef struct
{
    AESDRV_CMAC_AsynchContext_t aesdrv_asynch_ctx; /*!< AESDRV CMAC asynchronous
                                                     context */
}
mbedtls_cmac_asynch_context;
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */

/**
 * \brief          CMAC context structure
 */
typedef struct
{
    AESDRV_Context_t             aesdrv_ctx;  /*!< AESDRV context */
    unsigned int                 keybits;     /*!<  size of key */
    uint32_t                     key[8];      /*!<  AES key 128 or 256 bits */
}
mbedtls_cmac_context;

/**
 * \brief           Initialize CMAC context (just makes references valid)
 *                  Makes the context ready for mbedtls_cmac_setkey() or
 *                  mbedtls_cmac_free().
 *
 * \param ctx       CMAC context to initialize
 */
void mbedtls_cmac_init( mbedtls_cmac_context *ctx );

/**
 * \brief
 *   Set the device instance of an CMAC context.
 *
 * \details
 *   This function sets the AES/CRYPTO device instance of an CMAC context.
 *   Subsequent calls to CMAC API functions with this context will use the
 *   new AES/CRYPTO device instance.
 *
 * \param[in] ctx
 *   CMAC device context.
 *  
 * \param[in] devno
 *   AES/CRYPTO hardware device instance to use.
 *  
 * \return
 *   0 if success. Error code if failure, see \ref cmac.h.
 ******************************************************************************/
int mbedtls_cmac_set_device_instance(mbedtls_cmac_context* ctx,
                                     unsigned int          devno);

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
/**
 * \brief          Set an CMAC context in asynchronous mode.
 *
 * \details        
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   CMAC context. In order to enable, the user must set the
 *   @p asynch_ctx parameter to point to an asynchronous cmac context structure
 *   @ref mbedtls_cmac_asynch_context. Subsequent calls to the CMAC API
 *   functions with the specified context will behave asynchronously, i.e.
 *   initiate the hardware to execute the operation and return as soon as
 *   possible. The user may specify a callback function by setting the
 *   @p asynch_callback parameter which will called when the operation has
 *   completed.
 *   In order to disable, the user must set the @p asynch_ctx parameter
 *   to NULL. All subsequent calls to CMAC API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * \param ctx              CMAC context
 * \param asynch_ctx       CMAC asynchronous context structure
 * \param asynch_callback  Asynchronous callback
 * \param user_arg         User specific argument which will be
 *                         sent to callback.
 *
 * \return         0 if successful, or error code
 */
int mbedtls_cmac_set_asynch( mbedtls_cmac_context *ctx,
                             mbedtls_cmac_asynch_context *asynch_ctx,
                             mbedtls_asynch_callback asynch_callback,
                             void* user_arg );
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */

/**
 * \brief           CMAC initialization (encryption and decryption)
 *
 * \param ctx       CMAC context to be initialized
 * \param cipher    cipher to use (a 128-bit block cipher)
 * \param key       encryption key
 * \param keybits   key size in bits (must be acceptable by the cipher)
 *
 * \return          0 if successful, or a cipher specific error code
 */
int mbedtls_cmac_setkey( mbedtls_cmac_context *ctx,
                         mbedtls_cipher_id_t cipher,
                         const unsigned char *key,
                         unsigned int keybits );

/**
 * \brief           Free a CMAC context and underlying cipher sub-context
 *
 * \param ctx       CMAC context to free
 */
void mbedtls_cmac_free( mbedtls_cmac_context *ctx );

/**
 * \brief           Generate CMAC tag
 *
 * \param ctx       CMAC context
 * \param data      buffer holding the input data
 * \param data_len  length of the input data in bits
 * \param tag       buffer for holding the tag
 * \param tag_len   length of the tag to generate in bits
 *                  must be less than 128 bits
 *
 * \note            The tag is written to a separate buffer. To get the tag
 *                  concatenated with the output as in the CMAC spec, use
 *                  tag = data + length and make sure the output buffer is
 *                  at least data_len + tag_len wide.
 *
 * \return          0 if successful
 */
int mbedtls_cmac_generate_tag( mbedtls_cmac_context *ctx,
                               const unsigned char  *data,
                               size_t                data_len,
                               unsigned char        *tag,
                               size_t                tag_len );

/**
 * \brief           Verify CMAC tag
 *
 * \param ctx       CMAC context
 * \param data      buffer holding the input data
 * \param data_len  length of the input data in bits
 * \param tag       buffer holding the tag
 * \param tag_len   length of the tag in bits
 *                  must be less than 128 bits
 *
 * \return         0 if successful and authenticated,
 *                 MBEDTLS_ERR_CMAC_AUTH_FAILED if tag does not match data
 */
int mbedtls_cmac_verify_tag( mbedtls_cmac_context *ctx,
                             const unsigned char  *data,
                             size_t                data_len,
                             unsigned char        *tag,
                             size_t                tag_len );

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_CMAC_C)
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_cmac_self_test( int verbose );
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_CMAC_C */

#ifdef __cplusplus
}
#endif

#endif /* #if defined( MBEDTLS_CMAC_ALT ) */

/** \} (end addtogroup sl_crypto) */
/** \} (end addtogroup sl_crypto_cmac) */

#endif /* MBEDTLS_CMAC_ALT_H */
