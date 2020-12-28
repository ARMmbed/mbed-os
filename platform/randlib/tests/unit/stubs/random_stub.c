/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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

static uint32_t seed_value = 4;
static bool seed_inc = false;

void arm_random_module_init(void)
{

}

uint32_t arm_random_seed_get(void)
{
    uint32_t result = seed_value;
    if (seed_inc) {
        ++seed_value;
    }
    return result;
}

void random_stub_set_seed(uint32_t value, bool increment)
{
    seed_value = value;
    seed_inc = increment;
}
