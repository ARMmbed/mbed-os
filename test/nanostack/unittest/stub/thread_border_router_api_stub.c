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

#include <string.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include <ns_list.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "thread_border_router_api.h"
#include "thread_management_if.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "MLE/mle.h"

#define TRACE_GROUP "tBRa"

int thread_border_router_publish(int8_t interface_id)
{
    return 0;
}

int thread_border_router_proxy_state_update(int8_t caller_interface_id , int8_t handler_interface_id, bool status)
{
    return 0;
}

int8_t thread_border_router_init(int8_t interface_id)
{
    return 0;
}

void thread_border_router_delete(int8_t interface_id)
{
}

int thread_border_router_recursive_dns_server_option_set(int8_t interface_id, uint8_t *recursive_dns_server_option, uint16_t recursive_dns_server_option_len)
{
    return 0;
}

int thread_border_router_dns_search_list_option_set(int8_t interface_id, uint8_t *dns_search_list_option, uint16_t search_list_option_len)
{
    return 0;
}

int thread_border_router_prefix_context_id(uint8_t *prefix_tlv, uint16_t prefix_tlv_length)
{
    return 0;
}
