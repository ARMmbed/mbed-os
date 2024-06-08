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

#ifdef CRYPTO_MBEDTLS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../../../JEDEC_security_HAL/crypto_wrapper.h"
#include "mbedtls/entropy.h"
#include "mbedtls/gcm.h"
#include "mbedtls/ccm.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/hkdf.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ctr_drbg.h"
#include "../../../spi_nor_flash/spi_nor.h"

#define KEY_SIZE ((256 / 8) * 2 + 1)
#define KEY_MAX_NUM 0x20
#define KEY_UNUSED 0
#define KEY_USED 1

#define KEY_HANDLE_NOT_LOADED 0
#define MAC_SIZE              0x20
#define TAG_LENGTH            0x10

#define MAX_ROOT_KEY_NUM      0x10
#define MAX_SESSION_KEY_NUM   0x10
#define MAC_KEY_SIZE          0x20
#define ENC_KEY_SIZE          0x10

#define KEY_ID_LEN            4

typedef struct {
        uint8_t value[KEY_SIZE];
        uint32_t id;
        uint8_t used:1,
                len:7;
} key_manager_t;

typedef struct {
    uint32_t enc_key_id;
    uint32_t mac_key_id;
} session_key_id_t;

static session_key_id_t session_key_id_slot[MAX_SESSION_KEY_NUM] = {};
static key_manager_t key_manager[KEY_MAX_NUM] = {};

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
        if ((session_key_id_slot[i].enc_key_id == KEY_HANDLE_NOT_LOADED) &&
            (session_key_id_slot[i].mac_key_id == KEY_HANDLE_NOT_LOADED)) {
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

static int mbedtls_if_init()
{
    memset(key_manager, 0x00, sizeof(key_manager));
    memset(session_key_id_slot, 0x00, sizeof(session_key_id_slot));
    return JEDEC_ERROR_NONE;
}

static int mbedtls_if_deinit()
{
    memset(key_manager, 0x00, sizeof(key_manager));
    memset(session_key_id_slot, 0x00, sizeof(session_key_id_slot));
    return JEDEC_ERROR_NONE;
}

static int mbedtls_if_generate_random(uint8_t *odata, uint32_t odata_len)
{
    int32_t status = JEDEC_ERROR_NONE;
    mbedtls_entropy_context entropy_ctx;
    mbedtls_ctr_drbg_context ctr_drbg_ctx;

    mbedtls_entropy_init(&entropy_ctx);
    mbedtls_ctr_drbg_init(&ctr_drbg_ctx);

    if (0 != mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx, NULL, 0)) {
        status = JEDEC_ERROR_GENERATE_RANDOM;
        goto generate_random_exit_point;
    }
    
    if (0 != mbedtls_ctr_drbg_random(&ctr_drbg_ctx, odata, odata_len)) {
        status = JEDEC_ERROR_GENERATE_RANDOM;
        goto generate_random_exit_point;
    }

generate_random_exit_point:
    mbedtls_entropy_free(&entropy_ctx);
    mbedtls_ctr_drbg_free(&ctr_drbg_ctx);

    return status;
}

static int simple_import_key(uint32_t *key_id, const uint8_t *key, uint8_t key_len)
{
    int32_t status = JEDEC_ERROR_NONE;

    if (0 == *key_id) {
        uint8_t i = 0;
        do {
            status = mbedtls_if_generate_random((uint8_t *)key_id, KEY_ID_LEN);
            if (JEDEC_ERROR_NONE != status) {
                return status;
            }
            for (i = 0; i < KEY_MAX_NUM; i++) {
                if (KEY_USED == key_manager[i].used && *key_id == key_manager[i].id) {
                    break;
                }
            }
        }while (KEY_MAX_NUM != i);
    }
    for (uint8_t i = 0; i < KEY_MAX_NUM; i++) {
        if (KEY_USED == key_manager[i].used && *key_id == key_manager[i].id) {
            if (!memcmp(key, key_manager[i].value, key_manager[i].len)) {
                return JEDEC_ERROR_NONE;
            }            
            return JEDEC_ERROR_KEY_ID_CONFLICT;
        }
        if (KEY_UNUSED == key_manager[i].used) {
            memcpy(key_manager[i].value, key, key_len);
            key_manager[i].id = *key_id;
            key_manager[i].len = key_len;
            key_manager[i].used = KEY_USED;
            return JEDEC_ERROR_NONE;
        }
    }
    return JEDEC_ERROR_INSUFFICIENT_MEMORY;
}

