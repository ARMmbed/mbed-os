/*
 *  Hardware-accelerated AES implementation for Silicon Labs devices
 *  containing an AES peripheral.
 *
 *  Copyright 2017, Silicon Laboratories, Inc.
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

#include "mbedtls/aes.h"
#include "em_device.h"

#if defined(AES_PRESENT) && (AES_COUNT == 1)
#if defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_AES_ALT)
#include "em_aes.h"
#include "em_cmu.h"
#include "em_bus.h"
#include <string.h>

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#include "em_core.h"
/* Mutex for protecting access to the AES instance */
static mbedtls_threading_mutex_t aes_mutex;
static volatile bool aes_mutex_inited = false;
#endif

static void aes_lock( void )
{
#if defined(MBEDTLS_THREADING_C)
    if ( !aes_mutex_inited ) {
        /* Turn off interrupts that can cause preemption */
        CORE_irqState_t critical_irq_state = CORE_EnterCritical();
        if ( !aes_mutex_inited ) {
            mbedtls_mutex_init(&aes_mutex);
            aes_mutex_inited = true;
        }
        CORE_ExitCritical(critical_irq_state);
    }
    mbedtls_mutex_lock(&aes_mutex);
#endif
    BUS_RegBitWrite(&(CMU->HFCORECLKEN0), _CMU_HFCORECLKEN0_AES_SHIFT, 1);
    return;
}

static void aes_unlock( void )
{
#if defined(MBEDTLS_THREADING_C)
    if ( aes_mutex_inited ) {
        mbedtls_mutex_unlock(&aes_mutex);
    }
#endif
    BUS_RegBitWrite(&(CMU->HFCORECLKEN0), _CMU_HFCORECLKEN0_AES_SHIFT, 0);
    return;
}

/*
 * Initialize AES context
 */
void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

/*
 * Clear AES context
 */
void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    if ( ( 128 != keybits ) && ( 256 != keybits ) )
    {
        /* Unsupported key size */
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    ctx->keybits = keybits;
    memcpy( ctx->key, key, keybits/8 );

    return 0;
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    if ( ( 128 != keybits ) && ( 256 != keybits ) )
        /* Unsupported key size */
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );

    ctx->keybits = keybits;
    switch (keybits)
    {
        case 128:
            aes_lock();
            AES_DecryptKey128( ctx->key, key );
            aes_unlock();
            break;
        case 256:
            aes_lock();
            AES_DecryptKey256( ctx->key, key );
            aes_unlock();
            break;
        default:
            return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    return 0;
}

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt( mbedtls_aes_context *ctx,
                                   const unsigned char input[16],
                                   unsigned char output[16] )
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, input, output);
}

/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt( mbedtls_aes_context *ctx,
                                   const unsigned char input[16],
                                   unsigned char output[16] )
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_DECRYPT, input, output);
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    switch ( ctx->keybits )
    {
        case 128:
            aes_lock();
            AES_ECB128( output,
                        input,
                        16,
                        ctx->key,
                        mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;
        case 256:
            aes_lock();
            AES_ECB256( output,
                        input,
                        16,
                        ctx->key,
                        mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;
        default:
           return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    return( 0 );
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
    uint8_t tmpIv[16];

    if( length % 16 )
    {
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( mode == MBEDTLS_AES_DECRYPT )
    {
        if ( length >= 16 )
            memcpy( tmpIv, &input[length-16], 16 );
    }

    switch ( ctx->keybits )
    {
        case 128:
            aes_lock();
            AES_CBC128( output,
                        input,
                        length,
                        ctx->key,
                        iv,
                        mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;
        case 256:
            aes_lock();
            AES_CBC256( output,
                        input,
                        length,
                        ctx->key,
                        iv,
                        mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;
        default:
           return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( length >= 16 )
    {
        if ( mode == MBEDTLS_AES_ENCRYPT )
            memcpy( iv, &output[length-16], 16 );
        else
            memcpy( iv, tmpIv, 16 );
    }

    return( 0 );
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
    size_t n = ( iv_off != NULL ) ? *iv_off : 0;

    if ( ( n > 0 ) || ( length & 0xf ) )
    {
        // IV offset or length not aligned to block size
        int c;

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
        switch( ctx->keybits )
        {
        case 128:
            aes_lock();
            AES_CFB128(output,
                       input,
                       length,
                       ctx->key,
                       iv,
                       mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;

        case 256:
            aes_lock();
            AES_CFB256(output,
                       input,
                       length,
                       ctx->key,
                       iv,
                       mode == MBEDTLS_AES_ENCRYPT ? true : false );
            aes_unlock();
            break;

        default:
            return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
        }

        return( 0 );
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
/*
 * AES-CTR Nonce update function
 */
static void aes_ctr_update_nonce( uint8_t *nonce_counter )
{
    for( size_t i = 16; i > 0; i-- )
        if( ++nonce_counter[i - 1] != 0 )
            break;
}

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
    size_t n = ( nc_off != NULL ) ? *nc_off : 0;

    if ( ( n > 0 ) || ( length & 0xf ) )
    {
        // IV offset or length not aligned to block size
        int c, i;

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
        switch( ctx->keybits )
        {
        case 128:
            aes_lock();
            AES_CTR128( output,
                        input,
                        length,
                        ctx->key,
                        nonce_counter,
                        &aes_ctr_update_nonce );
            aes_unlock();
        break;

        case 256:
            aes_lock();
            AES_CTR256( output,
                        input,
                        length,
                        ctx->key,
                        nonce_counter,
                        &aes_ctr_update_nonce );
            aes_unlock();
            break;

        default:
            return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
            break;
        }

        return( 0 );
    }
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_AES_ALT */
#endif /* MBEDTLS_AES_C */
#endif /* AES_PRESENT && (AES_COUNT == 1) */
