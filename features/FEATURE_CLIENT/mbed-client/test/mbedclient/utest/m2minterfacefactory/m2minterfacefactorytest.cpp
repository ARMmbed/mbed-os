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
#include "test_m2minterfacefactory.h"

TEST_GROUP(M2MInterfaceFactory)
{
  Test_M2MInterfaceFactory* m2m_factory;

  void setup()
  {
    m2m_factory = new Test_M2MInterfaceFactory();
  }
  void teardown()
  {
    delete m2m_factory;
  }
};


TEST(M2MInterfaceFactory, create_interface)
{
    m2m_factory->test_create_interface();
}

TEST(M2MInterfaceFactory, create_device)
{
     m2m_factory->test_create_device();
}

TEST(M2MInterfaceFactory, create_security)
{
    m2m_factory->test_create_security();
}

TEST(M2MInterfaceFactory, create_server)
{
    m2m_factory->test_create_server();
}

TEST(M2MInterfaceFactory, create_object)
{
    m2m_factory->test_create_object();
}

TEST(M2MInterfaceFactory, create_firmware)
{
    m2m_factory->test_create_firmware();
}
