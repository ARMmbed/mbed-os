/*
 * Copyright (c) 2019, Arm Limited and affiliates
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
#include "nsapi_dns.h"
#include "features/netsocket/EthernetInterface.h"
#include "SocketAddress.h"
#include "EMAC_mock.h"
#include "OnboardNetworkStack_mock.h"
#include "EventQueue.h"
#include <future>
#include <thread>
#include <chrono>
#include <list>

// Control the rtos EventFlags stub. See EventFlags_stub.cpp
extern std::list<uint32_t> eventFlagsStubNextRetval;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::DoAll;

// This is the bottom layer, that goes outside of module's scope.
// This would be implemented by LWIPInterface, NanostackInterface, or board-specific stack.
OnboardNetworkStack &OnboardNetworkStack::get_default_instance()
{
    return OnboardNetworkStackMock::get_instance();
}

// Wrapper to avoid calling OnboardNetworkStack::get_default_instance() everywhere
static OnboardNetworkStackMock &stackMock() {
    return OnboardNetworkStackMock::get_instance();
}

class Test_IfaceDnsSocket : public testing::Test {
public:
    static nsapi_error_t call_in(int delay, mbed::Callback<void()> func)
    {
        (void) delay; // Ignore the delay, just call the callbacks in the order they came.
        eventQueue.push_back(std::async(std::launch::deferred, func));
        return NSAPI_ERROR_OK;
    }
protected:
    EthernetInterface *iface;

    virtual void SetUp()
    {
        iface = new EthernetInterface(MockEMAC::get_instance(), OnboardNetworkStackMock::get_instance());;
        nsapi_dns_reset();
        eventQueue.clear();
        hostname_cb_result = NSAPI_ERROR_DEVICE_ERROR;
        hostname_cb_address = NULL;
    }

    virtual void TearDown()
    {
        delete iface;

    }

    void executeEventQueueCallbacks(void)
    {
        size_t events_num = eventQueue.size(); // Events will re-register themselves.
        for (int i = 0; i < events_num; i++) {
            eventQueue.front().get();
            eventQueue.pop_front();
        }
    }

    static std::list<std::future<void>> eventQueue;

    static nsapi_error_t hostname_cb_result;
    static SocketAddress *hostname_cb_address;
    static void hostbyname_cb(nsapi_error_t result, SocketAddress *address)
    {
        hostname_cb_result = result;
        if (result == 0) {
            result = 1;
        }
        hostname_cb_address = new SocketAddress[result];
        for (int i = 0; i < result; i++) {
            hostname_cb_address[i].set_addr(address[i].get_addr());
        }
    }

    static int query_id;

    static constexpr unsigned int packet_ip4_size = 44;
    static const unsigned char packet_ip4[packet_ip4_size];
    static constexpr unsigned int packet_ip6_size = 56;
    static const unsigned char packet_ip6[packet_ip6_size];
    static constexpr unsigned int packet_ip4_3addresses_size = 76;
    static const unsigned char packet_ip4_3addresses[packet_ip4_3addresses_size];
};

std::list<std::future<void>> Test_IfaceDnsSocket::eventQueue;
nsapi_error_t Test_IfaceDnsSocket::hostname_cb_result = NSAPI_ERROR_DEVICE_ERROR;
SocketAddress *Test_IfaceDnsSocket::hostname_cb_address = NULL;
int Test_IfaceDnsSocket::query_id = 1; // Default after nsapi_dns_reset()

// An actual google.com DNS resolution packet captured using a greentea test.
const unsigned char Test_IfaceDnsSocket::packet_ip4[Test_IfaceDnsSocket::packet_ip4_size] = {
    0x00, 0x01, // ID
    0x81, 0x80, // Flags
    0x00, 0x01, // qdcount
    0x00, 0x01, // ancount
    0x00, 0x00, // nscount
    0x00, 0x00, // arcount

    0x06,                               // question, qdcount = 1, first byte is len = 6
    0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, // body of the question
    0x03,                               // len = 3
    0x63, 0x6f, 0x6d,                   // body of the question qtype and qclass of the question
    0x00,                               // len = 0
    0x00, 0x01,                         // qtype
    0x00, 0x01,                         // qclass

    0xc0,                        // answer len = 192 (means this is a link)
    0x0c,                        // this gets scanned away, because of previous byte being link
    0x00, 0x01,                  // rtype: RR_A = 1, RR_AAAA = 28
    0x00, 0x01,                  // rclass
    0x00, 0x00, 0x00, 0x22,      // ttl
    0x00, 0x04,                  // rdlength (4 for IPv4 and 16 for IPv6)
    0xd8, 0x3a, 0xcf, 0xee       // Address bytes
};

// Same as previous packet, but with a fake IPv6 address instead of the IPv4 one
const unsigned char Test_IfaceDnsSocket::packet_ip6[Test_IfaceDnsSocket::packet_ip6_size] = {
    0x00, 0x01, // ID
    0x81, 0x80, // Flags
    0x00, 0x01, // qdcount
    0x00, 0x01, // ancount
    0x00, 0x00, // nscount
    0x00, 0x00, // arcount

    0x06,                               // question, qdcount = 1, first byte is len = 6
    0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, // body of the question
    0x03,                               // len = 3
    0x63, 0x6f, 0x6d,                   // body of the question qtype and qclass of the question
    0x00,                               // len = 0
    0x00, 0x01,                         // qtype
    0x00, 0x01,                         // qclass

    0xc0,                        // answer len = 192 (means this is a link)
    0x0c,                        // this gets scanned away, because of previous byte being link
    0x00, 0x1c,                  // rtype: RR_A = 1, RR_AAAA = 28
    0x00, 0x01,                  // rclass
    0x00, 0x00, 0x00, 0x22,      // ttl
    0x00, 0x10,                  // rdlength (4 for IPv4 and 16 for IPv6)
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 // Address bytes
};

// Same as packet_ipv4, but with two fake ipv4 appended at the end.
const unsigned char Test_IfaceDnsSocket::packet_ip4_3addresses[Test_IfaceDnsSocket::packet_ip4_3addresses_size] = {
    0x00, 0x01, // ID
    0x81, 0x80, // Flags
    0x00, 0x01, // qdcount
    0x00, 0x03, // ancount
    0x00, 0x00, // nscount
    0x00, 0x00, // arcount

    0x06,                               // question, qdcount = 1, first byte is len = 6
    0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, // body of the question
    0x03,                               // len = 3
    0x63, 0x6f, 0x6d,                   // body of the question qtype and qclass of the question
    0x00,                               // len = 0
    0x00, 0x01,                         // qtype
    0x00, 0x01,                         // qclass

    0xc0,                        // answer len = 192 (means this is a link)
    0x0c,                        // this gets scanned away, because of previous byte being link
    0x00, 0x01,                  // rtype: RR_A = 1, RR_AAAA = 28
    0x00, 0x01,                  // rclass
    0x00, 0x00, 0x00, 0x22,      // ttl
    0x00, 0x04,                  // rdlength (4 for IPv4 and 16 for IPv6)
    0xd8, 0x3a, 0xcf, 0xee,      // Address bytes

    0xc0,                        // answer len = 192 (means this is a link)
    0x0c,                        // this gets scanned away, because of previous byte being link
    0x00, 0x01,                  // rtype: RR_A = 1, RR_AAAA = 28
    0x00, 0x01,                  // rclass
    0x00, 0x00, 0x00, 0x22,      // ttl
    0x00, 0x04,                  // rdlength (4 for IPv4 and 16 for IPv6)
    0xde, 0xad, 0xbe, 0xef,      // Address bytes

    0xc0,                        // answer len = 192 (means this is a link)
    0x0c,                        // this gets scanned away, because of previous byte being link
    0x00, 0x01,                  // rtype: RR_A = 1, RR_AAAA = 28
    0x00, 0x01,                  // rclass
    0x00, 0x00, 0x00, 0x22,      // ttl
    0x00, 0x04,                  // rdlength (4 for IPv4 and 16 for IPv6)
    0x01, 0x02, 0x03, 0x04       // Address bytes
};

// We cannot use SetArgArray, because this is void* type.
// Use a manual for loop, to avoid depending on local implementation of strncpy (had some issues with it).
ACTION_P2(SetArg2ToCharPtr, value, size)
{
    for (int i = 0; i < size; i++) {
        static_cast<char *>(arg2)[i] = reinterpret_cast<const char *>(value)[i];
    }
}

TEST_F(Test_IfaceDnsSocket, single_query)
{
    SocketAddress addr;
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    // Returning UNSUPPORTED will cause the nsapi_dns addresses to be used.
    EXPECT_CALL(stackMock(), get_dns_server(_, _, _))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<1>(SocketAddress("1.2.3.4", 53)), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), socket_sendto(_, _, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    // Return a predefined google.com resolution packet.
    EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
    .Times(1)
    .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip4, Test_IfaceDnsSocket::packet_ip4_size), Return(Test_IfaceDnsSocket::packet_ip4_size)));

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query(iface, "www.google.com", &addr));
    EXPECT_EQ(addr.get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr.get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));

    // Do not set any return values. Second call should use cache.
    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query(iface, "www.google.com", &addr));
    EXPECT_EQ(addr.get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr.get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));
}

TEST_F(Test_IfaceDnsSocket, single_query_ip6)
{
    SocketAddress addr;

    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    // Returning UNSUPPORTED will cause the nsapi_dns addresses to be used.
    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    // We want to send one query to the DNS server.
    EXPECT_CALL(stackMock(), socket_sendto(_, _, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    // Return a predefined google.com resolution packet.
    EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
    .Times(1)
    .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip6, Test_IfaceDnsSocket::packet_ip6_size), Return(Test_IfaceDnsSocket::packet_ip6_size)));

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query(iface, "www.google.com", &addr, NSAPI_IPv6));
    EXPECT_EQ(addr.get_ip_version(), NSAPI_IPv6);
    EXPECT_FALSE(strncmp(addr.get_ip_address(), "102:304:506:708:90a:b0c:d0e:f10", sizeof("102:304:506:708:90a:b0c:d0e:f10")));

    // Do not set any return values. Second call should use cache.
    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query(iface, "www.google.com", &addr, NSAPI_IPv6));
    EXPECT_EQ(addr.get_ip_version(), NSAPI_IPv6);
    EXPECT_FALSE(strncmp(addr.get_ip_address(), "102:304:506:708:90a:b0c:d0e:f10", sizeof("102:304:506:708:90a:b0c:d0e:f10")));
}

// Imitate the getaddrinfo to make the example more real-life.
int getaddrinfo(NetworkStack *stack, const char *hostname, SocketAddress hints, SocketAddress **res)
{
    SocketAddress *tmp = new SocketAddress[5];
    int num = nsapi_dns_query_multiple(stack, hostname, tmp, 5, NULL);
    *res = new SocketAddress[num];
    for (int i = 0; i < num; i++) {
        (*res)[i].set_addr(tmp[i].get_addr());
    }
    delete[] tmp;
    return num;
}

TEST_F(Test_IfaceDnsSocket, multiple_queries)
{
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    EXPECT_CALL(stackMock(), socket_sendto(_, _, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    // Return a predefined google.com resolution packet.
    EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
    .Times(1)
    .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip4_3addresses, Test_IfaceDnsSocket::packet_ip4_3addresses_size), Return(Test_IfaceDnsSocket::packet_ip4_3addresses_size)));

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    SocketAddress *addr;
    SocketAddress hints{{NSAPI_UNSPEC}, 443};
    EXPECT_EQ(3, getaddrinfo(&stackMock(), "www.google.com", hints, &addr));

    EXPECT_EQ(addr[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));
    EXPECT_EQ(addr[1].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr[1].get_ip_address(), "222.173.190.239", sizeof("222.173.190.239")));
    EXPECT_EQ(addr[2].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr[2].get_ip_address(), "1.2.3.4", sizeof("1.2.3.4")));
    delete[] addr;

    SocketAddress *addr_cache;
    // Cache will only raturn one address.
    EXPECT_EQ(Test_IfaceDnsSocket::query_id, getaddrinfo(&stackMock(), "www.google.com", hints, &addr_cache));

    // This is a bug which will be fixed in
    EXPECT_EQ(addr_cache[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr_cache[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));
    delete[] addr_cache;
}

TEST_F(Test_IfaceDnsSocket, single_query_errors)
{
    testing::InSequence s;
    SocketAddress addr {};

    // Error - empty host name
    EXPECT_EQ(NSAPI_ERROR_PARAMETER, nsapi_dns_query(iface, "", &addr));
    EXPECT_EQ(NSAPI_ERROR_PARAMETER, nsapi_dns_query_multiple_async(&stackMock(), "", &Test_IfaceDnsSocket::hostbyname_cb, 5, &Test_IfaceDnsSocket::call_in, NULL));

    // Error - socket_open failing
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)NULL), Return(NSAPI_ERROR_DEVICE_ERROR)));

    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, nsapi_dns_query(iface, "www.google.com", &addr));
}

TEST_F(Test_IfaceDnsSocket, simultaneous_query_async)
{
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    EXPECT_CALL(stackMock(), socket_sendto(_, _, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    {
        testing::InSequence s;

        // Return a predefined google.com resolution packet.
        EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip4, Test_IfaceDnsSocket::packet_ip4_size), Return(Test_IfaceDnsSocket::packet_ip4_size)));

        // There will be another try to recv from the socket, return 0, to show that no data is pending.
        EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
        .Times(1)
        .WillOnce(Return(0));
    }

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(Test_IfaceDnsSocket::query_id, nsapi_dns_query_async(&stackMock(), "www.google.com", &Test_IfaceDnsSocket::hostbyname_cb, Test_IfaceDnsSocket::call_in, NSAPI_IPv4));

    // Let another query to be stored in the queue.
    EXPECT_EQ(Test_IfaceDnsSocket::query_id + 1, nsapi_dns_query_async(&stackMock(), "www.amazon.com", &Test_IfaceDnsSocket::hostbyname_cb, Test_IfaceDnsSocket::call_in, NSAPI_IPv4));

    executeEventQueueCallbacks();

    // This will register the socket_callback.
    reinterpret_cast<mbed::Callback<void(void *)> *>((&stackMock())->socket_cb)->call((void *)packet_ip4);

    // Run through registered callbacks again, to execute socket_callback_handle
    executeEventQueueCallbacks();
    // Run again to execute response_handler
    executeEventQueueCallbacks();

    EXPECT_EQ(NSAPI_ERROR_OK, Test_IfaceDnsSocket::hostname_cb_result);
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));

    delete[] Test_IfaceDnsSocket::hostname_cb_address;

    // Second call should use cache.
    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query_async(&stackMock(), "www.google.com", &Test_IfaceDnsSocket::hostbyname_cb, Test_IfaceDnsSocket::call_in, NSAPI_IPv4));
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));

    delete[] Test_IfaceDnsSocket::hostname_cb_address;

    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query_async_cancel((intptr_t)Test_IfaceDnsSocket::query_id + 1));
}

// Imitate the getaddrinfo to make the example more real-life.
int getaddrinfo_async(NetworkStack *stack, const char *hostname, NetworkStack::hostbyname_cb_t cb)
{
    return nsapi_dns_query_multiple_async(stack, hostname, cb, 5, &Test_IfaceDnsSocket::call_in, NULL);
}

TEST_F(Test_IfaceDnsSocket, single_query_async_multiple)
{
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    EXPECT_CALL(stackMock(), socket_sendto(_, _, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    {
        testing::InSequence s;

        // Return a predefined google.com resolution packet.
        EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip4_3addresses, Test_IfaceDnsSocket::packet_ip4_3addresses_size), Return(Test_IfaceDnsSocket::packet_ip4_3addresses_size)));

        // There will be another try to recv from the socket, return 0, to show that no data is pending.
        EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
        .Times(1)
        .WillOnce(Return(0));
    }

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(Test_IfaceDnsSocket::query_id, getaddrinfo_async(&stackMock(), "www.google.com", &Test_IfaceDnsSocket::hostbyname_cb));

    executeEventQueueCallbacks();

    // This will register the socket_callback.
    reinterpret_cast<mbed::Callback<void(void *)> *>((&stackMock())->socket_cb)->call((void *)packet_ip4);

    // Run through registered callbacks again, to execute socket_callback_handle
    executeEventQueueCallbacks();
    // Run again to execute response_handler
    executeEventQueueCallbacks();

    EXPECT_EQ(3, Test_IfaceDnsSocket::hostname_cb_result);
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[1].get_ip_version(), NSAPI_IPv4);
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[2].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[1].get_ip_address(), "222.173.190.239", sizeof("222.173.190.239")));
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[2].get_ip_address(), "1.2.3.4", sizeof("1.2.3.4")));

    delete[] Test_IfaceDnsSocket::hostname_cb_address;
    Test_IfaceDnsSocket::hostname_cb_result = NSAPI_ERROR_DEVICE_ERROR;

    // Do not set any return values. Second call should use cache.
    // Cache will only return one address!
    EXPECT_EQ(0, getaddrinfo_async(&stackMock(), "www.google.com", &Test_IfaceDnsSocket::hostbyname_cb));
    EXPECT_EQ(NSAPI_ERROR_OK, Test_IfaceDnsSocket::hostname_cb_result);
    EXPECT_EQ(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(Test_IfaceDnsSocket::hostname_cb_address[0].get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));

    delete[] Test_IfaceDnsSocket::hostname_cb_address;
}

TEST_F(Test_IfaceDnsSocket, async_cancel)
{
    EXPECT_EQ(Test_IfaceDnsSocket::query_id, nsapi_dns_query_async(&stackMock(), "www.google.com", &Test_IfaceDnsSocket::hostbyname_cb, Test_IfaceDnsSocket::call_in, NSAPI_IPv4));

    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query_async_cancel((intptr_t)1));

    executeEventQueueCallbacks();

    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, Test_IfaceDnsSocket::hostname_cb_result);
    EXPECT_EQ(NULL, Test_IfaceDnsSocket::hostname_cb_address);

    // Try to cancel twice - should return error
    EXPECT_EQ(NSAPI_ERROR_PARAMETER, nsapi_dns_query_async_cancel((intptr_t)1));
}

TEST_F(Test_IfaceDnsSocket, add_server)
{
    SocketAddress server_address("1.2.3.4", 8000);
    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_add_server(server_address, NULL));
    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_add_server(server_address, NULL)); // Duplicate add - no error.

    SocketAddress addr;
    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    EXPECT_CALL(stackMock(), socket_sendto(_, server_address, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    // Return a predefined google.com resolution packet.
    EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
    .Times(1)
    .WillOnce(DoAll(SetArg2ToCharPtr(Test_IfaceDnsSocket::packet_ip4, Test_IfaceDnsSocket::packet_ip4_size), Return(Test_IfaceDnsSocket::packet_ip4_size)));

    EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_query(iface, "www.google.com", &addr));
    EXPECT_EQ(addr.get_ip_version(), NSAPI_IPv4);
    EXPECT_FALSE(strncmp(addr.get_ip_address(), "216.58.207.238", sizeof("216.58.207.238")));
}

TEST_F(Test_IfaceDnsSocket, attempts)
{
    constexpr int DNS_SERVER_SIZE = 5; // This must be kept equal to DNS_SERVER_SIZE macro define in nsapi_dns.cpp

    // Replace existing servers, so we can see that the switch to next server occurs.
    SocketAddress known_server_address[DNS_SERVER_SIZE];
    for (uint8_t i = DNS_SERVER_SIZE; i > 0; i--) {
        uint8_t bytes[NSAPI_IPv4_SIZE] = {i, i, i, i};
        known_server_address[i - 1] = SocketAddress(bytes, NSAPI_IPv4);
        EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_add_server(known_server_address[i - 1], NULL));
    }

    SocketAddress addr {};

    testing::InSequence s;

    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    int attempt = 0;
    while (attempt < DNS_SERVER_SIZE) {
        EXPECT_CALL(stackMock(), socket_sendto(_, known_server_address[attempt % DNS_SERVER_SIZE], _, _))
        .Times(1)
        .WillRepeatedly(Return(NSAPI_ERROR_DEVICE_ERROR));

        attempt++;
        if (attempt >= DNS_SERVER_SIZE) {
            break;
        }
    }

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(NSAPI_ERROR_DNS_FAILURE, nsapi_dns_query(iface, "www.google.com", &addr));
}

TEST_F(Test_IfaceDnsSocket, retries_attempts)
{
    constexpr int DNS_SERVER_SIZE = 5; // This must be kept equal to DNS_SERVER_SIZE macro define in nsapi_dns.cpp

    // Replace existing servers, so we can see that the switch to next server occurs.
    SocketAddress known_server_address[DNS_SERVER_SIZE];
    for (uint8_t i = DNS_SERVER_SIZE; i > 0; i--) {
        uint8_t bytes[NSAPI_IPv4_SIZE] = {i, i, i, i};
        known_server_address[i - 1] = SocketAddress(bytes, NSAPI_IPv4);
        EXPECT_EQ(NSAPI_ERROR_OK, nsapi_dns_add_server(known_server_address[i - 1], NULL));
    }

    SocketAddress addr {};

    testing::InSequence s;

    // Make sure socket opens successfully
    EXPECT_CALL(stackMock(), socket_open(_, NSAPI_UDP))
    .Times(1)
    .WillOnce(DoAll(SetArgPointee<0>((void **)&stackMock()), Return(NSAPI_ERROR_OK)));

    EXPECT_CALL(stackMock(), get_dns_server(_, _, _)).Times(1).WillOnce(Return(NSAPI_ERROR_UNSUPPORTED));

    // Set up all the expectation: sendto and recvfrom should come one after another, untill dns runs out of attempts.
    int attempt = 0;
    while (attempt < MBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS) {
        // We will retry the first server MBED_CONF_NSAPI_DNS_RETRIES times.
        int retry = 0;
        while (retry < MBED_CONF_NSAPI_DNS_RETRIES + 1) {
            int server_idx = attempt / (MBED_CONF_NSAPI_DNS_RETRIES + 1);
            EXPECT_CALL(stackMock(), socket_sendto(_, known_server_address[server_idx % DNS_SERVER_SIZE], _, _))
            .Times(1)
            .WillRepeatedly(Return(NSAPI_ERROR_OK));

            eventFlagsStubNextRetval.push_back(osFlagsError); // Break the wait loop

            // Make sure the retries will be necessary.
            EXPECT_CALL(stackMock(), socket_recvfrom(_, _, _, _))
            .Times(1)
            .WillRepeatedly(Return(NSAPI_ERROR_WOULD_BLOCK));

            attempt++;
            retry++;
            if (attempt >= MBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS) {
                break;
            }
        }
    }

    EXPECT_CALL(stackMock(), socket_close(_)).Times(1).WillOnce(Return(NSAPI_ERROR_OK));

    EXPECT_EQ(NSAPI_ERROR_DNS_FAILURE, nsapi_dns_query(iface, "www.google.com", &addr));
}
