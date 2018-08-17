/*
 * Copyright (c) 2018, Arm Limited and affiliates
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gtest/gtest.h"
#include "features/netsocket/UDPSocket.h"
#include "features/netsocket/nsapi_dns.h"
#include "NetworkStack_stub.h"

/**
 * This test needs to access a private function
 * which is why it is declared as friend
 * following the documentation of google test.
 * [*](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md)
 */
class stubUDPSocket : public UDPSocket {
    friend class TestUDPSocket;
    FRIEND_TEST(TestUDPSocket, get_proto);
};

class TestUDPSocket : public testing::Test {
protected:
    stubUDPSocket *socket;
    NetworkStackstub stack;

    virtual void SetUp()
    {
        socket = new stubUDPSocket;
        stack.return_value = NSAPI_ERROR_OK;
        socket->open((NetworkStack *)&stack);
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestUDPSocket, get_proto)
{
    EXPECT_EQ(socket->get_proto(), NSAPI_UDP);
}

TEST_F(TestUDPSocket, sendto_addr_port)
{
    stack.return_value = NSAPI_ERROR_PARAMETER;
    EXPECT_EQ(socket->sendto("127.0.0.1", 0, 0, 0), NSAPI_ERROR_DNS_FAILURE);
    stack.return_value = NSAPI_ERROR_OK;
    EXPECT_EQ(socket->sendto("127.0.0.1", 0, 0, 0), 0);
}

TEST_F(TestUDPSocket, sendto)
{
    SocketAddress addr("127.0.0.1", 1024);
    stack.return_value = NSAPI_ERROR_OK;

    socket->open((NetworkStack *)&stack);
    stack.return_value = 100;
    EXPECT_EQ(socket->sendto(addr, 0, 100), 100);

    socket->set_timeout(0);
    stack.return_value = NSAPI_ERROR_WOULD_BLOCK;
    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_WOULD_BLOCK);

    socket->set_timeout(-1);
    stack.return_value = NSAPI_ERROR_NO_CONNECTION;
    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_NO_CONNECTION);
}
