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
#ifndef __COAP_MESSAGE_HANDLER_STUB_H__
#define __COAP_MESSAGE_HANDLER_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
#include "coap_message_handler.h"

typedef struct {
    int8_t int8_value;
    int16_t int16_value;
    uint16_t uint16_value;
    coap_msg_handler_t *coap_ptr;
    coap_transaction_t *coap_tx_ptr;
    int16_t (*cb)(int8_t, sn_coap_hdr_s *, coap_transaction_t *);
} coap_message_handler_stub_def;

extern coap_message_handler_stub_def coap_message_handler_stub;

#ifdef __cplusplus
}
#endif

#endif // __COAP_MESSAGE_HANDLER_STUB_H__
