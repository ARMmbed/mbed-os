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
#include "mbedtls/aes.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v;
    while( n-- ) *p++ = 0;
}

void mbedtls_ccm_init( mbedtls_ccm_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_ccm_context ) );
}

void mbedtls_ccm_free( mbedtls_ccm_context *ctx )
{
    mbedtls_zeroize( ctx, sizeof( mbedtls_ccm_context ) );
}

int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits )
{
    if( ctx == NULL )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );

    if( cipher != MBEDTLS_CIPHER_ID_AES ||
         keybits != 128 )
    {
        return ( MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE );
    }

    memcpy( ctx->cipher_key , key, keybits / 8 );
    ctx->keySize_ID = CRYS_AES_Key128BitSize;

    return ( 0 );

}

/*
 * Authenticated encryption or decryption
 */

int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len )

{
    CRYSError_t CrysRet = CRYS_OK;
    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if( tag_len < 4 || tag_len > 16 || tag_len % 2 != 0 )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );

#if SIZE_MAX > UINT_MAX
    if( length > 0xFFFFFFFF || add_len > 0xFFFFFFFF )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );
#endif

    CrysRet =  CRYS_AESCCM( SASI_AES_ENCRYPT, ctx->cipher_key, ctx->keySize_ID,(uint8_t*)iv, iv_len,
                            (uint8_t*)add, add_len,  (uint8_t*)input, length, output, tag_len, tag );
    if( CrysRet != CRYS_OK )
        return ( MBEDTLS_ERR_CCM_HW_ACCEL_FAILED );

    return ( 0 );

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
    CRYSError_t CrysRet = CRYS_OK;
    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if( tag_len < 4 || tag_len > 16 || tag_len % 2 != 0 )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );

    /* Also implies q is within bounds */
    if( iv_len < 7 || iv_len > 13 )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );

#if SIZE_MAX > UINT_MAX
    if( length > 0xFFFFFFFF || add_len > 0xFFFFFFFF )
        return ( MBEDTLS_ERR_CCM_BAD_INPUT );
#endif

    CrysRet =  CRYS_AESCCM( SASI_AES_DECRYPT, ctx->cipher_key, ctx->keySize_ID,(uint8_t*)iv, iv_len,
                            (uint8_t*)add, add_len,  (uint8_t*)input, length, output, tag_len, (uint8_t*)tag );
    if ( CrysRet != CRYS_OK )
        return ( MBEDTLS_ERR_CCM_HW_ACCEL_FAILED );

    return ( 0 );

}

#endif
