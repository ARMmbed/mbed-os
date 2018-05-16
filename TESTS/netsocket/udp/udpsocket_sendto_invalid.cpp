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

void UDPSOCKET_SENDTO_INVALID()
{
    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));

    TEST_ASSERT(sock.sendto(NULL, 9, NULL, 0) < 0);
    TEST_ASSERT(sock.sendto("", 9, NULL, 0) < 0);
    TEST_ASSERT(sock.sendto("", 0, NULL, 0) < 0);
    TEST_ASSERT_EQUAL(5, sock.sendto(MBED_CONF_APP_ECHO_SERVER_ADDR, 0, "hello", 5));
    TEST_ASSERT_EQUAL(0, sock.sendto(MBED_CONF_APP_ECHO_SERVER_ADDR, 9, NULL, 0));
    TEST_ASSERT_EQUAL(5, sock.sendto(MBED_CONF_APP_ECHO_SERVER_ADDR, 9, "hello", 5));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
