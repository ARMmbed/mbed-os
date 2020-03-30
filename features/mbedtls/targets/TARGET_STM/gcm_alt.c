/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019-2020 STMicroelectronics, All Rights Reserved
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
 *  This file implements ST GCM HW services based on API from mbed TLS
 */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls/gcm.h"

#if defined(MBEDTLS_GCM_C)
#if defined(MBEDTLS_GCM_ALT)

#include <string.h>

#include "mbedtls/platform_util.h"
#include "mbedtls/platform.h"


/* Parameter validation macros */
#define GCM_VALIDATE_RET( cond ) \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_GCM_BAD_INPUT )
#define GCM_VALIDATE( cond ) \
    MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define IV_LENGTH        12U   /* implementations restrict support to 96 bits */

#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
#define AAD_WORD_ALIGN   4U   /* implementations restrict AAD support on a    */
/* buffer multiple of 32 bits                   */
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * Initialize a context
 */
void mbedtls_gcm_init(mbedtls_gcm_context *ctx)
{
    GCM_VALIDATE(ctx != NULL);

    __disable_irq();
#if defined(MBEDTLS_THREADING_C)
    /* mutex cannot be initialized twice */
    if (!cryp_mutex_started) {
        mbedtls_mutex_init(&cryp_mutex);
        cryp_mutex_started = 1;
    }
#endif /* MBEDTLS_THREADING_C */
    cryp_context_count++;
    __enable_irq();

    cryp_zeroize((void *)ctx, sizeof(mbedtls_gcm_context));
}

int mbedtls_gcm_setkey(mbedtls_gcm_context *ctx,
                       mbedtls_cipher_id_t cipher,
                       const unsigned char *key,
                       unsigned int keybits)
{
    unsigned int i;
    int ret = 0;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(key != NULL);

    /* Protect context access                                  */
    /* (it may occur at a same time in a threaded environment) */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&cryp_mutex) != 0) {
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);
    }
#endif /* MBEDTLS_THREADING_C */

    switch (keybits) {
        case 128:
            ctx->hcryp_gcm.Init.KeySize = CRYP_KEYSIZE_128B;
            break;

        case 192:
#if ( USE_AES_KEY192 == 1 )
            ctx->hcryp_gcm.Init.KeySize = CRYP_KEYSIZE_192B;
            break;
#else
            ret = MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
            goto exit;
#endif /* USE_AES_KEY192 */

        case 256:
            ctx->hcryp_gcm.Init.KeySize = CRYP_KEYSIZE_256B;
            break;

        default :
            ret = MBEDTLS_ERR_GCM_BAD_INPUT;
            goto exit;
    }

    /* Format and fill AES key  */
    for (i = 0; i < (keybits / 32); i++) {
        GET_UINT32_BE(ctx->gcm_key[i], key, 4 * i);
    }

    /* include the appropriate instance name */
#if defined (AES)
    ctx->hcryp_gcm.Instance = AES;
    ctx->hcryp_gcm.Init.Algorithm  = CRYP_AES_GCM_GMAC;
#elif defined (AES1)
    ctx->hcryp_gcm.Instance = AES1;
    ctx->hcryp_gcm.Init.Algorithm  = CRYP_AES_GCM_GMAC;
#else /* CRYP */
    ctx->hcryp_gcm.Instance = CRYP;
    ctx->hcryp_gcm.Init.Algorithm  = CRYP_AES_GCM;
#endif /* AES */

    ctx->hcryp_gcm.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_gcm.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;
    ctx->hcryp_gcm.Init.pKey = ctx->gcm_key;

    if (HAL_CRYP_Init(&ctx->hcryp_gcm) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

exit :
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&cryp_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_gcm_starts(mbedtls_gcm_context *ctx,
                       int mode,
                       const unsigned char *iv,
                       size_t iv_len,
                       const unsigned char *add,
                       size_t add_len)
{
    int ret = 0;
    unsigned int i;
    __ALIGN_BEGIN static uint32_t iv_32B[4] __ALIGN_END;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(mode != MBEDTLS_GCM_ENCRYPT || mode != MBEDTLS_GCM_DECRYPT);
    GCM_VALIDATE_RET(iv != NULL);
    GCM_VALIDATE_RET(add_len == 0 || add != NULL);

    /* IV and AD are limited to 2^64 bits, so 2^61 bytes */
    /* IV is not allowed to be zero length */
    if (iv_len == 0 ||
            ((uint64_t) iv_len) >> 61 != 0 ||
            ((uint64_t) add_len) >> 61 != 0) {
        return (MBEDTLS_ERR_GCM_BAD_INPUT);
    }

    /* implementation restrict support to the length of 96 bits */
    if (IV_LENGTH != iv_len) {
        return (MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED);
    }

#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
    /* implementation restrict support to a buffer multiple of 32 bits */
    if ((add_len % AAD_WORD_ALIGN) != 0U) {
        return (MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED);
    }
#endif

    /* Protect context access                                  */
    /* (it may occur at a same time in a threaded environment) */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&cryp_mutex) != 0) {
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);
    }
