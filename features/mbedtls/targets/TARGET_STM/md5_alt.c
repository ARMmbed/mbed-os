/*
 *  MD5 hw acceleration
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

#include "mbedtls/md5.h"

#if defined(MBEDTLS_MD5_ALT)

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

void mbedtls_md5_init( mbedtls_md5_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_md5_context ) );

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();
}

void mbedtls_md5_free( mbedtls_md5_context *ctx )
{
    if( ctx == NULL )
        return;

    /* Force the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_FORCE_RESET();

    /* Release the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_RELEASE_RESET();

    mbedtls_zeroize( ctx, sizeof( mbedtls_md5_context ) );
}

void mbedtls_md5_clone( mbedtls_md5_context *dst,
                        const mbedtls_md5_context *src )
{
    *dst = *src;
}

/*
 * MD5 context setup
 */
void mbedtls_md5_starts( mbedtls_md5_context *ctx )
{
    /* HASH IP initialization */
    HAL_HASH_DeInit(&ctx->hhash_md5);
    
    /* HASH Configuration */
    ctx->hhash_md5.Init.DataType = HASH_DATATYPE_8B;
    HAL_HASH_Init(&ctx->hhash_md5);
}

void mbedtls_md5_process( mbedtls_md5_context *ctx, const unsigned char data[64] )
{
    HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, (uint8_t *)data, 64);
}

/*
 * MD5 process buffer
 */
void mbedtls_md5_update( mbedtls_md5_context *ctx, const unsigned char *input, size_t ilen )
{
    HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, (uint8_t *)input, ilen);
}

/*
 * MD5 final digest
 */
void mbedtls_md5_finish( mbedtls_md5_context *ctx, unsigned char output[16] )
{
    __HAL_HASH_START_DIGEST();
        
    HAL_HASH_MD5_Finish(&ctx->hhash_md5, output, 10);
}

#endif /* MBEDTLS_MD5_ALT */
