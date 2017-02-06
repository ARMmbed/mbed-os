/*
 *  Hardware aes collector for the STM32F4 family
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
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
#if defined(MBEDTLS_AES_ALT)

#include <stdio.h>
#include "cmsis.h"
#include "string.h"
#include "aes.h"
CRYP_HandleTypeDef hcryp_aes;

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}

/**
 * \brief          Initialize AES context
 *
 * \param ctx      AES context to be initialized
 */
void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

/**
 * \brief          Clear AES context
 *
 * \param ctx      AES context to be cleared
 */
void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;
    /* Force the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_FORCE_RESET();

    /* Release the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_RELEASE_RESET();

    mbedtls_zeroize( ctx, sizeof( mbedtls_aes_context ) );

}

/**
 * \brief          AES key schedule (encryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      encryption key
 * \param keybits  must be 128, 192 or 256
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_KEY_LENGTH
 */
#if defined(MBEDTLS_AES_SETKEY_ENC_ALT)
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    switch( keybits )
    {
        case 128:
          ctx->nr = 10;
          memcpy(ctx->aes_enc_key, key, 16);
          hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B;
          break;
        case 192:
          ctx->nr = 12;
          memcpy(ctx->aes_enc_key, key, 24);
          hcryp_aes.Init.KeySize = CRYP_KEYSIZE_192B;
          break;
        case 256:
          ctx->nr = 14;
          memcpy(ctx->aes_enc_key, key, 32);
          hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B;
          break;
       default : return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    /* Deinitializes the CRYP peripheral */
    HAL_CRYP_DeInit(&hcryp_aes);

    hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;

    hcryp_aes.Instance = CRYP;
    /* Enable CRYP clock */
    __HAL_RCC_CRYP_CLK_ENABLE();

    hcryp_aes.Init.pKey = ctx->aes_enc_key;
    HAL_CRYP_Init(&hcryp_aes);
    
    return(0);
}
#endif /* MBEDTLS_AES_SETKEY_END_ALT */

/**
 * \brief          AES key schedule (decryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      decryption key
 * \param keybits  must be 128, 192 or 256
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_KEY_LENGTH
 */
#if defined(MBEDTLS_AES_SETKEY_DEC_ALT)
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                            unsigned int keybits )
{
    switch( keybits )
    {
      case 128:
        ctx->nr = 10;
        memcpy(ctx->aes_dec_key, key, 16);
        hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B;
        break;
      case 192:
        ctx->nr = 12;
        memcpy(ctx->aes_dec_key, key, 24);
        hcryp_aes.Init.KeySize = CRYP_KEYSIZE_192B;
        break;
      case 256:
        ctx->nr = 14;
        memcpy(ctx->aes_dec_key, key, 32);
        hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B;
        break;
      default : return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    /* Deinitializes the CRYP peripheral */
    HAL_CRYP_DeInit(&hcryp_aes);
    /* Enable CRYP clock */
    hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;

    hcryp_aes.Instance = CRYP;
    /* Enable CRYP clock */
    __HAL_RCC_CRYP_CLK_ENABLE();

    hcryp_aes.Init.pKey = ctx->aes_dec_key;
    
    HAL_CRYP_Init(&hcryp_aes);
    
    return( 0 );
}
#endif /* MBEDTLS_AES_SETKEY_DEC_ALT */

/**
 * \brief          AES-ECB block encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] )
{

    if ((ctx->nr != 10) && (ctx->nr != 12) && (ctx->nr != 14))
        return(MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);

    /*------------------ AES Decryption ------------------*/
    if(mode == MBEDTLS_AES_DECRYPT) /* AES decryption */
    {
        mbedtls_aes_decrypt( ctx, input, output );
    }
    /*------------------ AES Encryption ------------------*/
    else /* AES encryption */
    {
        mbedtls_aes_encrypt( ctx, input, output );
    }
    return( 0 );
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/**
 * \brief          AES-CBC buffer encryption/decryption
 *                 Length should be a multiple of the block
 *                 size (16 bytes)
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful, or MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output )
{
    int status=0;
    if( length % 16 )
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );

    switch( ctx->nr )
    {
        case 10: hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B; break;
        case 12: hcryp_aes.Init.KeySize = CRYP_KEYSIZE_192B; break;
        case 14: hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B; break;
        default : return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }
    
    if( mode == MBEDTLS_AES_DECRYPT )
    {
        hcryp_aes.Init.pInitVect = &iv[0]; // used in process, not in the init 

        status = HAL_CRYP_AESCBC_Decrypt(&hcryp_aes, (uint8_t *)input, length, (uint8_t *)output, 10);
    }
    else
    {
        hcryp_aes.Init.pInitVect = &iv[0]; // used in process, not in the init 
        
        status = HAL_CRYP_AESCBC_Encrypt(&hcryp_aes, (uint8_t *)input, length, (uint8_t *)output, 10);
    }
    return( status );
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/**
 * \brief          AES-CFB128 buffer encryption/decryption.
 *
 * Note: Due to the nature of CFB you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv_off   offset in IV (updated after use)
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c;
    size_t n = *iv_off;

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

    *iv_off = n;

    return( 0 );
}


/**
 * \brief          AES-CFB8 buffer encryption/decryption.
 *
 * Note: Due to the nature of CFB you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful
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

    while( length-- )
    {
        memcpy( ov, iv, 16 );
        mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return( 0 );
}

#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/**
 * \brief               AES-CTR buffer encryption/decryption
 *
 * Warning: You have to keep the maximum use of your counter in mind!
 *
 * Note: Due to the nature of CTR you should use the same key schedule for
 * both encryption and decryption. So a context initialized with
 * mbedtls_aes_setkey_enc() for both MBEDTLS_AES_ENCRYPT and MBEDTLS_AES_DECRYPT.
 *
 * \param ctx           AES context
 * \param length        The length of the data
 * \param nc_off        The offset in the current stream_block (for resuming
 *                      within current cipher stream). The offset pointer to
 *                      should be 0 at the start of a stream.
 * \param nonce_counter The 128-bit nonce and counter.
 * \param stream_block  The saved stream-block for resuming. Is overwritten
 *                      by the function.
 * \param input         The input data stream
 * \param output        The output data stream
 *
 * \return         0 if successful
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c, i;
    size_t n = *nc_off;

    while( length-- )
    {
        if( n == 0 ) {
            mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block );

            for( i = 16; i > 0; i-- )
                if( ++nonce_counter[i - 1] != 0 )
                    break;
        }
        c = *input++;
        *output++ = (unsigned char)( c ^ stream_block[n] );

        n = ( n + 1 ) & 0x0F;
    }

    *nc_off = n;

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

/**
 * \brief           Internal AES block encryption function
 *                  (Only exposed to allow overriding it,
 *                  see MBEDTLS_AES_ENCRYPT_ALT)
 *
 * \param ctx       AES context
 * \param input     Plaintext block
 * \param output    Output (ciphertext) block
 */
void mbedtls_aes_encrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    
    HAL_CRYP_AESECB_Encrypt(&hcryp_aes, (uint8_t *)input, 16, (uint8_t *)output, 10);

}

/**
 * \brief           Internal AES block decryption function
 *                  (Only exposed to allow overriding it,
 *                  see MBEDTLS_AES_DECRYPT_ALT)
 *
 * \param ctx       AES context
 * \param input     Ciphertext block
 * \param output    Output (plaintext) block
 */
void mbedtls_aes_decrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{

    HAL_CRYP_AESECB_Decrypt(&hcryp_aes, (uint8_t *)input, 16, (uint8_t *)output, 10);

}


#endif /*MBEDTLS_AES_ALT*/
