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
#include "test_m2msecurity.h"

TEST_GROUP(M2MSecurity)
{
  Test_M2MSecurity* m2m_security;

  void setup()
  {
    m2m_security = new Test_M2MSecurity();
  }
  void teardown()
  {
    delete m2m_security;
  }
};

TEST(M2MSecurity, Create)
{
    CHECK(m2m_security != NULL);
}

TEST(M2MSecurity, create_resource_int)
{
    m2m_security->test_create_resource_int();
}

TEST(M2MSecurity, delete_resource)
{
    m2m_security->test_delete_resource();
}

TEST(M2MSecurity, set_resource_value_int)
{
    m2m_security->test_set_resource_value_int();
}

TEST(M2MSecurity, set_resource_value_string)
{
    m2m_security->test_set_resource_value_string();
}

TEST(M2MSecurity, set_resource_value_buffer)
{
    m2m_security->test_set_resource_value_buffer();
}

TEST(M2MSecurity, resource_value_int)
{
    m2m_security->test_resource_value_int();
}

TEST(M2MSecurity, resource_value_string)
{
    m2m_security->test_resource_value_string();
}

TEST(M2MSecurity, resource_value_buffer)
{
    m2m_security->test_resource_value_buffer();
}

TEST(M2MSecurity, is_resource_present)
{
    m2m_security->test_is_resource_present();
}

TEST(M2MSecurity, total_resource_count)
{
    m2m_security->test_total_resource_count();
}

TEST(M2MSecurity, m2m_server_constructor)
{
    m2m_security->test_m2m_server_constructor();
}

TEST(M2MSecurity, server_type)
{
    m2m_security->test_server_type();
}
