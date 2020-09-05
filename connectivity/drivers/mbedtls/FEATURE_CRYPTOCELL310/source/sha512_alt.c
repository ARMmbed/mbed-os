/*
 *  sha512_alt.c
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

#include "mbedtls/sha512.h"
#if defined(MBEDTLS_SHA512_ALT)
#include <string.h>
#include "mbedtls/platform.h"

void mbedtls_sha512_init( mbedtls_sha512_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_free( mbedtls_sha512_context *ctx )
{
    if( ctx == NULL )
        return;
    CRYS_HASH_Free( &ctx->crys_hash_ctx );
    memset( ctx, 0, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_clone( mbedtls_sha512_context *dst,
                           const mbedtls_sha512_context *src )
{
    memcpy(dst,src,sizeof(mbedtls_sha512_context));
}


int mbedtls_sha512_starts_ret( mbedtls_sha512_context *ctx, int is384 )
{
    if( is384 )
        return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
    if( CRYS_HASH_Init( &ctx->crys_hash_ctx, CRYS_HASH_SHA512_mode ) != CRYS_OK )
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_internal_sha512_process( mbedtls_sha512_context *ctx,
                                     const unsigned char data[128] )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}

int mbedtls_sha512_update_ret( mbedtls_sha512_context *ctx,
                               const unsigned char *input,
                               size_t ilen )
{
    if( CRYS_HASH_Update( &ctx->crys_hash_ctx, (uint8_t*)input, ilen ) != CRYS_OK )
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_sha512_finish_ret( mbedtls_sha512_context *ctx,
                               unsigned char output[64] )
{
    CRYSError_t crys_err = CRYS_OK;
    CRYS_HASH_Result_t crys_result = {0};
    crys_err = CRYS_HASH_Finish( &ctx->crys_hash_ctx, crys_result );
    if( crys_err == CRYS_OK )
    {
        memcpy(output,crys_result,64);
        return ( 0 );
    }
    else
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
}
#endif //MBEDTLS_SHA512_ALT