static int simple_export_key(uint32_t key_id, const uint8_t *key, uint8_t key_len, uint8_t *actual_key_len)
{
    for (uint8_t i = 0; i < KEY_MAX_NUM; i++) {
        if (KEY_USED == key_manager[i].used && key_id == key_manager[i].id) {
            if (key_len < key_manager[i].len) {
                return JEDEC_ERROR_EXPORT_KEY;
            }
            memcpy(key, key_manager[i].value, key_manager[i].len);
            *actual_key_len = key_manager[i].len;
            return JEDEC_ERROR_NONE;
        }
    }
    return JEDEC_ERROR_EXPORT_KEY;
}

static int simple_destroy_key(uint32_t key_id)
{
    uint32_t index, target_key_id;

    if (true == find_session_key_id_slot(key_id, &index)) {
        target_key_id = session_key_id_slot[index].mac_key_id;
        
        for (uint8_t i = 0; i < KEY_MAX_NUM; i++) {
            if (target_key_id == key_manager[i].id) {
                memset(&key_manager[i], 0x00, sizeof(key_manager_t));
                break;
            }
        }
        target_key_id = session_key_id_slot[index].enc_key_id;
        
        for (uint8_t i = 0; i < KEY_MAX_NUM; i++) {
            if (target_key_id == key_manager[i].id) {
                memset(&key_manager[i], 0x00, sizeof(key_manager_t));
                break;
            }
        }
        memset(&session_key_id_slot[index], 0x00, sizeof(session_key_id_t));
        return JEDEC_ERROR_NONE;
    }    
    for (uint8_t i = 0; i < KEY_MAX_NUM; i++) {
        if (key_id == key_manager[i].id) {
            memset(&key_manager[i], 0x00, sizeof(key_manager_t));
            return JEDEC_ERROR_NONE;
        }
    }
    return JEDEC_ERROR_KEY_ID_NOT_FOUND;
}

static int simple_open_key(uint32_t key_id)
{
    (void)key_id;
    return JEDEC_ERROR_NONE;
}

static int simple_close_key(uint32_t key_id)
{
    (void)key_id;
    return JEDEC_ERROR_NONE;
}

