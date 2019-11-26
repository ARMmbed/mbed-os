/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "mbed_assert.h"
#include "mbed_error.h"
#include "PinNames.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cy_gpio.h"
#include "gpio_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void apply_config(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != CYHAL_NC_PIN_VALUE);

    cy_rslt_t rslt;
    if (CY_RSLT_SUCCESS != (rslt = cyhal_gpio_configure(obj->pin, obj->direction, obj->drive_mode))) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, CY_RSLT_GET_CODE(rslt)), "cyhal_gpio_configure failed");
    }

    if (obj->drive_mode == PullUp) {
        gpio_set_pull(obj, 1);
    } else if (obj->drive_mode == PullDown) {
        gpio_set_pull(obj, 0);
    }
}

uint32_t gpio_set(PinName pin)
{
    // unimplemented (appears to be unused)
    return 0;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin != CYHAL_NC_PIN_VALUE) {
        cy_rslt_t rslt;
        obj->direction = CYHAL_GPIO_DIR_INPUT;
        obj->drive_mode = CYHAL_GPIO_DRIVE_NONE;
        if (CY_RSLT_SUCCESS != (rslt = cyhal_gpio_init(obj->pin, obj->direction, obj->drive_mode, false))) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, CY_RSLT_GET_CODE(rslt)), "cyhal_gpio_init failed");
        }
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    obj->drive_mode = mode;
    apply_config(obj);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        obj->direction = CYHAL_GPIO_DIR_INPUT;
        if (obj->drive_mode == CYHAL_GPIO_DRIVE_STRONG) {
            obj->drive_mode = CYHAL_GPIO_DRIVE_NONE;
        }
    } else if (direction == PIN_OUTPUT) {
        // mbed reads from input buffer instead of DR even for output pins so always leave input buffer enabled
        obj->direction = CYHAL_GPIO_DIR_BIDIRECTIONAL;
        gpio_write(obj, obj->output_val);
        if (obj->drive_mode == CYHAL_GPIO_DRIVE_NONE || obj->drive_mode == CYHAL_GPIO_DRIVE_ANALOG) {
            obj->drive_mode = CYHAL_GPIO_DRIVE_STRONG;
        }
    }
    apply_config(obj);
}

void gpio_free(gpio_t *obj)
{
    cyhal_gpio_free(obj->pin);
}

#ifdef __cplusplus
}
#endif
