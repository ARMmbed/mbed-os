/*
 * Copyright (c) 2017-2018 ARM Limited
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
#include "tztrng.h"

#define BITS_PER_BYTE 8

void trng_init(trng_t *obj)
{
    /* Initialization is made in the driver */
    (void)obj;
}

void trng_free(trng_t *obj)
{
    /* The current implementation only contains a dummy version of TRNG */
    (void)obj;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint32_t retval;

    (void)obj;
    /* The TRNG driver expects a number of bits. */
    retval = CC_TrngGetSource(TRNG_BASE, output, output_length,
                              length * BITS_PER_BYTE);

    if (retval != 0) {
        return -1;
    }
    return 0;
}
