/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip_random.h"

#if FEATURE_COMMON_PAL

#include "randLIB.h"

void lwip_seed_random(void)
{
    randLIB_seed_random();
}

void lwip_add_random_seed(uint64_t seed)
{
    randLIB_add_seed(seed);
}

inline uint32_t lwip_get_random(void)
{
    return randLIB_get_32bit();
}

#else

void lwip_seed_random(void)
{
}

void lwip_add_random_seed(uint64_t seed)
{
}

uint32_t lwip_get_random(void)
{
    return rand();
}

#endif
