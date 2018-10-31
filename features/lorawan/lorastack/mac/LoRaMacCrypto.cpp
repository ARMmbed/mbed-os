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

#if defined(MBEDTLS_CMAC_C) && defined(MBEDTLS_AES_C) && defined(MBEDTLS_CIPHER_C)

/*
 * LoRaWAN spec 6.2: AppKey is AES-128 key
 */
#define APPKEY_KEY_LENGTH                           128

LoRaMacCrypto::LoRaMacCrypto() : _dev_addr(0)
{
    memset(&_keys, 0, sizeof(_keys));
}

lorawan_status_t LoRaMacCrypto::set_keys(uint8_t *nwk_key, uint8_t *app_key, uint8_t *nwk_skey,
                                         uint8_t *app_skey, uint8_t *snwk_sintkey, uint8_t *nwk_senckey)
{
    _keys.nwk_key = nwk_key;
    _keys.app_key = app_key;

    //ABP mode only, so all needs to be valid
    if (nwk_skey && app_skey && snwk_sintkey && nwk_senckey) {
        memcpy(_keys.nwk_skey, nwk_skey,
               sizeof(_keys.nwk_skey));

        memcpy(_keys.app_skey, app_skey,
               sizeof(_keys.app_skey));

        memcpy(_keys.nwk_skey, snwk_sintkey,
               sizeof(_keys.nwk_skey));

        memcpy(_keys.app_skey, nwk_senckey,
               sizeof(_keys.app_skey));
    }
    return LORAWAN_STATUS_OK;
}

int LoRaMacCrypto::compute_mic(const uint8_t *buffer, uint16_t size,
                               uint32_t args, uint32_t address,
                               uint8_t dir, uint32_t seq_counter,
                               uint32_t *mic)
{
    uint8_t computed_mic[16] = {};
    uint8_t mic_block_b0[16] = {};
    int ret = 0;

    //In case of LW_1_0_2 this is same as nwk_skey
    uint8_t *key = _keys.snwk_sintkey;

    //TODO: handle multicast based on address
    //_dev_addr

    mic_block_b0[0] = 0x49;

    mic_block_b0[1] = (args) & 0xFF;
    mic_block_b0[2] = (args >> 8) & 0xFF;
    mic_block_b0[3] = (args >> 16) & 0xFF;
    mic_block_b0[4] = (args >> 24) & 0xFF;

    mic_block_b0[5] = dir;

    mic_block_b0[6] = (address) & 0xFF;
    mic_block_b0[7] = (address >> 8) & 0xFF;
    mic_block_b0[8] = (address >> 16) & 0xFF;
    mic_block_b0[9] = (address >> 24) & 0xFF;

    mic_block_b0[10] = (seq_counter) & 0xFF;
    mic_block_b0[11] = (seq_counter >> 8) & 0xFF;
    mic_block_b0[12] = (seq_counter >> 16) & 0xFF;
    mic_block_b0[13] = (seq_counter >> 24) & 0xFF;

    mic_block_b0[14] = 0;

    mic_block_b0[15] = size & 0xFF;

    mbedtls_cipher_init(aes_cmac_ctx);

    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(aes_cmac_ctx, cipher_info);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_starts(aes_cmac_ctx, key, APPKEY_KEY_LENGTH);
        if (0 != ret)
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
                                   uint32_t address, uint8_t dir, uint32_t seq_counter,
                                   seq_counter_type_t seq_cnt_type,
                                   payload_type_t pld_type,
                                   uint8_t *enc_buffer,
                                   server_type_t serv_type, bool is_fopts)
{
    uint16_t i;
    uint8_t bufferIndex = 0;
    int ret = 0;
    uint8_t a_block[16] = {0};
    uint8_t s_block[16] = {0};
    const uint8_t *key;
    if (is_fopts) {
        key = _keys.nwk_senckey;
    } else {
        key = _keys.app_skey;
        //TODO: handle multicast based on address
        //_dev_addr
    }

    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, key, APPKEY_KEY_LENGTH);
    if (0 != ret)
        goto exit;
    }

    a_block[0] = 0x01;

    if (serv_type == LW1_1 && pld_type == FOPTS) {
        if ((seq_cnt_type == FCNT_UP || seq_cnt_type == NFCNT_DOWN)) {
            a_block[4] = 0x01;
        } else if (seq_cnt_type == AFCNT_DOWN) {
            a_block[4] = 0x02;
        }
    }

    a_block[5] = dir;

    a_block[6] = (address) & 0xFF;
    a_block[7] = (address >> 8) & 0xFF;
    a_block[8] = (address >> 16) & 0xFF;
    a_block[9] = (address >> 24) & 0xFF;

    a_block[10] = (seq_counter) & 0xFF;
    a_block[11] = (seq_counter >> 8) & 0xFF;
    a_block[12] = (seq_counter >> 16) & 0xFF;
    a_block[13] = (seq_counter >> 24) & 0xFF;

    a_block[15] = 0x01;

    while (size >= 16) {
        a_block[15]++;

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
                                   uint32_t address, uint8_t dir, uint32_t seq_counter,
                                   seq_counter_type_t seq_cnt_type,
                                   payload_type_t pld_type,
                                   uint8_t *dec_buffer,
                                   server_type_t serv_type, bool is_fopts)
{

    return encrypt_payload(buffer, size, address, dir, seq_counter,
                           seq_cnt_type, pld_type, dec_buffer, serv_type, is_fopts);
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *buffer, uint16_t size,
                                          join_frame_type_t type, uint32_t *mic)
{
    uint8_t computed_mic[16] = {};
    int ret = 0;

    uint8_t *key;
    if (type == JOIN_ACCEPT || type == REJOIN1_REQ) {
        key = _keys.js_intkey;
    } else if (type == JOIN_REQ) {
        key = _keys.nwk_key;
    } else { // REJOIN0_REQ || REJOIN2_REQ
        key = _keys.snwk_sintkey;
    }

    mbedtls_cipher_init(aes_cmac_ctx);
    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(aes_cmac_ctx, cipher_info);
        if (0 != ret) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_starts(aes_cmac_ctx, key, APPKEY_KEY_LENGTH);
        if (0 != ret)
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
                                      uint8_t *dec_buffer, bool is_join_req)
{
    int ret = 0;

    mbedtls_aes_init(&aes_ctx);

    uint8_t *key;
    if (is_join_req) {
        key = _keys.nwk_key;
    } else {
        key = _keys.js_enckey;
    }

    ret = mbedtls_aes_setkey_enc(&aes_ctx, key, APPKEY_KEY_LENGTH);
    if (0 != ret)
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

void LoRaMacCrypto::unset_js_keys()
{
    memcpy(_keys.js_intkey, _keys.nwk_key, sizeof(_keys.nwk_skey));
    memcpy(_keys.js_enckey, _keys.nwk_key, sizeof(_keys.nwk_skey));
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *args, uint8_t args_size,
                                                server_type_t stype)
{
    uint8_t nonce[16];
    int ret = 0;

    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, _keys.app_key, APPKEY_KEY_LENGTH);
    if (0 != ret)
        goto exit;

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x02;
    memcpy(nonce + 1, args, args_size);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.app_skey);
    if (0 != ret)
        goto exit;

    mbedtls_aes_free(&aes_ctx);
    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, _keys.nwk_key, APPKEY_KEY_LENGTH);
    if (0 != ret)
        goto exit;
    }

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x01;
    memcpy(nonce + 1, args, args_size);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.nwk_skey);
    if (0 != ret)
        goto exit;
    }

    if (stype == LW1_0_2) {
        memcpy(_keys.nwk_senckey, _keys.nwk_skey, APPKEY_KEY_LENGTH / 8);
        memcpy(_keys.snwk_sintkey, _keys.nwk_skey, APPKEY_KEY_LENGTH / 8);
    } else {
        memset(nonce, 0, sizeof(nonce));
        nonce[0] = 0x03;
        memcpy(nonce + 1, args, args_size);
        ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.snwk_sintkey);
        if (0 != ret)
            goto exit;

        memset(nonce, 0, sizeof(nonce));
        nonce[0] = 0x04;
        memcpy(nonce + 1, args, args_size);
        ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.nwk_senckey);
        if (0 != ret)
            goto exit;
    }

