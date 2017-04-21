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

Timeout timeout;
DigitalOut led(LED1);
volatile int ticker_count = 0;
volatile bool print_tick = false;
static const int total_ticks = 10;
const int ONE_SECOND_US = 1000000;

void send_kv_tick() {
    if (ticker_count <= total_ticks) {
        timeout.attach_us(send_kv_tick, ONE_SECOND_US);
        print_tick = true;
    }
}

void wait_and_print() {
    while(ticker_count <= total_ticks) {
        if (print_tick) {
            print_tick = false;
            greentea_send_kv("tick", ticker_count++);
            led = !led;
        }
    }
}

void test_case_ticker() {
    timeout.attach_us(send_kv_tick, ONE_SECOND_US);
    wait_and_print();
}

// Test cases
Case cases[] = {
    Case("Timers: toggle on/off", test_case_ticker),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(total_ticks + 5, "timing_drift_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}

