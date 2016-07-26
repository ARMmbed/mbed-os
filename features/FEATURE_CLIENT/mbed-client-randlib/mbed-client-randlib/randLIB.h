/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file randLIB.h
 * \brief Pseudo Random Library API:
 *
 *
 * \section net-boot Network Bootstrap Control API:
 *  - randLIB_seed_random(), Set seed for pseudo random
 *  - randLIB_get_8bit(), Generate 8-bit random number
 *  - randLIB_get_16bit(),Generate 16-bit random number
 *  - randLIB_get_32bit(),Generate 32-bit random number
 *  - randLIB_get_n_bytes_random(), Generate n-bytes random numbers
 *
 */

#ifndef RANDLIB_H_
#define RANDLIB_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This library is made for getting random numbers for Timing needs in protocols.
 *
 * **not safe to use for security or cryptographic operations.**
 *
 */


/**
  * \brief Init seed for Pseudo Random.
  *
  * \return None
  *
  */
extern void randLIB_seed_random(void);

/**
  * \brief Generate 8-bit random number.
  *
  * \param None
  * \return 8-bit random number
  *
  */
extern uint8_t randLIB_get_8bit(void);

/**
  * \brief Generate 16-bit random number.
  *
  * \param None
  * \return 16-bit random number
  *
  */
extern uint16_t randLIB_get_16bit(void);

/**
  * \brief Generate 32-bit random number.
  *
  * \param None
  * \return 16-bit random number
  *
  */
extern uint32_t randLIB_get_32bit(void);

/**
  * \brief Generate n-bytes random numbers.
  *
  * \param data_ptr pointer where random will be stored
  * \param eight_bit_boundary how many bytes need random
  * \return 0 process valid
  * \return -1 Unsupported Parameters
  *
  */
extern int8_t randLIB_get_n_bytes_random(uint8_t *data_ptr, uint8_t eight_bit_boundary);

/**
  * \brief Generate a random number within a range.
  *
  * The result is linearly distributed in the range [min..max], inclusive.
  *
  * \param min minimum value that can be generated
  * \param max maximum value that can be generated
  */
uint16_t randLIB_get_random_in_range(uint16_t min, uint16_t max);

/**
  * \brief Randomise a base 32-bit number by a jitter factor
  *
  * The result is linearly distributed in the jitter range, which is expressed
  * as fixed-point unsigned 1.15 values. For example, to produce a number in the
  * range [0.75 * base, 1.25 * base], set min_factor to 0x6000 and max_factor to
  * 0xA000.
  *
  * Result is clamped to 0xFFFFFFFF if it overflows.
  *
  * \param base The base 32-bit value
  * \param min_factor The minimum value for the random factor
  * \param max_factor The maximum value for the random factor
  */
uint32_t randLIB_randomise_base(uint32_t base, uint16_t min_factor, uint16_t max_factor);

#ifdef __cplusplus
}
#endif
#endif /* RANDLIB_H_ */
