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
#include "fhss_config.h"
#include "ws_management_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"

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
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!network_name_ptr || !fhss_timer_ptr) {
        return -2;
    }

    ws_phy_cfg_t phy_cfg;
    if (ws_cfg_phy_get(&phy_cfg, NULL) < 0) {
        return -3;
    }

    phy_cfg.regulatory_domain = regulatory_domain;

    if (ws_cfg_phy_set(cur, NULL, &phy_cfg, 0) < 0) {
        return -4;
    }

    ws_gen_cfg_t gen_cfg;
    if (ws_cfg_gen_get(&gen_cfg, NULL) < 0) {
        return -3;
    }

    strncpy(gen_cfg.network_name, network_name_ptr, 32);

    if (ws_cfg_gen_set(cur, NULL, &gen_cfg, 0) < 0) {
        return -4;
    }

    cur->ws_info->fhss_timer_ptr = fhss_timer_ptr;

    return 0;
}

int ws_management_network_name_set(
    int8_t interface_id,
    char *network_name_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!network_name_ptr) {
        return -2;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_gen_get(&cfg, NULL) < 0) {
        return -3;
    }

    strncpy(cfg.network_name, network_name_ptr, 32);

    if (ws_cfg_gen_set(cur, NULL, &cfg, 0) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_network_name_get(
    int8_t interface_id,
    char *network_name_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!network_name_ptr) {
        return -2;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_gen_get(&cfg, NULL) < 0) {
        return -3;
    }

    memcpy(network_name_ptr, cfg.network_name, 32);

    return 0;
}

int ws_management_network_name_validate(
    int8_t interface_id,
    char *network_name_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!network_name_ptr) {
        return -2;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_gen_get(&cfg, NULL) < 0) {
        return -3;
    }

    strncpy(cfg.network_name, network_name_ptr, 32);

    if (ws_cfg_gen_validate(NULL, &cfg) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_regulatory_domain_set(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t operating_class,
    uint8_t operating_mode)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_phy_cfg_t cfg;
    if (ws_cfg_phy_get(&cfg, NULL) < 0) {
        return -3;
    }

    if (regulatory_domain != 255) {
        cfg.regulatory_domain = regulatory_domain;
    }
    if (operating_mode != 255) {
        cfg.operating_mode = operating_mode;
    }
    if (operating_class != 255) {
        cfg.operating_class = operating_class;
    }

    if (ws_cfg_phy_set(cur, NULL, &cfg, 0) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_regulatory_domain_get(
    int8_t interface_id,
    uint8_t *regulatory_domain,
    uint8_t *operating_class,
    uint8_t *operating_mode)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!regulatory_domain || !operating_class || !operating_mode) {
        return -2;
    }

    ws_phy_cfg_t cfg;
    if (ws_cfg_phy_get(&cfg, NULL) < 0) {
        return -3;
    }

    *regulatory_domain = cfg.regulatory_domain;
    *operating_class = cfg.operating_class;
    *operating_mode = cfg.operating_mode;

    return 0;
}

int ws_management_regulatory_domain_validate(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t operating_class,
    uint8_t operating_mode)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_phy_cfg_t cfg;
    if (ws_cfg_phy_get(&cfg, NULL) < 0) {
        return -3;
    }

    cfg.regulatory_domain = regulatory_domain;
    cfg.operating_class = operating_class;
    cfg.operating_mode = operating_mode;

    if (ws_cfg_phy_validate(NULL, &cfg) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_network_size_set(
    int8_t interface_id,
    uint8_t network_size)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_network_size_get(&cfg, NULL) < 0) {
        return -3;
    }

    cfg.network_size = network_size;

    if (ws_cfg_network_size_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
    }

    return 0;
}

int ws_management_network_size_get(
    int8_t interface_id,
    uint8_t *network_size)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!network_size) {
        return -2;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_network_size_get(&cfg, NULL) < 0) {
        return -3;
    }

    *network_size = cfg.network_size;

    return 0;
}

int ws_management_network_size_validate(
    int8_t interface_id,
    uint8_t network_size)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_gen_cfg_t cfg;
    if (ws_cfg_network_size_get(&cfg, NULL) < 0) {
        return -3;
    }

    cfg.network_size = network_size;

    if (ws_cfg_network_size_validate(NULL, &cfg) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_channel_mask_set(
    int8_t interface_id,
    uint32_t channel_mask[8])
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    memcpy(cfg.fhss_channel_mask, channel_mask, sizeof(uint32_t) * 8);

    if (ws_cfg_fhss_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
    }

    return 0;
}

int ws_management_channel_mask_get(
    int8_t interface_id,
    uint32_t *channel_mask)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!channel_mask) {
        return -2;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    memcpy(channel_mask, cfg.fhss_channel_mask, sizeof(uint32_t) * 8);

    return 0;
}

int ws_management_channel_mask_validate(
    int8_t interface_id,
    uint32_t channel_mask[8])
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    memcpy(cfg.fhss_channel_mask, channel_mask, sizeof(uint32_t) * 8);

    if (ws_cfg_fhss_validate(NULL, &cfg) < 0) {
        return -4;
    }

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
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
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
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (fhss_uc_dwell_interval > 0) {
        cfg.fhss_uc_dwell_interval = fhss_uc_dwell_interval;
    }
    if (fhss_broadcast_interval > 0) {
        cfg.fhss_bc_interval = fhss_broadcast_interval;
    }
    if (fhss_bc_dwell_interval > 0) {
        cfg.fhss_bc_dwell_interval = fhss_bc_dwell_interval;
    }

    if (ws_cfg_fhss_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
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
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (dwell_interval > 0) {
        cfg.fhss_uc_dwell_interval = dwell_interval;
    }

    cfg.fhss_uc_channel_function = channel_function;
    cfg.fhss_uc_fixed_channel = fixed_channel;

    if (ws_cfg_fhss_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
    }

    return 0;
}

