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

void TCPSOCKET_RECV_TIMEOUT()
{
    static const int DATA_LEN = 100;
    char buff[DATA_LEN] = {0};

    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.set_timeout(100);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    int recvd = 0;
    int pkt_unrecvd;
    Timer timer;
    for (int i = 0; i < 5; i++) {
        pkt_unrecvd = DATA_LEN;
        TEST_ASSERT_EQUAL(DATA_LEN, sock.send(buff, DATA_LEN));

        while (pkt_unrecvd) {
            timer.reset();
            timer.start();
            recvd = sock.recv(&(buff[DATA_LEN-pkt_unrecvd]), pkt_unrecvd);
            timer.stop();

            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                }
                printf("MBED: recv() took: %dms\n", timer.read_ms());
                TEST_ASSERT_INT_WITHIN(50, 150, timer.read_ms());
                continue;
            } else if (recvd < 0) {
                printf("[pkt#%02d] network error %d\n", i, recvd);
                TEST_FAIL();
            }
            pkt_unrecvd -= recvd;
        }
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
