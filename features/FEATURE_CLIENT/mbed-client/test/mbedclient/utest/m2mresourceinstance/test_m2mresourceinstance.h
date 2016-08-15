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
#ifndef TEST_M2M_RESOURCE_INSTANCE_H
#define TEST_M2M_RESOURCE_INSTANCE_H

#include "m2mresourceinstance.h"

class Handler;
class Callback;
class ResourceCallback;

class Test_M2MResourceInstance
{
public:
    Test_M2MResourceInstance();

    ~Test_M2MResourceInstance();

    void test_copy_constructor();

    void test_assignment_constructor();

    void test_static_resource_instance();

    void test_base_type();

    void test_resource_instance_type();

    void test_handle_observation_attribute();

    void test_set_execute_function();

    void test_execute();

    void test_set_value();

    void test_clear_value();

    void test_get_value();

    void test_value();

    void test_value_length();

    void test_handle_get_request();

    void test_handle_put_request();

    void test_set_resource_observer();

    void test_get_object_name();

    void test_get_object_instance_id();

    M2MResourceInstance* resource_instance;

    Callback *callback;

    Handler* handler;
};

#endif // TEST_M2M_RESOURCE_INSTANCE_H
