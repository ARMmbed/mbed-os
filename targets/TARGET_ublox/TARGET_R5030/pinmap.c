/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* As well as claiming and setting pins, the functions here also need
 * to take into account the way the pins are powered.  On the Boudica
 * chip they are arranged in three banks, PIO 0:5, PIO 6:10 and
 * PIO 11:19.
 *
 * The arrangement for which PIO bank is powered is determined by the module
 * in which the HI2110 chip is mounted, hence the use of conditional
 * compilation below.
 */

#include "stdbool.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralNames.h"

/* Defined alternate functionality of pins (May be used later) */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    MBED_ASSERT(function != (PinFunction)PIN_FUNCTION_MAX_NUMBER);
}

/* Mode is selected in gpio_api, no need to redefine */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
}
