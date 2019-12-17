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
#include "CellularNonIPSocket.h"
#include "myCellularContext.h"

using namespace mbed;

// Control the rtos EventFlags stub. See EventFlags_stub.cpp
extern std::list<uint32_t> eventFlagsStubNextRetval;

static bool callback_is_called;
static void my_callback()
{
    callback_is_called = true;
}

class TestCellularNonIPSocket : public testing::Test {
protected:
    CellularNonIPSocket *socket;
    ControlPlane_netif_stub *cp_netif;
    myCellularContext cellular_context;
    nsapi_size_t dataSize;
    char dataBuf[10];

    virtual void SetUp()
    {
        socket = new CellularNonIPSocket();
        cp_netif = NULL;
        dataSize = 10;
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestCellularNonIPSocket, open_null_cp_netif)
{
    EXPECT_EQ(socket->open(static_cast<ControlPlane_netif *>(NULL)), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestCellularNonIPSocket, open_null_context)
{
    EXPECT_EQ(socket->open(static_cast<CellularContext *>(NULL)), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestCellularNonIPSocket, open_context)
{
    EXPECT_EQ(socket->open((CellularContext *)&cellular_context), NSAPI_ERROR_OK);
}

TEST_F(TestCellularNonIPSocket, open_cp_netif)
{
    cp_netif = cellular_context.get_cp_netif();
    EXPECT_EQ(socket->open((ControlPlane_netif *)cp_netif), NSAPI_ERROR_OK);
}

TEST_F(TestCellularNonIPSocket, open_twice)
{
    EXPECT_EQ(socket->open((CellularContext *)&cellular_context), NSAPI_ERROR_OK);
    EXPECT_EQ(socket->open((CellularContext *)&cellular_context), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestCellularNonIPSocket, close)
{
    socket->open((CellularContext *)&cellular_context);
    EXPECT_EQ(socket->close(), NSAPI_ERROR_OK);
}

TEST_F(TestCellularNonIPSocket, close_no_open)
{
    EXPECT_EQ(socket->close(), NSAPI_ERROR_NO_SOCKET);
}

TEST_F(TestCellularNonIPSocket, sigio)
{
    callback_is_called = false;
    socket->open((CellularContext *)&cellular_context);
    socket->sigio(mbed::callback(my_callback));
    socket->close(); // Trigger event;
    EXPECT_EQ(callback_is_called, true);
}

/* send */

TEST_F(TestCellularNonIPSocket, send_no_open)
{
    EXPECT_EQ(socket->send((char *)dataBuf, dataSize), NSAPI_ERROR_NO_SOCKET);
}

TEST_F(TestCellularNonIPSocket, send_error_would_block)
{
    socket->open((CellularContext *)&cellular_context);
    cp_netif = cellular_context.get_cp_netif();
    cp_netif->return_value = NSAPI_ERROR_WOULD_BLOCK;
    eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop
    EXPECT_EQ(socket->send(dataBuf, dataSize), NSAPI_ERROR_WOULD_BLOCK);
}

TEST_F(TestCellularNonIPSocket, send_error_other)
{
    socket->open((CellularContext *)&cellular_context);
    cp_netif = cellular_context.get_cp_netif();
    cp_netif->return_value = NSAPI_ERROR_NO_MEMORY;
    EXPECT_EQ(socket->send(dataBuf, dataSize), NSAPI_ERROR_NO_MEMORY);
}

TEST_F(TestCellularNonIPSocket, send_error_no_timeout)
{
    socket->open((CellularContext *)&cellular_context);
    cp_netif = cellular_context.get_cp_netif();
    cp_netif->return_value = NSAPI_ERROR_WOULD_BLOCK;
    socket->set_blocking(false);
    EXPECT_EQ(socket->send(dataBuf, dataSize), NSAPI_ERROR_WOULD_BLOCK);
}

TEST_F(TestCellularNonIPSocket, send)
{
    socket->open((CellularContext *)&cellular_context);
    cp_netif = cellular_context.get_cp_netif();
    cp_netif->return_value = dataSize;
    EXPECT_EQ(socket->send(dataBuf, dataSize), dataSize);
}

TEST_F(TestCellularNonIPSocket, recv)
{
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), NSAPI_ERROR_NO_SOCKET);

    socket->open((CellularContext *)&cellular_context);
    cp_netif = cellular_context.get_cp_netif();

    cp_netif->return_value = 100;
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), 100);

    cp_netif->return_value = NSAPI_ERROR_WOULD_BLOCK;
    eventFlagsStubNextRetval.push_back(0);
    eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop
    EXPECT_EQ(socket->recv(&dataBuf, dataSize), NSAPI_ERROR_WOULD_BLOCK);
}
