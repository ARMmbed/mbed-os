/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"

#define TRACE_GROUP "TMFs"

#include "net_interface.h"
#include "socket_api.h"
#include "thread_config.h"
#include "thread_common.h"
#include "thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "thread_commissioning_if.h"
#include "thread_leader_service.h"


int thread_leader_service_init(int8_t interface_id, uint8_t coap_service_id)
{
    return 0;
}

void thread_leader_service_delete(int8_t interface_id)
{
}
int thread_leader_service_start(int8_t interface_id)
{
    return 0;
}

void thread_leader_service_stop(int8_t interface_id)
{

}

void thread_leader_service_leader_data_free(thread_info_t *thread_info)
{
}
void thread_leader_service_generate_thread_partitition(int8_t interface_id, bool newPartition)
{
    (void)interface_id;
    (void)newPartition;
}
void thread_leader_service_update_id_set(protocol_interface_info_entry_t *cur)
{
}
void thread_leader_service_router_id_valid_lifetime_update(protocol_interface_info_entry_t *cur, uint32_t tickUpdate)
{
}
void thread_leader_service_generate_network_data(protocol_interface_info_entry_t *cur)
{
}
uint16_t thread_leader_service_child_id_from_networkdata_get(struct thread_info_s *thread_info, uint16_t router_short_addr)
{
    return 0;
}
void thread_leader_service_thread_partitition_generate(int8_t interface_id, bool newPartition)
{
}
