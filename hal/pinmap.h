
/** \addtogroup hal */
/** @{*/
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
#ifndef MBED_PINMAP_H
#define MBED_PINMAP_H

#include "PinNames.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    int peripheral;
    int function;
} PinMap;

typedef struct {
    uint32_t count;
    const PinName *pins;
} PinList;

void pin_function(PinName pin, int function);
void pin_mode(PinName pin, PinMode mode);

uint32_t pinmap_peripheral(PinName pin, const PinMap *map);
uint32_t pinmap_function(PinName pin, const PinMap *map);
uint32_t pinmap_merge(uint32_t a, uint32_t b);
void     pinmap_pinout(PinName pin, const PinMap *map);
uint32_t pinmap_find_peripheral(PinName pin, const PinMap *map);
uint32_t pinmap_find_function(PinName pin, const PinMap *map);

/**
 * Find a combination of pins suitable for use given the constraints
 *
 * This function finds pins which meet these specific properties:
 * - The pin is part of the form factor
 * - The pin is not in the restricted list
 * - The pin is contained within the respective pinmap
 * - The pin belongs to the given peripheral
 * - Each pin found is distinct; in the example below
 *      mosi and miso will never be assigned the same pin
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "pinmap.h"
 *
 * int main()
 * {
 *     int per = spi_master_cs_pinmap()->peripheral;
 *     const PinList *pins_ff = pinmap_ff_default_pins();
 *     const PinList *pins_avoid = pinmap_restricted_pins();
 *     PinName mosi = NC;
 *     PinName miso = NC;
 *     PinName sclk = NC;
 *     PinName ssel = NC;
 *     const PinMap *maps[] = {
 *         spi_master_mosi_pinmap(),
 *         spi_master_miso_pinmap(),
 *         spi_master_clk_pinmap(),
 *         spi_master_cs_pinmap()
 *     };
 *     PinName *pins[] = {
 *         &mosi,
 *         &miso,
 *         &sclk,
 *         &ssel
 *     };
 *     if (pinmap_find_peripheral_pins(pins_ff, pins_avoid, per, maps, pins, sizeof(maps) / sizeof(maps[0]))) {
 *         printf("Found SPI pins to test instance %i with:\n"
 *                "  mosi=%s\n"
 *                "  miso=%s\n"
 *                "  sclk=%s\n"
 *                "  ssel=%s\n", per,
 *                pinmap_ff_default_pin_to_string(mosi),
 *                pinmap_ff_default_pin_to_string(miso),
 *                pinmap_ff_default_pin_to_string(sclk),
 *                pinmap_ff_default_pin_to_string(ssel));
 *     } else {
 *         printf("Could not find SPI combination to test %i\n", per);
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * @param whitelist List of pins to choose from
 * @param blacklist List of pins which cannot be used
 * @param per Peripheral to which the pins belong
 * @param maps An array of pin maps to select from
 * @param pins An array of pins to find. Pins already set to a value will be
 *             left unchanged. Only pins initialized to NC will be updated by this function
 * @param count The size of maps and pins
 * @return true if a suitable combination of pins was found and
 *         written to the pins array, otherwise false
 */
bool pinmap_find_peripheral_pins(const PinList *whitelist, const PinList *blacklist, int per, const PinMap *const *maps, PinName **pins, uint32_t count);

/**
 * Check if the pin is in the list
 *
 * @param list pin list to check
 * @param pin pin to check for in the list
 * @return true if the pin is in the list, false otherwise
 */
bool pinmap_list_has_pin(const PinList *list, PinName pin);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
