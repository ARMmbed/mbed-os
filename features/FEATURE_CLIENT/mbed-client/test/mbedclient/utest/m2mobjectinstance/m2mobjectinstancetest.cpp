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
#include "test_m2mobjectinstance.h"

TEST_GROUP(M2MObjectInstance)
{
  Test_M2MObjectInstance* m2m_object_instance;

  void setup()
  {
    m2m_object_instance = new Test_M2MObjectInstance();
  }
  void teardown()
  {    
    delete m2m_object_instance;
  }
};

TEST(M2MObjectInstance, Create)
{
    CHECK(m2m_object_instance != NULL);
}

TEST(M2MObjectInstance, copy_constructor)
{
    m2m_object_instance->test_copy_constructor();
}

TEST(M2MObjectInstance, create_static_resource)
{
    m2m_object_instance->test_create_static_resource();
}

TEST(M2MObjectInstance, create_static_resource_instance)
{
    m2m_object_instance->test_create_static_resource_instance();
}

TEST(M2MObjectInstance, create_dynamic_resource)
{
    m2m_object_instance->test_create_dynamic_resource();
}

TEST(M2MObjectInstance, create_dynamic_resource_instance)
{
    m2m_object_instance->test_create_dynamic_resource_instance();
}

TEST(M2MObjectInstance, remove_resource)
{
    m2m_object_instance->test_remove_resource();
}

TEST(M2MObjectInstance, remove_resource_instance)
{
    m2m_object_instance->test_remove_resource_instance();
}

TEST(M2MObjectInstance, resource)
{
    m2m_object_instance->test_resource();
}

TEST(M2MObjectInstance, resources)
{
    m2m_object_instance->test_resources();
}

TEST(M2MObjectInstance, resource_count)
{
    m2m_object_instance->test_resource_count();
}

TEST(M2MObjectInstance, total_resource_count)
{
    m2m_object_instance->test_total_resource_count();
}

TEST(M2MObjectInstance, base_type)
{
    m2m_object_instance->test_base_type();
}

TEST(M2MObjectInstance, handle_get_request)
{
    m2m_object_instance->test_handle_get_request();
}

TEST(M2MObjectInstance, handle_put_request)
{
    m2m_object_instance->test_handle_put_request();
}

TEST(M2MObjectInstance, handle_post_request)
{
    m2m_object_instance->test_handle_post_request();
}

TEST(M2MObjectInstance, notification_update)
{
    m2m_object_instance->test_notification_update();
}

