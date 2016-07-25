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
#include "test_m2mresource.h"

TEST_GROUP(M2MResource)
{
  Test_M2MResource* m2m_resource;

  void setup()
  {
    m2m_resource = new Test_M2MResource();
  }
  void teardown()
  {
    delete m2m_resource;
  }
};

TEST(M2MResource, Create)
{
    CHECK(m2m_resource != NULL);
}

TEST(M2MResource, copy_constructor)
{
    m2m_resource->test_copy_constructor();
}

TEST(M2MResource, static_resource)
{
    m2m_resource->test_static_resource();
}

TEST(M2MResource, base_type)
{
    m2m_resource->test_base_type();
}

TEST(M2MResource, muliptle_instances)
{
    m2m_resource->test_muliptle_instances();
}

TEST(M2MResource, test_handle_observation_attribute)
{
    m2m_resource->test_handle_observation_attribute();
}

TEST(M2MResource, test_add_resource_instance)
{
    m2m_resource->test_add_resource_instance();
}

TEST(M2MResource, test_remove_resource_instance)
{
    m2m_resource->test_remove_resource_instance();
}

TEST(M2MResource, test_resource_instance_count)
{
    m2m_resource->test_resource_instance_count();
}

TEST(M2MResource, test_resource_instances)
{
    m2m_resource->test_resource_instances();
}

TEST(M2MResource, test_resource_instance)
{
    m2m_resource->test_resource_instance();
}

TEST(M2MResource, test_add_observation_level)
{
    m2m_resource->test_add_observation_level();
}

TEST(M2MResource, test_remove_observation_level)
{
    m2m_resource->test_remove_observation_level();
}

TEST(M2MResource, test_handle_get_request)
{
    m2m_resource->test_handle_get_request();
}

TEST(M2MResource, test_handle_put_request)
{
    m2m_resource->test_handle_put_request();
}

TEST(M2MResource, test_handle_post_request)
{
    m2m_resource->test_handle_post_request();
}

TEST(M2MResource, test_notification_update)
{
    m2m_resource->test_notification_update();
}

TEST(M2MResource, test_set_delayed_response)
{
    m2m_resource->test_set_delayed_response();
}

TEST(M2MResource, test_send_delayed_post_response)
{
    m2m_resource->test_send_delayed_post_response();
}

TEST(M2MResource, test_get_delayed_token)
{
    m2m_resource->test_get_delayed_token();
}

TEST(M2MResource, test_delayed_response)
{
    m2m_resource->test_delayed_response();
}

TEST(M2MResource, test_execute_params)
{
    m2m_resource->test_execute_params();
}
