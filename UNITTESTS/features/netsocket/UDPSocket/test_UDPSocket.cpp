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
class UDPSocketFriend : public UDPSocket {
    friend class TestUDPSocket;
    FRIEND_TEST(TestUDPSocket, get_proto);
};

// Control the rtos EventFlags stub. See EventFlags_stub.cpp
extern std::list<uint32_t> eventFlagsStubNextRetval;

class TestUDPSocket : public testing::Test {
protected:
    UDPSocket *socket;
    NetworkStackstub stack;
    unsigned int dataSize = 10;
    char dataBuf[10];

    virtual void SetUp()
    {
        socket = new UDPSocket;
        stack.return_value = NSAPI_ERROR_OK;
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestUDPSocket, get_proto)
{
    UDPSocketFriend udpFriend;
    EXPECT_EQ(udpFriend.get_proto(), NSAPI_UDP);
}

TEST_F(TestUDPSocket, sendto_addr_port)
{
    // The code below causes a segfault.  Should we add a check?
    //EXPECT_EQ(socket->sendto("127.0.0.1", 0, 0, 0), NSAPI_ERROR_NO_SOCKET);

    const SocketAddress a("127.0.0.1", 1024);
    EXPECT_EQ(socket->sendto(a, 0, 0), NSAPI_ERROR_NO_SOCKET);

    socket->open(&stack);

    stack.return_value = NSAPI_ERROR_PARAMETER;
    EXPECT_EQ(socket->sendto(a, 0, 0), NSAPI_ERROR_PARAMETER);

    stack.return_value = NSAPI_ERROR_OK;
    EXPECT_EQ(socket->sendto(a, 0, 0), 0);

    EXPECT_EQ(socket->setsockopt(NSAPI_SOCKET, NSAPI_BIND_TO_DEVICE, "12324", 5), NSAPI_ERROR_UNSUPPORTED);
    EXPECT_EQ(socket->sendto(a, 0, 0), 0);
}

TEST_F(TestUDPSocket, connect)
{
    stack.return_value = NSAPI_ERROR_OK;
    const nsapi_addr_t addr = {NSAPI_IPv4, {127, 0, 0, 1} };
    const SocketAddress a(addr, 1024);

    socket->open(&stack);
    EXPECT_EQ(socket->send(dataBuf, dataSize), NSAPI_ERROR_NO_ADDRESS);

    EXPECT_EQ(socket->connect(a), NSAPI_ERROR_OK);

    stack.return_value = 100;
    EXPECT_EQ(socket->send(dataBuf, dataSize), 100);
}

TEST_F(TestUDPSocket, sendto_timeout)
{
    const nsapi_addr_t saddr = {NSAPI_IPv4, {127, 0, 0, 1} };
    const SocketAddress addr(saddr, 1024);

    socket->open(&stack);

    stack.return_value = NSAPI_ERROR_WOULD_BLOCK;
    eventFlagsStubNextRetval.push_back(0);
    eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop
    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_WOULD_BLOCK);

    stack.return_value = NSAPI_ERROR_NO_MEMORY;
    socket->set_timeout(1);

    EXPECT_EQ(socket->sendto(addr, 0, 100), NSAPI_ERROR_NO_MEMORY);
}

TEST_F(TestUDPSocket, recv)
{
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), NSAPI_ERROR_NO_SOCKET);

    socket->open(&stack);

    stack.return_value = 100;
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), 100);

    stack.return_value = NSAPI_ERROR_WOULD_BLOCK;
    eventFlagsStubNextRetval.push_back(0);
    eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), NSAPI_ERROR_WOULD_BLOCK);
}

TEST_F(TestUDPSocket, recv_address_filtering)
{
    socket->open(&stack);
    const nsapi_addr_t addr1 = {NSAPI_IPv4, {127, 0, 0, 1} };
    const nsapi_addr_t addr2 = {NSAPI_IPv4, {127, 0, 0, 2} };
    SocketAddress a1(addr1, 1024);
    SocketAddress a2(addr2, 1024);

    EXPECT_EQ(socket->connect(a1), NSAPI_ERROR_OK);

    stack.return_values.push_back(100); //This will not return, because wrong address is used.
    stack.return_values.push_back(NSAPI_ERROR_NO_MEMORY); //Break the loop of waiting for data from a1.
    EXPECT_EQ(socket->recvfrom(&a2, &dataBuf, dataSize), NSAPI_ERROR_NO_MEMORY);

    stack.return_values.push_back(100);
    EXPECT_EQ(socket->recvfrom(&a1, &dataBuf, dataSize), 100);
}

TEST_F(TestUDPSocket, unsupported_api)
{
    nsapi_error_t error;
    EXPECT_EQ(socket->accept(&error), static_cast<Socket *>(NULL));
    EXPECT_EQ(error, NSAPI_ERROR_UNSUPPORTED);
    EXPECT_EQ(socket->listen(1), NSAPI_ERROR_UNSUPPORTED);
}
