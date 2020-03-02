/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#include "mbed_assert.h"
#include "hal/gpio_api.h"

void gpio_get_capabilities(gpio_t *obj, gpio_capabilities_t *cap)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    // fixed pull-ups (for I2C)
    if (obj->pin == PTE24 || obj->pin == PTE25) {
        cap->pull_none = 0;
        cap->pull_down = 0;
        cap->pull_up = 0;
    } else {
        cap->pull_none = 1;
        cap->pull_down = 1;
        cap->pull_up = 1;
    }
}
