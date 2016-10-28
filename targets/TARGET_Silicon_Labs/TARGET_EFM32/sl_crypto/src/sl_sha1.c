/*
 *  FIPS-180-1 compliant SHA-1 implementation
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
 * functions in sha1.c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */
/*
 *  The SHA-1 standard was published by NIST in 1993.
 *
 *  http://www.itl.nist.gov/fipspubs/fip180-1.htm
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_C)

#if defined(MBEDTLS_SHA1_ALT)

#if !defined( MBEDTLS_SLCL_PLUGINS )

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#include "em_crypto.h"

#include "mbedtls/sha1.h"

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
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    *dst = *src;
}

/*
 * SHA-1 context setup
 */
void mbedtls_sha1_starts( mbedtls_sha1_context *ctx )
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->state[5] = 0x0;
    ctx->state[6] = 0x0;
    ctx->state[7] = 0x0;

    ctx->total[0] = 0;
    ctx->total[1] = 0;
}

void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] )
{
    CRYPTO_CLOCK_ENABLE;
  
    /* Setup crypto module to do SHA-1. */
    CRYPTO->CTRL = CRYPTO_CTRL_SHA_SHA1 |
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
 * SHA-1 process buffer
 */
void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen )
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
        mbedtls_sha1_process( ctx, ctx->buffer );
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= 64 )
    {
        mbedtls_sha1_process( ctx, input );
        input += 64;
        ilen  -= 64;
    }

    if( ilen > 0 )
        memcpy( (void *) (ctx->buffer + left), input, ilen );
}

static const unsigned char sha1_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * SHA-1 final digest
 */
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] )
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

    mbedtls_sha1_update( ctx, sha1_padding, padn );
    mbedtls_sha1_update( ctx, msglen, 8 );

    /* Read resulting digest (big endian) */
    CRYPTO_CLOCK_ENABLE;
    ((uint32_t*)output)[0] = CRYPTO->DDATA0BIG;
    ((uint32_t*)output)[1] = CRYPTO->DDATA0BIG;
    ((uint32_t*)output)[2] = CRYPTO->DDATA0BIG;
    ((uint32_t*)output)[3] = CRYPTO->DDATA0BIG;
    ((uint32_t*)output)[4] = CRYPTO->DDATA0BIG;
    {
      /* Read 3 remaining 32-bit words from DDATA0BIG (shift register). */
      volatile uint32_t temp;
      temp = CRYPTO->DDATA0BIG;
      temp = CRYPTO->DDATA0BIG;
      temp = CRYPTO->DDATA0BIG;
      (void) temp;
    }

#if !defined( MBEDTLS_MPI_MUL_MPI_ALT )
    CRYPTO_CLOCK_DISABLE;
#endif
}

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */

#endif /* #if !defined(MBEDTLS_SLCL_PLUGINS) */

#endif /* #if defined(MBEDTLS_SHA1_ALT) */

#endif /* #if defined(MBEDTLS_SHA1_C) */
