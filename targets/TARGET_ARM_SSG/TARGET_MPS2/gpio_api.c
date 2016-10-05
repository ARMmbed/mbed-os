/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

// function to enable the GPIO pin
uint32_t gpio_set(PinName pin) {
    return (1);
}

//function to initialise the gpio pin
// this links the board control bits for each pin 
// with the object created for the pin
void gpio_init(gpio_t *obj, PinName pin) {
        if(pin == NC){ return;}
        else {
        int pin_value = 0;
        obj->pin = pin;
        if(pin <=15){
            pin_value = pin;
        }else if (pin >= 16 && pin <= 31){
            pin_value = pin-16;
        }else if (pin >= 32 && pin <= 47){
            pin_value = pin-32;
        }else if (pin >= 48 && pin <= 51){
            pin_value = pin-48;
        }else if (pin == 100 || pin == 101){
            pin_value = pin-100;
        }else if (pin == 110 || pin == 111){
            pin_value = pin-110;
        }else if (pin >= 200 && pin <= 207){
            pin_value = pin-200;
        }else if (pin >= 210 && pin <= 217){
            pin_value = pin-210;
        }else if (pin == 303){
            pin_value = pin-302;
        }else if (pin == 307){
            pin_value = pin-307;
        }else if (pin == 308){
            pin_value = pin-305;
        }else if (pin == 309){
            pin_value = pin-305;
        }else if (pin == 310){
            pin_value = pin-305;
        }else if (pin == 311){
            pin_value = pin-305;
        }else if (pin == 323){
            pin_value = pin-315;
        }else if (pin == 334){
            pin_value = pin-325;
        }else if (pin == 653){
            pin_value = pin-646;
        }
        
        obj->mask = 0x1 << pin_value;
        obj->pin_number = pin;
        if(pin <=15) {
            obj->reg_data = &CMSDK_GPIO0->DATAOUT ;
            obj->reg_in  =         &CMSDK_GPIO0->DATA ;
            obj->reg_dir =         &CMSDK_GPIO0->OUTENSET ;
            obj->reg_dirclr = &CMSDK_GPIO0->OUTENCLR ;
        } else if (pin >= 16 && pin <= 31){
            obj->reg_data = &CMSDK_GPIO1->DATAOUT ;
            obj->reg_in  =         &CMSDK_GPIO1->DATA ;
            obj->reg_dir =         &CMSDK_GPIO1->OUTENSET ;
            obj->reg_dirclr = &CMSDK_GPIO1->OUTENCLR ;
        }
        
        if (pin == 323){
            CMSDK_GPIO0->ALTFUNCSET |= 0x1000;
        }else if (pin == 653){
            CMSDK_GPIO1->ALTFUNCSET |= 0x0001;
        }
    }
}

void gpio_mode(gpio_t *obj, PinMode mode) {
        pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
        if(obj->pin >= 0 && obj->pin <= 51)
            {        
        switch (direction) {
          case PIN_INPUT : *obj->reg_dirclr = obj->mask;  break;
          case PIN_OUTPUT: *obj->reg_dir |= obj->mask; break;
        }
    } else {return;}
}

int gpio_is_connected(const gpio_t *obj){
    if(obj->pin != (PinName)NC){
        return 1;
    } else {
        return 0;
    }
}

