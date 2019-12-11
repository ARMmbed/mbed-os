/*
*  FIPS-197 compliant AES implementation
*
*  Copyright (C) 2006-2019, ARM Limited, All Rights Reserved
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
*  Copyright (C) 2006-2019 STMicroelectronics, All Rights Reserved
*
*  This file implements ST AES HW services based on API from mbed TLS
*
*  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
*
*  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
*  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
*/


/* Includes ------------------------------------------------------------------*/
#include "mbedtls/aes.h"

#if defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_AES_ALT)
#include "mbedtls/platform_util.h"
#include <string.h>

/* Parameter validation macros based on platform_util.h */
#define AES_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_AES_BAD_INPUT_DATA )
#define AES_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  TIMEOUT_VALUE 0xFF

/* Private macro -------------------------------------------------------------*/
#define SWAP_B32_TO_B8(b32,b8,i)                                 \
{                                                                \
  (b8)[(i) + 3] = (unsigned char) ( ( (b32)       ) & 0xFF );    \
  (b8)[(i) + 2] = (unsigned char) ( ( (b32) >>  8 ) & 0xFF );    \
  (b8)[(i) + 1] = (unsigned char) ( ( (b32) >> 16 ) & 0xFF );    \
  (b8)[(i)    ] = (unsigned char) ( ( (b32) >> 24 ) & 0xFF );    \
}

