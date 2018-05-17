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
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "udp_tests.h"

using namespace utest::v1;

namespace
{
    static const int SIGNAL_SIGIO = 0x1;
    static const int SIGIO_TIMEOUT = 5000; //[ms]
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

void UDPSOCKET_RECV_TIMEOUT()
{
    SocketAddress udp_addr;
    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);

    static const int DATA_LEN = 100;
    char buff[DATA_LEN] = {0};

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    sock.set_timeout(100);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    int recvd;
    Timer timer;
    SocketAddress temp_addr;
    int pkt_success = 0;
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(DATA_LEN, sock.sendto(udp_addr, buff, DATA_LEN));
        timer.reset();
        timer.start();
        recvd = sock.recvfrom(&temp_addr, buff, sizeof(buff));
        timer.stop();

        if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
            osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT);
            printf("MBED: recvfrom() took: %dms\n", timer.read_ms());
            TEST_ASSERT_INT_WITHIN(50, 150, timer.read_ms());
            continue;
        } else if (recvd < 0) {
            printf("[bt#%02d] network error %d\n", i, recvd);
            continue;
        } else if (temp_addr != udp_addr) {
            printf("[bt#%02d] packet from wrong address\n", i);
            continue;
        }
        TEST_ASSERT_EQUAL(DATA_LEN, recvd);
        pkt_success++;
    }

    TEST_ASSERT(pkt_success >= 5);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
