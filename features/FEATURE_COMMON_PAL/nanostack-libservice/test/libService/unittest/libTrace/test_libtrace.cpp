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
#include "test_libtrace.h"
#include <string.h>
#include "ip6tos_stub.h"

char buf[1024];

static void myprint(const char *str)
{
    strcpy(buf, str);
}

Test_LibTrace::Test_LibTrace()
{
    trace_init();
    set_trace_print_function(myprint);
}

Test_LibTrace::~Test_LibTrace()
{
    trace_free();
}

void Test_LibTrace::test_libTrace_tracef()
{
    set_trace_config(TRACE_MODE_PLAIN | TRACE_ACTIVE_LEVEL_ALL);

    memset(buf, 0, 1024);
    unsigned char longStr[1000] = {0x76};
    tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", trace_array(longStr, 1000));
    CHECK(buf[0] == '7');

    memset(buf, 0, 1024);
    const char longStr2[200] = {0x36};
    tracef(TRACE_LEVEL_DEBUG, "mygr", longStr2);
    CHECK(buf[0] == '6');


}

void Test_LibTrace::test_libTrace_trace_ipv6_prefix()
{
    memset(buf, 0, 1024);
    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;
    ip6tos_stub.c = '7';
    ip6tos_stub.h = false;
    char *str = trace_ipv6_prefix(prefix, prefix_len);
    SimpleString ss(str);

    CHECK("/64" == ss);

    ip6tos_stub.h = true;
    char *str2 = trace_ipv6_prefix(prefix, prefix_len);
    SimpleString ss2(str2);

    CHECK("7/64" == ss2);
}
