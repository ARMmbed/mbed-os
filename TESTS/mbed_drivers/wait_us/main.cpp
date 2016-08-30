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

using namespace utest::v1;

DigitalOut led(LED1);
Timer timer;
volatile bool print_tick = false;
const int ONE_SECOND_US = 1000000;
const int total_ticks = 10;

void test_case_ticker() {
    int before_print_us;
    int after_print_us;
    int wait_time_us = ONE_SECOND_US;
    
    timer.start();
    for (int i = 0; i <= total_ticks; ++i) {
        wait_us(wait_time_us);
        before_print_us = timer.read();
        greentea_send_kv("tick", i);
        after_print_us = timer.read();
        
        // This won't be 100% exact, but it should be pretty close
        wait_time_us =  ONE_SECOND_US - (after_print_us - before_print_us);
    }
    timer.stop();
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
