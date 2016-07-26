/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CppUTest/TestHarness.h"
#include "ip6string.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

TEST_GROUP(ip6tos)
{
    void setup() {
    }

    void teardown() {
    }
};

TEST(ip6tos, ip6_prefix_tos_func)
{
    char prefix_str[45] = {0};
    char str_len = 0;
    char *expected;

    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;
    expected = "146e:a00::/64";
    str_len = ip6_prefix_tos(prefix, prefix_len, prefix_str);
    CHECK(str_len == 13);
    STRCMP_EQUAL(expected, prefix_str);

    memset(prefix_str, 0, 45);
    str_len = 0;
    expected = "::/0";
    str_len = ip6_prefix_tos(NULL, 0, prefix_str);
    CHECK(str_len == 4);
    STRCMP_EQUAL(expected, prefix_str);

    str_len = 0;
    uint8_t prefix_2[16];
    memset(prefix_2, 0x88, 16);
    expected = "8888:8888:8888:8888:8888:8888:8888:8888/128";
    str_len = ip6_prefix_tos(prefix_2, 128, prefix_str);
    CHECK(str_len == 43);
    STRCMP_EQUAL(expected, prefix_str);

    memset(prefix_str, 0, 45);
    str_len = ip6_prefix_tos(prefix, 130, prefix_str);
    CHECK(str_len == 0);
}

/***********************************************************/
/* Second test group for the old tests that were once lost */

const char string_addr[][40] =
{
    "2001:db8::1:0:0:1",                    // 1
    "2001:db8:aaaa:bbbb:cccc:dddd:eeee:1",  // 2
    "2001:db8::1",                          // 3
    "2001:db8::2:1",                        // 4
    "2001:db8:aaaa:bbbb:cccc:dddd:0:1",     // 5
    "2001:db8::aaaa:0:0:1",                 // 6
    "2001:0:0:1::1",                        // 7
    "2001:0:0:1::",                         // 8
    "2001:db8::",                           // 9
    "::aaaa:0:0:1",                         // 10
    "::1",                                  // 11
    "::",                                   // 12
};


const uint8_t hex_addr[][16] =
{
    { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,1,0,0,0,0,0,1 },
    { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0xee, 0xee, 0x00, 0x01 },
    { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,0,0,0,0,0,0,1 },
    { 0x20, 0x01, 0xd,0xb8, 0,0,0,0,0,0,0,0, 0,2,0,1 },
    { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0,0, 0x00, 0x01 },
    { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0xaa,0xaa,0,0,0,0,0,1 },
    { 0x20, 0x01, 0,0 ,0,0 ,0,1,0,0,0,0,0,0,0,1 },
    { 0x20, 0x01, 0,0 ,0,0 ,0,1,0,0,0,0,0,0,0,0 },
    { 0x20, 0x01, 0xd, 0xb8 },
    { 0,0,0,0,0,0,0,0,0xaa,0xaa,0,0,0,0,0,1 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};

char buf[40];
int i = 0;

TEST_GROUP(ip6tos_2)
{
    void setUp(void)
    {
        memset(buf, 0, 40);
    }

    void tearDown(void)
    {
        i++;
    }
};

TEST(ip6tos_2, test_1)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}

TEST(ip6tos_2, test_2)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_3)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_4)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_5)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_6)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_7)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_8)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_9)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_10)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_11)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
TEST(ip6tos_2, test_12)
{
    CHECK(strlen(string_addr[i]) == ip6tos(hex_addr[i], buf));
    STRCMP_EQUAL(string_addr[i], buf);
}
