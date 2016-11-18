/*
 *  CCM cipher mode based on 128 bit AES and CRYPTO hw acceleration
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
 * Definition of CCM:
 * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
 * RFC 3610 "Counter with CBC-MAC (CCM)"
 *
 * Related:
 * RFC 5116 "An Interface and Algorithms for Authenticated Encryption"
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_CCM_C )

#if defined( MBEDTLS_CCM_ALT )

#if defined( MBEDTLS_SLCL_PLUGINS )

#include "em_device.h"
#include "mbedtls/ccm.h"
#include "mbedtls/aes.h"
#include "sl_crypto.h"
#include "aesdrv_internal.h"
#include "cryptodrv_internal.h"
#include "em_assert.h"
#include <string.h>

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

/*
 * Initialize context
 */
void mbedtls_ccm_init( mbedtls_ccm_context *ctx )
{
  memset( ctx, 0, sizeof( mbedtls_ccm_context ) );

  AESDRV_Init ( &ctx->aesdrv_ctx );
  AESDRV_SetDeviceInstance ( &ctx->aesdrv_ctx, 0 );
  AESDRV_SetIoMode         ( &ctx->aesdrv_ctx, aesdrvIoModeCore, 0 );
}

int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    if ( cipher != MBEDTLS_CIPHER_ID_AES )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    
    switch( keybits )
    {
    case 128:
      break;
    case 192:
    case 256:
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    default:
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }
    
    ctx->keybits = keybits;
    memcpy(ctx->key, key, keybits/8);
    
    return( 0 );
}

/*
 * Set the device instance of an CCM context.
 */
int mbedtls_ccm_set_device_instance(mbedtls_ccm_context *ctx,
                                    unsigned int         devno)
{
#if defined(AES_COUNT) && (AES_COUNT > 0)
    (void) ctx;
    if ((devno > AES_COUNT) || (devno != 0))
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    else
        return( 0 );
#endif
  
#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
    if (devno > CRYPTO_COUNT)
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
  
    return cryptodrvSetDeviceInstance( &ctx->aesdrv_ctx.cryptodrvContext,
                                       devno );
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
}

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )

/*
 * Set an CCM context in asynchronous mode.
 */
int mbedtls_ccm_set_asynch( mbedtls_ccm_context *ctx,
                            mbedtls_ccm_asynch_context *asynch_ctx,
                            mbedtls_asynch_callback asynch_callback,
                            void* asynch_callback_user_arg )
{
    Ecode_t status;
    AESDRV_CCM_AsynchContext_t *aesdrv_asynch_ctx =
      asynch_ctx ? &asynch_ctx->aesdrv_asynch_ctx : 0;
    
    status = AESDRV_SetAsynchMode(&ctx->aesdrv_ctx,
                                  cipherModeCcm,
                                  aesdrv_asynch_ctx,
                                  (AESDRV_AsynchCallback_t) asynch_callback,
                                  asynch_callback_user_arg);
    if (status != ECODE_OK)
    {
      return (int)status;
    }

    return( 0 );
}

#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */

/*
 *   Set the device I/O mode of an CCM context.
 */
int mbedtls_ccm_set_device_io_mode( mbedtls_ccm_context             *ctx,
                                    mbedtls_device_io_mode          mode,
                                    mbedtls_device_io_mode_specific *specific )
{
  return AESDRV_SetIoMode(&ctx->aesdrv_ctx,
                          (AESDRV_IoMode_t) mode,
                          (AESDRV_IoModeSpecific_t*) specific);
}

/*
 * Free context
 */
void mbedtls_ccm_free( mbedtls_ccm_context *ctx )
{
    AESDRV_DeInit ( &ctx->aesdrv_ctx );
    
    mbedtls_zeroize( ctx, sizeof( mbedtls_ccm_context ) );
}

/*
 * Authenticated encryption
 */
