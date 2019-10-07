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
#include "randLIB.h"
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "ws_management_api.h"
#include "mac_api.h"

#ifdef HAVE_WS
#define TRACE_GROUP "wscm"

// estimated sensitivity -93 dbm converted to Wi-SUN RSL range
// This provides a range of -174 (0) to +80 (254) dBm
uint8_t DEVICE_MIN_SENS = 174 - 93;

#define TRICKLE_IMIN_60_SECS (60 * 10)
#define TRICKLE_IMIN_30_SECS (30 * 10)
#define TRICKLE_IMIN_15_SECS (15 * 10)

static const trickle_params_t trickle_params_pan_discovery_large = {
    .Imin = TRICKLE_IMIN_60_SECS,           /* 60 second; ticks are 1s */
    .Imax = TRICKLE_IMIN_60_SECS << 4,      /* 960 seconds 16 min*/
                                 .k = 1,    /* 1 */
                                 .TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE
};

static const trickle_params_t trickle_params_pan_discovery_medium = {
    .Imin = TRICKLE_IMIN_30_SECS,           /* 30 second; ticks are 1s */
    .Imax = TRICKLE_IMIN_30_SECS << 5,      /* 960 seconds 16 min*/
                                 .k = 1,    /* 1 */
                                 .TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE
};

static const trickle_params_t trickle_params_pan_discovery_small = {
    .Imin = TRICKLE_IMIN_15_SECS,           /* 15 second; ticks are 1s */
    .Imax = TRICKLE_IMIN_15_SECS << 2,      /* 60 seconds 1 min*/
                                 .k = 1,    /* 1 */
                                 .TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE
};

uint16_t test_max_child_count_override = 0xffff;


int8_t ws_generate_channel_list(uint32_t *channel_mask, uint16_t number_of_channels, uint8_t regulatory_domain)
{
    (void)regulatory_domain;
    for (uint8_t i = 0; i < number_of_channels; i++) {
        channel_mask[0 + (i / 32)] |= (1 << (i % 32));
    }
    return 0;
}

static uint32_t ws_decode_channel_spacing(uint8_t channel_spacing)
{
    if (CHANNEL_SPACING_100 == channel_spacing) {
        return 100000;
    } else if (CHANNEL_SPACING_200 == channel_spacing) {
        return 200000;
    } else if (CHANNEL_SPACING_250 == channel_spacing) {
        return 250000;
    } else if (CHANNEL_SPACING_400 == channel_spacing) {
        return 400000;
    } else if (CHANNEL_SPACING_600 == channel_spacing) {
        return 600000;
    }
    return 0;
}

static uint32_t ws_get_datarate_using_operating_mode(uint8_t operating_mode)
{
    if ((OPERATING_MODE_1a == operating_mode) || (OPERATING_MODE_1b == operating_mode)) {
        return 50000;
    } else if ((OPERATING_MODE_2a == operating_mode) || (OPERATING_MODE_2b == operating_mode)) {
        return 100000;
    } else if (OPERATING_MODE_3 == operating_mode) {
        return 150000;
    } else if ((OPERATING_MODE_4a == operating_mode) || (OPERATING_MODE_4b == operating_mode)) {
        return 200000;
    } else if (OPERATING_MODE_5 == operating_mode) {
        return 300000;
    }
    return 0;
}

static phy_modulation_index_e ws_get_modulation_index_using_operating_mode(uint8_t operating_mode)
{
    if ((OPERATING_MODE_1b == operating_mode) || (OPERATING_MODE_2b == operating_mode) || (OPERATING_MODE_4b == operating_mode)) {
        return MODULATION_INDEX_1_0;
    } else {
        return MODULATION_INDEX_0_5;
    }
}

static int ws_set_domain_rf_config(protocol_interface_info_entry_t *cur)
{
    phy_rf_channel_configuration_s rf_configs;
    rf_configs.channel_0_center_frequency = (uint32_t)cur->ws_info->hopping_schdule.ch0_freq * 100000;
    rf_configs.channel_spacing = ws_decode_channel_spacing(cur->ws_info->hopping_schdule.channel_spacing);
    rf_configs.datarate = ws_get_datarate_using_operating_mode(cur->ws_info->hopping_schdule.operating_mode);
    rf_configs.modulation_index = ws_get_modulation_index_using_operating_mode(cur->ws_info->hopping_schdule.operating_mode);
    rf_configs.modulation = M_2FSK;
    rf_configs.number_of_channels = cur->ws_info->hopping_schdule.number_of_channels;
    ws_bootstrap_set_rf_config(cur, rf_configs);
    return 0;
}

