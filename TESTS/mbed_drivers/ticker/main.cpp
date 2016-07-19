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

static const int ONE_SECOND_MS = 1000;

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Ticker *ticker1;
Ticker *ticker2;

void send_kv_tick() {
    static int count = 0;
    if (count < 10) {
        greentea_send_kv("tick", count);
    } else if (count == 10) {
        count = 0;
        Harness::validate_callback();
    }
    count++;
}

void ticker_callback_0(void) {
    static int ticker_count = 0;
    if (ticker_count >= ONE_SECOND_MS) {
        send_kv_tick();
        ticker_count = 0;
        led1 = !led1;
    }
    ticker_count++;
}

void ticker_callback_1(void) {
    led1 = !led1;
    send_kv_tick();
}

void ticker_callback_2_led(void) {
    led2 = !led2;
}

void ticker_callback_2(void) {
    ticker_callback_2_led();
    send_kv_tick();
}

void ticker_callback_1_switch_to_2(void);
void ticker_callback_2_switch_to_1(void);

void ticker_callback_1_switch_to_2(void) {
    ticker1->detach();
    ticker1->attach(ticker_callback_2_switch_to_1, 1.0);
    ticker_callback_1();
}

void ticker_callback_2_switch_to_1(void) {
    ticker2->detach();
    ticker2->attach(ticker_callback_1_switch_to_2, 1.0);
    ticker_callback_2();
}

utest::v1::control_t test_case_1x_ticker() {
    led1 = 0;
    led2 = 0;
    ticker1->attach_us(ticker_callback_0, ONE_SECOND_MS);
    return CaseTimeout(15 * ONE_SECOND_MS);
}

control_t test_case_2x_ticker() {
    led1 = 0;
    led2 = 0;
    ticker1->attach(&ticker_callback_1, 1.0);
    ticker2->attach(&ticker_callback_2_led, 2.0);
    return CaseTimeout(15 * ONE_SECOND_MS);
}

utest::v1::control_t test_case_2x_callbacks() {
    led1 = 0;
    led2 = 0;
    ticker1->attach(ticker_callback_1_switch_to_2, 1.0);
    return CaseTimeout(15 * ONE_SECOND_MS);
}

utest::v1::status_t one_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case) {
  ticker1 = new Ticker();
  return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t two_ticker_case_setup_handler_t(const Case *const source, const size_t index_of_case) {
  ticker1 = new Ticker();
  ticker2 = new Ticker();
  return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t one_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason) {
  delete ticker1;
  return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t two_ticker_case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason) {
  delete ticker1;
  delete ticker2;
  return greentea_case_teardown_handler(source, passed, failed, reason);
}

// Test cases
Case cases[] = {
    Case("Timers: 1x ticker", one_ticker_case_setup_handler_t, test_case_1x_ticker, one_ticker_case_teardown_handler_t),
    Case("Timers: 2x tickers", two_ticker_case_setup_handler_t, test_case_2x_ticker, two_ticker_case_teardown_handler_t),
    Case("Timers: 2x callbacks", two_ticker_case_setup_handler_t, test_case_2x_callbacks,two_ticker_case_teardown_handler_t),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(60, "wait_us_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
