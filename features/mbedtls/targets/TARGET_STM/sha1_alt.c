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
static void mbedtls_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}

static int st_sha1_restore_hw_context(mbedtls_sha1_context *ctx)
{
    uint32_t i;
    uint32_t tickstart;
    /* allow multi-instance of HASH use: save context for HASH HW module CR */
    /* Check that there is no HASH activity on going */
    tickstart = HAL_GetTick();
    while ((HASH->SR & (HASH_FLAG_BUSY | HASH_FLAG_DMAS)) != 0) {
        if ((HAL_GetTick() - tickstart) > ST_SHA1_TIMEOUT) {
            return 0; // timeout: HASH processor is busy
        }
    }
    HASH->STR = ctx->ctx_save_str;
    HASH->CR = (ctx->ctx_save_cr | HASH_CR_INIT);
    for (i = 0; i < 38; i++) {
        HASH->CSR[i] = ctx->ctx_save_csr[i];
    }
    return 1;
}

static int st_sha1_save_hw_context(mbedtls_sha1_context *ctx)
{
    uint32_t i;
    uint32_t tickstart;
    /* Check that there is no HASH activity on going */
    tickstart = HAL_GetTick();
    while ((HASH->SR & (HASH_FLAG_BUSY | HASH_FLAG_DMAS)) != 0) {
        if ((HAL_GetTick() - tickstart) > ST_SHA1_TIMEOUT) {
            return 0; // timeout: HASH processor is busy
        }
    }
    /* allow multi-instance of HASH use: restore context for HASH HW module CR */
    ctx->ctx_save_cr = HASH->CR;
    ctx->ctx_save_str = HASH->STR;
    for (i = 0; i < 38; i++) {
        ctx->ctx_save_csr[i] = HASH->CSR[i];
    }
    return 1;
}

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    mbedtls_zeroize(ctx, sizeof(mbedtls_sha1_context));

    /* Enable HASH clock */
    __HAL_RCC_HASH_CLK_ENABLE();

}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL) {
        return;
    }
    mbedtls_zeroize(ctx, sizeof(mbedtls_sha1_context));
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src)
{
    *dst = *src;
}

int mbedtls_sha1_starts_ret(mbedtls_sha1_context *ctx)
{
    /* Deinitializes the HASH peripheral */
    if (HAL_HASH_DeInit(&ctx->hhash_sha1) == HAL_ERROR) {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }

    /* HASH Configuration */
    ctx->hhash_sha1.Init.DataType = HASH_DATATYPE_8B;
    /* clear CR ALGO value */
    HASH->CR &= ~HASH_CR_ALGO_Msk;
    if (HAL_HASH_Init(&ctx->hhash_sha1) == HAL_ERROR) {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    if (st_sha1_save_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_internal_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[ST_SHA1_BLOCK_SIZE])
{
    if (st_sha1_restore_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    if (HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *) data, ST_SHA1_BLOCK_SIZE) != 0) {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }

    if (st_sha1_save_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_sha1_update_ret(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    int err;
    size_t currentlen = ilen;
    if (st_sha1_restore_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }

    // store mechanism to accumulate ST_SHA1_BLOCK_SIZE bytes (512 bits) in the HW
    if (currentlen == 0) { // only change HW status is size if 0
        if (ctx->hhash_sha1.Phase == HAL_HASH_PHASE_READY) {
            /* Select the SHA1 mode and reset the HASH processor core, so that the HASH will be ready to compute
               the message digest of a new message */
            HASH->CR |= HASH_ALGOSELECTION_SHA1 | HASH_CR_INIT;
        }
        ctx->hhash_sha1.Phase = HAL_HASH_PHASE_PROCESS;
    } else if (currentlen < (ST_SHA1_BLOCK_SIZE - ctx->sbuf_len)) {
        // only buffurize
        memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
        ctx->sbuf_len += currentlen;
    } else {
        // fill buffer and process it
        memcpy(ctx->sbuf + ctx->sbuf_len, input, (ST_SHA1_BLOCK_SIZE - ctx->sbuf_len));
        currentlen -= (ST_SHA1_BLOCK_SIZE - ctx->sbuf_len);
        err = mbedtls_internal_sha1_process(ctx, ctx->sbuf);
        if (err != 0) {
            return err;
        }
        // Process every input as long as it is %64 bytes, ie 512 bits
        size_t iter = currentlen / ST_SHA1_BLOCK_SIZE;
        if (HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, (uint8_t *)(input + ST_SHA1_BLOCK_SIZE - ctx->sbuf_len), (iter * ST_SHA1_BLOCK_SIZE)) != 0) {
            return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
        }
        // sbuf is completely accumulated, now copy up to 63 remaining bytes
        ctx->sbuf_len = currentlen % ST_SHA1_BLOCK_SIZE;
        if (ctx->sbuf_len != 0) {
            memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
        }
    }
    if (st_sha1_save_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_sha1_finish_ret(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    if (st_sha1_restore_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }

    /* Last accumulation for extra bytes in sbuf_len */
    /* This allows the HW flags to be in place in case mbedtls_sha256_update has not been called yet */
    if (HAL_HASH_SHA1_Accumulate(&ctx->hhash_sha1, ctx->sbuf, ctx->sbuf_len) != 0) {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    mbedtls_zeroize(ctx->sbuf, ST_SHA1_BLOCK_SIZE);
    ctx->sbuf_len = 0;
    __HAL_HASH_START_DIGEST();

    if (HAL_HASH_SHA1_Finish(&ctx->hhash_sha1, output, 10) != 0) {
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    if (st_sha1_save_hw_context(ctx) != 1) {
        // return HASH_BUSY timeout Error here
        return MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
    }
    return 0;
}

#endif /*MBEDTLS_SHA1_ALT*/
