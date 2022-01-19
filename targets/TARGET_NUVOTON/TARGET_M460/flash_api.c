/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
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
#include "flash_data.h"
#include "mbed_critical.h"
#include "M460_mem.h"

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
// NOTE: On ARMv7-M/ARMv8-M, instruction fetches are always little-endian.
static uint32_t FLASH_ALGO[] = {
    0x4770ba40, 0x4770bac0, 0x0030ea4f, 0x00004770, 0xf8114601, 0xf8110b01, 0xeb003b01, 0xf8112003, 
    0xeb003b01, 0xf8114003, 0xeb003b01, 0x47706003, 0x4603b510, 0x4c952059, 0x20166020, 0xf8c405a4, 
    0x20880100, 0x0100f8c4, 0xf8d006c0, 0x28010100, 0x2001d001, 0x488ebd10, 0xf0406800, 0xf04f0004, 
    0xf8c44480, 0x46200200, 0x0204f8d0, 0x0004f040, 0x0204f8c4, 0x68004887, 0x0029f040, 0x60204c85, 
    0x68004620, 0x0029f000, 0xd0012829, 0xe7e12001, 0xe7df2000, 0xbf004601, 0x30c0487e, 0xf0006800, 
    0x28000001, 0x487bd1f8, 0xf0206800, 0x4a790001, 0x20006010, 0xb5304770, 0x460c4603, 0x4875bf00, 
    0x680030c0, 0x0001f000, 0xd1f82800, 0x68004871, 0x0040f040, 0x60284d6f, 0x60444628, 0x60c36082, 
    0x61282001, 0xbf00bf00, 0xf3bfbf00, 0xbf008f6f, 0xbf00bf00, 0x4867bf00, 0x680030c0, 0x0001f000, 
    0xd1f82800, 0x68014863, 0x0040f001, 0x4861b118, 0x20016001, 0x2000bd30, 0xb530e7fc, 0x25004604, 
    0x040bf36f, 0x0070f404, 0x6f72f1b0, 0xf1a4d102, 0x4d596472, 0x4621462a, 0xf7ff2022, 0xbd30ffbc, 
    0x4604b570, 0x2600460d, 0x4631e008, 0xf8541c76, 0xf7ff0021, 0xb108ffe2, 0xbd702001, 0xd3f442ae, 
    0xe7fa2000, 0x41f0e92d, 0x460d4604, 0xf0044616, 0xb1100003, 0xe8bd2001, 0x1ce881f0, 0x0503f020, 
    0x4844bf00, 0x680030c0, 0x0001f000, 0xd1f82800, 0x68004840, 0x0040f040, 0x6008493e, 0x60c82021, 
    0x483ce024, 0x46306044, 0xff32f7ff, 0x60884939, 0x61082001, 0xbf00bf00, 0xf3bfbf00, 0xbf008f6f, 
    0xbf00bf00, 0x4833bf00, 0x680030c0, 0x0001f000, 0xd1f82800, 0x6807482f, 0x0040f007, 0x482db118, 
    0x20016007, 0x1d24e7c7, 0x1f2d1d36, 0xd1d82d00, 0xe7c02000, 0x41f0e92d, 0x460d4604, 0xf0044616, 
    0xb1100003, 0xe8bd2001, 0x1ce881f0, 0x0503f020, 0x4820bf00, 0x680030c0, 0x0001f000, 0xd1f82800, 
    0x6800481c, 0x0040f040, 0x6008491a, 0x60c82000, 0x4818e027, 0x20016044, 0x61084916, 0xbf00bf00, 
    0xf3bfbf00, 0xbf008f6f, 0xbf00bf00, 0x4811bf00, 0x680030c0, 0x0001f000, 0xd1f82800, 0x6807480d, 
    0x0040f007, 0x480bb110, 0xe00c6007, 0x68874809, 0xf7ff4630, 0x42b8fecd, 0xe004d000, 0x1d361d24, 
    0x2d001f2d, 0xbf00d1d5, 0xe7bc4620, 0x40000100, 0x40000200, 0x4000c000, 0x0055aa03, 0x00000000,
};

static const flash_algo_t flash_algo_config = {
    .init = 0x00000031,
    .uninit = 0x00000095,
    .erase_sector = 0x0000011b,
    .program_page = 0x00000165,
    .static_base = 0x0000029c,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {MBED_ROM_START, 0x1000},       // (start, sector size)
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 4,                // 4 bytes
                                    // Here page_size is program unit, which is different than FMC definition.
    .flash_start = MBED_ROM_START,
    .flash_size = MBED_ROM_SIZE,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}
