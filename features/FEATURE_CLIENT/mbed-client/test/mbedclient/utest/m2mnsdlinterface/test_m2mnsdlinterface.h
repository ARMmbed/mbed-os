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
#ifndef TEST_M2M_NSDL_INTERFACE_H
#define TEST_M2M_NSDL_INTERFACE_H

#include "m2mnsdlinterface.h"
#include "common_stub.h"

class TestObserver;

class Test_M2MNsdlInterface
{
public:
    Test_M2MNsdlInterface();

    virtual ~Test_M2MNsdlInterface();

    void test_create_endpoint();

    void test_delete_endpoint();

    void test_create_nsdl_list_structure();

    void test_delete_nsdl_resource();

    void test_create_bootstrap_resource();

    void test_send_register_message();

    void test_send_update_registration();

    void test_send_unregister_message();

    void test_memory_alloc();

    void test_memory_free();

    void test_send_to_server_callback();

    void test_received_from_server_callback();

    void test_resource_callback();

    void test_resource_callback_get();

    void test_resource_callback_put();

    void test_resource_callback_post();

    void test_resource_callback_delete();

    void test_resource_callback_reset();

    void test_bootstrap_done_callback();

    void test_process_received_data();

    void test_stop_timers();

    void test_timer_expired();

    void test_observation_to_be_sent();

    void test_resource_to_be_deleted();

    void test_value_updated();

    void test_find_resource();

    void test_remove_object();

    void test_add_object_to_list(); //Special: Would be too difficult to test in normal ways

    void test_send_delayed_response();

    void test_get_nsdl_handle();

    void test_endpoint_name();

    M2MNsdlInterface* nsdl;

    TestObserver *observer;
};

#endif // TEST_M2M_NSDL_INTERFACE_H