static  int mbedtls_if_aes_gcm(crypto_indicator_t *indicator)
{
	int32_t status = JEDEC_ERROR_NONE, mbedl_status, index;
    mbedtls_gcm_context ctx = {};
    uint8_t key[16];
    uint8_t actual_key_len = 0;    

    mbedtls_gcm_init(&ctx);    
    
    /* export key from session key slot or key manager */
    bool ret = find_session_key_id_slot(indicator->aead.key_id, &index);
    if (true == ret) {
        status = simple_export_key(session_key_id_slot[index].enc_key_id, key, indicator->aead.key_len, &actual_key_len);
        if (JEDEC_ERROR_NONE != status) {
            goto aes_gcm_exit_point;
        }            
    } else {
        status = simple_export_key(indicator->aead.key_id, key, indicator->aead.key_len, &actual_key_len);
        if (JEDEC_ERROR_NONE != status) {
            goto aes_gcm_exit_point;
        }
    }

    if (0 != mbedtls_gcm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, indicator->aead.key_len * 8)) {
        status = JEDEC_ERROR_AEAD;
        goto aes_gcm_exit_point;
    }
    switch (indicator->property) {
    case PROP_AUTHEN_TAG_DECRYPT_DATA:
    case PROP_AUTHEN_TAG:
        mbedl_status = mbedtls_gcm_crypt_and_tag(&ctx, MBEDTLS_GCM_DECRYPT,
                                                  indicator->aead.text_len,
                                                  indicator->aead.iv, indicator->aead.iv_len,
                                                  indicator->aead.add, indicator->aead.add_len,
                                                  indicator->aead.cipher_text, indicator->aead.plain_text,
                                                  indicator->aead.tag_len, indicator->aead.tag);
        if (0 != mbedl_status) {            
            status = JEDEC_ERROR_AEAD;
            goto aes_gcm_exit_point;
        }        
        break;     
    case PROP_ENCRYPT_TAG_DATA:
    case PROP_ENCRYPT_TAG:        
        mbedl_status = mbedtls_gcm_crypt_and_tag(&ctx, MBEDTLS_GCM_ENCRYPT,
                                                 indicator->aead.text_len,
                                                 indicator->aead.iv, indicator->aead.iv_len,
                                                 indicator->aead.add, indicator->aead.add_len,
                                                 indicator->aead.plain_text, indicator->aead.cipher_text,
                                                 indicator->aead.tag_len, indicator->aead.tag);
        if (0 != mbedl_status) {
            status = JEDEC_ERROR_AEAD;
            goto aes_gcm_exit_point;
        }
        break;
    default:
        status = JEDEC_ERROR_AEAD;
        goto aes_gcm_exit_point;
    }
aes_gcm_exit_point:    
    mbedtls_gcm_free(&ctx);
    return status;
}

static mbedtls_md_type_t sha_select_md_type(CryptoAlgorithm alg)
{
    switch (alg) {
    case ALG_HKDF_SHA_1:
    case ALG_HMAC_SHA_1:
		return MBEDTLS_MD_SHA1;
    case ALG_HKDF_SHA_224:
    case ALG_HMAC_SHA_224:
    	return MBEDTLS_MD_SHA224;
    case ALG_HKDF_SHA_256:
    case ALG_HMAC_SHA_256:
		return MBEDTLS_MD_SHA256;
    case ALG_HKDF_SHA_384:
    case ALG_HMAC_SHA_384:
    	return MBEDTLS_MD_SHA384;
    case ALG_HKDF_SHA_512:
    case ALG_HMAC_SHA_512:
	    return MBEDTLS_MD_SHA512;
	default:
	    return MBEDTLS_MD_NONE;
    }    
    return MBEDTLS_MD_NONE;
}

static mbedtls_ecp_group_id ecdh_select_grp(CryptoAlgorithm alg)
{
    switch (alg) {
    case ALG_ECDH_SECP192R1:
        return MBEDTLS_ECP_DP_SECP192R1;
    case ALG_ECDH_SECP224R1:
        return MBEDTLS_ECP_DP_SECP224R1;
    case ALG_ECDH_SECP256R1:
        return MBEDTLS_ECP_DP_SECP256R1;
    case ALG_ECDH_SECP384R1:
        return MBEDTLS_ECP_DP_SECP384R1;
    // case ALG_ECDH_SECP521R1:
    //     return MBEDTLS_ECP_DP_SECP521R1;
    case ALG_ECDH_BP256R1:
        return MBEDTLS_ECP_DP_BP256R1;
    case ALG_ECDH_BP384R1:
        return MBEDTLS_ECP_DP_BP384R1;
    // case ALG_ECDH_BP512R1:
    //     return MBEDTLS_ECP_DP_BP512R1;
    case ALG_ECDH_CURVE25519:
        return MBEDTLS_ECP_DP_CURVE25519;
    case ALG_ECDH_SECP192K1:
        return MBEDTLS_ECP_DP_SECP192K1;
    case ALG_ECDH_SECP224K1:
        return MBEDTLS_ECP_DP_SECP224K1;
    case ALG_ECDH_SECP256K1:
        return MBEDTLS_ECP_DP_SECP256K1;
    default:
        return MBEDTLS_ECP_DP_NONE;
    }
    return MBEDTLS_ECP_DP_NONE;
}

