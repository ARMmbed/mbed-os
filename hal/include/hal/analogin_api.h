
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
#ifndef MBED_ANALOGIN_API_H
#define MBED_ANALOGIN_API_H

#include "device.h"
#include "pinmap.h"

#if DEVICE_ANALOGIN

#ifdef __cplusplus
extern "C" {
#endif

/** Analogin hal structure. analogin_s is declared in the target's hal
 */
typedef struct analogin_s analogin_t;

/** Analogin configuration hal structure. analogin_config_s is declared in the target's hal
 */
typedef struct analogin_config_s analogin_config_t;

/**
 * \defgroup hal_analogin Analogin hal functions
 *
 * # Defined behaviour
 * * The function ::analogin_init initializes the analogin_t control structure
 * * The function ::analogin_free returns the pin owned by the Analogin object to its reset state
 * * The function ::analogin_read reads the input voltage, represented as a float in the range [0.0 (GND), 1.0 (VCC)]
 * * The function ::analogin_read_u16 reads the value from analogin pin, represented as an unsigned 16bit value [0.0 (GND), MAX_UINT16 (VCC)]
 * * The accuracy of the ADC is +/- 10%
 * * The ADC operations ::analogin_read, ::analogin_read_u16 take less than 20us to complete
 *
 * # Undefined behaviour
 *
 * * ::analogin_init is called with invalid pin (which does not support analog input function)
 * * Calling ::analogin_read, ::analogin_read_u16 before ::analogin_init
 * @{
 */

/**
 * \defgroup hal_analogin_tests Analogin hal tests
 * The Analogin HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the Analogin hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-analogin
 *
 */

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pinmap pointer to structure which holds static pinmap
 */
void analogin_init_direct(analogin_t *obj, const PinMap *pinmap);

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin);

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 * @param pinmap pointer to structure which holds analogin configuration
 */
void __attribute__((weak)) analogin_configure(analogin_t *obj, const analogin_config_t *config);

/** Release the analogin peripheral
 *
 * Releases the pin used by analogin.
 * @param obj The analogin object to initialize
 */
void analogin_free(analogin_t *obj);

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj);

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj);

/** Get the pins that support analogin
 *
 * Return a PinMap array of pins that support analogin. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *analogin_pinmap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
