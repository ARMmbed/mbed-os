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
#ifndef MBEDTLS_SHA_ALT_HW_H
#define MBEDTLS_SHA_ALT_HW_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_C) || defined(MBEDTLS_SHA256_C) || defined(MBEDTLS_SHA512_C)

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SHA context structure
 */
typedef struct
{
    uint32_t total;                 /*!< number of bytes processed  */
    uint32_t lastword;              /*!< last word unprocessed */
    uint16_t lastword_size;         /*!< number of bytes of last word unprocessed */
    uint16_t blocksize;             /*!< block size */
    uint32_t blocksize_mask;        /*!< block size mask */
    
    int is224;                      /*!< 0 => SHA-256, else SHA-224 */
}
crypto_sha_context;

#if defined(MBEDTLS_SHA1_ALT)

struct mbedtls_sha1_context_s;

void mbedtls_sha1_hw_init( struct mbedtls_sha1_context_s *ctx );
void mbedtls_sha1_hw_free( struct mbedtls_sha1_context_s *ctx );
void mbedtls_sha1_hw_clone( struct mbedtls_sha1_context_s *dst,
                         const struct mbedtls_sha1_context_s *src );
void mbedtls_sha1_hw_starts( struct mbedtls_sha1_context_s *ctx );
void mbedtls_sha1_hw_update( struct mbedtls_sha1_context_s *ctx, const unsigned char *input, size_t ilen );
void mbedtls_sha1_hw_finish( struct mbedtls_sha1_context_s *ctx, unsigned char output[20] );
void mbedtls_sha1_hw_process( struct mbedtls_sha1_context_s *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA1_ALT */

#if defined(MBEDTLS_SHA256_ALT)

struct mbedtls_sha256_context_s;

void mbedtls_sha256_hw_init( struct mbedtls_sha256_context_s *ctx );
void mbedtls_sha256_hw_free( struct mbedtls_sha256_context_s *ctx );
void mbedtls_sha256_hw_clone( struct mbedtls_sha256_context_s *dst,
                           const struct mbedtls_sha256_context_s *src );
void mbedtls_sha256_hw_starts( struct mbedtls_sha256_context_s *ctx, int is224 );
void mbedtls_sha256_hw_update( struct mbedtls_sha256_context_s *ctx, const unsigned char *input,
                    size_t ilen );
void mbedtls_sha256_hw_finish( struct mbedtls_sha256_context_s *ctx, unsigned char output[32] );
void mbedtls_sha256_hw_process( struct mbedtls_sha256_context_s *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA256_ALT */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_C || MBEDTLS_SHA256_C || MBEDTLS_SHA512_C*/

#endif /* sha_alt.h */
