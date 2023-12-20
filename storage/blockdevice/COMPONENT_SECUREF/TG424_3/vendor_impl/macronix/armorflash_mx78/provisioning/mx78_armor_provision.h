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
#ifndef _MX78_ARMOR_PROVISION_H
#define _MX78_ARMOR_PROVISION_H

#include <stddef.h>
#include <stdint.h>
#include "../mx78_armor.h"

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * \file mx78_armor_provision.h
 *
 * \brief This file describes the structures of mx78 series secure Flash
 *        provisioning data template
 */

#define PROVISION_INFO_SIZE   0x80
/**The template structure of provisioning data blob
-----------------------------------------------------------
|                         Magic                            |
-----------------------------------------------------------
|Sub header num    |Sub header size     |    Version       |
-----------------------------------------------------------
|Provision_disable | ID2          |ID1        |ID0         |
|   Reserved         |   Total size                        |
-----------------------------------------------------------
|              Sub header 1:application info               |  /// app_info id; app_info  num; each app_info size; app_info storage flag
-----------------------------------------------------------
|              Detailed application info                   |  /// app_info id;
|                     description                          |      [app_id0, zone_id0, root_key_id0];[app_id1, zone_id1, root_key_id1]
-----------------------------------------------------------
|              Sub header 2:key derivation info            |  /// key_info id; key_info  num; each key_info size; key_info storage flag
-----------------------------------------------------------
|              Detailed key derivation info                |  /// key_info id; key exchange type: 00:send key directly to the other party
|                     description                          |                                      01:DH
-----------------------------------------------------------
|              Sub header 3:counter info                   |
-----------------------------------------------------------
|              Detailed counter info                       |  /// counter_info id;
|                     description                          |      [counter_id0, counter_init_value0];[counter_id1, counter_init_value1]
-----------------------------------------------------------
|              Sub header 4:configure info                 |  /// configure_info id; configure_info  num; each configure_info size; configure_info storage flag
-----------------------------------------------------------
|              Detailed configure info                     |  /// configure_info id;
|                     description                          |
-----------------------------------------------------------
|              Sub header 5:lock info                      |  /// lock_info id; lock_info num; each lock_info size; lock_info storage flag
-----------------------------------------------------------
|              Detailed lock info                          |  /// lock_info id;
|                     description                          |
-----------------------------------------------------------
As it doesn't need to store the whole provisioning data, the size of
provisioning data to be stored in memory is less than received provisioning data
blob's size.
The template structure of provisioning data in memory is similar to above
provisioning data blob's structure.

The template structure of provisioning data stored in memory
-----------------------------------------------------------
|                         Magic                            |
-----------------------------------------------------------
|Sub header stored num |Sub header size |    Version       |
-----------------------------------------------------------
|Provision_disable | ID2          |ID1        |ID0         |
|   Reserved         |   Total stored size                 |
-----------------------------------------------------------
|              Sub header 1:application info               |  /// app_info id; app_info  num; each app_info size; app_info storage flag
-----------------------------------------------------------
|              Detailed application info                   |  /// app_info id;
|                     description                          |      [app_id0, zone_id0, root_key_id0];[app_id1, zone_id1, root_key_id1]
-----------------------------------------------------------
|              Sub header 2:key derivation info            |  /// key_info id; key_info  num; each key_info size; key_info storage flag
-----------------------------------------------------------
|              Detailed key derivation info                |  /// key_info id; key exchange type: 00:send key directly to the other party
|                     description                          |                                      01:DH
-----------------------------------------------------------
|              Sub header 3:lock info                      |  /// lock_info id; lock_info num; each lock_info size; lock_info storage flag
-----------------------------------------------------------
|              Detailed lock info                          |  /// lock_info id;
|                     description                          |      lock_type:counter/datazone/key/...
-----------------------------------------------------------
*/

#define ARMOR_APP_INFO_MAX_NUM 16
#define ARMOR_LKD_INFO_MAX_NUM 8
#define KEY_INFO_MAX_NUM 16
#define MC_INFO_MAX_NUM 16
#define MC_MAX_SIZE 4
#define CFG_INFO_MAX_NUM 0x60

/**
 * Sub items' header id of ArmorFlash provisioning information.
 */
typedef enum {
    SUB_ID_DATA_CONFIG_INFO = 0,
    SUB_ID_KEY_CONFIG_INFO = 1,
    SUB_ID_CNT_CONFIG_INFO = 2,
    SUB_ID_APP_INFO = 3,
    SUB_ID_KEY_INFO = 4,
    SUB_ID_MC_INFO = 5,
    SUB_ID_LOCK_INFO = 6,
    SUB_ID_MAX_NUMBER,
} SubHeaderId;


/**
 * \struct provision_sub_item_header_t
 *
 * \brief Structure holding each sub item's header of ArmorFlash provisioning
 *        information.
 */
typedef struct {
    uint32_t id:         8, /*!< Sub item's header id */
             num:        8, /*!< Sub item's number */
             size:      15, /*!< Sub item's size */
             store:      1; /*!< Sub item's storage flag */
} provision_sub_item_header_t;


/**
 * \struct provision_major_header_t
 *
 * \brief Structure holding the major header of ArmorFlash provisioning
 *        information.
 */
