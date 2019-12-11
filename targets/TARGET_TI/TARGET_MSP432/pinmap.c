/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include <stdint.h>
#include "pinmap.h"
#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "gpio_msp432.h"

/*
 * Bitmask arrays for gpio mode configuration
 */
uint8_t open_drain[10]  = {0};
uint8_t open_source[10] = {0};
uint8_t pull_up[10]     = {0};
uint8_t pull_down[10]   = {0};

/*
 * Configure pin (SEL function and direction)
 */
void pin_function(PinName pin, int data)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint8_t port_index = MSP432_PORT_IDX(pin);
    uint8_t pin_index  = MSP432_PIN_IDX(pin);
    MBED_ASSERT(port_index < 10 && pin_index < 8);

    __IO uint8_t *pin_base = PIN_BASE(port_index, pin_index);

    // Set the SEL bits
    uint8_t sel = GET_DATA_SEL(data);
    PIN_REG(pin_base, REG_SEL0) = sel &  1;
    PIN_REG(pin_base, REG_SEL1) = sel >> 1;

    // Set gpio direction
    PIN_REG(pin_base, REG_DIR) = GET_DATA_DIR(data);
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint8_t port_index = MSP432_PORT_IDX(pin);
    uint8_t pin_index  = MSP432_PIN_IDX(pin);
    MBED_ASSERT(port_index < 10 && pin_index < 8);

    __IO uint8_t *pin_base = PIN_BASE(port_index, pin_index);

    uint8_t mask = 1 << pin_index;

    // Set mode to default state
    open_drain[port_index]  &= ~mask;
    open_source[port_index] &= ~mask;
    pull_up[port_index]     &= ~mask;
    pull_down[port_index]   &= ~mask;
    PIN_REG(pin_base, REG_REN) = 0;

    switch (mode) {
        case PullNone: {
            break;
        }
        case PullUp: {
            pull_up[port_index] |= mask;
            PIN_REG(pin_base, REG_REN) = 1;
            PIN_REG(pin_base, REG_OUT) = 1;
            break;
        }
        case PullDown: {
            pull_down[port_index] |= mask;
            PIN_REG(pin_base, REG_REN) = 1;
            PIN_REG(pin_base, REG_OUT) = 0;
            break;
        }
        case OpenDrain: {
            PIN_REG(pin_base, REG_DIR) = !PIN_REG(pin_base, REG_OUT);
            open_drain[port_index] |= mask;
            break;
        }
        case OpenDrainPullUp: {
            open_drain[port_index] |= mask;
            pull_up[port_index] |= mask;
            PIN_REG(pin_base, REG_REN) = 1;
            PIN_REG(pin_base, REG_DIR) = !PIN_REG(pin_base, REG_OUT);
            break;
        }
        case OpenSource: {
            open_source[port_index] |= mask;
            PIN_REG(pin_base, REG_DIR) = PIN_REG(pin_base, REG_OUT);
            break;
        }
        case OpenSourcePullDown: {
            open_source[port_index] |= mask;
            pull_down[port_index]   |= mask;
            PIN_REG(pin_base, REG_REN) = 1;
            PIN_REG(pin_base, REG_DIR) = PIN_REG(pin_base, REG_OUT);
            break;
        }
    }
}

