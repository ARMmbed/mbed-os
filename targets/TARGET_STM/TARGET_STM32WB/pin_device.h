/* mbed Microcontroller Library
 * Copyright (c) 2019 STMicroelectronics
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

#ifndef MBED_PIN_DEVICE_H
#define MBED_PIN_DEVICE_H

#include "cmsis.h"
#include "stm32wbxx_ll_gpio.h"

extern const uint32_t ll_pin_defines[16];

/* Family specific implementations */
static inline void stm_pin_DisconnectDebug(PinName pin)
{
    /* empty for now */
}

static inline void stm_pin_PullConfig(GPIO_TypeDef *gpio, uint32_t ll_pin, uint32_t pull_config)
{
    switch (pull_config) {
        case GPIO_PULLUP:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_UP);
            break;
        case GPIO_PULLDOWN:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_DOWN);
            break;
        default:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_NO);
            break;
    }
}

static inline void stm_pin_SetAFPin(GPIO_TypeDef *gpio, PinName pin, uint32_t afnum)
{
    uint32_t ll_pin  = ll_pin_defines[STM_PIN(pin)];

    if (STM_PIN(pin) > 7) {
        LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
    } else {
        LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
    }
}

#endif
