/*
 * \file sha1_alt.h
 *
 * \brief SHA1 hw acceleration (hash function)
 *
 *  Copyright (C) 2017, STMicroelectronics
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
#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#if defined (MBEDTLS_SHA1_ALT)


#include "cmsis.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ST_SHA1_BLOCK_SIZE   ((size_t) 64) // HW handles 512 bits, ie 64 bytes
#define ST_SHA1_TIMEOUT      ((uint32_t) 3)
/**
 * \brief    SHA-1 context structure
 * \note     HAL_HASH_SHA1_Accumulate will accumulate 512 bits packets, unless it is the last call to the  function
 *           A ST_SHA1_BLOCK_SIZE bytes buffer is used to save values and handle the processing
 *           multiples of ST_SHA1_BLOCK_SIZE bytes
 *           If SHA1_finish is called and sbuf_len>0, the remaining bytes are accumulated prior to the call to HAL_HASH_SHA1_Finish
 */
typedef struct {
    unsigned char sbuf[ST_SHA1_BLOCK_SIZE]; /*!< MBEDTLS_SHA1_BLOCK_SIZE buffer to store values so that algorithm is caled once the buffer is filled */
    unsigned char sbuf_len; /*!< number of bytes remaining in sbuf to be processed */
    HASH_HandleTypeDef hhash_sha1; /*!< ST HAL HASH struct */
    uint32_t ctx_save_cr;
    uint32_t ctx_save_str;
    uint32_t ctx_save_csr[38];
}
mbedtls_sha1_context;

/**
 * \brief          Initialize SHA-1 context
 *
 * \param ctx      SHA-1 context to be initialized
 */
void mbedtls_sha1_init(mbedtls_sha1_context *ctx);

/**
 * \brief          Clear SHA-1 context
 *
 * \param ctx      SHA-1 context to be cleared
 */
void mbedtls_sha1_free(mbedtls_sha1_context *ctx);

/**
 * \brief          Clone (the state of) a SHA-1 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src);

/**
 * \brief          SHA-1 context setup
 *
 * \param ctx      context to be initialized
 *
 * \returns        error code
 */
int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx);

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 *
 * \returns        error code
 */
int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen);

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 *
 * \returns        error code
 */
int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx, unsigned char output[20]);

/* Internal use */
int mbedtls_internal_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[ST_SHA1_BLOCK_SIZE]);

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
MBEDTLS_DEPRECATED void mbedtls_sha1_starts(mbedtls_sha1_context *ctx);

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
MBEDTLS_DEPRECATED void mbedtls_sha1_update(mbedtls_sha1_context *ctx,
                                            const unsigned char *input,
                                            size_t ilen);

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
MBEDTLS_DEPRECATED void mbedtls_sha1_finish(mbedtls_sha1_context *ctx,
                                            unsigned char output[20]);

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
MBEDTLS_DEPRECATED void mbedtls_sha1_process(mbedtls_sha1_context *ctx,
                                             const unsigned char data[64]);

#undef MBEDTLS_DEPRECATED
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT */

#endif /* sha1_alt.h */
