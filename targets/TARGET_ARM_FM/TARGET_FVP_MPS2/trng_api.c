/*
 * Copyright (c) 2017-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "trng_api.h"
#include "device.h"
#include <stdlib.h>

/** NOTE: FastModel are software models not contain any TRNG peripheral
 *  So C library srand() and rand() are used to simulate TRNG device
 */

void trng_init(trng_t *obj)
{
    srand(0);
    (void)obj;
}


void trng_free(trng_t *obj)
{
    (void)obj;
}

/** Get random data from TRNG peripheral
 *
 * @param obj The TRNG object
 * @param output The pointer to an output array
 * @param length The size of output data, to avoid buffer overwrite
 * @param output_length The length of generated data
 * @return 0 success, -1 fail
 */
int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;

    for (int i = 0; i < length; ++i) {
        output[i] = rand() % 256;
    }
    *output_length = length;

    return 0;
}
