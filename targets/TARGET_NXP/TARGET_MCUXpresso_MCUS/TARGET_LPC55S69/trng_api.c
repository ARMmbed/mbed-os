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
    /* Init RNGA */
    RNG_Init(RNG);
}

void trng_free(trng_t *obj)
{
    RNG_Deinit(RNG);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    status_t status;

    /* Get Random data*/
    status = RNG_GetRandomData(RNG, output, length);
    if (status == kStatus_Success) {
        *output_length = length;
        return 0;
    } else {
        return -1;
    }
}

#endif
