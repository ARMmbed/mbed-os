/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
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
#include "gpio_api.h"
#include "pinmap.h"
#include "error.h"

uint32_t gpio_set(PinName pin) {  
    if (pin == NC) return 0;

    pin_function(pin, STM_PIN_DATA(GPIO_Mode_IN, 0, GPIO_PuPd_NOPULL, 0));

    return (uint32_t)(1 << ((uint32_t)pin & 0xF)); // Return the pin mask
}

void gpio_init(gpio_t *obj, PinName pin, PinDirection direction) { 
    GPIO_TypeDef *gpio;
  
    if (pin == NC) return;

    uint32_t pin_number = (uint32_t)pin;
    uint32_t port_index = (pin_number >> 4);  

    // Get GPIO structure base address
    switch (port_index) {
        case PortA:
            gpio = (GPIO_TypeDef *)GPIOA_BASE;
            break;
        case PortB:
            gpio = (GPIO_TypeDef *)GPIOB_BASE;
            break;
        case PortC:
            gpio = (GPIO_TypeDef *)GPIOC_BASE;
            break;
        case PortD:
            gpio = (GPIO_TypeDef *)GPIOD_BASE;
            break;
        case PortH:
            gpio = (GPIO_TypeDef *)GPIOH_BASE;
            break;
        default:
            error("GPIO port number is not correct.");
            break;          
    }  
  
    // Fill GPIO object structure for future use
    obj->pin     = pin;
    obj->mask    = gpio_set(pin);
    obj->reg_in  = &gpio->IDR;
    obj->reg_set = &gpio->BSRRL;
    obj->reg_clr = &gpio->BSRRH;
  
    // Configure GPIO
    if (direction == PIN_OUTPUT) {
        pin_function(pin, STM_PIN_DATA(GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0));
    }
    else { // PIN_INPUT
        pin_function(pin, STM_PIN_DATA(GPIO_Mode_IN, 0, GPIO_PuPd_NOPULL, 0));
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    if (direction == PIN_OUTPUT) {
        pin_function(obj->pin, STM_PIN_DATA(GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, 0));
    }
    else { // PIN_INPUT
        pin_function(obj->pin, STM_PIN_DATA(GPIO_Mode_IN, 0, GPIO_PuPd_NOPULL, 0));
    }
}
