/*
 * Copyright (c) 2006-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */
/**
 * \file ns_sha256.h
 *
 * \brief SHA-256 cryptographic hash function
 *
 *  This file is derived from sha256.h in mbed TLS 2.3.0.
 *
 *  This file provides an API very similar to mbed TLS, either implemented
 *  locally, or by calling mbed TLS, depending on NS_USE_EXTERNAL_MBED_TLS.
 *
 *  Differences from mbed TLS:
 *
 *   a) ns_ prefix instead of mbedtls_;
 *   b) Pointers are void * instead of unsigned char * to avoid type clashes;
 *   c) SHA-224 not supported;
 *   d) Ability to output truncated hashes.
 */


#ifndef NS_SHA256_H_
#define NS_SHA256_H_

#ifdef NS_USE_EXTERNAL_MBED_TLS

#include <string.h>
#include "mbedtls/sha256.h"

typedef mbedtls_sha256_context ns_sha256_context;

static inline void ns_sha256_init(ns_sha256_context *ctx)
{
    mbedtls_sha256_init(ctx);
}

static inline void ns_sha256_free(ns_sha256_context *ctx)
{
    mbedtls_sha256_free(ctx);
}

static inline void ns_sha256_clone(ns_sha256_context *dst,
                                   const ns_sha256_context *src)
{
    mbedtls_sha256_clone(dst, src);
}

static inline void ns_sha256_starts(ns_sha256_context *ctx)
{
    mbedtls_sha256_starts(ctx, 0);
}

static inline void ns_sha256_update(ns_sha256_context *ctx, const void *input,
                                    size_t ilen)
{
    mbedtls_sha256_update(ctx, input, ilen);
}

static inline void ns_sha256_finish(ns_sha256_context *ctx, void *output)
{
    mbedtls_sha256_finish(ctx, output);
}

static inline void ns_sha256(const void *input, size_t ilen, void *output)
{
    mbedtls_sha256(input, ilen, output, 0);
}

/* Extensions to standard mbed TLS - output the first bits of a hash only */
/* Number of bits must be a multiple of 32, and <=256 */
static inline void ns_sha256_finish_nbits(ns_sha256_context *ctx, void *output, unsigned obits)
{
    if (obits == 256) {
        mbedtls_sha256_finish(ctx, output);
    } else {
        uint8_t sha256[32];
        mbedtls_sha256_finish(ctx, sha256);
        memcpy(output, sha256, obits / 8);
    }
}

static inline void ns_sha256_nbits(const void *input, size_t ilen, void *output, unsigned obits)
{
    if (obits == 256) {
        mbedtls_sha256(input, ilen, output, 0);
    } else {
        uint8_t sha256[32];
        mbedtls_sha256(input, ilen, sha256, 0);
        memcpy(output, sha256, obits / 8);
    }
}

#else /* NS_USE_EXTERNAL_MBED_TLS */

#include <stddef.h>
#include <stdint.h>

/**
 * \brief          SHA-256 context structure
 */
typedef struct
{
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
}
ns_sha256_context;

/**
 * \brief          Initialize SHA-256 context
 *
 * \param ctx      SHA-256 context to be initialized
 */
void ns_sha256_init( ns_sha256_context *ctx );

/**
 * \brief          Clear SHA-256 context
 *
 * \param ctx      SHA-256 context to be cleared
 */
void ns_sha256_free( ns_sha256_context *ctx );

/**
 * \brief          Clone (the state of) a SHA-256 context
 *
 * \param dst      The destination context
 * \param src      The context to be cloned
 */
void ns_sha256_clone( ns_sha256_context *dst,
                      const ns_sha256_context *src );

/**
 * \brief          SHA-256 context setup
 *
 * \param ctx      context to be initialized
 */
void ns_sha256_starts( ns_sha256_context *ctx );

/**
 * \brief          SHA-256 process buffer
 *
 * \param ctx      SHA-256 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void ns_sha256_update( ns_sha256_context *ctx, const void *input,
                       size_t ilen );

/**
 * \brief          SHA-256 final digest
 *
 * \param ctx      SHA-256 context
 * \param output   SHA-256 checksum result
 */
void ns_sha256_finish( ns_sha256_context *ctx, void *output );

/**
 * \brief          SHA-256 final digest
 *
 * \param ctx      SHA-256 context
 * \param output   SHA-256 checksum result
 * \param obits    Number of bits of to output - must be multiple of 32
 */
void ns_sha256_finish_nbits( ns_sha256_context *ctx,
                             void *output, unsigned obits );

/**
 * \brief          Output = SHA-256( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-256 checksum result
 */
void ns_sha256( const void *input, size_t ilen,
                void *output );

/**
 * \brief          Output = SHA-256( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-256 checksum result
 * \param obits    Number of bits of to output - must be multiple of 32
 */
void ns_sha256_nbits( const void *input, size_t ilen,
                      void *output, unsigned obits );

#endif /* NS_USE_EXTERNAL_MBED_TLS */


#endif /* NS_SHA256_H_ */
