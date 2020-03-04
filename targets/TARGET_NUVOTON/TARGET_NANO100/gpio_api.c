/* mbed Microcontroller Library
 * Copyright (c) 2015-2020 Nuvoton
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
    
    // GPIO PinMap
    pin_function(pin, 0 << NU_MFP_POS(pin_index));

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

    switch (mode) {
        case PullNone:
        case PullDown:
        case PullUp:
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We translate to input-only/push-pull output I/O mode dependent on direction. */
            obj->mode = (obj->direction == PIN_INPUT) ? InputOnly : PushPullOutput;
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

    switch (obj->mode) {
        case PullNone:
        case PullDown:
        case PullUp:
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We translate to input-only/push-pull output I/O mode dependent on direction. */
            obj->mode = (obj->direction == PIN_INPUT) ? InputOnly : PushPullOutput;
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

void gpio_get_capabilities(gpio_t *obj, gpio_capabilities_t *cap)
{
    // Pull modes not supported.
    cap->pull_none = 0;
    cap->pull_down = 0;
    cap->pull_up = 0;
}
