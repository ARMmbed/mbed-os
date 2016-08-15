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
#include "test_m2mnsdlinterface.h"

TEST_GROUP(M2MNsdlInterface)
{
  Test_M2MNsdlInterface* m2m_nsdl_interface;

  void setup()
  {
    m2m_nsdl_interface = new Test_M2MNsdlInterface();
  }
  void teardown()
  {
    delete m2m_nsdl_interface;
    m2m_nsdl_interface = NULL;
  }
};


TEST(M2MNsdlInterface, create)
{
    CHECK(m2m_nsdl_interface->nsdl != NULL);
}

TEST(M2MNsdlInterface, memory_free)
{
    m2m_nsdl_interface->test_memory_free();
}

TEST(M2MNsdlInterface, memory_alloc)
{
    m2m_nsdl_interface->test_memory_alloc();
}

TEST(M2MNsdlInterface, send_unregister_message)
{
    m2m_nsdl_interface->test_send_unregister_message();
}

TEST(M2MNsdlInterface, send_update_registration)
{
    m2m_nsdl_interface->test_send_update_registration();
}

TEST(M2MNsdlInterface, send_register_message)
{
    m2m_nsdl_interface->test_send_register_message();
}

TEST(M2MNsdlInterface, create_bootstrap_resource)
{
    m2m_nsdl_interface->test_create_bootstrap_resource();
}

TEST(M2MNsdlInterface, delete_nsdl_resource)
{
    m2m_nsdl_interface->test_delete_nsdl_resource();
}

TEST(M2MNsdlInterface, delete_endpoint)
{
    m2m_nsdl_interface->test_delete_endpoint();
}

TEST(M2MNsdlInterface, create_endpoint)
{
    m2m_nsdl_interface->test_create_endpoint();
}

TEST(M2MNsdlInterface, send_to_server_callback)
{
    m2m_nsdl_interface->test_send_to_server_callback();
}

TEST(M2MNsdlInterface, received_from_server_callback)
{
    m2m_nsdl_interface->test_received_from_server_callback();
}

TEST(M2MNsdlInterface, create_nsdl_list_structure)
{
    m2m_nsdl_interface->test_create_nsdl_list_structure();
}

TEST(M2MNsdlInterface, resource_callback)
{
    m2m_nsdl_interface->test_resource_callback();
}

TEST(M2MNsdlInterface, resource_callback_get)
{
    m2m_nsdl_interface->test_resource_callback_get();
}


TEST(M2MNsdlInterface, resource_callback_post)
{
    m2m_nsdl_interface->test_resource_callback_post();
}

TEST(M2MNsdlInterface, resource_callback_delete)
{
    m2m_nsdl_interface->test_resource_callback_delete();
}

TEST(M2MNsdlInterface, resource_callback_reset)
{
    m2m_nsdl_interface->test_resource_callback_reset();
}

TEST(M2MNsdlInterface, timer_expired)
{
    m2m_nsdl_interface->test_timer_expired();
}

TEST(M2MNsdlInterface, process_received_data)
{
    m2m_nsdl_interface->test_process_received_data();
}

TEST(M2MNsdlInterface, stop_timers)
{
    m2m_nsdl_interface->test_stop_timers();
}

TEST(M2MNsdlInterface, observation_to_be_sent)
{
    m2m_nsdl_interface->test_observation_to_be_sent();
}

TEST(M2MNsdlInterface, resource_to_be_deleted)
{
    m2m_nsdl_interface->test_resource_to_be_deleted();
}

TEST(M2MNsdlInterface, value_updated)
{
    m2m_nsdl_interface->test_value_updated();
}

TEST(M2MNsdlInterface, find_resource)
{
    m2m_nsdl_interface->test_find_resource();
}

TEST(M2MNsdlInterface, remove_object)
{
    m2m_nsdl_interface->test_remove_object();
}

TEST(M2MNsdlInterface, add_object_to_list)
{
    m2m_nsdl_interface->test_add_object_to_list();
}

TEST(M2MNsdlInterface, test_send_delayed_response)
{
    m2m_nsdl_interface->test_send_delayed_response();
}

TEST(M2MNsdlInterface, resource_callback_put)
{
    m2m_nsdl_interface->test_resource_callback_put();
}

TEST(M2MNsdlInterface, get_nsdl_handle)
{
    m2m_nsdl_interface->test_get_nsdl_handle();
}

TEST(M2MNsdlInterface, endpoint_name)
{
    m2m_nsdl_interface->test_endpoint_name();
}

