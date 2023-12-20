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
#ifndef _CRYPTO_DEFS_H_
#define _CRYPTO_DEFS_H_

#include <stdint.h>
#include "error.h"

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * \brief Cryptographic service error code
 *
 */

#define JEDEC_ERROR_NOTHING             -0x1001
#define JEDEC_ERROR_AEAD_ENC            -0x1002
#define JEDEC_ERROR_AEAD_DEC            -0x1003
#define JEDEC_ERROR_CIPHER_ENC          -0x1004
#define JEDEC_ERROR_CIPHER_DEC          -0x1005
#define JEDEC_ERROR_AEAD                -0x1006
#define JEDEC_ERROR_HMAC                -0x1007
#define JEDEC_ERROR_KDF                 -0x1008
#define JEDEC_ERROR_MAC_COMPUTE         -0x1009
#define JEDEC_ERROR_MAC_VERIFY          -0x100A
#define JEDEC_ERROR_GENERATE_RANDOM     -0x100B
#define JEDEC_ERROR_OPEN_KEY            -0x100C
#define JEDEC_ERROR_CLOSE_KEY           -0x100D
#define JEDEC_ERROR_EXPORT_KEY          -0x100E
#define JEDEC_ERROR_IMPORT_KEY          -0x100F
#define JEDEC_ERROR_DESTROY_KEY         -0x1010
#define JEDEC_ERROR_INVALID_PARAM       -0x1011
#define JEDEC_ERROR_NOT_SUPPORT         -0x1012
#define JEDEC_ERROR_GENERATE_KEY        -0x1013
#define JEDEC_ERROR_KEY_ID_CONFLICT     -0x1014
#define JEDEC_ERROR_KEY_ID_NOT_FOUND    -0x1015
#define JEDEC_ERROR_KEY_OPERATION       -0x1016
#define JEDEC_ERROR_RANDOM_GEN          -0x1017
#define JEDEC_ERROR_MAC                 -0x1018
#define JEDEC_ERROR_CRYPTO              -0x1019

#define MAX_CIPHER_DATA_SIZE 1024

#define INVALID_KEY_ID       0          /*!< Invalid key id */
#define RESVD_KEY_ID         0xFFFFFFFF /*!< Reserved key id */

#define ALG_TYPE(ALG) (ALG & 0xFFFFFFF0)

/* \brief Supported cipher types and modes
 *
 */
typedef enum {
    ALG_NONE = 0x00,             // 0x00

    ALG_AES_CCM = 0x10,          // 0x10
    ALG_AES_CCM_128,             // 0x11
    ALG_AES_CCM_192,             // 0x12
    ALG_AES_CCM_256,             // 0x13
    ALG_AES_GCM = 0x20,          // 0x20
    ALG_AES_GCM_128,             // 0x21
    ALG_AES_GCM_192,             // 0x22
    ALG_AES_GCM_256,             // 0x23
    ALG_AES_ECB = 0x30,          // 0x30
    ALG_AES_ECB_128,             // 0x31
    ALG_AES_ECB_192,             // 0x32
    ALG_AES_ECB_256,             // 0x33
    ALG_AES_CBC = 0x40,          // 0x40
    ALG_AES_CBC_128,             // 0x41
    ALG_AES_CBC_192,             // 0x42
    ALG_AES_CBC_256,             // 0x43
    ALG_AES_OFB = 0x50,          // 0x50
    ALG_AES_OFB_128,             // 0x51
    ALG_AES_OFB_192,             // 0x52
    ALG_AES_OFB_256,             // 0x53
    ALG_AES_CTR = 0x60,          // 0x60
    ALG_AES_CTR_128,             // 0x61
    ALG_AES_CTR_192,             // 0x62
    ALG_AES_CTR_256,             // 0x63

    ALG_ECDSA = 0x70,            // 0x70
    ALG_ECDSA_SECP192R1,         // 0x71
    ALG_ECDSA_SECP224R1,         // 0x72
    ALG_ECDSA_SECP256R1,         // 0x73
    ALG_ECDSA_SECP384R1,         // 0x74
    ALG_ECDSA_SECP521R1,         // 0x75
    ALG_ECDSA_BP256R1,           // 0x76
    ALG_ECDSA_BP384R1,           // 0x77
    ALG_ECDSA_BP512R1,           // 0x78
    ALG_ECDSA_CURVE25519,        // 0x79
    ALG_ECDSA_SECP192K1,         // 0x7A
    ALG_ECDSA_SECP224K1,         // 0x7B
    ALG_ECDSA_SECP256K1,         // 0x7C
    ALG_ECDSA_CURVE448,          // 0x7D

    ALG_ECDH = 0x80,             // 0x80
    ALG_ECDH_SECP192R1,          // 0x81
    ALG_ECDH_SECP224R1,          // 0x82
    ALG_ECDH_SECP256R1,          // 0x83
    ALG_ECDH_SECP384R1,          // 0x84
    ALG_ECDH_SECP521R1,          // 0x85
    ALG_ECDH_BP256R1,            // 0x86
    ALG_ECDH_BP384R1,            // 0x87
    ALG_ECDH_BP512R1,            // 0x88
    ALG_ECDH_CURVE25519,         // 0x89
    ALG_ECDH_SECP192K1,          // 0x8A
    ALG_ECDH_SECP224K1,          // 0x8B
    ALG_ECDH_SECP256K1,          // 0x8C

    ALG_HMAC = 0x90,             // 0x90
    ALG_HMAC_SHA_1,              // 0x91
    ALG_HMAC_SHA_224,            // 0x92
    ALG_HMAC_SHA_256,            // 0x93
    ALG_HMAC_SHA_384,            // 0x94
    ALG_HMAC_SHA_512,            // 0x95

    ALG_HKDF = 0xA0,             // 0xA0
    ALG_HKDF_SHA_1,              // 0xA1
    ALG_HKDF_SHA_224,            // 0xA2
    ALG_HKDF_SHA_256,            // 0xA3
    ALG_HKDF_SHA_384,            // 0xA4
    ALG_HKDF_SHA_512,            // 0xA5
} CryptoAlgorithm;

