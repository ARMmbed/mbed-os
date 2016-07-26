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
#include "test_m2mstring.h"

TEST_GROUP(M2MString)
{
  Test_M2MString* m2m_string;

  void setup()
  {
    m2m_string = new Test_M2MString();
  }
  void teardown()
  {
    delete m2m_string;
  }
};

TEST(M2MString, Create)
{
    CHECK(m2m_string != NULL);
}

TEST(M2MString, copy_constructor)
{
    m2m_string->test_copy_constructor();
}

TEST(M2MString, test_operator_assign)
{
    m2m_string->test_operator_assign();
}

TEST(M2MString, test_operator_add)
{
    m2m_string->test_operator_add();
}

TEST(M2MString, test_push_back)
{
    m2m_string->test_push_back();
}

TEST(M2MString, test_operator_equals)
{
    m2m_string->test_operator_equals();
}

TEST(M2MString, test_clear)
{
    m2m_string->test_clear();
}

TEST(M2MString, test_size)
{
    m2m_string->test_size();
}

TEST(M2MString, test_length)
{
    m2m_string->test_length();
}

TEST(M2MString, test_capacity)
{
    m2m_string->test_capacity();
}

TEST(M2MString, test_empty)
{
    m2m_string->test_empty();
}

TEST(M2MString, test_c_str)
{
    m2m_string->test_c_str();
}

TEST(M2MString, test_reserve)
{
    m2m_string->test_reserve();
}

TEST(M2MString, test_resize)
{
    m2m_string->test_resize();
}

TEST(M2MString, test_swap)
{
    m2m_string->test_swap();
}

TEST(M2MString, test_substr)
{
    m2m_string->test_substr();
}

TEST(M2MString, test_operator_get)
{
    m2m_string->test_operator_get();
}

TEST(M2MString, test_at)
{
    m2m_string->test_at();
}

TEST(M2MString, test_erase)
{
    m2m_string->test_erase();
}

TEST(M2MString, test_append)
{
    m2m_string->test_append();
}

TEST(M2MString, test_append_raw)
{
    m2m_string->test_append_raw();
}

TEST(M2MString, test_append_int)
{
    m2m_string->test_append_int();
}

TEST(M2MString, test_compare)
{
    m2m_string->test_compare();
}

TEST(M2MString, test_find_last_of)
{
    m2m_string->test_find_last_of();
}

TEST(M2MString, test_operator_lt)
{
    m2m_string->test_operator_lt();
}

TEST(M2MString, test_reverse)
{
    m2m_string->test_reverse();
}

TEST(M2MString, test_itoa_c)
{
    m2m_string->test_itoa_c();
}

TEST(M2MString, test_convert_integer_to_array)
{
    m2m_string->test_convert_integer_to_array();
}
