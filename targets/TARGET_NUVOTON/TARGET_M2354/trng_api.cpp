/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
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

#if DEVICE_TRNG

#include "cmsis.h"
#include <limits.h>
#include "crypto-misc.h"
#include "hal/trng_api.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "nu_modutil.h"

/* TRNG init counter. TRNG is kept active as it is non-zero. */
static uint16_t trng_init_counter = 0U;

void trng_init(trng_t *obj)
{
    (void) obj;

    core_util_critical_section_enter();
    if (trng_init_counter == USHRT_MAX) {
        core_util_critical_section_exit();
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_OVERFLOW), \
                   "TRNG initialization counter would overflow");
    }
    ++ trng_init_counter;
    if (trng_init_counter == 1) {
        nu_trng_init_s();
    }
    core_util_critical_section_exit();
}

void trng_free(trng_t *obj)
{
    (void) obj;

    core_util_critical_section_enter();
    if (trng_init_counter == 0) {
        core_util_critical_section_exit();
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_UNDERFLOW), \
                   "TRNG initialization counter would underflow");
    }
    -- trng_init_counter;
    if (trng_init_counter == 0) {
        nu_trng_free_s();
    }
    core_util_critical_section_exit();
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void) obj;

    uint32_t output_length_;
    int32_t rc = nu_trng_get_bytes_s(output, (uint32_t) length, &output_length_);
    if (output_length) {
        *output_length = output_length_;
    }
    return rc;
}

#endif  /* #if DEVICE_TRNG */
