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
/*
uint32_t gpio_set(PinName pin) {
    pin_function(pin, 1);
    return 1 << ((pin & 0x7F) >> 2);
}
*/
void gpio_init(gpio_t *obj, PinName pin, PinDirection direction) {
    if(pin == NC) return;

    obj->pin = pin;
    obj->mask = (1<<pin);//gpio_set(pin);

    unsigned int port = (unsigned int)pin/8;//(unsigned int)pin >> PORT_SHIFT;

   // FGPIO_Type *reg = (FGPIO_Type *)(FPTA_BASE + port * 0x40);
    obj->reg_set = &NRF_GPIO->OUTSET;// &reg->PSOR;
    obj->reg_clr = &NRF_GPIO->OUTCLR;//&reg->PCOR;
    obj->reg_in  = &NRF_GPIO->IN;//&reg->PDIR;
    obj->reg_dir = &NRF_GPIO->DIR;//&reg->PDDR;

    gpio_dir(obj, direction);
    switch (direction) {
        case PIN_OUTPUT: pin_mode(pin, PullNone); break;
        case PIN_INPUT : pin_mode(pin, PullUp); break;
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    switch (direction) {
        case PIN_INPUT : *obj->reg_dir &= ~obj->mask; break;
        case PIN_OUTPUT: *obj->reg_dir |=  obj->mask; break;
    }
}
/*
void gpio_write(gpio_t *obj, int value) {
    if (value)
        *obj->reg_set = obj->mask;
    else
        *obj->reg_clr = obj->mask;
}
 
int gpio_read(gpio_t *obj) {
    return ((*obj->reg_in & obj->mask) ? 1 : 0);
}*/

