/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef MBED_GPIO_OBJECTS_H
#define MBED_GPIO_OBJECTS_H

#include "mbed_assert.h"
#include "PinNamesTypes.h"
#include "PinNames.h"
#include "PortNames.h"
#include "cyhal_gpio.h"
#include "cyhal_pin_package.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    cyhal_gpio_t pin;
    cyhal_gpio_direction_t direction;
    cyhal_gpio_drive_mode_t drive_mode;
    int output_val;
} gpio_t;

struct gpio_irq_s {
    cyhal_gpio_t pin;
    void *handler;
    uint32_t id;
    cyhal_gpio_irq_event_t mask;
};

struct port_s {
    PortName port;
    uint8_t mask;
    cyhal_gpio_direction_t direction;
    cyhal_gpio_drive_mode_t drive_mode;
};

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
static inline void gpio_write(gpio_t *obj, int value)
{
    if (obj->direction != CYHAL_GPIO_DIR_INPUT) {
        MBED_ASSERT(obj->pin != CYHAL_NC_PIN_VALUE);
        cyhal_gpio_write(obj->pin, value != 0);
    } else {
        obj->output_val = value;
    }
}

/** Set the pull value
 *
 * @param obj   The GPIO object
 * @param value The pull value to be set
 */
static inline void gpio_set_pull(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != CYHAL_NC_PIN_VALUE);
    cyhal_gpio_write(obj->pin, value != 0);
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
static inline int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != CYHAL_NC_PIN_VALUE);
    return cyhal_gpio_read(obj->pin);
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != CYHAL_NC_PIN_VALUE;
}

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 */
static inline PinName port_pin(PortName port, int pin_n)
{
    return (PinName)CYHAL_GET_GPIO(port, pin_n);
}


#ifdef __cplusplus
}
#endif

#endif // MBED_GPIO_OBJECTS_H
