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
#include "features/netsocket/InternetSocket.h"
#include "NetworkStack_stub.h"

class stubInternetSocket : public InternetSocket {
protected:
    nsapi_error_t return_value = 0;
public:
    virtual void event()
    {
        if (_callback) {
            _callback.call();
        }
    }

    virtual nsapi_error_t connect(const SocketAddress &address)
    {
        return return_value;
    }
    virtual nsapi_size_or_error_t send(const void *data, nsapi_size_t size)
    {
        return return_value;
    }
    virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size)
    {
        return return_value;
    }
    virtual nsapi_size_or_error_t sendto(const SocketAddress &address,
                                         const void *data, nsapi_size_t size)
    {
        return return_value;
    }
    virtual nsapi_size_or_error_t recvfrom(SocketAddress *address,
                                           void *data, nsapi_size_t size)
    {
        return return_value;
    }
    virtual Socket *accept(nsapi_error_t *error = NULL)
    {
        return NULL;
    }
    virtual nsapi_error_t listen(int backlog = 1)
    {
        return return_value;
    }
protected:
    virtual nsapi_protocol_t get_proto()
    {
        return NSAPI_TCP;
    }
};

static bool callback_is_called;
static void my_callback()
{
    callback_is_called = true;
}

class TestInternetSocket : public testing::Test {
protected:
    stubInternetSocket *socket;
    NetworkStackstub stack;

    virtual void SetUp()
    {
        socket = new stubInternetSocket;
    }

    virtual void TearDown()
    {
        delete socket;
    }
};

TEST_F(TestInternetSocket, constructor)
{
    EXPECT_TRUE(socket);
}


TEST_F(TestInternetSocket, open_null_stack)
{
    EXPECT_EQ(socket->open(NULL), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestInternetSocket, open_error)
{
    stack.return_value = NSAPI_ERROR_PARAMETER;
    EXPECT_EQ(socket->open((NetworkStack *)&stack), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestInternetSocket, open)
{
    stack.return_value = NSAPI_ERROR_OK;
    EXPECT_EQ(socket->open((NetworkStack *)&stack), NSAPI_ERROR_OK);
}

TEST_F(TestInternetSocket, open_twice)
{
    stack.return_value = NSAPI_ERROR_OK;
    EXPECT_EQ(socket->open((NetworkStack *)&stack), NSAPI_ERROR_OK);
    EXPECT_EQ(socket->open((NetworkStack *)&stack), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestInternetSocket, close)
{
    stack.return_value = NSAPI_ERROR_OK;
    socket->open((NetworkStack *)&stack);
    EXPECT_EQ(socket->close(), NSAPI_ERROR_OK);
}

TEST_F(TestInternetSocket, modify_multicast_group)
{
    SocketAddress a("127.0.0.1", 1024);
    stack.return_value = NSAPI_ERROR_OK;
    socket->open((NetworkStack *)&stack);

    EXPECT_EQ(socket->join_multicast_group(a), NSAPI_ERROR_UNSUPPORTED);
    EXPECT_EQ(socket->leave_multicast_group(a), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestInternetSocket, set_blocking)
{
    socket->set_blocking(false);
    socket->set_blocking(true);
}

TEST_F(TestInternetSocket, setsockopt_no_stack)
{
    socket->close();
    EXPECT_EQ(socket->setsockopt(0, 0, 0, 0), NSAPI_ERROR_NO_SOCKET);
}

TEST_F(TestInternetSocket, sigio)
{
    callback_is_called = false;
    // I'm calling sigio() through the DEPRECATED method, just to get coverage for both.
    // Not sure if this is wise at all, we should not aim for 100%
    socket->sigio(mbed::callback(my_callback));
    socket->event();
    EXPECT_EQ(callback_is_called, true);
}
