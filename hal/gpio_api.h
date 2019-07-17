
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
#ifndef MBED_GPIO_API_H
#define MBED_GPIO_API_H

#include <stdint.h>
#include "device.h"
#include "pinmap.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_gpio GPIO HAL functions
 *
 * # Defined behavior
 * * ::gpio_init and other init functions can be called with NC or a valid PinName for the target - Verified by ::gpio_nc_test
 * * ::gpio_is_connected can be used to test whether a gpio_t object was initialized with NC - Verified by ::gpio_nc_test
 *
 * # Undefined behavior
 * * Calling any ::gpio_mode, ::gpio_dir, ::gpio_write or ::gpio_read on a gpio_t object that was initialized
 *   with NC.
 * * Calling ::gpio_set with NC.
 *
 * @{
 */

/** Set the given pin as GPIO
 *
 * @param pin The pin to be set as GPIO
 * @return The GPIO port mask for this pin
 **/
uint32_t gpio_set(PinName pin);

/** Checks if gpio object is connected (pin was not initialized with NC)
 * @param obj The GPIO object
 * @return 0 if object was initialized with NC
 * @return non-zero if object was initialized with a valid PinName
 **/
int gpio_is_connected(const gpio_t *obj);

/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize (may be NC)
 */
void gpio_init(gpio_t *obj, PinName pin);

/** Set the input pin mode
 *
 * @param obj  The GPIO object (must be connected)
 * @param mode The pin mode to be set
 */
void gpio_mode(gpio_t *obj, PinMode mode);

/** Set the pin direction
 *
 * @param obj       The GPIO object (must be connected)
 * @param direction The pin direction to be set
 */
void gpio_dir(gpio_t *obj, PinDirection direction);

/** Set the output value
 *
 * @param obj   The GPIO object (must be connected)
 * @param value The value to be set
 */
void gpio_write(gpio_t *obj, int value);

/** Read the input value
 *
 * @param obj The GPIO object (must be connected)
 * @return An integer value 1 or 0
 */
int gpio_read(gpio_t *obj);

// the following functions are generic and implemented in the common gpio.c file
// TODO: fix, will be moved to the common gpio header file

/** Init the input pin and set mode to PullDefault
 *
 * @param gpio The GPIO object
 * @param pin  The pin name (may be NC)
 */
void gpio_init_in(gpio_t *gpio, PinName pin);

/** Init the input pin and set the mode
 *
 * @param gpio  The GPIO object
 * @param pin   The pin name (may be NC)
 * @param mode  The pin mode to be set
 */
void gpio_init_in_ex(gpio_t *gpio, PinName pin, PinMode mode);

/** Init the output pin as an output, with predefined output value 0
 *
 * @param gpio The GPIO object
 * @param pin  The pin name (may be NC)
 * @return     An integer value 1 or 0
 */
void gpio_init_out(gpio_t *gpio, PinName pin);

/** Init the pin as an output and set the output value
 *
 * @param gpio  The GPIO object
 * @param pin   The pin name (may be NC)
 * @param value The value to be set
 */
void gpio_init_out_ex(gpio_t *gpio, PinName pin, int value);

/** Init the pin to be in/out
 *
 * @param gpio      The GPIO object
 * @param pin       The pin name (may be NC)
 * @param direction The pin direction to be set
 * @param mode      The pin mode to be set
 * @param value     The value to be set for an output pin
 */
void gpio_init_inout(gpio_t *gpio, PinName pin, PinDirection direction, PinMode mode, int value);

/** Get the pins that support all GPIO tests
 *
 * Return a PinMap array of pins that support GPIO. The
 * array is terminated with {NC, NC, 0}.
 *
 * Targets should override the weak implementation of this
 * function to provide the actual pinmap for GPIO testing.
 *
 * @return PinMap array
 */
const PinMap *gpio_pinmap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
