/*
 *  FIPS-197 compliant AES implementation
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
 * This file includes alternative plugin implementations of various
 * functions in aes.c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */

/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_C)

#if defined(MBEDTLS_AES_ALT)

#if defined( MBEDTLS_SLCL_PLUGINS )

#include "mbedtls/aes.h"
#include "sl_crypto.h"
#include "aesdrv_internal.h"
#include "cryptodrv_internal.h"
#include <string.h>

#define AES_BLOCKSIZE    ( 16 )

#define MBEDTLS_RETVAL_CHK(f) do { if( ( ret = f ) != 0 ) goto cleanup; } while( 0 )

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

/*
 * Initialize AES context
 */
void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );

    AESDRV_Init ( &ctx->aesdrv_ctx );
    AESDRV_SetDeviceInstance ( &ctx->aesdrv_ctx, 0 );
    AESDRV_SetIoMode         ( &ctx->aesdrv_ctx, aesdrvIoModeCore, 0 );
}

/*
 * Clear AES context
 */
void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    AESDRV_DeInit ( &ctx->aesdrv_ctx );

    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );
}

/*
 * Set the device instance of an AES context.
 */
int mbedtls_aes_set_device_instance(mbedtls_aes_context *ctx,
                                    unsigned int         devno)
{
#if defined(AES_COUNT) && (AES_COUNT > 0)
    (void) ctx;
    if ((devno > AES_COUNT) || (devno != 0))
        return( MBEDTLS_ERR_AES_INVALID_PARAM );
    else
        return( 0 );
#endif
  
#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
    if (devno > CRYPTO_COUNT)
        return( MBEDTLS_ERR_AES_INVALID_PARAM );

    return cryptodrvSetDeviceInstance( &ctx->aesdrv_ctx.cryptodrvContext,
                                       devno );
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
}

#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
/*
 * Set an AES context in asynchronous mode.
 */
int mbedtls_aes_set_asynch( mbedtls_aes_context *ctx,
                            mbedtls_aes_asynch_context *asynch_ctx,
                            mbedtls_asynch_callback asynch_callback,
                            void* asynch_callback_user_arg )
{
    Ecode_t status;
  
    AESDRV_BlockCipherAsynchContext_t *aesdrv_asynch_ctx =
      asynch_ctx ? &asynch_ctx->aesdrv_asynch_ctx : 0;
    
    status = AESDRV_SetAsynchMode(&ctx->aesdrv_ctx,
                                  cipherModeBlockCipher,
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
 *   Set the device I/O mode of an AES context.
 */
int mbedtls_aes_set_device_io_mode( mbedtls_aes_context             *ctx,
                                    mbedtls_device_io_mode          mode,
                                    mbedtls_device_io_mode_specific *specific )
{
  return AESDRV_SetIoMode(&ctx->aesdrv_ctx,
                          (AESDRV_IoMode_t) mode,
                          (AESDRV_IoModeSpecific_t*) specific);
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    if ( ( 128 != keybits ) && ( 256 != keybits ) )
        /* Unsupported key size */
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );

    ctx->keybits = keybits;
    memcpy(ctx->key, key, keybits/8);
    
    return 0;
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits )
{
    int ret = 0;
    Ecode_t status;
    
    switch( keybits )
    {
    case 128:
        status = AESDRV_DecryptKey128( &ctx->aesdrv_ctx,
                                       (uint8_t*)ctx->key,
                                       key );
        if (status != ECODE_OK)
          ret = status;
        break;
    
    case 256:
        status = AESDRV_DecryptKey256( &ctx->aesdrv_ctx,
                                       (uint8_t*)ctx->key,
                                       key );
        if (status != ECODE_OK)
          ret = status;
        break;
        
    default:
        /* Unsupported key size */
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    }
    
    ctx->keybits = keybits;

    return ret;
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    int     ret = 0;
    Ecode_t status;

    switch( ctx->keybits )
    {
    case 128:
        status = AESDRV_ECB128( &ctx->aesdrv_ctx,
                                output,
                                input,
                                16,
                                (uint8_t*)ctx->key,
                                mode == MBEDTLS_AES_ENCRYPT ?  true : false );

        if (status != ECODE_OK)
          ret = status;
        break;
    
    case 256:
        status = AESDRV_ECB256( &ctx->aesdrv_ctx,
                                output,
                                input,
                                16,
                                (uint8_t*)ctx->key,
                                mode == MBEDTLS_AES_ENCRYPT ?  true : false );

        if (status != ECODE_OK)
          ret = status;
        break;
        
    default:
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    }
    
    return ret;
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)

/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = 0;
    Ecode_t status;

    /* Input length must be a multiple of 16 bytes which is the AES block
       length. */
    if( length & 0xf )
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

    switch( ctx->keybits )
    {
    case 128:
        status = AESDRV_CBC128( &ctx->aesdrv_ctx,
                                output,
                                input,
                                length,
                                (uint8_t*)ctx->key,
                                iv,
                                mode == MBEDTLS_AES_ENCRYPT ?  true : false );

        if (status != ECODE_OK)
          ret = status;
        break;
    
    case 256:
        status = AESDRV_CBC256( &ctx->aesdrv_ctx,
                                output,
                                input,
                                length,
                                (uint8_t*)ctx->key,
                                iv,
                                mode == MBEDTLS_AES_ENCRYPT ?  true : false );

        if (status != ECODE_OK)
          ret = status;
        break;
        
    default:
        ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
        break;
    }
    
    return( ret );
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                              int mode,
                              size_t length,
                              size_t *iv_off,
                              unsigned char iv[16],
                              const unsigned char *input,
                              unsigned char *output )
{
    size_t n = iv_off ? *iv_off : 0;
    
    if ( n || ( length & 0xf ) )
    {
        int c;

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
        if (ctx->aesdrv_ctx.pAsynchContext)
        {
            /* Asynchronous calls are not supported when iv_off is non-zero. */
            return MBEDTLS_ERR_AES_NOT_SUPPORTED;
        }
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
        
        if( mode == MBEDTLS_AES_DECRYPT )
        {
            while( length-- )
            {
                if( n == 0 )
                    mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
                
                c = *input++;
                *output++ = (unsigned char)( c ^ iv[n] );
                iv[n] = (unsigned char) c;
                
                n = ( n + 1 ) & 0x0F;
            }
        }
        else
        {
            while( length-- )
            {
                if( n == 0 )
                    mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
                
                iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );
                
                n = ( n + 1 ) & 0x0F;
            }
        }

        if (iv_off)
        {
            *iv_off = n;
        }
        return( 0 );
    }
    else
    {
        int ret = 0;
        Ecode_t status;

        switch( ctx->keybits )
        {
        case 128:
            status = AESDRV_CFB128( &ctx->aesdrv_ctx,
                                    output,
                                    input,
                                    length,
                                    (uint8_t*)ctx->key,
                                    iv,
                                    mode == MBEDTLS_AES_ENCRYPT ?
                                    true : false );

        if (status != ECODE_OK)
          ret = status;
        break;
    
        case 256:
            status = AESDRV_CFB256( &ctx->aesdrv_ctx,
                                    output,
                                    input,
                                    length,
                                    (uint8_t*)ctx->key,
                                    iv,
                                    mode == MBEDTLS_AES_ENCRYPT ?
                                    true : false );
            
            if (status != ECODE_OK)
              ret = status;
            break;
        
        default:
            ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
            break;
        }
         
        return( ret );
    }
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];
    int ret = 0;

