/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "trng_api.h"

#if defined(DEVICE_TRNG)
#include "fsl_rng.h"

void trng_init(trng_t *obj)
{
}

void trng_free(trng_t *obj)
{
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint32_t skip;
    uint32_t data;
    size_t idx = 0;
    int i;

    /* Get Random data */
    while (idx < length) {

        data = RNG_GetRandomData();

        for (i = 0; ((i < 4) && (idx < length)); i++) {
            output[idx++] = (data >> (i * 8)) & 0xFF;
        }

        /* Skip next 32 random numbers for better entropy */
        for (skip = 0; skip < 32; skip++) {
            RNG_GetRandomData();
        }
    }

    *output_length = idx;

    /* Zeroize to avoid leakage of entropy on the stack. Also ensure this is not removed by compiler optimizations */
    *((volatile uint32_t*) &data) = 0;

    return (idx == length ? 0 : -1);
}
#endif
