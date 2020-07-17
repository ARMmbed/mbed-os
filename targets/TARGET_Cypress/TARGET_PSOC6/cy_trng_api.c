/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "trng_api.h"
#include "cyhal_trng.h"
#include "mbed_error.h"
#include <string.h>

#if DEVICE_TRNG

#ifdef __cplusplus
extern "C" {
#endif

void trng_init(trng_t *obj)
{
    if (CY_RSLT_SUCCESS != cyhal_trng_init(&(obj->trng))) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_trng_init");
    }
}

void trng_free(trng_t *obj)
{
    cyhal_trng_free(&(obj->trng));
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint32_t offset = 0;
    // If output is not word-aligned, write partial word
    uint32_t prealign = (uint32_t)((uintptr_t)output % sizeof(uint32_t));
    if (prealign != 0) {
        uint32_t value = cyhal_trng_generate(&(obj->trng));
        uint32_t count = sizeof(uint32_t) - prealign;
        memmove(&output[0], &value, count);
        offset += count;
    }
    // Write aligned full words
    for (; offset < length - (sizeof(uint32_t) - 1u); offset += sizeof(uint32_t)) {
        *(uint32_t *)(&output[offset]) = cyhal_trng_generate(&(obj->trng));
    }
    // Write partial trailing word if requested
    if (offset < length) {
        uint32_t value = cyhal_trng_generate(&(obj->trng));
        uint32_t count = length - offset;
        memmove(&output[offset], &value, count);
        offset += count;
    }
    *output_length = offset;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
