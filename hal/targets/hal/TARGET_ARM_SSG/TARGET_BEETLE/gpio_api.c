/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "gpio_api.h"
#include "pinmap.h"

// function to enable the GPIO pin
uint32_t gpio_set(PinName pin) {
    uint32_t pin_value = 0;

    if(pin <= 15) {
        pin_value = pin;
    } else if (pin >= 16 && pin <= 31) {
        pin_value = pin-16;
    } else if (pin >= 1001 && pin <= 1004) {
        /* Emulated LEDs */
        return (1);
    }

    pin_function(pin, 0);

    return (1 << pin_value);
}

//function to initialise the gpio pin
// this links the board control bits for each pin
// with the object created for the pin
void gpio_init(gpio_t *obj, PinName pin) {
    if (pin == NC) {
        return;
    } else {
        int pin_value = 0;
        obj->pin = pin;
        if (pin <=15) {
            pin_value = pin;
        } else if (pin >= 16 && pin <= 31) {
            pin_value = pin-16;
        } else if (pin >= 1001 && pin <= 1004) {
            /* Emulated LEDs */
            return;
        }

        obj->mask = 0x1 << pin_value;
        obj->pin_number = pin;
        if (pin <=15) {
            obj->reg_data = &CMSDK_GPIO0->DATAOUT;
            obj->reg_in = &CMSDK_GPIO0->DATA;
            obj->reg_dir = &CMSDK_GPIO0->OUTENABLESET;
            obj->reg_dirclr = &CMSDK_GPIO0->OUTENABLECLR;
        } else if (pin >= 16 && pin <= 31) {
            obj->reg_data = &CMSDK_GPIO1->DATAOUT;
            obj->reg_in = &CMSDK_GPIO1->DATA;
            obj->reg_dir = &CMSDK_GPIO1->OUTENABLESET;
            obj->reg_dirclr = &CMSDK_GPIO1->OUTENABLECLR;
        }
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    if(obj->pin >= 0 && obj->pin <= 31) {
        switch (direction) {
            case PIN_INPUT : *obj->reg_dirclr = obj->mask; break;
            case PIN_OUTPUT: *obj->reg_dir |= obj->mask; break;
        }
    } else {
        return;
    }
}

int gpio_is_connected(const gpio_t *obj){
    if(obj->pin != (PinName)NC){
        return 1;
    } else {
        return 0;
    }
}
