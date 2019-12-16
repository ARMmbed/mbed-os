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

#include "features/netsocket/EthernetInterface.h"
#include <iostream>
#include "EMAC_mock.h"

OnboardNetworkStack &OnboardNetworkStack::get_default_instance()
{
    return OnboardNetworkStackMock::get_instance();
}

// Implementaion in in NetworkInterfaceDefaults.cpp
MBED_WEAK EthInterface *EthInterface::get_default_instance()
{
    return get_target_default_instance();
}

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

class TestEthernetInterface: public testing::Test {
protected:
    EthernetInterface *iface;
    OnboardNetworkStackMock *stackMock;
    MockEMAC *emacMock;
    OnboardNetworkStackMock::InterfaceMock *netStackIface;
    virtual void SetUp()
    {
        stackMock = &OnboardNetworkStackMock::get_instance();
        emacMock = &MockEMAC::get_instance();
        netStackIface = &OnboardNetworkStackMock::InterfaceMock::get_instance();
        iface = new EthernetInterface(MockEMAC::get_instance(), OnboardNetworkStackMock::get_instance());
    }

    virtual void TearDown()
    {
        // Do not delete the mocks pointers, as they point to statically allocated singletons.
        delete iface;
    }

    /* Enclose the heavily-used connection procedure to improve code redability */
    void doConnect(bool dhcp = true, bool blocking = true)
    {
        EXPECT_CALL(*stackMock, add_ethernet_interface(testing::Ref(*emacMock), true, _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_OK)));
        EXPECT_CALL(*netStackIface, attach(_))
        .Times(1)
        .RetiresOnSaturation();;
        EXPECT_CALL(*netStackIface, bringup(dhcp, NULL, NULL, NULL, DEFAULT_STACK, blocking))
        .Times(1)
        .WillOnce(Return(NSAPI_ERROR_OK));
        EXPECT_EQ(NSAPI_ERROR_OK, iface->connect());
    }

    static void cb(nsapi_event_t ev, intptr_t ptr)
    {

    }
};

TEST_F(TestEthernetInterface, constructor_default)
{
    EXPECT_TRUE(iface);

    // Test that this clas presents itself correctly
    EXPECT_NE(nullptr, iface->ethInterface());
    EXPECT_NE(nullptr, iface->emacInterface());

    EXPECT_EQ(nullptr, iface->wifiInterface());
    EXPECT_EQ(nullptr, iface->cellularInterface());
    EXPECT_EQ(nullptr, iface->meshInterface());
}

TEST_F(TestEthernetInterface, constructor_getter)
{
    EthInterface *eth = EthInterface::get_default_instance();
}



TEST_F(TestEthernetInterface, connect)
{
    doConnect();
}

TEST_F(TestEthernetInterface, connect_failure)
{
    EXPECT_CALL(*stackMock, add_ethernet_interface(testing::Ref(*emacMock), true, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<2>(netStackIface), Return(NSAPI_ERROR_NO_MEMORY)));
    EXPECT_EQ(NSAPI_ERROR_NO_MEMORY, iface->connect());
}

TEST_F(TestEthernetInterface, disconnect_without_connecting)
{
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->disconnect());
}

TEST_F(TestEthernetInterface, disconnect)
{
    doConnect();

    EXPECT_CALL(*netStackIface, bringdown())
    .Times(1)
    .WillOnce(Return(NSAPI_ERROR_OK));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->disconnect());
}

