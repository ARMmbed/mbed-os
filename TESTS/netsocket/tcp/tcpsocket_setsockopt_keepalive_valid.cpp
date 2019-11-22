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

void TCPSOCKET_SETSOCKOPT_KEEPALIVE_VALID()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));
    int32_t seconds = 7200;

    int ret = sock.setsockopt(NSAPI_SOCKET, NSAPI_KEEPALIVE, &seconds, sizeof(int));

    if (ret == NSAPI_ERROR_UNSUPPORTED) {
        TEST_IGNORE_MESSAGE("NSAPI_KEEPALIVE option not supported");
        sock.close();
        return;
    }

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, ret);
    SocketAddress address;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &address));
    address.set_port(9);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(address));
    // LWIP stack does not support getsockopt so the part below is commented out
    //    int32_t optval;
    //    unsigned int optlen;
    //    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.getsockopt(NSAPI_SOCKET, NSAPI_KEEPALIVE, &optval, &optlen));
    //    TEST_ASSERT_EQUAL(optlen, sizeof(seconds));
    //    TEST_ASSERT_EQUAL(optval, seconds);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
