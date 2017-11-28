/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CppUTest/TestHarness.h"
#include "test_thread_commissioning_api.h"

TEST_GROUP(thread_commissioning_api)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};



TEST(thread_commissioning_api, test_commissioning_device_add_and_remove)
{
    CHECK(test_commissioning_device_add_and_remove());
}

TEST(thread_commissioning_api, test_commissioning_device_add)
{
    CHECK(test_commissioning_device_add());
} 

TEST(thread_commissioning_api, test_commissioning_device_delete)
{
    CHECK(test_commissioning_device_delete());
} 

TEST(thread_commissioning_api, test_commissioning_register_twice)
 {
     CHECK(test_commissioning_register_twice());
}

TEST(thread_commissioning_api, test_thread_commissioning_petition_start)
{
    CHECK(test_thread_commissioning_petition_start());
}
TEST(thread_commissioning_api, test_thread_commissioning_petition_keep_alive)
{
    CHECK(test_thread_commissioning_petition_keep_alive());
}

TEST(thread_commissioning_api, test_thread_commissioning_get_management_id)
{
    CHECK(test_thread_commissioning_get_management_id());
}

TEST(thread_commissioning_api, test_thread_commissioning_application_provision_cb)
{
    CHECK(test_thread_commissioning_application_provision_cb());
}

TEST(thread_commissioning_api, test_thread_commissioning_finalisation_req_recv_cb)
{
    CHECK(test_thread_commissioning_finalisation_req_recv_cb());
}

TEST(thread_commissioning_api, test_thread_commissioning_rx_receive_cb)
{
    CHECK(test_thread_commissioning_rx_receive_cb());
}
TEST(thread_commissioning_api, test_thread_commissioning_native_commissioner_start)
{
    CHECK(test_thread_commissioning_native_commissioner_start());
}
TEST(thread_commissioning_api, test_thread_commissioning_native_commissioner_connect)
{
    CHECK(test_thread_commissioning_native_commissioner_connect());
}





