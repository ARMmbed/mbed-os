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

/***************************************************************** Includes **/
#include "gpio_api.h"
#include "cmsis.h"
#include "xmc4_helper.h"

/******************************************************* Internal Functions **/

uint32_t gpio_set(PinName pin)
{


    return (uint32_t)(0x1U << ((uint32_t)pin & 0xF));
}

int gpio_is_connected(const gpio_t *obj)
{

    return obj->pin != (PinName)NC;
}

void gpio_init(gpio_t *obj, PinName pin)
{

    if (pin == (PinName)NC) {
        return;
    }

    /* Get PORT Base of Port Group */
    port_type_t *port = (port_type_t *)(PORT0_BASE + (uint32_t)((pin & 0xF0) << 4));

    obj->pin  = pin;
    obj->mask = gpio_set(pin);
    obj->OMR  = &port->OMR;
    obj->IN   = &port->IN;
    obj->IOCR = &port->IOCR[(pin & 0x0F) >> 2];

    /* Enable digital if analog => Port14 & Port15 */
    if(((pin & 0xF0) == 0xF0) || (pin & 0xF0) == 0xE0){
        port->PDISC &= ~(uint32_t)((uint32_t)0x1U << (pin & 0x0F));
    }
}

void gpio_mode(gpio_t *obj, PinMode mode)
{

    /* Set mode */
    *obj->IOCR |= (uint32_t)mode << ((uint32_t)8U * ((uint32_t)(obj->pin & 0x0F) & 0x3U));
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{

    switch(direction) {
        case PIN_INPUT:
            gpio_mode(obj, (PinMode)INPUT_PULL_UP);
            break;
        case PIN_OUTPUT:
            gpio_mode(obj, (PinMode)OUTPUT_PUSH_PULL);
            break;
        case PIN_INPUT_OUTPUT:
            gpio_mode(obj, (PinMode)INPUT_TRISTATE);
            break;
        default:
            gpio_mode(obj, (PinMode)PullDefault);
            break;
    }
}

void gpio_write(gpio_t *obj, int value)
{

    if(value) {
        *obj->OMR = (uint32_t)0x1U << (obj->pin & 0x0F);
    } else {
        *obj->OMR = (uint32_t)0x10000U << (obj->pin & 0x0F);
    }
}

int gpio_read(gpio_t *obj)
{

    return (((*obj->IN) >> (obj->pin & 0x0F)) & 0x1U);
}

/*EOF*/
