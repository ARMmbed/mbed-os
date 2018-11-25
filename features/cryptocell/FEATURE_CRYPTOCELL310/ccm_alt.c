/*
 *  ccm_alt.c
 *
 *  Copyright (C) 2018, Arm Limited, All Rights Reserved
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

#include "mbedtls/ccm.h"
#if defined(MBEDTLS_CCM_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/aes.h"
#include "crys_aesccm_error.h"

void mbedtls_ccm_init( mbedtls_ccm_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_ccm_context ) );
}

void mbedtls_ccm_free( mbedtls_ccm_context *ctx )
{
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_ccm_context ) );
}

int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    if( ctx == NULL )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( cipher != MBEDTLS_CIPHER_ID_AES )
    {
        return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
    }

    switch( keybits )
    {
        case 128:
        {
            memcpy( ctx->cipher_key , key, keybits / 8 );
            ctx->key_size = CRYS_AES_Key128BitSize;
        }
        break;
        case 192:
        case 256:
            return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
        default:
            return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }


    return( 0 );

}

/*
 * Authenticated encryption or decryption
 */

int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                                 const unsigned char *iv, size_t iv_len,
                                 const unsigned char *add, size_t add_len,
                                 const unsigned char *input,
                                 unsigned char *output,
                                 unsigned char *tag, size_t tag_len )

{
    CRYSError_t crys_ret = CRYS_OK;
    CRYS_AESCCM_Mac_Res_t cc_mac_res = { 0 };
    int ret = 0;
    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if( tag_len < 4 || tag_len > 16 || tag_len % 2 != 0 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( tag_len > sizeof( cc_mac_res ) )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );

#if SIZE_MAX > UINT_MAX
    if( length > 0xFFFFFFFF || add_len > 0xFFFFFFFF )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
#endif

    crys_ret =  CRYS_AESCCM( SASI_AES_ENCRYPT, ctx->cipher_key, ctx->key_size,
                             (uint8_t*)iv, iv_len, (uint8_t*)add, add_len,
                             (uint8_t*)input, length, output, tag_len,
                             cc_mac_res );
    if( crys_ret == CRYS_AESCCM_ILLEGAL_PARAMETER_SIZE_ERROR )
    {
        ret = MBEDTLS_ERR_CCM_BAD_INPUT;
        goto exit;
    }
    else if( crys_ret != CRYS_OK )
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    memcpy( tag, cc_mac_res, tag_len );

exit:
    return( ret );

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
    CRYSError_t crys_ret = CRYS_OK;
    int ret = 0;
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

#if SIZE_MAX > UINT_MAX
    if( length > 0xFFFFFFFF || add_len > 0xFFFFFFFF )
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
#endif

    crys_ret =  CRYS_AESCCM( SASI_AES_DECRYPT, ctx->cipher_key, ctx->key_size,
                             (uint8_t*)iv, iv_len, (uint8_t*)add, add_len,
                             (uint8_t*)input, length, output, tag_len,
                             (uint8_t*)tag );
    if( crys_ret == CRYS_AESCCM_ILLEGAL_PARAMETER_SIZE_ERROR )
    {
       /*
        * When CRYS_AESCCM_ILLEGAL_PARAMETER_SIZE_ERROR is returned,
        * no operation has occured, and no need to zeroize output.
        * In addition, it could be that the message length is too big,
        * returning this error code, and we don't want to overflow
        * the output buffer.
        */
        return( MBEDTLS_ERR_CCM_BAD_INPUT );
    }
    else if( crys_ret == CRYS_FATAL_ERROR )
    {
        /*
         * Unfortunately, Crys AESCCM returns CRYS_FATAL_ERROR when
         * MAC isn't as expected.
         */
        ret = MBEDTLS_ERR_CCM_AUTH_FAILED;
        goto exit;
    }
    else if( crys_ret != CRYS_OK )
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

exit:
    if( ret != 0 )
        mbedtls_platform_zeroize( output, length );
    return( ret );

}

int mbedtls_ccm_star_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                                      const unsigned char *iv, size_t iv_len,
                                      const unsigned char *add, size_t add_len,
                                      const unsigned char *input,
                                      unsigned char *output,
                                      unsigned char *tag, size_t tag_len )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}

int mbedtls_ccm_star_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                                   const unsigned char *iv, size_t iv_len,
                                   const unsigned char *add, size_t add_len,
                                   const unsigned char *input,
                                   unsigned char *output,
                                   const unsigned char *tag, size_t tag_len )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}

#endif
