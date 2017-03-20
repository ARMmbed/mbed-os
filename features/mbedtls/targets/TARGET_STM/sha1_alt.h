/*
 *  sha1_alt.h SHA-1 hash
 *******************************************************************************
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

#if defined MBEDTLS_SHA1_ALT

#include "mbedtls/platform.h"
#include "mbedtls/config.h"

#include "cmsis.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief    SHA-1 context structure
 * \note     HAL_HASH_SHA1_Accumulate cannot handle less than 4 bytes, unless it is the last call to the  function
 *           In case of buffer size < 4, flag is set to 1, remaining bytes are copied in a temp buffer.
 *           The pointer and the length are saved in sbuf and sbuf_len.
 *           At the next accumulation, the saved values are taken into account, and flag is set to 0
 *           If SHA1_finish is called and flag=1, the remaining bytes are accumulated before the call to HAL_HASH_SHA1_Finish
 */
typedef struct
{
    unsigned char *sbuf; /*!< pointer to the remaining buffer to be processed */
    unsigned char sbuf_len; /*!< number of bytes remaining in sbuf to be processed */
    HASH_HandleTypeDef hhash_sha1; /*!< ST HAL HASH struct */
    int flag;  /*!< 1 : there are sbuf_len bytes to be processed in sbuf, 0 : every data have been processed. */
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

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Output = SHA-1( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-1 checksum result
 */
void mbedtls_sha1( const unsigned char *input, size_t ilen, unsigned char output[20] );

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_sha1_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT */

#endif /* sha1_alt.h */
