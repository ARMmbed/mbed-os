/*
 *  FIPS-180-2 compliant SHA-256 implementation
 *
 *  Copyright (C) 2015-2016, Silicon Labs, http://www.silabs.com
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
 * This file includes alternative plugin implementations of various
 * functions in sha256.c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA256_C)

#if defined(MBEDTLS_SHA256_ALT)

#if !defined( MBEDTLS_SLCL_PLUGINS )

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#include "em_crypto.h"
#include "cryptodrv_internal.h"
#include "mbedtls/sha256.h"
#include <string.h>

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

#define CRYPTO_CLOCK_ENABLE  CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_CRYPTO;
#define CRYPTO_CLOCK_DISABLE CMU->HFBUSCLKEN0 &= ~CMU_HFBUSCLKEN0_CRYPTO;

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

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
void mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 )
{
    /* Create the init vector */
    if( is224 == 0 )
    {
        /* SHA-256 */
        ctx->state[0] = 0x6A09E667;
        ctx->state[1] = 0xBB67AE85;
        ctx->state[2] = 0x3C6EF372;
        ctx->state[3] = 0xA54FF53A;
        ctx->state[4] = 0x510E527F;
        ctx->state[5] = 0x9B05688C;
        ctx->state[6] = 0x1F83D9AB;
        ctx->state[7] = 0x5BE0CD19;
    }
    else
    {
        /* SHA-224 */
        ctx->state[0] = 0xC1059ED8;
        ctx->state[1] = 0x367CD507;
        ctx->state[2] = 0x3070DD17;
        ctx->state[3] = 0xF70E5939;
        ctx->state[4] = 0xFFC00B31;
        ctx->state[5] = 0x68581511;
        ctx->state[6] = 0x64F98FA7;
        ctx->state[7] = 0xBEFA4FA4;
    }
    
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->is224 = is224;
}

void mbedtls_sha256_process( mbedtls_sha256_context *ctx, const unsigned char data[64] )
{
    CRYPTO_CLOCK_ENABLE;
  
    /* Setup crypto module to do SHA-2. */
    CRYPTO->CTRL = CRYPTO_CTRL_SHA_SHA2 |
      /* Set DMA0 source to DDATA0 and transfer mode */
      CRYPTO_CTRL_DMA0RSEL_DDATA0    | CRYPTO_CTRL_DMA0MODE_FULL |
      /* Set DMA1 source to QDATA1BIG and transfer mode */
      CRYPTO_CTRL_DMA1RSEL_QDATA1BIG | CRYPTO_CTRL_DMA1MODE_FULL;
    
    /* Set result width of MADD32 operation. */
    CRYPTO_ResultWidthSet(CRYPTO, cryptoResult256Bits);
  
    /* Set sequence control registers */
    CRYPTO->SEQCTRL  = 16 & _CRYPTO_SEQCTRL_LENGTHA_MASK;
    CRYPTO->SEQCTRLB = 0;

    /* Initiate SHA instruction sequence. */
    CRYPTO_EXECUTE_6( CRYPTO,
                      CRYPTO_CMD_INSTR_DMA0TODATA,
                      CRYPTO_CMD_INSTR_DDATA0TODDATA1,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA1,
                      CRYPTO_CMD_INSTR_DMA1TODATA,
                      CRYPTO_CMD_INSTR_SHA,
                      CRYPTO_CMD_INSTR_MADD32);

    /* Write the state to crypto */
    CRYPTO_DDataWrite(&CRYPTO->DDATA0, ctx->state);

    /* Write block to QDATA1.  */
    /* Check data is 32bit aligned, if not move to temporary buffer. */
    if ((uint32_t)data & 0x3)
    {
        uint32_t temp[16];
        memcpy(temp, data, 64);
        CRYPTO_QDataWrite(&CRYPTO->QDATA1BIG, temp);
    }
    else
    {
        CRYPTO_QDataWrite(&CRYPTO->QDATA1BIG, (uint32_t*) data);
    }
    
    /* Read the state from crypto. */
    CRYPTO_DDataRead(&CRYPTO->DDATA0, ctx->state);

#if !defined( MBEDTLS_MPI_MUL_MPI_ALT )
    CRYPTO_CLOCK_DISABLE;
#endif
}

/*
 * SHA-256 process buffer
 */
void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input,
                    size_t ilen )
{
    size_t fill;
    uint32_t left;

    if( ilen == 0 )
        return;

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < (uint32_t) ilen )
        ctx->total[1]++;

    if( left && ilen >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), input, fill );
        mbedtls_sha256_process( ctx, ctx->buffer );
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= 64 )
    {
        mbedtls_sha256_process( ctx, input );
        input += 64;
        ilen  -= 64;
    }

    if( ilen > 0 )
        memcpy( (void *) (ctx->buffer + left), input, ilen );
}

static const unsigned char sha256_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * SHA-256 final digest
 */
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] )
{
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

    mbedtls_sha256_update( ctx, sha256_padding, padn );
    mbedtls_sha256_update( ctx, msglen, 8 );

    /* Read resulting digest (big endian) */
    CRYPTO_CLOCK_ENABLE;

    /* Read the digest from crypto (big endian). */
    CRYPTODRV_DDataReadUnaligned(&CRYPTO->DDATA0BIG, output);
    
#if !defined( MBEDTLS_MPI_MUL_MPI_ALT )
    CRYPTO_CLOCK_DISABLE;
#endif
    
    if( ctx->is224 )
      memset(&output[28], 0, 4);
}

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */

#endif /* #if !defined( MBEDTLS_SLCL_PLUGINS ) */

#endif /* #if defined(MBEDTLS_SHA256_ALT) */

#endif /* #if defined(MBEDTLS_SHA256_C) */
