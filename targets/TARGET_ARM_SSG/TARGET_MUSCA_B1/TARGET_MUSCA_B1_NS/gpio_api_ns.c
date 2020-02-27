/* mbed Microcontroller Library
 * Copyright (c) 2020 Arm Limited
 *
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

/*
 * This file implements APIS defined in hal/gpio_api.h
 * Note: Due to a HW limitation, GPIO in Musca-B1 is Secure only, so secure
 *       service should be used for GPIO in NS domain.
 */

#include <stddef.h>
#include "gpio_api.h"
#include "pinmap.h"
#include "objects.h"
#include "mbed_error.h"
#include "mbed_assert.h"
#include "rtx_lib.h"
#include "tfm_platform_api.h"
#include "tfm_ioctl_api.h"

#define RETURN_VOID_true return;
#define RETURN_VOID_false return 0;

#define IRQ_MODE_CHECK(is_func_void) \
    /* Secure service can't be called in interrupt context. */ \
    if (IsIrqMode()) { \
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_HAL, \
                MBED_ERROR_INVALID_OPERATION), \
                "GPIO secure service can't be called in interrupt context\n"); \
        RETURN_VOID_ ## is_func_void \
    }


uint32_t gpio_set(PinName pin)
{
    pin_function(pin, (int)PRIMARY_FUNC);

    /* Return the correct mask of the given PIN */
    return (1 << GPIO_PIN_NUMBER(pin));

}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin_num = (uint32_t)NC;
    uint32_t result = 0;
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;

    if (pin >= PA0 && pin <= PA15) {
        IRQ_MODE_CHECK(true);
        ret = tfm_platform_gpio_init(&result);
        if (ret || result) {
            return;
        }

        obj->pin_num = pin;
        /* GPIO is input by default */
        obj->direction = PIN_INPUT;
        return;
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin_num, mode);

}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t pin_dir = (direction == PIN_INPUT ? 0 : 1);
    uint32_t result = 0;
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;

    IRQ_MODE_CHECK(true);

    ret = tfm_platform_gpio_pin_config(obj->pin_num, pin_dir, &result);
    if (ret || result) {
        error("gpio_dir failed, error %d, gpio error %d", ret, result);
    }
    obj->direction = direction;
}


int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin_num == (uint32_t)NC) {
        return 0;
    } else {
        return 1;
    }

}

void gpio_write(gpio_t *obj, int value)
{
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;
    uint32_t result = 0;

    IRQ_MODE_CHECK(true);

    ret = tfm_platform_gpio_pin_write(obj->pin_num, (uint32_t)value, &result);

    if (ret || result) {
        error("Can not write pin %d; error %d, gpio error %d",
                                                     obj->pin_num, ret, result);
    }

}

int gpio_read(gpio_t *obj)
{
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;
    uint32_t result = 0;
    uint32_t data = 0;

    IRQ_MODE_CHECK(false);

    ret = tfm_platform_gpio_pin_read(obj->pin_num, &data, &result);

    /* The valid return values of this API are 0 and 1 */
    if (ret || result) {
        return 0;
    } else {
        return data;
    }
}
