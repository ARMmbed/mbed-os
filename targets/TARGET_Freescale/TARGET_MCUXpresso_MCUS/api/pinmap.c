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
#include "fsl_port.h"

/* Array of PORT peripheral base address. */
static PORT_Type *const port_addrs[] = PORT_BASE_PTRS;

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    clock_ip_name_t port_clocks[] = PORT_CLOCKS;

    CLOCK_EnableClock(port_clocks[pin >> GPIO_PORT_SHIFT]);

    PORT_SetPinMux(port_addrs[pin >> GPIO_PORT_SHIFT], pin & 0xFF, (port_mux_t)function);
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t instance = pin >> GPIO_PORT_SHIFT;
    uint32_t pinName = pin & 0xFF;
    PORT_Type *base = port_addrs[instance];
    uint32_t reg = base->PCR[pinName];

    switch (mode) {
        case PullNone:
            /* Write 0 to the PE, PS and ISF bits */
            reg &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK);
            break;
        case PullDown:
            /* Write 0 to PS and ISF bits and 1 to the PE bit to enable the pull configuration */
            reg &= ~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK);
            reg |= PORT_PCR_PE_MASK;
            break;
        case PullUp:
            /* Write 0 ISF bit, 1 to the PE & PS bits to enable the pull up configuration */
            reg &= ~(PORT_PCR_ISF_MASK);
            reg |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
            break;
        default:
            break;
    }

    base->PCR[pinName] = reg;
}