int ws_management_fhss_unicast_channel_function_get(
    int8_t interface_id,
    uint8_t *channel_function,
    uint16_t *fixed_channel,
    uint8_t *dwell_interval)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!channel_function || !fixed_channel || !dwell_interval) {
        return -2;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    *dwell_interval = cfg.fhss_uc_dwell_interval;
    *channel_function = cfg.fhss_uc_channel_function;
    *fixed_channel = cfg.fhss_uc_fixed_channel;

    return 0;
}

int ws_management_fhss_unicast_channel_function_validate(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    cfg.fhss_uc_dwell_interval = dwell_interval;
    cfg.fhss_uc_channel_function = channel_function;
    cfg.fhss_uc_fixed_channel = fixed_channel;

    if (ws_cfg_fhss_validate(NULL, &cfg) < 0) {
        return -4;
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
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (dwell_interval > 0) {
        cfg.fhss_bc_dwell_interval = dwell_interval;
    }
    if (broadcast_interval > 0) {
        cfg.fhss_bc_interval = broadcast_interval;
    }

    cfg.fhss_bc_channel_function = channel_function;
    cfg.fhss_bc_fixed_channel = fixed_channel;

    if (ws_cfg_fhss_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
    }

    return 0;
}

int ws_management_fhss_broadcast_channel_function_get(
    int8_t interface_id,
    uint8_t *channel_function,
    uint16_t *fixed_channel,
    uint8_t *dwell_interval,
    uint32_t *broadcast_interval)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!channel_function || !fixed_channel || !dwell_interval) {
        return -2;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    *dwell_interval = cfg.fhss_bc_dwell_interval;
    *broadcast_interval = cfg.fhss_bc_interval;
    *channel_function = cfg.fhss_bc_channel_function;
    *fixed_channel = cfg.fhss_bc_fixed_channel;

    return 0;
}

int ws_management_fhss_broadcast_channel_function_validate(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval,
    uint32_t broadcast_interval)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_fhss_cfg_t cfg;
    if (ws_cfg_fhss_get(&cfg, NULL) < 0) {
        return -2;
    }

    cfg.fhss_bc_dwell_interval = dwell_interval;
    cfg.fhss_bc_interval = broadcast_interval;
    cfg.fhss_bc_channel_function = channel_function;
    cfg.fhss_bc_fixed_channel = fixed_channel;

    if (ws_cfg_fhss_validate(NULL, &cfg) < 0) {
        return -4;
    }

    return 0;
}

int ws_management_timing_parameters_set(
    int8_t interface_id,
    uint16_t disc_trickle_imin,
    uint16_t disc_trickle_imax,
    uint8_t disc_trickle_k,
    uint16_t pan_timeout)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_timing_cfg_t cfg;
    if (ws_cfg_timing_get(&cfg, NULL) < 0) {
        return -2;
    }

    if (disc_trickle_imin > 0) {
        cfg.disc_trickle_imin = disc_trickle_imin;
    }
    if (disc_trickle_imax > 0) {
        cfg.disc_trickle_imax = disc_trickle_imax;
    }
    if (disc_trickle_k > 0) {
        cfg.disc_trickle_k = disc_trickle_k;
    }
    if (pan_timeout > 0) {
        cfg.pan_timeout = pan_timeout;
    }

    if (ws_cfg_timing_set(cur, NULL, &cfg, 0) < 0) {
        return -3;
    }

    return 0;
}

int ws_management_timing_parameters_get(
    int8_t interface_id,
    uint16_t *disc_trickle_imin,
    uint16_t *disc_trickle_imax,
    uint8_t *disc_trickle_k,
    uint16_t *pan_timeout)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }
    if (!disc_trickle_imin || !disc_trickle_imax || !disc_trickle_k || !pan_timeout) {
        return -2;
    }

    ws_timing_cfg_t cfg;
    if (ws_cfg_timing_get(&cfg, NULL) < 0) {
        return -2;
    }

    *disc_trickle_imin = cfg.disc_trickle_imin;
    *disc_trickle_imax = cfg.disc_trickle_imax;
    *disc_trickle_k = cfg.disc_trickle_k;
    *pan_timeout = cfg.pan_timeout;

    return 0;
}

int ws_management_timing_parameters_validate(
    int8_t interface_id,
    uint16_t disc_trickle_imin,
    uint16_t disc_trickle_imax,
    uint8_t disc_trickle_k,
    uint16_t pan_timeout)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (interface_id >= 0 && (!cur || !ws_info(cur))) {
        return -1;
    }

    ws_timing_cfg_t cfg;
    if (ws_cfg_timing_get(&cfg, NULL) < 0) {
        return -2;
    }

    cfg.disc_trickle_imin = disc_trickle_imin;
    cfg.disc_trickle_imax = disc_trickle_imax;
    cfg.disc_trickle_k = disc_trickle_k;
    cfg.pan_timeout = pan_timeout;

    if (ws_cfg_timing_validate(NULL, &cfg) < 0) {
        return -4;
    }

    return 0;
}

#endif // HAVE_WS
