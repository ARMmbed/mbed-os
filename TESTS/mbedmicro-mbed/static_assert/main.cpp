/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include <stdio.h>
#include <stdint.h>

#include "mbed_toolchain.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;

void no_test() {}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Compilation test", no_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