TEST_F(TestEthernetInterface, set_network)
{
    SocketAddress ipAddress("127.0.0.1");
    SocketAddress netmask("255.255.0.0");
    SocketAddress gateway("127.0.0.2");
    char macAddress[NSAPI_MAC_SIZE];
    memset(macAddress, '\0', NSAPI_MAC_SIZE);

    SocketAddress ipAddressArg;
    SocketAddress netmaskArg;
    SocketAddress gatewayArg;

    // Before connecting return NULL
    EXPECT_EQ(NULL, iface->get_mac_address());

    SocketAddress tmp;
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_ip_address(&tmp));
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_netmask(&tmp));
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_gateway(&tmp));

    // Set the network data
    EXPECT_EQ(NSAPI_ERROR_OK, iface->set_network(ipAddress, netmask, gateway));

    // Now the bringup should have different arguments. We can't use doConnect method.
    EXPECT_CALL(*stackMock, add_ethernet_interface(testing::Ref(*emacMock), true, _))
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
    EXPECT_EQ(ipAddress, ipAddressArg);
    EXPECT_EQ(netmask, netmaskArg);
    EXPECT_EQ(gateway, gatewayArg);

    // Testing the getters makes sense now.
    EXPECT_CALL(*netStackIface, get_mac_address(_, _))
    .Times(1)
    .WillOnce(DoAll(SetArrayArgument<0>(macAddress, macAddress+NSAPI_MAC_SIZE), Return(macAddress)));
    EXPECT_EQ(std::string(macAddress), std::string(iface->get_mac_address()));

    EXPECT_CALL(*netStackIface, get_ip_address(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(ipAddress), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_ip_address(&ipAddressArg));

    EXPECT_CALL(*netStackIface, get_netmask(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(netmask), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_netmask(&netmaskArg));

    EXPECT_CALL(*netStackIface, get_gateway(_))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(gateway), Return(NSAPI_ERROR_OK)));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_gateway(&gatewayArg));
}

TEST_F(TestEthernetInterface, get_connection_status)
{
    EXPECT_EQ(NSAPI_STATUS_DISCONNECTED, iface->get_connection_status());

    doConnect();

    EXPECT_CALL(*netStackIface, get_connection_status())
    .Times(1)
    .WillOnce(Return(NSAPI_STATUS_LOCAL_UP));
    EXPECT_EQ(NSAPI_STATUS_LOCAL_UP, iface->get_connection_status());
}

TEST_F(TestEthernetInterface, attach)
{
    doConnect();
    EXPECT_CALL(*netStackIface, attach(_)) // TODO: check that the correct function is passed.
    .Times(1);
    iface->attach(cb);
}


TEST_F(TestEthernetInterface, get_interface_name)
{
    char name[100] = "eth0";
    EXPECT_EQ(NULL, iface->get_interface_name(name));

    doConnect();

    // The parameter will be an internal variable.
    EXPECT_CALL(*netStackIface, get_interface_name(_))
    .Times(1)
    .WillOnce(Return(name));
    EXPECT_EQ(std::string(name), std::string(iface->get_interface_name(name)));
}

TEST_F(TestEthernetInterface, get_ipv6_link_local_address)
{
    SocketAddress addr("4.3.2.1");
    EXPECT_EQ(NSAPI_ERROR_NO_CONNECTION, iface->get_ipv6_link_local_address(&addr));
    EXPECT_EQ(std::string(addr.get_ip_address()), std::string("4.3.2.1"));
    doConnect();

    // The parameter will be an internal variable.
    EXPECT_CALL(*netStackIface, get_ipv6_link_local_address(&addr))
    .Times(1)
    .WillOnce(Return(NSAPI_ERROR_OK));
    EXPECT_EQ(NSAPI_ERROR_OK, iface->get_ipv6_link_local_address(&addr));
}

TEST_F(TestEthernetInterface, set_as_default)
{
    doConnect();

    EXPECT_CALL(*stackMock, set_default_interface(netStackIface))
    .Times(1);
    iface->set_as_default();
}


TEST_F(TestEthernetInterface, set_dhcp)
{
    EXPECT_EQ(NSAPI_ERROR_OK, iface->set_dhcp(false));
    doConnect(false, true);
}

TEST_F(TestEthernetInterface, set_blocking)
{
    EXPECT_EQ(NSAPI_ERROR_OK, iface->set_blocking(false));
    doConnect(true, false);
}
