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
#include <inttypes.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
//#include "eventOS_event.h"
#include <ns_list.h>
#include <net_thread_test.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "MLE/mle.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_constants.h"
//#include "thread_tmf_data_if.h"
#include "thread_commissioning_if.h"
#include "shalib.h"
#include "Common_Protocols/icmpv6.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"

uint16_t thread_joiner_port = THREAD_DEFAULT_JOINER_PORT;

int8_t thread_node_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode)
{
    return 0;
}

void arm_thread_private_ula_prefix_set(protocol_interface_info_entry_t *cur, const uint8_t *ula_prefix)
{
}

int thread_tmf_service_commision_request(int8_t commissioning_handler_id, int8_t interfaceId, uint8_t *parentLLAddressPtr)
{
    return 0;
}

int thread_tmf_service_commision_relay_control(int8_t interfaceId, int8_t commissioning_handler_id, uint8_t *borderRouterLocator, bool enableService)
{
    return 0;
}

int thread_management_router_select_threshold_values_set(
    int8_t interface_id,
    uint8_t upGradeThreshold,
    uint8_t downGradeThreshold)
{
    return 0;
}

int thread_management_node_init(
    int8_t interface_id,
    channel_list_s *channel_list,
    device_configuration_s *device_configuration,
    link_configuration_s *static_configuration)
{
    return 0;
}

int thread_management_get_ml16_address(int8_t interface_id, uint8_t *address_ptr)
{
    return 0;
}

int thread_management_get_ml_prefix(int8_t interface_id, uint8_t *prefix_ptr)
{
    return 0;
}

int thread_management_get_ml_prefix_112(int8_t interface_id, uint8_t *prefix_ptr)
{
    return 0;
}

int thread_management_get_leader_address(int8_t interface_id, uint8_t *address_ptr)
{
    return 0;
}
int thread_management_get_leader_aloc(int8_t interface_id, uint8_t *address_buffer)
{
    return 0;
}

link_configuration_s *thread_management_configuration_get(int8_t interface_id)
{
    return NULL;
}

int thread_management_security_material_set(int8_t interface_id, bool enableSecurity, uint8_t *thrMasterKey, uint32_t thrKeySequenceCounter, uint32_t thrKeyRotation)
{
    return 0;
}

int thread_commision_data_handler(uint8_t *thrMasterKey, uint32_t thrKeySequenceCounter, uint32_t thrKeyRotation, uint8_t *meshLocalPrefix)
{
    return 0;
}

int thread_management_commission_data_set(int8_t interface_id)
{
    return 0;
}

int thread_management_joiner_attach_start(int8_t interface_id)
{
    return 0;
}

uint8_t *thread_management_key_request(int8_t nwk_interface_id, uint8_t keyId)
{
    return NULL;
}

void thread_management_key_synch_req(int8_t interface_id, uint32_t keySequnce)
{
}

void thread_history_key_material_push(thread_info_t *thread_info, uint8_t *mleKeyPtr, uint8_t keyId)
{
}

void thread_security_trig_pending_key(protocol_interface_info_entry_t *cur)
{
}

void thread_security_next_key_generate(protocol_interface_info_entry_t *cur, uint8_t *masterKey, uint32_t keySequence)
{
}

int thread_management_increment_key_sequence_counter(int8_t interface_id)
{
    return 0;
}

int thread_management_key_sequence_counter_update(int8_t interface_id, uint32_t thrKeySequenceCounter)
{
    return 0;
}

int thread_management_add_route(int8_t interface_id, uint8_t *route_prefix_ptr, uint8_t prefix_length, bool stable_service)
{
    return 0;
}

int thread_management_del_route(int8_t interface_id, uint8_t *route_prefix_ptr, uint8_t prefix_length)
{
    return 0;

}

int thread_management_add_dhcpv6_server(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length, bool stable_service)
{
    return 0;
}

int thread_management_del_dhcpv6_server(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length)
{
    return 0;
}

int thread_management_add_contexts(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length, uint8_t cid, bool compress_supported)
{
    return 0;
}

int thread_management_read_leader_ula(int8_t interface_id, uint8_t *address_buffer)
{
    return 0;
}

int thread_management_network_data_tlv_push(protocol_interface_info_entry_t *cur, uint8_t *data_pointer, uint16_t length)
{
    return 0;
}

int thread_management_network_data_analyze_trig(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int thread_management_network_data_register(int8_t interface_id, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t routerId)
{
    return 0;
}

int thread_management_remove_router_by_id(int8_t interface_id, uint8_t routerId)
{
    return 0;
}

int thread_management_print_network_data(int8_t interface_id)
{
    return 0;
}

int thread_on_mesh_slaac_server_add(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length, bool stableData)
{
    return 0;
}

int thread_on_mesh_slaac_delete(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_length)
{
    return 0;
}

int thread_dhcpv6_server_add(int8_t interface_id, uint8_t *prefix_ptr, uint32_t max_client_cnt, bool stableData)
{
    return 0;
}

int thread_dhcpv6_server_set_lifetime(int8_t interface_id, uint8_t *prefix_ptr, uint32_t valid_lifetime)
{
    return 0;
}

int thread_dhcpv6_server_set_max_client(int8_t interface_id, uint8_t *prefix_ptr, uint32_t max_client_count)
{
    return 0;
}

int thread_dhcpv6_server_set_anonymous_addressing(int8_t interface_id, uint8_t *prefix_ptr, bool anonymous)
{
    return 0;
}

int thread_dhcpv6_server_delete(int8_t interface_id, uint8_t *prefix_ptr)
{
    return 0;
}

uint16_t thread_management_get_my_sht_addr(int8_t interface_id)
{
    return 0;
}

int thread_management_get_my_iid16(int8_t interface_id, uint8_t *iidPtr)
{
    return 0;
}

int thread_management_get_my_ml_prefix(int8_t interface_id, uint8_t *prefix)
{
    return 0;
}

/* Get my mesh local prefix /112 */
int thread_management_get_my_ml_prefix_112(int8_t interface_id, uint8_t *prefix)
{
    return 0;
}

int thread_management_get_my_ml16(int8_t interface_id, uint8_t *addressPtr)
{
    return 0;
}


int thread_management_get_current_keysequence(int8_t interface_id, uint32_t *sequencePtr)
{
    return 0;
}

int thread_management_native_commissioner_get_connection_info(int8_t interface_id, uint8_t *address_ptr, uint16_t *port, uint8_t *PSKc_ptr)
{
    return 0;
}

int thread_management_get_commissioner_address(int8_t interface_id, uint8_t *address_ptr, uint16_t *port_ptr)
{
    return 0;
}

void thread_key_get(uint8_t *key, uint8_t *key_material_buf,  uint32_t key_sequence_counter)
{

}

int thread_management_key_sets_calc(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, uint32_t thrKeySequenceCounter)
{
    return 0;
}

void thread_security_update_from_mac(protocol_interface_info_entry_t *cur)
{

}
int thread_management_get_parent_address(int8_t interface_id, uint8_t *address_ptr)
{
    return 0;
}
int thread_management_device_type_set(int8_t interface_id, thread_device_type_e device_type)
{
    return 0;
}
