/* mbed Microcontroller Library
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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

#include "device.h"

#if DEVICE_CRC

#include "mbed_assert.h"
#include "crc_api.h"

#include "psoc6_utils.h"

#include "cy_crypto_core_crc.h"

static uint32_t crcWidth = 0UL;
static uint32_t crcShift = 0UL;
static uint32_t crcXorMask;

/* Cypress CRYPTO hardware supports any CRC algorithms from CRC-3 to CRC-32 */
bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    return (config != NULL);
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    uint32_t myMask = 0UL;

    if (!hal_crc_is_supported(config) || (cy_reserve_crypto(CY_CRYPTO_CRC_HW) != 0U)) {
        return;
    }

    crcWidth = config->width;

    crcShift = (uint32_t)(!config->reflect_out) * (crcWidth & 7U);
    if (0U != crcShift) {
        crcShift = 8U - crcShift;
        for (uint32_t i = 0U; i < crcShift; i++) {
            myMask |= 1U << i;
        }
        crcXorMask = config->final_xor & myMask;
    }

    Cy_Crypto_Core_Crc_CalcInit(CRYPTO, config->width,
                                config->polynomial,
                                config->reflect_in,
                                0U,
                                config->reflect_out,
                                config->final_xor >> crcShift,
                                config->initial_xor);
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    if ((data == NULL) || (size <= 0U) || (crcWidth == 0U)) {
        return;
    }

    Cy_Crypto_Core_Crc_CalcPartial(CRYPTO, data, size);
}

uint32_t hal_crc_get_result(void)
{
    uint32_t result = 0UL;

    if (crcWidth == 0UL) {
        return 0xFFFFFFFFUL;
    }

    Cy_Crypto_Core_Crc_CalcFinish(CRYPTO, crcWidth, &result);

    if (crcShift) {
        result = result << crcShift;
        result = result ^  crcXorMask;
    }

    crcWidth = 0UL;

    cy_free_crypto(CY_CRYPTO_CRC_HW);

    return result;
}

#endif //DEVICE_CRC