int8_t ws_common_regulatory_domain_config(protocol_interface_info_entry_t *cur)
{
    cur->ws_info->hopping_schdule.channel_plan = 0;

    if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_KR) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 9171;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 9173;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_400;
        } else {
            return -1;
        }
    } else if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_EU) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 8631;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_100;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 8631;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else if (cur->ws_info->hopping_schdule.operating_class == 3) {
            cur->ws_info->hopping_schdule.ch0_freq = 8701;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_100;
        } else if (cur->ws_info->hopping_schdule.operating_class == 4) {
            cur->ws_info->hopping_schdule.ch0_freq = 8702;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else {
            return -1;
        }
    } else if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_IN) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 8651;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_100;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 8651;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else {
            return -1;
        }
    } else if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_NA) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 9022;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 9024;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_400;
        } else if (cur->ws_info->hopping_schdule.operating_class == 3) {
            cur->ws_info->hopping_schdule.ch0_freq = 9026;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_600;
        } else {
            return -1;
        }
    } else if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_JP) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 9206;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 9209;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_400;
        } else if (cur->ws_info->hopping_schdule.operating_class == 3) {
            cur->ws_info->hopping_schdule.ch0_freq = 9208;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_600;
        } else {
            return -1;
        }
    } else if (cur->ws_info->hopping_schdule.regulatory_domain == REG_DOMAIN_WW) {
        if (cur->ws_info->hopping_schdule.operating_class == 1) {
            cur->ws_info->hopping_schdule.ch0_freq = 24002;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_200;
        } else if (cur->ws_info->hopping_schdule.operating_class == 2) {
            cur->ws_info->hopping_schdule.ch0_freq = 24004;
            cur->ws_info->hopping_schdule.channel_spacing = CHANNEL_SPACING_400;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
    cur->ws_info->hopping_schdule.number_of_channels = (uint8_t)ws_common_channel_number_calc(cur->ws_info->hopping_schdule.regulatory_domain, cur->ws_info->hopping_schdule.operating_class);
    if (!cur->ws_info->hopping_schdule.number_of_channels) {
        return -1;
    }
    // Note: doesn't work for Brazil region
    ws_generate_channel_list(cur->ws_info->hopping_schdule.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);
    ws_set_domain_rf_config(cur);
    return 0;
}

uint16_t ws_common_channel_number_calc(uint8_t regulatory_domain, uint8_t operating_class)
{
    if (regulatory_domain == REG_DOMAIN_KR) {
        if (operating_class == 1) {
            return 32;
        } else if (operating_class == 2) {
            return 16;
        }
    } else if (regulatory_domain == REG_DOMAIN_EU) {
        if (operating_class == 1) {
            return 69;
        } else if (operating_class == 2) {
            return 35;
        } else if (operating_class == 3) {
            return 55;
        } else if (operating_class == 4) {
            return 27;
        }
    } else if (regulatory_domain == REG_DOMAIN_IN) {
        if (operating_class == 1) {
            return 19;
        } else if (operating_class == 2) {
            return 10;
        }
    } else if (regulatory_domain == REG_DOMAIN_NA) {
        if (operating_class == 1) {
            return 129;
        } else if (operating_class == 2) {
            return 64;
        } else if (operating_class == 3) {
            return 42;
        }
    } else if (regulatory_domain == REG_DOMAIN_JP) {
        if (operating_class == 1) {
            return 38;
        } else if (operating_class == 2) {
            return 18;
        } else if (operating_class == 3) {
            return 12;
        }
    } else if (regulatory_domain == REG_DOMAIN_WW) {
        if (operating_class == 1) {
            // TODO we dont support this yet, but it is used as test value
            return 416;
        } else if (operating_class == 2) {
            return 207;
        }
    }
    return 0;
}

int8_t ws_common_allocate_and_init(protocol_interface_info_entry_t *cur)
{

    if (!cur->ws_info) {
        cur->ws_info = ns_dyn_mem_alloc(sizeof(ws_info_t));
    }
    if (!cur->ws_info) {
        return -1;
    }

    memset(cur->ws_info, 0, sizeof(ws_info_t));
    ns_list_init(&cur->ws_info->active_nud_process);
    ns_list_init(&cur->ws_info->free_nud_entries);

    cur->ws_info->pan_information.use_parent_bs = true;
    cur->ws_info->pan_information.rpl_routing_method = true;
    cur->ws_info->pan_information.version = WS_FAN_VERSION_1_0;

    cur->ws_info->hopping_schdule.regulatory_domain = REG_DOMAIN_EU;
    cur->ws_info->hopping_schdule.operating_mode = OPERATING_MODE_3;
    cur->ws_info->hopping_schdule.operating_class = 2;
    ws_common_regulatory_domain_config(cur);
    cur->ws_info->network_size_config = NETWORK_SIZE_MEDIUM;
    ws_common_network_size_configure(cur, 200); // defaults to medium network size

    // Set defaults for the device. user can modify these.
    cur->ws_info->fhss_uc_fixed_channel = 0xffff;
    cur->ws_info->fhss_bc_fixed_channel = 0xffff;
    cur->ws_info->fhss_uc_dwell_interval = WS_FHSS_UC_DWELL_INTERVAL;
    cur->ws_info->fhss_bc_interval = WS_FHSS_BC_INTERVAL;
    cur->ws_info->fhss_bc_dwell_interval = WS_FHSS_BC_DWELL_INTERVAL;
    cur->ws_info->fhss_uc_channel_function = WS_DH1CF;
    cur->ws_info->fhss_bc_channel_function = WS_DH1CF;
    for (uint8_t n = 0; n < 8; n++) {
        cur->ws_info->fhss_channel_mask[n] = 0xffffffff;
    }

    return 0;
}
void ws_common_network_size_configure(protocol_interface_info_entry_t *cur, uint16_t network_size)
{
    // TODO Modify NUD timings based on network size
    // TODO Modify EAPOLL timings

    if (network_size < 100) {
        // Configure the Wi-SUN discovery trickle parameters
        cur->ws_info->trickle_params_pan_discovery = trickle_params_pan_discovery_small;
        // default values are for Wi-SUN small network parameters
        // imin: 14 (16s)
        // doublings:3 (128s)
        // redundancy; 0 Disabled
        if (cur->ws_info->network_size_config == NETWORK_SIZE_AUTOMATIC) {
            ws_bbr_rpl_config(14, 3, 0);
        } else {
            ws_bbr_rpl_config(0, 0, 0);
        }
        ws_pae_controller_timing_adjust(1); // Fast and reactive network
    } else if (network_size < 300) {
        // Configure the Wi-SUN discovery trickle parameters
        cur->ws_info->trickle_params_pan_discovery = trickle_params_pan_discovery_medium;
        // Something in between
        // imin: 15 (32s)
        // doublings:5 (960s)
        // redundancy; 10
        ws_bbr_rpl_config(15, 5, 10);
        ws_pae_controller_timing_adjust(9); // medium limited network
    } else {
        // Configure the Wi-SUN discovery trickle parameters
        cur->ws_info->trickle_params_pan_discovery = trickle_params_pan_discovery_large;
        // Wi-SUN Large network parameters
        // imin: 19 (524s, 9 min)
        // doublings:1 (1048s, 17 min)
        // redundancy; 10 May need some tuning still
        ws_bbr_rpl_config(19, 1, 10);
        ws_pae_controller_timing_adjust(24); // Very slow and high latency network
    }
    return;
}

void ws_common_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    ws_bbr_seconds_timer(cur, seconds);
    ws_bootstrap_seconds_timer(cur, seconds);
    blacklist_ttl_update(seconds);
}

