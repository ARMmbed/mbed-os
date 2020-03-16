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

#if MBED_CONF_MBED_TRACE_ENABLE
#define TLSPRINT 1
#endif

static uint32_t swap(uint32_t in)
{
    uint32_t in1, in2, in3, in4, out;

    in1 = ((in & 0xff000000) >> 24);
    in2 = ((in & 0x00FF0000) >> 8);
    in3 = ((in & 0x0000FF00) << 8);
    in4 = ((in & 0xFF) << 24);
    out = in1 | in2 | in3 | in4;

    return out;
}

static int aes_set_key(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
#if TLSPRINT
    mbedtls_printf(" ****** aes_set_key *******\n");
    mbedtls_printf("keybits = %d\n", keybits);
#endif

    switch (keybits) {
        case 128:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B;
            memcpy(ctx->aes_key, key, 16);

            ctx->aes_key[0] = swap(ctx->aes_key[0]);
            ctx->aes_key[1] = swap(ctx->aes_key[1]);
            ctx->aes_key[2] = swap(ctx->aes_key[2]);
            ctx->aes_key[3] = swap(ctx->aes_key[3]);

            break;
        case 192:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_192B;
            memcpy(ctx->aes_key, key, 24);

            ctx->aes_key[0] = swap(ctx->aes_key[0]);
            ctx->aes_key[1] = swap(ctx->aes_key[1]);
            ctx->aes_key[2] = swap(ctx->aes_key[2]);
            ctx->aes_key[3] = swap(ctx->aes_key[3]);
            ctx->aes_key[4] = swap(ctx->aes_key[4]);
            ctx->aes_key[5] = swap(ctx->aes_key[5]);

            break;
        case 256:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B;
            memcpy(ctx->aes_key, key, 32);

            ctx->aes_key[0] = swap(ctx->aes_key[0]);
            ctx->aes_key[1] = swap(ctx->aes_key[1]);
            ctx->aes_key[2] = swap(ctx->aes_key[2]);
            ctx->aes_key[3] = swap(ctx->aes_key[3]);
            ctx->aes_key[4] = swap(ctx->aes_key[4]);
            ctx->aes_key[5] = swap(ctx->aes_key[5]);
            ctx->aes_key[6] = swap(ctx->aes_key[6]);
            ctx->aes_key[7] = swap(ctx->aes_key[7]);

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
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_zeroize *******\n");
#endif

    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}


void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_init *******\n");
#endif

    memset(ctx, 0, sizeof(mbedtls_aes_context));
}


void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_free *******\n");
#endif

    if (ctx == NULL) {
        return;
    }
#if defined(DUAL_CORE)
    uint32_t timeout = HSEM_TIMEOUT;
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID) && (--timeout != 0)) {
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

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_setkey_enc *******\n");
    mbedtls_printf("enc keybits : %d\n", keybits);
    mbedtls_printf("enc key :\n");
    for (int i = 1; i <= keybits / 8; i++) {
        mbedtls_printf("%x\t", key[i - 1]);
        if ((i % 8) == 0) {
            mbedtls_printf("\n");
        }
    }
#endif

    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    int ret_val = 0;

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_setkey_dec *******\n");
    mbedtls_printf("dec keybits : %d\n", keybits);
    mbedtls_printf("enc key:\n");
    for (int i = 1; i <= keybits / 8; i++) {
        mbedtls_printf("%x\t", key[i - 1]);
        if ((i % 8) == 0) {
            mbedtls_printf("\n");
        }
    }
#endif

    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}


int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    int ret;

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_crypt_ecb (%s)*******\n", mode == MBEDTLS_AES_DECRYPT ? "decrypt" : "encrypt");
    mbedtls_printf("input:\n");
    for (int i = 1; i <= 16; i++) {
        mbedtls_printf("%x\t", input[i - 1]);
        if ((i % 8) == 0) {
            mbedtls_printf("\n");
        }
    }
