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

void UDPSOCKET_SENDTO_INVALID()
{
    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    SocketAddress addr;
    addr.set_ip_address("");
    addr.set_port(0);
    // UDP will be able to send 0 bytes, but no particular error is raised
    TEST_ASSERT_EQUAL(sock.sendto(addr, NULL, 0), 0);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &addr));
    addr.set_port(9);
    nsapi_error_t result = sock.sendto(addr, NULL, 0);
    if (result != NSAPI_ERROR_UNSUPPORTED) {
        TEST_ASSERT_EQUAL(0, result);
    }

    TEST_ASSERT_EQUAL(5, sock.sendto(addr, "hello", 5));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
