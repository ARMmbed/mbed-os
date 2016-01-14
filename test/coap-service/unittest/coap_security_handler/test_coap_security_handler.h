/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_COAP_SECURITY_HANDLER_H
#define TEST_COAP_SECURITY_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>

bool test_thread_security_create();

bool test_thread_security_destroy();

bool test_coap_security_handler_connect();

bool test_coap_security_handler_continue_connecting();

bool test_coap_security_handler_send_message();

bool test_thread_security_send_close_alert();

bool test_coap_security_handler_read();

#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_SECURITY_HANDLER_H

