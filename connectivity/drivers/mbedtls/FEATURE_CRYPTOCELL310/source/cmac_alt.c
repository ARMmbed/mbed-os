/*
 *  cmac_alt.c
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

#include "mbedtls/cmac.h"
#if defined(MBEDTLS_CMAC_ALT)
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#if defined(MBEDTLS_AES_C)
#include "mbedtls/aes.h"
#endif
#include "ssi_aes_defs.h"
#include <string.h>

static int init_cc( mbedtls_cmac_context_t *cmac_ctx )
{
    int ret = 0;
    SaSiAesUserKeyData_t CC_KeyData;
    if( SaSi_AesInit( &cmac_ctx->CC_Context, SASI_AES_ENCRYPT,
                      SASI_AES_MODE_CMAC, SASI_AES_PADDING_NONE ) != 0 )
    {
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }

    CC_KeyData.pKey = cmac_ctx->CC_Key;
    CC_KeyData.keySize = cmac_ctx->CC_keySizeInBytes;

    if( SaSi_AesSetKey( &cmac_ctx->CC_Context, SASI_AES_USER_KEY,
                        &CC_KeyData, sizeof( CC_KeyData ) ) != 0 )
    {
        ret =  MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    cmac_ctx->is_cc_initiated = 1;

exit:
    return( ret );
}

static int deinit_cc( mbedtls_cmac_context_t *cmac_ctx )
{
    if( cmac_ctx->is_cc_initiated == 1  &&
        SaSi_AesFree( &cmac_ctx->CC_Context ) != 0 )
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    return( 0 );
}

int mbedtls_cipher_cmac_starts( mbedtls_cipher_context_t *ctx,
                                const unsigned char *key, size_t keybits )
{
    mbedtls_cmac_context_t *cmac_ctx;
    mbedtls_cipher_type_t type;

    if( ctx == NULL || ctx->cipher_info == NULL || key == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    type = ctx->cipher_info->type;

    switch( type )
    {
        case MBEDTLS_CIPHER_AES_128_ECB:
            break;
        case MBEDTLS_CIPHER_AES_192_ECB:
        case MBEDTLS_CIPHER_AES_256_ECB:
        case MBEDTLS_CIPHER_DES_EDE3_ECB:
            return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
        default:
            return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }


    switch( keybits )
    {
        case 128:
            /* Allocated and initialise in the cipher context memory for the CMAC
             * context
             */
            cmac_ctx = mbedtls_calloc( 1, sizeof( mbedtls_cmac_context_t ) );
            if( cmac_ctx == NULL )
                return( MBEDTLS_ERR_CIPHER_ALLOC_FAILED );
            cmac_ctx->CC_keySizeInBytes = ( keybits / 8 );
            memcpy( cmac_ctx->CC_Key, key, cmac_ctx->CC_keySizeInBytes );
        break;
        case 192:
        case 256:
            return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
        default:
            return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );
    }

    ctx->cmac_ctx = cmac_ctx;
    return( init_cc( cmac_ctx ) );
}

