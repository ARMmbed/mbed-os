/*
 *  FIPS-180-2 compliant SHA-1 & SHA-256 implementation
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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
 */
/*
 * This file includes an alternative implementation of the standard
 * mbedtls/libary/sha[1][256].c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */
/*
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 *
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"

#if ( defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C) ) || ( defined(MBEDTLS_SHA1_ALT) && defined (MBEDTLS_SHA1_C) )

#include "em_device.h"

#if defined(CRYPTO_PRESENT)
#include "em_crypto.h"
#include "em_core.h"
#include "crypto_management.h"
#include "em_assert.h"
#include <string.h>

#define CRYPTO_SHA_BLOCK_SIZE (64)

#if defined(MBEDTLS_SHA1_C)
static const uint32_t init_state_sha1[8] =
{
    0x67452301UL,
    0xEFCDAB89UL,
    0x98BADCFEUL,
    0x10325476UL,
    0xC3D2E1F0UL,
    0x0UL,
    0x0UL,
    0x0UL
};
#endif /* defined(MBEDTLS_SHA1_C) */

#if defined(MBEDTLS_SHA256_C)
static const uint32_t init_state_sha256[8] =
{
    0x6A09E667UL,
    0xBB67AE85UL,
    0x3C6EF372UL,
    0xA54FF53AUL,
    0x510E527FUL,
    0x9B05688CUL,
    0x1F83D9ABUL,
    0x5BE0CD19UL
};

static const uint32_t init_state_sha224[8] =
{
    0xC1059ED8UL,
    0x367CD507UL,
    0x3070DD17UL,
    0xF70E5939UL,
    0xFFC00B31UL,
    0x68581511UL,
    0x64F98FA7UL,
    0xBEFA4FA4UL
};
#endif /* defined(MBEDTLS_SHA256_C) */

static const unsigned char sha_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

typedef enum {
#if defined(MBEDTLS_SHA1_C)
    CRYPTO_SHA1,
#endif /* defined(MBEDTLS_SHA1_C) */
#if defined(MBEDTLS_SHA256_C)
    CRYPTO_SHA2
#endif /* defined(MBEDTLS_SHA256_C) */
} crypto_sha_mode_t;

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif

/**
 * @brief unified SHA acceleration function.
 *
 * @param[inout] state  State context for SHA hashing
 * @param[in] data      Input block(s). Must contain N blocks
 *                      of SHA1/SHA256 block size (64 bytes)
 * @param blocks        Amount of blocks pointed to by data
 * @param mode          SHA operation mode
 */
static void crypto_sha_update_state( uint32_t state[8],
                                     const unsigned char *data,
                                     size_t blocks,
                                     crypto_sha_mode_t mode )
{
    CORE_DECLARE_IRQ_STATE;
    CRYPTO_TypeDef *crypto = crypto_management_acquire();

    switch ( mode ) {
#if defined(MBEDTLS_SHA1_C)
        case CRYPTO_SHA1:
            crypto->CTRL = CRYPTO_CTRL_SHA_SHA1;
            break;
#endif /* defined(MBEDTLS_SHA1_C) */
#if defined(MBEDTLS_SHA256_C)
        case CRYPTO_SHA2:
            crypto->CTRL = CRYPTO_CTRL_SHA_SHA2;
            break;
#endif /* defined(MBEDTLS_SHA256_C) */
    }

    crypto->WAC      = 0;
    crypto->IEN      = 0;

    /* Set result width of MADD32 operation. */
    CRYPTO_ResultWidthSet(crypto, cryptoResult256Bits);

    /* Clear sequence control registers */
    crypto->SEQCTRL  = 0;
    crypto->SEQCTRLB = 0;

    /* Put the state back */
    CORE_ENTER_CRITICAL();
    CRYPTO_DDataWrite(&crypto->DDATA1, state);
    CORE_EXIT_CRITICAL();

    CRYPTO_EXECUTE_3( crypto,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA0,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA1 );

    /* Load the data block(s) */
    for ( size_t i = 0; i < blocks; i++ ) {
        if ((uint32_t)(&data[i*CRYPTO_SHA_BLOCK_SIZE]) & 0x3)
        {
            uint32_t temp[CRYPTO_SHA_BLOCK_SIZE/sizeof(uint32_t)];
            memcpy(temp, &data[i*CRYPTO_SHA_BLOCK_SIZE], CRYPTO_SHA_BLOCK_SIZE);
            CORE_ENTER_CRITICAL();
            CRYPTO_QDataWrite(&crypto->QDATA1BIG, temp);
            CORE_EXIT_CRITICAL();
        }
        else
        {
            CORE_ENTER_CRITICAL();
            CRYPTO_QDataWrite(&crypto->QDATA1BIG,
                              (uint32_t*) &data[i*CRYPTO_SHA_BLOCK_SIZE]);
            CORE_EXIT_CRITICAL();
        }

        /* Process the data block */
        CRYPTO_EXECUTE_3(  crypto,
                           CRYPTO_CMD_INSTR_SHA,
                           CRYPTO_CMD_INSTR_MADD32,
                           CRYPTO_CMD_INSTR_DDATA0TODDATA1 );
    }
    CORE_ENTER_CRITICAL();
    CRYPTO_DDataRead(&crypto->DDATA0, state);
    CORE_EXIT_CRITICAL();

    crypto_management_release( crypto );
}

