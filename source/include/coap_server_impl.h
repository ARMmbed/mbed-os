/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.

 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * \file -
 * \brief Platform specifig implementation file
 * 
 */

#ifndef COAP_SERVER_IMPL_H_
#define COAP_SERVER_IMPL_H_

#include "ns_types.h"
#include "eventOS_event.h"
#include "coap_service_api.h"

int8_t coap_server_start(void);

void *memory_allocation(uint16_t size);

void memory_free(void* ptr);

static int8_t coap_server_service_tasklet_generated(void);

void coap_server_service_tasklet(arm_event_s * event);

uint16_t socket_open(uint16_t listen_port, coap_service_request_recv_cb *requst_recv_cb);

#endif /* COAP_SERVER_IMPL_H_ */