#endif

    /* allow multi-instance of CRYP use: restore context for CRYP hw module */
    ctx->hcryp_aes.Instance->CR = ctx->ctx_save_cr;
    ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_aes.Init.pKey = ctx->aes_key;

    /* Set the Algo if not configured till now */
    if (CRYP_AES_ECB != (ctx->hcryp_aes.Instance->CR & CRYP_AES_ECB)) {
        ctx->hcryp_aes.Init.Algorithm  = CRYP_AES_ECB;

        /* Configure the CRYP  */
        HAL_CRYP_SetConfig(&ctx->hcryp_aes, &ctx->hcryp_aes.Init);

#if TLSPRINT
        mbedtls_printf(" ****** AES ECB algo configuration set : %ld *******\n", CRYP_AES_ECB);
#endif
    }

    if (mode == MBEDTLS_AES_DECRYPT) { /* AES decryption */
        ret = mbedtls_internal_aes_decrypt(ctx, input, output);
        if (ret) {
            return ST_ERR_AES_BUSY;
        } else {
#if TLSPRINT
            mbedtls_printf("dec output :\n");
            for (int j = 1; j <= 16; j++) {
                mbedtls_printf("%x\t", output[j - 1]);
                if ((j % 8) == 0) {
                    mbedtls_printf("\n");
                }
            }
#endif
        }
    } else { /* AES encryption */
        ret = mbedtls_internal_aes_encrypt(ctx, input, output);
        if (ret) {
            return ST_ERR_AES_BUSY;
        } else {
#if TLSPRINT
            mbedtls_printf("enc output :\n");
            for (int k = 1; k <= 16; k++) {
                mbedtls_printf("%x\t", output[k - 1]);
                if ((k % 8) == 0) {
                    mbedtls_printf("\n");
                }
            }
#endif
        }
    }
    /* allow multi-instance of CRYP use: save context for CRYP HW module CR */
    ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;

    return (0);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
static int st_cbc_restore_context(mbedtls_aes_context *ctx)
{
#if TLSPRINT
    mbedtls_printf(" ****** st_cbc_restore_context *******\n");
#endif
    /* allow multi-instance of CRYP use: restore context for CRYP hw module */
    ctx->hcryp_aes.Instance->CR = ctx->ctx_save_cr;
    /* Re-initialize AES processor with proper parameters
       and (re-)apply key and IV for multi context usecases */
    if (HAL_CRYP_DeInit(&ctx->hcryp_aes) != HAL_OK) {
        return ST_ERR_AES_BUSY;
    }
    if (HAL_CRYP_Init(&ctx->hcryp_aes) != HAL_OK) {
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

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_crypt_cbc *******\n");
#endif

    if (length % 16) {
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);
    }
    ctx->hcryp_aes.Init.pInitVect = (uint32_t *)&iv[0];
    if (st_cbc_restore_context(ctx) != 0) {
        return (ST_ERR_AES_BUSY);
    }

    /* Set the Algo if not configured till now */
    if (CRYP_AES_CBC != (ctx->hcryp_aes.Instance->CR & CRYP_AES_CBC)) {
        ctx->hcryp_aes.Init.Algorithm  = CRYP_AES_CBC;

        /* Configure the CRYP  */
        HAL_CRYP_SetConfig(&ctx->hcryp_aes, &ctx->hcryp_aes.Init);
    }

    if (mode == MBEDTLS_AES_DECRYPT) {
        if (HAL_CRYP_Decrypt(&ctx->hcryp_aes, (uint32_t *)input, length / 4, (uint32_t *)output, 10) != HAL_OK) {
            return ST_ERR_AES_BUSY;
        }
        /* Save the internal IV vector for multi context purpose */
        tickstart = HAL_GetTick();
        while ((ctx->hcryp_aes.Instance->SR & (CRYP_SR_IFEM | CRYP_SR_OFNE | CRYP_SR_BUSY)) != CRYP_SR_IFEM) {
            if ((HAL_GetTick() - tickstart) > ST_AES_TIMEOUT) {
                return ST_ERR_AES_BUSY; // timeout: CRYP processor is busy
            }
        }
        ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR; // save here before overwritten
        ctx->hcryp_aes.Instance->CR &= ~CRYP_CR_CRYPEN;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IV0LR;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IV0RR;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IV1LR;
        *iv_ptr++ = ctx->hcryp_aes.Instance->IV1RR;
    } else {
        if (HAL_CRYP_Encrypt(&ctx->hcryp_aes, (uint32_t *)input, length / 4, (uint32_t *)output, 10) != HAL_OK) {
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

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_crypt_cfb128 *******\n");
#endif

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

#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_crypt_cfb8 *******\n");
#endif

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
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_internal_aes_encrypt *******\n");
#endif

    if (HAL_CRYP_Encrypt(&ctx->hcryp_aes, (uint32_t *)input, 4, (uint32_t *)output, 10) != HAL_OK) {
        // error found
        return ST_ERR_AES_BUSY;
    }
    return 0;

}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_internal_aes_decrypt *******\n");
#endif

    if (HAL_CRYP_Decrypt(&ctx->hcryp_aes, (uint32_t *)input, 4, (uint32_t *)output, 10) != HAL_OK) {
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
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_encrypt *******\n");
#endif
    mbedtls_internal_aes_encrypt(ctx, input, output);
}

void mbedtls_aes_decrypt(mbedtls_aes_context *ctx,
                         const unsigned char input[16],
                         unsigned char output[16])
{
#if TLSPRINT
    mbedtls_printf(" ****** mbedtls_aes_decrypt *******\n");
#endif
    mbedtls_internal_aes_decrypt(ctx, input, output);
}
#endif /* MBEDTLS_DEPRECATED_REMOVED */
#endif /*MBEDTLS_AES_ALT*/
