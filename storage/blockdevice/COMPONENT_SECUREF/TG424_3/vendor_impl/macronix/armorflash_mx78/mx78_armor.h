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
#ifndef _MX78_ARMOR_H_
#define _MX78_ARMOR_H_

#include <stdint.h>
#include "../../../JEDEC_security_HAL/vendor_security_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SF_WARN_PR
#define SF_DBG_PR
#define SF_DBG0_PR
#define SF_INFO_PR
#define SF_ERR_PR
#define SF_TMP_PR

#define SESSION_STATUS_EXHAUST  0x00
#define SESSION_STATUS_ABUNDANT 0x01
#define SESSION_STATUS_EMPTY    0x02
#define MAX_NONCE_SIZE          0x10
#define MAC_SIZE                0x20
#define MAX_RESP_SIZE           0x150
#define SCRATCHPAD_SIZE         0x150
/* Extended configure registers number */
#define EX_CR_NUM               0x06
/* The max number of concurrent operations */
#define CONC_OPER_NUM           0x04
/* Data region configure size */
#define CFG_SIZE_PER_REGION     0x04
#define INVALID_SESSION_KEY_ID  (0)
/**
 * MX78 ArmorFlash supported security operations definition
 *
 */
typedef enum {
    SECURITY_READ,       /*!< Security read */
    SECURITY_WRITE,      /*!< Security program */
    SECURITY_ERASE,      /*!< Security erase */
    MC_INCREASEMENT,     /*!< Increase Monotonic counter */
    MC_READ,             /*!< Read Monotonic counter */
    GENERATE_NONCE,      /*!< Generate nonce for cryptographic operations */
    READ_SESSION_STATUS, /*!< Read sessions status */
    CREATE_SESSION,      /*!< Create new sessions */
    CONFIRM_SESSION,     /*!< Session confirmation */
    TERMINATE_SESSION,   /*!< Session termination */
    GEN_PRIV_KEY,        /*!< Generate private key */
    GEN_PUB_KEY,         /*!< Generate public key */
    EXPORT_PUB_KEY,      /*!< Export public key */
    SYNC_HOST_PUB_KEY,   /*!< Synchronize host public key */
    SYNC_SALT,           /*!< Synchronize salt */
    GEN_ROOT_KEY,        /*!< Generate root key */
    SECURITY_GET_REGION_CONFIG,
    GET_CFG,             /*!< Read configuration information */
    SET_CFG,
    CFG_SECURITY_PROFILE,/*!< Configure security profile */
    SECURE_INIT,
    SECURE_UNINIT,
    HEALTH_TEST,
    LOCK_DOWN            /*!< Lock down */
} mx78_armor_operation_type_t;

enum {
    OPER_NOT_IN_USE = 0,
    OPER_IN_USE = 1,
};

#define PACKET_OUT 0
#define PACKET_IN 1

typedef enum {
    ITEM_DATA_CFG,
    ITEM_MC_CFG,
    ITEM_KEY_CFG,
    ITEM_LKD_CFG,
    ITEM_TOTAL_CFG,
    ITEM_MC,
    ITEM_KEY,
    ITEM_EXTRA,
} mx78_armor_security_item_t;

/**
 * \struct mx78_armor_security_operation_t
 *
 * \brief The structure holding MX78 ArmorFlash security operations' context.
 */
typedef struct {
    uint32_t in_use;
    mx78_armor_operation_type_t type; /*!< Security operation type */
    uint8_t *in_data;                 /*!< Pointer to current security operation input data */
    uint32_t in_size;                 /*!< Input data size in bytes */
    uint8_t *out_data;                /*!< Pointer to current security operation output data */
    uint32_t out_size;                /*!< Output data size in bytes */
    uint32_t addr;                    /*!< The access address of current security operation */
    union {
        uint8_t mc;                   /*!< Current security operation linked monotonic counter id */
        uint8_t profile;              /*!< Current security operation linked security profile id */
        uint8_t region;               /*!< Current security operation linked region id */
        uint32_t root_key;            /*!< Current security operation linked root key id */
    } id;
    uint32_t crypto_key_id;           /*!< Current security operation linked crypto service key id */
} mx78_armor_security_operation_t;


typedef struct {
    uint8_t status_register;
    uint8_t configure_register;
    uint8_t configure_register_extend[EX_CR_NUM];//cr2
    uint8_t secure_register;
} registers_t;


typedef struct {
    const char *name;
    registers_t regs;
    uint8_t scratchpad[SCRATCHPAD_SIZE];
} mx78_armor_context;

#ifdef __cplusplus
}
#endif

#endif /* _MX78_ARMOR_H_ */
