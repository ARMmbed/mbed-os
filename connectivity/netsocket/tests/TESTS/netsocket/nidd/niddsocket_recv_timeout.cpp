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
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "nidd_tests.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO = 0x1;
static const int SIGIO_TIMEOUT = 1000; //[ms]
static const int PKT_NUM = 2;
}

static void _sigio_handler(osThreadId id)
{
    osSignalSet(id, SIGNAL_SIGIO);
}

void NIDDSOCKET_RECV_TIMEOUT()
{
    CellularNonIPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(CellularContext::get_default_nonip_instance()));
    sock.set_timeout(100);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    static const int DATA_LEN = 4;
    char buff[DATA_LEN] = {0};
    int recvd;
    Timer timer;
    int pkt_success = 0;
    for (int i = 0; i < PKT_NUM; i++) {
        memset(buff, 'A', sizeof(buff));
        TEST_ASSERT_EQUAL(DATA_LEN, sock.send(buff, DATA_LEN));
        timer.reset();
        timer.start();
        recvd = sock.recv(buff, sizeof(buff));
        timer.stop();

        if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
            osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT);
            if (timer.read_ms() > 150) {
                TEST_ASSERT(150 - timer.read_ms() < 51);
            } else {
                TEST_ASSERT(timer.read_ms() - 150 < 51);
            }
            continue;
        } else if (recvd < 0) {
            tr_info("[bt#%02d] network error %d\n", i, recvd);
            continue;
        }
        TEST_ASSERT_EQUAL(DATA_LEN, recvd);
        pkt_success++;
    }
    poll_pending_messages(sock, PKT_NUM);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
