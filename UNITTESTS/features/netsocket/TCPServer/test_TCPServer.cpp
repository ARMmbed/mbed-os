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
#include "features/netsocket/TCPSocket.h"
#include "features/netsocket/TCPServer.h"
#include "NetworkStack_stub.h"

// Control the rtos EventFlags stub. See EventFlags_stub.cpp
extern std::list<uint32_t> eventFlagsStubNextRetval;

class TestTCPServer : public testing::Test {
public:
    unsigned int dataSize = 10;
    char dataBuf[10];
protected:
    TCPSocket *socket;
    TCPServer *server;
    NetworkStackstub stack;

    virtual void SetUp()
    {
        server = new TCPServer();
        socket = new TCPSocket();
    }

    virtual void TearDown()
    {
        stack.return_values.clear();
        eventFlagsStubNextRetval.clear();
        delete socket;
        delete server;
    }
};

TEST_F(TestTCPServer, constructor)
{
    EXPECT_TRUE(server);
}

TEST_F(TestTCPServer, constructor_parameters)
{
    TCPServer serverParam(&stack);
    const SocketAddress a("127.0.0.1", 1024);
    EXPECT_EQ(serverParam.connect(a), NSAPI_ERROR_OK);
}

TEST_F(TestTCPServer, accept)
{
    const SocketAddress a("127.0.0.1", 1024);
    EXPECT_EQ(socket->open(&stack), NSAPI_ERROR_OK);
    EXPECT_EQ(socket->connect(a), NSAPI_ERROR_OK);
    nsapi_error_t error;
    EXPECT_EQ(server->open(&stack), NSAPI_ERROR_OK);
    EXPECT_EQ(server->bind(a), NSAPI_ERROR_OK);
    server->listen(1);
    SocketAddress client_addr;
    EXPECT_EQ(server->accept(socket, &client_addr), NSAPI_ERROR_OK);
}

TEST_F(TestTCPServer, accept_no_socket)
{
    SocketAddress client_addr;
    EXPECT_EQ(server->accept(socket, &client_addr), NSAPI_ERROR_NO_SOCKET);
}

TEST_F(TestTCPServer, accept_error)
{
    SocketAddress client_addr;
    EXPECT_EQ(server->open(&stack), NSAPI_ERROR_OK);
    stack.return_value = NSAPI_ERROR_AUTH_FAILURE;
    EXPECT_EQ(server->accept(socket, &client_addr), NSAPI_ERROR_AUTH_FAILURE);
}

TEST_F(TestTCPServer, accept_error_would_block)
{
    SocketAddress client_addr;
    EXPECT_EQ(server->open(&stack), NSAPI_ERROR_OK);
    stack.return_value = NSAPI_ERROR_WOULD_BLOCK;
    eventFlagsStubNextRetval.push_back(0);
    eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop

    EXPECT_EQ(server->accept(socket, &client_addr), NSAPI_ERROR_WOULD_BLOCK);
}