/**
 * \brief Supported cryptographic operation properties
 *
 */
typedef enum {
    PROP_NO_SECURITY_OPERATION   , /*!< No security operation. */

    PROP_AUTHEN_TAG_DECRYPT_DATA , /*!< Authenticate tag and decrypt data */
    PROP_AUTHEN_TAG              , /*!< Authenticate tag only */
    PROP_DECRYPT_DATA            , /*!< Decrypt data only */

    PROP_ENCRYPT_TAG_DATA        , /*!< Encrypt data and generate authenticate tag */
    PROP_ENCRYPT_TAG             , /*!< Generate authenticate tag only */
    PROP_ENCRYPT_DATA            , /*!< Encrypt data only */

    PROP_HMAC_COMPUTE            , /*!< Compute Hash-based MAC */
    PROP_HMAC_VERIFY             , /*!< Verify Hash-based MAC */

    PROP_HKDF                    , /*!< HKDF Extract-Expand */
    PROP_HKDF_EXTRACT            , /*!< HKDF Extract */
    PROP_HKDF_EXPAND             , /*!< HKDF Expand */

    PROP_SIGNATURE_SIGN          , /*!< Generate signature */
    PROP_SIGNATURE_VERIFY        , /*!< Verify signature */

    PROP_GEN_KEY_PAIR            , /*!< Generate key pair */
    PROP_GEN_SHARED_SECRET       , /*!< Compute shared secret */
} CryptoOpProperty;

typedef enum {
    RAW_PUB_KEY,
    UNCOMPRESSED_PUB_KEY,
    COMPRESSED_PUB_KEY
} EcdhPubKeyType;

typedef enum {
    KEY_LIFETIME_VOLATILE,
    KEY_LIFETIME_PERSISTENT,
} KeyLifeTime;
/**
 * \brief Cryptographic operation indicator
 *
 */
typedef struct {
    union {
        /**
         * \struct aead
         *
         * \brief Structure containing AES-CCM/AES-GCM operation parameters.
         */
        struct {
            uint32_t key_id;
            uint32_t key_len;
            uint8_t *iv;
            uint32_t iv_len;
            uint8_t *add;
            uint32_t add_len;
            uint8_t *plain_text;
            uint8_t *cipher_text;
            uint32_t text_len;
            uint8_t *tag;
            uint32_t tag_len;
        } aead;
        /**
         * \struct hkdf
         *
         * \brief Structure containing HKDF operation parameters.
         */
        struct {
            uint32_t ikm_id;    /*!< input key material id */
            uint32_t ikm_len;
            uint8_t *salt;      /*!< optional salt value (a non-secret random value) */
            uint32_t salt_len;
            uint32_t prk_id;
            uint8_t *prk;       /*!< pseudo random key (of HashLen octets) */
            uint32_t prk_len;
            uint8_t *info;      /*!< optional context and application specific information */
            uint32_t info_len;
            uint32_t okm_id;
            uint8_t *okm;       /*!< output keying material (of okm_len octets) */
            uint32_t okm_len;   /*!< length of output keying material in octets */
        } hkdf;
        /**
         * \struct cipher
         *
         * \brief Structure containing AES CBC/ECB operation parameters.
         */
        struct {
            uint32_t key_id;
            uint8_t *iv;
            uint32_t iv_len;
            uint8_t *plain_text;
            uint8_t *cipher_text;
            uint32_t text_len;
        } enc_cipher;
        /**
         * \struct ecdh
         *
         * \brief Structure containing ECDH operation parameters.
         */
        struct {
            EcdhPubKeyType pub_key_type;
            uint8_t *pub_key;
            uint32_t pub_key_id;
            uint32_t pub_key_len;
            uint32_t pri_key_id;
            uint32_t pri_key_len;
            uint8_t *secret;
            uint32_t secret_id;
            uint32_t secret_len;
        } ecdh;
        /**
         * \struct hmac
         *
         * \brief Structure containing HMAC operation parameters.
         */
        struct {
            uint8_t *key;
            uint32_t key_id;
            uint32_t key_len;
            uint8_t *idata;
            uint32_t idata_len;
            uint8_t *mac;
            uint32_t mac_id;
            uint32_t mac_len;
        } hmac;
    };
    struct {
        uint8_t import_req;
        int flag;
        KeyLifeTime lifetime;
        int type;
    } key_attr;
    uint8_t buf[MAX_CIPHER_DATA_SIZE];
    /**
      * \brief Crypto algorithm type.
      */
    CryptoAlgorithm algorithm;
    /**
      * \brief Crypto operation property.
      */
    CryptoOpProperty property;
} crypto_indicator_t;

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTO_DEFS_H_ */
