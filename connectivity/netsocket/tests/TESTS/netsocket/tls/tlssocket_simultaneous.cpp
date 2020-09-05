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
#include "TLSSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tls_tests.h"

using namespace utest::v1;

#if defined(MBEDTLS_SSL_CLI_C)

void TLSSOCKET_SIMULTANEOUS()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TLSSocket sock1;
    TLSSocket sock2;
    tlssocket_connect_to_echo_srv(sock1);
    tlssocket_connect_to_echo_srv(sock2);

    int snd;
    int rcvd;
    static const char tx_buffer1[] = {'h', 'e', 'l', 'l', 'o', '1'};
    static const char tx_buffer2[] = {'h', 'e', 'l', 'l', 'o', '2'};
    char rx_buffer1[6];
    char rx_buffer2[6];
    for (int i = 0; i < 10; i++) {
        snd = sock1.send(tx_buffer1, sizeof(tx_buffer1));
        if (snd != sizeof(tx_buffer1)) {
            TEST_FAIL();
            break;
        }
        snd = sock2.send(tx_buffer2, sizeof(tx_buffer2));
        if (snd != sizeof(tx_buffer2)) {
            TEST_FAIL();
            break;
        }

        rcvd = sock1.recv(&rx_buffer1, 6);
        if ((rcvd != 6) || (memcmp(tx_buffer1, rx_buffer1, 6) != 0)) {
            TEST_FAIL();
            break;
        }
        rcvd = sock2.recv(&rx_buffer2, 6);
        if ((rcvd != 6) || (memcmp(tx_buffer2, rx_buffer2, 6) != 0)) {
            TEST_FAIL();
            break;
        }
    }

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock1.close());
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock2.close());
}

#endif // defined(MBEDTLS_SSL_CLI_C)
