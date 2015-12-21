/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_COAP_CONNECTION_HANDLER_H
#define TEST_COAP_CONNECTION_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_connection_handler_create();

bool test_connection_handler_destroy();

bool test_coap_connection_handler_open_connection();

bool test_coap_connection_handler_send_data();

bool test_coap_connection_handler_virtual_recv();

bool test_coap_connection_handler_socket_belongs_to();

bool test_timer_callbacks();

bool test_socket_api_callbacks();

bool test_security_callbacks();

#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_CONNECTION_HANDLER_H

