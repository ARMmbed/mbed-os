/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "test_coap_connection_handler.h"

TEST_GROUP(coap_connection_handler)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(coap_connection_handler, test_connection_handler_create)
{
    CHECK(test_connection_handler_create());
}

TEST(coap_connection_handler, test_connection_handler_destroy)
{
    CHECK(test_connection_handler_destroy());
}

TEST(coap_connection_handler, test_coap_connection_handler_open_connection)
{
    CHECK(test_coap_connection_handler_open_connection());
}

TEST(coap_connection_handler, test_coap_connection_handler_send_data)
{
    CHECK(test_coap_connection_handler_send_data());
}

TEST(coap_connection_handler, test_coap_connection_handler_virtual_recv)
{
    CHECK(test_coap_connection_handler_virtual_recv());
}

TEST(coap_connection_handler, test_coap_connection_handler_socket_belongs_to)
{
    CHECK(test_coap_connection_handler_socket_belongs_to());
}

TEST(coap_connection_handler, test_timer_callbacks)
{
    CHECK(test_timer_callbacks());
}

TEST(coap_connection_handler, test_socket_api_callbacks)
{
    CHECK(test_socket_api_callbacks());
}

TEST(coap_connection_handler, test_security_callbacks)
{
    CHECK(test_security_callbacks());
}

