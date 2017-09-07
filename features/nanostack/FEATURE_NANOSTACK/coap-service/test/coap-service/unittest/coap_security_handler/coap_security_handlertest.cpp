/*
 * Copyright (c) 2015-2017 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_coap_security_handler.h"
#include "mbedtls_stub.h"
#include "nsdynmemLIB_stub.h"

TEST_GROUP(coap_security_handler)
{
    void setup()
    {
        nsdynmemlib_stub.returnCounter = 0;
        mbedtls_stub.useCounter = false;
    }

    void teardown()
    {
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
