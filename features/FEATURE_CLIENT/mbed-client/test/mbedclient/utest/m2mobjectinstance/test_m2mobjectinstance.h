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
#ifndef TEST_M2M_OBJECT_INSTANCE_H
#define TEST_M2M_OBJECT_INSTANCE_H

#include "m2mobjectinstance.h"

class Handler;
class Callback;

class Test_M2MObjectInstance
{
public:
    Test_M2MObjectInstance();

    void test_copy_constructor();

    virtual ~Test_M2MObjectInstance();

    void test_create_static_resource();

    void test_create_static_resource_instance();

    void test_create_dynamic_resource();

    void test_create_dynamic_resource_instance();

    void test_remove_resource();

    void test_remove_resource_instance();

    void test_resource();

    void test_resources();

    void test_resource_count();

    void test_total_resource_count();

    void test_base_type();

    void test_handle_get_request();

    void test_handle_put_request();

    void test_handle_post_request();

    void test_notification_update();

    M2MObjectInstance* object;

    Handler* handler;

    Callback* callback;
};

#endif // TEST_M2M_OBJECT_INSTANCE_H