void ws_common_fast_timer(protocol_interface_info_entry_t *cur, uint16_t ticks)
{
    ws_bootstrap_trickle_timer(cur, ticks);
    ws_nud_active_timer(cur, ticks);
}


void ws_common_neighbor_update(protocol_interface_info_entry_t *cur, const uint8_t *ll_address)
{
    //Neighbor connectected update
    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), ll_address, false, NULL);
    if (mac_neighbor) {
        ws_nud_entry_remove_active(cur, mac_neighbor);
    }
}

void ws_common_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address)
{
    tr_warn("ARO registration Failure %s", trace_ipv6(ll_address));
    ws_bootstrap_aro_failure(cur, ll_address);
}

void ws_common_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address)
{
    tr_debug("neighbor remove %s", trace_ipv6(ll_address));
    ws_bootstrap_neighbor_remove(cur, ll_address);
}



bool ws_common_allow_child_registration(protocol_interface_info_entry_t *interface, const uint8_t *eui64)
{
    uint8_t child_count = 0;
    uint8_t max_child_count = mac_neighbor_info(interface)->list_total_size - WS_NON_CHILD_NEIGHBOUR_COUNT;

    // Test API to limit child count
    if (test_max_child_count_override != 0xffff) {
        max_child_count = test_max_child_count_override;
    }

    //Validate Is EUI64 already allocated for any address
    if (ipv6_neighbour_has_registered_by_eui64(&interface->ipv6_neighbour_cache, eui64)) {
        tr_info("Child registration from old child");
        return true;
    }

    //Verify that we have Selected Parent
    if (interface->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER && !rpl_control_parent_candidate_list_size(interface, true)) {
        tr_info("Do not accept new ARO child: no selected parent");
        return false;
    }


    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur, &mac_neighbor_info(interface)->neighbour_list) {

        if (ipv6_neighbour_has_registered_by_eui64(&interface->ipv6_neighbour_cache, cur->mac64)) {
            child_count++;
        }
    }
    if (child_count >= max_child_count) {
        tr_warn("Child registration not allowed %d/%d, max:%d", child_count, max_child_count, mac_neighbor_info(interface)->list_total_size);
        return false;
    }
    tr_info("Child registration allowed %d/%d, max:%d", child_count, max_child_count, mac_neighbor_info(interface)->list_total_size);
    return true;
}

