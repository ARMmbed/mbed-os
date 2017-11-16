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
/**
 * \file net.c
 * \brief Network API for library model
 *
 * The network API functions for library model
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_scheduler.h"
#include "string.h"
#include "ns_trace.h"
#include "socket_api.h"
#include "socket.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ccmLIB.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "6LoWPAN/ND/nd_router_object.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif

#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "net_pana_parameters_api.h"
#ifdef PANA
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#endif
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Security/Common/sec_lib_definitions.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "net_thread_test.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "BorderRouter/border_router.h"


int8_t arm_net_energy_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t* conf), uint8_t energy_tresshold)
{
    return 0;
}

int8_t arm_net_nwk_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t* conf), uint8_t scan_level)
{
    return 0;
}

nwk_pan_descriptor_t *arm_net_get_scanned_nwk_list(int8_t interface_id)
{
    return NULL;
}

uint16_t arm_net_get_nwk_pan_id_filter(int8_t interface_id)
{
    return 0;
}

int8_t arm_nwk_param_read(int8_t interface_id, link_layer_setups_s *network_params)
{
    return 0;
}

int8_t arm_nwk_mac_address_read(int8_t interface_id, link_layer_address_s *mac_params)
{
    return 0;
}

int8_t arm_nwk_nd_address_read(int8_t interface_id, network_layer_address_s *nd_addr_info)
{
    return 0;
}

int16_t arm_net_get_current_channel(int8_t interface_id)
{
    return 0;
}

void arm_net_host_enter_sleep_state_set(int8_t interface_id, uint8_t state)
{
}


void net_get_version_information(uint8_t *ptr)
{
}

int8_t arm_nwk_6lowpan_gp_address_mode(int8_t interface_id, net_6lowpan_gp_address_mode_e mode, uint16_t short_address_base, uint8_t define_new_short_address_at_DAD)
{
    return 0;
}

int8_t arm_net_address_get(int8_t interface_id, net_address_t addr_id, uint8_t *address)
{
    return 0;
}

int8_t arm_net_interface_address_list_size(int8_t interface_id, uint16_t *address_count)
{
    return 0;
}

int8_t arm_net_address_list_get(int8_t interface_id, uint8_t address_buf_size, uint8_t *address_buffer, int *writed_address_count)
{
    return 0;
}

#ifdef HAVE_ETHERNET
#include "ethernet_mac_api.h"
int8_t arm_nwk_interface_ethernet_init(eth_mac_api_t *api, const char *interface_name_ptr)
{
    return 0;
}
#endif

int8_t arm_nwk_interface_network_driver_set(int8_t interface_id, const channel_list_s *nwk_channel_list, network_driver_setup_s *link_setup)
{
    return 0;
}

int8_t arm_nwk_interface_up(int8_t interface_id)
{
    return 0;
}

int8_t arm_nwk_interface_down(int8_t interface_id)
{
    return 0;
}

int8_t arm_nwk_link_layer_security_mode(int8_t interface_id, net_6lowpan_link_layer_sec_mode_e  mode, uint8_t sec_level, const net_link_layer_psk_security_info_s *psk_key_info)
{
    return 0;
}

int8_t arm_network_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info)
{
    return 0;
}

int8_t arm_network_key_get(int8_t interface_id, ns_keys_t *key)
{
    return 0;
}

int8_t arm_pana_server_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key)
{
    return 0;
}

int8_t arm_pana_activate_new_key(int8_t interface_id)
{
    return 0;
}

int8_t arm_pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material)
{
    return 0;
}

int8_t net_pana_parameter_set(const pana_lib_parameters_s *parameter_ptr)
{
    return 0;
}

int8_t net_pana_parameter_read(pana_lib_parameters_s *parameter_ptr)
{
    return 0;
}


int8_t arm_pana_client_library_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id)
{
    return 0;
}


int8_t arm_nwk_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer)
{
    return 0;
}

int8_t arm_nwk_interface_configure_6lowpan_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension)
{
    return 0;
}


int8_t arm_nwk_6lowpan_link_scan_parameter_set(int8_t interface_id, uint8_t scan_time)
{
    return 0;
}

int8_t arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(int8_t interface_id, uint16_t pan_id_filter)
{
    return 0;
}

int8_t arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(int8_t interface_id, const uint8_t *nwk_id_filter)
{
    return 0;
}

int8_t arm_nwk_6lowpan_link_protocol_id_filter_for_nwk_scan(int8_t interface_id, uint8_t protocol_ID)
{
    return 0;
}

int8_t net_init_core(void)
{
    return 0;
}

int8_t arm_nwk_host_mode_set(int8_t interface_id, net_host_mode_t mode, uint32_t poll_time)
{
    return 0;
}

int8_t arm_nwk_host_mode_get(int8_t interface_id, net_host_mode_t *mode)
{
    return 0;
}

int8_t net_nvm_data_clean(int8_t interface_id)
{
    return 0;
}

void arm_print_routing_table(void)
{
}

void arm_ncache_flush(void)
{
}
