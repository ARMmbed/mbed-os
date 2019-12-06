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
#include "features/netsocket/NetworkInterface.h"
#include "NetworkStack_stub.h"

class stubNetworkInterface : public NetworkInterface {
public:
    virtual nsapi_error_t connect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual nsapi_error_t disconnect()
    {
        return NSAPI_ERROR_OK;
    };
    virtual NetworkStack *get_stack()
    {
        return &stack;
    };
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> cb)
    {
        status_cb = cb;
    }
    void event(nsapi_event_t e, intptr_t i)
    {
        status_cb(e, i);
    }
private:
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb;
    NetworkStackstub stack;
};

class TestNetworkInterface : public testing::Test {
protected:
    stubNetworkInterface *iface;

    virtual void SetUp()
    {
        iface = new stubNetworkInterface();
    }

    virtual void TearDown()
    {
        delete iface;
    }
};

TEST_F(TestNetworkInterface, constructor)
{
    EXPECT_TRUE(iface);
    iface->set_as_default(); //Empty function. Just trigger it, so it doesn't obscure coverage reports.
}

// get_default_instance is tested along with the implementations of NetworkInterface.

TEST_F(TestNetworkInterface, get_mac_address)
{
    char *n = 0;
    EXPECT_EQ(iface->get_mac_address(), n);
}

TEST_F(TestNetworkInterface, get_ip_address)
{
    SocketAddress addr;
    EXPECT_EQ(iface->get_ip_address(&addr), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, get_netmask)
{
    SocketAddress addr;
    EXPECT_EQ(iface->get_netmask(&addr), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, get_gateway)
{
    SocketAddress addr;
    EXPECT_EQ(iface->get_gateway(&addr), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, get_interface_name)
{
    char *n = 0;
    EXPECT_EQ(iface->get_interface_name(n), n);
}

TEST_F(TestNetworkInterface, set_network)
{
    EXPECT_EQ(iface->set_network("127.0.0.1", "255.255.0.0", "127.0.0.1"), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, set_dhcp)
{
    EXPECT_EQ(iface->set_dhcp(true), NSAPI_ERROR_OK);
    EXPECT_EQ(iface->set_dhcp(false), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, gethostbyname)
{
    SocketAddress a;
    EXPECT_EQ(iface->gethostbyname("host", &a, NSAPI_UNSPEC), NSAPI_ERROR_OK);
}


static bool callback_is_called;
static void my_callback(nsapi_error_t result, SocketAddress *address)
{
    (void)result;
    (void)address;
    callback_is_called = true;
}

TEST_F(TestNetworkInterface, gethostbyname_async)
{
    SocketAddress a;
    EXPECT_EQ(iface->gethostbyname_async("host", mbed::callback(my_callback), NSAPI_UNSPEC), NSAPI_ERROR_OK);
    EXPECT_EQ(iface->gethostbyname_async_cancel(1), NSAPI_ERROR_OK);
}

TEST_F(TestNetworkInterface, add_dns_server)
{
    SocketAddress a("127.0.0.1", 1024);
    EXPECT_EQ(iface->add_dns_server(a, ""), NSAPI_ERROR_OK);
}

TEST_F(TestNetworkInterface, get_connection_status)
{
    EXPECT_EQ(iface->get_connection_status(), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, set_blocking)
{
    EXPECT_EQ(iface->set_blocking(true), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, get_ipv6_link_local_address)
{
    SocketAddress a;
    EXPECT_EQ(iface->get_ipv6_link_local_address(&a), NSAPI_ERROR_UNSUPPORTED);
}

void my_iface_callback(nsapi_event_t e, intptr_t i)
{
    (void)e;
    (void)i;
    callback_is_called = true;
}
static bool second_callback_called;
void my_iface_callback2(nsapi_event_t e, intptr_t i)
{
    (void)e;
    (void)i;
    second_callback_called = true;
}

TEST_F(TestNetworkInterface, add_event_listener)
{
    callback_is_called = false;
    second_callback_called = false;
    iface->add_event_listener(my_iface_callback);
    iface->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, true);

    iface->remove_event_listener(my_iface_callback);
}

TEST_F(TestNetworkInterface, remove_event_listener)
{
    // Add two callbacks and check that both are called
    callback_is_called = false;
    second_callback_called = false;
    iface->add_event_listener(my_iface_callback);
    iface->add_event_listener(my_iface_callback2);
    iface->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, true);
    EXPECT_EQ(second_callback_called, true);

    // Remove one of the callbacks
    iface->remove_event_listener(my_iface_callback2);
    callback_is_called = false;
    second_callback_called = false;

    // expect only the one is called which remains in the list
    iface->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, true);
    EXPECT_EQ(second_callback_called, false);

    // Remove also the last callback, and expect nothing is called
    iface->remove_event_listener(my_iface_callback);
    callback_is_called = false;
    second_callback_called = false;
    iface->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, false);
    EXPECT_EQ(second_callback_called, false);
}

TEST_F(TestNetworkInterface, correct_event_listener_per_interface)
{
    stubNetworkInterface *iface2 = new stubNetworkInterface();
    iface->add_event_listener(my_iface_callback);
    iface2->add_event_listener(my_iface_callback2);

    callback_is_called = false;
    second_callback_called = false;
    iface->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, true);
    EXPECT_EQ(second_callback_called, false);

    callback_is_called = false;
    second_callback_called = false;
    iface2->event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, 0);
    EXPECT_EQ(callback_is_called, false);
    EXPECT_EQ(second_callback_called, true);

    iface->remove_event_listener(my_iface_callback);
    // Do not call iface2->remove_event_listener, so the destructor can take care of this.
    delete iface2;
}