int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len )
{
    Ecode_t ecode;

    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if( tag_len < 4 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( add_len > 0xFF00 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    ecode = AESDRV_CCM(&ctx->aesdrv_ctx,
                       input, output, length,
                       add, add_len,
                       (uint8_t*)ctx->key, 128/8,
                       iv, iv_len,
                       tag, tag_len,
                       true);
    
    return ( ECODE_OK == ecode ? 0 :
             ( MBEDTLS_ECODE_AESDRV_INVALID_PARAM == ecode ?
               MBEDTLS_ERR_CCM_BAD_INPUT : (int)ecode
               ) );
}

/*
 * Authenticated decryption
 */
int mbedtls_ccm_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                      const unsigned char *iv, size_t iv_len,
                      const unsigned char *add, size_t add_len,
                      const unsigned char *input, unsigned char *output,
                      const unsigned char *tag, size_t tag_len )
{
    Ecode_t ecode;
    
    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if( tag_len < 4 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( add_len > 0xFF00 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    ecode = AESDRV_CCM(&ctx->aesdrv_ctx,
                       input, output, length,
                       add, add_len,
                       (uint8_t*)ctx->key, 128/8,
                       iv, iv_len,
                       (uint8_t*)tag, tag_len,
                       false);

    return ( ECODE_OK == ecode ? 0 :
             ( MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == ecode ?
               MBEDTLS_ERR_CCM_AUTH_FAILED :
               ( MBEDTLS_ECODE_AESDRV_INVALID_PARAM == ecode ?
                 MBEDTLS_ERR_CCM_BAD_INPUT : (int)ecode
                 ) ) );
}

/*
 * CCM authenticated encryption optimized for BLE
 */
int mbedtls_ccm_encrypt_and_tag_ble( mbedtls_ccm_context *ctx,
                               unsigned char       *data,
                               size_t               length,
                               const unsigned char *iv,
                               unsigned char        header,
                               unsigned char       *tag )
{
    Ecode_t ecode;

    ecode = AESDRV_CCMBLE(&ctx->aesdrv_ctx,
                          data,
                          length,
                          header,
                          (uint8_t*)ctx->key,
                          iv,
                          tag,
                          true);
    
    return ( ECODE_OK == ecode ? 0 :
             ( MBEDTLS_ECODE_AESDRV_INVALID_PARAM == ecode ?
               MBEDTLS_ERR_CCM_BAD_INPUT : (int)ecode
               ) );
}

/*
 * CCM buffer authenticated decryption optimized for BLE
 */
int mbedtls_ccm_auth_decrypt_ble( mbedtls_ccm_context *ctx,
                             unsigned char       *data,
                             size_t               length,
                             const unsigned char *iv,
                             unsigned char        header,
                             unsigned char       *tag )
{
    Ecode_t ecode;

    ecode = AESDRV_CCMBLE(&ctx->aesdrv_ctx,
                          data,
                          length,
                          header,
                          (uint8_t*)ctx->key,
                          iv,
                          tag,
                          false );

    return ( ECODE_OK == ecode ? 0 :
             ( MBEDTLS_ECODE_AESDRV_AUTHENTICATION_FAILED == ecode ?
               MBEDTLS_ERR_CCM_AUTH_FAILED :
               ( MBEDTLS_ECODE_AESDRV_INVALID_PARAM == ecode ?
                 MBEDTLS_ERR_CCM_BAD_INPUT : (int)ecode
                 ) ) );
}

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
/*
 * Examples 1 to 3 from SP800-38C Appendix C
 */

#define NB_TESTS 3

/*
 * The data is the same for all tests, only the used length changes
 */
static const unsigned char key[] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
};

static const unsigned char iv[] = {
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b
};

static const unsigned char ad[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13
};

static const unsigned char msg[] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
};

static const size_t iv_len [NB_TESTS] = { 7, 8,  12 };
static const size_t add_len[NB_TESTS] = { 8, 16, 20 };
static const size_t msg_len[NB_TESTS] = { 4, 16, 24 };
static const size_t tag_len[NB_TESTS] = { 4, 6,  8  };

static const unsigned char res[NB_TESTS][32] = {
    {   0x71, 0x62, 0x01, 0x5b, 0x4d, 0xac, 0x25, 0x5d },
    {   0xd2, 0xa1, 0xf0, 0xe0, 0x51, 0xea, 0x5f, 0x62,
        0x08, 0x1a, 0x77, 0x92, 0x07, 0x3d, 0x59, 0x3d,
        0x1f, 0xc6, 0x4f, 0xbf, 0xac, 0xcd },
    {   0xe3, 0xb2, 0x01, 0xa9, 0xf5, 0xb7, 0x1a, 0x7a,
        0x9b, 0x1c, 0xea, 0xec, 0xcd, 0x97, 0xe7, 0x0b,
        0x61, 0x76, 0xaa, 0xd9, 0xa4, 0x42, 0x8a, 0xa5,
        0x48, 0x43, 0x92, 0xfb, 0xc1, 0xb0, 0x99, 0x51 }
};

int mbedtls_ccm_self_test( int verbose )
{
    mbedtls_ccm_context ctx;
    unsigned char out[32];
    size_t i;
    int ret;

    mbedtls_ccm_init( &ctx );

    if( mbedtls_ccm_setkey( &ctx, MBEDTLS_CIPHER_ID_AES, key, 8 * sizeof key ) != 0 )
    {
        if( verbose != 0 )
            mbedtls_printf( "  CCM: setup failed" );

        return( 1 );
    }

    for( i = 0; i < NB_TESTS; i++ )
    {
        if( verbose != 0 )
            mbedtls_printf( "  CCM-AES #%u: ", (unsigned int) i + 1 );

        ret = mbedtls_ccm_encrypt_and_tag( &ctx, msg_len[i],
                                   iv, iv_len[i], ad, add_len[i],
                                   msg, out,
                                   out + msg_len[i], tag_len[i] );

        if( ret != 0 ||
            memcmp( out, res[i], msg_len[i] + tag_len[i] ) != 0 )
        {
            if( verbose != 0 )
                mbedtls_printf( "failed\n" );

            return( 1 );
        }

        ret = mbedtls_ccm_auth_decrypt( &ctx, msg_len[i],
                                iv, iv_len[i], ad, add_len[i],
                                res[i], out,
                                res[i] + msg_len[i], tag_len[i] );

        if( ret != 0 ||
            memcmp( out, msg, msg_len[i] ) != 0 )
        {
            if( verbose != 0 )
                mbedtls_printf( "failed\n" );

            return( 1 );
        }

        if( verbose != 0 )
            mbedtls_printf( "passed\n" );
    }

    mbedtls_ccm_free( &ctx );

    if( verbose != 0 )
        mbedtls_printf( "\n" );

    return( 0 );
}

#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#endif /* MBEDTLS_SLCL_PLUGINS */

#endif /* MBEDTLS_CCM_ALT */

#endif /* MBEDTLS_CCM_C */
