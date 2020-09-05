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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "nidd_tests.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

void NIDDSOCKET_SEND_REPEAT()
{
    CellularNonIPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(CellularContext::get_default_nonip_instance()));

    int sent;
    Timer timer;
    int i;
    static const char tx_buffer[] = {'h', 'e', 'l', 'l', 'o'};
    bool oom_earlier = false; // 2 times in a row -> time to give up
    for (i = 0; i < nidd_global::SOCKET_SEND_COUNT; i++) {
        sent = sock.send(tx_buffer, sizeof(tx_buffer));
        if (sent == NSAPI_ERROR_NO_MEMORY) {
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
    poll_pending_messages(sock, nidd_global::SOCKET_SEND_COUNT);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
