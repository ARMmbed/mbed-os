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
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "randLIB.h"
#include "platform/arm_hal_random.h"

#if ((RAND_MAX+1) & RAND_MAX) != 0
#error "RAND_MAX isn't 2^n-1 :("
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
void randLIB_seed_random(void)
{
    uint32_t rand_seed;
    arm_random_module_init();
    rand_seed = arm_random_seed_get();
    srand(rand_seed);
}

/**
  * \brief Generate 8-bit random number.
  *
  * \param None
  * \return 8-bit random number
  *
  */
uint8_t randLIB_get_8bit(void)
{
    return rand();
}

/**
  * \brief Generate 16-bit random number.
  *
  * \param None
  * \return 16-bit random number
  *
  */
uint16_t randLIB_get_16bit(void)
{
    uint16_t ret_val;

    ret_val = rand();
#if RAND_MAX == 0x7FFF
    ret_val |= (uint16_t) rand() << 15;
#endif

    return ret_val;
}
/**
  * \brief Generate 32-bit random number.
  *
  * \param None
  * \return 32-bit random number
  *
  */
uint32_t randLIB_get_32bit(void)
{
    uint32_t ret_val;

    ret_val = rand();
#if RAND_MAX == 0x7FFF
    ret_val |= (uint32_t) rand() << 15;
    ret_val |= (uint32_t) rand() << 30;
#elif RAND_MAX == 0x3FFFFFFF /* IAR */
    ret_val |= (uint32_t) rand() << 30;
#elif RAND_MAX == 0x7FFFFFFF
    ret_val |= (uint32_t) rand() << 31;
#else
#error "randLIB_get_32bit - odd RAND_MAX"
#endif

    return ret_val;
}


/**
  * \brief Generate n-bytes random numbers.
  *
  * \param data_ptr pointer where random will be stored
  * \param eight_bit_boundary how many bytes need random
  * \return 0 process valid
  * \return -1 Unsupported Parameters
  *
  */
int8_t randLIB_get_n_bytes_random(uint8_t *data_ptr, uint8_t eight_bit_boundary)
{
    if ((data_ptr == 0) || (eight_bit_boundary == 0)) {
        return -1;
    }

    while (eight_bit_boundary) {
        *data_ptr++ = randLIB_get_8bit();
        eight_bit_boundary--;
    }
    return 0;
}

/**
  * \brief Generate a random number within a range.
  *
  * The result is linearly distributed in the range [min..max], inclusive.
  *
  * \param min minimum value that can be generated
  * \param max maximum value that can be generated
  */
uint16_t randLIB_get_random_in_range(uint16_t min, uint16_t max)
{
    /* This special case is potentially common, particularly in this routine's
     * first user (Trickle), so worth catching immediately */
    if (min == max) {
        return min;
    }

    /* 16-bit arithmetic below fails in this extreme case; we can optimise it */
    if (max - min == 0xFFFF) {
        return randLIB_get_16bit();
    }

    /* We get RAND_MAX+1 values from rand() in the range [0..RAND_MAX], and
     * need to divvy them up into the number of values we need. And reroll any
     * odd values off the end as we insist every value having equal chance.
     *
     * Special handling for systems where RAND_MAX is 0x7FFF; we use our
     * randLIB_get_16bit() and have to be a bit more careful about
     * unsigned integer overflow. (On other systems rand() returns int,
     * so we can't overflow if we use unsigned int).
     *
     * Eg, range(1,3), RAND_MAX = 0x7FFFFFFF:
     * We have 3 bands of size 0x2AAAAAAA (0x80000000/3).
     *
     * We roll: 0x00000000..0x2AAAAAAA9 -> 1
     *          0x2AAAAAAA..0x555555553 -> 2
     *          0x55555554..0x7FFFFFFFD -> 3
     *          0x7FFFFFFE..0x7FFFFFFFF -> reroll
     *
     * (Bias problem clearly pretty insignificant there, but gets worse as
     * range increases).
     */
    unsigned int values_needed = max + 1 - min;
#if RAND_MAX > 0xFFFF
    unsigned int band_size = (RAND_MAX + 1u) / values_needed;
#elif UINT_MAX > 0xFFFF
    unsigned int band_size = 0x10000u / values_needed;
#else
    /* Avoid the need for long division, at the expense of fractionally
     * increasing reroll chance. */
    unsigned int band_size = 0xFFFFu / values_needed;
#endif
    unsigned int top_of_bands = band_size * values_needed;
    unsigned int result;
    do {
#if RAND_MAX > 0xFFFF
        result = rand();
#else
        result = randLIB_get_16bit();
#endif
    } while (result >= top_of_bands);

    return min + (uint16_t)(result / band_size);
}

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
uint32_t randLIB_randomise_base(uint32_t base, uint16_t min_factor, uint16_t max_factor)
{
    uint16_t random_factor = randLIB_get_random_in_range(min_factor, max_factor);

    /* 32x16-bit long multiplication, to get 48-bit result */
    uint32_t hi = (base >> 16) * random_factor;
    uint32_t lo = (base & 0xFFFF) * random_factor;
    /* Add halves, and take top 32 bits of 48-bit result */
    uint32_t res = hi + (lo >> 16);

    /* Randomisation factor is *2^15, so need to shift up 1 more bit, avoiding overflow */
    if (res & 0x80000000) {
        res = 0xFFFFFFFF;
    } else {
        res = (res << 1) | ((lo >> 15) & 1);
    }

    return res;
}
