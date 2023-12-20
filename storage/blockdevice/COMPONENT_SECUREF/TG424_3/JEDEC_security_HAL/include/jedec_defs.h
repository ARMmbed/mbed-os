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
#ifndef _JEDEC_DEFS_H_
#define _JEDEC_DEFS_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* secure read is supported */
#define SECURE_READ
/* fixme: These macro could be defined in jedec_config.cmake file */
#define PACKET_MAX_LEN    0x400
#define MAX_RANDOM_SIZE   0x20
#define MAX_REGION_NUM    0x10

/* Persistent key ids */
/* fixme: 0X55AA55 is just a example key id */
#define SECUREFLASH_AUTHEN_KEY_ID 0x55AA55 /*!< Authentication key id for secure init/uninit */
/**
 * \brief Region access restriction
 *
 */
typedef enum {
    NONE_PROC, /*!< no access restriction */
    WR_PROC,   /*!< write protection */
    RD_PROC,   /*!< read protection */
    RW_PROC,   /*!< read&write protection */
    RD_ONLY,   /*!< readonly, write is not allowed */
} access_restr_t;

/**
 * \brief Region protection type
 *
 */
typedef enum {
    LOCK,        /*!< need unlock before read/write */
    CONTINUOUS,  /*!< need sign each read/write command */
} proc_type_t;

/**
 * \brief region attributes
 *
 */
typedef struct secure_region_attributes {
    uint32_t address;     /*!< region start address */
    uint32_t length;      /*!< region size */
    uint32_t root_key_id; /*!< region root key id */
    uint8_t auth_algo;    /*!< region authentication algorithm */
    uint8_t encr_algo;    /*!< region encryption algorithm */
    uint8_t access_restr; /*!< region access restriction */
    uint8_t prot_type;    /*!< region protection type */
} secure_region_attributes_t;

/**
 * \brief region link list node
 *
 */
typedef struct region_ll_node{
    secure_region_attributes_t attributes; /*!< region attributes */
    uint32_t session_key_id;               /*!< region session key id */
    struct region_ll_node *next;           /*!< pointer to next node */
} region_ll_node_t;

#ifdef __cplusplus
}
#endif

#endif /* _JEDEC_DEFS_H_ */