#endif /* MBEDTLS_THREADING_C */

    if (HAL_CRYP_Init(&ctx->hcryp_gcm) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    ctx->mode = mode;
    ctx->len = 0;

    /* Set IV with invert endianness */
    for (i = 0; i < 3; i++) {
        GET_UINT32_BE(iv_32B[i], iv, 4 * i);
    }

    /* According to NIST specification, the counter value is 0x2 when
       processing the first block of payload */
    iv_32B[3] = 0x00000002;

    ctx->hcryp_gcm.Init.pInitVect = iv_32B;

    if (add_len > 0) {
        ctx->hcryp_gcm.Init.Header = (uint32_t *)add;
#if defined(STM32_AAD_ANY_LENGTH_SUPPORT)
        /* header buffer in byte length */
        ctx->hcryp_gcm.Init.HeaderSize = (uint32_t)add_len;
#else
        /* header buffer in word length */
        ctx->hcryp_gcm.Init.HeaderSize = (uint32_t)(add_len / AAD_WORD_ALIGN);
#endif
    } else {
        ctx->hcryp_gcm.Init.Header = NULL;
        ctx->hcryp_gcm.Init.HeaderSize = 0;
    }

#if defined(STM32_AAD_ANY_LENGTH_SUPPORT)
    /* Additional Authentication Data in bytes unit */
    ctx->hcryp_gcm.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_BYTE;
#endif

    /* Do not Allow IV reconfiguration at every gcm update */
    ctx->hcryp_gcm.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ONCE;

    /* reconfigure the CRYP */
    if (HAL_CRYP_SetConfig(&ctx->hcryp_gcm, &ctx->hcryp_gcm.Init) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

exit:
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&cryp_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_gcm_update(mbedtls_gcm_context *ctx,
                       size_t length,
                       const unsigned char *input,
                       unsigned char *output)
{
    int ret = 0;
#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
    size_t rest_length = 0;
#endif

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(length == 0 || input != NULL);
    GCM_VALIDATE_RET(length == 0 || output != NULL);

    if (output > input && (size_t)(output - input) < length) {
        return (MBEDTLS_ERR_GCM_BAD_INPUT);
    }

    /* Total length is restricted to 2^39 - 256 bits, ie 2^36 - 2^5 bytes
     * Also check for possible overflow */
    if (((ctx->len + length) < ctx->len) ||
            ((uint64_t)(ctx->len + length) > 0xFFFFFFFE0ull)) {
        return (MBEDTLS_ERR_GCM_BAD_INPUT);
    }

    /* Protect context access                                  */
    /* (it may occur at a same time in a threaded environment) */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&cryp_mutex) != 0) {
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);
    }
#endif /* MBEDTLS_THREADING_C */

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    ctx->len += length;

#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
    /* compute remaining data (data buffer in word) */
    if ((length % AAD_WORD_ALIGN) != 0U) {
        rest_length = length % AAD_WORD_ALIGN;
    }
#endif

    if (ctx->mode == MBEDTLS_GCM_DECRYPT) {
        if (HAL_CRYP_Decrypt(&ctx->hcryp_gcm,
                             (uint32_t *)input,
                             length,
                             (uint32_t *)output,
                             ST_CRYP_TIMEOUT) != HAL_OK) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
        /* manage last bytes */
        if (rest_length != 0) {
            if (HAL_CRYP_Decrypt(&ctx->hcryp_gcm,
                                 (uint32_t *)(input + (length / AAD_WORD_ALIGN)),
                                 rest_length,
                                 (uint32_t *)(output + (length / AAD_WORD_ALIGN)),
                                 ST_CRYP_TIMEOUT) != HAL_OK) {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        }
#endif
    } else {
        if (HAL_CRYP_Encrypt(&ctx->hcryp_gcm,
                             (uint32_t *)input,
                             length,
                             (uint32_t *)output,
                             ST_CRYP_TIMEOUT) != HAL_OK) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
#if !defined(STM32_AAD_ANY_LENGTH_SUPPORT)
        /* manage last bytes */
        if (rest_length != 0) {
            if (HAL_CRYP_Encrypt(&ctx->hcryp_gcm,
                                 (uint32_t *)(input + (length / AAD_WORD_ALIGN)),
                                 rest_length,
                                 (uint32_t *)(output + (length / AAD_WORD_ALIGN)),
                                 ST_CRYP_TIMEOUT) != HAL_OK) {
                ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
                goto exit;
            }
        }
#endif
    }

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

exit:
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&cryp_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_gcm_finish(mbedtls_gcm_context *ctx,
                       unsigned char *tag,
                       size_t tag_len)
{
    int ret = 0;
    __ALIGN_BEGIN uint8_t mac[16]      __ALIGN_END; /* temporary mac         */

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(tag != NULL);

    if (tag_len > 16 || tag_len < 4) {
        return (MBEDTLS_ERR_GCM_BAD_INPUT);
    }

    /* Protect context access                                  */
    /* (it may occur at a same time in a threaded environment) */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_lock(&cryp_mutex) != 0) {
        return (MBEDTLS_ERR_THREADING_MUTEX_ERROR);
    }
#endif /* MBEDTLS_THREADING_C */

    /* allow multi-context of CRYP use: restore context */
    ctx->hcryp_gcm.Instance->CR = ctx->ctx_save_cr;

    /* Tag has a variable length */
    memset(mac, 0, sizeof(mac));

    /* Generate the authentication TAG */
    if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&ctx->hcryp_gcm,
                                          (uint32_t *)mac,
                                          ST_CRYP_TIMEOUT) != HAL_OK) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    memcpy(tag, mac, tag_len);

    /* allow multi-context of CRYP : save context */
    ctx->ctx_save_cr = ctx->hcryp_gcm.Instance->CR;

