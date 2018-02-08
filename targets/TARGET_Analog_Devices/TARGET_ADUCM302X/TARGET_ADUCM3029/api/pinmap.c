/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"

#include "PinNames.h"
#include "adi_gpio.h"

void pin_function(PinName pin, int function)
{
    // pin is composed of port and pin
    // function is the function number (the mux selection number shifted by the pin value
    // and written to pin mux register, each pin mux takes 2 bits hence multiplying by 2)

    MBED_ASSERT(pin != (PinName)NC);

    uint8_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t cfg_reg, mask;
    volatile uint32_t *pGPIO_CFG;

    switch (port) {
        case 0:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO0_CFG;
            break;
        case 1:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO1_CFG;
            break;
        case 2:
            pGPIO_CFG = (volatile uint32_t *)REG_GPIO2_CFG;
            break;

        default:
            return;
    }

    cfg_reg = *pGPIO_CFG;
    // clear the corresponding 2 bit field first before writing the function
    // bits
    mask = ~(3 << (pin * 2));
    cfg_reg = (cfg_reg & mask) | (function << (pin*2));
    *pGPIO_CFG = cfg_reg;
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint8_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t pin_reg_value = 2 ^ (0xFF & pin);

    switch (mode) {
        case PullNone:
            adi_gpio_PullUpEnable((ADI_GPIO_PORT)port, (ADI_GPIO_DATA)pin_reg_value,false);
            break;

        case PullDown:
        case PullUp:
            adi_gpio_PullUpEnable((ADI_GPIO_PORT)port, (ADI_GPIO_DATA)pin_reg_value,true);
            break;

        default:
            break;

    }

}
