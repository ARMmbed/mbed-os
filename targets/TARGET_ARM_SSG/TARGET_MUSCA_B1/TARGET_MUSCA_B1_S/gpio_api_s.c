/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file implements APIS defined in hal/gpio_api.h
 */

#include <stddef.h>
#include "gpio_api.h"
#include "pinmap.h"
#include "objects.h"
#include "mbed_error.h"

uint32_t gpio_set(PinName pin)
{
    pin_function(pin, (int)PRIMARY_FUNC);

    /* Return the correct mask of the given PIN */
    return (1 << GPIO_PIN_NUMBER(pin));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    struct gpio_cmsdk_dev_t *gpio_dev;
    obj->pin_num = (uint32_t)NC;

    if (pin >= PA0 && pin <= PA15) {
#ifdef GPIO0_CMSDK_DEV
    gpio_dev = &GPIO0_CMSDK_DEV;
#else
    error("GPIO associated with expansion pin %d, is disabled", pin);
    /* error() calls exit() eventually, but to be safe we return here */
    return;
#endif

    gpio_cmsdk_init(gpio_dev);

    obj->gpio_dev = gpio_dev;
    obj->pin_num = pin;
    /* GPIO is input by default */
    obj->direction = PIN_INPUT;
    return;
    }

    error("pin %d is not a GPIO", pin);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin_num, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    enum gpio_cmsdk_direction_t pin_dir =
              (direction == PIN_INPUT) ? GPIO_CMSDK_INPUT : GPIO_CMSDK_OUTPUT;

    if (gpio_cmsdk_pin_config(obj->gpio_dev, obj->pin_num, pin_dir,
                                  GPIO_CMSDK_MAIN_FUNC) == GPIO_CMSDK_ERR_NONE) {
        obj->direction = direction;
    }
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
    enum gpio_cmsdk_error_t ret =
           gpio_cmsdk_pin_write(obj->gpio_dev, obj->pin_num, (uint32_t)value);

    if (ret) {
        error("Can not write pin %d", obj->pin_num);
    }
}

int gpio_read(gpio_t *obj)
{
    uint32_t data = 0;
    (void)gpio_cmsdk_pin_read(obj->gpio_dev, obj->pin_num, &data);

    return (int)data;
}
