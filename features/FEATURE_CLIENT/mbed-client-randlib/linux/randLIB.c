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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "randLIB.h"

/**
  * \brief Init seed for Pseudo Random.
  * On a Linux, this does nothing.
  *
  * \return None
  *
  */
void randLIB_seed_random(void)
{
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
    uint8_t ret_val;
    randLIB_get_n_bytes_random(&ret_val, 1);
    return ret_val;
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

    randLIB_get_n_bytes_random((uint8_t*)&ret_val, 2);
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
    randLIB_get_n_bytes_random((uint8_t*)&ret_val, 4);
    return ret_val;
}


/**
  * \brief Generate n-bytes random numbers.
  *
  * \param data_ptr pointer where random will be stored
  * \param eight_bit_boundary how many bytes need random
  * \return 0 process valid
  * \return -1 Unsupported Parameters or failed to get random data.
  *
  */
int8_t randLIB_get_n_bytes_random(uint8_t *data_ptr, uint8_t eight_bit_boundary)
{
    if ((data_ptr == 0) || (eight_bit_boundary == 0)) {
        return -1;
    }

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd != -1) {
        size_t len = read(fd, data_ptr, eight_bit_boundary);
        close(fd);
        if (len == eight_bit_boundary)
            return 0;
    }

    return -1;
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

    unsigned int values_needed = max + 1 - min;
    unsigned int band_size = 0x10000u / values_needed;
    unsigned int top_of_bands = band_size * values_needed;
    unsigned int result;
    do {
        result = randLIB_get_16bit();
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
