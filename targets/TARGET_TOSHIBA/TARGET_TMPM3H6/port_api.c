/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "pinmap.h"
#include "gpio_include.h"

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint8_t i = 0;

    // Assert that port is valid
    MBED_ASSERT(port <= PortR);

    // Store port and port mask for future use
    obj->port = port;
    obj->mask = mask;

    // Enabling Port Clock Supply
    TSB_CG->FSYSENA |= (1 << (obj->port));

    // Set port function and port direction
    for (i = 0; i < PORT_PIN_NUM; i++) {
        if (obj->mask & (1 << i)) {  // If the pin is used
            pin_function(port_pin(obj->port, i), dir);
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    uint8_t i = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortR);

    // Set mode for masked pins
    for (i = 0; i < PORT_PIN_NUM; i++) {
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint8_t bit = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortR);

    // Set direction for masked pins
    switch (dir) {
        case PIN_INPUT:
            for (bit = 0; bit < PORT_PIN_NUM; bit++) {
                if (((obj->mask >> bit) & 0x01) == 0x01) {
                    pin_function((PinName)bit, PIN_INPUT);
                }
            }
            break;
        case PIN_OUTPUT:
            for (bit = 0; bit < PORT_PIN_NUM; bit++) {
                if (((obj->mask >> bit) & 0x01) == 0x01) {
                    pin_function((PinName)bit, PIN_OUTPUT);
                }
            }
            break;
        case PIN_INOUT:
            for (bit = 0; bit < PORT_PIN_NUM; bit++) {
                if (((obj->mask >> bit) & 0x01) == 0x01) {
                    pin_function((PinName)bit, PIN_INOUT);
                }
            }
            break;
        default:
            break;
    }
}

void port_write(port_t *obj, int value)
{
    uint8_t  port_data = 0;
    uint8_t  data      = 0;
    int      bit       = 0;
    uint8_t  val       = 0;
    uint32_t base      = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortR);

    base = BITBAND_PORT_BASE(obj->port);
    base = BITBAND_PORT_MODE_BASE(base, GPIO_Mode_DATA);

    // Get current data of port
    for (bit = 7; bit >= 0; bit--) {
        BITBAND_PORT_READ(val, base, bit);
        port_data <<= 1;
        port_data |= val;
    }

    // Calculate data to write to masked pins
    data = (port_data & ~obj->mask) | (value & obj->mask);

    for (bit = 0; bit < PORT_PIN_NUM; bit++) {
        if (((obj->mask >> bit) & 0x01) == 0x01) {
            if (((data >> bit) & 0x01) == GPIO_PIN_SET) {
                BITBAND_PORT_SET(base, bit);
            } else {
                BITBAND_PORT_CLR(base, bit);
            }
        }
    }
}

int port_read(port_t *obj)
{
    uint8_t  port_data = 0;
    uint8_t  data      = 0;
    int      bit       = 0;
    uint8_t  val       = 0;
    uint32_t base      = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortR);

    base = BITBAND_PORT_BASE(obj->port);
    base = BITBAND_PORT_MODE_BASE(base, GPIO_Mode_DATA);

    // Get current data of port
    for (bit = 7; bit >= 0; bit--) {
        BITBAND_PORT_READ(val, base, bit);
        port_data <<= 1;
        port_data |= val;
    }

    // Calculate data of masked pins
    data = port_data & obj->mask;

    return data;
}

PinName port_pin(PortName port, int pin_n)
{
    PinName pin = NC;
    pin = (PinName)((port << 3 ) | pin_n);
    return pin;
}
