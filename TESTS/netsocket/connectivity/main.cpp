/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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

 #ifndef MBED_CONF_APP_CONNECT_STATEMENT
     #error [NOT_SUPPORTED] No network configuration found for this target.
 #endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include MBED_CONF_APP_HEADER_FILE

using namespace utest::v1;

// Bringing the network up and down
template <int COUNT>
void test_bring_up_down() {
    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;

    for (int i = 0; i < COUNT; i++) {
        int err = MBED_CONF_APP_CONNECT_STATEMENT;
        TEST_ASSERT_EQUAL(0, err);

        printf("MBED: IP Address %s\r\n", net->get_ip_address());
        TEST_ASSERT(net->get_ip_address());

        UDPSocket udp;
        err = udp.open(net);
        TEST_ASSERT_EQUAL(0, err);
        err = udp.close();
        TEST_ASSERT_EQUAL(0, err);

        TCPSocket tcp;
        err = tcp.open(net);
        TEST_ASSERT_EQUAL(0, err);
        err = tcp.close();
        TEST_ASSERT_EQUAL(0, err);

        err = net->disconnect();
        TEST_ASSERT_EQUAL(0, err);
    }
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Bringing the network up and down", test_bring_up_down<1>),
    Case("Bringing the network up and down twice", test_bring_up_down<2>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
