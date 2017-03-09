/*
 *  sha256_alt.c for SHA256 HASH
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
#include "mbedtls/sha256.h"

#if defined(MBEDTLS_SHA256_ALT)

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );	

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;
	
	/* Force the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_FORCE_RESET();

    /* Release the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_RELEASE_RESET();

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                         const mbedtls_sha256_context *src )
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
void mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 )
{
    /* HASH IP initialization */
    HAL_HASH_DeInit(&ctx->hhash_sha256);
    
    /* HASH Configuration */
    ctx->hhash_sha256.Init.DataType = HASH_DATATYPE_8B;
    HAL_HASH_Init(&ctx->hhash_sha256);

    ctx->is224 = is224;
}

void mbedtls_sha256_process( mbedtls_sha256_context *ctx, const unsigned char data[64] )
{
    if (ctx->is224 == 0)
        HAL_HASHEx_SHA256_Accumulate(&ctx->hhash_sha256, (uint8_t *) data, 64);
    else
        HAL_HASHEx_SHA224_Accumulate(&ctx->hhash_sha256, (uint8_t *) data, 64);
}

/*
 * SHA-256 process buffer
 */
void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen )
{
    if (ctx->is224 == 0)
        HAL_HASHEx_SHA256_Accumulate(&ctx->hhash_sha256, (uint8_t *)input, ilen);
    else
        HAL_HASHEx_SHA224_Accumulate(&ctx->hhash_sha256, (uint8_t *)input, ilen);
}

/*
 * SHA-256 final digest
 */
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] )
{
    __HAL_HASH_START_DIGEST();
        
    if (ctx->is224 == 0)
        HAL_HASHEx_SHA256_Finish(&ctx->hhash_sha256, output, 10);
    else
        HAL_HASHEx_SHA224_Finish(&ctx->hhash_sha256, output, 10);
}

#endif /*MBEDTLS_SHA256_ALT*/
