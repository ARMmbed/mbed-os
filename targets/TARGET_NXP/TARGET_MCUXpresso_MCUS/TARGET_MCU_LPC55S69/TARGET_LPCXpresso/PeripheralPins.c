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

#include "PeripheralPins.h"
#include "PeripheralPinMaps.h"

// List of GPIOs with limited functionality
const PinList *pinmap_gpio_restricted_pins()
{
    static const PinName pins[] = {
        A4,   // fixed pull-up (for I2C)
        A5,   // fixed pull-up (for I2C)
        D5,   // fixed pull-up (for LED)
        D3,   // fixed pull-up (for LED)
        D4,   // fixed pull-up (for LED)
        D7,   // fixed pull-up
        D15,  // fixed pull-up (for I2C)
        D14   // fixed pull-up (for I2C)
    };

    static const PinList pin_list = {
        sizeof(pins) / sizeof(pins[0]),
        pins
    };
    return &pin_list;
}
