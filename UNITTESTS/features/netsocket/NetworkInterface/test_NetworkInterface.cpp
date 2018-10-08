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
public:
    NetworkStackstub stack;
};

class TestNetworkInterface : public testing::Test {
protected:
    NetworkInterface *iface;

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
}

// get_default_instance is tested along with the implementations of NetworkInterface.

TEST_F(TestNetworkInterface, get_mac_address)
{
    char *n = 0;
    EXPECT_EQ(iface->get_mac_address(), n);
}

TEST_F(TestNetworkInterface, get_ip_address)
{
    char *n = 0;
    EXPECT_EQ(iface->get_ip_address(), n);
}

TEST_F(TestNetworkInterface, get_netmask)
{
    char *n = 0;
    EXPECT_EQ(iface->get_netmask(), n);
}

TEST_F(TestNetworkInterface, get_gateway)
{
    char *n = 0;
    EXPECT_EQ(iface->get_gateway(), n);
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
    EXPECT_EQ(iface->add_dns_server(a), NSAPI_ERROR_OK);
}

TEST_F(TestNetworkInterface, get_connection_status)
{
    EXPECT_EQ(iface->get_connection_status(), NSAPI_ERROR_UNSUPPORTED);
}

TEST_F(TestNetworkInterface, set_blocking)
{
    EXPECT_EQ(iface->set_blocking(true), NSAPI_ERROR_UNSUPPORTED);
}

// No way to test attach as it doesn't do or return anything.
