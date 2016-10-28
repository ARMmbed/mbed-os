/**
 * \file sha1_alt.h
 *
 * \brief SHA-1 cryptographic hash function
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
#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

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
 * \addtogroup sl_crypto_sha1 SHA-1 cryptographic hash function
 * \brief CRYPTO hardware accelerated SHA-1 cryptographic hash function.
 * \{
 ******************************************************************************/

#if defined(MBEDTLS_SHA1_ALT)

/* SiliconLabs CRYPTO hardware acceleration implementation */

#include "sl_crypto.h"
#include <stddef.h>
#include <stdint.h>

#define MBEDTLS_ERR_SHA1_BAD_INPUT      -0x5100  /**< Bad input parameters to function. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SHA-1 context structure
 */
typedef struct
{
#if defined( MBEDTLS_SLCL_PLUGINS )
    CRYPTODRV_Context_t  cryptodrv_ctx; /*!< CRYPTODRV Context */
#else
    uint32_t state[8];            /*!< intermediate digest state  */
#endif
    uint32_t total[2];          /*!< number of bytes processed  */
    unsigned char buffer[64];   /*!< data block being processed */
}
mbedtls_sha1_context;

/**
 * \brief          Initialize SHA-1 context
 *
 * \param ctx      SHA-1 context to be initialized
 */
void mbedtls_sha1_init( mbedtls_sha1_context *ctx );

/**
 * \brief          Clear SHA-1 context
 *
 * \param ctx      SHA-1 context to be cleared
 */
void mbedtls_sha1_free( mbedtls_sha1_context *ctx );

#if defined( MBEDTLS_SLCL_PLUGINS )
/**
 * \brief
 *   Set the device instance of an SHA1 context.
 *
 * \details
 *   This function sets the AES/CRYPTO device instance of an SHA1 context.
 *   Subsequent calls to SHA1 API functions with this context will use the
 *   new AES/CRYPTO device instance.
 *
 * \param[in] ctx
 *   SHA1 device context.
 *  
 * \param[in] devno
 *   AES/CRYPTO hardware device instance to use.
 *  
 * \return
 *   0 if success. Error code if failure, see \ref sha1.h.
 ******************************************************************************/
int mbedtls_sha1_set_device_instance(mbedtls_sha1_context* ctx,
                                    unsigned int           devno);
#endif /* #if defined( MBEDTLS_SLCL_PLUGINS ) */
  
/**
 * \brief          Clone (the state of) a SHA-1 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src );

/**
 * \brief          SHA-1 context setup
 *
 * \param ctx      context to be initialized
 */
void mbedtls_sha1_starts( mbedtls_sha1_context *ctx );

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 */
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] );

/* Internal use */
void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] );

/**
 * \brief          Output = SHA-1( input buffer )
 *
 * \param input    Buffer holding the data. The input buffer needs to be big
 *                 enough to hold the padding (MD-strenghtening) at the end,
 *                 after the message of 'ilen' length. This function will
 *                 perform MD-strengthening and append to the end of the
 *                 input buffer.
 * \param ilen     length of the input data
 * \param output   SHA-1 checksum result
 *
 */
void mbedtls_sha1( const unsigned char *input, size_t ilen,
                  unsigned char output[20]);

#ifdef __cplusplus
}
#endif

#endif /* #if defined(MBEDTLS_SHA1_ALT) */

/** \} (end addtogroup sl_crypto) */
/** \} (end addtogroup sl_crypto_sha1) */

#endif /* #ifndef MBEDTLS_SHA1_ALT_H */
