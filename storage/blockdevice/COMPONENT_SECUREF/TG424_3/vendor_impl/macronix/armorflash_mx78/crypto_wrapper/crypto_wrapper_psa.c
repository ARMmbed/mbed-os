/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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

#ifdef CRYPTO_PSA

#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../../../../JEDEC_security_HAL/crypto_wrapper.h"
#include "psa/crypto.h"

#define KEY_HANDLE_NOT_LOADED (0)
#define MAC_SIZE              (0x20)
#define TAG_LENGTH            (0x10)

#define MAX_ROOT_KEY_NUM      (0x10)
#define MAX_SESSION_KEY_NUM   (0x10)
#define MAC_KEY_SIZE          (0x20)
#define ENC_KEY_SIZE          (0x10)


typedef struct {
    uint32_t session_enc_key_id;
    uint32_t session_mac_key_id;
} session_key_id_t;


typedef struct {
    uint32_t root_key_id;
    uint32_t root_key_handle;
} root_key_handle_t;

static session_key_id_t session_key_id_slot[MAX_SESSION_KEY_NUM] = {KEY_HANDLE_NOT_LOADED};
static root_key_handle_t root_key_slot[MAX_ROOT_KEY_NUM] = {KEY_HANDLE_NOT_LOADED};

/**
 * \brief Find an empty session key slot.
 *
 * \param[out] slot_index  Returned empty slot index
 *
 * \return true if an empty session key slot was found,
 *         or false there's no empty session key slot
 */
static bool find_empty_session_key_id_slot(uint32_t *slot_index)
{
    for (uint32_t i = 0; i < MAX_SESSION_KEY_NUM; i++) {
        if ((session_key_id_slot[i].session_enc_key_id == KEY_HANDLE_NOT_LOADED) &&
            (session_key_id_slot[i].session_mac_key_id == KEY_HANDLE_NOT_LOADED)) {
            *slot_index = i;
            return true;
        }
    }
    return false;
}

/**
 * \brief Find the session key slot index corresponding to session key id.
 * \param[in] session_key_id  Input session key id
 * \param[out] slot_index     Returned empty slot index
 *
 * \return true if corresponding session key slot was found,
 *         or false there's no matching session key slot
 */
static bool find_session_key_id_slot(uint32_t session_key_id, uint32_t *slot_index)
{
    for (uint32_t i = 0; i < MAX_SESSION_KEY_NUM; i++) {
        if ((uint32_t)&session_key_id_slot[i] == session_key_id) {
            *slot_index = i;
            return true;
        }
    }
    return false;
}

/**
 * \brief Find an empty root key slot.
 *
 * \param[out] slot_index  Returned empty slot index
 *
 * \return true if an empty root key slot was found,
 *         or false there's no empty root key slot
 */
static bool find_empty_root_key_id_slot(uint32_t *slot_index)
{
    for (uint32_t i = 0; i < MAX_ROOT_KEY_NUM; i++) {
        if ((root_key_slot[i].root_key_handle == KEY_HANDLE_NOT_LOADED) &&
            (root_key_slot[i].root_key_id == KEY_HANDLE_NOT_LOADED)) {
            *slot_index = i;
            return true;
        }
    }
    return false;
}

/**
 * \brief Find the root key slot index corresponding to root key id.
 * \param[in] root_key_id  Input root key id
 * \param[out] slot_index  Returned empty slot index
 *
 * \return true if corresponding session key slot was found,
 *         or false there's no matching session key slot
 */
static bool find_root_key_id_slot(uint32_t root_key_id, uint32_t *slot_index)
{
    for (uint32_t i = 0; i < MAX_ROOT_KEY_NUM; i++) {
        if (root_key_slot[i].root_key_id == root_key_id) {
            *slot_index = i;
            return true;
        }
    }
    return false;
}

