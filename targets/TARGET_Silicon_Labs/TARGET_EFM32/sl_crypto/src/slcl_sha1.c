/*
 *  FIPS-180-1 compliant SHA-1 implementation
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
 * mbedtls/libary/sha1.c using the CRYPTO hardware accelerator incorporated
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

#if defined(MBEDTLS_SLCL_PLUGINS)

#include "em_device.h"

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)

#include "mbedtls/sha1.h"
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

    /* Set device instance to 0 by default. */
    mbedtls_sha1_set_device_instance(ctx, 0);
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

/*
 * Set the device instance of an SHA context.
 */
int mbedtls_sha1_set_device_instance(mbedtls_sha1_context *ctx,
                                     unsigned int          devno)
{
#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
    if (devno > CRYPTO_COUNT)
        return( MBEDTLS_ERR_SHA1_BAD_INPUT );
  
    return cryptodrvSetDeviceInstance( &ctx->cryptodrv_ctx, devno );
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
}

/**
 * \brief          SHA-1 asynchronous context structure
 */
typedef struct
{
  mbedtls_asynch_callback  asynch_callback;   /*!< Completion callback
                                                    function pointer. */
  void*                asynch_callback_user_arg; /*!< User defined parameter to
                                                   completion callback. */
}
mbedtls_sha1_asynch_context;

/**
 * \brief          Set an SHA1 context in asynchronous mode.
 *
 * \details        
 *   This function enables or disables asynchronous (non-blocking) mode of an
 *   SHA1 context. In order to enable, the user must set the
 *   @p asynch_ctx parameter to point to an asynchronous sha1 context structure
 *   @ref mbedtls_sha1_asynch_context. Subsequent calls to the SHA1 API
 *   functions with the specified context will behave asynchronously, i.e.
 *   initiate the hardware to execute the operation and return as soon as
 *   possible. The user may specify a callback function by setting the
 *   @p asynch_callback parameter which will called when the operation has
 *   completed.
 *   In order to disable, the user must set the @p asynch_context parameter
 *   to NULL. All subsequent calls to SHA1 API functions with the specified
 *   context will block until the corresponding operation has completed, and
 *   then return.
 *
 * \param ctx              SHA1 context
 * \param asynch_ctx       SHA1 asynchronous context structure
 * \param asynch_callback  Asynchronous callback
 * \param user_arg         User specific argument which will be
 *                         sent to callback.
 *
 * \return         0 if successful, or error code
 */
int mbedtls_sha1_set_asynch( mbedtls_sha1_context *ctx,
                             mbedtls_sha1_asynch_context *asynch_ctx,
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

void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    (void) dst;
    (void) src;
    
    /* Cloning a SHA256 CRYPTODRV context is not supported. */
    while(1);
}

/*
 * SHA-1 context setup
 */
int mbedtls_sha1_starts( mbedtls_sha1_context *ctx )
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
    
    /* Setup CRYPTO for SHA-1 operation: */
    crypto->CTRL = CRYPTO_CTRL_SHA_SHA1;
    crypto->WAC  = 0;

    /* Set result width of MADD32 operation. */
    CRYPTO_ResultWidthSet(crypto, cryptoResult256Bits);
  
    /* Clear sequence control registers */
    crypto->SEQCTRL  = 0;
    crypto->SEQCTRLB = 0;
    
    /* Setup the initial sha digest state */
    init_state[0] = 0x67452301;
    init_state[1] = 0xEFCDAB89;
    init_state[2] = 0x98BADCFE;
    init_state[3] = 0x10325476;
    init_state[4] = 0xC3D2E1F0;
    init_state[5] = 0x0;
    init_state[6] = 0x0;
    init_state[7] = 0x0;

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
    
    return ( ECODE_OK == ecode ? 0 : (int)ecode );
}

void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] )
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

    mbedtls_sha1_update( ctx, sha1_padding, padn );
    mbedtls_sha1_update( ctx, msglen, 8 );

    /* Enter critical CRYPTO region in order to read final SHA digest/state. */
    ecode = CRYPTODRV_EnterCriticalRegion( cryptodrv_ctx );
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region entry is ok. */
    
    /* Read the digest from crypto (big endian). */
    ((uint32_t*)output)[0] = crypto->DDATA0BIG;
    ((uint32_t*)output)[1] = crypto->DDATA0BIG;
    ((uint32_t*)output)[2] = crypto->DDATA0BIG;
    ((uint32_t*)output)[3] = crypto->DDATA0BIG;
    ((uint32_t*)output)[4] = crypto->DDATA0BIG;
    {
      /* Read 3 remaining 32-bit words from DDATA0BIG (shift register). */
      volatile uint32_t temp;
      temp = crypto->DDATA0BIG;
      temp = crypto->DDATA0BIG;
      temp = crypto->DDATA0BIG;
      (void) temp;
    }
    
    ecode = CRYPTODRV_ExitCriticalRegion( cryptodrv_ctx );
    
    EFM_ASSERT(ECODE_OK == ecode); /* Assert critical region exit is ok. */

    /* Finally release CRYPTO since SHA operation has completed. */
    ecode = CRYPTODRV_Release( cryptodrv_ctx );
   
    EFM_ASSERT(ECODE_OK == ecode); /* Assert crypto release is ok. */
}

/*
 * output = SHA-1( input buffer )
 */
int mbedtls_sha1( const unsigned char *input, size_t ilen, unsigned char output[20] )
{
    mbedtls_sha1_context ctx;
    int ret;
    
    mbedtls_sha1_init( &ctx );
    
    ret = mbedtls_sha1_starts( &ctx );
    if (ret != 0)
      return ret;
    
    mbedtls_sha1_update( &ctx, input, ilen );
    mbedtls_sha1_finish( &ctx, output );
    mbedtls_sha1_free( &ctx );

    return( 0 );
}

#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */

#endif /* #if defined(MBEDTLS_SLCL_PLUGINS) */

#endif /* MBEDTLS_SHA1_ALT */

#endif /* MBEDTLS_SHA1_C */
