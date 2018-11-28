/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed_critical.h"

// This file is automagically generated

#if DEVICE_FLASH

#include "flash_data.h"
#include "partition_ARMCM33.h"
#include "fsl_iap.h"

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#define SECURE_FLASH_START_ADDR    0x10000000

static flash_config_t flash_config;

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
static uint32_t FLASH_ALGO[] = {
    0xf240b580, 0xf2c0000c, 0x44480000, 0xf862f000, 0xbf182800, 0xbd802001, 0x47702000, 0xf240b510,
    0xf2c0040c, 0xf2460400, 0xeb09636c, 0xf6c60004, 0x21003365, 0x2218f44f, 0xf86ef000, 0xbf182800,
    0xeb09bd10, 0x21000004, 0x2218f44f, 0x4010e8bd, 0xb8b2f000, 0xf240b5b0, 0xf2c0050c, 0xeb090500,
    0x690a0105, 0xf2464604, 0xf6c6636c, 0x46083365, 0xf0004621, 0x2800f851, 0xbdb0bf18, 0x0005eb09,
    0x46216902, 0x40b0e8bd, 0xb896f000, 0xb085b5f0, 0x070cf240, 0x4606460d, 0x0700f2c0, 0x0007eb09,
    0x462b4631, 0xf0004614, 0xb108f85f, 0xbdf0b005, 0xf10da804, 0x90000c0c, 0x0007eb09, 0x462a4631,
    0xf8cd4623, 0xf000c004, 0xb005f88d, 0x0000bdf0, 0xe7fdbe00, 0x1100f241, 0x3100f2c1, 0x29006809,
    0x2260d00c, 0x680a6282, 0x0308f240, 0x2207f3c2, 0x0300f2c0, 0x2003f849, 0x47086849, 0x3048f240,
    0x0000f2c0, 0x3155f240, 0x0100f2c0, 0x44794478, 0xf7ff2273, 0xbf00ffdd, 0x0c08f240, 0x0c00f2c0,
    0xc00cf859, 0x0f00f1bc, 0xf241d00b, 0xf2c11c00, 0xf8dc3c00, 0xf1bcc000, 0xd0070f00, 0xc008f8dc,
    0xf2444760, 0xf2c11c3b, 0x47603c00, 0x20f8f240, 0x0000f2c0, 0x3105f240, 0x0100f2c0, 0x44794478,
    0xf7ff2284, 0xbf00ffb5, 0x0c08f240, 0x0c00f2c0, 0xc00cf859, 0x0f00f1bc, 0xf241d00b, 0xf2c11c00,
    0xf8dc3c00, 0xf1bcc000, 0xd0070f00, 0xc00cf8dc, 0xf2444760, 0xf2c11c9d, 0x47603c00, 0x20a8f240,
    0x0000f2c0, 0x21b5f240, 0x0100f2c0, 0x44794478, 0xf7ff2294, 0xbf00ff8d, 0x1300f241, 0x3300f2c1,
    0x2b00681b, 0x691bd001, 0xf2404718, 0xf2c0207a, 0xf2400000, 0xf2c02187, 0x44780100, 0x229c4479,
    0xff76f7ff, 0x0c08f240, 0x0c00f2c0, 0xc00cf859, 0x0f00f1bc, 0xf241d00b, 0xf2c11c00, 0xf8dc3c00,
    0xf1bcc000, 0xd0070f00, 0xc014f8dc, 0xf2444760, 0xf2c12c7d, 0x47603c00, 0x202cf240, 0x0000f2c0,
    0x2139f240, 0x0100f2c0, 0x44794478, 0xf7ff22b0, 0xbf00ff4f, 0x1300f241, 0x3300f2c1, 0x2b00681b,
    0x699bd001, 0xf2404718, 0xf2c010fe, 0xf2400000, 0xf2c0210b, 0x44780100, 0x22b94479, 0xff38f7ff,
    0x1100f241, 0x3100f2c1, 0x29006809, 0x69c9d001, 0xf2404708, 0xf2c010d2, 0xf2400000, 0xf2c011df,
    0x44780100, 0x22c34479, 0xff22f7ff, 0x1100f241, 0x3100f2c1, 0x29006809, 0x6a09d001, 0xf2404708,
    0xf2c010a6, 0xf2400000, 0xf2c011b3, 0x44780100, 0x22ca4479, 0xff0cf7ff, 0x1100f241, 0x3100f2c1,
    0x29006809, 0xf644d004, 0xf2c11151, 0x47083100, 0x1074f240, 0x0000f2c0, 0x1181f240, 0x0100f2c0,
    0x44794478, 0xf7ff22d0, 0xbf00fef3, 0x230bf644, 0x3300f2c1, 0xbf004718, 0x1300f241, 0x3300f2c1,
    0x2b00681b, 0x6a5bd001, 0xf2404718, 0xf2c0103a, 0xf2400000, 0xf2c01147, 0x44780100, 0x22de4479,
    0xfed6f7ff, 0x1c00f241, 0x3c00f2c1, 0xc000f8dc, 0x0f00f1bc, 0xf8dcd002, 0x4760c02c, 0x1008f240,
    0x0000f2c0, 0x1115f240, 0x0100f2c0, 0x44794478, 0xf7ff22e5, 0xbf00febd, 0x1200f241, 0x3200f2c1,
    0x2a006812, 0x6b12d001, 0xf2404710, 0xf2c000da, 0xf2400000, 0xf2c001e7, 0x44780100, 0x22ec4479,
    0xfea6f7ff, 0x1200f241, 0x3200f2c1, 0x2a006812, 0x6b52d001, 0xf2404710, 0xf2c000ae, 0xf2400000,
    0xf2c001bb, 0x44780100, 0x22f34479, 0xfe90f7ff, 0x1300f241, 0x3300f2c1, 0x2b00681b, 0x6b9bd001,
    0xf2404718, 0xf2c00082, 0xf2400000, 0xf2c0018f, 0x44780100, 0x22fa4479, 0xfe7af7ff, 0x5cb3f644,
    0x3c00f2c1, 0xbf004760, 0x6c15f644, 0x3c00f2c1, 0xbf004760, 0x1200f241, 0x3200f2c1, 0x2a006812,
    0x6a92d001, 0xf2404710, 0xf2c0003e, 0xf2400000, 0xf2c0014b, 0x44780100, 0xf44f4479, 0xf7ff7287,
    0xbf00fe57, 0x1c00f241, 0x3c00f2c1, 0xc000f8dc, 0x0f00f1bc, 0xf8dcd002, 0x4760c040, 0x0008f240,
    0x0000f2c0, 0x0115f240, 0x0100f2c0, 0x44794478, 0x1215f240, 0xfe3cf7ff, 0x53414c46, 0x50415f48,
    0x52545f49, 0x2e004545, 0x2e2e2f2e, 0x2f2e2e2f, 0x72756f73, 0x6e2f6563, 0x6c2f7078, 0x35356370,
    0x2f393653, 0x5f6c7366, 0x2e706169, 0x00000063, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static const flash_algo_t flash_algo_config = {
    .init = 0x1,
    .uninit = 0x19,
    .erase_sector = 0x55,
    .program_page = 0x8d,
    .static_base = 0x490,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {SECURE_FLASH_START_ADDR, 0x8000},
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 0x200,
    .flash_start = SECURE_FLASH_START_ADDR,
    .flash_size = SECURE_ROM_SIZE,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

/* Non-secure flash */
static const sector_info_t sectors_info_ns[] = {
    {CODE_FLASH_START_NS, 0x8000},
};

/* Non-secure flash */
static const flash_target_config_t flash_target_config_ns = {
    .page_size  = 0x200,
    .flash_start = CODE_FLASH_START_NS,
    .flash_size = FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES - SECURE_ROM_SIZE,
    .sectors = sectors_info_ns,
    .sector_info_count = sizeof(sectors_info_ns) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
    obj->target_config_ns = &flash_target_config_ns;
    FLASH_Init(&flash_config);
}

MBED_NONSECURE_ENTRY int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    /* Check if flash is empty */
    if (FLASH_VerifyErase(&flash_config, address, size) == kStatus_FLASH_Success) {
        memset(data, 0xFF, size);
        return 0;
    }

    memcpy(data, (const void *)address, size);
    return 0;
}

#endif  // #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#endif

