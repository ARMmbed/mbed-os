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

#ifndef MBED_EXTENDED_TESTS
#error [NOT_SUPPORTED] Pressure tests are not supported by default
#endif

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "tcp_tests.h"

using namespace utest::v1;

#ifndef MBED_CFG_TCP_CLIENT_ECHO_TIMEOUT
#define MBED_CFG_TCP_CLIENT_ECHO_TIMEOUT 500 //[ms]
#endif

static NetworkInterface* net;

NetworkInterface* get_interface()
{
    return net;
}

void drop_bad_packets(TCPSocket& sock) {
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recv(NULL, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(MBED_CFG_TCP_CLIENT_ECHO_TIMEOUT);
}

static void _ifup() {
    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    nsapi_error_t err = MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());
}

static void _ifdown() {
    net->disconnect();
    printf("MBED: ifdown\n");
}

void tcpsocket_connect_to_echo_srv(TCPSocket& sock) {
    SocketAddress tcp_addr;

    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(tcp_addr));
}

void tcpsocket_connect_to_discard_srv(TCPSocket& sock) {
    SocketAddress tcp_addr;

    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(9);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(tcp_addr));
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
    GREENTEA_SETUP(240, "default_auto");
    _ifup();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}


Case cases[] = {
    Case("Invalid endpoint rejected", test_tcpsocket_connect_invalid),
    Case("Echo", test_tcpsocket_echotest),
    Case("Echo non-block", test_tcpsocket_echotest_nonblock),
    Case("Echo burst", test_tcpsocket_echotest_burst),
    Case("Echo burst non-block", test_tcpsocket_echotest_burst_nonblock),
    Case("Reuse a socket", test_tcpsocket_open_close_repeat),
    Case("Open at least 4 sockets", test_tcpsocket_open_limit),
    Case("Receive 100k from CHARGEN service", test_tcpsocket_recv_100k),
    Case("Receive 100k from CHARGEN service non-block", test_tcpsocket_recv_100k_nonblock),
    Case("Receive in given time", test_tcpsocket_recv_timeout),
    Case("Send repeatedly", test_tcpsocket_send_repeat),
    Case("Sending shall not take too long", test_tcpsocket_send_timeout),
    Case("Parallel socket thread safety", test_tcpsocket_thread_per_socket_safety),
    Case("Endpoint initiated close", test_tcpsocket_endpoint_close),
};

Specification specification(greentea_setup, cases, greentea_teardown);

int main()
{
    return !Harness::run(specification);
}
