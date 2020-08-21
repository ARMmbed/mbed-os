/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include <string.h>
#include "CellularUtil.h"

using namespace mbed_cellular_util;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class Testutil : public testing::Test {
protected:

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

TEST_F(Testutil, test_util_binary_str_to_uint)
{
    char binary_str[] = "011001011101101000";
    uint32_t value = binary_str_to_uint(binary_str, strlen(binary_str) + 1);
    EXPECT_TRUE(value == 104296);
    value = binary_str_to_uint(binary_str, strlen(binary_str));
    EXPECT_TRUE(value == 104296);
    value = binary_str_to_uint(binary_str, strlen(binary_str) - 1);
    EXPECT_TRUE(value == 52148);
    value = binary_str_to_uint(binary_str, strlen(binary_str) - 3);
    EXPECT_TRUE(value == 13037);
    value = binary_str_to_uint(binary_str + 5, strlen(binary_str) - 5);
    EXPECT_TRUE(value == 5992);
    EXPECT_TRUE(0 == binary_str_to_uint(NULL, 5));
    EXPECT_TRUE(0 == binary_str_to_uint(binary_str, 0));
}

TEST_F(Testutil, hex_to_char)
{
    char output;

    // 0
    hex_to_char("00", output);
    EXPECT_EQ((char)0x00, output);

    // <128
    hex_to_char("10", output);
    EXPECT_EQ((char)0x10, output);

    // =128
    hex_to_char("80", output);
    EXPECT_EQ((char)0x80, output);

    // >128
    hex_to_char("FF", output);
    EXPECT_EQ((char)0xFF, output);

    // Null -> output is not modified
    hex_to_char(NULL, output);
    EXPECT_EQ((char)0xFF, output);
}

TEST_F(Testutil, hex_str_to_char_str)
{
    char input[] = "0165AABBCC";
    char output[32];
    EXPECT_EQ(5, hex_str_to_char_str(input, strlen(input), output));
    EXPECT_EQ((char)0x01, output[0]);
    EXPECT_EQ((char)0x65, output[1]);
    EXPECT_EQ((char)0xAA, output[2]);
    EXPECT_EQ((char)0xBB, output[3]);
    EXPECT_EQ((char)0xCC, output[4]);

    // NULL params
    EXPECT_EQ(0, hex_str_to_char_str(NULL, 2, output));
    EXPECT_EQ(0, hex_str_to_char_str(input, strlen(input), NULL));
}

TEST_F(Testutil, test_util_uint_to_binary_string)
{
    char str[33];
    uint_to_binary_str(15, str, 33, 32);
    str[32] = '\0';
    // 15 is "1111" in binary but we ask all 32 bits so it should return "00000000000000000000000000001111"
    EXPECT_STREQ("00000000000000000000000000001111", str);

    // test NULL pointer
    uint_to_binary_str(15, NULL, 0, 32);

    // test give too small buffer
    char too_small[5];
    uint_to_binary_str(15, too_small, 5, 6);
}

TEST_F(Testutil, char_str_to_hex)
{
    // basic conversion test, happy days
    char hex_buf[50];
    uint16_t number_of_hex_chars = char_str_to_hex_str("1234", 4, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    EXPECT_STREQ("31323334", hex_buf);
    EXPECT_EQ(8, number_of_hex_chars);

    number_of_hex_chars = char_str_to_hex_str("wuhuu", 5, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    EXPECT_STREQ("7775687575", hex_buf);
    EXPECT_EQ(10, number_of_hex_chars);

    // First don't omit the leading zero and then omit and check that leading zero is missing
    number_of_hex_chars = char_str_to_hex_str("\nwuhuu", 6, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    EXPECT_STREQ("0A7775687575", hex_buf);
    EXPECT_EQ(12, number_of_hex_chars);

    number_of_hex_chars = char_str_to_hex_str("\nwuhuu", 6, hex_buf, true);
    hex_buf[number_of_hex_chars] = '\0';
    EXPECT_STREQ("A7775687575", hex_buf);
    EXPECT_EQ(11, number_of_hex_chars);

    // test giving a null pointer
    number_of_hex_chars = char_str_to_hex_str(NULL, 4, hex_buf);
    EXPECT_EQ(0, number_of_hex_chars);
    number_of_hex_chars = char_str_to_hex_str("1234", 4, NULL);
    EXPECT_EQ(0, number_of_hex_chars);
}

TEST_F(Testutil, convert_ipv6)
{
    // leading zeros omitted
    char ipv6[64];
    strncpy(ipv6, "1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1", 64);
    convert_ipv6(ipv6);
    EXPECT_STREQ("101:101:101:101:101:101:101:101", ipv6);
    EXPECT_EQ(31, strlen(ipv6));

    // some omitted and some not so much
    strncpy(ipv6, "255.1.120.2.244.12.55.45.201.110.11.2.233.154.85.96", 64);
    convert_ipv6(ipv6);
    EXPECT_STREQ("FF01:7802:F40C:372D:C96E:B02:E99A:5560", ipv6);
    EXPECT_EQ(38, strlen(ipv6));

    // test giving a null pointer
    convert_ipv6(NULL);
}

TEST_F(Testutil, prefer_ipv6)
{
    char tt[20] = "62.241.198.246";
    char temp[64] = "2001:14B8:1000:000:000:000:000:002";

    // not enough space to swap, arrays should stay the same
    prefer_ipv6(tt, sizeof(tt), temp, sizeof(temp));
    EXPECT_STREQ("62.241.198.246", tt);
    EXPECT_STREQ("2001:14B8:1000:000:000:000:000:002", temp);

    // should swap as first one was ip4 and later was ipv6 and enough space
    char tt2[64] = "62.241.198.246";
    prefer_ipv6(tt2, sizeof(tt2), temp, sizeof(temp));
    EXPECT_STREQ("62.241.198.246", temp);
    EXPECT_STREQ("2001:14B8:1000:000:000:000:000:002", tt2);
}

TEST_F(Testutil, separate_ip_addresses)
{
    char s[128] = {'\0'};
    char ip[64] = {0};
    char subnet[64] = {0};

    // IP address representations formats are for IPv6 or IPv4 address, with and without mask, in dotted or colon (IPv6 only) notation

    // IPv6 with a mask in dotted notation
    strncpy(s, "32.1.20.187.1.112.139.245.251.136.232.110.123.51.230.138.0.1.2.3.4.5.6.7.8.9.10.11.12.13.14.15\0",
            sizeof("32.1.20.187.1.112.139.245.251.136.232.110.123.51.230.138.0.1.2.3.4.5.6.7.8.9.10.11.12.13.14.15\0"));
    separate_ip_addresses(NULL, ip, sizeof(ip), subnet, sizeof(subnet));

    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("2001:14BB:170:8BF5:FB88:E86E:7B33:E68A", ip);
    EXPECT_STREQ("001:203:405:607:809:A0B:C0D:E0F", subnet);

    // IPv6 with mask in colon notation
    strncpy(s, "32:1:20:187:1:112:139:1245 0:1:2:3:4:5:6:7\0", sizeof("32:1:20:187:1:112:139:1245 0:1:2:3:4:5:6:7\0"));
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("32:1:20:187:1:112:139:1245", ip);
    EXPECT_STREQ("0:1:2:3:4:5:6:7", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    // IPv6 without mask in dotted notation
    strncpy(s, "0.2.3.4.5.6.7.8.90.100.11.12.13.14.15.16\0", sizeof("0.2.3.4.5.6.7.8.90.100.11.12.13.14.15.16\0"));
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("002:304:506:708:5A64:B0C:D0E:F10", ip);
    EXPECT_STREQ("", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    // IPv6 without mask in colon notation
    strncpy(s, "0032:1:20:187:0:112:139:245f\0", sizeof("0032:1:20:187:0:112:139:245f\0"));
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("0032:1:20:187:0:112:139:245f", ip);
    EXPECT_STREQ("", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    // IPv4 with mask
    strncpy(s, "100.0.3.40.255.6.7.80\0", sizeof("100.0.3.40.255.6.7.80\0"));
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("100.0.3.40", ip);
    EXPECT_STREQ("255.6.7.80", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    // IPv4 without mask
    strncpy(s, "1.255.3.0\0", sizeof("1.255.3.0\0"));
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    EXPECT_STREQ("1.255.3.0", ip);
    EXPECT_STREQ("", subnet);
}

TEST_F(Testutil, get_dynamic_ip_port)
{
    uint16_t port = get_dynamic_ip_port();
    uint16_t port2 = get_dynamic_ip_port();

    EXPECT_TRUE(port != port2);
}

TEST_F(Testutil, int_to_hex_str)
{
    char buf[2];
    int_to_hex_str(100, (char *)buf);

    EXPECT_TRUE(buf[0] == '6');
    EXPECT_TRUE(buf[1] == '4');
}