#define SWAP_B8_TO_B32(b32,b8,i)                         \
{                                                        \
  (b32) = ( (uint32_t) (b8)[(i) + 3]       )             \
        | ( (uint32_t) (b8)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b8)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b8)[(i)    ] << 24 );            \
}

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int aes_set_key(mbedtls_aes_context *ctx,
                       const unsigned char *key,
                       unsigned int keybits)
{
    /* Deinitializes the CRYP peripheral */
    if (HAL_CRYP_DeInit(&ctx->hcryp_aes) == HAL_ERROR) {
        return (HAL_ERROR);
    }

    switch (keybits) {
        case 128:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_128B;;

            SWAP_B8_TO_B32(ctx->aes_key[0], key, 0);
            SWAP_B8_TO_B32(ctx->aes_key[1], key, 4);
            SWAP_B8_TO_B32(ctx->aes_key[2], key, 8);
            SWAP_B8_TO_B32(ctx->aes_key[3], key, 12);

            break;
        case 192:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_192B;

            SWAP_B8_TO_B32(ctx->aes_key[0], key, 0);
            SWAP_B8_TO_B32(ctx->aes_key[1], key, 4);
            SWAP_B8_TO_B32(ctx->aes_key[2], key, 8);
            SWAP_B8_TO_B32(ctx->aes_key[3], key, 12);
            SWAP_B8_TO_B32(ctx->aes_key[4], key, 16);
            SWAP_B8_TO_B32(ctx->aes_key[5], key, 20);

            break;

        case 256:
            ctx->hcryp_aes.Init.KeySize = CRYP_KEYSIZE_256B;

            SWAP_B8_TO_B32(ctx->aes_key[0], key, 0);
            SWAP_B8_TO_B32(ctx->aes_key[1], key, 4);
            SWAP_B8_TO_B32(ctx->aes_key[2], key, 8);
            SWAP_B8_TO_B32(ctx->aes_key[3], key, 12);
            SWAP_B8_TO_B32(ctx->aes_key[4], key, 16);
            SWAP_B8_TO_B32(ctx->aes_key[5], key, 20);
            SWAP_B8_TO_B32(ctx->aes_key[6], key, 24);
            SWAP_B8_TO_B32(ctx->aes_key[7], key, 28);

            break;
        default :
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_aes.Instance = CRYP;
    ctx->hcryp_aes.Init.pKey = ctx->aes_key;

    /* Enable CRYP clock */
    __HAL_RCC_CRYP_CLK_ENABLE();

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
    AES_VALIDATE(ctx != NULL);

    memset(ctx, 0, sizeof(mbedtls_aes_context));
}


void mbedtls_aes_free(mbedtls_aes_context *ctx)
{

    if (ctx == NULL) {
        return;
    }
    /* Force the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_FORCE_RESET();

    /* Release the CRYP Periheral Clock Reset */
    __HAL_RCC_CRYP_RELEASE_RESET();

    mbedtls_zeroize(ctx, sizeof(mbedtls_aes_context));
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
void mbedtls_aes_xts_init(mbedtls_aes_xts_context *ctx)
{
    AES_VALIDATE(ctx != NULL);

    mbedtls_aes_init(&ctx->crypt);
    mbedtls_aes_init(&ctx->tweak);
}

void mbedtls_aes_xts_free(mbedtls_aes_xts_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_aes_free(&ctx->crypt);
    mbedtls_aes_free(&ctx->tweak);
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    int ret_val = 0;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key,
                           unsigned int keybits)
{
    int ret_val = 0;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret_val = aes_set_key(ctx, key, keybits);
    return (ret_val);
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
static int mbedtls_aes_xts_decode_keys(const unsigned char *key,
                                       unsigned int keybits,
                                       const unsigned char **key1,
                                       unsigned int *key1bits,
                                       const unsigned char **key2,
                                       unsigned int *key2bits)
{
    const unsigned int half_keybits = keybits / 2;
    const unsigned int half_keybytes = half_keybits / 8;

    switch (keybits) {
        case 256:
            break;
        case 512:
            break;
        default :
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    *key1bits = half_keybits;
    *key2bits = half_keybits;
    *key1 = &key[0];
    *key2 = &key[half_keybytes];

    return 0;
}

int mbedtls_aes_xts_setkey_enc(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    int ret;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret = mbedtls_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return (ret);
    }

    /* Set the tweak key. Always set tweak key for the encryption mode. */
    ret = mbedtls_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return (ret);
    }

    /* Set crypt key for encryption. */
    return mbedtls_aes_setkey_enc(&ctx->crypt, key1, key1bits);
}

int mbedtls_aes_xts_setkey_dec(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    int ret;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret = mbedtls_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return (ret);
    }

    /* Set the tweak key. Always set tweak key for encryption. */
    ret = mbedtls_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return (ret);
    }

    /* Set crypt key for decryption. */
    return mbedtls_aes_setkey_dec(&ctx->crypt, key1, key1bits);
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    int ret;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);

    /* allow multi-instance of CRYP use: restore context for CRYP hw module */
    ctx->hcryp_aes.Instance->CR = ctx->ctx_save_cr;

    ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
    ctx->hcryp_aes.Init.pKey = ctx->aes_key;

    /* Set the Algo if not configured till now */
    if (CRYP_AES_ECB != ctx->hcryp_aes.Init.Algorithm) {
        ctx->hcryp_aes.Init.Algorithm  = CRYP_AES_ECB;

        /* Configure the CRYP  */
        HAL_CRYP_SetConfig(&ctx->hcryp_aes, &ctx->hcryp_aes.Init);
    }

    if (mode == MBEDTLS_AES_DECRYPT) { /* AES decryption */
        ret = mbedtls_internal_aes_decrypt(ctx, input, output);
        if (ret) {
            return ST_ERR_AES_BUSY;
        }
    } else { /* AES encryption */
        ret = mbedtls_internal_aes_encrypt(ctx, input, output);
        if (ret) {
            return ST_ERR_AES_BUSY;
        }
    }
    /* allow multi-instance of CRYP use: save context for CRYP HW module CR */
    ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;

    return (0);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
