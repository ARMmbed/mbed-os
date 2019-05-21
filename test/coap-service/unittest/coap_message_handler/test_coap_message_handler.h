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
bool test_coap_message_handler_request_delete();
bool test_coap_message_handler_request_delete_by_service_id();
bool test_coap_message_handler_exec();

#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_MESSAGE_HANDLER_H