bool ws_common_negative_aro_mark(protocol_interface_info_entry_t *interface, const uint8_t *eui64)
{
    mac_neighbor_table_entry_t *neighbour = mac_neighbor_table_address_discover(mac_neighbor_info(interface), eui64, ADDR_802_15_4_LONG);
    if (!neighbour) {
        return false;
    }
    ws_neighbor_class_entry_t *ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbour->index);
    ws_neighbor->negative_aro_send = true;
    neighbour->lifetime = WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT; //Remove anyway if Packet is freed before MAC push
    return true;
}

void ws_common_etx_validate(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neigh)
{
    etx_storage_t *etx_entry = etx_storage_entry_get(interface->id, neigh->index);

    if (neigh->nud_active || !neigh->trusted_device || !etx_entry || etx_entry->etx_samples) {
        return; //Do not trig Second NS if Active NUD already, not trusted or ETX samples already done
    }

    ws_bootstrap_etx_accelerate(interface, neigh);
}

uint32_t ws_common_version_lifetime_get(uint8_t config)
{
    uint32_t lifetime;
    if (config == NETWORK_SIZE_SMALL) {
        lifetime = PAN_VERSION_SMALL_NETWORK_LIFETIME;
    } else if (config == NETWORK_SIZE_MEDIUM) {
        lifetime = PAN_VERSION_MEDIUM_NETWORK_LIFETIME;
    } else {
        lifetime = PAN_VERSION_LARGE_NETWORK_LIFETIME;
    }

    return lifetime;

}

uint32_t ws_common_version_timeout_get(uint8_t config)
{
    uint32_t lifetime;
    if (config == NETWORK_SIZE_SMALL) {
        lifetime = PAN_VERSION_SMALL_NETWORK_TIMEOUT;
    } else if (config == NETWORK_SIZE_MEDIUM) {
        lifetime = PAN_VERSION_MEDIUM_NETWORK_TIMEOUT;
    } else {
        lifetime = PAN_VERSION_LARGE_NETWORK_TIMEOUT;
    }

    return lifetime;
}


#endif // HAVE_WS

