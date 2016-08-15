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
#include "test_nsdlaccesshelper.h"

TEST_GROUP(NsdlAccessHelper)
{
  Test_NsdlAccessHelper* nsdl;

  void setup()
  {
    nsdl = new Test_NsdlAccessHelper();
  }
  void teardown()
  {
    delete nsdl;
  }
};


TEST(NsdlAccessHelper, create)
{
    CHECK(nsdl != NULL);
}

TEST(NsdlAccessHelper, test_nsdl_c_callback)
{
    nsdl->test_nsdl_c_callback();
}

TEST(NsdlAccessHelper, test_nsdl_c_memory_alloc)
{
    nsdl->test_nsdl_c_memory_alloc();
}

TEST(NsdlAccessHelper, test_nsdl_c_memory_free)
{
    nsdl->test_nsdl_c_memory_free();
}

TEST(NsdlAccessHelper, test_nsdl_c_send_to_server)
{
    nsdl->test_nsdl_c_send_to_server();
}

TEST(NsdlAccessHelper, test_nsdl_c_received_from_server)
{
    nsdl->test_nsdl_c_received_from_server();
}


TEST(NsdlAccessHelper, test_socket_malloc)
{
    nsdl->test_socket_malloc();
}

TEST(NsdlAccessHelper, test_socket_free)
{
    nsdl->test_socket_free();
}

TEST(NsdlAccessHelper, test_mutex_claim)
{
    nsdl->test_mutex_claim();
}

TEST(NsdlAccessHelper, test_mutex_release)
{
    nsdl->test_mutex_release();
}
