/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

#include "flash_api.h"

#if DEVICE_FLASH

#include <string.h>
#include "flash_data.h"
#include "mbed_critical.h"
#include "partition_M2354.h"

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#include "device/partition_M2354_mem.h"

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
// NOTE: On ARMv7-M/ARMv8-M, instruction fetches are always little-endian.
static uint32_t FLASH_ALGO[] = {
    0x9002b084, 0x92009101, 0x4820e7ff, 0x07c06800, 0xd0012800, 0xe7f8e7ff, 0x6801481d, 0x43112240, 
    0x98026001, 0x6008491b, 0x491b9801, 0x98006008, 0xd1052800, 0x2000e7ff, 0x491843c0, 0xe0036008, 
    0x49174816, 0xe7ff6001, 0x21014816, 0xf3bf6001, 0xe7ff8f6f, 0x6800480d, 0x280007c0, 0xe7ffd001, 
    0x480be7f8, 0x06406800, 0xd5082800, 0x4808e7ff, 0x22406801, 0x60014311, 0x90032001, 0x2000e002, 
    0xe7ff9003, 0xb0049803, 0x46c04770, 0x4000c040, 0x4000c000, 0x4000c00c, 0x4000c004, 0x4000c008, 
    0x0055aa03, 0x4000c010, 0x9003b085, 0x92019102, 0x21594819, 0x21166001, 0x21886001, 0x68006001, 
    0x280007c0, 0xe7ffd103, 0x90042001, 0x4813e020, 0x22046801, 0x60014311, 0x68014811, 0x60014311, 
    0x4810e7ff, 0x06c06800, 0xd4012800, 0xe7f8e7ff, 0x6801480d, 0x43112229, 0x68006001, 0x28294010, 
    0xe7ffd003, 0x90042001, 0x2000e002, 0xe7ff9004, 0xb0059804, 0x46c04770, 0x40000100, 0x40000200, 
    0x40000204, 0x40000250, 0x4000c000, 0x9000b081, 0x4807e7ff, 0x07c06800, 0xd0012800, 0xe7f8e7ff, 
    0x68014804, 0x43912201, 0x20006001, 0x4770b001, 0x4000c040, 0x4000c000, 0xb084b580, 0x6800480c, 
    0x40082107, 0x98029002, 0xd3032802, 0x2001e7ff, 0xe00a9003, 0x21014807, 0x20266001, 0x91012100, 
    0xf7ff9a01, 0x9003ff3d, 0x9803e7ff, 0xbd80b004, 0x4002f704, 0x4000c01c, 0xb082b580, 0x98019001, 
    0x070a2101, 0x90014390, 0x4a0e9801, 0x90014010, 0x220f9801, 0x40100512, 0x42880549, 0xe7ffd107, 
    0x49099801, 0x90011840, 0x90002001, 0x2000e002, 0xe7ff9000, 0x9a009901, 0xf7ff2022, 0xb002ff11, 
    0x46c0bd80, 0xfffff800, 0xffe00000, 0xb086b580, 0x91039004, 0x90022000, 0x9802e7ff, 0x42889903, 
    0xe7ffd211, 0x99029804, 0x92021c4a, 0x58400089, 0xffc2f7ff, 0x98019001, 0xd0032800, 0x9801e7ff, 
    0xe0039005, 0x2000e7e9, 0xe7ff9005, 0xb0069805, 0x46c0bd80, 0xb084b580, 0x91019002, 0x98019200, 
    0x21031cc0, 0x90014388, 0x21019802, 0x43880709, 0xe7ff9002, 0x68004823, 0x280007c0, 0xe7ffd001, 
    0x4821e7f8, 0x22406801, 0x60014311, 0x2121481f, 0xe7ff6001, 0x28009801, 0xe7ffd02d, 0x491c9802, 
    0x98006008, 0xf83af000, 0x6008491a, 0x2101481a, 0xf3bf6001, 0xe7ff8f6f, 0x68004812, 0x280007c0, 
    0xe7ffd001, 0x4810e7f8, 0x06406800, 0xd5082800, 0x480de7ff, 0x22406801, 0x60014311, 0x90032001, 
    0x9802e00c, 0x90021d00, 0x1d009800, 0x98019000, 0x90011f00, 0x2000e7ce, 0xe7ff9003, 0xb0049803, 
    0x46c0bd80, 0x4000c040, 0x4000c000, 0x4000c00c, 0x4000c004, 0x4000c008, 0x4000c010, 0x9001b082, 
    0x1c419801, 0x78009101, 0x98019000, 0x91011c41, 0x02007800, 0x18089900, 0x98019000, 0x91011c41, 
    0x04007800, 0x18089900, 0x98019000, 0x91011c41, 0x06007800, 0x18089900, 0x98009000, 0x4770b002, 
    0xb086b580, 0x91049005, 0x98049203, 0x21031cc0, 0x90044388, 0x21019805, 0x40080709, 0x98059002, 
    0x90054388, 0x4825e7ff, 0x07c06800, 0xd0012800, 0xe7f8e7ff, 0x68014822, 0x43112240, 0x48216001, 
    0x60012100, 0x9804e7ff, 0xd0322800, 0x9805e7ff, 0x6008491d, 0x2101481d, 0xf3bf6001, 0xe7ff8f6f, 
    0x68004816, 0x280007c0, 0xe7ffd001, 0x4814e7f8, 0x06406800, 0xd5062800, 0x4811e7ff, 0x22406801, 
    0x60014311, 0x4812e015, 0x99036800, 0x46089001, 0xff94f7ff, 0x42819901, 0xe7ffd001, 0x9805e009, 
    0x90051d00, 0x1d009803, 0x98049003, 0x90041f00, 0x9805e7c9, 0x43089902, 0xbd80b006, 0x4000c040, 
    0x4000c000, 0x4000c00c, 0x4000c004, 0x4000c010, 0x4000c008, 0x00000000, 
};

static const flash_algo_t flash_algo_config = {
    .init = 0x000000a9,
    .uninit = 0x0000012d,
    .erase_sector = 0x00000199,
    .program_page = 0x00000235,
    .static_base = 0x00000414,
    .algo_blob = FLASH_ALGO
};

/* Secure flash */
static const sector_info_t sectors_info[] = {
    {NU_FLASHIAP_SECURE_START, 0x800},                      // (start, sector size)
};

/* Secure flash */
static const flash_target_config_t flash_target_config = {
    .page_size  = 4,                                        // 4 bytes
                                                            // Here page_size is program unit, which is different
                                                            // than FMC definition.
    .flash_start = NU_FLASHIAP_SECURE_START,
    .flash_size = NU_FLASHIAP_SECURE_SIZE,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

/* Non-secure flash */
static const sector_info_t sectors_info_ns[] = {
    {NU_FLASHIAP_NONSECURE_START, 0x800},                   // (start, sector size)
};

/* Non-secure flash */
static const flash_target_config_t flash_target_config_ns = {
    .page_size  = 4,                                        // 4 bytes
                                                            // Here page_size is program unit, which is different
                                                            // than FMC definition.
    .flash_start = NU_FLASHIAP_NONSECURE_START,
    .flash_size = NU_FLASHIAP_NONSECURE_SIZE,
    .sectors = sectors_info_ns,
    .sector_info_count = sizeof(sectors_info_ns) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
    obj->target_config_ns = &flash_target_config_ns;
}

#endif  // #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#endif  // #if DEVICE_FLASH