#if defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C)
void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }

    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }

    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    if ( dst != NULL && src != NULL ) {
        *dst = *src;
    }
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 )
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    if ( is224 != 0 ) {
        ctx->is224 = true;
        memcpy(ctx->state, init_state_sha224, sizeof(ctx->state));
    } else {
        ctx->is224 = false;
        memcpy(ctx->state, init_state_sha256, sizeof(ctx->state));
    }

    return 0;
}

int mbedtls_internal_sha256_process( mbedtls_sha256_context *ctx,
                             const unsigned char data[64] )
{
    crypto_sha_update_state( ctx->state, data, 1, CRYPTO_SHA2 );
    return 0;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret( mbedtls_sha256_context *ctx,
                               const unsigned char *input,
                               size_t ilen )
{
    int err;
    size_t fill;
    uint32_t left;

    if( ilen == 0 ) {
        return 0;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < (uint32_t) ilen ) {
        ctx->total[1]++;
    }

    if( left && ilen >= fill ) {
        memcpy( (void *) (ctx->buffer + left), input, fill );
        err = mbedtls_internal_sha256_process( ctx, ctx->buffer );
        if( err != 0 ) {
            return err;
        }
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= 64 ) {
        size_t blocks = ilen / 64;
        crypto_sha_update_state( ctx->state, input, blocks, CRYPTO_SHA2 );
        input += blocks * 64;
        ilen  -= blocks * 64;
    }

    if( ilen > 0 ) {
        memcpy( (void *) (ctx->buffer + left), input, ilen );
    }

    return 0;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret( mbedtls_sha256_context *ctx,
                               unsigned char output[32] )
{
    int             err;
    uint32_t        last, padn;
    uint32_t        high, low;
    unsigned char   msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT32_BE( high, msglen, 0 );
    PUT_UINT32_BE( low,  msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    err = mbedtls_sha256_update_ret( ctx, sha_padding, padn );
    if( err != 0 ) {
        return err;
    }
    err = mbedtls_sha256_update_ret( ctx, msglen, 8 );
    if( err != 0 ) {
        return err;
    }

    for ( size_t i = 0; i < (ctx->is224 ? 28 : 32); i+=4) {
        *((uint32_t*)(&output[i])) = __REV(ctx->state[i >> 2]);
    }

    return 0;
}
#endif /* #if defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C) */

#if defined(MBEDTLS_SHA1_ALT) && defined(MBEDTLS_SHA1_C)

/**
 * \brief          Initialize SHA-1 context
 */
void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }
    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

/**
 * \brief          Clear SHA-1 context
 */
void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }
    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

/**
 * \brief          Clone (the state of) a SHA-1 context
 */
void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    if ( dst != NULL && src != NULL ) {
        *dst = *src;
    }
}

/**
 * \brief          SHA-1 context setup
 *
 * \param ctx      context to be initialized
 *
 * \returns        error code
 */
int mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx )
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    memcpy(ctx->state, init_state_sha1, 32);
    return 0;
}

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 *
 * \returns        error code
 */
int mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx,
                             const unsigned char *input,
                             size_t ilen )
{
    int err;
    size_t fill;
    uint32_t left;

    if( ilen == 0 ) {
        return 0;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < (uint32_t) ilen ) {
        ctx->total[1]++;
    }

    if( left && ilen >= fill ) {
        memcpy( (void *) (ctx->buffer + left), input, fill );
        err = mbedtls_internal_sha1_process( ctx, ctx->buffer );
        if( err != 0 ) {
            return err;
        }
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= 64 ) {
        size_t blocks = ilen / 64;
        crypto_sha_update_state( ctx->state, input, blocks, CRYPTO_SHA1 );
        input += blocks * 64;
        ilen  -= blocks * 64;
    }

    if( ilen > 0 ) {
        memcpy( (void *) (ctx->buffer + left), input, ilen );
    }

    return 0;
}

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 *
 * \returns        error code
 */
int mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx,
                             unsigned char output[20] )
{
    int err;
    uint32_t last, padn;
    uint32_t high, low;
    unsigned char msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT32_BE( high, msglen, 0 );
    PUT_UINT32_BE( low,  msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    err = mbedtls_sha1_update_ret( ctx, sha_padding, padn );
    if( err != 0 ) {
        return err;
    }
    err = mbedtls_sha1_update_ret( ctx, msglen, 8 );
    if( err != 0 ) {
        return err;
    }

    for ( size_t i = 0; i < 20; i+=4) {
        *((uint32_t*)(&output[i])) = __REV(ctx->state[i >> 2]);
    }

    return 0;
}

/* Internal use */
int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx,
                           const unsigned char data[64] )
{
    crypto_sha_update_state( ctx->state, data, 1, CRYPTO_SHA1 );
    return 0;
}

#endif /* defined(MBEDTLS_SHA1_ALT) && defined(MBEDTLS_SHA1_C) */

#endif /* #if defined(CRYPTO_PRESENT) */

#endif /* #if ( defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C) ) || ( defined(MBEDTLS_SHA1_ALT) && defined (MBEDTLS_SHA1_C) ) */
