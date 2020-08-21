/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbedtls/des.h"

#if defined(MBEDTLS_DES_C)
#if defined(MBEDTLS_DES_ALT)

#include <string.h>
#include <stdbool.h>
#include "crypto-misc.h"
#include "nu_bitutil.h"
#include "mbed_toolchain.h"
#include "mbed_error.h"

/* DES DMA compatible buffer requirements
 * 
 * DES DMA buffer location requires to be:
 * (1) Word-aligned
 * (2) Located in 0x2xxxxxxx region. Check linker files to ensure global variables are placed in this region.
 * 
 * DES DMA buffer size MAXSIZE_DMABUF must be a multiple of 64-bit block size.
 * Its value is estimated to trade memory footprint off against performance.
 *
 */
#define MAXSIZE_DMABUF  (8 * 5)
MBED_ALIGN(4) static uint8_t dmabuf_in[MAXSIZE_DMABUF];
MBED_ALIGN(4) static uint8_t dmabuf_out[MAXSIZE_DMABUF];

static int mbedtls_des_docrypt(uint16_t keyopt, uint8_t key[3][MBEDTLS_DES_KEY_SIZE], int enc, uint32_t tdes_opmode, size_t length,
                               unsigned char iv[8], const unsigned char *input, unsigned char *output);

void mbedtls_des_init(mbedtls_des_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_des_context));
}

void mbedtls_des_free( mbedtls_des_context *ctx )
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(ctx, sizeof(mbedtls_des_context));
}

void mbedtls_des3_init( mbedtls_des3_context *ctx )
{
    memset(ctx, 0, sizeof(mbedtls_des3_context));
}

void mbedtls_des3_free( mbedtls_des3_context *ctx )
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(ctx, sizeof (mbedtls_des3_context));
}

static const unsigned char odd_parity_table[128] = { 1,  2,  4,  7,  8,
                                                     11, 13, 14, 16, 19, 21, 22, 25, 26, 28, 31, 32, 35, 37, 38, 41, 42, 44,
                                                     47, 49, 50, 52, 55, 56, 59, 61, 62, 64, 67, 69, 70, 73, 74, 76, 79, 81,
                                                     82, 84, 87, 88, 91, 93, 94, 97, 98, 100, 103, 104, 107, 109, 110, 112,
                                                     115, 117, 118, 121, 122, 124, 127, 128, 131, 133, 134, 137, 138, 140,
                                                     143, 145, 146, 148, 151, 152, 155, 157, 158, 161, 162, 164, 167, 168,
                                                     171, 173, 174, 176, 179, 181, 182, 185, 186, 188, 191, 193, 194, 196,
                                                     199, 200, 203, 205, 206, 208, 211, 213, 214, 217, 218, 220, 223, 224,
                                                     227, 229, 230, 233, 234, 236, 239, 241, 242, 244, 247, 248, 251, 253,
                                                     254
                                                   };

void mbedtls_des_key_set_parity(unsigned char key[MBEDTLS_DES_KEY_SIZE])
{
    int i;

    for (i = 0; i < MBEDTLS_DES_KEY_SIZE; i++) {
        key[i] = odd_parity_table[key[i] / 2];
    }
}

/*
 * Check the given key's parity, returns 1 on failure, 0 on SUCCESS
 */
int mbedtls_des_key_check_key_parity( const unsigned char key[MBEDTLS_DES_KEY_SIZE] )
{
    int i;

    for( i = 0; i < MBEDTLS_DES_KEY_SIZE; i++ )
        if( key[i] != odd_parity_table[key[i] / 2] )
            return( 1 );

    return( 0 );
}

/*
 * Table of weak and semi-weak keys
 *
 * Source: http://en.wikipedia.org/wiki/Weak_key
 *
 * Weak:
 * Alternating ones + zeros (0x0101010101010101)
 * Alternating 'F' + 'E' (0xFEFEFEFEFEFEFEFE)
 * '0xE0E0E0E0F1F1F1F1'
 * '0x1F1F1F1F0E0E0E0E'
 *
 * Semi-weak:
 * 0x011F011F010E010E and 0x1F011F010E010E01
 * 0x01E001E001F101F1 and 0xE001E001F101F101
 * 0x01FE01FE01FE01FE and 0xFE01FE01FE01FE01
 * 0x1FE01FE00EF10EF1 and 0xE01FE01FF10EF10E
 * 0x1FFE1FFE0EFE0EFE and 0xFE1FFE1FFE0EFE0E
 * 0xE0FEE0FEF1FEF1FE and 0xFEE0FEE0FEF1FEF1
 *
 */

