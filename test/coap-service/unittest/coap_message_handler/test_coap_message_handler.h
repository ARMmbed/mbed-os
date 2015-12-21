/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_COAP_MESSAGE_HANDLER_H
#define TEST_COAP_MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_coap_message_handler_init();
bool test_coap_message_handler_destroy();
bool test_coap_message_handler_find_transaction();
bool test_coap_message_handler_coap_msg_process();
bool test_coap_message_handler_request_send();
bool test_coap_message_handler_response_send();
bool test_coap_message_handler_exec();

#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_MESSAGE_HANDLER_H