int mbedtls_cipher_cmac_update( mbedtls_cipher_context_t *ctx,
                                const unsigned char *input, size_t ilen )
{
    mbedtls_cmac_context_t *cmac_ctx;
    int ret = 0;
    size_t block_size;

    if( ctx == NULL || ctx->cipher_info == NULL || input == NULL ||
        ctx->cmac_ctx == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    if( ctx == NULL || ctx->cipher_info == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    block_size = ctx->cipher_info->block_size;
    if( block_size != SASI_AES_BLOCK_SIZE_IN_BYTES )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;

    /* Is there data still to process from the last call?
     */
    if( cmac_ctx->unprocessed_len > 0  )
    {
        const size_t size_to_copy = ilen > ( block_size - cmac_ctx->unprocessed_len ) ?
                                    block_size - cmac_ctx->unprocessed_len : ilen;
        memcpy( &cmac_ctx->unprocessed_block[cmac_ctx->unprocessed_len],
                input, size_to_copy );
        cmac_ctx->unprocessed_len += size_to_copy;
        input += size_to_copy;
        ilen -= size_to_copy;

        /*
         * Process the unproccessed data, in case it reached a full AES block,
         * and there is still input data.
         */
        if( cmac_ctx->unprocessed_len == SASI_AES_BLOCK_SIZE_IN_BYTES && ilen > 0 )
        {
            if( SaSi_AesBlock( &cmac_ctx->CC_Context, cmac_ctx->unprocessed_block,
                              SASI_AES_BLOCK_SIZE_IN_BYTES, NULL ) != 0 )
            {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
            cmac_ctx->unprocessed_len = 0;
        }
    }

    if( ilen > 0 )
    {
        const size_t size_to_store = ( ilen % SASI_AES_BLOCK_SIZE_IN_BYTES == 0 ) ?
                                     SASI_AES_BLOCK_SIZE_IN_BYTES : ilen % SASI_AES_BLOCK_SIZE_IN_BYTES;
        memcpy( cmac_ctx->unprocessed_block,
                input + ilen - size_to_store,
                size_to_store );
        cmac_ctx->unprocessed_len = size_to_store;
        ilen -= size_to_store;
        if( ilen > 0 )
        {
            if( SaSi_AesBlock( &cmac_ctx->CC_Context, (uint8_t *)input,
                               ilen, NULL ) != 0 )
            {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        }
    }

exit:
    if( ret != 0 )
    {
        deinit_cc( cmac_ctx );
        mbedtls_platform_zeroize( cmac_ctx, sizeof( *cmac_ctx ) );
        mbedtls_free( cmac_ctx );
    }
    return( ret );
}

int mbedtls_cipher_cmac_finish( mbedtls_cipher_context_t *ctx,
                                unsigned char *output )
{
    mbedtls_cmac_context_t *cmac_ctx;
    int ret = 0;
    size_t olen = SASI_AES_BLOCK_SIZE_IN_BYTES;

    if( ctx == NULL || ctx->cipher_info == NULL ||
        ctx->cmac_ctx == NULL || output == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;

    if( ( ret = SaSi_AesFinish( &cmac_ctx->CC_Context, cmac_ctx->unprocessed_len,
                                cmac_ctx->unprocessed_block,
                                cmac_ctx->unprocessed_len, output, &olen ) ) != 0 )
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

exit:
    if( deinit_cc( cmac_ctx ) && ret == 0 )
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return( ret );
}

int mbedtls_cipher_cmac_reset( mbedtls_cipher_context_t *ctx )
{
    mbedtls_cmac_context_t *cmac_ctx;

    if( ctx == NULL || ctx->cipher_info == NULL || ctx->cmac_ctx == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    cmac_ctx = ctx->cmac_ctx;

    /* Reset the internal state */
    cmac_ctx->unprocessed_len = 0;
    mbedtls_platform_zeroize( cmac_ctx->unprocessed_block,
                              sizeof( cmac_ctx->unprocessed_block ) );

    if( deinit_cc( cmac_ctx ) != 0 )
        return( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );

    return( init_cc( cmac_ctx ) );
}

int mbedtls_cipher_cmac( const mbedtls_cipher_info_t *cipher_info,
                         const unsigned char *key, size_t keylen,
                         const unsigned char *input, size_t ilen,
                         unsigned char *output )
{
    int ret = 0;
    mbedtls_cipher_context_t ctx;
    size_t olen = SASI_AES_BLOCK_SIZE_IN_BYTES;

    if( cipher_info == NULL || key == NULL ||
        input == NULL || output == NULL )
        return( MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA );

    mbedtls_cipher_init( &ctx );

    if( ( ret = mbedtls_cipher_setup( &ctx, cipher_info ) ) != 0 )
        goto exit;

    ret = mbedtls_cipher_cmac_starts( &ctx, key, keylen );
    if( ret != 0 )
        goto exit;

    if( SaSi_AesFinish( &ctx.cmac_ctx->CC_Context, ilen, ( uint8_t * ) input,
                        ilen, output, &olen ) != 0 )
   {
       ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
       goto clear_cc;
   }

clear_cc:
    if( deinit_cc( ctx.cmac_ctx ) != 0 && ret == 0 )
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

exit:
    mbedtls_cipher_free( &ctx );

    return( ret );

}

#if defined(MBEDTLS_AES_C)

int mbedtls_aes_cmac_prf_128( const unsigned char *key, size_t key_len,
                              const unsigned char *input, size_t in_len,
                              unsigned char output[16] )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}
#endif /* MBEDTLS_AES_C */


#endif /* MBEDTLS_CMAC_ALT */
