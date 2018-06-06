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

#define PAYLOAD_LENGTH 36

using namespace utest::v1;

// Fill a buffer with a slice of the ASCII alphabet.
void fill_buffer(char* buffer, unsigned int length, unsigned int index) {
    unsigned int start = length * index;
    for (int i = 0; i < length - 1; i++) {
        buffer[i] = 'a' + ((start + i) % 26);
    }
    buffer[length - 1] = '\0';
}

// Echo server (echo payload to host)
template<int N>
void test_case_echo_server_x() {
    char _key[11] = {};
    char _tx_value[PAYLOAD_LENGTH + 1] = {};
    char _rx_value[PAYLOAD_LENGTH + 1] = {};
    const int echo_count = N;
    const char _echo_count_key_const[] = "echo_count";
    const char _echo_key_const[] = "echo";
    int expected_key = 1;

    // Send up the echo count
    greentea_send_kv(_echo_count_key_const, echo_count);
    // Handshake with host
    do {
        greentea_parse_kv(_key, _rx_value, sizeof(_key), sizeof(_rx_value));
        // Ensure the key received is "echo_count" and not some old data
        expected_key = strcmp(_echo_count_key_const, _key);
    } while (expected_key);
    TEST_ASSERT_EQUAL_INT(echo_count, atoi(_rx_value));

    for (int i=0; i < echo_count; ++i) {
        fill_buffer(_tx_value, PAYLOAD_LENGTH, i);
        greentea_send_kv(_echo_key_const, _tx_value);
        do {
            greentea_parse_kv(_key, _rx_value, sizeof(_key), sizeof(_rx_value));
            // Ensure the key received is "echo" and not some old data
            expected_key = strcmp(_echo_key_const, _key);
        } while (expected_key);
        TEST_ASSERT(strncmp(_tx_value, _rx_value, PAYLOAD_LENGTH) == 0);
    }
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Echo server: x16", test_case_echo_server_x<16>, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(30, "device_echo");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
