/**
 * \file md5_alt.h
 *
 * \brief MD5 hw acceleration (hash function)
 *
 * Copyright (c) 2017, STMicroelectronics
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
#ifndef MBEDTLS_MD5_ALT_H
#define MBEDTLS_MD5_ALT_H

#if defined(MBEDTLS_MD5_ALT)

#include "cmsis.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ST_MD5_BLOCK_SIZE ((size_t)(64)) // HW handles 512 bits, ie 64 bytes
#define ST_MD5_TIMEOUT    ((uint32_t) 3)

/**
 * \brief          MD5 context structure
 * \note     HAL_HASH_MD5_Accumulate will accumulate 512 bits packets, unless it is the last call to the  function
 *           A ST_MD5_BLOCK_SIZE bytes buffer is used to save values and handle the processing
 *           ST_MD5_BLOCK_SIZE bytes per ST_MD5_BLOCK_SIZE bytes
 *           If MD5_finish is called and sbuf_len>0, the remaining bytes are accumulated prior to the call to HAL_HASH_MD5_Finish
 */
typedef struct
{
    HASH_HandleTypeDef hhash_md5;/*!< ST HAL HASH struct */
    unsigned char sbuf[ST_MD5_BLOCK_SIZE]; /*!< MBEDTLS_MD5_BLOCK_SIZE buffer to store values so that algorithm is caled once the buffer is filled */
    unsigned char sbuf_len; /*!< number of bytes to be processed in sbuf */
    uint32_t ctx_save_cr;
    uint32_t ctx_save_str;
    uint32_t ctx_save_csr[38];
}
mbedtls_md5_context;

/**
 * \brief          Initialize MD5 context
 *
 * \param ctx      MD5 context to be initialized
 */
void mbedtls_md5_init( mbedtls_md5_context *ctx );

/**
 * \brief          Clear MD5 context
 *
 * \param ctx      MD5 context to be cleared
 */
void mbedtls_md5_free( mbedtls_md5_context *ctx );

/**
 * \brief          Clone (the state of) an MD5 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_md5_clone( mbedtls_md5_context *dst,
                        const mbedtls_md5_context *src );

/**
 * \brief          MD5 context setup
 *
 * \param ctx      context to be initialized
 */
void mbedtls_md5_starts( mbedtls_md5_context *ctx );

/**
 * \brief          MD5 process buffer
 *
 * \param ctx      MD5 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void mbedtls_md5_update( mbedtls_md5_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          MD5 final digest
 *
 * \param ctx      MD5 context
 * \param output   MD5 checksum result
 */
void mbedtls_md5_finish( mbedtls_md5_context *ctx, unsigned char output[16] );

/* Internal use */
void mbedtls_md5_process( mbedtls_md5_context *ctx, const unsigned char data[ST_MD5_BLOCK_SIZE] );

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_MD5_ALT */

#endif /* md5_alt.h */
