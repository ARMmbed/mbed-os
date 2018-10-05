/**
 * \file sha512.h
 *
 * \brief SHA-384 and SHA-512 cryptographic hash function
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_SHA512_ALT_SW_H
#define MBEDTLS_SHA512_ALT_SW_H

#if defined(MBEDTLS_SHA512_ALT)

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SHA-512 context structure
 */
typedef struct {
    uint64_t total[2];          /*!< number of bytes processed  */
    uint64_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[128];  /*!< data block being processed */
    int is384;                  /*!< 0 => SHA-512, else SHA-384 */
}
mbedtls_sha512_sw_context;

/**
 * \brief          Initialize SHA-512 context
 *
 * \param ctx      SHA-512 context to be initialized
 */
void mbedtls_sha512_sw_init( mbedtls_sha512_sw_context *ctx );

/**
 * \brief          Clear SHA-512 context
 *
 * \param ctx      SHA-512 context to be cleared
 */
void mbedtls_sha512_sw_free( mbedtls_sha512_sw_context *ctx );

/**
 * \brief          Clone (the state of) a SHA-512 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void mbedtls_sha512_sw_clone( mbedtls_sha512_sw_context *dst,
                              const mbedtls_sha512_sw_context *src );

/**
 * \brief          SHA-512 context setup
 *
 * \param ctx      context to be initialized
 * \param is384    0 = use SHA512, 1 = use SHA384
 */
void mbedtls_sha512_sw_starts( mbedtls_sha512_sw_context *ctx, int is384 );

/**
 * \brief          SHA-512 process buffer
 *
 * \param ctx      SHA-512 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void mbedtls_sha512_sw_update( mbedtls_sha512_sw_context *ctx, const unsigned char *input,
                               size_t ilen );

/**
 * \brief          SHA-512 final digest
 *
 * \param ctx      SHA-512 context
 * \param output   SHA-384/512 checksum result
 */
void mbedtls_sha512_sw_finish( mbedtls_sha512_sw_context *ctx, unsigned char output[64] );

/* Internal use */
void mbedtls_sha512_sw_process( mbedtls_sha512_sw_context *ctx, const unsigned char data[128] );

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA512_ALT */

#endif /* sha512_alt_sw.h */
