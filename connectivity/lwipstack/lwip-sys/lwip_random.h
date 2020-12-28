/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef LWIP_HDR_RANDOM_H
#define LWIP_HDR_RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

/** Init seed for Pseudo Random
 *
 *  Function shall seed the pseudo-random generator. Function
 *  is called during lwip initialisation.
 */
void lwip_seed_random(void);

/** Update seed for pseudo-random generator
 *
 *  Adds seed information to existing generator, to perturb the
 *  sequence.
 *
 *  @param seed     64 bits of data to add to the seed.
 */
void lwip_add_random_seed(uint64_t seed);

/** Generates random number.
 *
 *  @return         32-bit random number
 */
uint32_t lwip_get_random(void);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_RANDOM_H */