static int mbedtls_if_ecdh_gen_key_pair(crypto_indicator_t *indicator)
{
    /* The procedure refer to the Mbedtls's test suite (test_suite_ecdh.c) */
    int32_t status = JEDEC_ERROR_NONE;
    size_t olen;
    uint8_t pri_key[(256/8)+1] = {}, pub_key[(512/8)+1] ={};
    mbedtls_ecp_group_id ecp_grp_id;
    mbedtls_mpi host_pri;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point host_pub;
    mbedtls_entropy_context entropy_ctx;
    mbedtls_ctr_drbg_context ctr_drbg_ctx;

    /* Init. data struct */
    mbedtls_mpi_init(&host_pri);
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&host_pub);
    mbedtls_entropy_init(&entropy_ctx);
    mbedtls_ctr_drbg_init(&ctr_drbg_ctx);
    /* Update random seed use custom string(pers) */
    mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx, NULL, 0);
    ecp_grp_id = ecdh_select_grp(indicator->algorithm);
    if (ecp_grp_id == MBEDTLS_ECP_DP_NONE) {
        status = JEDEC_ERROR_NOT_SUPPORT;
        goto mbedtls_if_ecdh_gen_key_pair_exit_point;
    }    
    mbedtls_ecp_group_load(&grp, ecp_grp_id);        
    /* Generate an ECDH key pair on an elliptic curve, private key in host_pri and public key in host_pub */
    mbedtls_ecdh_gen_public(&grp, &host_pri, &host_pub, mbedtls_ctr_drbg_random, &ctr_drbg_ctx);
    /* Derived public key */
    switch (indicator->ecdh.pub_key_type) {
    case RAW_PUB_KEY:
        mbedtls_ecp_point_write_binary(&grp, &host_pub, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, 
                                       pub_key, indicator->ecdh.pub_key_len + 1);
        memmove(pub_key, &pub_key[1], olen - 1);
        if(indicator->ecdh.pub_key_len != (olen - 1)) {
            status = JEDEC_ERROR_INV_ARGS;
            goto mbedtls_if_ecdh_gen_key_pair_exit_point;
        }        
        break;
    case UNCOMPRESSED_PUB_KEY:
        mbedtls_ecp_point_write_binary(&grp, &host_pub, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, 
                                       pub_key, indicator->ecdh.pub_key_len);
        if(indicator->ecdh.pub_key_len != olen) {
            status = JEDEC_ERROR_INV_ARGS;
            goto mbedtls_if_ecdh_gen_key_pair_exit_point;
        }
        break;
    case COMPRESSED_PUB_KEY:
        mbedtls_ecp_point_write_binary(&grp, &host_pub, MBEDTLS_ECP_PF_COMPRESSED, &olen, 
                                       pub_key, indicator->ecdh.pub_key_len);
        if(indicator->ecdh.pub_key_len != olen) {
            status = JEDEC_ERROR_INV_ARGS;
            goto mbedtls_if_ecdh_gen_key_pair_exit_point;
        }
        break;
    default:
        status = JEDEC_ERROR_NOT_SUPPORT;
        goto mbedtls_if_ecdh_gen_key_pair_exit_point;
    }
    memcpy(indicator->ecdh.pub_key, pub_key, indicator->ecdh.pub_key_len);
    indicator->ecdh.pri_key_len = mbedtls_mpi_size(&host_pri);    

    /* Derived private key */
    mbedtls_mpi_write_binary(&host_pri, pri_key, mbedtls_mpi_size(&host_pri));    
    status = simple_import_key(&indicator->ecdh.pub_key_id, indicator->ecdh.pub_key, indicator->ecdh.pub_key_len);
    if (status != JEDEC_ERROR_NONE) {
        goto mbedtls_if_ecdh_gen_key_pair_exit_point;
    }
    status = simple_import_key(&indicator->ecdh.pri_key_id, pri_key, indicator->ecdh.pri_key_len);    
    if (status != JEDEC_ERROR_NONE) {
        goto mbedtls_if_ecdh_gen_key_pair_exit_point;
    }
    memset(pri_key, 0x00, sizeof(pri_key));