static int st_cbc_restore_context(mbedtls_aes_context *ctx)
{
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

    ALIGN_32BYTES(static uint32_t iv_32B[4]);

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    if (length % 16) {
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);
    }

    if (st_cbc_restore_context(ctx) != 0) {
        return (ST_ERR_AES_BUSY);
    }

    /* Set the Algo if not configured till now */
    if (CRYP_AES_CBC != ctx->hcryp_aes.Init.Algorithm) {
        ctx->hcryp_aes.Init.Algorithm  = CRYP_AES_CBC;
    }

    if (mode == MBEDTLS_AES_DECRYPT) {
        ctx->hcryp_aes.Init.pInitVect = (uint32_t *)&iv[0];

        /* reconfigure the CRYP */
        HAL_CRYP_SetConfig(&ctx->hcryp_aes, &ctx->hcryp_aes.Init);

        if (HAL_CRYP_Decrypt(&ctx->hcryp_aes, (uint32_t *)input, length / 4, (uint32_t *)output, TIMEOUT_VALUE) != HAL_OK) {
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
        ctx->hcryp_aes.Init.DataType = CRYP_DATATYPE_8B;
        ctx->hcryp_aes.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;

        SWAP_B8_TO_B32(iv_32B[0], iv, 0);
        SWAP_B8_TO_B32(iv_32B[1], iv, 4);
        SWAP_B8_TO_B32(iv_32B[2], iv, 8);
        SWAP_B8_TO_B32(iv_32B[3], iv, 12);

        ctx->hcryp_aes.Init.pInitVect = iv_32B;

        /* reconfigure the CRYP */
        HAL_CRYP_SetConfig(&ctx->hcryp_aes, &ctx->hcryp_aes.Init);

        if (HAL_CRYP_Encrypt(&ctx->hcryp_aes, (uint32_t *)input, length, (uint32_t *)output, TIMEOUT_VALUE) != HAL_OK) {
            return ST_ERR_AES_BUSY;
        }

        memcpy(iv, output, 16);   /* current output is the IV vector for the next call */
        ctx->ctx_save_cr = ctx->hcryp_aes.Instance->CR;
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_XTS)

/* Endianess with 64 bits values */
#ifndef GET_UINT64_LE
#define GET_UINT64_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint64_t) (b)[(i) + 7] << 56 )             \
        | ( (uint64_t) (b)[(i) + 6] << 48 )             \
        | ( (uint64_t) (b)[(i) + 5] << 40 )             \
        | ( (uint64_t) (b)[(i) + 4] << 32 )             \
        | ( (uint64_t) (b)[(i) + 3] << 24 )             \
        | ( (uint64_t) (b)[(i) + 2] << 16 )             \
        | ( (uint64_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint64_t) (b)[(i)    ]       );            \
}
#endif

