/*
 * Copyright (c) 2015, 2018, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "arm_hal_random.h"

#include "mbedtls/entropy_poll.h"

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#endif
#endif

void arm_random_module_init(void)
{
}

uint32_t arm_random_seed_get(void)
{
    uint32_t result = 0;
#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_setup(NULL);
#endif /* MBEDTLS_PLATFORM_C */
    /* Grab a seed from a function we provide for mbedtls */
    size_t len;
    mbedtls_hardware_poll(NULL, (uint8_t *) &result, sizeof result, &len);
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif /* MBEDTLS_PLATFORM_C */
#endif
    return result;
}
