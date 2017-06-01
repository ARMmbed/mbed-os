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
#ifdef DEVICE_FLASH
#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"

// This file is automagically generated

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM

static unsigned FLASH_ALGO[] = {
    0x20004A4B,0x60111E41,0x6211494A,0x60912107,0x074B6811,0xF011D5FC,0xD0000F30,0x21002001,
    0x47706211,0x2400B510,0xD1082A01,0xF872F000,0x6D09493F,0xD00207C8,0xFFE2F7FF,0x46204604,
    0x493BBD10,0x62082000,0xB5104770,0xF862F000,0x4010E8BD,0x4601E7D4,0x20004A35,0x60131E43,
    0x49346191,0x21066211,0x68116091,0xD5FC074B,0x0F30F011,0x2001D000,0x62112100,0xB57C4770,
    0x4B2C4C2B,0x62232500,0xF04FE03E,0x602333FF,0xD3042908,0x61236813,0x61636853,0xF04FE025,
    0xE9CD33FF,0x29083300,0xE8DFD21A,0x1619F001,0x0A0D1013,0x79910407,0x1006F88D,0xF88D7951,
    0x79111005,0x1004F88D,0xF88D78D1,0x78911003,0x1002F88D,0xF88D7851,0x78111001,0x1000F88D,
    0x1300E9DD,0x61636121,0x60E02108,0x60A32304,0xF0136823,0xD0010F30,0xE0072501,0x075B6823,
    0x3008D5FC,0x32083908,0xD1BE2900,0x62202000,0xBD7C4628,0x21004806,0x4A066041,0x4A066202,
    0x22046342,0x22016382,0x62016542,0x00004770,0x40018000,0x676C7565,0xB8950950,0
};

static const flash_algo_t flash_algo_config = {
    .init         = 0x00000025,
    .uninit       = 0x00000043,
    .erase_sector = 0x00000057,
    .program_page = 0x0000007F,
    .static_base  = 0x0000013C,
    .algo_blob    = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x0, 0x800},
};

static const flash_target_config_t flash_target_config = {
    .page_size   = 0x800,
    .flash_start = 0x0,
    .flash_size  = 0x0007F000,
    .sectors     = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}
#endif
