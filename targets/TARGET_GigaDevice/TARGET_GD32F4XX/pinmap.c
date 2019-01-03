/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "PortNames.h"
#include "mbed_error.h"

extern uint32_t gpio_clock_enable(uint32_t port_idx);

extern const int GD_GPIO_MODE[];
extern const int GD_GPIO_PULL_UP_DOWN[];
extern const int GD_GPIO_OUTPUT_MODE[];
extern const int GD_GPIO_SPEED[];
extern const int GD_GPIO_AF[];

/** Configure pin (mode, speed, reamp function )
 *
 * @param pin gpio pin name
 * @param function gpio pin mode, speed, remap function
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t mode  = GD_PIN_MODE_GET(function);
    uint32_t output = GD_PIN_OUTPUT_MODE_GET(function) ;
    uint32_t pull  = GD_PIN_PULL_STATE_GET(function);
    uint32_t speed = GD_PIN_SPEED_GET(function);
    uint32_t af = GD_PIN_REMAP_GET(function);

    uint32_t port  = GD_PORT_GET(pin);
    uint32_t gd_pin  = 1 << GD_PIN_GET(pin);

    uint32_t gpio = gpio_clock_enable(port);
    gpio_af_set(gpio, GD_GPIO_AF[af], gd_pin);
    gpio_mode_set(gpio, GD_GPIO_MODE[mode], GD_GPIO_PULL_UP_DOWN[pull], gd_pin);
    gpio_output_options_set(gpio, GD_GPIO_OUTPUT_MODE[output], GD_GPIO_SPEED[speed], gd_pin);
}

/** Only configure pin mode
 *
 * @param pin gpio pin name
 * @param function gpio pin mode
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t port  = GD_PORT_GET(pin);
    uint32_t gd_pin  = 1 << GD_PIN_GET(pin);

    uint32_t gpio = gpio_clock_enable(port);
    /* get pin mode */
    uint32_t pin_mode = (GPIO_CTL(gpio) >> (gd_pin * 2)) & 0x03;
    if ((pin_mode == PIN_MODE_OUTPUT) || (pin_mode == PIN_MODE_ANALOG)) {
        if ((mode == OpenDrainNoPull) || (mode == OpenDrainPullUp) || (mode == OpenDrainPullDown)) {
            /* set pin open drain mode */
            GPIO_OMODE(gpio) |= (uint32_t)gd_pin;
        } else {
            /* set pin push pull mode */
            GPIO_OMODE(gpio) &= (uint32_t)(~gd_pin);
        }
    }

    if ((mode == OpenDrainPullUp) || (mode == PullUp)) {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull up */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_PULLUP);
    } else if ((mode == OpenDrainPullDown) || (mode == PullDown)) {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull down */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_PULLDOWN);
    } else {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull none */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_NONE);
    }
}