static int crypto_wrapper_init(void)
{
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_deinit(void)
{
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_mx78_kdf(crypto_indicator_t *indicator)
{
    psa_status_t status;
    uint8_t mac[MAC_SIZE];
    size_t mac_length;
    uint32_t vol_root_key_id;
    uint32_t index;
    /* session_key_id ==> {
     *                       session_enc_key_id,
     *                       session_mac_key_id
     *                     }
     */
    if (true == find_empty_session_key_id_slot(&index)) {
        indicator->hkdf.okm_id = (uint32_t)&session_key_id_slot[index];
    } else {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    /* Open root key to tf-m crypto key slot */
    status = psa_open_key(indicator->hkdf.prk_id, &vol_root_key_id);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_OPEN_KEY;
    }
    /* Multi-part mac compute */
    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;
    status = psa_mac_sign_setup(&operation,
                                vol_root_key_id,
                                PSA_ALG_HMAC(PSA_ALG_SHA_256));
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_update(&operation,
                            indicator->hkdf.info,
                            indicator->hkdf.info_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_sign_finish(&operation, mac, MAC_SIZE, &mac_length);
    if ((status != PSA_SUCCESS) ||
        (mac_length != MAC_SIZE)) {
        return JEDEC_ERROR_HMAC;
    }
    /* import volatile hmac key */
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&attributes, PSA_BYTES_TO_BITS(MAC_KEY_SIZE));
    status = psa_import_key(&attributes,
                            mac, MAC_KEY_SIZE,
                            &(session_key_id_slot[index].session_mac_key_id));
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_IMPORT_KEY;
    }
    /* import volatile aes-gcm key */
    memset(&attributes, 0x00, sizeof(attributes));
    psa_set_key_usage_flags(&attributes,
                            PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes,
                          PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_GCM, TAG_LENGTH));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, PSA_BYTES_TO_BITS(ENC_KEY_SIZE));
    status = psa_import_key(&attributes,
                            mac, ENC_KEY_SIZE,
                            &(session_key_id_slot[index].session_enc_key_id));
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_IMPORT_KEY;
    }
    /* Close root key handle from tf-m crypto key slot */
    status = psa_close_key(vol_root_key_id);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_CLOSE_KEY;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_ecdh_gen_key_pair(crypto_indicator_t *indicator)
{
    psa_status_t status;
    uint16_t key_bits;
    size_t exported_length = 0;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    /* Generate an ECC key pair */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
    psa_set_key_algorithm(&attributes, PSA_ALG_KEY_AGREEMENT(PSA_ALG_ECDH, 0x00));
    switch (indicator->algorithm) {
    case ALG_ECDH_SECP192R1:
    case ALG_ECDH_SECP224R1:
    case ALG_ECDH_SECP256R1:
    case ALG_ECDH_SECP384R1:
        psa_set_key_type(&attributes,
                         PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
        break;
    case ALG_ECDH_BP256R1:
    case ALG_ECDH_BP384R1:
        psa_set_key_type(&attributes,
                         PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1));
        break;
    case ALG_ECDH_SECP192K1:;
    case ALG_ECDH_SECP256K1:
        psa_set_key_type(&attributes,
                         PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1));
        break;
    default:
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    key_bits = 8 * indicator->ecdh.pri_key_len;
    psa_set_key_bits(&attributes, key_bits);
    status = psa_generate_key(&attributes, &indicator->ecdh.pri_key_id);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_GENERATE_KEY;
    }
    indicator->ecdh.pub_key_id = indicator->ecdh.pri_key_id;
    status = psa_export_public_key(indicator->ecdh.pub_key_id,
                                   indicator->ecdh.pub_key,
                                   indicator->ecdh.pub_key_len,
                                   &exported_length);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_EXPORT_KEY;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_ecdh_gen_shared_secret(crypto_indicator_t *indicator)
{
    psa_status_t status;
    uint32_t actual_size;
    status = psa_raw_key_agreement(PSA_ALG_ECDH,
                                   indicator->ecdh.pri_key_id,
                                   indicator->ecdh.pub_key,
                                   indicator->ecdh.pub_key_len,
                                   indicator->ecdh.secret,
                                   indicator->ecdh.secret_len,
                                   &actual_size);
    if (status) {
        return JEDEC_ERROR_GENERATE_KEY;
    }
    return JEDEC_ERROR_NONE;
}

