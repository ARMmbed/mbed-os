/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "mbed_error.h"

#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"
#include "ti/devices/cc32xx/inc/hw_ocp_shared.h"
#include "ti/devices/cc32xx/driverlib/pin.h"

/**
 * Configure pin (mode, speed, output type and pull-up/pull-down)
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    switch (mode) {
        case PullNone:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_STD);
            break;
        case PullUp:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_STD_PU);
            break;
        case PullDown:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_STD_PD);
            break;
        case OpenDrain:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_OD);
            break;
        case OpenDrainPullUp:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_OD_PU);
            break;
        case OpenDrainPullDown:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_OD_PD);
            break;
        case Analog:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_ANALOG);
            break;
        default:
            PinConfigSet(pin, PIN_STRENGTH_2MA, PIN_TYPE_STD);
            break;
    }
}
