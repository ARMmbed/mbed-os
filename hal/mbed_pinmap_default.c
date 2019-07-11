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

#include <stdlib.h>
#include "hal/pinmap.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_assert.h"
#include "device.h"

//*** Common form factors ***
#ifdef TARGET_FF_ARDUINO

static const PinName ff_arduino_pins[] = {
    D0, D1, D2, D3, D4, D5, D6, D7,
    D8, D9, D10, D11, D12, D13, D14, D15,
    A0, A1, A2, A3, A4, A5
};

static const char *ff_arduino_names[] = {
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
    "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15",
    "A0", "A1", "A2", "A3", "A4", "A5"
};

static const PinList ff_arduino_list = {
    sizeof(ff_arduino_pins) / sizeof(ff_arduino_pins[0]),
    ff_arduino_pins
};

MBED_STATIC_ASSERT(sizeof(ff_arduino_pins) / sizeof(ff_arduino_pins[0]) == sizeof(ff_arduino_names) / sizeof(ff_arduino_names[0]),
                   "Arrays must have the same length");

const PinList *pinmap_ff_arduino_pins()
{
    return &ff_arduino_list;
}

const char *pinmap_ff_arduino_pin_to_string(PinName pin)
{
    if (pin == NC) {
        return "NC";
    }
    for (size_t i = 0; i < ff_arduino_list.count; i++) {
        if (ff_arduino_list.pins[i] == pin) {
            return ff_arduino_names[i];
        }
    }
    return "Unknown";
}

#endif

//*** Default restricted pins ***
MBED_WEAK const PinList *pinmap_restricted_pins()
{
    static const PinName pins[] = {
        USBTX, USBRX
    };
    static const PinList pin_list = {
        sizeof(pins) / sizeof(pins[0]),
        pins
    };
    return &pin_list;
}

//*** Default restricted peripherals ***
MBED_WEAK const PeripheralList *pinmap_restricted_peripherals()
{
    static const PeripheralList peripheral_list = {
        0,
        0
    };
    return &peripheral_list;
}