/**
 * \brief Open root key corresponding to root_key_id.
 * \param[in] root_key_id       Input root key id
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_open_key(uint32_t root_key_id)
{
    psa_status_t status;
    uint32_t key_handle;
    uint32_t index;

    status = psa_open_key(root_key_id, &key_handle);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_OPEN_KEY;
    }
    if (find_empty_root_key_id_slot(&index) == true) {
        root_key_slot[index].root_key_id = root_key_id;
        root_key_slot[index].root_key_handle = key_handle;
    } else {
        return JEDEC_ERROR_INSUFFICIENT_MEMORY;
    }
    return JEDEC_ERROR_NONE;
}

/**
 * \brief Close root key corresponding to root_key_id.
 * \param[in] root_key_id       Input root key id
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_close_key(uint32_t root_key_id)
{
    psa_status_t status;
    uint32_t index;

    if (find_root_key_id_slot(root_key_id, &index) == true) {
        status = psa_close_key(root_key_slot[index].root_key_handle);
        if (status != PSA_SUCCESS) {
            return JEDEC_ERROR_CLOSE_KEY;
        }
        root_key_slot[index].root_key_handle = KEY_HANDLE_NOT_LOADED;
        root_key_slot[index].root_key_id = KEY_HANDLE_NOT_LOADED;
    } else {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_export_public_key(uint32_t key_id, uint8_t *key_buf,
                                            uint32_t buf_size, uint32_t *actual_size)
{
    psa_status_t status;
    status = psa_export_public_key(key_id, key_buf, buf_size,
                                   actual_size);
    if (status != PSA_SUCCESS) {
        return status;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_export_key(uint32_t key_id, uint8_t *key_buf,
                                     uint32_t buf_size, uint32_t *actual_size)
{
    psa_status_t status;
    status = psa_export_key(key_id, key_buf, buf_size,
                            actual_size);
    if (status != PSA_SUCCESS) {
        return status;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_import_key(uint32_t *key_id, uint8_t *key_buf,
                                     uint32_t key_size, KeyLifeTime lifetime)
{
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&attributes, PSA_BYTES_TO_BITS(key_size));
    if (lifetime == KEY_LIFETIME_VOLATILE) {
        *key_id = PSA_KEY_ID_NULL;
    } else {
        psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_PERSISTENT);
        psa_set_key_id(&attributes, *key_id);
    }
    status = psa_import_key(&attributes, key_buf, key_size, key_id);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_IMPORT_KEY;
    }
    return JEDEC_ERROR_NONE;
}
/**
 * \brief Destroy session key corresponding to session_key_id.
 * \param[in] session_key_id       Input session key id
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_destroy_key(uint32_t key_id)
{
    psa_status_t status;
    uint32_t index;

    if (find_session_key_id_slot(key_id, &index) == true) {
        /* Destroy session's encryption key */
        status = psa_destroy_key(session_key_id_slot[index].session_enc_key_id);
        if (status != PSA_SUCCESS) {
            return JEDEC_ERROR_DESTROY_KEY;
        }
        session_key_id_slot[index].session_enc_key_id = KEY_HANDLE_NOT_LOADED;
        /* Destroy session's hmac key */
        status = psa_destroy_key(session_key_id_slot[index].session_mac_key_id);
        if (status != PSA_SUCCESS) {
            return JEDEC_ERROR_DESTROY_KEY;
        }
        session_key_id_slot[index].session_mac_key_id = KEY_HANDLE_NOT_LOADED;
    } else {
        status = psa_destroy_key(key_id);
        if (status != PSA_SUCCESS) {
            return JEDEC_ERROR_DESTROY_KEY;
        }
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_aes_ccm(crypto_indicator_t *indicator)
{
    /* TODO */
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_aes_ecb(crypto_indicator_t *indicator)
{
    /* TODO */
    return JEDEC_ERROR_NONE;
}


/**
 * \brief Implement AEAD encryption with input session_key_id.
 * \param[in] session_key_id  Input session key id
 * \param[in] indicator       AEAD encryption parameters
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_aead_enc(uint32_t session_key_id, crypto_indicator_t *indicator)
{
    psa_status_t status;
    size_t out_len;

    if (sizeof(indicator->buf) <
        PSA_AEAD_ENCRYPT_OUTPUT_SIZE(PSA_KEY_TYPE_AES,
                                     PSA_ALG_GCM,
                                     indicator->aead.text_len)) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    session_key_id_t *session_key_id_ptr = (session_key_id_t *)session_key_id;
    status = psa_aead_encrypt((psa_key_id_t)session_key_id_ptr->session_enc_key_id,
                              PSA_ALG_GCM,
                              indicator->aead.iv, indicator->aead.iv_len,
                              indicator->aead.add, indicator->aead.add_len,
                              indicator->aead.plain_text, indicator->aead.text_len,
                              indicator->buf, sizeof(indicator->buf), &out_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_AEAD_ENC;
    }
    /* Authentication tag is appended to the encrypted data */
    /* NOTE:The indicator->aead.cipher_text has been set to point to send packet previously,
       here just copy the actual mac and ciphertext to send packet */
    indicator->aead.text_len = out_len -
                                      PSA_AEAD_TAG_LENGTH(PSA_KEY_TYPE_AES, 128, PSA_ALG_GCM);
    memcpy(indicator->aead.cipher_text, indicator->buf, indicator->aead.text_len);
    indicator->aead.tag_len = PSA_AEAD_TAG_LENGTH(PSA_KEY_TYPE_AES, 128, PSA_ALG_GCM);
    memcpy(indicator->aead.tag,
           indicator->buf + indicator->aead.text_len,
           indicator->aead.tag_len);
    return JEDEC_ERROR_NONE;
}

/**
 * \brief Implement AEAD decryption with input session_key_id.
 * \param[in] session_key_id  Input session key id
 * \param[in] indicator       AEAD decryption parameters
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_aead_dec(uint32_t session_key_id, crypto_indicator_t *indicator)
{
    psa_status_t status;
    size_t out_len;

    session_key_id_t *session_key_id_ptr = (session_key_id_t *)session_key_id;
    memcpy(indicator->buf, indicator->aead.cipher_text, indicator->aead.text_len);
    memcpy(indicator->buf + indicator->aead.text_len,
           indicator->aead.tag, indicator->aead.tag_len);
    status = psa_aead_decrypt((psa_key_id_t)session_key_id_ptr->session_enc_key_id,
                              PSA_ALG_GCM,
                              indicator->aead.iv, indicator->aead.iv_len,
                              indicator->aead.add, indicator->aead.add_len,
                              indicator->buf,
                              indicator->aead.text_len + indicator->aead.tag_len,
                              indicator->aead.plain_text,
                              indicator->aead.text_len, &out_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_AEAD_DEC;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_aes_gcm(crypto_indicator_t *indicator)
{
    psa_status_t status;
    size_t out_len;

    switch (indicator->property) {
    case PROP_AUTHEN_TAG_DECRYPT_DATA:
    case PROP_AUTHEN_TAG:
        return crypto_wrapper_aead_dec(indicator->aead.key_id, indicator);
        break;
    case PROP_ENCRYPT_TAG_DATA:
    case PROP_ENCRYPT_TAG:
        return crypto_wrapper_aead_enc(indicator->aead.key_id, indicator);
        break;
    default:
        return JEDEC_ERROR_AEAD;
    }
}


/**
 * \brief Compute MAC with input session key.
 * \param[in] session_key_id  Input session key id
 * \param[in] indicator       MAC operation parameters
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_mac_compute(uint32_t key_id, crypto_indicator_t *indicator)
{
    psa_status_t status;
    size_t actual_mac_size;
    uint32_t index;

    /* No need for computing mac */
    if ((indicator->hmac.idata == NULL) && (indicator->hmac.idata_len == 0)) {
        return JEDEC_ERROR_NONE;
    }
    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

    if (find_session_key_id_slot(key_id, &index) == true) {
        session_key_id_t *session_key_id_ptr = key_id;
        status = psa_mac_sign_setup(&operation,
                                    session_key_id_ptr->session_mac_key_id,
                                    PSA_ALG_HMAC(PSA_ALG_SHA_256));
    } else {
        status = psa_mac_sign_setup(&operation,
                                    key_id,
                                    PSA_ALG_HMAC(PSA_ALG_SHA_256));
    }
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_update(&operation,
                            indicator->hmac.idata,
                            indicator->hmac.idata_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_sign_finish(&operation,
                                 indicator->hmac.mac,
                                 indicator->hmac.mac_len,
                                 &actual_mac_size);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    return JEDEC_ERROR_NONE;
}

/**
 * \brief Implement MAC verify with input session key.
 * \param[in] session_key_id  Input session key id
 * \param[in] indicator       MAC verify parameters
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_mac_verify(uint32_t key_id, crypto_indicator_t *indicator)
{
    psa_status_t status;
    uint32_t index;

    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;
    if (find_session_key_id_slot(key_id, &index) == true) {
        session_key_id_t *session_key_id_ptr = key_id;
        status = psa_mac_verify_setup(&operation,
                                      session_key_id_ptr->session_mac_key_id,
                                      PSA_ALG_HMAC(PSA_ALG_SHA_256));
    } else {
        status = psa_mac_verify_setup(&operation,
                                      key_id,
                                      PSA_ALG_HMAC(PSA_ALG_SHA_256));
    }
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_update(&operation,
                            indicator->hmac.idata,
                            indicator->hmac.idata_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    status = psa_mac_verify_finish(&operation,
                                   indicator->hmac.mac,
                                   indicator->hmac.mac_len);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_HMAC;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_hmac(crypto_indicator_t *indicator)
{
    if ((0 == indicator->hmac.key_id && NULL == indicator->hmac.key) ||
        NULL == indicator->hmac.idata || 0 == indicator->hmac.idata_len) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    if (PROP_HMAC_VERIFY == indicator->property && NULL == indicator->hmac.mac) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    if (NULL == indicator->hmac.mac && 0 == indicator->key_attr.import_req) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    if (ALG_HMAC != ALG_TYPE(indicator->algorithm)) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    switch (indicator->property) {
    case PROP_HMAC_COMPUTE:
        return crypto_wrapper_mac_compute(indicator->hmac.key_id, indicator);
        break;
    case PROP_HMAC_VERIFY:
        return crypto_wrapper_mac_verify(indicator->hmac.key_id, indicator);
        break;
    default:
        return JEDEC_ERROR_INVALID_PARAM;
    }
}
/**
 * \brief Generate random number.
 * \param[out] output      Output buffer for generated random number
 * \param[in] output_size  Number of bytes to generate and output
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_generate_random(uint8_t *output, uint32_t output_size)
{
    psa_status_t status;

    status = psa_generate_random(output, output_size);
    if (status != PSA_SUCCESS) {
        return JEDEC_ERROR_GENERATE_RANDOM;
    }
    return JEDEC_ERROR_NONE;
}

const static crypto_func_t CRYPTO_FUNC[] = {
    0,                       //ALG_NONE    0
    crypto_wrapper_aes_ccm,  //ALG_AES_CCM 1
    crypto_wrapper_aes_gcm,  //ALG_AES_GCM 2
    crypto_wrapper_aes_ecb,  //ALG_AES_ECB 3
    0,                       //ALG_AES_CBC 4
    0,                       //ALG_AES_OFB 5
    0,                       //ALG_AES_CTR 6
    0,                       //ALG_ECDSA   7
    0,                       //ALG_ECDH    8
    crypto_wrapper_hmac,     //ALG_HMAC    9
    crypto_wrapper_mx78_kdf, //ALG_HKDF   10
};

static int crypto_wrapper_algorithm_support(int alg, int index)
{
    if (ALG_NONE == alg) {
        return JEDEC_ERROR_NOTHING;
    }
    if ((sizeof(CRYPTO_FUNC)/sizeof(crypto_func_t)) <= index || 0 == CRYPTO_FUNC[index]) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    return JEDEC_ERROR_NONE;
}

static int crypto_wrapper_crypto_func(crypto_indicator_t *indicator)
{
    int32_t status = JEDEC_ERROR_NONE, index = ALG_TYPE(indicator->algorithm) >> 4;

    status = crypto_wrapper_algorithm_support(indicator->algorithm, index);
    if (JEDEC_ERROR_NOTHING == status) {
        return JEDEC_ERROR_NONE;
    }
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    return CRYPTO_FUNC[index](indicator);
}

/**
 * \brief Generate session key from root key.
 * \param[in] crypto_indicator  KDF parameters
 * \param[out] session_key_id   Returned session key id
 *
 * \return JEDEC_ERROR_NONE if success,
 *         or JEDEC_ERROR_XX if fail
 */
static int crypto_wrapper_kdf(crypto_indicator_t *indicator,
                              uint32_t *output_key_id)
{
    int32_t status = JEDEC_ERROR_NONE, index = ALG_TYPE(indicator->algorithm) >> 4;

    status = crypto_wrapper_algorithm_support(indicator->algorithm, index);
    if (JEDEC_ERROR_NOTHING == status) {
        return JEDEC_ERROR_NONE;
    }
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    status = CRYPTO_FUNC[index](indicator);
    if (JEDEC_ERROR_NONE != status) {
        return JEDEC_ERROR_KDF;
    }
    switch (ALG_TYPE(indicator->algorithm)) {
    case ALG_HKDF:
        *output_key_id = indicator->hkdf.okm_id;
        break;
    case ALG_HMAC:
        *output_key_id = indicator->hmac.mac_id;
        break;
    default:
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    return JEDEC_ERROR_NONE;
}

crypto_wrapper_t mx78_armor_crypto_wrapper = {
    .init = crypto_wrapper_init,
    .deinit = crypto_wrapper_deinit,
    .algorithm_support = crypto_wrapper_algorithm_support,
    .crypto_func = crypto_wrapper_crypto_func,
    .key_derive = crypto_wrapper_kdf,
    .generate_random = crypto_wrapper_generate_random,
    .ecdh_gen_key_pair = crypto_wrapper_ecdh_gen_key_pair,
    .ecdh_gen_shared_secret = crypto_wrapper_ecdh_gen_shared_secret,
    .open_key = crypto_wrapper_open_key,
    .close_key = crypto_wrapper_close_key,
    .destroy_key = crypto_wrapper_destroy_key,
    .export_public_key = crypto_wrapper_export_public_key,
    .export_key = crypto_wrapper_export_key,
    .import_key = crypto_wrapper_import_key,

};
#endif /* CRYPTO_PSA */
