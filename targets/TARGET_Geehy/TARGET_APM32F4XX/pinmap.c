/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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
#include "apm32f4xx_gpio.h"

extern uint32_t gpio_clock_enable(uint32_t port_idx);

extern const int APM_GPIO_MODE[];
extern const int APM_GPIO_PULL_UP_DOWN[];
extern const int APM_GPIO_OUTPUT_MODE[];
extern const int APM_GPIO_SPEED[];
extern const int APM_GPIO_AF[];

 /*!
 * @brief     Configure pin (mode, speed, reamp function )
 *
 * @param     pin : gpio pin name
 *
 * @param     function : gpio pin mode, speed, remap function
 *
 * @retval    None
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t mode  = APM_PIN_MODE_GET(function);
    uint32_t output = APM_PIN_OUTPUT_MODE_GET(function);
    uint32_t pull  = APM_PIN_PULL_STATE_GET(function);
    uint32_t speed = APM_PIN_SPEED_GET(function);
    uint32_t af = APM_PIN_REMAP_GET(function);

    uint32_t port = APM_PORT_GET(pin);
    uint32_t apm_pin = 1 << APM_PIN_GET(pin);

    GPIO_T* gpio = (GPIO_T*)gpio_clock_enable(port);
    GPIO_ConfigPinAF(gpio, APM_PIN_GET(pin), APM_GPIO_AF[af]);

    GPIO_Config_T gpioConfig;

    gpioConfig.mode = APM_GPIO_MODE[mode];
    gpioConfig.otype = APM_GPIO_OUTPUT_MODE[output];
    gpioConfig.pin = apm_pin;
    gpioConfig.pupd = APM_GPIO_PULL_UP_DOWN[pull];
    gpioConfig.speed = APM_GPIO_SPEED[speed];
    GPIO_Config(gpio,&gpioConfig);
}

 /*!
 * @brief     Only configure pin mode
 *
 * @param     pin : gpio pin name
 *
 * @param     function : gpio pin mode
 *
 * @retval    None
 */
void pin_mode(PinName pin, PinMode mode)
{
    uint32_t port = APM_PORT_GET(pin);
    uint32_t apm_pin  = APM_PIN_GET(pin);

    GPIO_T* gpio = (GPIO_T*)gpio_clock_enable(port);

    gpio->PUPD &= ~(0x03 << ((uint16_t)apm_pin * 2));
    gpio->PUPD |= (((uint32_t)mode) << (apm_pin * 2));
}