mbedtls_if_ecdh_gen_key_pair_exit_point:
    mbedtls_mpi_free(&host_pri);
    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&host_pub);
    mbedtls_entropy_free(&entropy_ctx);
    mbedtls_ctr_drbg_free(&ctr_drbg_ctx);
    return status;
}

static int32_t mbedlts_if_ecdh_gen_shared_secret(crypto_indicator_t *indicator)
{
    int32_t status = JEDEC_ERROR_NONE;
    const char *pers = "mbedtls_ecdh";
    mbedtls_mpi host_secret;
    mbedtls_mpi host_pri;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point dev_pub;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ecp_group_id ecp_grp_id;    
    uint8_t host_pri_key[256 / 8 +1] = {};
    uint8_t secret[256 / 8] = {};
    uint8_t actual_key_len;
    uint8_t pub_key_half_len, ofs = 0;

    if (NULL == indicator->ecdh.secret && !indicator->key_attr.import_req) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    /* Init. data struct */
    mbedtls_mpi_init(&host_secret);
    mbedtls_mpi_init(&host_pri);
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_point_init(&dev_pub);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    /* Update random seed use custom string(pers) */
    // mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t *)pers, strlen(pers));
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
    ecp_grp_id = ecdh_select_grp(indicator->algorithm);
    if (ecp_grp_id == MBEDTLS_ECP_DP_NONE) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }    
    mbedtls_ecp_group_load(&grp, ecp_grp_id);
    status = simple_export_key(indicator->ecdh.pri_key_id, host_pri_key, sizeof(host_pri_key), &actual_key_len);
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    switch (indicator->ecdh.pub_key_type) {
    case RAW_PUB_KEY:
        pub_key_half_len = indicator->ecdh.pub_key_len / 2;
        ofs = 0;
        break;
    case UNCOMPRESSED_PUB_KEY:
        pub_key_half_len = (indicator->ecdh.pub_key_len - 1) / 2;
        ofs = 1;
        break;
    default:
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    /* Import device public key to dev_pub */
    mbedtls_mpi_read_binary(&dev_pub.X, indicator->ecdh.pub_key + ofs, pub_key_half_len);
    mbedtls_mpi_read_binary(&dev_pub.Y, indicator->ecdh.pub_key + ofs + pub_key_half_len, pub_key_half_len);
    mbedtls_mpi_lset(&dev_pub.Z, 1);

    /* Import host private key to host_pri */
    mbedtls_mpi_read_binary(&host_pri, host_pri_key, indicator->ecdh.pri_key_len);

    /* Compute the shared secret */
    mbedtls_ecdh_compute_shared(&grp, &host_secret, &dev_pub, &host_pri, mbedtls_ctr_drbg_random, &ctr_drbg);

    /* Derived shared secret */
    mbedtls_mpi_write_binary(&host_secret, secret, mbedtls_mpi_size(&host_secret));
    indicator->ecdh.secret_len = mbedtls_mpi_size(&host_secret);

    if (indicator->key_attr.import_req) {
        status = simple_import_key(&indicator->ecdh.secret_id, secret, indicator->ecdh.secret_len);
        if (status != JEDEC_ERROR_NONE) {
            return JEDEC_ERROR_GENERATE_KEY;
        }
        memset(secret, 0x00, indicator->ecdh.secret_len);
    } else {
        memcpy(indicator->ecdh.secret, secret, indicator->ecdh.secret_len);
    }

    mbedtls_mpi_free(&host_pri);
    mbedtls_mpi_free(&host_secret);
    mbedtls_ecp_group_free(&grp);
    mbedtls_ecp_point_free(&dev_pub);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return status;
}

