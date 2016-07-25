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
#include "test_m2mdevice.h"

TEST_GROUP(M2MDevice)
{
  Test_M2MDevice* m2m_device;

  void setup()
  {
    m2m_device = new Test_M2MDevice();
  }
  void teardown()
  {
    delete m2m_device;
  }
};

TEST(M2MDevice, Create)
{
    CHECK(m2m_device != NULL);
}

TEST(M2MDevice, create_resource_instance)
{
    m2m_device->test_create_resource_instance();
}

TEST(M2MDevice, create_resource_string)
{
    m2m_device->test_create_resource_string();
}

TEST(M2MDevice, create_resource_int)
{
    m2m_device->test_create_resource_int();
}

TEST(M2MDevice, create_resource_no_param)
{
    m2m_device->test_create_resource_no_param();
}

TEST(M2MDevice, delete_resource)
{
    m2m_device->test_delete_resource();
}

TEST(M2MDevice, delete_resource_instance)
{
    m2m_device->test_delete_resource_instance();
}

TEST(M2MDevice, set_resource_value_int)
{
    m2m_device->test_set_resource_value_int();
}

TEST(M2MDevice, set_resource_value_string)
{
    m2m_device->test_set_resource_value_string();
}

TEST(M2MDevice, resource_value_int)
{
    m2m_device->test_resource_value_int();
}

TEST(M2MDevice, resource_value_string)
{
    m2m_device->test_resource_value_string();
}

TEST(M2MDevice, is_resource_present)
{
    m2m_device->test_is_resource_present();
}

TEST(M2MDevice, per_resource_count)
{
    m2m_device->test_per_resource_count();
}

TEST(M2MDevice, total_resource_count)
{
    m2m_device->test_total_resource_count();
}
