/**
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *    (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * Description: LoRa MAC crypto implementation
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jäckle ( STACKFORCE )
 *
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdlib.h>
#include <stdint.h>

#include "LoRaMacCrypto.h"
#include "system/lorawan_data_structures.h"
#include "mbedtls/platform.h"


#if defined(MBEDTLS_CMAC_C) && defined(MBEDTLS_AES_C) && defined(MBEDTLS_CIPHER_C)

LoRaMacCrypto::LoRaMacCrypto()
{
#if defined(MBEDTLS_PLATFORM_C)
    int ret = mbedtls_platform_setup(NULL);
    if (ret != 0) {
        MBED_ASSERT(0 && "LoRaMacCrypto: Fail in mbedtls_platform_setup.");
    }
#endif /* MBEDTLS_PLATFORM_C */
}

LoRaMacCrypto::~LoRaMacCrypto()
{
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif /* MBEDTLS_PLATFORM_C */
}

int LoRaMacCrypto::compute_mic(const uint8_t *buffer, uint16_t size,
                               const uint8_t *key, const uint32_t key_length,
                               uint32_t address, uint8_t dir, uint32_t seq_counter,
                               uint32_t *mic)
{
    uint8_t computed_mic[16] = {};
    uint8_t mic_block_b0[16] = {};
    int ret = 0;

    mic_block_b0[0] = 0x49;

    mic_block_b0[5] = dir;

    mic_block_b0[6] = (address) & 0xFF;
    mic_block_b0[7] = (address >> 8) & 0xFF;
    mic_block_b0[8] = (address >> 16) & 0xFF;
    mic_block_b0[9] = (address >> 24) & 0xFF;

    mic_block_b0[10] = (seq_counter) & 0xFF;
    mic_block_b0[11] = (seq_counter >> 8) & 0xFF;
    mic_block_b0[12] = (seq_counter >> 16) & 0xFF;
    mic_block_b0[13] = (seq_counter >> 24) & 0xFF;

    mic_block_b0[15] = size & 0xFF;

    mbedtls_cipher_init(aes_cmac_ctx);

    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(aes_cmac_ctx, cipher_info);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_starts(aes_cmac_ctx, key, key_length);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_update(aes_cmac_ctx, mic_block_b0, sizeof(mic_block_b0));
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_update(aes_cmac_ctx, buffer, size & 0xFF);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_finish(aes_cmac_ctx, computed_mic);
        if (0 != ret) {
            goto exit;
        }

        *mic = (uint32_t)((uint32_t) computed_mic[3] << 24
                          | (uint32_t) computed_mic[2] << 16
                          | (uint32_t) computed_mic[1] << 8 | (uint32_t) computed_mic[0]);
    } else {
        ret = MBEDTLS_ERR_CIPHER_ALLOC_FAILED;
    }

exit:
    mbedtls_cipher_free(aes_cmac_ctx);
    return ret;
}

int LoRaMacCrypto::encrypt_payload(const uint8_t *buffer, uint16_t size,
                                   const uint8_t *key, const uint32_t key_length,
                                   uint32_t address, uint8_t dir, uint32_t seq_counter,
                                   uint8_t *enc_buffer)
{
    uint16_t i;
    uint8_t bufferIndex = 0;
    uint16_t ctr = 1;
    int ret = 0;
    uint8_t a_block[16] = {};
    uint8_t s_block[16] = {};

    mbedtls_aes_init(&aes_ctx);
    ret = mbedtls_aes_setkey_enc(&aes_ctx, key, key_length);
    if (0 != ret) {
        goto exit;
    }

    a_block[0] = 0x01;
    a_block[5] = dir;

    a_block[6] = (address) & 0xFF;
    a_block[7] = (address >> 8) & 0xFF;
    a_block[8] = (address >> 16) & 0xFF;
    a_block[9] = (address >> 24) & 0xFF;

    a_block[10] = (seq_counter) & 0xFF;
    a_block[11] = (seq_counter >> 8) & 0xFF;
    a_block[12] = (seq_counter >> 16) & 0xFF;
    a_block[13] = (seq_counter >> 24) & 0xFF;

    while (size >= 16) {
        a_block[15] = ((ctr) & 0xFF);
        ctr++;
        ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, a_block,
                                    s_block);
        if (0 != ret) {
            goto exit;
        }

        for (i = 0; i < 16; i++) {
            enc_buffer[bufferIndex + i] = buffer[bufferIndex + i] ^ s_block[i];
        }
        size -= 16;
        bufferIndex += 16;
    }

    if (size > 0) {
        a_block[15] = ((ctr) & 0xFF);
        ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, a_block,
                                    s_block);
        if (0 != ret) {
            goto exit;
        }

        for (i = 0; i < size; i++) {
            enc_buffer[bufferIndex + i] = buffer[bufferIndex + i] ^ s_block[i];
        }
    }

