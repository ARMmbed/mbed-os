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

void TCPSOCKET_CONNECT_INVALID()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    SocketAddress address;
    address.set_port(9);

    TEST_ASSERT_FALSE(address.set_ip_address(NULL));

    // Valid address for the final check
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &address));
    address.set_port(ECHO_SERVER_DISCARD_PORT);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(address));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
