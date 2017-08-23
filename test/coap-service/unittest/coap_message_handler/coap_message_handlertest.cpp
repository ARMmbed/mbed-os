/*
 * Copyright (c) 2015-2017 ARM. All rights reserved.
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