exit:
    mbedtls_aes_free(&aes_ctx);
    return ret;
}

int LoRaMacCrypto::decrypt_payload(const uint8_t *buffer, uint16_t size,
                                   const uint8_t *key, uint32_t key_length,
                                   uint32_t address, uint8_t dir, uint32_t seq_counter,
                                   uint8_t *dec_buffer)
{
    return encrypt_payload(buffer, size, key, key_length, address, dir, seq_counter,
                           dec_buffer);
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *buffer, uint16_t size,
                                          const uint8_t *key, uint32_t key_length,
                                          uint32_t *mic)
{
    uint8_t computed_mic[16] = {};
    int ret = 0;

    mbedtls_cipher_init(aes_cmac_ctx);
    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(aes_cmac_ctx, cipher_info);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_starts(aes_cmac_ctx, key, key_length);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_update(aes_cmac_ctx, buffer, size & 0xFF);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_finish(aes_cmac_ctx, computed_mic);
        if (0 != ret) {
            goto exit;
        }

        *mic = (uint32_t)((uint32_t) computed_mic[3] << 24
                          | (uint32_t) computed_mic[2] << 16
                          | (uint32_t) computed_mic[1] << 8 | (uint32_t) computed_mic[0]);
    } else {
        ret = MBEDTLS_ERR_CIPHER_ALLOC_FAILED;
    }

exit:
    mbedtls_cipher_free(aes_cmac_ctx);
    return ret;
}

int LoRaMacCrypto::decrypt_join_frame(const uint8_t *buffer, uint16_t size,
                                      const uint8_t *key, uint32_t key_length,
                                      uint8_t *dec_buffer)
{
    int ret = 0;

    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, key, key_length);
    if (0 != ret) {
        goto exit;
    }

    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, buffer,
                                dec_buffer);
    if (0 != ret) {
        goto exit;
    }

    // Check if optional CFList is included
    if (size >= 16) {
        ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, buffer + 16,
                                    dec_buffer + 16);
    }

exit:
    mbedtls_aes_free(&aes_ctx);
    return ret;
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *key, uint32_t key_length,
                                                const uint8_t *app_nonce, uint16_t dev_nonce,
                                                uint8_t *nwk_skey, uint8_t *app_skey)
{
    uint8_t nonce[16];
    uint8_t *p_dev_nonce = (uint8_t *) &dev_nonce;
    int ret = 0;

    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, key, key_length);
    if (0 != ret) {
        goto exit;
    }

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x01;
    memcpy(nonce + 1, app_nonce, 6);
    memcpy(nonce + 7, p_dev_nonce, 2);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, nwk_skey);
    if (0 != ret) {
        goto exit;
    }

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x02;
    memcpy(nonce + 1, app_nonce, 6);
    memcpy(nonce + 7, p_dev_nonce, 2);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, app_skey);

exit:
    mbedtls_aes_free(&aes_ctx);
    return ret;
}
#else

LoRaMacCrypto::LoRaMacCrypto()
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");
}

LoRaMacCrypto::~LoRaMacCrypto()
{
}

// If mbedTLS is not configured properly, these dummies will ensure that
// user knows what is wrong and in addition to that these ensure that
// Mbed-OS compiles properly under normal conditions where LoRaWAN in conjunction
// with mbedTLS is not being used.
int LoRaMacCrypto::compute_mic(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                               uint8_t dir, uint32_t, uint32_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::encrypt_payload(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                                   uint8_t, uint32_t, uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::decrypt_payload(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                                   uint8_t, uint32_t, uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::decrypt_join_frame(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *, uint32_t, const uint8_t *, uint16_t,
                                                uint8_t *, uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

#endif