#define WEAK_KEY_COUNT 16

static const unsigned char weak_key_table[WEAK_KEY_COUNT][MBEDTLS_DES_KEY_SIZE] = {
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE },
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E },
    { 0xE0, 0xE0, 0xE0, 0xE0, 0xF1, 0xF1, 0xF1, 0xF1 },

    { 0x01, 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E },
    { 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E, 0x01 },
    { 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1 },
    { 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1, 0x01 },
    { 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE },
    { 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01 },
    { 0x1F, 0xE0, 0x1F, 0xE0, 0x0E, 0xF1, 0x0E, 0xF1 },
    { 0xE0, 0x1F, 0xE0, 0x1F, 0xF1, 0x0E, 0xF1, 0x0E },
    { 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E, 0xFE },
    { 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E },
    { 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1, 0xFE },
    { 0xFE, 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1 }
};

int mbedtls_des_key_check_weak( const unsigned char key[MBEDTLS_DES_KEY_SIZE] )
{
    int i;

    for( i = 0; i < WEAK_KEY_COUNT; i++ )
        if( memcmp( weak_key_table[i], key, MBEDTLS_DES_KEY_SIZE) == 0 )
            return( 1 );

    return( 0 );
}

/*
 * DES key schedule (56-bit, encryption)
 */
int mbedtls_des_setkey_enc( mbedtls_des_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE] )
{
    ctx->enc = 1;
    // Keying option 3: All three keys are identical, i.e. K1 = K2 = K3.
    ctx->keyopt = 3;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * DES key schedule (56-bit, decryption)
 */
int mbedtls_des_setkey_dec( mbedtls_des_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE] )
{
    ctx->enc = 0;
    // Keying option 3: All three keys are identical, i.e. K1 = K2 = K3.
    ctx->keyopt = 3;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * Triple-DES key schedule (112-bit, encryption)
 */
int mbedtls_des3_set2key_enc( mbedtls_des3_context *ctx,
                              const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2] )
{
    ctx->enc = 1;
    // Keying option 2: K1 and K2 are independent, and K3 = K1.
    ctx->keyopt = 2;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key + MBEDTLS_DES_KEY_SIZE, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * Triple-DES key schedule (112-bit, decryption)
 */
int mbedtls_des3_set2key_dec( mbedtls_des3_context *ctx,
                              const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2] )
{
    ctx->enc = 0;
    // Keying option 2: K1 and K2 are independent, and K3 = K1.
    ctx->keyopt = 2;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key + MBEDTLS_DES_KEY_SIZE, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * Triple-DES key schedule (168-bit, encryption)
 */
int mbedtls_des3_set3key_enc( mbedtls_des3_context *ctx,
                              const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3] )
{
    ctx->enc = 1;
    // Keying option 1: All three keys are independent.
    ctx->keyopt = 1;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key + MBEDTLS_DES_KEY_SIZE, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key + MBEDTLS_DES_KEY_SIZE * 2, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * Triple-DES key schedule (168-bit, decryption)
 */
int mbedtls_des3_set3key_dec( mbedtls_des3_context *ctx,
                              const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3] )
{
    ctx->enc = 0;
    // Keying option 1: All three keys are independent.
    ctx->keyopt = 1;
    memcpy(ctx->key[0], key, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[1], key + MBEDTLS_DES_KEY_SIZE, MBEDTLS_DES_KEY_SIZE);
    memcpy(ctx->key[2], key + MBEDTLS_DES_KEY_SIZE * 2, MBEDTLS_DES_KEY_SIZE);

    return 0;
}

/*
 * DES-ECB block encryption/decryption
 */
