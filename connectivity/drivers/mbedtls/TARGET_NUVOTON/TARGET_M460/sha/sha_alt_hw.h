/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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
 * \brief       Max size of SHA block in bytes
 *
 * SHA-160/224/256: 64 bytes
 * SHA-384/512: 128 bytes
 *
 * \note        Support SHA1/2 only, no SHA3
 */
#define NU_CRYPTO_SHA_MAXSIZE_BLOCK_BYTES       128

/**
 * \brief       Max size of SHA DMA block run in bytes
 *
 * \note        DMA cascade mode requires block-aligned except for the last block.
 * \note        This also defines DMA intermediary buffer size.
 */
#define NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES     \
    (NU_CRYPTO_SHA_MAXSIZE_BLOCK_BYTES * 4)

/**
 * \brief       Max size of SHA feedback information in words, used in DMA cascade mode
 *
 * SHA1/2: 54 words
 * SHA3: 88 words
 *
 * \note        Support SHA1/2 only, no SHA3
 * \note        According to spec, reserve 54 words for SHA1/2. But per real
 *              test, SHA H/W will overwrite beyond 54 words. Workaround by
 *              reserving 88 words anyway.
 */
#define NU_CRYPTO_SHA_MAXSIZE_FBINFO_WORDS      88

/**
 * \brief          SHA context structure
 *
 * \note           SHA type/opmode
 * 1. For type, borrow from opmode defines and can only be SHA_MODE_SHA1/SHA256/SHA512.
 * 2. Initialize opmode/digestsize dependent on type
 * 3. For SHA_MODE_SHA256/512, opmode will change to SHA_MODE_SHA224/384
 *    when is224_384 is non-zero in the call to crypto_sha_starts().
 * 4. Following above, for opmode being SHA_MODE_SHA224/384, change digestsize to 28/48
 */
typedef struct {
    /* These fields will initialize at crypto_sha_init() */
    uint32_t type;                  /*!< SHA type */
    uint32_t opmode;                /*!< SHA operation mode */
    uint32_t digestsize;            /*!< SHA digest size */
    int err_hw_accel;               /*!< Mbed TLS error code for the SHA type */
    int err_bad_input;              /*!< Mbed TLS error code for the SHA type */

    /* These fields will initialize at crypto_sha_starts() */
    uint32_t total;                 /*!< number of bytes processed  */
    union {
        uint8_t buffer[NU_CRYPTO_SHA_MAXSIZE_BLOCK_BYTES];         /*!< data block being processed. Max of SHA-1/SHA-256/SHA-512 */
        uint32_t buffer_u32[NU_CRYPTO_SHA_MAXSIZE_BLOCK_BYTES/4];  /*!< make buffer word aligned */
    };
    uint16_t buffer_left;
    uint16_t blocksize;             /*!< block size */
    uint32_t blocksize_mask;        /*!< block size mask */
    int is224_384;                  /*!< 0 => SHA-256/SHA-512, else SHA-224/384 */
    uint32_t fbinfo[NU_CRYPTO_SHA_MAXSIZE_FBINFO_WORDS];           /*!< feedback information in DMA cascade mode */
}
crypto_sha_context;

void crypto_sha_init(crypto_sha_context *ctx, uint32_t type);
void crypto_sha_free(crypto_sha_context *ctx);
int crypto_sha_starts(crypto_sha_context *ctx, int is224_384);
int crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen);
int crypto_sha_update_nobuf(crypto_sha_context *ctx, const unsigned char *input, size_t ilen, int isfirst, int islast);
int crypto_sha_finish(crypto_sha_context *ctx, unsigned char output[], size_t olen);
int crypto_sha_getdigest(unsigned char output[], size_t olen);

#if defined(MBEDTLS_SHA1_ALT)

void mbedtls_sha1_hw_init( crypto_sha_context *ctx );
void mbedtls_sha1_hw_free( crypto_sha_context *ctx );
int mbedtls_sha1_hw_starts( crypto_sha_context *ctx );
int mbedtls_sha1_hw_update( crypto_sha_context *ctx, const unsigned char *input, size_t ilen );
int mbedtls_sha1_hw_finish( crypto_sha_context *ctx, unsigned char output[20] );
int mbedtls_sha1_hw_process( crypto_sha_context *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA1_ALT */

#if defined(MBEDTLS_SHA256_ALT)

void mbedtls_sha256_hw_init( crypto_sha_context *ctx );
void mbedtls_sha256_hw_free( crypto_sha_context *ctx );
int mbedtls_sha256_hw_starts( crypto_sha_context *ctx, int is224 );
int mbedtls_sha256_hw_update( crypto_sha_context *ctx, const unsigned char *input,
                               size_t ilen );
int mbedtls_sha256_hw_finish( crypto_sha_context *ctx, unsigned char output[32] );
int mbedtls_sha256_hw_process( crypto_sha_context *ctx, const unsigned char data[64] );

#endif /* MBEDTLS_SHA256_ALT */

#if defined(MBEDTLS_SHA512_ALT)

void mbedtls_sha512_hw_init( crypto_sha_context *ctx );
void mbedtls_sha512_hw_free( crypto_sha_context *ctx );
int mbedtls_sha512_hw_starts( crypto_sha_context *ctx, int is384 );
int mbedtls_sha512_hw_update( crypto_sha_context *ctx, const unsigned char *input,
                               size_t ilen );
int mbedtls_sha512_hw_finish( crypto_sha_context *ctx, unsigned char output[64] );
int mbedtls_sha512_hw_process( crypto_sha_context *ctx, const unsigned char data[128] );

#endif /* MBEDTLS_SHA512_ALT */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */

#endif /* sha_alt.h */
