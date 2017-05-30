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
    0x2000B508,0x4A5B1E41,0x495B6011,0x21076211,0xBF006091,0x68094957,0x99009100,0x0104F001,
    0xD0F72900,0xF0019900,0xB1010130,0x21002001,0x62114A50,0xB5F0BD08,0x460F4606,0x4D4D4614,
    0x20013554,0x20007028,0x6048494A,0xD1072C01,0x6D004608,0x0001F000,0xF7FFB110,0xBDF0FFD1,
    0xE7FC2000,0x20004601,0x62104A42,0xB5084770,0x20004601,0x4B3F1E42,0x461A601A,0x4A3E6191,
    0x2206621A,0xBF00609A,0x68124A3A,0x9A009200,0x0204F002,0xD0F72A00,0xF0029A00,0xB1020230,
    0x22002001,0x621A4B33,0xB5FEBD08,0x460B4604,0x46252600,0x48304611,0x62384F2E,0xF04FE052,
    0x4F2C30FF,0x2B086038,0x6808D304,0x68486138,0xE02F6178,0x3CFFF04F,0xC000F8CD,0xC004F8CD,
    0x2B084668,0xE8DFD21A,0x1619F003,0x0A0D1013,0x798F0407,0xBF007187,0x7147794F,0x790FBF00,
    0xBF007107,0x70C778CF,0x788FBF00,0xBF007087,0x7047784F,0x780FBF00,0xE0007007,0xBF00BF00,
    0xC050F8DF,0xF8CC9F00,0x9F017010,0x7014F8CC,0xBF002308,0x60C5480F,0x4F0E2004,0x463860B8,
    0xF0006800,0xB1080030,0xE00D2601,0x4809BF00,0x90026800,0xF0009802,0x28000004,0x3B08D0F7,
    0x35083108,0xD1AA2B00,0x2000BF00,0x62384F01,0xBDFE4630,0x40018000,0x676C7565,0
};

static const flash_algo_t flash_algo_config = {
    .init         = 0x00000037,
    .uninit       = 0x00000065,
    .erase_sector = 0x0000006F,
    .program_page = 0x000000AB,
    .static_base  = 0x0000017C,
    .algo_blob    = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x0, 0x800},
};

static const flash_target_config_t flash_target_config = {
    .page_size   = 0x800,
    .flash_start = 0x0,
    .flash_size  = 0x00040000,
    .sectors     = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}
#endif
