
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
#ifndef MBED_ANALOGOUT_API_H
#define MBED_ANALOGOUT_API_H

#include "device.h"
#include "pinmap.h"

#if DEVICE_ANALOGOUT

#ifdef __cplusplus
extern "C" {
#endif

/** Analogout hal structure. dac_s is declared in the target's hal
 */
typedef struct dac_s dac_t;

/**
 * \defgroup hal_analogout Analogout hal functions
 *
 * # Defined behaviour
 * * The function ::analogout_init initializes the dac_t control structure
 * * The function ::analogout_write sets the output voltage, specified as a percentage (float) in range [0.0 (GND), 1.0 (VCC)]
 * * The function ::analogout_write_u16 sets the output voltage, specified as unsigned 16-bit value [0 (GND), MAX_UINT16 (VCC)]
 * * The function ::analogout_read reads the current voltage value on the pin and returns a floating-point value representing the current voltage in range [0.0 (GND), 1.0 (VCC)]
 * * The function ::analogout_read_u16 reads the current voltage value on the pin and returns the output voltage, specified as unsigned 16-bit value [0 (GND), MAX_UINT16 (VCC)]
 * * The accuracy of the DAC is +/- 10%
 * * The DAC operations ::analogout_write, ::analogout_write_u16, ::analogout_read, ::analogout_read_u16 take less than 20us to complete
 * * The function ::analogout_free releases the analogout object
 *
 * # Undefined behaviour
 *
 * * ::analogout_init is called with invalid pin (which does not support analog output function)
 * * Calling other functions before ::analogout_init
 * @{
 */

/**
 * \defgroup hal_analogin_tests Analogout hal tests
 * The Analogout HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the Analogout hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-analogout
 *
 */

/** Initialize the analogout peripheral
 *
 * Configures the pin used by analogout.
 * @param obj The analogout object to initialize
 * @param pinmap pointer to structure which holds static pinmap
 */
void analogout_init_direct(dac_t *obj, const PinMap *pinmap);

/** Initialize the analogout peripheral
 *
 * Configures the pin used by analogout.
 * @param obj The analogout object to initialize
 * @param pin The analogout pin name
 */
void analogout_init(dac_t *obj, PinName pin);

/** Release the analogout object
 *
 * @param obj The analogout object
 */
void analogout_free(dac_t *obj);

/** Set the output voltage, specified as a percentage (float)
 *
 * @param obj The analogin object
 * @param value The floating-point output voltage to be set
 */
void analogout_write(dac_t *obj, float value);

/** Set the output voltage, specified as unsigned 16-bit
 *
 * @param obj The analogin object
 * @param value The unsigned 16-bit output voltage to be set
 */
void analogout_write_u16(dac_t *obj, uint16_t value);

/** Read the current voltage value on the pin
 *
 * @param obj The analogin object
 * @return A floating-point value representing the current voltage on the pin,
 *     measured as a percentage
 */
float analogout_read(dac_t *obj);

/** Read the current voltage value on the pin, as a normalized unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16-bit value representing the current voltage on the pin
 */
uint16_t analogout_read_u16(dac_t *obj);

/** Get the pins that support analogout
 *
 * Return a PinMap array of pins that support analogout. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *analogout_pinmap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
