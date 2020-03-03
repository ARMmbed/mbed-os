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
#include "gmock/gmock.h"

#include "OnboardNetworkStack_mock.h"

#include "features/netsocket/PPPInterface.h"
#include <iostream>
#include "FileHandle_stub.h"


OnboardNetworkStack &OnboardNetworkStack::get_default_instance()
{
    return OnboardNetworkStackMock::get_instance();
}

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

class MockPPP : public PPP {
public:
    MOCK_METHOD0(power_up, bool());
    MOCK_METHOD0(power_down, void());
    MOCK_METHOD0(get_mtu_size, uint32_t());
    MOCK_CONST_METHOD0(get_align_preference, uint32_t());
    MOCK_CONST_METHOD2(get_ifname, void(char *name, uint8_t size));
    MOCK_METHOD2(link_out, bool(net_stack_mem_buf_t *buf, nsapi_ip_stack_t ip_stack));
    MOCK_METHOD1(set_stream, void(mbed::FileHandle *stream));
    MOCK_METHOD1(set_ip_stack, void(nsapi_ip_stack_t ip_stack));
    MOCK_METHOD2(set_credentials, void(const char *uname, const char *password));
    MOCK_METHOD1(get_ip_address, const nsapi_addr_t *(nsapi_version_t version));
    MOCK_METHOD0(get_netmask, const nsapi_addr_t *());
    MOCK_METHOD0(get_gateway, const nsapi_addr_t *());
    MOCK_METHOD1(get_dns_server, const nsapi_addr_t *(uint8_t index));
    MOCK_METHOD1(set_link_input_cb, void(ppp_link_input_cb_t input_cb));
    MOCK_METHOD1(set_link_state_cb, void(ppp_link_state_change_cb_t state_cb));
    MOCK_METHOD1(set_memory_manager, void(NetStackMemoryManager &mem_mngr));

    static MockPPP &get_instance()
    {
        static MockPPP pppMock1;
        return pppMock1;
    }
};

MBED_WEAK PPP &PPP::get_default_instance()
{
    return MockPPP::get_instance();
}

class TestPPPInterface: public testing::Test {
protected:
    PPPInterface *iface;
    OnboardNetworkStackMock *stackMock;
    MockPPP *pppMock;
    OnboardNetworkStackMock::InterfaceMock *netStackIface;
    virtual void SetUp()
    {
        stackMock = &OnboardNetworkStackMock::get_instance();
        pppMock = &MockPPP::get_instance();
        netStackIface = &OnboardNetworkStackMock::InterfaceMock::get_instance();
        iface = new PPPInterface(MockPPP::get_instance(), OnboardNetworkStackMock::get_instance());
    }

    virtual void TearDown()
    {
        // Do not delete the mocks pointers, as they point to statically allocated singletons.
        delete iface;
    }

    /* Enclose the heavily-used connection procedure to improve code redability */
    void doConnect(bool blocking = true)
    {
        EXPECT_CALL(*pppMock, set_stream(_));
        EXPECT_CALL(*pppMock, set_ip_stack(_));
        EXPECT_CALL(*pppMock, set_credentials(_, _));
        EXPECT_CALL(*stackMock, add_ppp_interface(testing::Ref(*pppMock), true, _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_OK)));
        EXPECT_CALL(*netStackIface, attach(_))
        .Times(1)
        .RetiresOnSaturation();
        EXPECT_CALL(*netStackIface, bringup(false, NULL, NULL, NULL, DEFAULT_STACK, blocking))
        .Times(1)
        .WillOnce(Return(NSAPI_ERROR_OK));
        EXPECT_EQ(NSAPI_ERROR_OK, iface->connect());
    }

    static void cb(nsapi_event_t ev, intptr_t ptr)
    {

    }
};

#if 0
/* Test is invalid, as it's working on a PPPInterface pointer rather than a NetworkInterface pointer. */
/* NetworkInterface does not yet offer the pppInterface method for a dynamic cast */
TEST_F(TestPPPInterface, constructor_default)
{
    EXPECT_TRUE(iface);
    // Test that this clas presents itself correctly
    EXPECT_EQ(iface,   iface->pppInterface());

    EXPECT_EQ(nullptr, iface->emacInterface());
    EXPECT_EQ(nullptr, iface->ethInterface());
    EXPECT_EQ(nullptr, iface->wifiInterface());
    EXPECT_EQ(nullptr, iface->cellularInterface());
    EXPECT_EQ(nullptr, iface->meshInterface());
}
#endif

TEST_F(TestPPPInterface, connect)
{
    mbed::FileHandle_stub handle;
    nsapi_ip_stack_t stack = IPV4_STACK;
    iface->set_credentials("uname", "passwd");
    iface->set_stream(&handle);
    iface->set_ip_stack(stack);

    EXPECT_CALL(*pppMock, set_stream(&handle));
    EXPECT_CALL(*pppMock, set_ip_stack(stack));
    EXPECT_CALL(*pppMock, set_credentials("uname", "passwd"));
    EXPECT_CALL(*stackMock, add_ppp_interface(testing::Ref(*pppMock), true, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_OK)));
    EXPECT_CALL(*netStackIface, attach(_))
    .Times(1)
    .RetiresOnSaturation();
    EXPECT_CALL(*netStackIface, bringup(false, NULL, NULL, NULL, stack, true))
    .Times(1)
    .WillOnce(Return(NSAPI_ERROR_OK));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->connect());
}

