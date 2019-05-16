/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#if DEVICE_TRNG
#include "trng_api.h"

#if defined(TARGET_GR_LYCHEE)

extern void trng_init_esp32(void);
extern void trng_free_esp32(void);
extern int trng_get_bytes_esp32(uint8_t *output, size_t length, size_t *output_length);

void trng_init(trng_t *obj)
{
    trng_init_esp32();
}

void trng_free(trng_t *obj)
{
    trng_free_esp32();
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    return trng_get_bytes_esp32(output, length, output_length);
}
#else
#error "There is no initialization processing."
#endif
#endif
