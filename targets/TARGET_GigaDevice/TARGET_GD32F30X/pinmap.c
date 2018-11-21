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

extern const int GD_GPIO_REMAP[];
extern const int GD_GPIO_MODE[];
extern const int GD_GPIO_SPEED[];

static void gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pin);

/** Configure pin (mode, speed, reamp function )
 *
 * @param pin gpio pin name
 * @param function gpio pin mode, speed, remap function
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t mode  = GD_PIN_MODE_GET(function);
    uint32_t remap = GD_PIN_REMAP_GET(function);
    uint32_t speed = GD_PIN_SPEED_GET(function);
    uint32_t port  = GD_PORT_GET(pin);
    uint32_t gd_pin  = 1 << GD_PIN_GET(pin);

    uint32_t gpio = gpio_clock_enable(port);
    gpio_para_init(gpio, GD_GPIO_MODE[mode], GD_GPIO_SPEED[speed], gd_pin);

    if (remap != 0) {
        rcu_periph_clock_enable(RCU_AF);
        gpio_pin_remap_config(GD_GPIO_REMAP[remap], ENABLE);
    }
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
    if (mode != PullNone) {
        gpio_mode_set(gpio, GD_GPIO_MODE[mode], gd_pin);
    }
}

/** configure gpio pin mode
 *
 * @param gpio_periph gpio port name
 * @param mode gpio pin mode
 * @param pin gpio pin number
 */
static void gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pin)
{
    uint16_t i;
    uint32_t temp_mode = 0U;
    uint32_t reg = 0U;

    /* GPIO mode configuration */
    temp_mode = (uint32_t)(mode & ((uint32_t)0x0FU));

    /* configure the eight low port pins with GPIO_CTL0 */
    for (i = 0U; i < 8U; i++) {
        if ((1U << i) & pin) {
            reg = GPIO_CTL0(gpio_periph);

            /* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i, temp_mode);

            /* set IPD or IPU */
            if (GPIO_MODE_IPD == mode) {
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)pin;
            } else {
                /* set the corresponding OCTL bit */
                if (GPIO_MODE_IPU == mode) {
                    GPIO_BOP(gpio_periph) = (uint32_t)pin;
                }
            }

            /* set GPIO_CTL0 register */
            GPIO_CTL0(gpio_periph) = reg;
        }
    }
    /* configure the eight high port pins with GPIO_CTL1 */
    for (i = 8U; i < 16U; i++) {
        if ((1U << i) & pin) {
            reg = GPIO_CTL1(gpio_periph);

            /* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i - 8U, temp_mode);

            /* set IPD or IPU */
            if (GPIO_MODE_IPD == mode) {
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)pin;
            } else {
                /* set the corresponding OCTL bit */
                if (GPIO_MODE_IPU == mode) {
                    GPIO_BOP(gpio_periph) = (uint32_t)pin;
                }
            }

            /* set GPIO_CTL1 register */
            GPIO_CTL1(gpio_periph) = reg;
        }
    }
}


