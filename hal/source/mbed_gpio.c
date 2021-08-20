/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#include "hal/gpio_api.h"
#include "platform/mbed_toolchain.h"
#include "hal/PinNameAliases.h"

static inline void _gpio_init_in(gpio_t *gpio, PinName pin, PinMode mode)
{
    gpio_init(gpio, pin);
    if (pin != NC) {
        gpio_dir(gpio, PIN_INPUT);
        gpio_mode(gpio, mode);
    }
}

static inline void _gpio_init_out(gpio_t *gpio, PinName pin, PinMode mode, int value)
{
    gpio_init(gpio, pin);
    if (pin != NC) {
        gpio_write(gpio, value);
        gpio_dir(gpio, PIN_OUTPUT);
        gpio_mode(gpio, mode);
    }
}

void gpio_init_in(gpio_t *gpio, PinName pin)
{
    gpio_init_in_ex(gpio, pin, PullDefault);
}

void gpio_init_in_ex(gpio_t *gpio, PinName pin, PinMode mode)
{
    _gpio_init_in(gpio, pin, mode);
}

void gpio_init_out(gpio_t *gpio, PinName pin)
{
    gpio_init_out_ex(gpio, pin, 0);
}

void gpio_init_out_ex(gpio_t *gpio, PinName pin, int value)
{
    _gpio_init_out(gpio, pin, PullNone, value);
}

void gpio_init_inout(gpio_t *gpio, PinName pin, PinDirection direction, PinMode mode, int value)
{
    if (direction == PIN_INPUT) {
        _gpio_init_in(gpio, pin, mode);
        if (pin != NC) {
            gpio_write(gpio, value);    // we prepare the value in case it is switched later
        }
    } else {
        _gpio_init_out(gpio, pin, mode, value);
    }
}

// To be re-implemented in the target layer if required.
MBED_WEAK void gpio_get_capabilities(gpio_t *gpio, gpio_capabilities_t *cap)
{
    (void)gpio; // By default, every pin supports all basic input pull modes.
    cap->pull_none = 1;
    cap->pull_down = 1;
    cap->pull_up = 1;
}

typedef enum {
    DEFAULT_GPIO = 0,
} DefaultGPIOPeripheralName;

MBED_WEAK const PinMap *gpio_pinmap()
{
    // Targets should override this weak implementation to provide correct data.
    static const PinMap empty_gpio_pinmap[] = {
#if defined (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO)
        {ARDUINO_UNO_D0, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D1, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D2, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D3, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D4, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D5, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D6, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D7, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D8, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D9, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D10, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D11, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D12, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D13, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D14, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_D15, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A0, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A1, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A2, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A3, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A4, DEFAULT_GPIO, 0},
        {ARDUINO_UNO_A5, DEFAULT_GPIO, 0},
#endif
        {NC, NC, 0},
    };
    return empty_gpio_pinmap;
}
