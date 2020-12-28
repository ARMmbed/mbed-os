/*
 *  sha1_alt.c
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

#include "mbedtls/sha1.h"
#if defined(MBEDTLS_SHA1_ALT)
#include <string.h>
#include "mbedtls/platform.h"

void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL )
        return;

    CRYS_HASH_Free( &ctx->crys_hash_ctx );

    memset( ctx, 0, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    memcpy( dst, src, sizeof( mbedtls_sha1_context ) );
}

int mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx )
{
    if( CRYS_HASH_Init( &ctx->crys_hash_ctx, CRYS_HASH_SHA1_mode ) != CRYS_OK )
            return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    return ( 0 );
}


int mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx,
                             const unsigned char *input,
                             size_t ilen )
{
    if( CRYS_HASH_Update( &ctx->crys_hash_ctx, (uint8_t*)input, ilen ) != CRYS_OK )
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    return ( 0 );
}

int mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx,
                             unsigned char output[20] )
{
    CRYSError_t crys_err = CRYS_OK;
    CRYS_HASH_Result_t crys_result = {0};
    crys_err = CRYS_HASH_Finish( &ctx->crys_hash_ctx, crys_result );
    if( crys_err == CRYS_OK )
    {
        memcpy( output, crys_result, 20 );
        return ( 0 );
    }
    else
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
}

int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx,
                                   const unsigned char data[64] )
{
    return( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );
}
#endif //MBEDTLS_SHA1_ALT
