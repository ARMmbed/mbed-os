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
 *  This file implements STMicroelectronics MD5 with HW services based on API
 *  from mbed TLS
 */
/*
 *  The MD5 algorithm was designed by Ron Rivest in 1991.
 *
 *  http://www.ietf.org/rfc/rfc1321.txt
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/md5.h"

#if defined(MBEDTLS_MD5_C)
#if defined(MBEDTLS_MD5_ALT)
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MD5_VALIDATE_RET(cond)                             \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_MD5_BAD_INPUT_DATA )
#define MD5_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void mbedtls_md5_init(mbedtls_md5_context *ctx)
{
    MD5_VALIDATE(ctx != NULL);

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

    hash_zeroize(ctx, sizeof(mbedtls_md5_context));
}

void mbedtls_md5_free(mbedtls_md5_context *ctx)
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

    hash_zeroize(ctx, sizeof(mbedtls_md5_context));
}

void mbedtls_md5_clone(mbedtls_md5_context *dst,
                       const mbedtls_md5_context *src)
{
    MD5_VALIDATE(dst != NULL);
    MD5_VALIDATE(src != NULL);

    *dst = *src;
}

int mbedtls_md5_starts_ret(mbedtls_md5_context *ctx)
{
    int ret = 0;

    MD5_VALIDATE_RET(ctx != NULL);

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

int mbedtls_internal_md5_process(mbedtls_md5_context *ctx,
                                 const unsigned char data[ST_MD5_BLOCK_SIZE])
{
    int ret = 0;

    MD5_VALIDATE_RET(ctx != NULL);
    MD5_VALIDATE_RET((const unsigned char *)data != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    if (HAL_HASH_MD5_Accmlt(&ctx->hhash,
                            (uint8_t *) data,
                            ST_MD5_BLOCK_SIZE) != 0) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
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

int mbedtls_md5_update_ret(mbedtls_md5_context *ctx,
                           const unsigned char *input,
                           size_t ilen)
{
    int ret = 0;
    size_t currentlen = ilen;

    MD5_VALIDATE_RET(ctx != NULL);
    MD5_VALIDATE_RET(ilen == 0 || input != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    if (currentlen < (ST_MD5_BLOCK_SIZE - ctx->sbuf_len)) {
        /* only store input data in context buffer */
        memcpy(ctx->sbuf + ctx->sbuf_len, input, currentlen);
        ctx->sbuf_len += currentlen;
    } else {
        /* fill context buffer until ST_MD5_BLOCK_SIZE bytes, and process it */
        memcpy(ctx->sbuf + ctx->sbuf_len,
               input,
               (ST_MD5_BLOCK_SIZE - ctx->sbuf_len));
        currentlen -= (ST_MD5_BLOCK_SIZE - ctx->sbuf_len);

        if (HAL_HASH_MD5_Accmlt(&ctx->hhash,
                                (uint8_t *)(ctx->sbuf),
                                ST_MD5_BLOCK_SIZE) != 0) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }

        /* Process following input data
                      with size multiple of ST_MD5_BLOCK_SIZE bytes */
        size_t iter = currentlen / ST_MD5_BLOCK_SIZE;
        if (iter != 0) {
            if (HAL_HASH_MD5_Accmlt(&ctx->hhash,
                                    (uint8_t *)(input + ST_MD5_BLOCK_SIZE - ctx->sbuf_len),
                                    (iter * ST_MD5_BLOCK_SIZE)) != 0) {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        }

        /* Store only the remaining input data
                        up to (ST_MD5_BLOCK_SIZE - 1) bytes */
        ctx->sbuf_len = currentlen % ST_MD5_BLOCK_SIZE;
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

int mbedtls_md5_finish_ret(mbedtls_md5_context *ctx, unsigned char output[32])
{
    int ret = 0;

    MD5_VALIDATE_RET(ctx != NULL);
    MD5_VALIDATE_RET((unsigned char *)output != NULL);

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&hash_mutex)) != 0) {
        return (ret);
    }
#endif /* MBEDTLS_THREADING_C */

    /* restore hw context */
    HAL_HASH_ContextRestoring(&ctx->hhash, (uint8_t *)ctx->ctx_save_regs);

    /* Last accumulation for pending bytes in sbuf_len,
                             then trig processing and get digest */
    if (HAL_HASH_MD5_Accmlt_End(&ctx->hhash,
                                ctx->sbuf,
                                ctx->sbuf_len,
                                output,
                                ST_HASH_TIMEOUT) != 0) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
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

#endif /* MBEDTLS_MD5_ALT*/
#endif /* MBEDTLS_MD5_C */