exit:
    /* Free context access */
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&cryp_mutex) != 0) {
        ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif /* MBEDTLS_THREADING_C */

    return (ret);
}

int mbedtls_gcm_crypt_and_tag(mbedtls_gcm_context *ctx,
                              int mode,
                              size_t length,
                              const unsigned char *iv,
                              size_t iv_len,
                              const unsigned char *add,
                              size_t add_len,
                              const unsigned char *input,
                              unsigned char *output,
                              size_t tag_len,
                              unsigned char *tag)
{
    int ret;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(iv != NULL);
    GCM_VALIDATE_RET(add_len == 0 || add != NULL);
    GCM_VALIDATE_RET(length == 0 || input != NULL);
    GCM_VALIDATE_RET(length == 0 || output != NULL);
    GCM_VALIDATE_RET(tag != NULL);

    if ((ret = mbedtls_gcm_starts(ctx, mode, iv, iv_len, add, add_len)) != 0) {
        return (ret);
    }

    if ((ret = mbedtls_gcm_update(ctx, length, input, output)) != 0) {
        return (ret);
    }

    if ((ret = mbedtls_gcm_finish(ctx, tag, tag_len)) != 0) {
        return (ret);
    }

    return (0);
}

int mbedtls_gcm_auth_decrypt(mbedtls_gcm_context *ctx,
                             size_t length,
                             const unsigned char *iv,
                             size_t iv_len,
                             const unsigned char *add,
                             size_t add_len,
                             const unsigned char *tag,
                             size_t tag_len,
                             const unsigned char *input,
                             unsigned char *output)
{
    int ret;
    unsigned char check_tag[16];
    size_t i;
    int diff;

    GCM_VALIDATE_RET(ctx != NULL);
    GCM_VALIDATE_RET(iv != NULL);
    GCM_VALIDATE_RET(add_len == 0 || add != NULL);
    GCM_VALIDATE_RET(tag != NULL);
    GCM_VALIDATE_RET(length == 0 || input != NULL);
    GCM_VALIDATE_RET(length == 0 || output != NULL);

    if ((ret = mbedtls_gcm_crypt_and_tag(ctx, MBEDTLS_GCM_DECRYPT, length,
                                         iv, iv_len, add, add_len,
                                         input, output, tag_len, check_tag)) != 0) {
        return (ret);
    }

    /* Check tag in "constant-time" */
    for (diff = 0, i = 0; i < tag_len; i++) {
        diff |= tag[i] ^ check_tag[i];
    }

    if (diff != 0) {
        mbedtls_platform_zeroize(output, length);
        return (MBEDTLS_ERR_GCM_AUTH_FAILED);
    }

    return (0);
}

void mbedtls_gcm_free(mbedtls_gcm_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    __disable_irq();
    if (cryp_context_count > 0) {
        cryp_context_count--;
    }

#if defined(MBEDTLS_THREADING_C)
    if (cryp_mutex_started) {
        mbedtls_mutex_free(&cryp_mutex);
        cryp_mutex_started = 0;
    }
#endif /* MBEDTLS_THREADING_C */
    __enable_irq();

    /* Shut down CRYP on last context */
    if (cryp_context_count == 0) {
        HAL_CRYP_DeInit(&ctx->hcryp_gcm);
    }

    cryp_zeroize((void *)ctx, sizeof(mbedtls_gcm_context));
}

#endif /*MBEDTLS_GCM_ALT*/
#endif /*MBEDTLS_GCM_C*/
