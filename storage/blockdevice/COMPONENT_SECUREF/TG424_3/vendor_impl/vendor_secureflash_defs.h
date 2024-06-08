/*
 * Copyright (c) 2020-2023 Macronix International Co. LTD. All rights reserved.
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
#ifndef _VENDOR_SECUREFLASH_DEFS_H
#define _VENDOR_SECUREFLASH_DEFS_H

#include "../JEDEC_security_HAL/jedec_security_hal.h"
#include "vendor_provisioning_impl.h"
#include "secureflash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SECURE_FLASH_MAX_ID_LEN 6

/*!
 * \struct security_feature_t
 *
 * \brief Structure to store the security features of secure Flash.
 */
typedef struct {
    uint32_t security_storage: 1, /*!< Support security storage of data */
             rpmc: 1,             /*!< Support replay protection monotonic counter */
             uid: 1,              /*!< Support unique id */
             rng: 1,              /*!< Support random number generator */
             puf: 1,              /*!< Support physical unclonable function */
             reserved: 27;        /*!< Reserved */
} security_feature_t;

/*!
 * \struct cipher_suite_t
 *
 * \brief Structure to store the cipher suites of secure Flash.
 */
typedef struct {
    uint32_t key_exchange_alg: 8,  /*!< Key exchange algorithm */
             key_derive_alg: 8,    /*!< Key derivation algorithm */
             encryption_alg: 8,    /*!< Encryption algorithm */
             signature_alg:8;      /*!< Signature algorithm */
} cipher_suite_t;

/*!
 * \struct key_size_t
 *
 * \brief Structure to store secure Flash various keys' size.
 */
typedef struct {
    uint32_t session_key_size: 16,
             private_key_size: 16;
    uint32_t public_key_size: 16,
             preshare_key_size: 16;
    uint32_t salt_key_size: 16,
             root_key_size: 16;
    uint32_t rpmc_root_key_size: 16,
             rpmc_hmac_key_size: 16;
} key_size_t;

/*!
 * \struct architecture_t
 *
 * \brief Structure to store secure Flash architecture.
 */
typedef struct {
    uint8_t erase_value;
    uint32_t secure_read_size;              /*!< Security read size in bytes */
    uint32_t secure_program_size;           /*!< Security program size in bytes */
    uint32_t secure_erase_size[4];          /*!< Security erase size in bytes */
    uint32_t regions_min_secure_erase_size; /*!< Minimum security erase size in bytes */
    uint32_t sector_size;
    uint32_t secure_zone_number;            /*!< Secure data zone number */
    uint32_t secure_zone_size;              /*!< Individual data zone size */
    uint32_t secure_zone_total_size;        /*!< Whole data zones size */
} architecture_t;

/*!
 * \struct flash_profile_t
 *
 * \brief Structure to store secure Flash profile.
 */
typedef struct {
    security_feature_t security_feature; /*!< Secure Flash security features */
    cipher_suite_t cipher_suite;         /*!< Secure Flash cipher suites */
    key_size_t key_size;                 /*!< Secure Flash keys size */
    architecture_t architecture;         /*!< Secure Flash architecture */
} flash_profile_t;


 /*!
  * \struct secure_flash_info_t
  *
  * \brief Structure to store secure Flash specific information.
  */
typedef struct {
    char *name; /*! Secure Flash name */
    uint8_t id[SECURE_FLASH_MAX_ID_LEN];              /*! Secure Flash ID */
    uint8_t id_len;                                   /*! Secure Flash ID length */
    flash_profile_t *flash_profile;
    vendor_security_op_t *vendor_security_op;         /*! Vendor specific security operations */
    vendor_provisioning_op_t *vendor_provisioning_op; /*! Vendor specific secure Flash provisioning operations */
    crypto_wrapper_t *crypto_wrapper;                 /*! Vendor specific crypto wrapper functions */
    void *vendor_ctx;                                 /*! Vendor specific context */
} secure_flash_info_t;

extern const secure_flash_info_t macronix_mx78_info;

#ifdef __cplusplus
}
#endif

#endif  /* _VENDOR_SECUREFLASH_DEFS_H */
