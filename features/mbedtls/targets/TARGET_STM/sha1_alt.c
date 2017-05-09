/*
 *  sha1_alt.c for SHA1 HASH
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
#include "mbedtls/sha1.h"
#if defined(MBEDTLS_SHA1_ALT)
#include "mbedtls/platform.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = (unsigned char*)v; while( n-- ) *p++ = 0;
}

/* mbedtls_sha1_store will store in ctx->sbuf size new values located at *ptr */
/* wether ctx->sbuf already contains something or not */
static void mbedtls_sha1_store( mbedtls_sha1_context *ctx, uint8_t *ptr, unsigned char size)
{
    if (ctx->sbuf == NULL) { // new allocation
        ctx->sbuf = malloc(size);
    } else { // realloc
        ctx->sbuf = realloc(ptr, size);
    }
    if (ctx->sbuf !=NULL) { // allocation occured
        memcpy(ctx->sbuf, ptr, size);
        ctx->flag = 1;
        ctx->sbuf_len += size;
    }
}

/* mbedtls_sha1_clear_ctxbuf will clear the ctx buff, free memory  */
static void mbedtls_sha1_clear_ctxbuf( mbedtls_sha1_context *ctx)
{
    ctx->flag=0;
    mbedtls_zeroize( ctx->sbuf, ctx->sbuf_len);
    free(ctx->sbuf);
    ctx->sbuf = NULL;
    ctx->sbuf_len = 0;

}

void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();

    ctx->flag=0;
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if( ctx == NULL )
        return;

	/* Force the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_FORCE_RESET();

    /* Release the HASH Periheral Clock Reset */
    __HAL_RCC_HASH_RELEASE_RESET();

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_clone( mbedtls_sha1_context *dst,
                         const mbedtls_sha1_context *src )
{
    *dst = *src;
}

/*
 * SHA-1 context setup
 */
void mbedtls_sha1_starts( mbedtls_sha1_context *ctx )
{
    /* Deinitializes the HASH peripheral */
    if (HAL_HASH_DeInit(&ctx->hhash_sha1) == HAL_ERROR) {
        // error found to be returned
        return;
    }

    /* HASH Configuration */
    ctx->hhash_sha1.Init.DataType = HASH_DATATYPE_8B;
    if (HAL_HASH_Init(&ctx->hhash_sha1) == HAL_ERROR) {
        // error found to be returned
        return;
    }

    ctx->flag=0;
}

void mbedtls_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] )
{
	HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *) data, 64);
}

/*
 * SHA-1 process buffer
 */
void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen )
{
    unsigned char *intermediate_buf=NULL;
    unsigned char modulus=0;
    unsigned char buf_len=0;
    // Accumulate cannot be called for a size <4 unless it is the last call
    modulus = ilen % 4;

    if (ilen <4) {
        mbedtls_sha1_store(ctx, (uint8_t *)input, ilen);
    } else {
        if (modulus !=0) {
            buf_len = ilen - modulus;
            HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *)input, buf_len);
            mbedtls_sha1_store(ctx, (uint8_t *)(input+buf_len), modulus);
        } else {
            if (ctx->flag==0)
                HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *)input, ilen);
            else {
                intermediate_buf=malloc(ilen + ctx->sbuf_len);
                memcpy(intermediate_buf, ctx->sbuf, ctx->sbuf_len);
                memcpy(intermediate_buf+ctx->sbuf_len, input, ilen);
                HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, intermediate_buf, ilen+ctx->sbuf_len);
                mbedtls_zeroize( intermediate_buf, (ilen + ctx->sbuf_len ) );
                free(intermediate_buf);
                intermediate_buf = NULL;
                mbedtls_sha1_clear_ctxbuf(ctx);
            }
        }
    }
}

/*
 * SHA-1 final digest
 */
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] )
{

    if (ctx->flag == 1) {
        HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, ctx->sbuf, ctx->sbuf_len);
        mbedtls_sha1_clear_ctxbuf(ctx);
    }

    __HAL_HASH_START_DIGEST();

    if (HAL_HASH_SHA1_Finish(&ctx->hhash_sha1, output, 10)){
        // error code to be returned
    }
}

#endif /*MBEDTLS_SHA1_ALT*/
