/*
 *  sha1_alt.h
 *
 *  Copyright (C) 2018, Arm Limited, All Rights Reserved
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
 *
 */

#ifndef __SHA1_ALT__
#define __SHA1_ALT__
#if defined(MBEDTLS_SHA1_ALT)
#include "crys_hash.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief          SHA-1 context structure
 */
typedef struct
{
    CRYS_HASHUserContext_t crys_hash_ctx;
} mbedtls_sha1_context;

/**
 * \brief          This function initializes a SHA-1 context.
 *
 * \param ctx      The SHA-1 context to initialize.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_init( mbedtls_sha1_context *ctx );

/**
 * \brief          This function clears a SHA-1 context.
 *
 * \param ctx      The SHA-1 context to clear.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_free( mbedtls_sha1_context *ctx );

/**
 * \brief          This function clones the state of a SHA-1 context.
 *
 * \param dst      The destination context.
 * \param src      The context to clone.
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src );

/**
 * \brief          This function starts a SHA-1 checksum calculation.
 *
 * \param ctx      The context to initialize.
 *
 * \return         \c 0 if successful
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx );

/**
 * \brief          This function feeds an input buffer into an ongoing SHA-1
 *                 checksum calculation.
 *
 * \param ctx      The SHA-1 context.
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 *
 * \return         \c 0 if successful
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx,
                             const unsigned char *input,
                             size_t ilen );

/**
 * \brief          This function finishes the SHA-1 operation, and writes
 *                 the result to the output buffer.
 *
 * \param ctx      The SHA-1 context.
 * \param output   The SHA-1 checksum result.
 *
 * \return         \c 0 if successful
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx,
                             unsigned char output[20] );

/**
 * \brief          SHA-1 process data block (internal use only)
 *
 * \param ctx      SHA-1 context
 * \param data     The data block being processed.
 *
 * \return         \c 0 if successful
 *
 * \warning        SHA-1 is considered a weak message digest and its use
 *                 constitutes a security risk. We recommend considering
 *                 stronger message digests instead.
 *
 */
int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx,
                                   const unsigned char data[64] );

#ifdef __cplusplus
}
#endif

#endif //MBEDTLS_SHA1_ALT
#endif //__SHA1_ALT__

