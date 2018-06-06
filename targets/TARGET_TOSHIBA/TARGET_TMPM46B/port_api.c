/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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

#define PORT_PIN_NUM        8

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
    MBED_ASSERT(port <= PortL);

    // Store port and port mask for future use
    obj->port = port;
    obj->mask = mask;

    CG_SetFcPeriphA((1 << obj->port), ENABLE);

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
    MBED_ASSERT(obj->port <= PortL);

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
    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortL);

    // Set direction for masked pins
    switch (dir) {
        case PIN_INPUT:
            GPIO_SetOutputEnableReg((GPIO_Port)obj->port, obj->mask, DISABLE);
            GPIO_SetInputEnableReg((GPIO_Port)obj->port, obj->mask, ENABLE);
            break;
        case PIN_OUTPUT:
            GPIO_SetInputEnableReg((GPIO_Port)obj->port, obj->mask, DISABLE);
            GPIO_SetOutputEnableReg((GPIO_Port)obj->port, obj->mask, ENABLE);
            break;
        case PIN_INOUT:
            GPIO_SetOutputEnableReg((GPIO_Port)obj->port, obj->mask, ENABLE);
            GPIO_SetInputEnableReg((GPIO_Port)obj->port, obj->mask, ENABLE);
            break;
        default:
            break;
    }
}

void port_write(port_t *obj, int value)
{
    uint8_t port_data = 0;
    uint8_t data = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortL);

    // Get current data of port
    port_data = GPIO_ReadData((GPIO_Port)obj->port);
    // Calculate data to write to masked pins
    data = (port_data & ~obj->mask) | (value & obj->mask);
    // Write data to masked pins of the port
    GPIO_WriteData((GPIO_Port)obj->port, data);
}

int port_read(port_t *obj)
{
    uint8_t port_data = 0;
    uint8_t data = 0;

    // Assert that port is valid
    MBED_ASSERT(obj->port <= PortL);

    // Get current data of port
    port_data = GPIO_ReadData((GPIO_Port)obj->port);
    // Calculate data of masked pins
    data = port_data & obj->mask;
    return data;
}
