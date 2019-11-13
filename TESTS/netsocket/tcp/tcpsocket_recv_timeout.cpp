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

void TCPSOCKET_RECV_TIMEOUT()
{
    SKIP_IF_TCP_UNSUPPORTED();
    static const int DATA_LEN = 100;
    char buff[DATA_LEN] = {0};
    int time_allotted = split2half_rmng_tcp_test_time(); // [s]
    Timer tc_exec_time;
    tc_exec_time.start();

    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.set_timeout(100);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    int recvd = 0;
    int pkt_unrecvd;
    Timer timer;
    for (int i = 0; i < 5; i++) {
        pkt_unrecvd = DATA_LEN;
        TEST_ASSERT_EQUAL(DATA_LEN, sock.send(buff, DATA_LEN));

        while (pkt_unrecvd) {
            timer.reset();
            timer.start();
            recvd = sock.recv(&(buff[DATA_LEN - pkt_unrecvd]), pkt_unrecvd);
            timer.stop();

            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if (tc_exec_time.read() >= time_allotted ||
                        (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout)) {
                    TEST_FAIL();
                    goto CLEANUP;
                }
                int recv_time_ms = (timer.read_us() + 500) / 1000;
                tr_info("MBED: recv() took: %dus", recv_time_ms);
                if (recv_time_ms > 150) {
                    TEST_ASSERT(150 - recv_time_ms < 51);
                } else {
                    TEST_ASSERT(recv_time_ms - 150 < 51);
                }
                continue;
            } else if (recvd < 0) {
                tr_error("[pkt#%02d] network error %d", i, recvd);
                TEST_FAIL();
                goto CLEANUP;
            }
            pkt_unrecvd -= recvd;
        }
    }

CLEANUP:
    tc_exec_time.stop();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
