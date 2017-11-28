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

/*
 * \file thread_network_data_storage.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "ip6string.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"

#define TRACE_GROUP "thrd"

void thread_nd_network_data_print(thread_network_data_cache_entry_t *networkData, uint16_t routerId)
{
}

uint8_t thread_nd_server_external_route_list_size(thread_network_server_data_list_t *list, bool stable)
{
    return 0;
}

uint8_t thread_nd_border_routerserver_list_size(thread_network_server_data_list_t *list, bool stable)
{
    return 0;
}

uint8_t thread_nd_context_list_size(thread_data_context_list_t *list, bool stable)
{
    return 0;
}

uint8_t *thread_nd_context_list_write(thread_data_context_list_t *list, uint8_t *dataPtr, bool stableData)
{
    return NULL;
}

uint8_t *thread_nd_server_list_write(thread_network_server_data_list_t *list, uint8_t *dataPtr, uint8_t type, bool stable)
{
    return NULL;
}

uint8_t thread_nd_prefix_based_on_list_entry_size(thread_network_data_prefix_cache_entry_t *entry, bool fullList)
{
    return 0;
}

int thread_nd_local_data_length_updated(thread_network_data_cache_entry_t *networkDataList)
{
    return 0;
}


thread_network_local_data_entry_t *thread_local_service_list_allocate(thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_data_prefix_cache_entry_t *thread_prefix_service_list_allocate(thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_server_data_entry_t *thread_server_list_allocate(thread_border_router_tlv_entry_t *service)
{
    return NULL;
}

thread_network_data_context_entry_t *thread_context_service_list_allocate(uint8_t contextLength, uint8_t cid, bool compersioEnabled)
{
    return NULL;
}

int  thread_nd_verify_contex_id_is_free(thread_network_data_cache_entry_t *list, uint8_t *prefixPtr, thread_network_local_data_context_entry_t *context)
{
    return 0;
}

thread_network_local_data_entry_t *thread_get_local_prefix_entry(thread_network_data_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_local_data_entry_t *thread_get_local_prefix_list(thread_network_data_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_data_prefix_cache_entry_t *thread_get_main_prefix_entry(thread_network_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_data_prefix_cache_entry_t *thread_get_main_prefix_list(thread_network_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    return NULL;
}

thread_network_server_data_entry_t *thread_search_server_list(thread_network_server_data_list_t *list, uint8_t routerId)
{
    return NULL;
}

thread_network_server_data_entry_t *thread_get_server_list(thread_network_server_data_list_t *list, thread_border_router_tlv_entry_t *service)
{
    return NULL;
}


uint8_t thread_get_context_id_by_length(thread_data_context_list_t *list, uint8_t contextLength)
{
    return 0;
}

thread_network_data_context_entry_t *thread_get_context_by_id(thread_data_context_list_t *list, uint8_t cid)
{
    return NULL;
}

thread_network_data_context_entry_t *thread_get_main_context_list(thread_data_context_list_t *list, thread_network_local_data_context_entry_t *context)
{
    return NULL;
}

void thread_server_list_free(thread_network_server_data_list_t *listPtr)
{
}

int thread_server_context_delete_mark(thread_data_context_list_t *listPtr)
{
    return 0;
}

int thread_server_data_delete_mark_by_router_id(thread_network_server_data_list_t *listPtr, uint16_t routerID, bool subSet)
{
    return 0;
}

int thread_server_context_clean(thread_network_data_cache_entry_t *cachePtr, thread_data_context_list_t *listPtr, thread_network_data_prefix_cache_entry_t *prefixEntry)
{
    return 0;
}


bool thread_server_data_clean_by_router_id(thread_network_data_cache_entry_t *cachePtr, thread_network_server_data_list_t *listPtr, thread_network_data_prefix_cache_entry_t *prefixEntry, bool routeInfo, protocol_interface_info_entry_t *curInterface)
{
    return false;
}

void thread_context_service_list_free(thread_data_context_list_t *listPtr)
{
}
void thread_network_data_base_init(thread_network_data_cache_entry_t *cachePtr)
{
}

void thread_network_local_server_data_base_init(thread_network_local_data_cache_entry_t *cachePtr)
{
}

void thread_network_data_router_id_mark_delete(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID, bool subSet)
{
}

bool thread_network_data_router_id_free(thread_network_data_cache_entry_t *cachePtr, bool subSet, protocol_interface_info_entry_t *curInterface)
{
    return false;
}

void thread_network_data_context_re_use_timer_update(thread_network_data_cache_entry_t *cachePtr, uint32_t ticks, lowpan_context_list_t *context_list)
{

}

void thread_network_data_free_and_clean(thread_network_data_cache_entry_t *cachePtr)
{
}

void thread_network_local_data_free_and_clean(thread_network_local_data_cache_entry_t *cachePtr, int8_t interface_id)
{
}

int thread_nd_local_list_add_route(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *route)
{
    return 0;
}

int thread_nd_local_list_add_on_mesh_prefix(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    return 0;
}

int thread_nd_local_list_del_on_mesh_server(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    return 0;
}

int thread_local_server_list_add_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    return 0;
}

int thread_local_server_list_del_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv)
{
    return 0;
}

int thread_local_server_add_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *route)
{
    return 0;
}

int thread_local_server_del_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv)
{
    return 0;
}

uint8_t thread_nd_context_id_allocate(thread_network_data_cache_entry_t *networkDataList, thread_network_local_data_cache_entry_t *localDataList, uint8_t *prefixPtr, uint8_t prefixLength)
{
    return 0;
}

int thread_nd_local_list_add_contexts(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_network_local_data_context_entry_t *context)
{
    return 0;
}

uint8_t thread_server_prefix_length(thread_network_local_data_entry_t *cur)
{
    return 0;
}

uint16_t thread_nd_own_service_list_data_size(thread_network_local_data_cache_entry_t *serverDataList)
{
    return 0;
}

uint8_t * thread_nd_own_service_list_data_write(thread_network_local_data_cache_entry_t *serverDataList, uint8_t *ptr, uint16_t routerID)
{
    return NULL;
}

uint16_t thread_nd_data_size(thread_network_data_cache_entry_t *networkDataList, bool fullList)
{
    return 0;
}

int thread_nd_commission_data_len_update(thread_network_data_cache_entry_t *networkDataList, link_configuration_s *linkConfiguration, thread_commissioner_t *registered_commissioner)
{
    return 0;
}

uint8_t thread_nd_commission_data_length_size(link_configuration_s *linkConfiguration, thread_commissioner_t *registered_commissioner)
{
    return 0;
}

uint8_t *thread_nd_commission_data_write(uint8_t *ptr, link_configuration_s *linkConfiguration, thread_commissioner_t *registered_commissioner)
{
    return NULL;
}

bool thread_check_network_data_prefix_stable_boolean(thread_network_data_prefix_cache_entry_t *dataList)
{
    return false;
}

uint8_t *thread_nd_generate_from_local_data_list(thread_network_data_cache_entry_t *networkDataList, link_configuration_s *linkConfiguration, thread_commissioner_t *registered_commissioner, uint8_t *ptr, bool fullList)
{
    return NULL;
}

uint8_t *thread_nd_generate_from_local_data_list_to_sleepy_child(thread_network_data_cache_entry_t *networkDataList, struct link_configuration *linkConfiguration, struct thread_commissioner *registered_commissioner, uint8_t *ptr, bool fullList)
{
    return NULL;
}

uint16_t thread_network_data_prefix_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList)
{
    return 0;
}
thread_network_data_service_cache_entry_t *thread_network_data_service_entry_find(thread_network_data_service_cache_list_t *list, thread_network_data_service_entry_t *service)
{
    return NULL;
}
uint16_t thread_network_data_service_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList)
{
    return 0;
}
uint8_t *thread_network_data_prefix_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr)
{
    return NULL;
}
uint8_t *thread_network_data_service_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr)
{
    return NULL;
}
bool thread_network_data_service_hosted_by_this_router_id(thread_network_data_service_cache_entry_t *dataList, uint16_t router_id)
{
    return false;
}
bool thread_nd_on_mesh_address_valid(thread_network_server_data_entry_t *curRoute)
{
    return true;
}

thread_network_server_data_entry_t *thread_nd_hosted_by_this_routerid(uint16_t routerId, thread_network_server_data_list_t *list)
{
    return NULL;
}
bool thread_network_data_services_registered(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID)
{
    return true;
}

uint16_t thread_network_data_service_child_id_from_networkdata_get(thread_network_data_cache_entry_t *networkDataList, uint16_t router_short_addr)
{
    return 0;
}
int thread_nd_local_list_add_service(thread_network_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service, thread_network_data_service_server_entry_t *server)
{
    return 0;
}

