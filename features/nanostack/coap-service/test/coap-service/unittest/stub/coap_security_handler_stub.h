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
#ifndef __COAP_SECURITY_HANDLER_STUB_H__
#define __COAP_SECURITY_HANDLER_STUB_H__

#include <inttypes.h>
#include "coap_security_handler.h"

typedef struct tsh{
    coap_security_t *sec_obj;
    int int_value;
    int counter;
    int values[10];

    int (*send_cb)(int8_t socket_id, const uint8_t *address_ptr, uint16_t port, const void *, size_t);
    int (*receive_cb)(int8_t socket_id, unsigned char *, size_t);
    void (*start_timer_cb)(int8_t timer_id, uint32_t min, uint32_t fin);
    int (*timer_status_cb)(int8_t timer_id);
} thread_sec_def;

extern thread_sec_def coap_security_handler_stub;

coap_security_t *coap_security_handler_stub_alloc(void);

#endif //__COAP_SECURITY_HANDLER_STUB_H__
