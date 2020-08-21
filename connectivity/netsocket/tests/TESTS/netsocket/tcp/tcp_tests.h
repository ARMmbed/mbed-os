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

#ifndef TCP_TESTS_H
#define TCP_TESTS_H

#include "../test_params.h"
#include "mbed_trace.h"

#define TRACE_GROUP "GRNT"

NetworkInterface *get_interface();
void drop_bad_packets(TCPSocket &sock, int orig_timeout);
nsapi_version_t get_ip_version();
void fill_tx_buffer_ascii(char *buff, size_t len);
nsapi_error_t tcpsocket_connect_to_echo_srv(TCPSocket &sock);
nsapi_error_t tcpsocket_connect_to_discard_srv(TCPSocket &sock);
bool is_tcp_supported();

#define SKIP_IF_TCP_UNSUPPORTED() \
    if (!is_tcp_supported()) { \
        TEST_SKIP_MESSAGE("TCP not supported"); \
    }

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
extern mbed_stats_socket_t tcp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
int fetch_stats(void);
#endif

/**
 * Single testcase might take only half of the remaining execution time
 */
int split2half_rmng_tcp_test_time(); // [s]

namespace tcp_global {
#ifdef MBED_GREENTEA_TEST_TCPSOCKET_TIMEOUT_S
static const int TESTS_TIMEOUT = MBED_GREENTEA_TEST_TCPSOCKET_TIMEOUT_S;
#else
#define MESH 3
#define WISUN 0x2345
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH && MBED_CONF_NSAPI_DEFAULT_MESH_TYPE == WISUN
static const int TESTS_TIMEOUT = (25 * 60);
#else
static const int TESTS_TIMEOUT = (10 * 60);
#endif
#endif

static const int TCP_OS_STACK_SIZE = 2048;

static const int RX_BUFF_SIZE = 1220;
static const int TX_BUFF_SIZE = 1220;

extern char rx_buffer[RX_BUFF_SIZE];
extern char tx_buffer[TX_BUFF_SIZE];
}

/*
 * Test cases
 */
void TCPSOCKET_CONNECT_INVALID();
void TCPSOCKET_ECHOTEST();
void TCPSOCKET_ECHOTEST_NONBLOCK();
void TCPSOCKET_ECHOTEST_BURST();
void TCPSOCKET_ECHOTEST_BURST_NONBLOCK();
void TCPSOCKET_ENDPOINT_CLOSE();
void TCPSOCKET_OPEN_DESTRUCT();
void TCPSOCKET_OPEN_CLOSE_REPEAT();
void TCPSOCKET_OPEN_LIMIT();
void TCPSOCKET_OPEN_TWICE();
void TCPSOCKET_BIND_PORT();
void TCPSOCKET_BIND_PORT_FAIL();
void TCPSOCKET_BIND_ADDRESS_PORT();
void TCPSOCKET_BIND_ADDRESS_NULL();
void TCPSOCKET_BIND_ADDRESS_INVALID();
void TCPSOCKET_BIND_ADDRESS();
void TCPSOCKET_BIND_WRONG_TYPE();
void TCPSOCKET_BIND_UNOPENED();
void TCPSOCKET_RECV_100K();
void TCPSOCKET_RECV_100K_NONBLOCK();
void TCPSOCKET_RECV_TIMEOUT();
void TCPSOCKET_SEND_REPEAT();
void TCPSOCKET_SEND_TIMEOUT();
void TCPSOCKET_THREAD_PER_SOCKET_SAFETY();
void TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID();

#endif //TCP_TESTS_H
