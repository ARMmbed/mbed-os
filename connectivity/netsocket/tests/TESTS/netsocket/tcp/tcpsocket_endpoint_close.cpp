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

#if defined(MBED_CONF_RTOS_PRESENT)
#include "mbed.h"
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO = 0x1;
static const int SIGIO_TIMEOUT = 20000; //[ms]
}

static void _sigio_handler(osThreadId id)
{
    osSignalSet(id, SIGNAL_SIGIO);
}

static nsapi_error_t _tcpsocket_connect_to_daytime_srv(TCPSocket &sock)
{
    SocketAddress tcp_addr;

    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(13);

    nsapi_error_t err = sock.open(NetworkInterface::get_default_instance());
    if (err != NSAPI_ERROR_OK) {
        return err;
    }

    return sock.connect(tcp_addr);
}

void TCPSOCKET_ENDPOINT_CLOSE()
{
    SKIP_IF_TCP_UNSUPPORTED();
    static const int MORE_THAN_AVAILABLE = 30;
    char buff[MORE_THAN_AVAILABLE];
    int time_allotted = split2half_rmng_tcp_test_time(); // [s]
    Timer tc_exec_time;
    tc_exec_time.start();

    TCPSocket sock;
    if (_tcpsocket_connect_to_daytime_srv(sock) != NSAPI_ERROR_OK) {
        TEST_FAIL();
        return;
    }
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    int recvd = 0;
    int recvd_total = 0;
    while (true) {
        recvd = sock.recv(buff, MORE_THAN_AVAILABLE);
        if (recvd_total > 0 && recvd == 0) {
            break; // Endpoint closed socket, success
        } else if (recvd <= 0) {
            TEST_ASSERT_EQUAL(0, recvd);
            break;
        } else if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
            if (tc_exec_time.read() >= time_allotted ||
                    osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                TEST_FAIL();
                break;
            }
            continue;
        }
        recvd_total += recvd;
        TEST_ASSERT(recvd_total < MORE_THAN_AVAILABLE);
    }
    tc_exec_time.stop();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
