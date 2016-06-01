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

void send_kv_tick() {
    static int count = 0;
    if (count < 10) {
        greentea_send_kv("tick", count);
    } else if (count == 10) {
        Harness::validate_callback();
    }
    count++;
}

Ticker flipper_1;
DigitalOut led1(LED1);

void flip_1() {
    static int led1_state = 0;
    if (led1_state) {
        led1 = 0; led1_state = 0;
    } else {
        led1 = 1; led1_state = 1;
    }
    send_kv_tick();
}

Ticker flipper_2;
DigitalOut led2(LED2);

void flip_2() {
    static int led2_state = 0;
    if (led2_state) {
        led2 = 0; led2_state = 0;
    } else {
        led2 = 1; led2_state = 1;
    }
}

control_t test_case_ticker() {
    led1 = 0;
    led2 = 0;
    flipper_1.attach(&flip_1, 1.0);
    flipper_2.attach(&flip_2, 2.0);
    return CaseTimeout(15 * 1000);
}

// Test cases
Case cases[] = {
    Case("Timers: 2 x tickers", test_case_ticker),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "wait_us_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
