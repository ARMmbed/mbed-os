/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "cy_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_s {
    GPIO_PRT_Type *port;
    PinName pin;
    PinDirection dir;
    PinMode mode;
};

typedef struct gpio_s   gpio_t;

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
static inline void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != NC);

    Cy_GPIO_Write(obj->port, CY_PIN(obj->pin), value);
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
static inline int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != NC);

    return Cy_GPIO_Read(obj->port, CY_PIN(obj->pin));
}

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != NC;
}

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 */
inline PinName port_pin(PortName port, int pin_n)
{
    return (PinName)((port << 8) + pin_n);
}


#ifdef __cplusplus
}
#endif

#endif // MBED_GPIO_OBJECTS_H
