/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019-2020, STMicroelectronics, All Rights Reserved
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
 *  This file implements STMicroelectronics SHA256 with HW services based on API
 *  from mbed TLS
 */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/sha256.h"

#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SHA256_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA256_BAD_INPUT_DATA )
#define SHA256_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    SHA256_VALIDATE(ctx != NULL);

    __disable_irq();
#if defined(MBEDTLS_THREADING_C)
    /* mutex cannot be initialized twice */
    if (!hash_mutex_started) {
        mbedtls_mutex_init(&hash_mutex);
        hash_mutex_started = 1;
    }
#endif /* MBEDTLS_THREADING_C */
    hash_context_count++;
    __enable_irq();

    hash_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    __disable_irq();
    if (hash_context_count > 0) {
        hash_context_count--;
    }

#if defined(MBEDTLS_THREADING_C)
    if (hash_mutex_started) {
        mbedtls_mutex_free(&hash_mutex);
        hash_mutex_started = 0;
    }
#endif /* MBEDTLS_THREADING_C */
    __enable_irq();

    /* Shut down HASH on last context */
    if (hash_context_count == 0) {
        HAL_HASH_DeInit(&ctx->hhash);
    }

    hash_zeroize(ctx, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
    SHA256_VALIDATE(dst != NULL);
    SHA256_VALIDATE(src != NULL);

    *dst = *src;
}

int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    int ret = 0;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(is224 == 0 || is224 == 1);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* HASH Configuration */
    if (HAL_HASH_DeInit(&ctx->hhash) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }
    ctx->hhash.Init.DataType = HASH_DATATYPE_8B;
    if (HAL_HASH_Init(&ctx->hhash) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    ctx->is224 = is224;

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

exit :
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&hash_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx,
                                    const unsigned char data[ST_SHA256_BLOCK_SIZE])
{
    int ret = 0;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((const unsigned char *)data != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    if (ctx->is224 == 0) {
        if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash,
                                     (uint8_t *) data,
                                     ST_SHA256_BLOCK_SIZE) != 0) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    } else {
        if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash,
                                     (uint8_t *) data,
                                     ST_SHA256_BLOCK_SIZE) != 0) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

exit :
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&hash_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx,
                              const unsigned char *input,
                              size_t ilen)
{
    int ret = 0;
    size_t currentlen = ilen;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(ilen == 0 || input != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    if (currentlen < (ST_SHA256_BLOCK_SIZE - ctx->sbuf_len)) {
        /* only store input data in context buffer */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
        ctx->sbuf_len += currentlen;
    } else {
        /* fill context buffer until ST_SHA256_BLOCK_SIZE bytes, and process it */
        memcpy(ctx->sbuf + ctx->sbuf_len,
               input,
               (ST_SHA256_BLOCK_SIZE - ctx->sbuf_len));
        currentlen -= (ST_SHA256_BLOCK_SIZE - ctx->sbuf_len);

        if (ctx->is224 == 0) {
            if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash,
                                         (uint8_t *)(ctx->sbuf),
                                         ST_SHA256_BLOCK_SIZE) != 0) {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        } else {
            if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash,
                                         (uint8_t *)(ctx->sbuf),
                                         ST_SHA256_BLOCK_SIZE) != 0) {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        }

        /* Process following input data
                     with size multiple of ST_SHA256_BLOCK_SIZE bytes */
        size_t iter = currentlen / ST_SHA256_BLOCK_SIZE;
        if (iter != 0) {
            if (ctx->is224 == 0) {
                if (HAL_HASHEx_SHA256_Accmlt(&ctx->hhash,
                                             (uint8_t *)(input + ST_SHA256_BLOCK_SIZE - ctx->sbuf_len),
                                             (iter * ST_SHA256_BLOCK_SIZE)) != 0) {
                    ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                    goto exit;
                }
            } else {
                if (HAL_HASHEx_SHA224_Accmlt(&ctx->hhash,
                                             (uint8_t *)(input + ST_SHA256_BLOCK_SIZE - ctx->sbuf_len),
                                             (iter * ST_SHA256_BLOCK_SIZE)) != 0) {
                    ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                    goto exit;
                }
            }
        }

        /* Store only the remaining input data
                        up to (ST_SHA256_BLOCK_SIZE - 1) bytes */
        ctx->sbuf_len = currentlen % ST_SHA256_BLOCK_SIZE;
        if (ctx->sbuf_len != 0) {
            memcpy(ctx->sbuf, input + ilen - ctx->sbuf_len, ctx->sbuf_len);
        }
    }

    /* save hw context */
    HAL_HASH_ContextSaving(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

exit :
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&hash_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx,
                              unsigned char output[32])
{
    int ret = 0;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((unsigned char *)output != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    /* Last accumulation for pending bytes in sbuf_len,
                             then trig processing and get digest */
    if (ctx->is224 == 0) {
        if (HAL_HASHEx_SHA256_Accmlt_End(&ctx->hhash,
                                         ctx->sbuf,
                                         ctx->sbuf_len,
                                         output,
                                         ST_HASH_TIMEOUT) != 0) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    } else {
        if (HAL_HASHEx_SHA224_Accmlt_End(&ctx->hhash,
                                         ctx->sbuf,
                                         ctx->sbuf_len,
                                         output,
                                         ST_HASH_TIMEOUT) != 0) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    }

    ctx->sbuf_len = 0;

exit :
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&hash_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

#endif /* MBEDTLS_SHA256_ALT*/
#endif /* MBEDTLS_SHA256_C */
