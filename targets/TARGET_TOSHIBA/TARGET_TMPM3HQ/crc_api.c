/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include <stdbool.h>
#include "crc_api.h"
#include "device.h"

#ifdef DEVICE_CRC

static uint32_t final_xor;

bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    if (config == NULL) {
        return false;
    }
    // Currently supported only CRC16_CCITT polynomial.
    if (config->polynomial != POLY_16BIT_CCITT) {
        return false;
    }

    if (config->width != 16) {
        return false;
    }
    // Not support for reflect_in and reflect_out.
    if ((config->reflect_in == true) || (config->reflect_out == true)) {
        return false;
    }

    return true;
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    TSB_CG_FSYSENB_IPENB20 = 1;

    // Intial Value as initial_xor
    TSB_CRC->CLC = config->initial_xor;
    final_xor = config->final_xor;

    // Data width setting CRC data width is 8 bits.
    // Form setting CRC form is CRC16.
    TSB_CRC->TYP = 0x01;
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    if (data && size) {
        uint32_t index = 0U;
        for(index = 0U; index < size; index++) {
            TSB_CRC->DIN = data[index];
        }
    }
}

uint32_t hal_crc_get_result(void)
{
    uint32_t result;

    // Note: Please read [CRCCLC] twice and use the result of the 2nd time
    result = TSB_CRC->CLC;
    result = TSB_CRC->CLC ^ final_xor;

    return (result);
}
#endif // DEVICE_CRC
