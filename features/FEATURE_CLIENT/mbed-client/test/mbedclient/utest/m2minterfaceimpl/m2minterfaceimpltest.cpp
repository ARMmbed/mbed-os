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
#include "test_m2minterfaceimpl.h"

TEST_GROUP(M2MInterfaceImpl)
{
  Test_M2MInterfaceImpl* m2m_interface_impl;

  void setup()
  {
    m2m_interface_impl = new Test_M2MInterfaceImpl();
  }
  void teardown()
  {
    delete m2m_interface_impl;
  }
};

TEST(M2MInterfaceImpl, create)
{
    CHECK(m2m_interface_impl->impl != NULL);
}

TEST(M2MInterfaceImpl, test_constructor)
{
    m2m_interface_impl->test_constructor();
}

TEST(M2MInterfaceImpl, bootstrap)
{
    m2m_interface_impl->test_bootstrap();
}

TEST(M2MInterfaceImpl, cancel_bootstrap)
{
    m2m_interface_impl->test_cancel_bootstrap();
}

TEST(M2MInterfaceImpl, register_object)
{
    m2m_interface_impl->test_register_object();
}

TEST(M2MInterfaceImpl, update_registration)
{
    m2m_interface_impl->test_update_registration();
}

TEST(M2MInterfaceImpl, unregister_object)
{
    m2m_interface_impl->test_unregister_object();
}

TEST(M2MInterfaceImpl, set_queue_sleep_handler)
{
    m2m_interface_impl->test_set_queue_sleep_handler();
}

TEST(M2MInterfaceImpl, test_set_random_number_callback)
{
    m2m_interface_impl->test_set_random_number_callback();
}

TEST(M2MInterfaceImpl, test_set_entropy_callback)
{
    m2m_interface_impl->test_set_entropy_callback();
}

TEST(M2MInterfaceImpl, test_set_platform_network_handler)
{
    m2m_interface_impl->test_set_platform_network_handler();
}

TEST(M2MInterfaceImpl, coap_message_ready)
{
    m2m_interface_impl->test_coap_message_ready();
}

TEST(M2MInterfaceImpl, client_registered)
{
    m2m_interface_impl->test_client_registered();
}

TEST(M2MInterfaceImpl, test_registration_updated)
{
    m2m_interface_impl->test_registration_updated();
}

TEST(M2MInterfaceImpl, registration_error)
{
    m2m_interface_impl->test_registration_error();
}

TEST(M2MInterfaceImpl, client_unregistered)
{
    m2m_interface_impl->test_client_unregistered();
}

TEST(M2MInterfaceImpl, bootstrap_done)
{
    m2m_interface_impl->test_bootstrap_done();
}

TEST(M2MInterfaceImpl, bootstrap_error)
{
    m2m_interface_impl->test_bootstrap_error();
}

TEST(M2MInterfaceImpl, coap_data_processed)
{
    m2m_interface_impl->test_coap_data_processed();
}

TEST(M2MInterfaceImpl, test_value_updated)
{
    m2m_interface_impl->test_value_updated();
}

TEST(M2MInterfaceImpl, data_available)
{
    m2m_interface_impl->test_data_available();
}

TEST(M2MInterfaceImpl, socket_error)
{
    m2m_interface_impl->test_socket_error();
}

TEST(M2MInterfaceImpl, address_ready)
{
    m2m_interface_impl->test_address_ready();
}

TEST(M2MInterfaceImpl, data_sent)
{
    m2m_interface_impl->test_data_sent();
}

TEST(M2MInterfaceImpl, timer_expired)
{
    m2m_interface_impl->test_timer_expired();
}
