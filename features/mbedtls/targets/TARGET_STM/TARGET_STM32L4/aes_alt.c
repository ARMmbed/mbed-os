/*
 *  Hardware aes implementation for STM32F4 STM32F7 and STM32L4 families
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

#include <string.h>
#include "mbedtls/aes.h"

#if defined(MBEDTLS_AES_ALT)

#include "mbedtls/platform.h"
//the following defines are provided to maintain compatibility between STM32 families
#define __HAL_RCC_CRYP_CLK_ENABLE    __HAL_RCC_AES_CLK_ENABLE
#define __HAL_RCC_CRYP_FORCE_RESET   __HAL_RCC_AES_FORCE_RESET
#define __HAL_RCC_CRYP_RELEASE_RESET __HAL_RCC_AES_RELEASE_RESET
#define CRYP                         AES

static int aes_set_key(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    switch (keybits) {
        case 128:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B;
            memcpy(ctx->aes_key, key, 16);
            break;
        case 192:
            return (MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED);
        case 256:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B;
            memcpy(ctx->aes_key, key, 32);
            break;
        default :
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_aes.Instance = CRYP;

    /* Deinitializes the CRYP peripheral */
    if (HAL_CRYP_DeInit(&ctx->hcryp_aes) == HAL_ERROR) {
        return (HAL_ERROR);
    }

    /* Enable CRYP clock */
    __HAL_RCC_CRYP_CLK_ENABLE();

    ctx->hcryp_aes.Init.pKey = ctx->aes_key;
    ctx->hcryp_aes.Init.KeyWriteFlag = CRYP_KEY_WRITE_ENABLE;
    if (HAL_CRYP_Init(&ctx->hcryp_aes) == HAL_ERROR) {
        return (HAL_ERROR);
    }

    /* allow multi-instance of CRYP use: save context for CRYP HW module CR */
    ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;
    return (0);

}

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}


void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_aes_context));

}


void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
    if (ctx == NULL) {
        return;
    }
#if defined(DUAL_CORE)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
    }
#endif /* DUAL_CORE */
    /* Force the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_FORCE_RESET();

    /* Release the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_RELEASE_RESET();
#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    mbedtls_zeroize(ctx, sizeof(mbedtls_aes_context));
}


int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    int ret_val = 0;
    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    int ret_val = 0;
    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}


int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{

    /* allow multi-instance of CRYP use: restore context for CRYP hw module */
    ctx->hcryp_aes.Instance->CR = ctx->ctx_save_cr;
    ctx->hcryp_aes.Phase = HAL_CRYP_PHASE_READY;
    ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_aes.Init.pKey = ctx->aes_key;

    if (mode == MBEDTLS_AES_DECRYPT) { /* AES decryption */
        if (mbedtls_internal_aes_decrypt(ctx, input, output)) {
            return ST_ERR_AES_BUSY;
        }
    } else { /* AES encryption */
        if (mbedtls_internal_aes_encrypt(ctx, input, output)) {
            return ST_ERR_AES_BUSY;
        }
    }
    /* allow multi-instance of CRYP use: save context for CRYP HW module CR */
    ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;

    return (0);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
static int st_cbc_restore_context(mbedtls_aes_context *ctx)
{
    uint32_t tickstart;
    tickstart = HAL_GetTick();
    while ((ctx->hcryp_aes.Instance->SR & AES_SR_BUSY) != 0) {
        if ((HAL_GetTick() - tickstart) > ST_AES_TIMEOUT) {
            return ST_ERR_AES_BUSY; // timeout: CRYP processor is busy
        }
    }
    /* allow multi-instance of CRYP use: restore context for CRYP hw module */
    ctx->hcryp_aes.Instance->CR = ctx->ctx_save_cr;
    return 0;
}

static int st_hal_cryp_cbc(mbedtls_aes_context *ctx, uint32_t opmode, size_t length,
                           unsigned char iv[16], uint8_t *input, uint8_t *output)
{
    ctx->hcryp_aes.Init.pInitVect = &iv[0]; // used in process, not in the init
    /* At this moment only, we know we have CBC mode: Re-initialize AES
        IP with proper parameters and apply key and IV for multi context usecase */
    if (HAL_CRYP_DeInit(&ctx->hcryp_aes) != HAL_OK) {
        return ST_ERR_AES_BUSY;
    }
    ctx->hcryp_aes.Init.OperatingMode = opmode;
    ctx->hcryp_aes.Init.ChainingMode = CRYP_CHAINMODE_AES_CBC;
    ctx->hcryp_aes.Init.KeyWriteFlag = CRYP_KEY_WRITE_ENABLE;
    if (HAL_CRYP_Init(&ctx->hcryp_aes) != HAL_OK) {
        return ST_ERR_AES_BUSY;
    }

    if (HAL_CRYPEx_AES(&ctx->hcryp_aes, input, length, output, 10) != 0) {
        return ST_ERR_AES_BUSY;
    }
    return 0;
}

