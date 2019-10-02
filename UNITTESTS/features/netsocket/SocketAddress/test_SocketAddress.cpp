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
#include "features/netsocket/SocketAddress.h"
#include <iostream>

class TestSocketAddress: public testing::Test {
protected:
    SocketAddress *address;
    virtual void SetUp()
    {
        address = new SocketAddress;
    }

    virtual void TearDown()
    {
        delete address;
    }
    void isEqual(SocketAddress addr1, SocketAddress addr2)
    {
        EXPECT_EQ(std::string(addr1.get_ip_address()), std::string(addr2.get_ip_address()));
        EXPECT_EQ(addr1.get_addr().version, addr2.get_addr().version);
        EXPECT_TRUE(0 == memcmp(addr1.get_addr().bytes, addr2.get_addr().bytes, NSAPI_IPv4_BYTES));
        EXPECT_TRUE(0 == memcmp(addr1.get_ip_bytes(), addr2.get_ip_bytes(), NSAPI_IP_BYTES));
        EXPECT_EQ(addr1.get_ip_version(), addr2.get_ip_version());
        EXPECT_EQ(addr1.get_port(), addr2.get_port());
    }
};

TEST_F(TestSocketAddress, constructor_default)
{
    EXPECT_TRUE(address);
    EXPECT_EQ(NULL, address->get_ip_address());
    EXPECT_EQ(0, address->get_port());
    EXPECT_EQ(NSAPI_UNSPEC, address->get_ip_version());
}

TEST_F(TestSocketAddress, constructor_address_ip4)
{
    SocketAddress addr("127.0.0.1", 80);
    EXPECT_EQ(std::string("127.0.0.1"), std::string(addr.get_ip_address()));
    EXPECT_EQ(80, addr.get_port());
    EXPECT_EQ(NSAPI_IPv4, addr.get_ip_version());
}

TEST_F(TestSocketAddress, constructor_address_ip6)
{
    SocketAddress addr("1234:5678:9abc:def0:1234:5678::def0", 80);
    EXPECT_EQ(80, addr.get_port());
    EXPECT_EQ(NSAPI_IPv6, addr.get_ip_version());
    // Note that a missing zero is added between two neighbouring colons
    EXPECT_EQ(std::string("1234:5678:9abc:def0:1234:5678:0:def0"), std::string(addr.get_ip_address()));
}

TEST_F(TestSocketAddress, constructor_address_ip6_invalid)
{
    SocketAddress addr("zzzz:yyyy:xxxx:def0:1234:5678:9abc:def0", 80);
    EXPECT_EQ(80, addr.get_port());
    EXPECT_EQ(NSAPI_UNSPEC, addr.get_ip_version());
    EXPECT_EQ(NULL, addr.get_ip_address());
}

TEST_F(TestSocketAddress, constructor_address_bytes_ip4)
{
    uint8_t addrBytes[NSAPI_IP_BYTES] = {127, 0, 0, 2};

    SocketAddress addr(addrBytes, NSAPI_IPv4, 80);
    EXPECT_EQ(std::string("127.0.0.2"), std::string(addr.get_ip_address()));
    EXPECT_EQ(80, addr.get_port());
    EXPECT_EQ(NSAPI_IPv4, addr.get_ip_version());
    EXPECT_TRUE(0 == memcmp(addrBytes, static_cast<const uint8_t *>(addr.get_ip_bytes()), sizeof(addrBytes)));
}

TEST_F(TestSocketAddress, constructor_address_bytes_ip6)
{
    uint8_t addrBytes[NSAPI_IP_BYTES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};

    SocketAddress addr(addrBytes, NSAPI_IPv6, 80);
    // the leading zeroes are removed
    EXPECT_EQ(std::string("1:203:405:607:809:a0b:c0d:e0f"), std::string(addr.get_ip_address()));
    EXPECT_EQ(80, addr.get_port());
    EXPECT_EQ(NSAPI_IPv6, addr.get_ip_version());
    EXPECT_TRUE(0 == memcmp(addrBytes, static_cast<const uint8_t *>(addr.get_ip_bytes()), NSAPI_IP_BYTES));
}

TEST_F(TestSocketAddress, constructor_copy)
{
    SocketAddress addr(*address);
    EXPECT_EQ(address->get_ip_address(), addr.get_ip_address());
    EXPECT_EQ(address->get_port(), addr.get_port());
    EXPECT_EQ(address->get_ip_version(), addr.get_ip_version());
}

TEST_F(TestSocketAddress, assignment_and_equality_operator_ip4)
{
    SocketAddress addr;
    address->set_ip_address("127.0.0.2");
    address->set_port(81);
    // First verify assignment operator (including return value) using equality operator.
    EXPECT_EQ((addr = *address), *address);
    // Then check inequality operator
    EXPECT_FALSE(addr != *address);
    // Now proceed to check that the equality operator really works.
    isEqual(addr, *address);
}

TEST_F(TestSocketAddress, assignment_and_equality_operator_ip6)
{
    SocketAddress addr;
    address->set_ip_address("0:0:0:0:0:ffff:7f00:1");
    address->set_port(81);
    // First verify assignment operator (including return value) using equality operator.
    EXPECT_EQ((addr = *address), *address);
    // Then check inequality operator
    EXPECT_FALSE(addr != *address);
    // Now proceed to check that the equality operator really works.
    isEqual(addr, *address);
}

TEST_F(TestSocketAddress, equality_null_check)
{
    address->set_ip_address("127.0.0.2");
    EXPECT_NE(*address, static_cast<SocketAddress>(NULL));
}

TEST_F(TestSocketAddress, equality_wrong_version)
{
    SocketAddress addr4("127.0.0.1", 80);
    SocketAddress addr6("0:0:0:0:0:ffff:7f00:1", 80); // This is converted 127.0.0.1
    EXPECT_NE(addr4, addr6); // But they should still be inequal.
}

TEST_F(TestSocketAddress, bool_operator_false)
{
    EXPECT_FALSE(*address ? true : false);
}

TEST_F(TestSocketAddress, bool_operator_ip4_true)
{
    SocketAddress addr("127.0.0.1", 80);
    EXPECT_TRUE(addr ? true : false);
}

TEST_F(TestSocketAddress, bool_operator_ip6_true)
{
    SocketAddress addr("1234:5678:9abc:def0:1234:5678:9abc:def0", 80);
    EXPECT_TRUE(addr ? true : false);
}

TEST_F(TestSocketAddress, bool_operator_ip6_false)
{
    SocketAddress addr("0:0:0:0:0:0:0:0", 80);
    EXPECT_FALSE(addr ? true : false);
}

