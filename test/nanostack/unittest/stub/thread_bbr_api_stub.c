/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include <ns_list.h>
#include "ns_trace.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "common_functions.h"
#include "thread_border_router_api.h"
#include "thread_bbr_api.h"
#include "Common_Protocols/ipv6_constants.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_constants.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_mdns.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "coap_service_api.h"
#include "thread_management_server.h"
#include "socket_api.h"
#include "coap_service_api.h"

bool thread_bbr_routing_enabled(protocol_interface_info_entry_t *cur)
{
    return true;
}

void thread_bbr_network_data_update_notify(protocol_interface_info_entry_t *cur)
{
}


int thread_bbr_commissioner_proxy_service_update(int8_t interface_id)
{
    return -1;
}
int8_t thread_bbr_init(int8_t interface_id, uint16_t external_commisssioner_port)
{
    return -1;
}

void thread_bbr_delete(int8_t interface_id)
{
}

void thread_bbr_seconds_timer(int8_t interface_id, uint32_t seconds)
{
}

int thread_bbr_proxy_state_update(int8_t caller_interface_id , int8_t handler_interface_id, bool status)
{
    return -1;
}
