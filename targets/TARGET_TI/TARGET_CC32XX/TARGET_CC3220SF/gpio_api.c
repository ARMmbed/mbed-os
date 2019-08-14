/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/pin.h"
#include "ti/devices/cc32xx/driverlib/gpio.h"
#include "ti/devices/cc32xx/inc/hw_ints.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"


uint32_t gpio_set(PinName pin)
{
    pin_function(pin, 0);
    return (1);
}

// function to initialise the gpio pin
// this links the board control bits for each pin
// with the object created for the pin
void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC) {
        return;
    }

    unsigned long gpio_base = (unsigned long)pinmap_peripheral(pin, PinMap_GPIO);
    obj->baseAddr = gpio_base;
    obj->pin_mask = 1 << (pinmap_find_function(pin, PinMap_GPIO) % 8);

    // determine PRCM GPIO CLOCK index
    unsigned short prcm_peripheral = 0;
    switch (gpio_base) {
        case CC3220SF_GPIOA0_BASE:
            prcm_peripheral = PRCM_GPIOA0;
            break;
        case CC3220SF_GPIOA1_BASE:
            prcm_peripheral = PRCM_GPIOA1;
            break;
        case CC3220SF_GPIOA2_BASE:
            prcm_peripheral = PRCM_GPIOA2;
            break;
        case CC3220SF_GPIOA3_BASE:
            prcm_peripheral = PRCM_GPIOA3;
            break;
        default:
            break;
    }

    // initialize GPIO PORT clock
    PRCMPeripheralClkEnable(prcm_peripheral, PRCM_RUN_MODE_CLK | PRCM_SLP_MODE_CLK);

    // wait for GPIO clock to settle
    while (!PRCMPeripheralStatusGet(prcm_peripheral));
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    obj->mode = mode;
    //set the pin mux to be GPIO which is PIN MODE 0
    pin_mode(obj->pin, mode);
    PinModeSet(obj->pin, PIN_MODE_0);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    obj->dir = direction;
    GPIODirModeSet(obj->baseAddr, obj->pin_mask, direction);
}

int gpio_is_connected(const gpio_t *obj)
{
    return (obj->pin == NC);
}

void gpio_write(gpio_t *obj, int value)
{
    GPIOPinWrite(obj->baseAddr, obj->pin_mask, value * obj->pin_mask);
}

int gpio_read(gpio_t *obj)
{
    return (GPIOPinRead(obj->baseAddr, obj->pin_mask) != 0);
}
