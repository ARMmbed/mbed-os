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
# ENTRANCE EXAM REGISTER AND VARIABLE CONSTANTS
#################################################################################
ENTRANCE_EXAM_EFUSE_ASSET_HASH_BASE_REG = 0x402C0840
ENTRANCE_EXAM_ASSET_HASH_LEN = 16
ENTRANCE_EXAM_ASSET_HASH_ZEROS = 72
ENTRANCE_EXAM_ASSET_HASH_EXPECTED_STR = "0xD9 0x1B 0x11 0xAA 0x5D 0x75 0x68 0x6A " \
                                        "0x12 0x10 0xE0 0x88 0x38 0xE1 0x2B 0x79"

ENTRANCE_EXAM_LIFECYCLE_STAGE_REG = 0x402C082b
ENTRANCE_EXAM_LIFECYCLE_STAGE_EXPECTED_VAL = 0x01

ENTRANCE_EXAM_FW_STATUS_REG = 0x08044800
ENTRANCE_EXAM_FW_STATUS_VAL = 0xF0000000
ENTRANCE_EXAM_FW_STATUS_MASK = 0xF0800000

ENTRANCE_EXAM_SRAM_ADDR = 0x0802c000
ENTRANCE_EXAM_SRAM_SIZE = 0x00004000

ENTRANCE_EXAM_REGION_HASH_ADDR = 0x10000000
ENTRANCE_EXAM_REGION_HASH_SIZE = 0x000e0000
ENTRANCE_EXAM_REGION_HASH_MODE = 255
ENTRANCE_EXAM_REGION_HASH_EXPECTED_VAL = 0x00
