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
#include "ipv6_test_values.h"
#include <stdlib.h>
#include <string.h>

TEST_GROUP(stoip6)
{
    void setup() {
    }

    void teardown() {
    }
};

TEST(stoip6, ZeroAddress)
{
    for (int i = 0; ipv6_test_values[i].addr; i++) {
        uint8_t ip[16];
        char *addr = ipv6_test_values[i].addr;
        stoip6(addr, strlen(addr), ip);
        CHECK(0 == memcmp(ip, ipv6_test_values[i].bin, 16));
    }
}

TEST(stoip6, TooShort)
{
    char *addr = "FFFF:FFFF:";
    uint8_t ip[16];
    uint8_t correct[16] = {0};
    // This should stop parsing when too short address given.
    // Despite partial parsing, the entire buffer should be filled with zeroes
    CHECK(false == stoip6(addr, strlen(addr), ip));
    CHECK(0 == memcmp(ip, correct, 16));
}

TEST(stoip6, TooLongString)
{
    // String len must be less than 40, otherwise not valid
    char *addr = "FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:";
    uint8_t ip[16] = {0};
    uint8_t correct[16] = {0};
    // This should not fill anything, too long string.
    // This is basically only validation we do
    CHECK(false == stoip6(addr, strlen(addr), ip));
    CHECK(0 == memcmp(ip, correct, 16));
}

TEST(stoip6, TooManyFields)
{
    // String len must be less than 40
    char *addr = "FF:FF:FF:FF:FF:FF:FFFF:FFFF:FFFF:FFFF:";
    uint8_t ip[16] = {0};
    uint8_t correct[16] = {0};

    CHECK(false == stoip6(addr, strlen(addr), ip));
    CHECK(0 == memcmp(ip, correct, 16));
}

TEST(stoip6, Prefixlen)
{
    CHECK(0 == sipv6_prefixlength("::"));
    CHECK(64 == sipv6_prefixlength("::/64"));
}

// This test revealed a off-by-one error in stoip6() when the code was ran under valgrind.
// The IP address is copied from the test_2duts_ping -test, where the valgrind message
// was originally spotted.
TEST(stoip6, RegressionTestForOffByOne)
{
    const char *sourceAddr = "fd00:db8::643f:f54a:ec29:cdbb";

    // Use heap based test string to make valgrind spot the problem.
    char *sourceTemp = (char *)strdup(sourceAddr);
    size_t sourceTempLen = strlen(sourceTemp);

    uint8_t ip[16];
    const uint8_t correct[16] = { 0xfd, 0x00, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
                                  0x64, 0x3f, 0xf5, 0x4a, 0xec, 0x29, 0xcd, 0xbb
                                };

    CHECK(true == stoip6(sourceTemp, sourceTempLen, ip));
    CHECK(0 == memcmp(ip, correct, 16));

    free(sourceTemp);
}

// Test various illegal formats to ensure proper rejection
TEST(stoip6, InvalidAddresses)
{
    uint8_t ip[16];
    uint8_t correct[16] = {0};

    const char *invalidArray[] = {
        "FFFF:FFFF::FFFF::FFFF", // Two ::
        "F:F:F:FqF:F:F:F:F",     // Non-hex character
        "F:F:F:FFFFF:F:F:F:F"    // >4 hex characters in a segment
    };

    for (uint8_t i = 0; i < 3; ++i) {
        CHECK(false == stoip6(invalidArray[i], strlen(invalidArray[i]), ip));
        CHECK(0 == memcmp(ip, correct, 16));
    }
}

/***********************************************************/
/* Second test group for the old tests that were once lost */

const char string_addr[][40] = {
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


const uint8_t hex_addr[][16] = {
    { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0xee, 0xee, 0x00, 0x01 },
    { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1 },
    { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0, 0, 0x00, 0x01 },
    { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0xaa, 0xaa, 0, 0, 0, 0, 0, 1 },
    { 0x20, 0x01, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0x20, 0x01, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x20, 0x01, 0xd, 0xb8 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0xaa, 0xaa, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

char buf[40];
int i = 0;

TEST_GROUP(stoip6_2)
{
    void setUp(void) {
        memset(buf, 0, 40);
    }

    void tearDown(void) {
        i++;
    }
};

/* Unity test code starts */


TEST(stoip6_2, test_2_1)
{
    i = 0;
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}

TEST(stoip6_2, test_2_2)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_3)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_4)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_5)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_6)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_7)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_8)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_9)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, strlen(buf)));
}
TEST(stoip6_2, test_2_10)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, 16));
}
TEST(stoip6_2, test_2_11)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, 16));
}
TEST(stoip6_2, test_2_12)
{
    CHECK(true == stoip6(string_addr[i], strlen(string_addr[i]), buf));
    CHECK(0 == memcmp(hex_addr[i], buf, 16));
}

/***********************************************************/
/* Third test group for stoip6_prefix */

const char string_prefix_addr[][40] = {
    "2001:db8::1:0:0:1/64",     // 1
    "2001::/60",                // 2
    "::1/48",                   // 3
    "::/00",                    // 4
    "2002::02/99",              // 5
    "2003::03/",                // 6
    "2004::04",                 // 7
    "2005::05/2000",            // 8
    "2005::05/-1",              // 9
};


const uint8_t hex_prefix_addr[][16] = {
    { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },  // 1
    { 0x20, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // 2
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },             // 3
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },             // 4
    { 0x20, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },       // 5
    { 0x20, 0x03, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },       // 6
    { 0x20, 0x04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },       // 7
    { 0x20, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },       // 8
    { 0x20, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },       // 9
};

const int_fast16_t prefix_len_tbl[] = {64, 60, 48, 0, 99, 0, -1, -1, -1};
const int prefix_status_tbl[] = {0, 0, 0, 0, 0, 0, 0, -1, -1};

TEST_GROUP(stoip6_3)
{
    void setup() {
    }

    void teardown() {
    }
};

TEST(stoip6_3, stoip6_prefix_test)
{
    for (int i = 0; i < 9; i++) {
        uint8_t ip[16];
        int_fast16_t prefix_len;
        int result;
        const char *addr = &string_prefix_addr[i][0];

        result = stoip6_prefix(addr, ip, &prefix_len);
        CHECK(result == prefix_status_tbl[i]);
        if (result == 0) {
            CHECK(0 == memcmp(ip, &hex_prefix_addr[i][0], 16));
            CHECK(prefix_len == prefix_len_tbl[i])
        }
    }
}

