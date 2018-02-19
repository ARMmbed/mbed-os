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

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA512_ALT)

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SHA context structure
 */
typedef struct {
    uint32_t total;                 /*!< number of bytes processed  */
    uint8_t buffer[128];      /*!< data block being processed. Max of SHA-1/SHA-256/SHA-512 */
    uint16_t buffer_left;
    uint16_t blocksize;             /*!< block size */
    uint32_t blocksize_mask;        /*!< block size mask */

    int is224;                      /*!< 0 => SHA-256, else SHA-224 */
}
crypto_sha_context;

void crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen);
void crypto_sha_update_nobuf(crypto_sha_context *ctx, const unsigned char *input, size_t ilen, int islast);
void crypto_sha_getinternstate(unsigned char output[], size_t olen);

#if defined(MBEDTLS_SHA1_ALT)

void mbedtls_sha1_hw_init( crypto_sha_context *ctx );
void mbedtls_sha1_hw_free( crypto_sha_context *ctx );
void mbedtls_sha1_hw_starts( crypto_sha_context *ctx );
void mbedtls_sha1_hw_update( crypto_sha_context *ctx, const unsigned char *input, size_t ilen );
void mbedtls_sha1_hw_finish( crypto_sha_context *ctx, unsigned char output[20] );
void mbedtls_sha1_hw_process( crypto_sha_context *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA1_ALT */

#if defined(MBEDTLS_SHA256_ALT)

void mbedtls_sha256_hw_init( crypto_sha_context *ctx );
void mbedtls_sha256_hw_free( crypto_sha_context *ctx );
void mbedtls_sha256_hw_starts( crypto_sha_context *ctx, int is224 );
void mbedtls_sha256_hw_update( crypto_sha_context *ctx, const unsigned char *input,
                               size_t ilen );
void mbedtls_sha256_hw_finish( crypto_sha_context *ctx, unsigned char output[32] );
void mbedtls_sha256_hw_process( crypto_sha_context *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA256_ALT */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */

#endif /* sha_alt_hw.h */
