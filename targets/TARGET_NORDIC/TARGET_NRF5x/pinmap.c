/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_error.h"
#include "pinmap.h"
#include "nrf_gpio.h"

void pin_function(PinName pin, int function)
{
    /* Avoid compiler warnings */
    (void) pin;
    (void) function;
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t pin_number = (uint32_t)pin;

    NRF_GPIO_Type * reg = nrf_gpio_pin_port_decode(&pin_number);

    reg->PIN_CNF[pin_number] &= ~GPIO_PIN_CNF_PULL_Msk;
    reg->PIN_CNF[pin_number] |= (mode << GPIO_PIN_CNF_PULL_Pos);
}
