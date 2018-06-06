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
#include "test_coap_message_handler.h"

TEST_GROUP(coap_message_handler)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(coap_message_handler, test_coap_message_handler_init)
{
    CHECK(test_coap_message_handler_init());
}

TEST(coap_message_handler, test_coap_message_handler_destroy)
{
    CHECK(test_coap_message_handler_destroy());
}

TEST(coap_message_handler, test_coap_message_handler_find_transaction)
{
    CHECK(test_coap_message_handler_find_transaction());
}

TEST(coap_message_handler, test_coap_message_handler_coap_msg_process)
{
    CHECK(test_coap_message_handler_coap_msg_process());
}

TEST(coap_message_handler, test_coap_message_handler_request_send)
{
    CHECK(test_coap_message_handler_request_send());
}

TEST(coap_message_handler, test_coap_message_handler_response_send)
{
    CHECK(test_coap_message_handler_response_send());
}

TEST(coap_message_handler, test_coap_message_handler_request_delete)
{
    CHECK(test_coap_message_handler_request_delete());
}

TEST(coap_message_handler, test_coap_message_handler_exec)
{
    CHECK(test_coap_message_handler_exec());
}


