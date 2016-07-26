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
//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"
#include "test_libtrace.h"

TEST_GROUP(LibTrace)
{
    Test_LibTrace *lib_trace;

    void setup() {
        lib_trace = new Test_LibTrace();
    }

    void teardown() {
        delete lib_trace;
    }
};

TEST(LibTrace, Create)
{
    CHECK(lib_trace != NULL);
}

TEST(LibTrace, test_libTrace_tracef)
{
    lib_trace->test_libTrace_tracef();
}

TEST(LibTrace, test_libTrace_trace_ipv6_prefix)
{
    lib_trace->test_libTrace_trace_ipv6_prefix();
}
