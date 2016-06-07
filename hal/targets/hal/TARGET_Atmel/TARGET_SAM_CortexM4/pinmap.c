/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <stddef.h>

#include "cmsis.h"
#include "mbed_assert.h"
#include "compiler.h"

#include "pinmap.h"
#include "ioport.h"

/** Change the MUX padding of input pin
 *
 * Configure the pin for specific module
 * @param[in]  pin      Pin name whose MUX padding is to be changed
 * @param[in]  function The MUX mode to be selected
 * @return              void
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    ioport_set_pin_mode(pin, function);
}

/** Change the pin pull mode
 *
 * Configure the pin pull mode
 * @param[in]  pin      Pin name whose MUX padding is to be changed
 * @param[in]  mode     Pin pull mode to be set
 * @return              void
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    if (mode == PullUp) {
        ioport_set_pin_mode(pin, IOPORT_MODE_PULLUP);
    } else if (mode == PullDown) {
        ioport_set_pin_mode(pin, IOPORT_MODE_PULLDOWN);
    } else {
        ioport_set_pin_mode(pin, IOPORT_MODE_OPEN_DRAIN);
    }
}