static int mbedtls_if_hmac_compute(crypto_indicator_t *indicator, mbedtls_md_info_t *md, uint8_t *key)
{
    int32_t status = JEDEC_ERROR_NONE;
    uint8_t mac[256 / 8] = {};

	if (0 != mbedtls_md_hmac(md, key, indicator->hmac.key_len, indicator->hmac.idata, indicator->hmac.idata_len, mac)) {
        return JEDEC_ERROR_HMAC;
    }
    if (indicator->key_attr.import_req) {
        status = simple_import_key(&indicator->hmac.mac_id, mac, indicator->hmac.key_len);
        if (JEDEC_ERROR_NONE != status) {
            return status;
        }
        memset (mac, 0x00, indicator->hmac.mac_len);
    } else {
        memcpy(indicator->hmac.mac, mac, indicator->hmac.mac_len);
    }
    return status;	
}

static int mbedtls_if_hmac_verify(crypto_indicator_t *indicator, mbedtls_md_info_t *md, uint8_t *key)
{    
    uint8_t mac[256 / 8] = {};

    if (0 != mbedtls_md_hmac(md,
     		                 key,
						     indicator->hmac.key_len,
                             indicator->hmac.idata,
						     indicator->hmac.idata_len,
						     mac)) {
        return JEDEC_ERROR_HMAC;
    }
    if (memcmp(mac, indicator->hmac.mac, indicator->hmac.mac_len)) {
        return JEDEC_ERROR_HMAC;
    }    
    return JEDEC_ERROR_NONE;
}

static int mbedtls_if_hmac(crypto_indicator_t *indicator)
{
    int32_t status = JEDEC_ERROR_NONE;
    mbedtls_md_type_t md_type = MBEDTLS_MD_NONE;
    mbedtls_md_info_t *md = NULL;
    uint8_t key[256 / 8], actual_key_len = 0;

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
    md_type = sha_select_md_type(indicator->algorithm);
	if (MBEDTLS_MD_NONE == md_type) {
		return JEDEC_ERROR_NOT_SUPPORT;
	}
    md = mbedtls_md_info_from_type(md_type);
    if (NULL == md) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    if (indicator->hmac.mac_len != mbedtls_md_get_size(md)) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    /* If hmac.key is NULL, then export key from session key slot or key manager */
    if (NULL == indicator->hmac.key) {
        int index;
        bool ret = find_session_key_id_slot(indicator->hmac.key_id, & index);
        if (true == ret) {
            status = simple_export_key(session_key_id_slot[index].mac_key_id, key, indicator->hmac.key_len, &actual_key_len);
            if (JEDEC_ERROR_NONE != status) {
                return status;
            }            
        } else {
            status = simple_export_key(indicator->hmac.key_id, key, indicator->hmac.key_len, &actual_key_len);
            if (JEDEC_ERROR_NONE != status) {
                return status;
            }
        }
    } else {
        memcpy(key , indicator->hmac.key, indicator->hmac.key_len);
    }
    
    switch (indicator->property) {
    case PROP_HMAC_COMPUTE:
        status = mbedtls_if_hmac_compute(indicator, md, key);
        break;
    case PROP_HMAC_VERIFY:    
        status = mbedtls_if_hmac_verify(indicator, md, key);
        break;
    default:
        return JEDEC_ERROR_INVALID_PARAM;
    }
    return status;
}

