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
#include "gpio_api.h"
#include "pinmap.h"

uint32_t gpio_set(PinName pin) {  
    if (pin == NC) return 0;

    pin_function(pin, STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0));

    return (uint32_t)(1 << ((uint32_t)pin & 0xF)); // Return the pin mask
}

void gpio_init(gpio_t *obj, PinName pin, PinDirection direction) {    
    if (pin == NC) return;

    // Get GPIO structure base address
    uint32_t pin_number = (uint32_t)pin;
    uint32_t port_index = (pin_number >> 4);  
    GPIO_TypeDef *gpio  = (GPIO_TypeDef *)(GPIOA_BASE + (port_index << 10));
  
    // Fill GPIO object structure for future use
    obj->pin     = pin;
    obj->mask    = gpio_set(pin);
    obj->reg_in  = &gpio->IDR;
    obj->reg_set = &gpio->BSRR;
    obj->reg_clr = &gpio->BRR;    
  
    // Configure GPIO
    if (direction == PIN_OUTPUT) {
        pin_function(pin, STM_PIN_DATA(GPIO_Mode_Out_PP, 0));
    }
    else { // PIN_INPUT
        pin_function(pin, STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0));
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    if (direction == PIN_OUTPUT) {
        pin_function(obj->pin, STM_PIN_DATA(GPIO_Mode_Out_PP, 0));
    }
    else { // PIN_INPUT
        pin_function(obj->pin, STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0));
    }
}
