/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"

/**
  NOTE: This test will have a bit of inherent drift due to it being
  single-threaded, so having a drift that is non-zero should be ok. However,
  it should still be well under the limit.
**/


using namespace utest::v1;

DigitalOut led(LED1);
volatile bool print_tick = false;
const int ONE_SECOND_US = 1000000;
const int total_ticks = 10;

void test_case_ticker() {  
    for (int i = 0; i <= total_ticks; i++) {
        wait_us(ONE_SECOND_US);
        greentea_send_kv("tick", i);
    }
}

// Test cases
Case cases[] = {
    Case("Timers: wait_us", test_case_ticker),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(total_ticks + 5, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
