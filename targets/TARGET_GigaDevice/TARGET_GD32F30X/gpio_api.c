/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "gd32f30x_gpio.h"
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"

extern const int GD_GPIO_REMAP[];
extern const int GD_GPIO_SPEED[];
extern const int GD_GPIO_MODE[];

/* Enable GPIO clock and return GPIO base address */
uint32_t gpio_clock_enable(uint32_t port_idx)
{
    uint32_t gpio_add = 0;
    switch (port_idx) {
        case PORTA:
            gpio_add = GPIOA;
            rcu_periph_clock_enable(RCU_GPIOA);
            break;
        case PORTB:
            gpio_add = GPIOB;
            rcu_periph_clock_enable(RCU_GPIOB);
            break;
        case PORTC:
            gpio_add = GPIOC;
            rcu_periph_clock_enable(RCU_GPIOC);
            break;
        case PORTD:
            gpio_add = GPIOD;
            rcu_periph_clock_enable(RCU_GPIOD);
            break;
        case PORTE:
            gpio_add = GPIOE;
            rcu_periph_clock_enable(RCU_GPIOE);
            break;
        default:
            error("port number not exist");
            break;
    }
    return gpio_add;
}

/** Set the given pin as GPIO
 *
 * @param pin The pin to be set as GPIO
 * @return The GPIO port mask for this pin
 */
uint32_t gpio_set(PinName pin)
{

    MBED_ASSERT(pin != (PinName)NC);
    pin_function(pin, MODE_IN_FLOATING);
    /* return pin mask */
    return (uint32_t)(1 << ((uint32_t)pin & 0xF));
}

/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize
 */
void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC) {
        return;
    }
    /* fill struct parameter for future use */
    uint32_t port_index = GD_PORT_GET(pin);
    uint32_t gpio = gpio_clock_enable(port_index);
    obj->mask    = gpio_set(pin);
    obj->gpio_periph  = gpio;
}

/** Set the input pin mode
 *
 * @param obj  The GPIO object
 * @param mode The pin mode to be set
 */
void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
void gpio_write(gpio_t *obj, int value)
{
    /* set or reset GPIO pin */
    if (value) {
        GPIO_BOP(obj->gpio_periph) = (1 << (uint32_t)GD_PIN_GET(obj->pin));
    } else {
        GPIO_BC(obj->gpio_periph) = (1 << (uint32_t)GD_PIN_GET(obj->pin));
    }
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
int gpio_read(gpio_t *obj)
{
    /* return state of GPIO pin */
    return ((GPIO_ISTAT(obj->gpio_periph) & obj->mask) ? 1 : 0);
}

/* Checks if gpio object is connected (pin was not initialized with NC)
 * @param pin The pin to be set as GPIO
 * @return 0 if port is initialized with NC
 **/
int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != (PinName)NC;
}

/** Set the pin direction
 *
 * @param obj       The GPIO object
 * @param direction The pin direction to be set
 */
void gpio_dir(gpio_t *obj, PinDirection direction)
{

    /* config GPIO pin as input or output */
    if (direction == PIN_INPUT) {
        gpio_para_init(obj->gpio_periph, GD_GPIO_MODE[MODE_IN_FLOATING], GD_GPIO_SPEED[0], (1 << (uint32_t)GD_PIN_GET(obj->pin)));
    } else {
        gpio_para_init(obj->gpio_periph, GD_GPIO_MODE[MODE_OUT_PP], GD_GPIO_SPEED[0], (1 << (uint32_t)GD_PIN_GET(obj->pin)));
    }
}
