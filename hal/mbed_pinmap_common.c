/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "hal/pinmap.h"
#include "platform/mbed_error.h"

void pinmap_pinout(PinName pin, const PinMap *map)
{
    if (pin == NC) {
        return;
    }

    while (map->pin != NC) {
        if (map->pin == pin) {
            pin_function(pin, map->function);

            pin_mode(pin, PullNone);
            return;
        }
        map++;
    }
    MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "could not pinout", pin);
}

uint32_t pinmap_merge(uint32_t a, uint32_t b)
{
    // both are the same (inc both NC)
    if (a == b) {
        return a;
    }

    // one (or both) is not connected
    if (a == (uint32_t)NC) {
        return b;
    }
    if (b == (uint32_t)NC) {
        return a;
    }

    // mis-match error case
    MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "pinmap mis-match", a);
    return (uint32_t)NC;
}

uint32_t pinmap_find_peripheral(PinName pin, const PinMap *map)
{
    while (map->pin != NC) {
        if (map->pin == pin) {
            return map->peripheral;
        }
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_peripheral(PinName pin, const PinMap *map)
{
    uint32_t peripheral = (uint32_t)NC;

    if (pin == (PinName)NC) {
        return (uint32_t)NC;
    }
    peripheral = pinmap_find_peripheral(pin, map);
    if ((uint32_t)NC == peripheral) { // no mapping available
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "pinmap not found for peripheral", peripheral);
    }
    return peripheral;
}

uint32_t pinmap_find_function(PinName pin, const PinMap *map)
{
    while (map->pin != NC) {
        if (map->pin == pin) {
            return map->function;
        }
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_function(PinName pin, const PinMap *map)
{
    uint32_t function = (uint32_t)NC;

    if (pin == (PinName)NC) {
        return (uint32_t)NC;
    }
    function = pinmap_find_function(pin, map);
    if ((uint32_t)NC == function) { // no mapping available
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "pinmap not found for function", function);
    }
    return function;
}

bool pinmap_find_peripheral_pins(const PinList *whitelist, const PinList *blacklist, int per, const PinMap *const *maps, PinName **pins, uint32_t count)
{
    /*
     * This function uses recursion to find a suitable set of pins which meet the requirements.
     * Recursion is at max the number of pinmaps passed in - the 'count' parameter. Because of this
     * there is no risk of a stack overflow due to unbounded recursion.
     *
     * Below is a psuedo code example of this function's operation when finding a set of 4 pins.
     * The recursion depth is indicated by the number in front.
     *
     *  1. Given 4 maps and a peripheral find 4 suitable pins
     *      2. Given 4 maps, a peripheral and 1 pin find 3 suitable pins
     *          3. Given 4 maps, a peripheral and 2 pins find 2 suitable pins
     *              4. Given 4 maps, a peripheral and 3 pins find 1 suitable pin
     *              4. Return success if all pins are found, return failure if there are no suitable pins, otherwise choose the next pin and retry
     *          3. Return success if all pins are found, return failure if there are no suitable pins, otherwise choose the next pin and retry
     *      2. Return success if all pins are found, return failure if there are no suitable pins, otherwise choose the next pin and retry
     *  1. Return success if all pins are found, return failure if there are no suitable pins, otherwise choose the next pin and retry
     *
     */

    for (uint32_t i = 0; i < count; i++) {
        const PinMap *map = maps[i];
        PinName *pin = pins[i];
        if (*pin == NC) {
            for (; map->pin != NC; map++) {
                if (map->peripheral != per) {
                    continue;
                }
                if (!pinmap_list_has_pin(whitelist, map->pin)) {
                    // Not part of this form factor
                    continue;
                }
                if (pinmap_list_has_pin(blacklist, map->pin)) {
                    // Restricted pin
                    continue;
                }
                bool already_in_use = false;
                for (uint32_t j = 0; j < count; j++) {
                    if (j == i) {
                        // Don't compare with self
                        continue;
                    }
                    if (map->pin == *pins[j]) {
                        already_in_use = true;
                        break;
                    }
                }
                if (already_in_use) {
                    continue;
                }
                *pin = map->pin;
                if (pinmap_find_peripheral_pins(whitelist, blacklist, per, maps, pins, count)) {
                    return true;
                }
            }
            *pin = NC;
            return false;
        }
    }
    return true;
}

bool pinmap_list_has_pin(const PinList *list, PinName pin)
{
    for (uint32_t i = 0; i < list->count; i++) {
        if (list->pins[i] == pin) {
            return true;
        }
    }
    return false;
}

bool pinmap_list_has_peripheral(const PeripheralList *list, int peripheral)
{
    for (uint32_t i = 0; i < list->count; i++) {
        if (list->peripheral[i] == peripheral) {
            return true;
        }
    }
    return false;
}
