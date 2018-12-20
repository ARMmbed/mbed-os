/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed_stress_test_file.h"

#ifndef MBED_EXTENDED_TESTS
#error [NOT_SUPPORTED] Use -DMBED_EXTENDED_TESTS flag to enable filesystem tests
#endif

#if !defined(MBED_CONF_STRESS_PROTAGONIST_FILE)
#error [NOT_SUPPORTED] Requires parameters from mbed_lib.json
#endif

#include MBED_CONF_STRESS_PROTAGONIST_FILE

using namespace utest::v1;

static control_t format_storage(const size_t call_count)
{
    mbed_stress_test_format_file();

    return CaseNext;
}

static control_t test_buffer_1(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 1);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 1);

    return CaseNext;
}

static control_t test_buffer_2(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 2);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 2);

    return CaseNext;
}

static control_t test_buffer_4(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 4);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 4);

    return CaseNext;
}

static control_t test_buffer_8(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 8);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 8);

    return CaseNext;
}

static control_t test_buffer_16(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 16);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 16);

    return CaseNext;
}

static control_t test_buffer_32(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 32);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 32);

    return CaseNext;
}

static control_t test_buffer_64(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 64);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 64);

    return CaseNext;
}

static control_t test_buffer_128(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 128);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 128);

    return CaseNext;
}

static control_t test_buffer_256(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 256);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 256);

    return CaseNext;
}

static control_t test_buffer_512(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 512);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 512);

    return CaseNext;
}

static control_t test_buffer_1k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 1024);

    return CaseNext;
}

static control_t test_buffer_2k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 2 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 2 * 1024);

    return CaseNext;
}

static control_t test_buffer_4k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 4 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 4 * 1024);

    return CaseNext;
}

static control_t test_buffer_8k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 8 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 8 * 1024);

    return CaseNext;
}

static control_t test_buffer_16k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 16 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 16 * 1024);

    return CaseNext;
}

static control_t test_buffer_32k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 32 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 32 * 1024);

    return CaseNext;
}

static control_t test_buffer_64k(const size_t call_count)
{
    mbed_stress_test_write_file("mbed-stress-test.txt", 0, story, sizeof(story), 64 * 1024);
    mbed_stress_test_compare_file("mbed-stress-test.txt", 0, story, sizeof(story), 64 * 1024);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Format storage", format_storage),
    Case("story     1", test_buffer_1),
//    Case("story     2", test_buffer_2),
//    Case("story     4", test_buffer_4),
//    Case("story     8", test_buffer_8),
//    Case("story    16", test_buffer_16),
//    Case("story    32", test_buffer_32),
//    Case("story    64", test_buffer_64),
//   Case("story   128", test_buffer_128),
//   Case("story   256", test_buffer_256),
    Case("story   512", test_buffer_512),
    Case("story  1k", test_buffer_1k),
    Case("story  2k", test_buffer_2k),
    Case("story  4k", test_buffer_4k),
    Case("story  8k", test_buffer_8k),
    Case("story 16k", test_buffer_16k),
    Case("story 32k", test_buffer_32k),
};

Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
