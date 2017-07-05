/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

uint32_t gpio_set(PinName pin)
{
    if (pin == (PinName) NC) {
        return 0;
    }
    
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    
    // GPIO PinMap
    pin_function(pin, 0 << NU_MFP_POS(pin_index));

    return (uint32_t)(1 << pin_index);    // Return the pin mask
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    
    if (obj->pin == (PinName) NC) {
        return;
    }

    obj->mask = gpio_set(pin);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    if (obj->pin == (PinName) NC) {
        return;
    }
    
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (obj->pin == (PinName) NC) {
        return;
    }
    
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    
    uint32_t mode_intern = GPIO_PMD_INPUT;
    
    switch (direction) {
        case PIN_INPUT:
            mode_intern = GPIO_PMD_INPUT;
            break;
        
        case PIN_OUTPUT:
            mode_intern = GPIO_PMD_OUTPUT;
            break;
            
        default:
            return;
    }
    
    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);
}
