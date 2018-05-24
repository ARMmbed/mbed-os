/*
 *  sha256_alt.c
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

#include "mbedtls/sha256.h"
#if defined(MBEDTLS_SHA256_ALT)
#include <string.h>

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );

}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;
    CRYS_HASH_Free( &ctx->crys_hash_ctx );
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    memcpy( dst, src, sizeof( mbedtls_sha256_context ) );
}


int mbedtls_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 )
{
    if(CRYS_HASH_Init( &ctx->crys_hash_ctx, is224 ?
                    CRYS_HASH_SHA224_mode : CRYS_HASH_SHA256_mode ) != CRYS_OK )
        return ( MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_internal_sha256_process( mbedtls_sha256_context *ctx,
                                     const unsigned char data[64] )
{
    if( CRYS_HASH_Update( &ctx->crys_hash_ctx, (uint8_t*)data, 64 ) != CRYS_OK )
        return ( MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_sha256_update_ret( mbedtls_sha256_context *ctx,
                               const unsigned char *input,
                               size_t ilen )
{
    if( CRYS_HASH_Update( &ctx->crys_hash_ctx, (uint8_t*)input, ilen ) != CRYS_OK )
        return ( MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_sha256_finish_ret( mbedtls_sha256_context *ctx,
                               unsigned char output[32] )
{
    CRYSError_t CrysErr = CRYS_OK;
    CRYS_HASH_Result_t crys_result = {0};
    CrysErr = CRYS_HASH_Finish( &ctx->crys_hash_ctx, crys_result );
    if( CrysErr == CRYS_OK )
    {
        memcpy( output, crys_result, 32 );
        return ( 0 );
    }
    else
        return ( MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED );
}
#endif //MBEDTLS_SHA256_ALT