typedef struct {
        uint8_t magic[4];                /*!< The magic bytes of this
                                              provisioning information */
        uint32_t minor_version: 4,       /*!< The version of this provisioning information */
                 major_version: 4,
                 sub_header_num: 8,      /*!< The number of this provisioning
                                              information's sub headers */
                 total_size: 16;         /*!< The total size of this
                                              provisioning information */
        uint8_t id[4];                   /*!< The secure Flash id */
} provision_major_header_t;

#define SFPI_MAJOR_HEADER_SIZE (sizeof(provision_major_header_t))
#define SFPI_SUB_HEADER_SIZE   (sizeof(provision_sub_item_header_t))

/**
 * \struct mx_app_data_t
 *
 * \brief Structure to store a certain application item's
 *        provisioning information.
 */
typedef struct {
    uint32_t app_id;    /*!< Application id */
    uint32_t key_id;    /*!< Application binded crypto key id */
    uint32_t zone_id:8, /*!< Application binded security zone id */
             mc_id:8,   /*!< Application binded monotonic counter id */
             reserved:16;
} mx_app_data_t;
/**
 * \struct mx_app_info_t
 *
 * \brief Structure holding provisioning information for applications.
 */
typedef struct {
	provision_sub_item_header_t header;
    mx_app_data_t app_data[ARMOR_APP_INFO_MAX_NUM]; /*!< Buffer holding
                                                         application items'
                                                         provisioning
                                                         information */
} mx_app_info_t;

/**  provision for lock_info
 *
 *   DWORD 0: [07:00] ID, [31:08] reserved
 *   DWORD 1: [07:00] config_regs, [15:08] ind_key_lock,
 *            [23:16] ind_key_dis, [31:24] reserved
 *   DWORD 2: [15:00] datazone lock(bitwise), [31:16] provision (bitwise)
 **/

/**
 * \struct lock_info_t
 *
 * \brief Structure holding provisioning information for
 *        ArmorFlash's lock down configuration.
 */
typedef struct {
    provision_sub_item_header_t header;
    uint8_t lock_data[ARMOR_LKD_INFO_MAX_NUM]; /*!< Buffer holding lock
                                                    down information */
} lock_info_t;

/** provision for key_info
 *
 *   DWORD 0: [07:00] ID, [15:08] key number, [31:16] Reserved
 *   key 0
 *     DWORD 1: [31:00] key id
 *     DWORD 2: [31:00] key derive message
 *     DWORD 3: [07:00] key derive params suite, [23:08] key length (in bytes),
 *              [31:24] key inject type
 *   key 1 ~ (key number -1)
 **/
/**
 * \struct key_data_t
 *
 * \brief Structure holding provisioning information used to derive a
 *        certain root key of ArmorFlash.
 */
typedef struct {
    uint32_t key_id;                /*!< Root key id */
    uint32_t derive_message;        /*!< Specific information for root key
                                         derivation */
    uint32_t key_derive_suite: 8,   /*!< Key derivation cipher algorithm */
             key_exchange_type: 4,  /*!< Key exchange type: DH or directly share to the other party */
             key_len: 8,            /*!< Derived root key length in bytes */
             zone_id: 8,            /*!< Corresponding */
             inject_type: 4;        /*!< The mode of synchronizing ArmorFlash's
                                         root key */
} key_data_t;
/**
 * \struct key_info_t
 *
 * \brief Structure holding provisioning information for ArmorFlash root keys
 *        derivation.
 */
typedef struct {
    provision_sub_item_header_t header;
    key_data_t key_data[KEY_INFO_MAX_NUM]; /*!< Buffer holding root keys'
                                                provisioning information */
} key_info_t;

/** provision for mc_info
 *
 *  DWORD 0: [07:00] ID, [15:08] mc number, [31:16] reserved
 *  mc 0
 *  DWORD 1: [31:00] mc value
 *  mc 1 ~ (mc number -1)
 **/
typedef struct {
    uint8_t value[MC_MAX_SIZE]; // power of two, '0' is not supported
} mc_data_t;
/**
 * \struct mc_info_t
 *
 * \brief Structure holding ArmorFlash monotonic counters' provisioning
 *        information.
 */
typedef struct {
    provision_sub_item_header_t header;
    mc_data_t mc_data[MC_INFO_MAX_NUM]; /*!< Buffer holding monotonic counters'
                                             provisioning information */
} mc_info_t;


/**
 * \struct config_info_t
 *
 * \brief Structure holding provisioning information used to configure ArmorFlash.
 */
typedef struct {
	provision_sub_item_header_t header;
    uint8_t config_data[CFG_INFO_MAX_NUM]; /*!< Buffer holding ArmorFlash
                                                configuration' provisioning
                                                information */
} config_info_t;

int32_t mx78_armor_provision_perform(mx78_armor_context *mx78_armor_ctx,
                                     uint8_t *provision_data_blob,
                                     uint32_t data_size,
                                     uint8_t *provision_data_buf,
                                     uint32_t *data_store_size);
#ifdef __cplusplus
}
#endif

#endif /* _MX78_ARMOR_PROVISION_H */
