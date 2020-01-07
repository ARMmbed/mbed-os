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
#include "features/netsocket/NetworkStack.h"
#include "netsocket/nsapi_dns.h"
#include "events/EventQueue.h"
#include <string>

#include "equeue_stub.h"

#define EXPECTED_DNS_RESULT 1

// Control nsapi stub return value. See stubs/nsapi_dns_stub.cpp
extern nsapi_error_t nsapi_stub_return_value;

// For testing async calls which require callback
static bool callback_is_called;
static SocketAddress address_received;
static nsapi_error_t result_received;
static void my_callback(nsapi_error_t result, SocketAddress *address)
{
    result_received = result;
    address_received = *address;
    callback_is_called = true;
}

static bool noarg_callback_is_called;
static void noarg_callback(void)
{
    noarg_callback_is_called = true;
}

extern call_in_callback_cb_t callin_callback;

namespace mbed {
extern events::EventQueue *mbed_shared_queue_stub;
}

// NetworkStack is an abstract class we need to provide a child class for tests.
class NetworkStackChild : public NetworkStack {
    virtual nsapi_error_t socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t socket_close(nsapi_socket_t handle)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t socket_bind(nsapi_socket_t handle, const SocketAddress &address)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address = 0)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_size_or_error_t socket_send(nsapi_socket_t handle,
                                              const void *data, nsapi_size_t size)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_size_or_error_t socket_recv(nsapi_socket_t handle,
                                              void *data, nsapi_size_t size)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_size_or_error_t socket_sendto(nsapi_socket_t handle, const SocketAddress &address,
                                                const void *data, nsapi_size_t size)
    {
        return NSAPI_ERROR_OK;
    }
    virtual nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t handle, SocketAddress *address,
                                                  void *buffer, nsapi_size_t size)
    {
        return NSAPI_ERROR_OK;
    }
    virtual void socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data)
    {
    }
public:
    std::string ip_address;
    const char *get_ip_address()
    {
        return ip_address.c_str();

    }
};

class TestNetworkStack : public testing::Test {
protected:
    NetworkStackChild *stack;

    virtual void SetUp()
    {
        stack = new NetworkStackChild();
        address_received = SocketAddress();
        result_received = NSAPI_ERROR_OK;
        callback_is_called = false;
        noarg_callback_is_called = false;
        mbed::mbed_shared_queue_stub = 0;
    }

    virtual void TearDown()
    {
        delete stack;
        if (mbed::mbed_shared_queue_stub) {
            delete mbed::mbed_shared_queue_stub;
            mbed::mbed_shared_queue_stub = 0;
        }
    }

    void SetUpQueue()
    {
        mbed::mbed_shared_queue_stub = new events::EventQueue();
    }
};

TEST_F(TestNetworkStack, constructor)
{
    EXPECT_TRUE(stack);
}

TEST_F(TestNetworkStack, get_ip_address_default)
{
    SocketAddress a;
    EXPECT_EQ(stack->NetworkStack::get_ip_address(&a), NSAPI_ERROR_UNSUPPORTED);
}

/* gethostbyname */

TEST_F(TestNetworkStack, gethostbyname)
{
    SocketAddress a;
    stack->ip_address = std::string("127.0.0.1");
    EXPECT_EQ(stack->gethostbyname("host", &a, NSAPI_UNSPEC), NSAPI_ERROR_DNS_FAILURE);
}

TEST_F(TestNetworkStack, gethostbyname_simple_address)
{
    SocketAddress a;
    EXPECT_EQ(stack->gethostbyname("127.0.0.1", &a, NSAPI_UNSPEC), NSAPI_ERROR_OK);
}

TEST_F(TestNetworkStack, gethostbyname_simple_address_right_version)
{
    SocketAddress a;
    EXPECT_EQ(stack->gethostbyname("127.0.0.1", &a, NSAPI_IPv4), NSAPI_ERROR_OK);
}

TEST_F(TestNetworkStack, gethostbyname_simple_address_wrong_version)
{
    SocketAddress a;
    EXPECT_EQ(stack->gethostbyname("127.0.0.1", &a, NSAPI_IPv6), NSAPI_ERROR_DNS_FAILURE);
}

TEST_F(TestNetworkStack, gethostbyname_empty_host)
{
    SocketAddress a;
    EXPECT_EQ(stack->gethostbyname("", &a, NSAPI_UNSPEC), NSAPI_ERROR_PARAMETER);
}

/* gethostbyname_async */

TEST_F(TestNetworkStack, gethostbyname_async_delay)
{
    SocketAddress a;
    stack->ip_address = std::string("127.0.0.1");
    SetUpQueue();
    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    EXPECT_EQ(stack->gethostbyname_async("localhost", mbed::callback(my_callback), NSAPI_UNSPEC), NSAPI_ERROR_DNS_FAILURE);
    EXPECT_EQ(callin_callback(1, mbed::callback(noarg_callback)), NSAPI_ERROR_OK);
    EXPECT_TRUE(noarg_callback_is_called);
    EXPECT_FALSE(callback_is_called);
}

TEST_F(TestNetworkStack, gethostbyname_async)
{
    SocketAddress a;
    stack->ip_address = std::string("127.0.0.1");
    SetUpQueue();
    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    EXPECT_EQ(stack->gethostbyname_async("localhost", mbed::callback(my_callback), NSAPI_UNSPEC), NSAPI_ERROR_DNS_FAILURE);
    EXPECT_EQ(callin_callback(0, mbed::callback(noarg_callback)), NSAPI_ERROR_OK);
    EXPECT_TRUE(noarg_callback_is_called);
    EXPECT_FALSE(callback_is_called);
}

TEST_F(TestNetworkStack, gethostbyname_async_eventqueue_simple_address)
{
    SocketAddress a;
    stack->ip_address = std::string("127.0.0.1");
    EXPECT_EQ(stack->gethostbyname_async("127.0.0.1", mbed::callback(my_callback), NSAPI_IPv6), NSAPI_ERROR_DNS_FAILURE);
    EXPECT_FALSE(callback_is_called);
    EXPECT_EQ(stack->gethostbyname_async("127.0.0.1", mbed::callback(my_callback), NSAPI_IPv4), NSAPI_ERROR_OK);
    EXPECT_TRUE(callback_is_called);
    EXPECT_EQ(result_received, EXPECTED_DNS_RESULT);
    EXPECT_EQ(std::string(address_received.get_ip_address()), stack->ip_address);
}

TEST_F(TestNetworkStack, gethostbyname_async_empty_host)
{
    SocketAddress a;
    EXPECT_EQ(stack->gethostbyname_async("", mbed::callback(my_callback), NSAPI_UNSPEC), NSAPI_ERROR_PARAMETER);
}

TEST_F(TestNetworkStack, getstackopt)
{
    EXPECT_EQ(stack->getstackopt(0, 0, 0, 0), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkStack, setstackopt)
{
    EXPECT_EQ(stack->setstackopt(0, 0, 0, 0), NSAPI_ERROR_UNSUPPORTED);
}

