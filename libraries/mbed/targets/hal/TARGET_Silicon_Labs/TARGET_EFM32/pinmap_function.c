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

#include "pinmap_function.h"
#include "PinNames.h"
#include "error.h"

/**
 * Get the value of the function field for a pin in a pinmap
 * @param pin A pin
 * @param map A pinmap for a given peripheral
 * @return Content of function field in pinmap for the given pin
 */
uint32_t pinmap_get_function_field(PinName pin, const PinMap *map)
{
    while (map->pin != NC) {
        if (map->pin == pin) {
            return map->function;
        }
        map++;
    }
    return (uint32_t) NC;
}

/**
 * Get the location a given peripheral is routed to from pin
 * This is stored in the function field of the pinmap
 * @param pin The pin
 * @param map Pinmap for the given peripheral
 * @return uint32 describing location (0, 1, 2, ...)
 */
uint32_t pin_location(PinName pin, const PinMap *map)
{
    if (pin == (PinName) NC) {
        return (uint32_t) NC;
    }

    uint32_t location = pinmap_get_function_field(pin, map);
    if ((uint32_t) NC == location) { // no mapping available
        error("location not found for pin");
    }
    return location;
}
