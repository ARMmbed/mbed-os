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
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "udp_tests.h"

using namespace utest::v1;

void UDPSOCKET_SENDTO_REPEAT()
{
    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    SocketAddress udp_addr;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK,
                      NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &udp_addr));
    udp_addr.set_port(9);

    int sent;
    Timer timer;
    int i;
    static const char tx_buffer[] = {'h', 'e', 'l', 'l', 'o'};
    bool oom_earlier = false; // 2 times in a row -> time to give up
    for (i = 0; i < 100; i++) {
        sent = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
        if (sent == NSAPI_ERROR_NO_MEMORY || sent == NSAPI_ERROR_BUSY) {
            if (oom_earlier) {
                break;
            }
            oom_earlier = true;
            ThisThread::sleep_for(1000);
            continue;
        }
        oom_earlier = false;
        TEST_ASSERT_EQUAL(sizeof(tx_buffer), sent);
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
