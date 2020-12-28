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

#include "cmsis.h"
#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

void set_alt(uint8_t port, uint8_t pin, uint8_t alt)
{
    switch (alt) {
        case ALT0 :
        case ALT1 :
            modifyreg32(GPIO_PORT_BASE(port) + GPIO_ALTFUNCSET_OFFSET,
                        0x01 << pin, 0x01 << pin);
            modifyreg32(GPIO_PORT_BASE(port) + GPIO_ALTFUNCSEL_OFFSET,
                        0x01 << pin, 0x01 << pin);
            modifyreg32(GPIO_PORT_BASE(port) + GPIO_ALTFUNCSEL_OFFSET,
                        0x1 << (pin), alt << (pin));
            break;
        default:
            break;
    }
}

void set_pullup(uint8_t port, uint8_t pin, uint8_t pullup)
{
    if (pullup == PullNone) {
        return;
    }

    uint8_t idx = port * 4 + pin / 4 + 2;
    uint8_t bit = (pin << 3) % 32;

    modifyreg32(&BP_SYSCON->IOCFG[idx], 0x1C << bit, pullup << bit);
}

void set_drvstr(uint8_t port, uint8_t pin, uint8_t str)
{
    modifyreg32(&BP_SYSCON->IOCFG_GENERAL, 0x3, str);
}

void pin_function(PinName pin, int function)
{
    uint8_t port_n = BP6A_PORT_IDX(pin);
    uint8_t pin_n =  BP6A_PIN_IDX(pin);

    MBED_ASSERT(port_n < 3 && pin_n < 16);

    bp6a_set_pin_config(port_n, pin_n, GET_PIN_DIR(function));
    set_alt(port_n, pin_n, GET_PIN_SEL(function));
    set_pullup(port_n, pin_n, GET_PIN_MODE(function));

}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint8_t port_n = BP6A_PORT_IDX(pin);
    uint8_t pin_n =  BP6A_PIN_IDX(pin);

    set_pullup(port_n, pin_n, mode);
}
