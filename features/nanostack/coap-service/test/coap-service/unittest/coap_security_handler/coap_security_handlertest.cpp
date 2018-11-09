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
#include "test_coap_security_handler.h"
#include "mbedtls_stub.h"
#include "nsdynmemLIB_stub.h"

TEST_GROUP(coap_security_handler)
{
    void setup() {
        nsdynmemlib_stub.returnCounter = 0;
        mbedtls_stub.useCounter = false;
    }

    void teardown() {
    }
};

TEST(coap_security_handler, test_thread_security_create)
{
    CHECK(test_thread_security_create());
}

TEST(coap_security_handler, test_thread_security_destroy)
{
    CHECK(test_thread_security_destroy());
}

TEST(coap_security_handler, test_coap_security_handler_connect)
{
    CHECK(test_coap_security_handler_connect());
}

TEST(coap_security_handler, test_coap_security_handler_continue_connecting)
{
    CHECK(test_coap_security_handler_continue_connecting());
}

TEST(coap_security_handler, test_coap_security_handler_send_message)
{
    CHECK(test_coap_security_handler_send_message());
}

TEST(coap_security_handler, test_thread_security_send_close_alert)
{
    CHECK(test_thread_security_send_close_alert());
}

TEST(coap_security_handler, test_coap_security_handler_read)
{
    CHECK(test_coap_security_handler_read());
}
