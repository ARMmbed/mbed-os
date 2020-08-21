/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_gpio.h"

void bp6a_gpio_set_dir(uint8_t port, uint8_t pin, bool is_input)
{
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    uint32_t mask = 1 << pin;

    putreg32(&(gpio_base->ALTFUNCCLR), mask);//, mask);

    if (is_input) {
        putreg32(&(gpio_base->OUTENCLR), mask);//, mask);
    } else {
        putreg32(&(gpio_base->OUTENSET), mask);//, mask);
    }

}

void bp6a_gpio_set_int(uint8_t port, uint8_t pin, bool enable)
{
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    if (enable) {
        putreg32(&(gpio_base->INTENSET), 0x01 << pin);
    } else {
        putreg32(&(gpio_base->INTENCLR), 0x01 << pin);
    }
}

void bp6a_gpio_clear_pend(uint8_t port, uint8_t pin)
{
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);
    putreg32(&(gpio_base->INTCLR), 0x01 << pin);
}

void bp6a_gpio_set_int_type(uint8_t port, uint8_t pin, bool isHigh, bool isEadge)
{
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    if (isHigh) {
        putreg32(&(gpio_base->INTPOLSET), 0x01 << pin);
    } else {
        putreg32(&(gpio_base->INTPOLCLR), 0x01 << pin);

    }

    if (isEadge) {
        putreg32(&(gpio_base->INTTYPESET), 0x01 << pin);
    } else {
        putreg32(&(gpio_base->INTTYPECLR), 0x01 << pin);
    }
}

void bp6a_set_pin_config(uint8_t port, uint8_t pin, uint8_t cfg)
{
    uint8_t idx = port * 4 + pin / 4 + 2;
    uint8_t bit = (pin << 3) % 32;

    modifyreg32(&BP_SYSCON->IOCFG[idx], 0x3 << bit, cfg << bit);
}