int mbedtls_des_crypt_ecb( mbedtls_des_context *ctx,
                           const unsigned char input[8],
                           unsigned char output[8] )
{
    unsigned char iv[8] = {0x00};
    return mbedtls_des_docrypt(ctx->keyopt, ctx->key, ctx->enc, DES_MODE_ECB, 8, iv, input, output);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * DES-CBC buffer encryption/decryption
 */
int mbedtls_des_crypt_cbc( mbedtls_des_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[8],
                           const unsigned char *input,
                           unsigned char *output )
{
    return mbedtls_des_docrypt(ctx->keyopt, ctx->key, mode == MBEDTLS_DES_ENCRYPT, DES_MODE_CBC, length, iv, input, output);
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

/*
 * 3DES-ECB block encryption/decryption
 */
int mbedtls_des3_crypt_ecb( mbedtls_des3_context *ctx,
                            const unsigned char input[8],
                            unsigned char output[8] )
{
    unsigned char iv[8] = {0x00};
    return mbedtls_des_docrypt(ctx->keyopt, ctx->key, ctx->enc, TDES_MODE_ECB, 8, iv, input, output);
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * 3DES-CBC buffer encryption/decryption
 */
int mbedtls_des3_crypt_cbc( mbedtls_des3_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[8],
                            const unsigned char *input,
                            unsigned char *output )
{
    return mbedtls_des_docrypt(ctx->keyopt, ctx->key, mode == MBEDTLS_DES_ENCRYPT, TDES_MODE_CBC, length, iv, input, output);
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */



static int mbedtls_des_docrypt(uint16_t keyopt, uint8_t key[3][MBEDTLS_DES_KEY_SIZE], int enc, uint32_t tdes_opmode, size_t length,
                               unsigned char iv[8], const unsigned char *input, unsigned char *output)
{
    if ((keyopt < 1) || (keyopt > 3)) {
        error("Key option in DES alter. requires to be 1/2/3.");
    }
    
    if (key == NULL) {
        error("Meet null key pointer in DES alter.");
    }
    
    if ((enc != 0) && (enc != 1)) {
        error("Enc/dec flag in DES alter. requires to be 0/1.");
    }
    
    if (tdes_opmode & ~(CRPT_TDES_CTL_TMODE_Msk | CRPT_TDES_CTL_OPMODE_Msk)) {
        error("Invalid TMODE/OPMODE in DES alter.");
    }
    
    if (length % 8) {
        return MBEDTLS_ERR_DES_INVALID_INPUT_LENGTH;
    }

    if (iv == NULL) {
        error("Meet null IV pointer in DES alter.");
    }
    
    if (input == NULL || output == NULL) {
        error("Meet null input/output pointer in DES alter.");
    }
    
    /* DES DMA buffer has the following requirements:
     * (1) Word-aligned buffer base address
     * (2) 8-byte aligned buffer size
     * (3) Located in 0x20000000-0x2FFFFFFF region
     */
    if ((! crypto_dma_buff_compat(dmabuf_in, MAXSIZE_DMABUF, 8)) ||
        (! crypto_dma_buff_compat(dmabuf_out, MAXSIZE_DMABUF, 8))) {
        error("Buffer for DES alter. DMA requires to be word-aligned and located in 0x20000000-0x2FFFFFFF region.");
    }
    
    /* Acquire ownership of DES H/W */
    crypto_des_acquire();

    /* Init crypto module */
    crypto_init();
    /* Enable DES interrupt */
    TDES_ENABLE_INT(CRPT);
    
    /* Configure TDES_CTL register
     *
     * BSP TDES driver supports multiple channels. Just use channel #0.
     *
     * Relationship of keying option and TDES H/W mode configuration
     * 1: All three keys are independent                    ==> TDES 3-key mode (TMODE=1, 3KEYS=1)
     * 2: K1 and K2 are independent, and K3 = K1            ==> TDES 2-key mode (TMODE=1, 3KEYS=0)
     * 3: All three keys are identical, i.e. K1 = K2 = K3   ==> DES mode (TMODE=0)
     *
     * tdes_opmode is combination of TMODE/OPMODE, but TDES_Open I/F requires TMODE/OPMODE to be separate.
     * We need to divide tdes_opmode to TMODE and OPMODE.
     *
     * TDES_IN_OUT_WHL_SWAP lets TDES H/W know input/output data are arranged in below for DMA transfer:
     * 1. BE for byte sequence in word
     * 2. BE for word sequence in double-word
     */
    TDES_Open(CRPT
            0,                                                // Channel number (0~4) 
            enc,                                                // 0: decode, 1: encode
            (tdes_opmode & CRPT_TDES_CTL_TMODE_Msk) ? 1 : 0,    // 0: DES, 1: TDES 
            (keyopt == 1) ? 1 : 0,                              // 0: TDES 2-key mode, 1: TDES 3-key mode
            tdes_opmode & CRPT_TDES_CTL_OPMODE_Msk,             // ECB/CBC/CFB/OFB/CTR
            TDES_IN_OUT_WHL_SWAP);                              // TDES_NO_SWAP~TDES_IN_OUT_WHL_SWAP

    /* Set DES/TDES keys 
     *
     * TDES_SetKey requires 3x2 word array. Change 3x8 byte array to 3x2 word array.
     */
    unsigned i;
    uint32_t keys3x2[3][2];
    for (i = 0; i < 3; i ++ ) {
        keys3x2[i][0] = nu_get32_be(key[i] + 0);
        keys3x2[i][1] = nu_get32_be(key[i] + 4);
    }
    TDES_SetKey(CRPT, 0, keys3x2);

    uint32_t rmn = length;
    const unsigned char *in_pos = input;
    unsigned char *out_pos = output;

    while (rmn > 0) {
        uint32_t data_len = (rmn <= MAXSIZE_DMABUF) ? rmn : MAXSIZE_DMABUF;

        uint32_t ivh, ivl;
        ivh = nu_get32_be(iv);
        ivl = nu_get32_be(iv + 4);
        TDES_SetInitVect(CRPT, 0, ivh, ivl);

        memcpy(dmabuf_in, in_pos, data_len);

        /* We always use DMA backup buffers, which are guaranteed to be non-overlapped. */
        TDES_SetDMATransfer(CRPT, 0, (uint32_t) dmabuf_in, (uint32_t) dmabuf_out, data_len);

        crypto_des_prestart();
        TDES_Start(CRPT, 0, CRYPTO_DMA_ONE_SHOT);
        crypto_des_wait();

        memcpy(out_pos, dmabuf_out, data_len);
        in_pos += data_len;
        out_pos += data_len;
        rmn -= data_len;

        // Update IV for next block enc/dec in next function call
        switch (tdes_opmode) {
        case DES_MODE_OFB:
        case TDES_MODE_OFB: {
            // OFB: IV (enc/dec) = output block XOR input block
            uint32_t lbh, lbl;
            // Last block of input data
            lbh = nu_get32_be(dmabuf_in + data_len - 8 + 4);
            lbl = nu_get32_be(dmabuf_in + data_len - 8 + 0);
            // Last block of output data
            ivh = nu_get32_be(dmabuf_out + 4);
            ivl = nu_get32_be(dmabuf_out + 0);
            ivh = ivh ^ lbh;
            ivl = ivl ^ lbl;
            nu_set32_be(iv + 4, ivh);
            nu_set32_be(iv, ivl);
            break;
        }
        case DES_MODE_CBC:
        case DES_MODE_CFB:
        case TDES_MODE_CBC:
        case TDES_MODE_CFB: {
            // CBC/CFB: IV (enc) = output block
            //          IV (dec) = input block
            if (enc) {
                memcpy(iv, dmabuf_out + data_len - 8, 8);
            } else {
                memcpy(iv, dmabuf_in + data_len - 8, 8);
            }
        }
        }
    }

    /* Disable DES interrupt */
    TDES_DISABLE_INT(CRPT);
    /* Uninit crypto module */
    crypto_uninit();
    
    /* Release ownership of DES H/W */
    crypto_des_release();
    
    return 0;
}

#endif /* MBEDTLS_DES_ALT */
#endif /* MBEDTLS_DES_C */