#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
    if (ctx->aesdrv_ctx.pAsynchContext)
    {
      /* Asynchronous calls are not supported by this function. */
      return MBEDTLS_ERR_AES_NOT_SUPPORTED;
    }
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
        
    while( length-- )
    {
        memcpy( ov, iv, 16 );
        MBEDTLS_RETVAL_CHK( mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv ) );

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

 cleanup:
    
    return( ret );
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    size_t n = nc_off ? *nc_off : 0;
    
    if ( n || ( length & 0xf ) )
    {
        int c, i;
    
#if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0)
#if defined( MBEDTLS_INCLUDE_ASYNCH_API )
        if (ctx->aesdrv_ctx.pAsynchContext)
        {
            /* Asynchronous calls are not supported when nc_off is non-zero. */
            return MBEDTLS_ERR_AES_NOT_SUPPORTED;
        }
#endif /* #if defined( MBEDTLS_INCLUDE_ASYNCH_API ) */
#endif /* #if defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) */
        
        while( length-- )
        {
            if( n == 0 )
            {
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block );

                for( i = 16; i > 0; i-- )
                    if( ++nonce_counter[i - 1] != 0 )
                        break;
            }
            c = *input++;
            *output++ = (unsigned char)( c ^ stream_block[n] );

            n = ( n + 1 ) & 0x0F;
        }

        if (nc_off)
        {
            *nc_off = n;
        }
        return( 0 );
    }
    else
    {
        int ret = 0;
        Ecode_t status;

        switch( ctx->keybits )
        {
        case 128:
            status = AESDRV_CTR128( &ctx->aesdrv_ctx,
                                    output,
                                    input,
                                    length,
                                    (uint8_t*)ctx->key,
                                    nonce_counter,
                                    0);
        if (status != ECODE_OK)
          ret = status;
        break;
    
        case 256:
            status = AESDRV_CTR256( &ctx->aesdrv_ctx,
                                    output,
                                    input,
                                    length,
                                    (uint8_t*)ctx->key,
                                    nonce_counter,
                                    0);
            if (status != ECODE_OK)
              ret = status;
            break;
        
        default:
            ret = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
            break;
        }
        
        return ret;
    }
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_SLCL_PLUGINS */

#endif /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_C */
