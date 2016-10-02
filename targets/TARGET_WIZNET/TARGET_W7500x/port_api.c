/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
 
#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "mbed_error.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

extern uint32_t Get_GPIO_BaseAddress(uint32_t port_idx);

//port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, ...)
PinName port_pin(PortName port, int pin_n)
{
    int af_num = 0;

    if( (port == 2) && ((pin_n == 0) || (pin_n == 1) || (pin_n == 4) || (pin_n == 8) || (pin_n == 9)) )
    {
        af_num = 1;
    }


    return (PinName)(pin_n + (port << 4) + (af_num << 8));
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint32_t port_num = (uint32_t)port;

    // Enable GPIO clock
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(port_num);
    

    // Fill PORT object structure for future use
    obj->port      = port;
    obj->mask      = mask;
    obj->direction = dir;
    obj->reg_in    = &gpio->DATA;
    obj->reg_out   = &gpio->DATAOUT;

    port_dir(obj, dir);
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint32_t i;
    obj->direction = dir;
    for (i = 0; i < 16; i++) { // Process all pins
        if (obj->mask & (1 << i)) { // If the pin is used
            if (dir == PIN_OUTPUT) {
                pin_function(port_pin(obj->port, i), WIZ_PIN_DATA(WIZ_MODE_OUTPUT, WIZ_GPIO_NOPULL, 0));
            } else { // PIN_INPUT
                pin_function(port_pin(obj->port, i), WIZ_PIN_DATA(WIZ_MODE_INPUT, WIZ_GPIO_NOPULL, 0));
            }
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    for (i = 0; i < 16; i++) { // Process all pins
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_write(port_t *obj, int value)
{
    *obj->reg_out = (*obj->reg_out & ~obj->mask) | (value & obj->mask);
}

int port_read(port_t *obj)
{
    if (obj->direction == PIN_OUTPUT) {
        return (*obj->reg_out & obj->mask);
    } else { // PIN_INPUT
        return (*obj->reg_in & obj->mask);
    }
}

#endif
