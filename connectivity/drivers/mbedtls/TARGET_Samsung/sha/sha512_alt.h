/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef MBEDTLS_SHA512_ALT_H
#define MBEDTLS_SHA512_ALT_H

#include "mbedtls/sha512.h"

#if defined(MBEDTLS_SHA512_ALT)

#include "sss_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ST_SHA512_BUF_SIZE ((size_t)  512)
struct mbedtls_sha512_context_s;

/**
 * \brief          SHA-512 context structure
 */
typedef struct mbedtls_sha512_context_s {
    /* for S/W SHA-384 */
    uint64_t total[2];          /*!< The number of Bytes processed. */
    uint64_t state[8];          /*!< The intermediate digest state. */
    unsigned char buffer[128];  /*!< The data block being processed. */
    int is384;                  /*!< Determines which function to use:
                                     0: Use SHA-512, or 1: Use SHA-384. */
    /* for H/W SHA-512 */
    uint32_t totals;
    uint32_t hw;
    unsigned char sbuf[ST_SHA512_BUF_SIZE]; /*!< ST_SHA512_BLOCK_SIZE buffer to store values so that algorithm is called once the buffer is filled */
    stOCTET_STRING pstMessage;
    stOCTET_STRING pstDigest;

}
mbedtls_sha512_context;

/**
 * \brief          Initialize SHA-512 context
 *
 * \param ctx      SHA-512 context to be initialized
 */
void mbedtls_sha512_init(mbedtls_sha512_context *ctx);

/**
 * \brief          Clear SHA-512 context
 *
 * \param ctx      SHA-512 context to be cleared
 */
void mbedtls_sha512_free(mbedtls_sha512_context *ctx);

/**
 * \brief          Clone (the state of) a SHA-512 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha512_clone(mbedtls_sha512_context *dst,
                          const mbedtls_sha512_context *src);

/**
 * \brief          SHA-512 context setup
 *
 * \param ctx      context to be initialized
 * \param is224    0 = use SHA256, 1 = use SHA224
 *
 * \returns        error code
 */
int mbedtls_sha512_starts_ret(mbedtls_sha512_context *ctx, int is224);

/**
 * \brief          SHA-512 process buffer
 *
 * \param ctx      SHA-512 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 *
 * \returns        error code
 */
int mbedtls_sha512_update_ret(mbedtls_sha512_context *ctx, const unsigned char *input,
                              size_t ilen);

/**
 * \brief          SHA-512 final digest
 *
 * \param ctx      SHA-512 context
 * \param output   SHA-224/256 checksum result
 *
 * \returns        error code
 */
int mbedtls_sha512_finish_ret(mbedtls_sha512_context *ctx, unsigned char output[64]);

/* Internal use */
int mbedtls_internal_sha512_process(mbedtls_sha512_context *ctx, const unsigned char data[128]);

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#if defined(MBEDTLS_DEPRECATED_WARNING)
#define MBEDTLS_DEPRECATED      __attribute__((deprecated))
#else
#define MBEDTLS_DEPRECATED
#endif
/**
 * \brief          This function starts a SHA-512 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha512_starts_ret() in 2.7.0.
 *
 * \param ctx      The SHA-512 context to initialize.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-512.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 */
MBEDTLS_DEPRECATED void mbedtls_sha512_starts(mbedtls_sha512_context *ctx,
                                              int is224);

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SHA-512 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha512_update_ret() in 2.7.0.
 *
 * \param ctx      The SHA-512 context to initialize.
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 */
MBEDTLS_DEPRECATED void mbedtls_sha512_update(mbedtls_sha512_context *ctx,
                                              const unsigned char *input,
                                              size_t ilen);

/**
 * \brief          This function finishes the SHA-512 operation, and writes
 *                 the result to the output buffer.
 *
 * \deprecated     Superseded by mbedtls_sha512_finish_ret() in 2.7.0.
 *
 * \param ctx      The SHA-512 context.
 * \param output   The SHA-224or SHA-512 checksum result.
 */
MBEDTLS_DEPRECATED void mbedtls_sha512_finish(mbedtls_sha512_context *ctx,
                                              unsigned char output[64]);

/**
 * \brief          This function processes a single data block within
 *                 the ongoing SHA-512 computation. This function is for
 *                 internal use only.
 *
 * \deprecated     Superseded by mbedtls_internal_sha512_process() in 2.7.0.
 *
 * \param ctx      The SHA-512 context.
 * \param data     The buffer holding one block of data.
 */
MBEDTLS_DEPRECATED void mbedtls_sha512_process(mbedtls_sha512_context *ctx,
                                               const unsigned char data[128]);

#undef MBEDTLS_DEPRECATED
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA512_ALT */

#endif /* sha512_alt.h */
