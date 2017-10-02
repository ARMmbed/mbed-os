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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "us_ticker_api_tests.h"
#include "hal/us_ticker_api.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

/* Test that the ticker has the correct frequency and number of bits. */
void us_ticker_info_test()
{
    const ticker_info_t* p_ticker_info = us_ticker_get_info();

    TEST_ASSERT(p_ticker_info->frequency >= 250000);
    TEST_ASSERT(p_ticker_info->frequency <= 8000000);
    TEST_ASSERT(p_ticker_info->bits >= 16);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("us ticker info test", us_ticker_info_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
