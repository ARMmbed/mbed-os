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
#ifndef TEST_M2M_INTERFACE_IMPL_H
#define TEST_M2M_INTERFACE_IMPL_H

#include "m2minterfaceimpl.h"

class TestObserver;

uint32_t test_random_callback(void);

class Test_M2MInterfaceImpl
{
public:
    Test_M2MInterfaceImpl();

    virtual ~Test_M2MInterfaceImpl();

    void test_constructor();

    void test_bootstrap();

    void test_cancel_bootstrap();

    void test_register_object();

    void test_update_registration();

    void test_unregister_object();

    void test_set_queue_sleep_handler();

    void test_set_random_number_callback();

    void test_set_entropy_callback();

    void test_set_platform_network_handler();

    void test_coap_message_ready();

    void test_client_registered();

    void test_registration_updated();

    void test_registration_error();

    void test_client_unregistered();

    void test_bootstrap_done();

    void test_bootstrap_error();

    void test_coap_data_processed();

    void test_value_updated();

    void test_data_available();

    void test_socket_error();

    void test_address_ready();

    void test_data_sent();

    void test_timer_expired();

    void test_callback_handler();

    M2MInterfaceImpl*   impl;
    TestObserver        *observer;
    bool visited;
};

#endif // TEST_M2M_INTERFACE_IMPL_H
