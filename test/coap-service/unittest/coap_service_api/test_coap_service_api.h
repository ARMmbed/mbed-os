/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_COAP_SERVICE_API_H
#define TEST_COAP_SERVICE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_coap_service_initialize();

bool test_coap_service_delete();

bool test_coap_service_virtual_socket_recv();

bool test_coap_service_virtual_socket_set_cb();

bool test_coap_service_register_uri();

bool test_coap_service_unregister_uri();

bool test_coap_service_request_send();

bool test_coap_service_response_send();

bool test_coap_callbacks();

bool test_eventOS_callbacks();

bool test_conn_handler_callbacks();


#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_SERVICE_API_H

