/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#include "mbedtls/sha1.h"

#if defined(MBEDTLS_SHA1_ALT)

#include "sha_alt_hw.h"
#include "sha1_alt_sw.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mbedtls_sha1_context_s;

/**
 * \brief          SHA-1 context structure
 */
typedef struct mbedtls_sha1_context_s {
    void *active_ctx;
    crypto_sha_context hw_ctx;
    mbedtls_sha1_sw_context sw_ctx;
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
 *
 * \returns        error code
 */
int mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx );

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 *
 * \returns        error code
 */
int mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 *
 * \returns        error code
 */
int mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx, unsigned char output[20] );

/* Internal use */
int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] );

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#if defined(MBEDTLS_DEPRECATED_WARNING)
#define MBEDTLS_DEPRECATED      __attribute__((deprecated))
#else
#define MBEDTLS_DEPRECATED
#endif
/**
 * \brief          SHA-1 context setup
 *
 * \deprecated     Superseded by mbedtls_sha1_starts_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context to be initialized.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
MBEDTLS_DEPRECATED void mbedtls_sha1_starts( mbedtls_sha1_context *ctx );

/**
 * \brief          SHA-1 process buffer
 *
 * \deprecated     Superseded by mbedtls_sha1_update_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context.
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
MBEDTLS_DEPRECATED void mbedtls_sha1_update( mbedtls_sha1_context *ctx,
                                             const unsigned char *input,
                                             size_t ilen );

/**
 * \brief          SHA-1 final digest
 *
 * \deprecated     Superseded by mbedtls_sha1_finish_ret() in 2.7.0
 *
 * \param ctx      The SHA-1 context.
 * \param output   The SHA-1 checksum result.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
MBEDTLS_DEPRECATED void mbedtls_sha1_finish( mbedtls_sha1_context *ctx,
                                             unsigned char output[20] );

/**
 * \brief          SHA-1 process data block (internal use only)
 *
 * \deprecated     Superseded by mbedtls_internal_sha1_process() in 2.7.0
 *
 * \param ctx      The SHA-1 context.
 * \param data     The data block being processed.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
MBEDTLS_DEPRECATED void mbedtls_sha1_process( mbedtls_sha1_context *ctx,
                                              const unsigned char data[64] );

#undef MBEDTLS_DEPRECATED
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT */

#endif /* sha1_alt.h */
