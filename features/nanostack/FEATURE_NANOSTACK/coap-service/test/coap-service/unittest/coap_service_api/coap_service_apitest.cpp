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
#include "test_coap_service_api.h"

TEST_GROUP(coap_service_api)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(coap_service_api, test_coap_service_initialize)
{
    CHECK(test_coap_service_initialize());
}

TEST(coap_service_api, test_coap_service_delete)
{
    CHECK(test_coap_service_delete());
}

TEST(coap_service_api, test_coap_service_virtual_socket_recv)
{
    CHECK(test_coap_service_virtual_socket_recv());
}

TEST(coap_service_api, test_coap_service_virtual_socket_set_cb)
{
    CHECK(test_coap_service_virtual_socket_set_cb());
}

TEST(coap_service_api, test_coap_service_register_uri)
{
    CHECK(test_coap_service_register_uri());
}

TEST(coap_service_api, test_coap_service_unregister_uri)
{
    CHECK(test_coap_service_unregister_uri());
}

TEST(coap_service_api, test_coap_service_request_send)
{
    CHECK(test_coap_service_request_send());
}

TEST(coap_service_api, test_coap_service_request_delete)
{
    CHECK(test_coap_service_request_delete());
}

TEST(coap_service_api, test_coap_service_response_send)
{
    CHECK(test_coap_service_response_send());
}

TEST(coap_service_api, test_coap_callbacks)
{
    CHECK(test_coap_callbacks());
}

TEST(coap_service_api, test_eventOS_callbacks)
{
    CHECK(test_eventOS_callbacks());
}

TEST(coap_service_api, test_conn_handler_callbacks)
{
    CHECK(test_conn_handler_callbacks());
}

TEST(coap_service_api, test_certificate_set)
{
    CHECK(test_certificate_set());
}

TEST(coap_service_api, test_handshake_timeout_set)
{
    CHECK(test_handshake_timeout_set());
}

TEST(coap_service_api, test_coap_duplcate_msg_buffer_set)
{
    CHECK(test_coap_duplcate_msg_buffer_set());
}

TEST(coap_service_api, test_coap_service_get_internal_timer_ticks)
{
    CHECK(test_coap_service_get_internal_timer_ticks())
}

TEST(coap_service_api, test_coap_service_if_find_by_socket)
{
    CHECK(test_coap_service_if_find_by_socket())
}

TEST(coap_service_api, test_coap_service_handshake_limit_set)
{
    CHECK(test_coap_service_handshake_limit_set())
}
