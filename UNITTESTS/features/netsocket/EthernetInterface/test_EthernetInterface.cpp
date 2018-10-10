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
#include "features/netsocket/EthernetInterface.h"
#include <iostream>

class MockEMAC : public EMAC {
public:
    MOCK_METHOD0(power_up, bool());
    MOCK_METHOD0(power_down, void());
    MOCK_CONST_METHOD0(get_mtu_size, uint32_t());
    MOCK_CONST_METHOD0(get_align_preference, uint32_t());
    MOCK_CONST_METHOD2(get_ifname, void(char *name, uint8_t size));
    MOCK_CONST_METHOD0(get_hwaddr_size, uint8_t());
    MOCK_CONST_METHOD1(get_hwaddr, bool(uint8_t *addr));
    MOCK_METHOD1(set_hwaddr, void(const uint8_t *));
    MOCK_METHOD1(link_out, bool(emac_mem_buf_t *buf));
    MOCK_METHOD1(set_link_input_cb, void(emac_link_input_cb_t input_cb));
    MOCK_METHOD1(set_link_state_cb, void(emac_link_state_change_cb_t state_cb));
    MOCK_METHOD1(add_multicast_group, void(const uint8_t *address));
    MOCK_METHOD1(remove_multicast_group, void(const uint8_t *address));
    MOCK_METHOD1(set_all_multicast, void(bool all));
    MOCK_METHOD1(set_memory_manager, void(EMACMemoryManager &mem_mngr));

    static MockEMAC &get_instance()
    {
        static MockEMAC emacMock1;
        return emacMock1;
    }
};

MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return MockEMAC::get_instance();
}

class EmacNetworkStackMock : public OnboardNetworkStack {
public:
    MOCK_METHOD3(gethostbyname, nsapi_error_t(const char *host, SocketAddress *address, nsapi_version_t version));
    MOCK_METHOD1(add_dns_server, nsapi_error_t(const SocketAddress &address));
    MOCK_METHOD2(call_in, nsapi_error_t(int delay, mbed::Callback<void()> func));
    MOCK_METHOD2(socket_open, nsapi_error_t(nsapi_socket_t *handle, nsapi_protocol_t proto));
    MOCK_METHOD1(socket_close, nsapi_error_t(nsapi_socket_t handle));
    MOCK_METHOD2(socket_bind, nsapi_error_t(nsapi_socket_t handle, const SocketAddress &address));
    MOCK_METHOD2(socket_listen, nsapi_error_t(nsapi_socket_t handle, int backlog));
    MOCK_METHOD2(socket_connect, nsapi_error_t(nsapi_socket_t handle, const SocketAddress &address));
    MOCK_METHOD3(socket_accept, nsapi_error_t(nsapi_socket_t server, nsapi_socket_t *handle, SocketAddress *address));
    MOCK_METHOD3(socket_send, nsapi_error_t(nsapi_socket_t handle, const void *data, nsapi_size_t size));
    MOCK_METHOD3(socket_recv, nsapi_error_t(nsapi_socket_t handle, void *data, nsapi_size_t size));
    MOCK_METHOD4(socket_sendto, nsapi_error_t(nsapi_socket_t handle, const SocketAddress &address, const void *data, nsapi_size_t size));
    MOCK_METHOD4(socket_recvfrom, nsapi_error_t(nsapi_socket_t handle, SocketAddress *address, void *data, nsapi_size_t size));
    MOCK_METHOD5(setsockopt, nsapi_error_t(nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen));
    MOCK_METHOD5(getsockopt, nsapi_error_t(nsapi_socket_t handle, int level, int optname, const void *optval, unsigned *optlen));
    MOCK_METHOD3(socket_attach, void(nsapi_socket_t handle, void (*callback)(void *), void *data));
    MOCK_METHOD3(add_ethernet_interface, nsapi_error_t(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out));

    static EmacNetworkStackMock &get_instance()
    {
        static EmacNetworkStackMock stackMock1;
        return stackMock1;
    }

    class InterfaceMock : public OnboardNetworkStack::Interface {
    public:

        static InterfaceMock &get_instance()
        {
            static InterfaceMock test_interface;
            return test_interface;
        }
        MOCK_METHOD6(bringup, nsapi_error_t(bool dhcp, const char *ip,
                                            const char *netmask, const char *gw,
                                            nsapi_ip_stack_t stack,
                                            bool blocking
                                           ));
        MOCK_METHOD0(bringdown, nsapi_error_t());
        MOCK_METHOD1(attach, void(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb));
        MOCK_CONST_METHOD0(get_connection_status, nsapi_connection_status_t());
        MOCK_METHOD2(get_mac_address, char *(char *buf, nsapi_size_t buflen));
        MOCK_METHOD2(get_ip_address, char *(char *buf, nsapi_size_t buflen));
        MOCK_METHOD2(get_netmask, char *(char *buf, nsapi_size_t buflen));
        MOCK_METHOD2(get_gateway, char *(char *buf, nsapi_size_t buflen));
    };
};

OnboardNetworkStack &OnboardNetworkStack::get_default_instance()
{
    return EmacNetworkStackMock::get_instance();
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
    EmacNetworkStackMock *stackMock;
    MockEMAC *emacMock;
    EmacNetworkStackMock::InterfaceMock *netStackIface;
    virtual void SetUp()
    {
        stackMock = &EmacNetworkStackMock::get_instance();
        emacMock = &MockEMAC::get_instance();
        netStackIface = &EmacNetworkStackMock::InterfaceMock::get_instance();
        iface = new EthernetInterface(MockEMAC::get_instance(), EmacNetworkStackMock::get_instance());
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
    char ipAddress[NSAPI_IPv4_SIZE] = "127.0.0.1";
    char netmask[NSAPI_IPv4_SIZE] = "255.255.0.0";
    char gateway[NSAPI_IPv4_SIZE] = "127.0.0.2";

    const char *ipAddressArg;
    const char *netmaskArg;
    const char *gatewayArg;

    EXPECT_EQ(0, iface->get_ip_address());
    EXPECT_EQ(0, iface->get_netmask());
    EXPECT_EQ(0, iface->get_gateway());

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
    EXPECT_TRUE(0 == strcmp(ipAddress, ipAddressArg));
    EXPECT_TRUE(0 == strcmp(netmask, netmaskArg));
    EXPECT_TRUE(0 == strcmp(gateway, gatewayArg));

    // Testing the getters makes sense now.
    EXPECT_CALL(*netStackIface, get_ip_address(_, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(*ipAddress), Return(ipAddress)));
    EXPECT_EQ(std::string(ipAddress), std::string(iface->get_ip_address()));

    EXPECT_CALL(*netStackIface, get_netmask(_, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(*netmask), Return(netmask)));
    EXPECT_EQ(std::string(netmask), std::string(iface->get_netmask()));

    EXPECT_CALL(*netStackIface, get_gateway(_, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>(*gateway), Return(gateway)));
    EXPECT_EQ(std::string(gateway), std::string(iface->get_gateway()));
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
