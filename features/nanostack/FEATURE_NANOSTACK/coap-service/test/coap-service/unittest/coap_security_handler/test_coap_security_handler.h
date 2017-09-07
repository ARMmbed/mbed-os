/*
 * Copyright (c) 2015-2017 ARM Limited. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

