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
#if defined(MBEDTLS_MD5_C)

#if defined(MBEDTLS_MD5_ALT)
#include "mbedtls/platform.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

static int st_md5_restore_hw_context(mbedtls_md5_context *ctx)
{
    uint32_t i;
    uint32_t tickstart;
    /* allow multi-instance of HASH use: save context for HASH HW module CR */
    /* Check that there is no HASH activity on going */
    tickstart = HAL_GetTick();
    while ((HASH->SR & (HASH_FLAG_BUSY | HASH_FLAG_DMAS)) != 0) {
        if ((HAL_GetTick() - tickstart) > ST_MD5_TIMEOUT) {
            return 0; // timeout: HASH processor is busy
        }
    }
    HASH->STR = ctx->ctx_save_str;
    HASH->CR = (ctx->ctx_save_cr | HASH_CR_INIT);
    for (i=0;i<38;i++) {
        HASH->CSR[i] = ctx->ctx_save_csr[i];
    }
    return 1;
}

static int st_md5_save_hw_context(mbedtls_md5_context *ctx)
{
    uint32_t i;
    uint32_t tickstart;
    /* Check that there is no HASH activity on going */
    tickstart = HAL_GetTick();
    while ((HASH->SR & (HASH_FLAG_BUSY | HASH_FLAG_DMAS)) != 0) {
        if ((HAL_GetTick() - tickstart) > ST_MD5_TIMEOUT) {
            return 0; // timeout: HASH processor is busy
        }
    }
    /* allow multi-instance of HASH use: restore context for HASH HW module CR */
    ctx->ctx_save_cr = HASH->CR;
    ctx->ctx_save_str = HASH->STR;
    for (i=0;i<38;i++) {
        ctx->ctx_save_csr[i] = HASH->CSR[i];
    }
    return 1;
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
    mbedtls_zeroize( ctx, sizeof( mbedtls_md5_context ) );
}

void mbedtls_md5_clone( mbedtls_md5_context *dst,
                        const mbedtls_md5_context *src )
{
    *dst = *src;
}

int mbedtls_md5_starts_ret( mbedtls_md5_context *ctx )
{
    /* HASH IP initialization */
    if (HAL_HASH_DeInit(&ctx->hhash_md5) != 0) {
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }

    /* HASH Configuration */
    ctx->hhash_md5.Init.DataType = HASH_DATATYPE_8B;
    /* clear CR ALGO value */
    HASH->CR &= ~HASH_CR_ALGO_Msk;
    if (HAL_HASH_Init(&ctx->hhash_md5) != 0) {
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    if (st_md5_save_hw_context(ctx) != 1) {
        // Return HASH_BUSY timeout error here
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_internal_md5_process( mbedtls_md5_context *ctx, const unsigned char data[ST_MD5_BLOCK_SIZE] )
{
    if (st_md5_restore_hw_context(ctx) != 1) {
        // Return HASH_BUSY timeout error here
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    if (HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, (uint8_t *)data, ST_MD5_BLOCK_SIZE) != 0) {
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    if (st_md5_save_hw_context(ctx) != 1) {
        // Return HASH_BUSY timeout error here
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_md5_update_ret( mbedtls_md5_context *ctx, const unsigned char *input, size_t ilen )
{
    int err;
    size_t currentlen = ilen;
    /* If ilen = 0 : do nothing */
    if (currentlen != 0) {
        if (st_md5_restore_hw_context(ctx) != 1) {
            // Return HASH_BUSY timeout error here
            return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
        }

        // store mechanism to accumulate ST_MD5_BLOCK_SIZE bytes (512 bits) in the HW
        if (currentlen < (ST_MD5_BLOCK_SIZE - ctx->sbuf_len)) {
            // only buffurize
            memcpy(ctx->sbuf+ctx->sbuf_len, input, currentlen);
            ctx->sbuf_len += currentlen;
        } else {
            // fill buffer and process it
            memcpy(ctx->sbuf + ctx->sbuf_len, input, (ST_MD5_BLOCK_SIZE - ctx->sbuf_len));
            currentlen -= (ST_MD5_BLOCK_SIZE - ctx->sbuf_len);
            err = mbedtls_internal_md5_process(ctx, ctx->sbuf);
            if (err != 0) {
                return err;
            }
            // Process every input as long as it is %64 bytes, ie 512 bits
            size_t iter = currentlen / ST_MD5_BLOCK_SIZE;
            if (iter !=0) {
                if (HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, (uint8_t *)(input + ST_MD5_BLOCK_SIZE - ctx->sbuf_len), (iter * ST_MD5_BLOCK_SIZE)) != 0) {
                    return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
                }
            }
            // sbuf is completely accumulated, now copy up to 63 remaining bytes
            ctx->sbuf_len = currentlen % ST_MD5_BLOCK_SIZE;
            if (ctx->sbuf_len !=0) {
                memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
            }
        }

        if (st_md5_save_hw_context(ctx) != 1) {
            // Return HASH_BUSY timeout error here
            return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
        }
    }
    return 0;
}

int mbedtls_md5_finish_ret( mbedtls_md5_context *ctx, unsigned char output[16] )
{
    if (st_md5_restore_hw_context(ctx) != 1) {
        // Return HASH_BUSY timeout error here
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    /* Last accumulation for extra bytes in sbuf_len */
    /* This sets HW flags in case mbedtls_md5_update has not been called yet */
    if (HAL_HASH_MD5_Accumulate(&ctx->hhash_md5, ctx->sbuf, ctx->sbuf_len) != 0) {
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }

    mbedtls_zeroize( ctx->sbuf, ST_MD5_BLOCK_SIZE);
    ctx->sbuf_len = 0;
    __HAL_HASH_START_DIGEST();

    if (HAL_HASH_MD5_Finish(&ctx->hhash_md5, output, 10)) {
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    if (st_md5_save_hw_context(ctx) != 1) {
        // Return HASH_BUSY timeout error here
        return MBEDTLS_ERR_MD5_HW_ACCEL_FAILED;
    }
    return 0;
}

#endif /* MBEDTLS_MD5_ALT */
#endif /* MBEDTLS_MD5_C */