static int mbedtls_if_mx78_kdf(crypto_indicator_t *indicator)
{
    int status = JEDEC_ERROR_NONE, index = 0;
    uint8_t actual_key_len = 0, prk[256 / 8], okm[256 / 8];
	mbedtls_md_type_t md_type = MBEDTLS_MD_NONE;
    mbedtls_md_info_t *md = NULL;
    
    if (true != find_empty_session_key_id_slot(&index)) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    md_type = sha_select_md_type(indicator->algorithm);
    if (MBEDTLS_MD_NONE == md_type) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    md = mbedtls_md_info_from_type(md_type);
    if (NULL == md) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }
    if (indicator->hkdf.prk_len != mbedtls_md_get_size(md)) {
        return JEDEC_ERROR_INVALID_PARAM;
    }
    if (NULL == indicator->hkdf.prk) {
        status = simple_export_key(indicator->hkdf.prk_id, prk, indicator->hkdf.prk_len, &actual_key_len);
        if (status != JEDEC_ERROR_NONE) {
            return status;
        }
    } else {
        memcpy(prk, indicator->hkdf.prk, indicator->hkdf.prk_len);
    }
    if (0 != mbedtls_md_hmac(md, prk, indicator->hkdf.prk_len, indicator->hkdf.info, indicator->hkdf.info_len, okm)) {
        return JEDEC_ERROR_KDF;
    }
    /* import mac and enc key */
    status = simple_import_key(&session_key_id_slot[index].mac_key_id, okm, MAC_KEY_SIZE);
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    status = simple_import_key(&session_key_id_slot[index].enc_key_id, okm, ENC_KEY_SIZE);
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    memset(okm, 0x00, mbedtls_md_get_size(md));
    memset(prk, 0x00, mbedtls_md_get_size(md));
    indicator->hkdf.okm_id = (uint32_t)&session_key_id_slot[index];
    return status;

}

const static crypto_func_t CRYPTO_FUNC[] = {
    0,                   //ALG_NONE   0
    0,                   //ALG_AES_CCM 1
    mbedtls_if_aes_gcm,  //ALG_AES_GCM 2
    0,                   //ALG_AES_ECB 3
    0,                   //ALG_AES_CBC 4
    0,                   //ALG_AES_OFB 5
    0,                   //ALG_AES_CTR 6
    0,                   //ALG_ECDSA   7
    0,                   //ALG_ECDH    8
    mbedtls_if_hmac,     //ALG_HMAC    9
    mbedtls_if_mx78_kdf, //ALG_HKDF   10
};

static int mbedtls_if_algorithm_support(int alg, int index)
{
    if (ALG_NONE == alg) {
        return JEDEC_ERROR_NOTHING;
    }
    if ((sizeof(CRYPTO_FUNC)/sizeof(crypto_func_t)) <= index || 0 == CRYPTO_FUNC[index]) {
        return JEDEC_ERROR_NOT_SUPPORT;
    }    
    return JEDEC_ERROR_NONE;
}

static int mbedtls_if_crypto_func(crypto_indicator_t *indicator)
{
    int status = JEDEC_ERROR_NONE, index = ALG_TYPE(indicator->algorithm) >> 4;
        
    status = mbedtls_if_algorithm_support(indicator->algorithm, index);
    if (JEDEC_ERROR_NOTHING == status) {
        return JEDEC_ERROR_NONE;
    }
    if (JEDEC_ERROR_NONE != status) {
        return status;
    }
    return CRYPTO_FUNC[index](indicator);
}

static int mbedtls_if_key_derive(crypto_indicator_t *indicator, uint32_t *output_key_id)
{
    int32_t status = JEDEC_ERROR_NONE, index = ALG_TYPE(indicator->algorithm) >> 4;
    
    status = mbedtls_if_algorithm_support(indicator->algorithm, index);
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

const crypto_wrapper_t mx78_armor_crypto_wrapper = {
    .init = mbedtls_if_init,
    .deinit = mbedtls_if_deinit,
    .algorithm_support = mbedtls_if_algorithm_support,
    .crypto_func = mbedtls_if_crypto_func,
    .key_derive = mbedtls_if_key_derive,
    .generate_random = mbedtls_if_generate_random,
    .ecdh_gen_key_pair = mbedtls_if_ecdh_gen_key_pair,
    .ecdh_gen_shared_secret = mbedlts_if_ecdh_gen_shared_secret,
    .open_key = simple_open_key,
    .close_key = simple_close_key,
    .destroy_key = simple_destroy_key,
    .export_key = simple_export_key,
    .import_key = simple_import_key
};
#endif /* CRYPTO_MBEDTLS */
