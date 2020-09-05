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

#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "cmsis.h"

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;

    if (pin == (PinName)NC) {
        return;
    }

    uint32_t port = BP6A_PORT_IDX(obj->pin);
    uint32_t mask = 1 << pin;
    uint8_t pin_n =  BP6A_PIN_IDX(pin);

    bp6a_set_pin_config(port, pin_n, (PIN_INPUT | PIN_OUTPUT));

    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);
    putreg32(&(gpio_base->ALTFUNCCLR), mask);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    bp6a_gpio_set_dir(BP6A_PORT_IDX(obj->pin), BP6A_PIN_IDX(obj->pin), (PIN_INPUT == direction));
}

void gpio_write(gpio_t *obj, int value)
{
    if (obj->pin == (PinName)NC) {
        return;
    }

    uint32_t port = BP6A_PORT_IDX(obj->pin);
    uint32_t mask = 1 << BP6A_PIN_IDX(obj->pin);
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    modifyreg32(&(gpio_base->DATAOUT), mask, (value > 0) ? mask : 0);

}

int gpio_read(gpio_t *obj)
{
    if (obj->pin == (PinName)NC) {
        return 0;
    }

    uint32_t port = BP6A_PORT_IDX(obj->pin);
    uint32_t pin_n = BP6A_PIN_IDX(obj->pin);
    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    return (getreg32(&(gpio_base->DATA)) >> pin_n) & 0x01;
}

uint32_t gpio_set(PinName pin)
{
    uint32_t port = BP6A_PORT_IDX(pin);
    uint32_t pin_n = BP6A_PIN_IDX(pin);

    bp6a_gpio_set_dir(port, pin_n, true);
    return (1u << (uint32_t) pin);
}

int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin == (PinName) NC) {
        return 0;
    }

    uint32_t port = BP6A_PORT_IDX(obj->pin);
    uint32_t pin_n = BP6A_PIN_IDX(obj->pin);

    BP_GPIO_TypeDef *gpio_base = (BP_GPIO_TypeDef *)GPIO_PORT_BASE(port);

    return !(getreg32(&(gpio_base->ALTFUNCSET)) >> pin_n);
}
