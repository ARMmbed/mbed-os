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
#include "test_m2mfirmware.h"

TEST_GROUP(M2MFirmware)
{
  Test_M2MFirmware* m2m_firmware;

  void setup()
  {
    m2m_firmware = new Test_M2MFirmware();
  }
  void teardown()
  {
    delete m2m_firmware;
  }
};

TEST(M2MFirmware, Create)
{
    CHECK(m2m_firmware != NULL);
}

TEST(M2MFirmware, create_resource_string)
{
    m2m_firmware->test_create_resource_string();
}

TEST(M2MFirmware, create_resource_int)
{
    m2m_firmware->test_create_resource_int();
}

TEST(M2MFirmware, delete_resource)
{
    m2m_firmware->test_delete_resource();
}

TEST(M2MFirmware, set_resource_value_int)
{
    m2m_firmware->test_set_resource_value_int();
}

TEST(M2MFirmware, set_resource_value_string)
{
    m2m_firmware->test_set_resource_value_string();
}

TEST(M2MFirmware, resource_value_int)
{
    m2m_firmware->test_resource_value_int();
}

TEST(M2MFirmware, resource_value_string)
{
    m2m_firmware->test_resource_value_string();
}

TEST(M2MFirmware, is_resource_present)
{
    m2m_firmware->test_is_resource_present();
}

TEST(M2MFirmware, per_resource_count)
{
    m2m_firmware->test_per_resource_count();
}

TEST(M2MFirmware, total_resource_count)
{
    m2m_firmware->test_total_resource_count();
}

TEST(M2MFirmware, set_resource_value_buffer)
{
    m2m_firmware->test_set_resource_value_buffer();
}

TEST(M2MFirmware, resource_value_buffer)
{
    m2m_firmware->test_resource_value_buffer();
}

TEST(M2MFirmware, resource_name)
{
    m2m_firmware->test_resource_name();
}