#ifndef PUT_UINT64_LE
#define PUT_UINT64_LE(n,b,i)                            \
{                                                       \
    (b)[(i) + 7] = (unsigned char) ( (n) >> 56 );       \
    (b)[(i) + 6] = (unsigned char) ( (n) >> 48 );       \
    (b)[(i) + 5] = (unsigned char) ( (n) >> 40 );       \
    (b)[(i) + 4] = (unsigned char) ( (n) >> 32 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
}
#endif

/*
 * GF(2^128) multiplication function
 *
 * This function multiplies a field element by x in the polynomial field
 * representation. It uses 64-bit word operations to gain speed but compensates
 * for machine endianess and hence works correctly on both big and little
 * endian machines.
 */
static void mbedtls_gf128mul_x_ble(unsigned char r[16],
                                   const unsigned char x[16])
{
    uint64_t a, b, ra, rb;

    GET_UINT64_LE(a, x, 0);
    GET_UINT64_LE(b, x, 8);

    ra = (a << 1)  ^ 0x0087 >> (8 - ((b >> 63) << 3));
    rb = (a >> 63) | (b << 1);

    PUT_UINT64_LE(ra, r, 0);
    PUT_UINT64_LE(rb, r, 8);
}

/*
 * AES-XTS buffer encryption/decryption
 */
int mbedtls_aes_crypt_xts(mbedtls_aes_xts_context *ctx,
                          int mode,
                          size_t length,
                          const unsigned char data_unit[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(data_unit != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    /* Data units must be at least 16 bytes long. */
    if (length < 16) {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
    if (length > (1 << 20) * 16) {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }

    /* Compute the tweak. */
    ret = mbedtls_aes_crypt_ecb(&ctx->tweak, MBEDTLS_AES_ENCRYPT,
                                data_unit, tweak);
    if (ret != 0) {
        return (ret);
    }

    while (blocks--) {
        size_t i;

        if (leftover && (mode == MBEDTLS_AES_DECRYPT) && blocks == 0) {
            /* We are on the last block in a decrypt operation that has
             * leftover bytes, so we need to use the next tweak for this block,
             * and this tweak for the lefover bytes. Save the current tweak for
             * the leftovers and then update the current tweak for use on this,
             * the last full block. */
            memcpy(prev_tweak, tweak, sizeof(tweak));
            mbedtls_gf128mul_x_ble(tweak, tweak);
        }

        for (i = 0; i < 16; i++) {
            tmp[i] = input[i] ^ tweak[i];
        }

        ret = mbedtls_aes_crypt_ecb(&ctx->crypt, mode, tmp, tmp);
        if (ret != 0) {
            return (ret);
        }

        for (i = 0; i < 16; i++) {
            output[i] = tmp[i] ^ tweak[i];
        }

        /* Update the tweak for the next block. */
        mbedtls_gf128mul_x_ble(tweak, tweak);

        output += 16;
        input += 16;
    }

    if (leftover) {
        /* If we are on the leftover bytes in a decrypt operation, we need to
         * use the previous tweak for these bytes (as saved in prev_tweak). */
        unsigned char *t = mode == MBEDTLS_AES_DECRYPT ? prev_tweak : tweak;

        /* We are now on the final part of the data unit, which doesn't divide
         * evenly by 16. It's time for ciphertext stealing. */
        size_t i;
        unsigned char *prev_output = output - 16;

        /* Copy ciphertext bytes from the previous block to our output for each
         * byte of cyphertext we won't steal. At the same time, copy the
         * remainder of the input for this final round (since the loop bounds
         * are the same). */
        for (i = 0; i < leftover; i++) {
            output[i] = prev_output[i];
            tmp[i] = input[i] ^ t[i];
        }

        /* Copy ciphertext bytes from the previous block for input in this
         * round. */
        for (; i < 16; i++) {
            tmp[i] = prev_output[i] ^ t[i];
        }

        ret = mbedtls_aes_crypt_ecb(&ctx->crypt, mode, tmp, tmp);
        if (ret != 0) {
            return ret;
        }

        /* Write the result back to the previous block, overriding the previous
         * output we copied. */
        for (i = 0; i < 16; i++) {
            prev_output[i] = tmp[i] ^ t[i];
        }
    }

    return (0);
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    int c;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv_off != NULL);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *iv_off;

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

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    unsigned char c;
    unsigned char ov[17];

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

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

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int mbedtls_aes_crypt_ofb(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *iv_off,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = 0;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(iv_off != NULL);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *iv_off;

    if (n > 15) {
        return (MBEDTLS_ERR_AES_BAD_INPUT_DATA);
    }

    while (length--) {
        if (n == 0) {
            ret = mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
            if (ret != 0) {
                goto exit;
            }
        }
        *output++ =  *input++ ^ iv[n];

        n = (n + 1) & 0x0F;
    }

    *iv_off = n;

exit:
    return (ret);
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int c, i;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(nc_off != NULL);
    AES_VALIDATE_RET(nonce_counter != NULL);
    AES_VALIDATE_RET(stream_block != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *nc_off;

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

    if (HAL_CRYP_Encrypt(&ctx->hcryp_aes, (uint32_t *)input, 4, (uint32_t *)output, TIMEOUT_VALUE) != HAL_OK) {
        // error found
        return ST_ERR_AES_BUSY;
    }
    return 0;

}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{
    if (HAL_CRYP_Decrypt(&ctx->hcryp_aes, (uint32_t *)input, 4, (uint32_t *)output, TIMEOUT_VALUE) != HAL_OK) {
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
#endif /* MBEDTLS_AES_C */
