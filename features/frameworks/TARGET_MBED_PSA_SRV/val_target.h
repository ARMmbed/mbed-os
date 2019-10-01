/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _TARGET_INFO_DATA_H_
#define _TARGET_INFO_DATA_H_

#include "val.h"

#define TARGET_CONFIG_CREATE_ID(major, minor, index) \
                    (((major & 0xFF) << 24) | ((minor & 0xFF) << 16) | (index & 0xFFFF))
#define TARGET_CONFIG_GET_MAJOR(config_id) ((config_id >> 24) & 0xFF)
#define TARGET_CONFIG_GET_MINOR(config_id) ((config_id >> 16) & 0xFF)
#define TARGET_CONFIG_INCREMENT_INDEX(config_id) \
                    ((config_id & 0xFFFF0000) | ((config_id & 0xFFFF) + 1))
#define GET_NUM_INSTANCE(struct_type)   (struct_type->cfg_type.size >> 24)
#define VAL_TEST_MAJOR_GROUP_MASK      0xFF000000UL
#define VAL_TEST_MINOR_GROUP_MASK      0x00FF0000UL
#define VAL_TEST_CFG_INSTANCE_MASK     0x0000FFFFUL
#define VAL_TEST_INVALID_CFG_ID        0xFFFFFFFFUL
#define TARGET_MIN_CFG_ID TARGET_CONFIG_CREATE_ID(GROUP_SOC_PERIPHERAL, 0, 0)
#define TARGET_MAX_CFG_ID TARGET_CONFIG_CREATE_ID(GROUP_MAX, 0, 0)

/**
    Config IDs for each group/component
    31:24 : MAJOR (group)
    23:16 : MINOR (component)
    16:8  : SUB-component
    7:0  : INSTANCE (instance of same component)
**/

/*
   MAJOR IDs
*/
typedef enum _GROUP_CONFIG_ID_ {
  GROUP_SOC_PERIPHERAL      = 0x1,
  GROUP_MEMORY              = 0x2,
  GROUP_MISCELLANEOUS       = 0x3,
  GROUP_MAX                 = 0xFF,
} group_cfg_id_t;

/*
   MINOR IDs
 */
typedef enum _SOC_PERIPHERAL_CONFIG_ID_ {
  SOC_PERIPHERAL_UART = 0x1,
  SOC_PERIPHERAL_TIMER = 0x2,
  SOC_PERIPHERAL_WATCHDOG = 0x3,
} soc_peripheral_cfg_id_t;

typedef enum _MEMORY_CONFIG_ID_ {
  MEMORY_NVMEM                    = 0x2,
  MEMORY_NSPE_MMIO                = 0x3,
  MEMORY_CLIENT_PARTITION_MMIO    = 0x4,
  MEMORY_DRIVER_PARTITION_MMIO    = 0x5,
} memory_cfg_id_t;

typedef enum _MISCELLANEOUS_CONFIG_ID_ {
  MISCELLANEOUS_BOOT         = 0x1,
  MISCELLANEOUS_DUT          = 0x2
} miscellaneous_cfg_id_t;

/**
  Assign group type to each system component
**/
typedef enum _COMPONENT_GROUPING_{
  UART                     = GROUP_SOC_PERIPHERAL,
  TIMER                    = GROUP_SOC_PERIPHERAL,
  WATCHDOG                 = GROUP_SOC_PERIPHERAL,
  NVMEM                    = GROUP_MEMORY,
  NSPE_MMIO                = GROUP_MEMORY,
  CLIENT_PARTITION_MMIO    = GROUP_MEMORY,
  DRIVER_PARTITION_MMIO    = GROUP_MEMORY,
  BOOT                     = GROUP_MISCELLANEOUS,
  DUT                      = GROUP_MISCELLANEOUS,
} comp_group_assign_t;

/**
  Target Configuration Header
**/
typedef struct _TARGET_CFG_HDR_ {
    /* PSA_CFG */
    uint32_t  signature[2];
    /* 8 byte String describing the Target platform */
    uint32_t  target_string[2];
    /* version = 1 for now */
    uint32_t  version;
    /* Header Size */
    uint32_t  size;
}target_cfg_hdr_t;

typedef enum {
    LEVEL1 = 0x1,
    LEVEL2,
    LEVEL3,
} firmware_level_t;

typedef enum {
    NOT_AVAILABLE   = 0x0,
    AVAILABLE       = 0x1,
} is_available_t;

typedef enum {
    SECURE_ACCESS           = 0x100,
    NONSECURE_ACCESS,
    SECURE_PROGRAMMABLE,
    NONSECURE_PROGRAMMABLE
} dev_attr_t;

typedef enum {
    MEM_SECURE      = 0x100,
    MEM_NONSECURE,
    MEM_NSC,
} mem_tgt_attr_t;

typedef enum {
    TYPE_READ_ONLY      = 0x10,
    TYPE_WRITE_ONLY,
    TYPE_READ_WRITE,
    TYPE_EXECUTE,
    TYPE_RESERVED,
} perm_type_t;

typedef struct _CFG_HDR_TYPE_ {
    cfg_id_t cfg_id;
    /* size inclusive of this header */
    uint32_t size;
} cfg_type_t;

/**
   Memory Information
**/
typedef struct _MEM_INFO_DESC_ {
    cfg_type_t  cfg_type;
    uint32_t    num;
} memory_hdr_t;

typedef struct _MEM_REGION_ {
  cfg_type_t cfg_type;
  addr_t     start;
  addr_t     end;
  mem_tgt_attr_t attribute;
  perm_type_t permission;
} memory_desc_t;

/*
   SOC Peripheral description structures
*/
typedef struct _SOC_PER_INFO_NUM_ {
    cfg_type_t cfg_type;
    uint32_t   num;
} soc_peripheral_hdr_t;

typedef struct _SOC_PER_INFO_DESC_ {
  cfg_type_t  cfg_type;
  uint32_t    vendor_id;
  uint32_t    device_id;
  addr_t      base;
  uint32_t    size;
  uint32_t    intr_id;
  perm_type_t permission;
  uint32_t    timeout_in_micro_sec_low;
  uint32_t    timeout_in_micro_sec_medium;
  uint32_t    timeout_in_micro_sec_high;
  uint32_t    timeout_in_micro_sec_crypto;
  uint32_t    num_of_tick_per_micro_sec;
  dev_attr_t  attribute;
} soc_peripheral_desc_t;

/**
  System Miscellaneous Information
**/

typedef struct _MISCELLANEOUS_INFO_HDR_ {
    cfg_type_t cfg_type;
    uint32_t   num;
} miscellaneous_hdr_t;

typedef struct _MISCELLANEOUS_INFO_DESC_ {
    cfg_type_t              cfg_type;
    firmware_level_t        implemented_psa_firmware_isolation_level;
    addr_t                  ns_start_addr_of_combine_test_binary;
    is_available_t          combine_test_binary_in_ram;
    addr_t                  ns_test_addr;
} miscellaneous_desc_t;

/*val target config read apis */
STATIC_DECLARE val_status_t val_target_get_config(cfg_id_t cfg_id, uint8_t **data, uint32_t *size);
STATIC_DECLARE val_status_t val_target_cfg_get_next(void **blob);
STATIC_DECLARE val_status_t val_target_get_cfg_blob(cfg_id_t cfg_id, uint8_t **data, uint32_t *size);
STATIC_DECLARE val_status_t val_target_get_config(cfg_id_t cfg_id, uint8_t **data, uint32_t *size);
#endif
