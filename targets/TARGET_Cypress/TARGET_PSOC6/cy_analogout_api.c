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

#include "analogout_api.h"
#include "mbed_error.h"
#include "cyhal_dac.h"
#include "limits.h"

#if DEVICE_ANALOGOUT

#ifdef __cplusplus
extern "C" {
#endif

void analogout_init(dac_t *obj, PinName pin)
{
    if (CY_RSLT_SUCCESS != cyhal_dac_init(&(obj->hal_dac), pin)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ANALOG, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_dac_init");
    }
}

void analogout_free(dac_t *obj)
{
    cyhal_dac_free(&(obj->hal_dac));
}

void analogout_write(dac_t *obj, float value)
{
    MBED_ASSERT(value >= 0.0 && value <= 1.0f);
    analogout_write_u16(obj, (uint16_t)(value * UINT16_MAX));
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    cyhal_dac_write(&(obj->hal_dac), value);
}

float analogout_read(dac_t *obj)
{
    return analogout_read_u16(obj) * (1.0f / UINT16_MAX);
}

uint16_t analogout_read_u16(dac_t *obj)
{
    return cyhal_dac_read(&(obj->hal_dac));
}

const PinMap *analogout_pinmap(void)
{
    return PinMap_DAC;
}

#ifdef __cplusplus
}
#endif

#endif
