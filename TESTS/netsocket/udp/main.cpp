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

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif
#ifndef MBED_CONF_APP_ECHO_SERVER_ADDR
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "udp_tests.h"

using namespace utest::v1;

namespace {
NetworkInterface *net;
}

#if defined(MBED_NW_STATS_ENABLED)
mbed_stats_socket_t udp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT] = {0};
#endif

NetworkInterface *get_interface()
{
    return net;
}

static void _ifup()
{
    net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: UDPClient IP address is '%s'\n", net->get_ip_address());
}

static void _ifdown()
{
    net->disconnect();
    printf("MBED: ifdown\n");
}

void drop_bad_packets(UDPSocket &sock, int orig_timeout)
{
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
    for (size_t i = 0; i < len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

int fetch_stats()
{
#if defined(MBED_NW_STATS_ENABLED)
    return SocketStats::mbed_stats_socket_get_each(&udp_stats[0], MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT);
#else
    return 0;
#endif
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

Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

int main()
{
    return !Harness::run(specification);
}
