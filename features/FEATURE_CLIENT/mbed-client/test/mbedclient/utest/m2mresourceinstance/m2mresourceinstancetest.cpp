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
#include "test_m2mresourceinstance.h"

TEST_GROUP(M2MResourceInstance)
{
  Test_M2MResourceInstance* m2m_resourceinstance;

  void setup()
  {
    m2m_resourceinstance = new Test_M2MResourceInstance();
  }
  void teardown()
  {
    delete m2m_resourceinstance;
  }
};

TEST(M2MResourceInstance, Create)
{
    CHECK(m2m_resourceinstance != NULL);
}

TEST(M2MResourceInstance, test_static_resource_instance)
{
    m2m_resourceinstance->test_static_resource_instance();
}

TEST(M2MResourceInstance, copy_constructor)
{
    m2m_resourceinstance->test_copy_constructor();
}

TEST(M2MResourceInstance, assignment_constructor)
{
    m2m_resourceinstance->test_assignment_constructor();
}

TEST(M2MResourceInstance, base_type)
{
    m2m_resourceinstance->test_base_type();
}

TEST(M2MResourceInstance, test_handle_observation_attribute)
{
    m2m_resourceinstance->test_handle_observation_attribute();
}

TEST(M2MResourceInstance, test_set_execute_function)
{
    m2m_resourceinstance->test_set_execute_function();
}

TEST(M2MResourceInstance, test_execute)
{
    m2m_resourceinstance->test_execute();
}

TEST(M2MResourceInstance, test_resource_instance_type)
{
    m2m_resourceinstance->test_resource_instance_type();
}

TEST(M2MResourceInstance, test_set_value)
{
    m2m_resourceinstance->test_set_value();
}

TEST(M2MResourceInstance, test_clear_value)
{
    m2m_resourceinstance->test_clear_value();
}

TEST(M2MResourceInstance, test_get_value)
{
    m2m_resourceinstance->test_get_value();
}

TEST(M2MResourceInstance, test_value)
{
    m2m_resourceinstance->test_value();
}

TEST(M2MResourceInstance, test_value_length)
{
    m2m_resourceinstance->test_value_length();
}

TEST(M2MResourceInstance, test_handle_get_request)
{
    m2m_resourceinstance->test_handle_get_request();
}

TEST(M2MResourceInstance, test_handle_put_request)
{
    m2m_resourceinstance->test_handle_put_request();
}

TEST(M2MResourceInstance, test_set_resource_observer)
{
    m2m_resourceinstance->test_set_resource_observer();
}

TEST(M2MResourceInstance, test_get_object_instance_id)
{
    m2m_resourceinstance->test_get_object_instance_id();
}

TEST(M2MResourceInstance, test_get_object_name)
{
    m2m_resourceinstance->test_get_object_name();
}
