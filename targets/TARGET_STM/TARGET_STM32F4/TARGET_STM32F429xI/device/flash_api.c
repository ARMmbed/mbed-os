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
#include "flash_data.h"
#include "mbed_critical.h"

// This file is automagically generated

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
static uint32_t FLASH_ALGO[] = {
    0xf3c04601, 0x28203007, 0x2204bf24, 0x1050eb02, 0x2810d205, 0x2203bf26, 0x1010eb02, 0xf4110880,
    0xbf181f80, 0x0010f040, 0x48714770, 0x6001496f, 0x60014970, 0x68014870, 0x01f0f041, 0x486f6001,
    0xf0106800, 0xd1080f20, 0xf245486d, 0x60015155, 0x60412106, 0x71fff640, 0x20006081, 0x49694770,
    0xf4206808, 0x600a52f8, 0x48676008, 0xf0416801, 0x60014100, 0x47702000, 0xc18cf8df, 0x0000f8dc,
    0x0004f040, 0x0000f8cc, 0x0000f8dc, 0x4000f440, 0x0000f8cc, 0x0000f8dc, 0x3080f440, 0x0000f8cc,
    0x0004f1ac, 0xf4116801, 0xbf1c3f80, 0x21aaf64a, 0xd0044a53, 0x68036011, 0x3f80f413, 0xf8dcd1fa,
    0xf0200000, 0xf8cc0004, 0xf8dc0000, 0xf4200000, 0xf8cc4000, 0x20000000, 0xf3c04770, 0x29203107,
    0x2204bf24, 0x1151eb02, 0x2910d205, 0x2203bf26, 0x1111eb02, 0xf4100889, 0xbf181f80, 0x0110f041,
    0x6802483d, 0x02f0f042, 0xf1006002, 0x22020c04, 0x2000f8cc, 0x2000f8dc, 0xea0323f8, 0x431101c1,
    0x1000f8cc, 0x1000f8dc, 0x3180f441, 0x1000f8cc, 0xf4116801, 0xbf1c3f80, 0x21aaf64a, 0xd0044a30,
    0x68036011, 0x3f80f413, 0xf8dcd1fa, 0xf0211000, 0xf8cc0102, 0x68011000, 0x0ff0f011, 0x2000bf04,
    0x68014770, 0x01f0f041, 0x20016001, 0x4b224770, 0x1cc9b430, 0xc000f8d3, 0x0103f031, 0x0cf0f04c,
    0xc000f8c3, 0x0404f103, 0x0c00f04f, 0xc000f8c4, 0xf240bf18, 0xd0252501, 0xc000f8d4, 0x0c05ea4c,
    0xc000f8c4, 0xc000f8d2, 0xc000f8c0, 0xc000f8d3, 0x3f80f41c, 0xf8d4d1fa, 0xf02cc000, 0xf8c40c01,
    0xf8d3c000, 0xf01cc000, 0xd0060ff0, 0xf0406818, 0x601800f0, 0x2001bc30, 0x1d004770, 0xf1021f09,
    0xd1d90204, 0x2000bc30, 0x00004770, 0x45670123, 0x40023c04, 0xcdef89ab, 0x40023c0c, 0x40023c14,
    0x40003000, 0x40023c00, 0x40023c10, 0x00000000
};

static const flash_algo_t flash_algo_config = {
    .init = 0x2b,
    .uninit = 0x5f,
    .erase_sector = 0xdb,
    .program_page = 0x16f,
    .static_base = 0x20c,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x8000000, 0x4000},
    {0x8010000, 0x10000},
    {0x8020000, 0x20000},
    {0x8100000, 0x4000},
    {0x8110000, 0x10000},
    {0x8120000, 0x20000},
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 0x400,
    .flash_start = 0x8000000,
    .flash_size = 0x200000,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}
