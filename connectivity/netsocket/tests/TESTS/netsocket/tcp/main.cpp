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

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] tcp test cases require a RTOS to run
#else

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "tcp_tests.h"
#include "ip6string.h"

#ifndef ECHO_SERVER_ADDR
#error [NOT_SUPPORTED] Requires parameters for echo server
#else

using namespace utest::v1;

namespace {
Timer tc_bucket; // Timer to limit a test cases run time
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
mbed_stats_socket_t tcp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
#endif

char tcp_global::rx_buffer[RX_BUFF_SIZE];
char tcp_global::tx_buffer[TX_BUFF_SIZE];

void drop_bad_packets(TCPSocket &sock, int orig_timeout)
{
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recv(NULL, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(orig_timeout);
}

nsapi_version_t get_ip_version()
{
    SocketAddress test;
    if (NetworkInterface::get_default_instance()->get_ip_address(&test) != NSAPI_ERROR_OK) {
        return NSAPI_UNSPEC;
    }
    return test.get_ip_version();
}

static void _ifup()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    SocketAddress address;
    net->get_ip_address(&address);

#define MESH 3
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH
    printf("Waiting for GLOBAL_UP\n");
    while (net->get_connection_status() != NSAPI_STATUS_GLOBAL_UP) {
        ThisThread::sleep_for(500);
    }
#endif
    printf("MBED: TCPClient IP address is '%s'\n", address ? address.get_ip_address() : "null");
}

static void _ifdown()
{
    NetworkInterface::get_default_instance()->disconnect();
    tr_info("MBED: ifdown");
}

nsapi_error_t tcpsocket_connect_to_srv(TCPSocket &sock, uint16_t port)
{
    SocketAddress tcp_addr;

    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(port);

    tr_info("MBED: Server '%s', port %d", tcp_addr.get_ip_address(), tcp_addr.get_port());

    nsapi_error_t err = sock.open(NetworkInterface::get_default_instance());
    if (err != NSAPI_ERROR_OK) {
        tr_error("Error from sock.open: %d", err);
        return err;
    }

    err = sock.connect(tcp_addr);
    if (err != NSAPI_ERROR_OK) {
        tr_error("Error from sock.connect: %d", err);
        return err;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t tcpsocket_connect_to_echo_srv(TCPSocket &sock)
{
    return tcpsocket_connect_to_srv(sock, ECHO_SERVER_PORT);
}

nsapi_error_t tcpsocket_connect_to_discard_srv(TCPSocket &sock)
{
    return tcpsocket_connect_to_srv(sock, ECHO_SERVER_DISCARD_PORT);
}

bool is_tcp_supported()
{
    static bool supported;
    static bool tested = false;
    if (!tested) {
        TCPSocket socket;
        supported = socket.open(NetworkInterface::get_default_instance()) == NSAPI_ERROR_OK;
    }
    return supported;
}

void fill_tx_buffer_ascii(char *buff, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

int split2half_rmng_tcp_test_time()
{
    return (tcp_global::TESTS_TIMEOUT - tc_bucket.read()) / 2;
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
int fetch_stats()
{
    return SocketStats::mbed_stats_socket_get_each(&tcp_stats[0], MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT);
}
#endif

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(tcp_global::TESTS_TIMEOUT, "default_auto");
    _ifup();
    tc_bucket.start();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    tc_bucket.stop();
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}

utest::v1::status_t greentea_case_setup_handler_tcp(const Case *const source, const size_t index_of_case)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    int count = fetch_stats();
    for (int j = 0; j < count; j++) {
        TEST_ASSERT_EQUAL(SOCK_CLOSED,  tcp_stats[j].state);
    }
#endif
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t greentea_case_teardown_handler_tcp(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    int count = fetch_stats();
    for (int j = 0; j < count; j++) {
        TEST_ASSERT_EQUAL(SOCK_CLOSED,  tcp_stats[j].state);
    }
#endif
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

static void test_failure_handler(const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN) {
        verbose_test_failure_handler(failure);
        GREENTEA_TESTSUITE_RESULT(false);
        while (1) ;
    }
}


Case cases[] = {
    Case("TCPSOCKET_OPEN_LIMIT", TCPSOCKET_OPEN_LIMIT),
    Case("TCPSOCKET_ECHOTEST", TCPSOCKET_ECHOTEST),
    Case("TCPSOCKET_ECHOTEST_NONBLOCK", TCPSOCKET_ECHOTEST_NONBLOCK),
    Case("TCPSOCKET_OPEN_CLOSE_REPEAT", TCPSOCKET_OPEN_CLOSE_REPEAT),
    Case("TCPSOCKET_THREAD_PER_SOCKET_SAFETY", TCPSOCKET_THREAD_PER_SOCKET_SAFETY),
    Case("TCPSOCKET_CONNECT_INVALID", TCPSOCKET_CONNECT_INVALID),
    Case("TCPSOCKET_ECHOTEST_BURST", TCPSOCKET_ECHOTEST_BURST),
    Case("TCPSOCKET_ECHOTEST_BURST_NONBLOCK", TCPSOCKET_ECHOTEST_BURST_NONBLOCK),
    Case("TCPSOCKET_OPEN_DESTRUCT", TCPSOCKET_OPEN_DESTRUCT),
    Case("TCPSOCKET_OPEN_TWICE", TCPSOCKET_OPEN_TWICE),
    Case("TCPSOCKET_BIND_PORT", TCPSOCKET_BIND_PORT),
    Case("TCPSOCKET_BIND_PORT_FAIL", TCPSOCKET_BIND_PORT_FAIL),
    Case("TCPSOCKET_BIND_ADDRESS_INVALID", TCPSOCKET_BIND_ADDRESS_INVALID),
    Case("TCPSOCKET_BIND_ADDRESS", TCPSOCKET_BIND_ADDRESS),
    Case("TCPSOCKET_BIND_WRONG_TYPE", TCPSOCKET_BIND_WRONG_TYPE),
    Case("TCPSOCKET_BIND_UNOPENED", TCPSOCKET_BIND_UNOPENED),
    Case("TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID", TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID),
    Case("TCPSOCKET_RECV_100K", TCPSOCKET_RECV_100K),
    Case("TCPSOCKET_RECV_100K_NONBLOCK", TCPSOCKET_RECV_100K_NONBLOCK),
    Case("TCPSOCKET_RECV_TIMEOUT", TCPSOCKET_RECV_TIMEOUT),
    Case("TCPSOCKET_SEND_REPEAT", TCPSOCKET_SEND_REPEAT),
    Case("TCPSOCKET_SEND_TIMEOUT", TCPSOCKET_SEND_TIMEOUT),
    Case("TCPSOCKET_ENDPOINT_CLOSE", TCPSOCKET_ENDPOINT_CLOSE),
};

handlers_t tcp_test_case_handlers = {
    default_greentea_test_setup_handler,
    greentea_test_teardown_handler,
    test_failure_handler,
    greentea_case_setup_handler_tcp,
    greentea_case_teardown_handler_tcp,
    greentea_case_failure_continue_handler
};

Specification specification(greentea_setup, cases, greentea_teardown, tcp_test_case_handlers);

int main()
{
    return !Harness::run(specification);
}

#endif // ECHO_SERVER_ADDR
#endif // !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#endif // !defined(MBED_CONF_RTOS_PRESENT)
