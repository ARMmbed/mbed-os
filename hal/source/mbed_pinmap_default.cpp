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
#include "hal/serial_api.h"
#include "hal/PinNameAliases.h"

//*** Common form factors ***
#if defined (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO)

static const PinName ff_arduino_uno_pins[] = {
    ARDUINO_UNO_D0, ARDUINO_UNO_D1, ARDUINO_UNO_D2, ARDUINO_UNO_D3, ARDUINO_UNO_D4, ARDUINO_UNO_D5, ARDUINO_UNO_D6, ARDUINO_UNO_D7,
    ARDUINO_UNO_D8, ARDUINO_UNO_D9, ARDUINO_UNO_D10, ARDUINO_UNO_D11, ARDUINO_UNO_D12, ARDUINO_UNO_D13, ARDUINO_UNO_D14, ARDUINO_UNO_D15,
    ARDUINO_UNO_A0, ARDUINO_UNO_A1, ARDUINO_UNO_A2, ARDUINO_UNO_A3, ARDUINO_UNO_A4, ARDUINO_UNO_A5
};

static const char *ff_arduino_uno_names[] = {
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
    "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15",
    "A0", "A1", "A2", "A3", "A4", "A5"
};

static const PinList ff_arduino_uno_list = {
    sizeof(ff_arduino_uno_pins) / sizeof(ff_arduino_uno_pins[0]),
    ff_arduino_uno_pins
};

static_assert(sizeof(ff_arduino_uno_pins) / sizeof(ff_arduino_uno_pins[0]) == sizeof(ff_arduino_uno_names) / sizeof(ff_arduino_uno_names[0]),
              "Arrays must have the same length");

const PinList *pinmap_ff_arduino_uno_pins()
{
    return &ff_arduino_uno_list;
}

const char *pinmap_ff_arduino_uno_pin_to_string(PinName pin)
{
    if (pin == (PinName)NC) {
        return "NC";
    }
    for (size_t i = 0; i < ff_arduino_uno_list.count; i++) {
        if (ff_arduino_uno_list.pins[i] == pin) {
            return ff_arduino_uno_names[i];
        }
    }
    return "Unknown";
}

#endif // defined (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO)

//*** Default restricted pins ***
MBED_WEAK const PinList *pinmap_restricted_pins()
{
    static const PinName pins[] = {
        CONSOLE_TX, CONSOLE_RX
    };
    static const PinList pin_list = {
        sizeof(pins) / sizeof(pins[0]),
        pins
    };
    return &pin_list;
}

//*** Default restricted gpio pins ***
// GPIO pins are special case because there are no pin-maps for GPIO
MBED_WEAK const PinList *pinmap_gpio_restricted_pins()
{
    static const PinList pin_list = {
        0,
        0
    };
    return &pin_list;
}

//*** Default restricted peripherals ***
#if DEVICE_SERIAL
MBED_WEAK const PeripheralList *pinmap_uart_restricted_peripherals()
{
    static const int stdio_uart = pinmap_peripheral(CONSOLE_TX, serial_tx_pinmap());

    static const int peripherals[] = {
        stdio_uart
    };

    static const PeripheralList peripheral_list = {
        sizeof peripherals / sizeof peripherals[0],
        peripherals
    };
    return &peripheral_list;
}
#endif
