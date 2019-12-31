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

#ifndef UDP_TESTS_H
#define UDP_TESTS_H

#include "../test_params.h"
#include "mbed_trace.h"

#define TRACE_GROUP "GRNT"

NetworkInterface *get_interface();
void drop_bad_packets(UDPSocket &sock, int orig_timeout);
nsapi_version_t get_ip_version();
bool check_oversized_packets(nsapi_error_t error, int &size);
void fill_tx_buffer_ascii(char *buff, size_t len);

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
extern mbed_stats_socket_t udp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
int fetch_stats(void);
#endif

/**
 * Single testcase might take only half of the remaining execution time
 */
int split2half_rmng_udp_test_time(); // [s]

namespace udp_global {
#ifdef MBED_GREENTEA_TEST_UDPSOCKET_TIMEOUT_S
static const int TESTS_TIMEOUT = MBED_GREENTEA_TEST_UDPSOCKET_TIMEOUT_S;
#else
#define MESH 3
#define WISUN 0x2345
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH && MBED_CONF_NSAPI_DEFAULT_MESH_TYPE == WISUN
static const int TESTS_TIMEOUT = (25 * 60);
#else
static const int TESTS_TIMEOUT = (20 * 60);
#endif
#endif

static const int MAX_SEND_SIZE_IPV4 = 536;
static const int MAX_SEND_SIZE_IPV6 = 1220;
}

/*
 * Test cases
 */
void UDPSOCKET_ECHOTEST();
void UDPSOCKET_ECHOTEST_CONNECT_SEND_RECV();
void UDPSOCKET_ECHOTEST_NONBLOCK();
void UDPSOCKET_ECHOTEST_NONBLOCK_CONNECT_SEND_RECV();
void UDPSOCKET_ECHOTEST_BURST();
void UDPSOCKET_ECHOTEST_BURST_NONBLOCK();
void UDPSOCKET_OPEN_CLOSE_REPEAT();
void UDPSOCKET_OPEN_DESTRUCT();
void UDPSOCKET_OPEN_LIMIT();
void UDPSOCKET_OPEN_TWICE();
void UDPSOCKET_BIND_PORT();
void UDPSOCKET_BIND_PORT_FAIL();
void UDPSOCKET_BIND_ADDRESS_PORT();
void UDPSOCKET_BIND_ADDRESS_NULL();
void UDPSOCKET_BIND_ADDRESS_INVALID();
void UDPSOCKET_BIND_ADDRESS();
void UDPSOCKET_BIND_WRONG_TYPE();
void UDPSOCKET_BIND_UNOPENED();
void UDPSOCKET_RECV_TIMEOUT();
void UDPSOCKET_SENDTO_INVALID();
void UDPSOCKET_SENDTO_REPEAT();
void UDPSOCKET_SENDTO_TIMEOUT();

#endif //UDP_TESTS_H
