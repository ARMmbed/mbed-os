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

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

namespace
{
    static const int SIGNAL_SIGIO = 0x1;
    static const int SIGIO_TIMEOUT = 5000; //[ms]
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

static void _tcpsocket_connect_to_daytime_srv(TCPSocket& sock) {
    SocketAddress tcp_addr;

    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(13);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(tcp_addr));
}


void TCPSOCKET_ENDPOINT_CLOSE()
{
    static const int MORE_THAN_AVAILABLE = 30;
    char buff[MORE_THAN_AVAILABLE];

    TCPSocket sock;
    _tcpsocket_connect_to_daytime_srv(sock);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    int recvd = 0;
    int recvd_total = 0;
    while (true) {
         recvd = sock.recv(&(buff[recvd_total]), MORE_THAN_AVAILABLE);
         if (recvd_total > 0 && recvd == 0) {
             break; // Endpoint closed socket, success
         } else if (recvd == 0) {
            TEST_FAIL();
         } else if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
             if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                 TEST_FAIL();
             }
             continue;
         }
         recvd_total += recvd;
         TEST_ASSERT(recvd_total < MORE_THAN_AVAILABLE);
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
