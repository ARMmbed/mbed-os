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
#include "test_m2mserver.h"

TEST_GROUP(M2MServer)
{
  Test_M2MServer* m2m_server;

  void setup()
  {
    m2m_server = new Test_M2MServer();
  }
  void teardown()
  {
    delete m2m_server;
  }
};

TEST(M2MServer, Create)
{
    CHECK(m2m_server->server != NULL);
}

TEST(M2MServer, create_resource_int)
{
    m2m_server->test_create_resource_int();
}

TEST(M2MServer, create_resource)
{
    m2m_server->test_create_resource();
}

TEST(M2MServer, delete_resource)
{
    m2m_server->test_delete_resource();
}

TEST(M2MServer, set_resource_value_int)
{
    m2m_server->test_set_resource_value_int();
}

TEST(M2MServer, set_resource_value_string)
{
    m2m_server->test_set_resource_value_string();
}

TEST(M2MServer, resource_value_int)
{
    m2m_server->test_resource_value_int();
}

TEST(M2MServer, resource_value_string)
{
    m2m_server->test_resource_value_string();
}

TEST(M2MServer, is_resource_present)
{
    m2m_server->test_is_resource_present();
}

TEST(M2MServer, total_resource_count)
{
    m2m_server->test_total_resource_count();
}
