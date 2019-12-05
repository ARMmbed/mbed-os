/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
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
#include "port_api.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "txz_gpio.h"
#include "pinmap.h"

#define PORT_PIN_NUM                    (8)
#define ALT_FUNC_GPIO                   (0)

extern _gpio_t gpio_port_add;
static void gpio_pin_dir(port_t *obj, PinDirection dir, uint32_t pin_num);

PinName port_pin(PortName port, int pin_n)
{
    PinName pin = NC;
    pin = (PinName)((port << 3) | pin_n);
    return pin;
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint8_t i = 0;

    // Assert that port is valid
    MBED_ASSERT(port <= PortY);

    // Store port and port mask for future use
    obj->port = port;
    obj->mask = mask;

    // Enable the clock for particular port
    _gpio_init(&gpio_port_add, obj->port);

    // Set port function and port direction
    for (i = 0; i < PORT_PIN_NUM; i++) {
        // If the pin is used
        if (obj->mask & (1 << i)) {
            pin_function(port_pin(obj->port, i), dir);
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    uint8_t i = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortY);

    // Set mode for masked pins
    for (i = 0; i < PORT_PIN_NUM; i++) {
        // If the pin is used
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint8_t i = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortY);

    for (i = 0; i < PORT_PIN_NUM; i++) {
        // Set direction for masked pins
        if (obj->mask & (1 << i)) {
            gpio_pin_dir(obj, dir, i);
        }
    }
}

void port_write(port_t *obj, int value)
{
    uint32_t port_data = 0;
    uint32_t data      = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortY);

    // Get current data of port
    gpio_read_mode(&gpio_port_add, obj->port, GPIO_Mode_DATA, &port_data);

    // Calculate data to write to masked pins
    data = (port_data & ~obj->mask) | (value & obj->mask);

    // Write data to masked pins of the port
    gpio_write_mode(&gpio_port_add, obj->port, GPIO_Mode_DATA, data);
}

int port_read(port_t *obj)
{
    uint32_t port_data = 0;
    uint32_t data      = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortY);

    // Get current data of port
    gpio_read_mode(&gpio_port_add, obj->port, GPIO_Mode_DATA, &port_data);

    // Calculate data of masked pins
    data = port_data & obj->mask;

    return data;
}

static void gpio_pin_dir(port_t *obj, PinDirection dir, uint32_t pin_num)
{
    switch (dir) {
        case PIN_INPUT:
            // Set pin input
            gpio_func(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)pin_num, (uint32_t)ALT_FUNC_GPIO, GPIO_PIN_INPUT);
            break;
        case PIN_OUTPUT:
            // Set pin output
            gpio_func(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)pin_num, (uint32_t)ALT_FUNC_GPIO, GPIO_PIN_OUTPUT);
            break;
        case PIN_INOUT:
            // Set pin both input and output
            gpio_func(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)pin_num, (uint32_t)ALT_FUNC_GPIO, GPIO_PIN_INOUT);
            break;
        default:
            // error("Invalid direction\n");
            break;
    }
}
