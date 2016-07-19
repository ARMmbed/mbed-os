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
#include "unity/unity.h"
#include "utest/utest.h"

using namespace utest::v1;

#define CUSTOM_TIME  1256729737

void test_case_rtc_strftime() {
    greentea_send_kv("timestamp", CUSTOM_TIME);

    char buffer[32] = {0};
    char kv_buff[64] = {0};
    set_time(CUSTOM_TIME);  // Set RTC time to Wed, 28 Oct 2009 11:35:37

    for (int i=0; i<10; ++i) {
        time_t seconds = time(NULL);
        sprintf(kv_buff, "[%ld] ", seconds);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %p", localtime(&seconds));
        strcat(kv_buff, buffer);
        greentea_send_kv("rtc", kv_buff);
        wait(1);
    }
}

Case cases[] = {
    Case("RTC strftime", test_case_rtc_strftime),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "rtc_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
