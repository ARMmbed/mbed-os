/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "crc_api.h"

#include "drivers/fsl_crc.h"
#include "platform/mbed_assert.h"

#if DEVICE_CRC

static crc_bits_t width;
static uint32_t final_xor;

void hal_crc_compute_partial_start(const crc_mbed_config_t* config)
{
    if (config == NULL) {
        return;
    }

    width = (config->width == 32) ? kCrcBits32 : kCrcBits16;
    final_xor = config->final_xor;

    crc_config_t platform_config;
    platform_config.polynomial = config->polynomial;
    platform_config.seed = config->initial_xor;
    platform_config.reflectIn = config->reflect_in;
    platform_config.reflectOut = config->reflect_out;
    platform_config.complementChecksum = false;
    platform_config.crcBits = width;
    platform_config.crcResult = kCrcFinalChecksum;

    CRC_Init(CRC0, &platform_config);
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    if (data == NULL) {
        return;
    }

    if (size == 0) {
        return;
    }

    CRC_WriteData(CRC0, data, size);
}

uint32_t hal_crc_get_result(void)
{
    uint32_t result;

    switch (width)
    {
        case kCrcBits16:
            result = CRC_Get16bitResult(CRC0);
            result ^= final_xor;
            return result;
        case kCrcBits32:
            result = CRC_Get32bitResult(CRC0);
             result ^= final_xor;
            return result;
        default:
            MBED_ASSERT("Unhandled switch case");
        return 0;
    }
}

#endif // DEVICE_CRC
