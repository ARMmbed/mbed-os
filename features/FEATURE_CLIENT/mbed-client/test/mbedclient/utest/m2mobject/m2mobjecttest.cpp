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
#include "test_m2mobject.h"

TEST_GROUP(M2MObject)
{
  Test_M2MObject* m2m_object;

  void setup()
  {
    m2m_object = new Test_M2MObject();
  }
  void teardown()
  {
    delete m2m_object;
  }
};

TEST(M2MObject, Create)
{
    CHECK(m2m_object != NULL);
}

TEST(M2MObject, base_type)
{
    m2m_object->test_base_type();
}

TEST(M2MObject, copy_constructor)
{
    m2m_object->test_copy_constructor();
}

TEST(M2MObject, create_object_instance)
{
    m2m_object->test_create_object_instance();
}

TEST(M2MObject, remove_object_instance)
{
    m2m_object->test_remove_object_instance();
}

TEST(M2MObject, object_instance)
{
    m2m_object->test_object_instance();
}

TEST(M2MObject, instance_count)
{
    m2m_object->test_instance_count();
}

TEST(M2MObject, instances)
{
    m2m_object->test_instances();
}

TEST(M2MObject, handle_get_request)
{
    m2m_object->test_handle_get_request();
}

TEST(M2MObject, handle_put_request)
{
    m2m_object->test_handle_put_request();
}

TEST(M2MObject, handle_post_request)
{
    m2m_object->test_handle_post_request();
}

TEST(M2MObject, notification_update)
{
    m2m_object->test_notification_update();
}