int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    uint32_t tickstart;
    uint32_t *iv_ptr = (uint32_t *)&iv[0];
    if (length % 16) {
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);
    }
    ctx->hcryp_aes.Init.pInitVect = &iv[0];
    if (st_cbc_restore_context(ctx) != 0) {
        return (ST_ERR_AES_BUSY);
    }

    if (mode == MBEDTLS_AES_DECRYPT) {
        if (st_hal_cryp_cbc(ctx, CRYP_ALGOMODE_KEYDERIVATION_DECRYPT, length, iv, (uint8_t *)input, (uint8_t *)output) != 0) {
            return ST_ERR_AES_BUSY;
        }
        /* Save the internal IV vector for multi context purpose */
        tickstart = HAL_GetTick();
        while ((ctx->hcryp_aes.Instance->SR & AES_SR_BUSY) != 0) {
            if ((HAL_GetTick() - tickstart) > ST_AES_TIMEOUT) {
                return ST_ERR_AES_BUSY; // timeout: CRYP processor is busy
            }
        }
        ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR; // save here before overwritten
        ctx->hcryp_aes.Instance->CR &= ~AES_CR_EN;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IVR3;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IVR2;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IVR1;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IVR0;
    } else {
        if (st_hal_cryp_cbc(ctx, CRYP_ALGOMODE_ENCRYPT, length, iv, (uint8_t *)input, (uint8_t *)output) != 0) {
            return ST_ERR_AES_BUSY;
        }
        memcpy(iv, output, 16);   /* current output is the IV vector for the next call */
        ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    int c;
    size_t n = *iv_off;

    if (mode == MBEDTLS_AES_DECRYPT) {
        while (length--) {
            if (n == 0)
                if (mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv) != 0) {
                    return ST_ERR_AES_BUSY;
                }

            c = *input++;
            *output++ = (unsigned char)(c ^ iv[n]);
            iv[n] = (unsigned char) c;

            n = (n + 1) & 0x0F;
        }
    } else {
        while (length--) {
            if (n == 0)
                if (mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv) != 0) {
                    return ST_ERR_AES_BUSY;
                }

            iv[n] = *output++ = (unsigned char)(iv[n] ^ *input++);

            n = (n + 1) & 0x0F;
        }
    }

    *iv_off = n;

    return (0);
}


int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    unsigned char c;
    unsigned char ov[17];

    while (length--) {
        memcpy(ov, iv, 16);
        if (mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv) != 0) {
            return ST_ERR_AES_BUSY;
        }

        if (mode == MBEDTLS_AES_DECRYPT) {
            ov[16] = *input;
        }

        c = *output++ = (unsigned char)(iv[0] ^ *input++);

        if (mode == MBEDTLS_AES_ENCRYPT) {
            ov[16] = c;
        }

        memcpy(iv, ov + 1, 16);
    }

    return (0);
}

#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int c, i;
    size_t n = *nc_off;

    while (length--) {
        if (n == 0) {
            if (mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block) != 0) {
                return ST_ERR_AES_BUSY;
            }

            for (i = 16; i > 0; i--)
                if (++nonce_counter[i - 1] != 0) {
                    break;
                }
        }
        c = *input++;
        *output++ = (unsigned char)(c ^ stream_block[n]);

        n = (n + 1) & 0x0F;
    }

    *nc_off = n;

    return (0);
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    if (HAL_CRYP_AESECB_Encrypt(&ctx->hcryp_aes, (uint8_t *)input, 16, (uint8_t *)output, 10) != HAL_OK) {
        // error found
        return ST_ERR_AES_BUSY;
    }
    return 0;

}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    if (HAL_CRYP_AESECB_Decrypt(&ctx->hcryp_aes, (uint8_t *)input, 16, (uint8_t *)output, 10) != HAL_OK) {
        // error found
        return ST_ERR_AES_BUSY;
    }
    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_aes_encrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
    mbedtls_internal_aes_encrypt(ctx, input, output);
}

void mbedtls_aes_decrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
    mbedtls_internal_aes_decrypt(ctx, input, output);
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */
#endif /*MBEDTLS_AES_ALT*/
