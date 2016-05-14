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
 
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"

extern uint32_t Get_GPIO_BaseAddress(uint32_t port_idx);

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);

    pin_function(pin, WIZ_PIN_DATA(WIZ_MODE_INPUT, WIZ_GPIO_NOPULL, Px_AFSR_AF1));

    return (uint32_t)(1 << ((uint32_t)pin & 0xF)); // Return the pin mask
}

void gpio_init(gpio_t *obj, PinName pin)
{
    if (pin == (PinName)NC) {
        return;
    }
    
    obj->port_num = WIZ_PORT(pin);
    obj->pin_index    = WIZ_PIN_INDEX(pin);
    obj->pin = pin;
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(obj->port_num);
    obj->reg_data_in    = &gpio->DATA;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
    obj->mode = mode;
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    obj->direction = direction;

    pin_function(obj->pin, WIZ_PIN_DATA(obj->direction, obj->mode, 1));
}
