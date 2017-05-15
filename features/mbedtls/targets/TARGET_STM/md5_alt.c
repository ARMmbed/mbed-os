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
#include "mbedtls/platform.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

void mbedtls_md5_init( mbedtls_md5_context *ctx )
{
    mbedtls_zeroize( ctx, sizeof( mbedtls_md5_context ) );

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
    if (HAL_HASH_Init(&ctx->hhash_md5) == HAL_ERROR) {
        // return error code
        return;
    }
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
    unsigned char i=0;
    int currentlen = ilen;
    /* store mechanism to handle 64 bytes per 64 bytes */
    if (currentlen == 0){ // change HW status is size if 0
        if(ctx->hhash_md5.Phase == HAL_HASH_PHASE_READY)
        {
          /* Select the MD5 mode and reset the HASH processor core, so that the HASH will be ready to compute
             the message digest of a new message */
          HASH->CR |= HASH_ALGOSELECTION_MD5 | HASH_CR_INIT;
        }
        ctx->hhash_md5.Phase = HAL_HASH_PHASE_PROCESS;
    }
    while ((currentlen+ctx->sbuf_len) >=64) {
        if (ctx->sbuf_len ==0) { /* straight forward */
            mbedtls_md5_process(ctx, input+(i*64));
        } else {
            memcpy(ctx->sbuf+ctx->sbuf_len, input+(i*64),64-ctx->sbuf_len);
            mbedtls_md5_process(ctx, ctx->sbuf);
            memcpy(ctx->sbuf,input+(i+1)*64-ctx->sbuf_len, ctx->sbuf_len);
            // ctx->sbuf_len remains the same
        }
        currentlen -= 64;
        i++;
    }
    if (currentlen <0) {
        currentlen +=64;
    }
    /* Store the remaining <64 values */
    memcpy(ctx->sbuf+ctx->sbuf_len, input+(i*64), currentlen);
    ctx->sbuf_len += currentlen;

}

/*
 * MD5 final digest
 */
void mbedtls_md5_finish( mbedtls_md5_context *ctx, unsigned char output[16] )
{
    if (ctx->sbuf_len > 0) {
        HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, ctx->sbuf, ctx->sbuf_len);
    }
    mbedtls_zeroize( ctx->sbuf, 64);
    ctx->sbuf_len = 0;
    __HAL_HASH_START_DIGEST();

    if (HAL_HASH_MD5_Finish(&ctx->hhash_md5, output, 10)) {
        // error code to be returned
    }
}

#endif /* MBEDTLS_MD5_ALT */
