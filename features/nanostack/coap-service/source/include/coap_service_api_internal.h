/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#ifndef __COAP_SERVICE_API_INTERNAL_H__
#define __COAP_SERVICE_API_INTERNAL_H__

#include "coap_message_handler.h"


extern coap_msg_handler_t *coap_service_handle;

uint32_t coap_service_get_internal_timer_ticks(void);

uint16_t coap_service_id_find_by_socket(int8_t socket_id);

#endif
