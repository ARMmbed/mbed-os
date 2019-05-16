/*
* Copyright (c) 2015-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "mbed_assert.h"
#define THE_ANSWER 42

// Tests for static asserts in different contexts
// multiple asserts are used to guarantee no conflicts occur in generated labels

// Test for static asserts in global context
MBED_STATIC_ASSERT(sizeof(int) >= sizeof(char),
                   "An int must be larger than char");
MBED_STATIC_ASSERT(2 + 2 == 4,
                   "Hopefully the universe is mathematically consistent");
MBED_STATIC_ASSERT(THE_ANSWER == 42,
                   "Said Deep Thought, with infinite majesty and calm");

struct test {
    int dummy;

    // Test for static asserts in struct context
    MBED_STRUCT_STATIC_ASSERT(sizeof(int) >= sizeof(char),
                              "An int must be larger than char");
    MBED_STRUCT_STATIC_ASSERT(2 + 2 == 4,
                              "Hopefully the universe is mathematically consistent");
    MBED_STRUCT_STATIC_ASSERT(THE_ANSWER == 42,
                              "Said Deep Thought, with infinite majesty and calm");
};

MBED_STATIC_ASSERT(sizeof(struct test) == sizeof(int),
                   "Static assertions should not change the size of a struct");

void doit_c(void)
{
    // Test for static asserts in function context
    MBED_STATIC_ASSERT(sizeof(int) >= sizeof(char),
                       "An int must be larger than char");
    MBED_STATIC_ASSERT(2 + 2 == 4,
                       "Hopefully the universe is mathematically consistent");
    MBED_STATIC_ASSERT(THE_ANSWER == 42,
                       "Said Deep Thought, with infinite majesty and calm");
}

