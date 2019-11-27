/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "mbed_assert.h"
#include "mbed_error.h"
#include "cyhal_crc.h"

#if DEVICE_CRC

#ifdef __cplusplus
extern "C" {
#endif

static cyhal_crc_t cy_crc;
static crc_algorithm_t cy_crc_cfg;
static bool cy_crc_initialized = false;

bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    return config->width <= 32;
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    if (!cy_crc_initialized) {
        if (CY_RSLT_SUCCESS != cyhal_crc_init(&cy_crc)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_crc_init");
        }
        cy_crc_initialized = true;
    }
    if (!hal_crc_is_supported(config)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_UNSUPPORTED), "unsupported CRC width");
    }
    cy_crc_cfg.width = config->width;
    cy_crc_cfg.polynomial = config->polynomial;
    cy_crc_cfg.lfsrInitState = config->initial_xor;
    cy_crc_cfg.dataXor = 0;
    cy_crc_cfg.remXor = config->final_xor;
    cy_crc_cfg.dataReverse = config->reflect_in ? 1 : 0;
    cy_crc_cfg.remReverse = config->reflect_out ? 1 : 0;
    if (CY_RSLT_SUCCESS != cyhal_crc_start(&cy_crc, &cy_crc_cfg)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_crc_start");
    }
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    MBED_ASSERT(cy_crc_initialized);
    if (NULL == data || 0 == size) {
        return;
    }
    if (CY_RSLT_SUCCESS != cyhal_crc_compute(&cy_crc, data, size)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_crc_compute");
    }
}

uint32_t hal_crc_get_result(void)
{
    uint32_t value;
    if (CY_RSLT_SUCCESS != cyhal_crc_finish(&cy_crc, &value)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_crc_finish");
    }
    cyhal_crc_free(&cy_crc);
    cy_crc_initialized = false;
    // mbed expects result to be aligned unusually in this case
    if (0 != (cy_crc_cfg.width % 8) && 0 == cy_crc_cfg.remReverse) {
        value ^= cy_crc_cfg.remXor; // Undo result XOR
        value <<= 8 - (cy_crc_cfg.width % 8);   // Left align to nearest byte
        value ^= cy_crc_cfg.remXor; // Redo result XOR
    }
    return value;
}

#ifdef __cplusplus
};
#endif

#endif // DEVICE_CRC
