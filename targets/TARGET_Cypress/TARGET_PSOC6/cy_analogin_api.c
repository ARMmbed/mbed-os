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

#include "analogin_api.h"
#include "mbed_error.h"
#include "cyhal_adc.h"
#include "cyhal_utils.h"
#include "cyhal_pin_package.h"
#include "cy_pdl.h"

#if DEVICE_ANALOGIN

#ifdef __cplusplus
extern "C" {
#endif

static cyhal_adc_t cy_mbed_adc_inst[CY_IP_MXS40PASS_SAR_INSTANCES];

void analogin_init(analogin_t *obj, PinName pin)
{
    const cyhal_resource_pin_mapping_t *map = CY_UTILS_GET_RESOURCE(pin, cyhal_pin_map_pass_sarmux_pads);
    if (NULL == map) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ANALOG, MBED_ERROR_CODE_INVALID_ARGUMENT), "pin does not support analog in");
    } else {
        const uint8_t adc_num = map->inst->block_num;
        obj->hal_adc = &(cy_mbed_adc_inst[adc_num]);
        if (obj->hal_adc->base == NULL) {
            if (CY_RSLT_SUCCESS != cyhal_adc_init(obj->hal_adc, pin, NULL)) {
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ANALOG, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_adc_init");
            }
        }

        if (CY_RSLT_SUCCESS != cyhal_adc_channel_init(&(obj->hal_adc_channel), obj->hal_adc, pin)) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_ANALOG, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_adc_channel_init");
        }
    }
}

float analogin_read(analogin_t *obj)
{
    return analogin_read_u16(obj) * (1.0f / (1.0f + CYHAL_ADC_MAX_VALUE));
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    return cyhal_adc_read_u16(&(obj->hal_adc_channel));
}

void analogin_free(analogin_t *obj)
{
    cyhal_adc_channel_free(&(obj->hal_adc_channel));
    cyhal_adc_free(obj->hal_adc);
}

const PinMap *analogin_pinmap(void)
{
    return PinMap_ADC;
}

#ifdef __cplusplus
}
#endif

#endif
