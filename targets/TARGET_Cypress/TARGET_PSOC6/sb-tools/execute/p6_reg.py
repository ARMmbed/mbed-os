# Copyright 2019 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#################################################################################
# PSOC6 BLE REGISTER ADDRESSES
#################################################################################
CYREG_IPC2_STRUCT_ACQUIRE = 0x40230040  # 0x40220040 2M
CYREG_IPC2_STRUCT_NOTIFY = 0x40230048
CYREG_IPC2_STRUCT_DATA = 0x4023004C
CYREG_IPC2_STRUCT_DATA1 = 0x40230050  # for 2M only
CYREG_IPC2_STRUCT_LOCK_STATUS = 0x40230050  # 0x4023005C
CYREG_CPUSS_PROTECTION = 0x40210500  # 0x4023005C 2M

CYREG_EFUSE_FACTORY_HASH = 0x402c082c
CYREG_EFUSE_LIFECYCLE_STAGE = 0x402c082b
CYREG_EFUSE_SECURE_HASH = 0x402c0814
CYREG_EFUSE_LAST_BYTE = 0x402c0879

NVSTORE_AREA_1_ADDRESS = 0x100FB600
NVSTORE_OEM_ADDRESS = 0x100FFA00
NVSTORE_DEV_KEY_ADDRESS = 0x100FFC00
NVSTORE_UDS_ADDRESS = 0x100FFE00

#################################################################################
# PSOC6 BLE SFLASH  ADDRESSES
#################################################################################
SFLASH_TOC1_ADDR = 0x16007800
SFLASH_TOC1_LEN = 0x1C
SFLASH_TOC2_ADDR = 0x16007c00
SFLASH_TOC2_LEN = 0x30
TOC1_ROW_IDX = 60
TOC1_DUPL_ROW_IDX = 61
TOC2_ROW_IDX = 62
TOC2_DUPL_ROW_IDX = 63
TOC1_BYTE_SIZE = 0x1C  # Stored as 1st word of TOC1
MAGIC_NUMBER1 = 0x01211219  # Stored as 2nd word of TOC1
NUM_OBJECTS_TOC1 = 0x4  # Stored as 3rd word of TOC1
SFLASH_TRIM_START_ADDR = 0x16000200  # Stored as 4th word of TOC1
SFLASH_HV_PARAM_TABLE_ADDR = 0x16000400
SRAM_HV_PARAM_TABLE_ADDR = 0x08001000  # Only for *C PSoC6ABLE2 silicon
SFLASH_UNIQUEID_START_ADDR = 0x16000600  # Stored as 5th word of TOC1
FLASHBOOT_START_ADDR = 0x16002000  # Stored as 6th word of TOC1
SFLASH_SYSCALL_TABLE_PTR_ADDR = 0x16000004  # Stored as 7th word of TOC1
SFLASH_SYSCALL_TABLE_PTR = 0x16004100
SFLASH_SKIP_HASH = 0x16000008
SFLASH_FLL_CONTROL = 0x16000008
SFLASH_NORMAL_ACCESS_CTL = 0x16000008
MAGIC_NUMBER2 = 0x01211220
SFLASH_FLL_EN_IDX = 0x2

#################################################################################
# CONSTANTS AND DEFINITIONS
#################################################################################
# any random SRAM address. Fixing it to 0x2000 for all sram_scratch purposes.
# SRAM_SCRATCH = 0x08046000
# SRAM_SCRATCH_SIZE = 0x00000800
FLASH_START_ADDR = 0x10000000
FLASH_SHA256_STR_ADDR = 0x10000100
FLASH_SHA256_STR_LEN = 12
FLASH_SHA256_DIG_ADDR = 0x10000110
FLASH_SIZE = 0x200000  # 2 MB for PSoC 6A-2M
SFLASH_START_ADDR = 0x16000000
SFLASH_SIZE = 0x8000

GENERAL_TRIM_TABLE_HASH_ADDR = 0x16000200
TRIM_START_ADDR = 0x16000203
TRIM_TABLE_LEN_ADDR = 0x16000200
WFLASH_START_ADDR = 0x14000000
WFLASH_SIZE = 0x8000
SRAM_START_ADDR = 0x08000000
SRAM_SIZE = 0x48000
# SRAM_PUB_ADDR = 0x08046800  # 2M: 0x080ff400
ADDR_FMPARAMSTRUCT = 0x08000800
FLASH_ROW_SIZE = 512
# Number of trials
TRIAL_MAX = 300
SYSTEM_RESET = 0x0
ONLY_CM4_RESET = 0x1
INVALIDRESETTYPE = 0x3
R_ACCESS = 0x01
W_ACCESS = 0x02
X_ACCESS = 0x04
NIL_ACCESS = 0x00
