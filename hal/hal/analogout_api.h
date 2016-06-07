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
#ifndef MBED_ANALOGOUT_API_H
#define MBED_ANALOGOUT_API_H

#include "device.h"

#if DEVICE_ANALOGOUT

#ifdef __cplusplus
extern "C" {
#endif

/** Analogout hal structure. dac_s is declared in the target's hal
 */
typedef struct dac_s dac_t;

/**
 * \defgroup hal_analogout Analogout hal functions
 * @{
 */

/** Initialize the analogout peripheral
 *
 * Configures the pin used by analogout.
 * @param obj The analogout object to initialize
 * @param pin The analogout pin name
 */
void analogout_init(dac_t *obj, PinName pin);

/** Release the analogout object
 *
 * Note: This is not currently used in the mbed-drivers
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

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
