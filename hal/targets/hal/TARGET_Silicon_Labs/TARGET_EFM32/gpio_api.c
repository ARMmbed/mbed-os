/***************************************************************************//**
 * @file gpio_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "gpio_api.h"
#include "pinmap.h"
#include "em_cmu.h"
#include "mbed_assert.h"
#include "sleepmodes.h"


void gpio_write(gpio_t *obj, int value)
{
    if (value) {
        GPIO_PinOutSet((GPIO_Port_TypeDef)(obj->pin >> 4 & 0xF), obj->pin & 0xF); // Pin number encoded in first four bits of obj->pin
    } else {
        GPIO_PinOutClear((GPIO_Port_TypeDef)(obj->pin >> 4 & 0xF), obj->pin & 0xF);
    }
}

int gpio_read(gpio_t *obj)
{
    if (obj->dir == PIN_INPUT) {
        return GPIO_PinInGet((GPIO_Port_TypeDef)(obj->pin >> 4 & 0xF), obj->pin & 0xF); // Pin number encoded in first four bits of obj->pin
    } else {
        return GPIO_PinOutGet((GPIO_Port_TypeDef)(obj->pin >> 4 & 0xF), obj->pin & 0xF);
    }
}

int gpio_is_connected(const gpio_t *obj)
{
    return ((uint32_t)obj->pin | 0xFFFFFF00 ) != (uint32_t)((PinName)NC);
}

/*
 * @return the GPIO port mask for this pin
 * Pin and port index encoded in one uint32.
 * First four bits represent the pin number
 * The remaining bits represent the pin mode
 */
uint32_t gpio_set(PinName pin)
{
    return 1 << ((uint32_t) pin & 0xF);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    MBED_ASSERT(pin != NC);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    obj->pin = pin;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    uint32_t pin = 1 << (obj->pin & 0xF);
    uint32_t port = (obj->pin >> 4) & 0xF;

    if(obj->dir == PIN_INPUT) {
        switch(mode) {
            case PullDefault:
                mode = Input;
                break;
            case PullUp:
                mode = InputPullUp;
                break;
            case PullDown:
                mode = InputPullDown;
                break;
            default:
                break;
        }

        //Handle DOUT setting
        if((mode & 0x10) != 0) {
            //Set DOUT
#ifdef _GPIO_P_DOUTSET_MASK
            GPIO->P[port].DOUTSET = pin;
#else
            GPIO->P[port].DOUT |= pin;
#endif
        } else {
            //Clear DOUT
#ifdef _GPIO_P_DOUTCLR_MASK
            GPIO->P[port].DOUTCLR = pin;
#else
            GPIO->P[port].DOUT &= ~pin;
#endif
        }
    } else {
        switch(mode) {
            case PullDefault:
                mode = PushPull;
                break;
            case PullUp:
                mode = WiredAndPullUp;
                break;
            case PullDown:
                mode = WiredOrPullDown;
                break;
            default:
                break;
        }
    }

    obj->mode = mode; // Update object
    pin_mode(obj->pin, mode); // Update register
}

// Used by DigitalInOut to set correct mode when direction is set
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    obj->dir = direction;
    switch (direction) {
        case PIN_INPUT:
            gpio_mode(obj, PullDefault);
            break;
        case PIN_OUTPUT:
            gpio_mode(obj, PullNone);
            break;
    }
}
