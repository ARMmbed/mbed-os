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
#include "features/netsocket/DTLSSocket.h"
#include "NetworkStack_stub.h"

#include "mbed_error.h"
mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    return 0;
}

// Control the rtos EventFlags stub. See EventFlags_stub.cpp
extern std::list<uint32_t> eventFlagsStubNextRetval;

class TestDTLSSocket : public testing::Test {
public:
    unsigned int dataSize = 10;
    char dataBuf[10];
protected:
    DTLSSocket *socket;
    NetworkStackstub stack;

    virtual void SetUp()
    {
        socket = new DTLSSocket();
    }

    virtual void TearDown()
    {
        stack.return_values.clear();
        eventFlagsStubNextRetval.clear();
        delete socket;
    }
};

TEST_F(TestDTLSSocket, constructor)
{
    EXPECT_TRUE(socket);
}

/* connect */

TEST_F(TestDTLSSocket, connect)
{
    socket->open(&stack);

    stack.return_value = NSAPI_ERROR_OK;
    SocketAddress a("127.0.0.1", 1024);
    EXPECT_EQ(socket->connect(a), NSAPI_ERROR_OK);
}
