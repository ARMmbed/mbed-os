/*
 *  FIPS-180-2 compliant SHA-256 implementation
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
 * mbedtls/libary/sha256.c using the CRYPTO hardware accelerator incorporated
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

#if defined(MBEDTLS_SLCL_PLUGINS)

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#include "mbedtls/sha256.h"
#include "cryptodrv_internal.h"
#include "em_assert.h"
#include <string.h>

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

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
    
    /* Set device instance to 0 by default. */
    mbedtls_sha256_set_device_instance(ctx, 0);
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

/*
 * Set the device instance of an SHA context.
 */
int mbedtls_sha256_set_device_instance(mbedtls_sha256_context *ctx,
                                       unsigned int            devno)
{
#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
    if (devno > CRYPTO_COUNT)
        return( MBEDTLS_ERR_SHA256_BAD_INPUT );
  
    return cryptodrvSetDeviceInstance( &ctx->cryptodrv_ctx, devno );
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
}

/**
 * \brief          SHA-256 asynchronous context structure
 */
typedef struct
{
  mbedtls_asynch_callback  asynch_callback;   /*!< Completion callback
                                                    function pointer. */
  void*                asynch_callback_user_arg; /*!< User defined parameter to
                                                   completion callback. */
}
mbedtls_sha256_asynch_context;

/**
 * \brief          Set an SHA256 context in asynchronous mode.
 *
 * \details        
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   SHA256 context. In order to enable, the user must set the
 *   @p asynch_ctx parameter to point to an asynchronous sha256 context
 *   structure @ref mbedtls_sha256_asynch_context. Subsequent calls to the
 *   SHA256 API functions with the specified context will behave asynchronously,
 *   i.e. initiate the hardware to execute the operation and return as soon as
 *   possible. The user may specify a callback function by setting the
 *   @p asynch_callback parameter which will called when the operation has
 *   completed.
 *   In order to disable, the user must set the @p asynch_context parameter
 *   to NULL. All subsequent calls to SHA256 API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * \param ctx              SHA256 context
 * \param asynch_ctx       SHA256 asynchronous context structure
 * \param asynch_callback  Asynchronous callback
 * \param user_arg         User specific argument which will be
 *                         sent to callback.
 *
 * \return         0 if successful, or error code
 */
int mbedtls_sha256_set_asynch( mbedtls_sha256_context *ctx,
                               mbedtls_sha256_asynch_context *asynch_ctx,
                               mbedtls_asynch_callback asynch_callback,
                               void* asynch_callback_user_arg )
{
    (void) ctx;
    (void) asynch_ctx;
    (void) asynch_callback;
    (void) asynch_callback_user_arg;
  
    /* Asynchronous mode is not supported yet. */
    while(1);
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    (void) dst;
    (void) src;

    /* Cloning a SHA256 CRYPTODRV context is not supported. */
    while(1);
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 )
{
    CRYPTODRV_Context_t* cryptodrv_ctx = &ctx->cryptodrv_ctx;
    CRYPTO_TypeDef* crypto = cryptodrv_ctx->device->crypto;
    uint32_t init_state[8];
    Ecode_t ecode;

    /* Request CRYPTO usage. */
    ecode = CRYPTODRV_Arbitrate(cryptodrv_ctx);
    if (ECODE_OK != ecode)
    {
        return ecode;
    }
    
    /* Enter critial crypto region in order to initialize crypto for
       SHA operation. */
    ecode = CRYPTODRV_EnterCriticalRegion(cryptodrv_ctx);
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region entry is ok. */
    
    /* Setup CRYPTO for SHA-2 operation: */
    crypto->CTRL     = CRYPTO_CTRL_SHA_SHA2;
    crypto->WAC      = 0;

    /* Set result width of MADD32 operation. */
    CRYPTO_ResultWidthSet(crypto, cryptoResult256Bits);
  
    /* Clear sequence control registers */
    crypto->SEQCTRL  = 0;
    crypto->SEQCTRLB = 0;
    
    /* Setup the initial sha digest state */
    if( is224 == 0 )
    {
        /* SHA-256 */
        init_state[0] = 0x6A09E667;
        init_state[1] = 0xBB67AE85;
        init_state[2] = 0x3C6EF372;
        init_state[3] = 0xA54FF53A;
        init_state[4] = 0x510E527F;
        init_state[5] = 0x9B05688C;
        init_state[6] = 0x1F83D9AB;
        init_state[7] = 0x5BE0CD19;
    }
    else
    {
        /* SHA-224 */
        init_state[0] = 0xC1059ED8;
        init_state[1] = 0x367CD507;
        init_state[2] = 0x3070DD17;
        init_state[3] = 0xF70E5939;
        init_state[4] = 0xFFC00B31;
        init_state[5] = 0x68581511;
        init_state[6] = 0x64F98FA7;
        init_state[7] = 0xBEFA4FA4;
    }

    /* Push init vector to crypto module */
    CRYPTO_DDataWrite(&crypto->DDATA1, init_state);

    /* Initialize CRYPTO sequencer to execute main SHA instruction
       sequence. */
    CRYPTO_EXECUTE_3( crypto,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA0,
                      CRYPTO_CMD_INSTR_DDATA1TODDATA2,
                      CRYPTO_CMD_INSTR_SELDDATA0DDATA1 );

    /* Load main SHA instruction sequence */
    CRYPTO_SEQ_LOAD_3( crypto,
                       CRYPTO_CMD_INSTR_SHA,
                       CRYPTO_CMD_INSTR_MADD32,
                       CRYPTO_CMD_INSTR_DDATA0TODDATA1 );
    
    ecode = CRYPTODRV_ExitCriticalRegion( cryptodrv_ctx );
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region exit is ok. */
    
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->is224 = is224;

    return ( ECODE_OK == ecode ? 0 : (int)ecode );
}

