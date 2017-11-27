/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "thread_tmfcop_lib.h"

#include "thread_config.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_management_client.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_MANAGEMENT_CLIENT

typedef struct thread_management {
    thread_management_client_router_id_cb *router_id_cb_ptr;
    thread_management_client_network_data_set_cb *network_data_set_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} thread_management_t;


void thread_management_client_init(int8_t interface_id)
{
}

void thread_management_client_delete(int8_t interface_id)
{
}

int thread_management_client_router_id_get(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb, uint8_t cause)
{
    return 0;
}

int thread_management_client_router_id_release(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb)
{
    return 0;
}

int thread_management_client_network_data_register(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len, thread_management_client_network_data_set_cb *set_cb)
{
    return 0;
}

int8_t thread_management_query_network_parameters(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}

int8_t thread_management_send_set_network_parameters(int8_t interface_id)
{
    return 0;
}

int8_t thread_management_client_pending_get(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}

int8_t thread_management_client_active_get(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}

int8_t thread_management_client_active_set(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}
int8_t thread_management_client_pending_set(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}
int thread_management_client_network_data_unregister(int8_t interface_id, uint16_t rloc16)
{
    return 0;
}
int thread_management_client_neighbor_discovery_data_request(int8_t interface_id, const uint8_t destination[16],  const uint8_t *options, uint8_t options_len, thread_management_client_network_data_set_cb *set_cb)
{
    return 0;
}
int thread_management_client_provision_request(int8_t interface_id, uint8_t *dst_address, uint16_t port)
{
    return 0;
}
void thread_management_client_pending_coap_request_kill(int8_t interface_id)
{
}

void thread_management_client_proactive_an(int8_t interface_id, const uint8_t address[16], const uint16_t rloc, const uint8_t ml_eid[8], const uint8_t dst_addr[16])
{
}
