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
#ifndef TEST_M2M_RESOURCE_H
#define TEST_M2M_RESOURCE_H

#include "m2mresource.h"

class Callback;
class Handler;

class Test_M2MResource
{
public:

    Test_M2MResource();

    void test_copy_constructor();

    virtual ~Test_M2MResource();

    void test_static_resource();

    void test_base_type();

    void test_muliptle_instances();

    void test_handle_observation_attribute();

    void test_add_resource_instance();

    void test_remove_resource_instance();

    void test_resource_instance();

    void test_resource_instances();

    void test_resource_instance_count();

    void test_add_observation_level();

    void test_remove_observation_level();

    void test_handle_get_request();

    void test_handle_put_request();

    void test_handle_post_request();

    void test_notification_update();

    void test_set_delayed_response();

    void test_send_delayed_post_response();

    void test_get_delayed_token();

    void test_delayed_response();

    void test_execute_params();

    M2MResource* resource;
    Callback *callback;

    Handler* handler;
};

#endif // TEST_M2M_RESOURCE_H
