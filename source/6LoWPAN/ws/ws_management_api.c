/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include <string.h>
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"

#include "ws_management_api.h"

#define TRACE_GROUP "wsmg"

#ifdef HAVE_WS

int ws_management_node_init(
    int8_t interface_id,
    uint8_t regulatory_domain,
    char *network_name_ptr,
    fhss_timer_t *fhss_timer_ptr)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (!network_name_ptr || !fhss_timer_ptr) {
        return -2;
    }
    cur->ws_info->hopping_schdule.regulatory_domain = regulatory_domain;
    if (ws_common_regulatory_domain_config(cur) < 0) {
        // Invalid regulatory domain set
        return -3;
    }
    strncpy(cur->ws_info->network_name, network_name_ptr, 32);
    cur->ws_info->fhss_timer_ptr = fhss_timer_ptr;
    return 0;
}

int ws_management_network_name_set(
    int8_t interface_id,
    char *network_name_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (!network_name_ptr || strlen(network_name_ptr) == 0 || strlen(network_name_ptr) > 32) {
        return -2;
    }
    if (strcmp(cur->ws_info->network_name, network_name_ptr) == 0) {
        // Network name is the same no further actions required.
        return 0;
    }
    strncpy(cur->ws_info->network_name, network_name_ptr, 32);
    // if settings change reset_restart for the settings needed
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        ws_bootstrap_restart(interface_id);
    }

    return 0;
}
int ws_management_regulatory_domain_set(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t operating_class,
    uint8_t operating_mode)
{
    uint8_t regulatory_domain_saved;
    uint8_t operating_class_saved;
    uint8_t operating_mode_saved;
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    regulatory_domain_saved = cur->ws_info->hopping_schdule.regulatory_domain;
    operating_class_saved = cur->ws_info->hopping_schdule.operating_mode;
    operating_mode_saved = cur->ws_info->hopping_schdule.operating_class;
    if (regulatory_domain != 255) {
        cur->ws_info->hopping_schdule.regulatory_domain = regulatory_domain;
    }
    if (operating_mode != 255) {
        cur->ws_info->hopping_schdule.operating_mode = operating_mode;
    }
    if (operating_class != 255) {
        cur->ws_info->hopping_schdule.operating_class = operating_class;
    }
    if (ws_common_regulatory_domain_config(cur) != 0) {
        // Restore old config on failure
        //tr_error("unsupported regulatory domain: %d class: %d, mode: %d", regulatory_domain, operating_class, operating_mode);
        cur->ws_info->hopping_schdule.regulatory_domain = regulatory_domain_saved;
        cur->ws_info->hopping_schdule.operating_mode = operating_mode_saved;
        cur->ws_info->hopping_schdule.operating_class = operating_class_saved;
        ws_common_regulatory_domain_config(cur);
        return -1;
    }
    // if settings change reset_restart for the settings needed
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        ws_bootstrap_restart(interface_id);
    }

    return 0;
}

int ws_management_network_size_set(
    int8_t interface_id,
    uint8_t network_size)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (network_size > NETWORK_SIZE_LARGE) {
        return -2;
    }
    ws_info(cur)->network_size_config = network_size;

    if (network_size == NETWORK_SIZE_LARGE) {
        ws_common_network_size_configure(cur, 5000);
    } else if (network_size == NETWORK_SIZE_MEDIUM) {
        ws_common_network_size_configure(cur, 200);
    } else {
        ws_common_network_size_configure(cur, 10);
    }
    return 0;
}

int ws_management_channel_mask_set(
    int8_t interface_id,
    uint32_t channel_mask[8])
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    memcpy(cur->ws_info->fhss_channel_mask, channel_mask, sizeof(uint32_t) * 8);
    return 0;
}

int ws_management_channel_plan_set(
    int8_t interface_id,
    uint8_t channel_plan,
    uint8_t uc_channel_function,
    uint8_t bc_channel_function,
    uint32_t ch0_freq, // Stack can not modify this
    uint8_t channel_spacing,// Stack can not modify this
    uint8_t number_of_channels)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    cur->ws_info->hopping_schdule.channel_plan = channel_plan;
    cur->ws_info->hopping_schdule.uc_channel_function = uc_channel_function;
    cur->ws_info->hopping_schdule.bc_channel_function = bc_channel_function;
    cur->ws_info->hopping_schdule.ch0_freq = ch0_freq;
    cur->ws_info->hopping_schdule.channel_spacing = channel_spacing;
    cur->ws_info->hopping_schdule.number_of_channels = number_of_channels;

    // TODO update fields to llc
    return 0;
}

int ws_management_fhss_timing_configure(
    int8_t interface_id,
    uint8_t fhss_uc_dwell_interval,
    uint32_t fhss_broadcast_interval,
    uint8_t fhss_bc_dwell_interval)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (fhss_uc_dwell_interval > 0) {
        cur->ws_info->fhss_uc_dwell_interval = fhss_uc_dwell_interval;
    }
    if (fhss_broadcast_interval > 0) {
        cur->ws_info->fhss_bc_interval = fhss_broadcast_interval;

    }
    if (fhss_bc_dwell_interval > 0) {
        cur->ws_info->fhss_bc_dwell_interval = fhss_bc_dwell_interval;

    }

    // if settings change reset_restart for the settings needed
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        ws_bootstrap_restart(interface_id);
    }
    return 0;
}

int ws_management_fhss_unicast_channel_function_configure(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (channel_function != WS_FIXED_CHANNEL &&
            channel_function != WS_VENDOR_DEF_CF &&
            channel_function != WS_DH1CF &&
            channel_function != WS_TR51CF) {
        return -2;
    }
    cur->ws_info->fhss_uc_channel_function = channel_function;
    cur->ws_info->fhss_uc_fixed_channel = fixed_channel;
    cur->ws_info->fhss_uc_dwell_interval = dwell_interval;

    // if settings change reset_restart for the settings needed
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        ws_bootstrap_restart(interface_id);
    }
    return 0;

}

int ws_management_fhss_broadcast_channel_function_configure(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval,
    uint32_t broadcast_interval)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    if (channel_function != WS_FIXED_CHANNEL &&
            channel_function != WS_VENDOR_DEF_CF &&
            channel_function != WS_DH1CF &&
            channel_function != WS_TR51CF) {
        return -2;
    }
    cur->ws_info->fhss_bc_channel_function = channel_function;
    cur->ws_info->fhss_bc_fixed_channel = fixed_channel;
    cur->ws_info->fhss_bc_dwell_interval = dwell_interval;
    cur->ws_info->fhss_bc_interval = broadcast_interval;

    // if settings change reset_restart for the settings needed
    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        // bootstrap active need to restart
        ws_bootstrap_restart(interface_id);
    }
    return 0;

}
#endif // HAVE_WS
