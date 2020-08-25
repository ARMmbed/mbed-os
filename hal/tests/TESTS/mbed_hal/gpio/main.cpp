/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

#include "PinNames.h"
#include "gpio_api.h"

static void gpio_nc_test()
{
    gpio_t nc_obj;
    gpio_init(&nc_obj, NC);
    TEST_ASSERT_FALSE(gpio_is_connected(&nc_obj));

    gpio_t led_obj;
    gpio_init(&led_obj, LED1);
    if (LED1 == NC) {
        TEST_ASSERT_FALSE(gpio_is_connected(&led_obj));
    } else {
        TEST_ASSERT_TRUE(gpio_is_connected(&led_obj));
    }
}

Case cases[] = {
    Case("gpio NC test", gpio_nc_test)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
