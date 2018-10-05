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
#include "mbed_assert.h"
#include "pinmap.h"
#include "tmpm46b_gpio.h"

#define PIN_FUNC_MAX        6

void pin_function(PinName pin, int function)
{
    uint32_t port = 0;
    uint8_t bit = 0;
    uint8_t i = 0;
    uint8_t func = 0;
    uint8_t dir = 0;

    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Calculate pin function and pin direction
    func = PIN_FUNC(function);
    dir  = PIN_DIR(function);
    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit  = PIN_POS(pin);
    // Set function if function is in range
    if (func <= PIN_FUNC_MAX) {
        // Disable other functions
        for (i = 0; i < PIN_FUNC_MAX; i++) {
            if (i != (func - 1)) {
                GPIO_DisableFuncReg((GPIO_Port)port, i, (1 << bit));
            }
        }
        // Set pin function
        if (func) {
            GPIO_EnableFuncReg((GPIO_Port)port, (uint8_t)(func - 1), (1 << bit));
        }
    }

    // Set direction if direction is in range
    switch (dir) {
        case PIN_INPUT:
            GPIO_SetInputEnableReg((GPIO_Port)port, (1 << bit), ENABLE);
            GPIO_SetOutputEnableReg((GPIO_Port)port, (1 << bit), DISABLE);
            break;
        case PIN_OUTPUT:
            GPIO_SetOutputEnableReg((GPIO_Port)port, (1 << bit), ENABLE);
            GPIO_SetInputEnableReg((GPIO_Port)port, (1 << bit), DISABLE);
            break;
        case PIN_INOUT:
            GPIO_SetOutputEnableReg((GPIO_Port)port, (1 << bit), ENABLE);
            GPIO_SetInputEnableReg((GPIO_Port)port, (1 << bit), ENABLE);
            break;
        default:
            break;
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    uint32_t port = 0;
    uint8_t bit = 0;

    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Check if function is in range
    if (mode > OpenDrain) {
        return;
    }
    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit = PIN_POS(pin);
    // Set pin mode
    switch (mode) {
        case PullNone:
            GPIO_SetPullUp((GPIO_Port)port, (1 << bit), DISABLE);
            GPIO_SetPullDown((GPIO_Port)port, (1 << bit), DISABLE);
            GPIO_SetOpenDrain((GPIO_Port)port, (1 << bit), DISABLE);
            break;
        case PullUp:
            GPIO_SetPullUp((GPIO_Port)port, (1 << bit), ENABLE);
            break;
        case PullDown:
            GPIO_SetPullDown((GPIO_Port)port, (1 << bit), ENABLE);
            break;
        case OpenDrain:
            GPIO_SetOpenDrain((GPIO_Port)port, (1 << bit), ENABLE);
            break;
        default:
            break;
    }
}
