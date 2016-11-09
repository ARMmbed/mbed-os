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
#include <limits.h>
#include "randLIB.h"
#include "platform/arm_hal_random.h"

/**
 * This library is made for getting random numbers for timing needs in
 * protocols, plus to generate dynamic ports, random IDs etc.
 *
 * **not safe to use for security or cryptographic operations.**
 *
 * Base implementation is a pseudo-RNG, but may also use a system RNG.
 * Replay of sequence by reseeding is not possible.
 *
 * Base pseudo-RNG is the xoroshiro128+ generator by Marsaglia, Blackman and
 * Vigna:
 *
 * http://xoroshiro.di.unimi.it/
 *
 * Certainly not the fastest for 32-bit or smaller platforms, but speed
 * is not critical. None of the long operations in the core are actually hard,
 * unlike the divisions and multiplies in the utility functions below, where we
 * do try to keep the operations narrow.
 */

/* On some platforms, read from a system RNG, rather than use our own */
/* RANDLIB_PRNG disables this and forces use of the PRNG (useful for test only?) */
#ifndef RANDLIB_PRNG
#ifdef __linux
#define RANDOM_DEVICE "/dev/urandom"
#endif
#endif // RANDLIB_PRNG

/* RAM usage - 16 bytes of state (or a FILE * pointer and underlying FILE, which
 * will include a buffer) */
#ifdef RANDOM_DEVICE
#include <stdio.h>
static FILE *random_file;
#else
static uint64_t state[2];
#endif

#ifdef RANDLIB_PRNG
void randLIB_reset(void)
{
    state[0] = 0;
    state[1] = 0;
}
#endif

#ifndef RANDOM_DEVICE
static inline uint64_t rol(uint64_t n, int bits)
{
    return (n << bits) | (n >> (64 - bits));
}

/* Lower-quality generator used only for initial seeding, if platform
 * isn't returning multiple seeds itself. Multiplies are rather heavy
 * for lower-end platforms, but this is initialisation only.
 */
static uint64_t splitmix64(uint64_t *seed)
{
    uint64_t z = (*seed += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}
#endif // RANDOM_DEVICE

void randLIB_seed_random(void)
{
#ifdef RANDOM_DEVICE
    if (!random_file) {
        random_file = fopen(RANDOM_DEVICE, "rb");
    }
#else
    arm_random_module_init();

    /* We exclusive-OR with the current state, in case they make this call
     * multiple times,or in case someone has called randLIB_add_seed before
     * this. We don't want to potentially lose entropy.
     */

    /* Spell out expressions so we get known ordering of 4 seed calls */
    uint64_t s = (uint64_t) arm_random_seed_get() << 32;
    state[0] ^= ( s | arm_random_seed_get());

    s = (uint64_t) arm_random_seed_get() << 32;
    state[1] ^= s | arm_random_seed_get();

    /* This check serves to both to stir the state if the platform is returning
     * constant seeding values, and to avoid the illegal all-zero state.
     */
    if (state[0] == state[1]) {
        randLIB_add_seed(state[0]);
    }
#endif // RANDOM_DEVICE
}

void randLIB_add_seed(uint64_t seed)
{
#ifndef RANDOM_DEVICE
    state[0] ^= splitmix64(&seed);
    state[1] ^= splitmix64(&seed);
    /* This is absolutely necessary, but I challenge you to add it to line coverage */
    if (state[1] == 0 && state[0] == 0) {
        state[0] = 1;
    }
#endif
}

uint8_t randLIB_get_8bit(void)
{
    uint64_t r = randLIB_get_64bit();
    return (uint8_t) (r >> 56);
}

uint16_t randLIB_get_16bit(void)
{
    uint64_t r = randLIB_get_64bit();
    return (uint16_t) (r >> 48);
}

uint32_t randLIB_get_32bit(void)
{
    uint64_t r = randLIB_get_64bit();
    return (uint32_t) (r >> 32);
}


uint64_t randLIB_get_64bit(void)
{
#ifdef RANDOM_DEVICE
    if (!random_file) {
        return 0;
    }
    uint64_t result;
    if (fread(&result, sizeof result, 1, random_file) != 1) {
        result = 0;
    }
    return result;
#else
    const uint64_t s0 = state[0];
    uint64_t s1 = state[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    state[0] = rol(s0, 55) ^ s1 ^ (s1 << 14);
    state[1] = rol(s1, 36);

    return result;
#endif
}

void *randLIB_get_n_bytes_random(void *ptr, uint8_t count)
{
    uint8_t *data_ptr = ptr;
    uint64_t r = 0;
    for (uint_fast8_t i = 0; i < count; i++) {
        /* Take 8 bytes at a time */
        if (i % 8 == 0) {
            r = randLIB_get_64bit();
        } else {
            r >>= 8;
        }
        data_ptr[i] = (uint8_t) r;
    }
    return data_ptr;
}

uint16_t randLIB_get_random_in_range(uint16_t min, uint16_t max)
{
    /* This special case is potentially common, particularly in this routine's
     * first user (Trickle), so worth catching immediately */
    if (min == max) {
        return min;
    }

#if UINT_MAX >= 0xFFFFFFFF
    const unsigned int rand_max = 0xFFFFFFFFu; // will use rand32
#else
    const unsigned int rand_max = 0xFFFFu; // will use rand16

    /* 16-bit arithmetic below fails in this extreme case; we can optimise it */
    if (max - min == 0xFFFF) {
        return randLIB_get_16bit();
    }
#endif

    /* We get rand_max values from rand16 or 32() in the range [0..rand_max-1], and
     * need to divvy them up into the number of values we need. And reroll any
     * odd values off the end as we insist every value having equal chance.
     *
     * Using the range [0..rand_max-1] saves long division on the band
     * calculation - it means rand_max ends up always being rerolled.
     *
     * Eg, range(1,2), rand_max = 0xFFFF:
     * We have 2 bands of size 0x7FFF (0xFFFF/2).
     *
     * We roll: 0x0000..0x7FFE -> 1
     *          0x7FFF..0xFFFD -> 2
     *          0xFFFE..0xFFFF -> reroll
     * (calculating band size as 0x10000/2 would have avoided the reroll cases)
     *
     * Eg, range(1,3), rand_max = 0xFFFFFFFF:
     * We have 3 bands of size 0x55555555 (0xFFFFFFFF/3).
     *
     * We roll: 0x00000000..0x555555554 -> 1
     *          0x55555555..0xAAAAAAAA9 -> 2
     *          0xAAAAAAAA..0xFFFFFFFFE -> 3
     *          0xFFFFFFFF              -> reroll
     *
     * (Bias problem clearly pretty insignificant there, but gets worse as
     * range increases).
     */
    const unsigned int values_needed = max + 1 - min;
    /* Avoid the need for long division, at the expense of fractionally
     * increasing reroll chance. */
    const unsigned int band_size = rand_max / values_needed;
    const unsigned int top_of_bands = band_size * values_needed;
    unsigned int result;
    do {
#if UINT_MAX > 0xFFFF
        result = randLIB_get_32bit();
#else
        result = randLIB_get_16bit();
#endif
    } while (result >= top_of_bands);

    return min + (uint16_t)(result / band_size);
}

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
