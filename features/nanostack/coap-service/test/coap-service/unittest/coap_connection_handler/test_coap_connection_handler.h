/*
 * Copyright (c) 2015-2018, Pelion and affiliates.
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

bool test_coap_connection_handler_msg_prevalidate_cb_read_and_set();

bool test_coap_connection_handler_find_by_socket_port();

#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_CONNECTION_HANDLER_H

