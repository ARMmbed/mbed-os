/*
 * Copyright (c) 2015-2017, Nuvoton Technology Corporation
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

#include "gpio_api.h"
#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

uint32_t gpio_set(PinName pin)
{
    if (pin == (PinName) NC) {
        return 0;
    }
    
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    
#if 1
    pin_function(pin, 0 << NU_MFP_POS(pin_index));
#else
    pinmap_pinout(pin, PinMap_GPIO);
#endif

    return (uint32_t)(1 << pin_index);    // Return the pin mask
}

int gpio_is_connected(const gpio_t *obj)
{
    return (obj->pin != (PinName) NC);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    
    if (obj->pin == (PinName) NC) {
        return;
    }

    obj->mask = gpio_set(pin);
    /* Default mode/direction */
    obj->mode = PullUp;
    obj->direction = PIN_INPUT;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    if (obj->pin == (PinName) NC) {
        return;
    }

    uint32_t pin_index = NU_PININDEX(obj->pin);
    uint32_t port_index = NU_PINPORT(obj->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    switch (mode) {
        case PullNone:
            if (mode == PullNone) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_DISABLE);
            }
        case PullDown:
            if (mode == PullDown) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_PULL_DOWN);
            }
        case PullUp:
            if (mode == PullUp) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_PULL_UP);
            }
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We translate to input-only/push-pull output I/O mode dependent on direction. */
            obj->mode = (obj->direction == PIN_INPUT) ? InputOnly : PushPullOutput;
            break;

        case QuasiBidirectional:
            /* With quasi-bidirectional I/O mode, before digital input function is performed,
             * the corresponding bit in GPIOx_DOUT must be set to 1. */
            obj->mode = QuasiBidirectional;
            if (obj->direction == PIN_INPUT) {
                gpio_write(obj, 1);
            }
            break;

        case InputOnly:
        case PushPullOutput:
            /* We may meet contradictory I/O mode/direction configuration. Favor I/O mode
             * in the gpio_mode call here. */
            if (mode == InputOnly) {
                obj->direction = PIN_INPUT;
                obj->mode = InputOnly;
            } else {
                obj->direction = PIN_OUTPUT;
                obj->mode = PushPullOutput;
            }
            break;

        default:
            /* Allow for configuring other I/O modes directly */
            obj->mode = mode;
            break;
    }

    pin_mode(obj->pin, obj->mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (obj->pin == (PinName) NC) {
        return;
    }

    obj->direction = direction;

    uint32_t pin_index = NU_PININDEX(obj->pin);
    uint32_t port_index = NU_PINPORT(obj->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    switch (obj->mode) {
        case PullNone:
            if (obj->mode == PullNone) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_DISABLE);
            }
        case PullDown:
            if (obj->mode == PullDown) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_PULL_DOWN);
            }
        case PullUp:
            if (obj->mode == PullUp) {
                GPIO_SetPullCtl(gpio_base, 1 << pin_index, GPIO_PUSEL_PULL_UP);
            }
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We translate to input-only/push-pull output I/O mode dependent on direction. */
            obj->mode = (obj->direction == PIN_INPUT) ? InputOnly : PushPullOutput;
            break;
            
        case QuasiBidirectional:
            /* With quasi-bidirectional I/O mode, before digital input function is performed,
             * the corresponding bit in GPIOx_DOUT must be set to 1. */
            if (obj->direction == PIN_INPUT) {
                gpio_write(obj, 1);
            }
            break;

        case InputOnly:
        case PushPullOutput:
            /* We may meet contradictory I/O mode/direction configuration. Favor direction
             * in the gpio_dir call here. */
            obj->mode = (obj->direction == PIN_INPUT) ? InputOnly : PushPullOutput;
            break;

        default:
            break;
    }

    pin_mode(obj->pin, obj->mode);
}
