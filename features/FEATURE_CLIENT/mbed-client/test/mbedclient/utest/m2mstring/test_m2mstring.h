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
#ifndef TEST_M2M_STRING_H
#define TEST_M2M_STRING_H

#include "m2mstring.h"

using namespace m2m;

class Test_M2MString
{
public:
    Test_M2MString();
    virtual ~Test_M2MString();

    void test_copy_constructor();
    void test_operator_assign();
    void test_operator_add();
    void test_push_back();
    void test_operator_equals();
    void test_clear();
    void test_size();
    void test_length();
    void test_capacity();
    void test_empty();
    void test_c_str();
    void test_reserve();
    void test_resize();
    void test_swap();
    void test_substr();
    void test_operator_get();
    void test_at();
    void test_erase();
    void test_append();
    void test_append_raw();
    void test_append_int();
    void test_compare();
    void test_find_last_of();
    void test_operator_lt();
    void test_reverse();
    void test_itoa_c();
    void test_convert_integer_to_array();

    String* str;
};

#endif // TEST_M2M_STRING_H
