/*******************************************************************************
 * Copyright (c) 2010-2018 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#if DEVICE_FLASH
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
    .algo_blob    = (uint32_t *)FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x0, 0x800},
};

static const flash_target_config_t flash_target_config = {
    .page_size   = 0x8,                       // minimal programmable unit size
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
