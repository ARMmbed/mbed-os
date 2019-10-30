/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/* Low-level implementation of gpio functionality for MSP432.
 * This implementation uses bit-banding, so no masking of bits
 * is necessary during simple get/set operations for a single pin.
 */

#include <stdint.h>
#include "gpio_api.h"
#include "mbed_assert.h"
#include "gpio_msp432.h"

extern uint8_t open_drain[10];
extern uint8_t open_source[10];
extern uint8_t pull_up[10];
extern uint8_t pull_down[10];

/* Helper macro to access single bits in the
 * bitmap arrays above
 */
#define FLAG_SET(array) (array[obj->port_index] & obj->mask)

/** Set the given pin as GPIO
 *
 * @param pin The pin to be set as GPIO
 * @return The GPIO port mask for this pin
 **/
uint32_t gpio_set(PinName pin)
{
    pin_function(pin, MSP432_PIN_DATA(SEL0, PIN_INPUT, PullNone, 0));
    return 1 << MSP432_PIN_IDX(pin);
}

/* Checks if gpio object is connected (pin was not initialized with NC)
 * @param pin The pin to be set as GPIO
 * @return 0 if port is initialized with NC
 **/
int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin_base != 0;
}

/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize
 */
void gpio_init(gpio_t *obj, PinName pin)
{
    if (pin == (PinName)NC) {
        obj->pin_base = 0;
        return;
    }
    uint8_t port_index = MSP432_PORT_IDX(pin);
    uint8_t pin_index  = MSP432_PIN_IDX(pin);
    if (port_index > 9 || pin_index > 7) {
        obj->pin_base = 0;
        return;
    }
    /* Initialize the gpio_t object */
    obj->pin        = pin;
    obj->mask       = gpio_set(pin);
    obj->port_index = port_index;
    obj->pin_base = PIN_BASE(port_index, pin_index);

    /* Initialize the gpio flags */
    open_drain[port_index]  &= ~obj->mask;
    open_source[port_index] &= ~obj->mask;
    pull_up[port_index]     &= ~obj->mask;
    pull_down[port_index]   &= ~obj->mask;
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

/** Set the pin direction
 *
 * @param obj       The GPIO object
 * @param direction The pin direction to be set
 */
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    PIN_REG(obj->pin_base, REG_DIR) = direction;
}

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
void gpio_write(gpio_t *obj, int value)
{
    if (FLAG_SET(open_drain)) {
        if (FLAG_SET(pull_up)) {
            if (value) {
                PIN_REG(obj->pin_base, REG_DIR) = 0;
                PIN_REG(obj->pin_base, REG_OUT) = 1;
            } else {
                PIN_REG(obj->pin_base, REG_OUT) = 0;
                PIN_REG(obj->pin_base, REG_DIR) = 1;
            }
        } else {
            // Open drain without pullup
            PIN_REG(obj->pin_base, REG_DIR) = !value;
        }
    } else if (FLAG_SET(open_source)) {
        if (FLAG_SET(pull_down)) {
            if (value) {
                PIN_REG(obj->pin_base, REG_OUT) = 1;
                PIN_REG(obj->pin_base, REG_DIR) = 1;
            } else {
                PIN_REG(obj->pin_base, REG_DIR) = 0;
                PIN_REG(obj->pin_base, REG_OUT) = 0;
            }
        } else {
            // Open source without pull down
            PIN_REG(obj->pin_base, REG_DIR) = value;
        }
    } else {
        // Normal push/pull operation
        PIN_REG(obj->pin_base, REG_OUT) = value;
    }
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
int gpio_read(gpio_t *obj)
{
    return PIN_REG(obj->pin_base, REG_IN);
}

