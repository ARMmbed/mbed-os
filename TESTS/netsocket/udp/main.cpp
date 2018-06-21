/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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
#include MBED_CONF_APP_HEADER_FILE
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "udp_tests.h"

using namespace utest::v1;

namespace
{
    NetworkInterface* net;
}

NetworkInterface* get_interface()
{
    return net;
}

static void _ifup() {
    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    nsapi_error_t err = MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: UDPClient IP address is '%s'\n", net->get_ip_address());
}

static void _ifdown() {
    net->disconnect();
    printf("MBED: ifdown\n");
}

void drop_bad_packets(UDPSocket& sock, int orig_timeout) {
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recvfrom(NULL, 0, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(orig_timeout);
}

void fill_tx_buffer_ascii(char *buff, size_t len)
{
    for (size_t i = 0; i<len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(480, "default_auto");
    _ifup();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
        Case("UDPSOCKET_ECHOTEST_NONBLOCK", UDPSOCKET_ECHOTEST_NONBLOCK),
        Case("UDPSOCKET_OPEN_CLOSE_REPEAT", UDPSOCKET_OPEN_CLOSE_REPEAT),
        Case("UDPSOCKET_OPEN_LIMIT", UDPSOCKET_OPEN_LIMIT),
        Case("UDPSOCKET_SENDTO_TIMEOUT", UDPSOCKET_SENDTO_TIMEOUT),
#ifdef MBED_EXTENDED_TESTS
        Case("UDPSOCKET_SENDTO_INVALID", UDPSOCKET_SENDTO_INVALID),
        Case("UDPSOCKET_ECHOTEST", UDPSOCKET_ECHOTEST),
        Case("UDPSOCKET_ECHOTEST_BURST", UDPSOCKET_ECHOTEST_BURST),
        Case("UDPSOCKET_ECHOTEST_BURST_NONBLOCK", UDPSOCKET_ECHOTEST_BURST_NONBLOCK),
        Case("UDPSOCKET_SENDTO_REPEAT", UDPSOCKET_SENDTO_REPEAT),
#endif
};

Specification specification(greentea_setup, cases, greentea_teardown);

int main()
{
    return !Harness::run(specification);
}
