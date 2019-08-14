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
        if (CY_RSLT_SUCCESS != (rslt = cyhal_gpio_init(obj->pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_ANALOG, false))) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, CY_RSLT_GET_CODE(rslt)), "cyhal_gpio_init failed");
        }
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != CYHAL_NC_PIN_VALUE);
    cy_rslt_t rslt;
    if (CY_RSLT_SUCCESS != (rslt = cyhal_gpio_drivemode(obj->pin, mode))) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_GPIO, CY_RSLT_GET_CODE(rslt)), "cyhal_gpio_mode failed");
    }
    if (mode == PullUp) {
        gpio_write(obj, 1);
    } else if (mode == PullDown) {
        gpio_write(obj, 0);
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (direction == PIN_INPUT) {
        cyhal_gpio_direction(obj->pin, CYHAL_GPIO_DIR_INPUT);
        gpio_mode(obj, CYHAL_GPIO_DRIVE_ANALOG);
    } else if (direction == PIN_OUTPUT) {
        // mbed reads from input buffer instead of DR even for output pins so always leave input buffer enabled
        cyhal_gpio_direction(obj->pin, CYHAL_GPIO_DIR_BIDIRECTIONAL);
        gpio_mode(obj, CYHAL_GPIO_DRIVE_STRONG);
    }
}

#ifdef __cplusplus
}
#endif
