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
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "adi_gpio.h"

#define MUX_FUNC_0 0x0

extern uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];
extern uint8_t gpio_initialized;

uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_num = pin & 0xFF;

    pin_function(pin, MUX_FUNC_0);

    return (1 << pin_num);
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;

    if (pin == (PinName)NC)
        return;

    if (!gpio_initialized)
        adi_gpio_Init(gpioMemory, ADI_GPIO_MEMORY_SIZE);

    pin_function(pin, MUX_FUNC_0);
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    uint32_t pin = obj->pin;

    pin_mode((PinName)pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    MBED_ASSERT(obj->pin != (PinName)NC);
	uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;

	adi_gpio_InputEnable(port, 1 << pin_num, true);
    if (direction ==  PIN_OUTPUT)
        adi_gpio_OutputEnable(port, 1 << pin_num, true);
}

void gpio_write(gpio_t *obj, int value) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;

    if (value & 1) {
        adi_gpio_SetHigh(port, (1 << pin_num));
	}
	else {
	    adi_gpio_SetLow(port, (1 << pin_num));
	}
}

int gpio_read(gpio_t *obj) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;
    uint16_t Data;
    uint16_t Temp;

    // check whether the pin is configured as input or output
    adi_gpio_GetOutputEnable(port, (1 << pin_num), &Temp);

    // if configured as output, call GetOutputData
    // otherwise call GetData
    if ((1 << pin_num) & Temp) {
        adi_gpio_GetOutputData(port, (1 << pin_num), &Data);
    }
    else {
        adi_gpio_GetData(port, (1 << pin_num), &Data);
    }

    return ((((uint32_t)Data) >> pin_num) & 1);
}
