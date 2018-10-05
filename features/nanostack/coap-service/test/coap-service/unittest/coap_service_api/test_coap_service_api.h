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

bool test_coap_service_request_delete();

bool test_coap_service_response_send();

bool test_coap_callbacks();

bool test_eventOS_callbacks();

bool test_conn_handler_callbacks();

bool test_certificate_set();

bool test_handshake_timeout_set();

bool test_coap_duplcate_msg_buffer_set();

bool test_coap_service_get_internal_timer_ticks();

bool test_coap_service_if_find_by_socket();

bool test_coap_service_handshake_limit_set();


#ifdef __cplusplus
}
#endif

#endif // TEST_COAP_SERVICE_API_H