void mbedtls_sha256_process( mbedtls_sha256_context *ctx, const unsigned char data[64] )
{
    CRYPTODRV_Context_t* cryptodrv_ctx = &ctx->cryptodrv_ctx;
    CRYPTO_TypeDef* crypto = cryptodrv_ctx->device->crypto;
    Ecode_t ecode;

    ecode = CRYPTODRV_EnterCriticalRegion( cryptodrv_ctx );
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region entry is ok. */

    /* Write block to QDATA1.  */
    /* Check data is 32bit aligned, if not move via aligned buffer before writing. */
    if ((uint32_t)data & 0x3)
    {
        uint32_t temp[16];
        memcpy(temp, data, 64);
        CRYPTO_QDataWrite(&crypto->QDATA1BIG, temp);
    }
    else
    {
        CRYPTO_QDataWrite(&crypto->QDATA1BIG, (uint32_t*) data);
    }

    /* Execute SHA */
    crypto->CMD |= CRYPTO_CMD_SEQSTART;

    ecode = CRYPTODRV_ExitCriticalRegion( cryptodrv_ctx );
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region exit is ok. */
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
    CRYPTODRV_Context_t* cryptodrv_ctx = &ctx->cryptodrv_ctx;
    CRYPTO_TypeDef* crypto = cryptodrv_ctx->device->crypto;
    Ecode_t ecode;

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT32_BE( high, msglen, 0 );
    PUT_UINT32_BE( low,  msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    mbedtls_sha256_update( ctx, sha256_padding, padn );
    mbedtls_sha256_update( ctx, msglen, 8 );

    /* Enter critical CRYPTO region in order to read final SHA digest/state. */
    ecode = CRYPTODRV_EnterCriticalRegion( cryptodrv_ctx );
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region entry is ok. */
    
    /* Read the digest from crypto (big endian). */
    CRYPTODRV_DDataReadUnaligned(&crypto->DDATA0BIG, output);
    
    ecode = CRYPTODRV_ExitCriticalRegion( cryptodrv_ctx );
    
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region exit is ok. */

    /* Finally release CRYPTO since SHA operation has completed. */
    ecode = CRYPTODRV_Release( cryptodrv_ctx );
   
    EFM_ASSERT(ECODE_OK == ecode); /* Assert crypto release is ok. */
    
    if( ctx->is224 )
      memset(&output[28], 0, 4);
}

/*
 * output = SHA-256( input buffer )
 */
int mbedtls_sha256( const unsigned char *input, size_t ilen,
                       unsigned char output[32], int is224 )
{
    mbedtls_sha256_context ctx;
    int ret;

    mbedtls_sha256_init( &ctx );

    ret = mbedtls_sha256_starts( &ctx, is224 );
    if (ret != 0)
      return ret;
    
    mbedtls_sha256_update( &ctx, input, ilen );
    mbedtls_sha256_finish( &ctx, output );
    mbedtls_sha256_free( &ctx );
    
    return( 0 );
}

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */

#endif /* #if defined(MBEDTLS_SLCL_PLUGINS) */

#endif /* #if defined(MBEDTLS_SHA256_ALT) */

#endif /* #if defined(MBEDTLS_SHA256_C) */
