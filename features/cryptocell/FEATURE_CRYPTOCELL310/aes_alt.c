/*
 *  aes_alt.c
 *
 *  Copyright (C) 2019, Arm Limited, All Rights Reserved
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
 *
 */

#include "mbedtls/aes.h"
#if defined(MBEDTLS_AES_ALT)
#include <string.h>
#include "ssi_aes_defs.h"
#include "mbedtls/platform.h"

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
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
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
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_XTS)

int mbedtls_aes_xts_setkey_enc( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}

int mbedtls_aes_xts_setkey_dec( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}

int mbedtls_aes_crypt_xts( mbedtls_aes_xts_context *ctx,
                           int mode,
                           size_t length,
                           const unsigned char data_unit[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
int mbedtls_aes_crypt_ofb( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *iv_off,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output );
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_aes_context ) );
}
#if defined(MBEDTLS_CIPHER_MODE_XTS)

void mbedtls_aes_xts_init( mbedtls_aes_xts_context *ctx ){}

void mbedtls_aes_xts_free( mbedtls_aes_xts_context *ctx ){}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

static int CC_aes_setkey( mbedtls_aes_context *ctx, const unsigned char *key,
                          unsigned int keybits, SaSiAesEncryptMode_t cipher_flag )
{
    int ret = 0;
    if( ctx == NULL )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    switch( keybits )
    {
        case 128:
        {
            ctx->CC_cipherFlag = cipher_flag;
            ctx->CC_keySizeInBytes = ( keybits / 8 );
            memcpy( ctx->CC_Key, key, ctx->CC_keySizeInBytes );
        }
        break;
        case 192:
        case 256:
            return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
        default:
            return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    return( 0 );
}
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    return( CC_aes_setkey( ctx, key, keybits, SASI_AES_ENCRYPT ) );
}

int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    return( CC_aes_setkey( ctx, key, keybits, SASI_AES_DECRYPT ) );
}

static int CC_aes_cipher( mbedtls_aes_context *ctx,
                          int mode,
                          SaSiAesOperationMode_t aes_mode,
                          size_t length,
                          unsigned char* iv,
                          size_t iv_len,
                          const unsigned char *input,
                          unsigned char *output )
{
    int ret = 0;
    SaSiAesUserKeyData_t CC_KeyData = { ctx->CC_Key,
                                        ctx->CC_keySizeInBytes };

    ret = SaSi_AesInit( &ctx->CC_Context,
                        ctx->CC_cipherFlag,
                        aes_mode, SASI_AES_PADDING_NONE );
    if( ret != 0 )
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    ret = SaSi_AesSetKey( &ctx->CC_Context, SASI_AES_USER_KEY,
                          &CC_KeyData, sizeof( CC_KeyData ) );
    if( ret != 0 )
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    if( iv )
    {
        if( iv_len != SASI_AES_IV_SIZE_IN_BYTES )
            return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

        ret = SaSi_AesSetIv( &ctx->CC_Context, iv );
        if( ret != 0 )
            return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
     }

     ret = SaSi_AesFinish( &ctx->CC_Context, length,
                           ( unsigned char* )input, length, output, &length );
     if( ret != 0 )
         return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

     /* update the IV for next block
      * For CTR mode, update the nonce only if the current length is a full AES block length
      */

     if( ( ( aes_mode == SASI_AES_MODE_CBC ) ||
          ( (aes_mode == SASI_AES_MODE_CTR) && ( ( length % SASI_AES_BLOCK_SIZE_IN_BYTES) == 0) ) )
             && iv )
     {
          ret = SaSi_AesGetIv( &ctx->CC_Context, iv );
          if( ret != 0 )
              return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
      }

      ret = SaSi_AesFree( &ctx->CC_Context );
      if( ret != 0 )
          return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

      return( 0 );
}

int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    if( ctx == NULL )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    if( ( mode == MBEDTLS_AES_ENCRYPT  && ctx->CC_cipherFlag !=  SASI_AES_ENCRYPT ) ||
        ( mode == MBEDTLS_AES_DECRYPT && ctx->CC_cipherFlag !=  SASI_AES_DECRYPT ) )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    return( CC_aes_cipher( ctx, mode, SASI_AES_MODE_ECB, 16, NULL, 0, input, output ) );
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    if( ctx == NULL )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    if( length % SASI_AES_BLOCK_SIZE_IN_BYTES )
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

    if( ( mode != MBEDTLS_AES_ENCRYPT  || ctx->CC_cipherFlag !=  SASI_AES_ENCRYPT ) &&
        ( mode != MBEDTLS_AES_DECRYPT || ctx->CC_cipherFlag !=  SASI_AES_DECRYPT ) )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    return( CC_aes_cipher( ctx, mode, SASI_AES_MODE_CBC, length, iv, 16, input, output ) );
}

#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *nc_off,
                           unsigned char nonce_counter[16],
                           unsigned char stream_block[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = 0;
    int n = *nc_off, c, i;
    size_t j;
    if( ctx == NULL )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    if( *nc_off )
    {
        /* handle corner case where we are resuming a previous encryption,
         * and we are resuming within current cipher stream(stream_block) */
        while( n != 0 )
        {
            c = *input++;
            *output++ = (unsigned char)( c ^ stream_block[n] );
            n = ( n + 1) & 0x0F;
            if( length > 0)
                --length;
        }
        /*
         * Increase the nonce_counter by 1 since we now passed one block
         */
          for( i = 16; i > 0; i-- )
              if( ++nonce_counter[i - 1] != 0 )
                  break;
    }
    if( CC_aes_cipher( ctx, MBEDTLS_AES_ENCRYPT, SASI_AES_MODE_CTR,
                       length, nonce_counter, SASI_AES_IV_SIZE_IN_BYTES, input, output ) != 0 )
    {
        ret = -1;
    }
    if( ( ( length % SASI_AES_BLOCK_SIZE_IN_BYTES ) != 0 ) && ret == 0 )
    {
        /* in case the length is not aligned, generate stream block for resuming
         * increase nonce_block to the correct value*/
        for( j = 0; j < ( length/SASI_AES_BLOCK_SIZE_IN_BYTES ); j++)
            for( i = 16; i > 0; i-- )
                if( ++nonce_counter[i - 1] != 0 )
                    break;
        if( ( ret = CC_aes_cipher( ctx, MBEDTLS_AES_ENCRYPT, SASI_AES_MODE_ECB,
                                   SASI_AES_BLOCK_SIZE_IN_BYTES, NULL, 0,
                                   nonce_counter, stream_block ) ) != 0 )
        {
            goto exit;
        }
    }
    *nc_off = ( length % SASI_AES_BLOCK_SIZE_IN_BYTES );

exit:
    return( ret );
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */
#endif/* MBEDTLS_AES_ALT */
