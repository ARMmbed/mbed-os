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
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

void TCPSOCKET_CONNECT_INVALID()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    TEST_ASSERT(sock.connect(NULL, 9) < 0);
    TEST_ASSERT(sock.connect("", 9) < 0);
    TEST_ASSERT(sock.connect("", 0) < 0);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(ECHO_SERVER_ADDR, ECHO_SERVER_DISCARD_PORT));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