exit: mbedtls_aes_free(&aes_ctx);
    return ret;
}

int LoRaMacCrypto::compute_join_server_keys(const uint8_t *eui)
{
    uint8_t nonce[16];
    int ret = 0;

    if( MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_0_2 ) {
        memcpy(_keys.js_intkey, _keys.nwk_key, APPKEY_KEY_LENGTH/8);
        memcpy(_keys.js_enckey, _keys.nwk_key, APPKEY_KEY_LENGTH/8);
        return ret;
    }

    mbedtls_aes_init(&aes_ctx);

    ret = mbedtls_aes_setkey_enc(&aes_ctx, _keys.nwk_key, APPKEY_KEY_LENGTH);
    if (0 != ret)
        goto exit;

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x05;
    memcpy(nonce + 1, eui, 8);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.js_enckey);
    if (0 != ret)
        goto exit;

    memset(nonce, 0, sizeof(nonce));
    nonce[0] = 0x06;
    memcpy(nonce + 1, eui, 8);
    ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, nonce, _keys.js_intkey);
    if (0 != ret)
        goto exit;

exit:
    mbedtls_aes_free(&aes_ctx);
    return ret;
}

#else

LoRaMacCrypto::LoRaMacCrypto()
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");
}

lorawan_status_t LoRaMacCrypto::set_keys(uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

// If mbedTLS is not configured properly, these dummies will ensure that
// user knows what is wrong and in addition to that these ensure that
// Mbed-OS compiles properly under normal conditions where LoRaWAN in conjunction
// with mbedTLS is not being used.
int LoRaMacCrypto::compute_mic(const uint8_t *, uint16_t , uint32_t, uint32_t,
                               uint8_t, uint32_t, uint32_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::encrypt_payload(const uint8_t *, uint16_t ,
                                   uint32_t , uint8_t , uint32_t ,
                                   seq_counter_type_t ,
                                   payload_type_t ,
                                   uint8_t *,
                                   server_type_t , bool)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::decrypt_payload(const uint8_t *, uint16_t ,
                                   uint32_t , uint8_t , uint32_t ,
                                   seq_counter_type_t ,
                                   payload_type_t ,
                                   uint8_t *,
                                   server_type_t , bool)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *, uint16_t, join_frame_type_t, uint32_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::decrypt_join_frame(const uint8_t *, uint16_t , uint8_t *, bool)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *, uint8_t,
                                                server_type_t)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

int LoRaMacCrypto::compute_join_server_keys(const uint8_t *)
{
    MBED_ASSERT(0 && "[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORAWAN_STATUS_CRYPTO_FAIL;
}

#endif
