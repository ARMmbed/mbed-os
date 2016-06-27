/***************************************************************************//**
 * @file pinmap_function.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

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
