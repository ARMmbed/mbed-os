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
#include "CppUTest/TestHarness.h"
#include "test_util.h"
#include <string.h>
#include "CellularUtil.h"

using namespace mbed_cellular_util;

Test_util::Test_util()
{

}

Test_util::~Test_util()
{
}

void Test_util::test_util_uint_to_binary_string()
{
    char str[33];
    uint_to_binary_str(15, str, 33, 32);
    str[32] = '\0';
    // 15 is "1111" in binary but we ask all 32 bits so it should return "00000000000000000000000000001111"
    STRCMP_EQUAL("00000000000000000000000000001111", str);

    // test NULL pointer
    uint_to_binary_str(15, NULL, 0, 32);

    // test give too small buffer
    char too_small[5];
    uint_to_binary_str(15, too_small, 5, 6);
}

void Test_util::test_util_char_str_to_hex()
{
    // basic conversion test, happy days
    char hex_buf[50];
    uint16_t number_of_hex_chars = char_str_to_hex_str("1234", 4, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    STRCMP_EQUAL("31323334", hex_buf);
    LONGS_EQUAL(8, number_of_hex_chars);

    number_of_hex_chars = char_str_to_hex_str("wuhuu", 5, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    STRCMP_EQUAL("7775687575", hex_buf);
    LONGS_EQUAL(10, number_of_hex_chars);

    // First don't omit the leading zero and then omit and check that leading zero is missing
    number_of_hex_chars = char_str_to_hex_str("\nwuhuu", 6, hex_buf);
    hex_buf[number_of_hex_chars] = '\0';
    STRCMP_EQUAL("0A7775687575", hex_buf);
    LONGS_EQUAL(12, number_of_hex_chars);

    number_of_hex_chars = char_str_to_hex_str("\nwuhuu", 6, hex_buf, true);
    hex_buf[number_of_hex_chars] = '\0';
    STRCMP_EQUAL("A7775687575", hex_buf);
    LONGS_EQUAL(11, number_of_hex_chars);

    // test giving a null pointer
    number_of_hex_chars = char_str_to_hex_str(NULL, 4, hex_buf);
    LONGS_EQUAL(0, number_of_hex_chars);
    number_of_hex_chars = char_str_to_hex_str("1234", 4, NULL);
    LONGS_EQUAL(0, number_of_hex_chars);
}

void Test_util::test_util_convert_ipv6()
{
    // leading zeros omitted
    char ipv6[64];
    strncpy(ipv6, "1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1", 64);
    convert_ipv6(ipv6);
    STRCMP_EQUAL("101:101:101:101:101:101:101:101", ipv6);
    LONGS_EQUAL(31, strlen(ipv6));

    // some omitted and some not so much
    strncpy(ipv6, "255.1.120.2.244.12.55.45.201.110.11.2.233.154.85.96", 64);
    convert_ipv6(ipv6);
    STRCMP_EQUAL("FF01:7802:F40C:372D:C96E:B02:E99A:5560", ipv6);
    LONGS_EQUAL(38, strlen(ipv6));

    // test giving a null pointer
    convert_ipv6(NULL);
}

void Test_util::test_util_prefer_ipv6()
{
    char tt[20] = "62.241.198.246";
    char temp[64] = "2001:14B8:1000:000:000:000:000:002";

    // not enough space to swap, arrays should stay the same
    prefer_ipv6(tt, sizeof(tt), temp, sizeof(temp));
    STRCMP_EQUAL("62.241.198.246", tt);
    STRCMP_EQUAL("2001:14B8:1000:000:000:000:000:002", temp);

    // should swap as first one was ip4 and later was ipv6 and enough space
    char tt2[64] = "62.241.198.246";
    prefer_ipv6(tt2, sizeof(tt2), temp, sizeof(temp));
    STRCMP_EQUAL("62.241.198.246", temp);
    STRCMP_EQUAL("2001:14B8:1000:000:000:000:000:002", tt2);
}

void Test_util::test_util_separate_ip_addresses()
{
    char* s = (char*)malloc(128);

    char ip[64] = {0};
    char subnet[64] = {0};

    strncpy(s, "32.1.20.187.1.112.139.245.251.136.232.110.123.51.230.138.0.1.2.3.4.5.6.7.8.9.10.11.12.13.14.15", 94);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("2001:14BB:170:8BF5:FB88:E86E:7B33:E68A", ip);
    STRCMP_EQUAL("001:203:405:607:809:A0B:C0D:E0F", subnet);

    strncpy(s, "32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138 0:1:2:3:4:5:6:7:8:9:10:11:12:13:14:15", 94);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138", ip);
    STRCMP_EQUAL("0:1:2:3:4:5:6:7:8:9:10:11:12:13:14:15", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "1.2.3.4\0", 8);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("1.2.3.4", ip);
    STRCMP_EQUAL("", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "1.2.3.4.5.6.7.8\0", 16);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("1.2.3.4", ip);
    STRCMP_EQUAL("5.6.7.8", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "1.2.3.4.5.6.7.8.9.10.11.12.13.14.15.16\0", 39);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("102:304:506:708:90A:B0C:D0E:F10", ip);
    STRCMP_EQUAL("", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138\0", 57);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138", ip);
    STRCMP_EQUAL("", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "1.2.3.4 32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138\0", 65);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("1.2.3.4", ip);
    STRCMP_EQUAL("32:1:20:187:1:112:139:245:251:136:232:110:123:51:230:138", subnet);

    ip[0] = '\0';
    subnet[0] = '\0';
    strncpy(s, "1.2.3.4 5.6.7.8.9.10.11.12.13.14.15.16.17.18.19.20\0", 51);
    separate_ip_addresses(s, ip, sizeof(ip), subnet, sizeof(subnet));
    STRCMP_EQUAL("1.2.3.4", ip);
    STRCMP_EQUAL("506:708:90A:B0C:D0E:F10:1112:1314", subnet);
    STRCMP_EQUAL("1.2.3.4 5.6.7.8.9.10.11.12.13.14.15.16.17.18.19.20", s);

}
