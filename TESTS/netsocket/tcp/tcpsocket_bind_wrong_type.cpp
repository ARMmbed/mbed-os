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
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "tcp_tests.h"
#include "TCPSocket.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

void TCPSOCKET_BIND_WRONG_TYPE()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));
    char addr_bytes[16] = {0xfe, 0x80, 0xff, 0x1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    SocketAddress sockAddr;
    if (get_ip_version() == NSAPI_IPv4) {
        sockAddr = SocketAddress(addr_bytes, NSAPI_IPv4, 80);
    } else if (get_ip_version() == NSAPI_IPv6) {
        sockAddr = SocketAddress(addr_bytes, NSAPI_IPv6, 80);
    } else {
        TEST_FAIL_MESSAGE("This stack is neither IPv4 nor IPv6");
    }
    nsapi_error_t bind_result = sock.bind(sockAddr);
    if (bind_result == NSAPI_ERROR_UNSUPPORTED) {
        TEST_IGNORE_MESSAGE("bind() not supported");
    } else {
        TEST_ASSERT_EQUAL(NSAPI_ERROR_PARAMETER, bind_result);
    }
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
