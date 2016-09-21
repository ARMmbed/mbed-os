/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#ifndef MBED_RNG_API_H
#define MBED_RNG_API_H

#include <stddef.h>
#include "device.h"

#if DEVICE_RNG

/** RNG HAL structure. rng_s is declared in the target's HAL
 */
typedef struct rng_s rng_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_rng RNG hal functions
 * @{
 */

/** Initialize the RNG peripheral
 *
 * @param obj The RNG object
 */
void rng_init(rng_t *obj);

/** Deinitialize the RNG peripheral
 *
 * @param obj The RNG object
 */
void rng_free(rng_t *obj);

/** Get random data from RNG peripheral
 *
 * @param obj The RNG object
 * @param output The pointer to an output array
 * @param length The length of output data
 * @param output_length The length of generated data
 * @return 0 success, -1 fail
 */
int rng_get_bytes(rng_t *obj, uint8_t *output, size_t length, size_t *output_length);

/** Set an external entropy value
 *
 * @param obj The RNG object
 * @param seed_value Entropy value to be set
 */
void rng_set_seed(rng_t *obj, uint32_t seed_value);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
