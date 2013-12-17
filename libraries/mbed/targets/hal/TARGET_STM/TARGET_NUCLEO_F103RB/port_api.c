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
#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

// high nibble = port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, ...)
// low nibble  = pin number
PinName port_pin(PortName port, int pin_n) {
  return (PinName)(pin_n + (port << 4));
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir) {

    uint32_t port_index = (uint32_t)port; // (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, ...)

    // Enable GPIO clock
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA << port_index), ENABLE);

    // Get GPIO structure base address
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)(GPIOA_BASE + (port_index << 10));

    // Fill PORT object structure for future use
    obj->port      = port;
    obj->mask      = mask;
    obj->direction = dir;  
    obj->reg_in    = &gpio->IDR;
    obj->reg_out   = &gpio->ODR;  

    port_dir(obj, dir);
}

void port_dir(port_t *obj, PinDirection dir) {
    uint32_t i;
    obj->direction = dir;
    for (i = 0; i < 16; i++) { // Process all pins
        if (obj->mask & (1 << i)) { // If the pin is used
            if (dir == PIN_OUTPUT) {
                pin_function(port_pin(obj->port, i), STM_PIN_DATA(GPIO_Mode_Out_PP, 0));
            }
            else { // PIN_INPUT
                pin_function(port_pin(obj->port, i), STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0));
            }
        }
    }  
}

void port_mode(port_t *obj, PinMode mode) {
    uint32_t i;  
    for (i = 0; i < 16; i++) { // Process all pins
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_write(port_t *obj, int value) {
    *obj->reg_out = (*obj->reg_out & ~obj->mask) | (value & obj->mask);
}

int port_read(port_t *obj) {
    if (obj->direction == PIN_OUTPUT) {
        return (*obj->reg_out & obj->mask);
    }
    else { // PIN_INPUT
        return (*obj->reg_in & obj->mask);
    }
}

#endif
