/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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

#ifndef NIDD_TESTS_H
#define NIDD_TESTS_H

#include "CellularNonIPSocket.h"
#include "CellularLog.h"
#include "../test_params.h"

NetworkInterface *get_interface();
void drop_bad_packets(CellularNonIPSocket &sock, int orig_timeout);
bool check_oversized_packets(nsapi_error_t error, int &size);
void fill_tx_buffer_ascii(char *buff, size_t len);
void poll_pending_messages(CellularNonIPSocket &sock, int count);

/**
 * Single testcase might take only half of the remaining execution time
 */
int split2half_rmng_nidd_test_time(); // [s]

namespace nidd_global {
#ifdef MBED_GREENTEA_TEST_NIDDSOCKET_TIMEOUT_S
static const int TESTS_TIMEOUT = MBED_GREENTEA_TEST_NIDDSOCKET_TIMEOUT_S;
#else
static const int TESTS_TIMEOUT = (3 * 60);
#endif
static const int SOCKET_SEND_COUNT = 4;
}

/*
 * Test cases
 */
void NIDDSOCKET_CONNECT();
void NIDDSOCKET_DISCONNECT();
void NIDDSOCKET_OPEN_CLOSE_REPEAT();
void NIDDSOCKET_OPEN_LIMIT();
void NIDDSOCKET_RECV_TIMEOUT();
void NIDDSOCKET_SEND_TIMEOUT();
void NIDDSOCKET_OPEN_DESTRUCT();
void NIDDSOCKET_OPEN_TWICE();
void NIDDSOCKET_SEND_INVALID();
void NIDDSOCKET_ECHOTEST_NONBLOCK();
void NIDDSOCKET_SEND_REPEAT();

#endif //NIDD_TESTS_H
