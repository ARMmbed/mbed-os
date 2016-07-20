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
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

using namespace utest::v1;

// Echo server (echo payload to host)
template<int N>
void test_case_echo_server_x() {
    char _key[10] = {};
    char _value[128] = {};
    const int echo_count = N;

    // Handshake with host
    greentea_send_kv("echo_count", echo_count);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_INT(echo_count, atoi(_value));

    for (int i=0; i < echo_count; ++i) {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        greentea_send_kv(_key, _value);
    }
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Echo server: x16", test_case_echo_server_x<16>, greentea_failure_handler),
    Case("Echo server: x32", test_case_echo_server_x<32>, greentea_failure_handler),
    Case("Echo server: x64", test_case_echo_server_x<64>, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(180, "echo");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
