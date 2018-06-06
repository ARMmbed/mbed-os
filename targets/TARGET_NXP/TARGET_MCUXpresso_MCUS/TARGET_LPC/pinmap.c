/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_error.h"
#include "fsl_common.h"

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    clock_ip_name_t gpio_clocks[] = GPIO_CLOCKS;
    uint32_t pin_number = pin & 0x1F;
    uint8_t port_number = pin / 32;
    uint32_t reg;

    CLOCK_EnableClock(gpio_clocks[port_number]);
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Set the DIGIMODE bit */
    IOCON->PIO[port_number][pin_number] |= IOCON_PIO_DIGIMODE_MASK;

    reg = IOCON->PIO[port_number][pin_number];
    reg = (reg & ~0x7) | (function & 0x7);
    IOCON->PIO[port_number][pin_number] = reg;
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_number = pin & 0x1F;
    uint8_t port_number = pin / 32;
    uint32_t reg = IOCON->PIO[port_number][pin_number] & ~IOCON_PIO_MODE_MASK;

    switch (mode) {
        case PullNone:
            break;
        case PullDown:
            reg |= (1 << IOCON_PIO_MODE_SHIFT);
            break;
        case PullUp:
            reg |= (2 << IOCON_PIO_MODE_SHIFT);
            break;
        default:
            break;
    }

    IOCON->PIO[port_number][pin_number] = reg;
}