TEST_F(TestPPPInterface, connect_failure)
{
    EXPECT_CALL(*pppMock, set_stream(_));
    EXPECT_CALL(*pppMock, set_ip_stack(_));
    EXPECT_CALL(*pppMock, set_credentials(_, _));
    EXPECT_CALL(*stackMock, add_ppp_interface(testing::Ref(*pppMock), true, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_NO_MEMORY)));
    EXPECT_EQ(NSAPI_ERROR_NO_MEMORY, iface->connect());
}

TEST_F(TestPPPInterface, disconnect_without_connecting)
{
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->disconnect());
}

TEST_F(TestPPPInterface, disconnect)
{
    doConnect();

    EXPECT_CALL(*netStackIface, bringdown())
    .Times(1)
    .WillOnce(Return(NSAPI_ERROR_OK));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->disconnect());
}

TEST_F(TestPPPInterface, set_network)
{
    SocketAddress ipAddress("127.0.0.1");
    SocketAddress netmask("255.255.0.0");
    SocketAddress gateway("127.0.0.2");

    const char *ipAddressArg;
    const char *netmaskArg;
    const char *gatewayArg;

    SocketAddress ipAddr;
    SocketAddress netmaskAddr;
    SocketAddress gatewayAddr;

    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_ip_address(&ipAddr));
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_netmask(&netmaskAddr));
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_gateway(&gatewayAddr));

    EXPECT_CALL(*pppMock, set_stream(_));
    EXPECT_CALL(*pppMock, set_ip_stack(_));
    EXPECT_CALL(*pppMock, set_credentials(_, _));

    // Set the network data
    EXPECT_EQ(NSAPI_ERROR_OK, iface->set_network(ipAddress, netmask, gateway));

    // Now the bringup should have different arguments. We can't use doConnect method.
    EXPECT_CALL(*stackMock, add_ppp_interface(testing::Ref(*pppMock), true, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_OK)));
    EXPECT_CALL(*netStackIface, attach(_))
    .Times(1)
    .RetiresOnSaturation();
    // Do not put the expected char * arguments, as they are pointers and would not match
    EXPECT_CALL(*netStackIface, bringup(false, _, _, _, DEFAULT_STACK, true))
    .Times(1)
    .WillOnce(DoAll(SaveArg<1>(&ipAddressArg),
                    SaveArg<2>(&netmaskArg),
                    SaveArg<3>(&gatewayArg),
                    Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->connect());
    // Check the contents of the stored pointer arguments.
    EXPECT_TRUE(0 == strcmp(ipAddress.get_ip_address(), ipAddressArg));
    EXPECT_TRUE(0 == strcmp(netmask.get_ip_address(), netmaskArg));
    EXPECT_TRUE(0 == strcmp(gateway.get_ip_address(), gatewayArg));

    // Testing the getters makes sense now.
    EXPECT_CALL(*netStackIface, get_ip_address(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(ipAddress), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_ip_address(&ipAddr));
    EXPECT_EQ(ipAddress, ipAddr);

    EXPECT_CALL(*netStackIface, get_netmask(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(netmask), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_netmask(&netmaskAddr));
    EXPECT_EQ(netmask, netmaskAddr);

    EXPECT_CALL(*netStackIface, get_gateway(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(gateway), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_gateway(&gatewayAddr));
    EXPECT_EQ(gateway, gatewayAddr);
}

TEST_F(TestPPPInterface, get_connection_status)
{
    EXPECT_EQ(NSAPI_STATUS_DISCONNECTED, iface->get_connection_status());

    doConnect();

    EXPECT_CALL(*netStackIface, get_connection_status())
    .Times(1)
    .WillOnce(Return(NSAPI_STATUS_LOCAL_UP));
    EXPECT_EQ(NSAPI_STATUS_LOCAL_UP, iface->get_connection_status());
}

TEST_F(TestPPPInterface, get_interface_name)
{
    char name[100];
    memset(name, '\0', 100);
    EXPECT_EQ(NULL, iface->get_interface_name(name));

    doConnect();

    EXPECT_EQ(NULL, iface->get_interface_name(name));

    EXPECT_CALL(*netStackIface, get_interface_name(name))
    .Times(1)
    .WillOnce(Return(name));
    EXPECT_EQ(name, iface->get_interface_name(name));
}

TEST_F(TestPPPInterface, set_as_default)
{
    doConnect();

    EXPECT_CALL(*stackMock, set_default_interface(netStackIface))
    .Times(1);
    iface->set_as_default();
}

TEST_F(TestPPPInterface, attach)
{
    doConnect();
    EXPECT_CALL(*netStackIface, attach(_)) // TODO: check that the correct function is passed.
    .Times(1);
    iface->attach(cb);
}

TEST_F(TestPPPInterface, set_dhcp)
{
    EXPECT_EQ(NSAPI_ERROR_UNSUPPORTED, iface->set_dhcp(false));
    doConnect(true);
}

TEST_F(TestPPPInterface, set_blocking)
{
    EXPECT_EQ(NSAPI_ERROR_OK, iface->set_blocking(false));
    doConnect(false);
}
