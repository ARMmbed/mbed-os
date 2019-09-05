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
#include "net_interface.h"
#include "eventOS_event.h"
#include "randLIB.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "sw_mac.h"
#include "ccmLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/MAC/mac_ie_lib.h"
#include "MPL/mpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Service_Libs/Trickle/trickle.h"
#include "Service_Libs/fhss/channel_list.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_bbr_api_internal.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_llc.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "6LoWPAN/ws/ws_ie_lib.h"
#include "6LoWPAN/ws/ws_stats.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "platform/topo_trace.h"
#include "libDHCPv6/libDHCPv6.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "ws_management_api.h"
#include "net_rpl.h"
#include "mac_api.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"
#include "6LoWPAN/ws/ws_eapol_auth_relay.h"
#include "6LoWPAN/ws/ws_eapol_relay.h"

#define TRACE_GROUP "wsbs"

#ifdef HAVE_WS

static void ws_bootstrap_event_handler(arm_event_s *event);
static void ws_bootstrap_state_change(protocol_interface_info_entry_t *cur, icmp_state_t nwk_bootstrap_state);
//static bool ws_bootstrap_state_active(struct protocol_interface_info_entry *cur);
//static bool ws_bootstrap_state_wait_rpl(struct protocol_interface_info_entry *cur);
static bool ws_bootstrap_state_discovery(struct protocol_interface_info_entry *cur);
static int8_t ws_bootsrap_event_trig(ws_bootsrap_event_type_e event_type, int8_t interface_id, arm_library_event_priority_e priority, void *event_data);

static bool ws_bootstrap_neighbor_info_request(struct protocol_interface_info_entry *interface, const uint8_t *mac_64, llc_neighbour_req_t *neighbor_buffer, bool request_new);
static uint16_t ws_bootstrap_routing_cost_calculate(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_rank_get(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_min_rank_inc_get(protocol_interface_info_entry_t *cur);

static void ws_bootstrap_mac_security_enable(protocol_interface_info_entry_t *cur);
static void ws_bootstrap_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t operation, uint8_t index, uint8_t *key);
static void ws_bootstrap_nw_key_clear(protocol_interface_info_entry_t *cur, uint8_t slot);
static void ws_bootstrap_nw_key_index_set(protocol_interface_info_entry_t *cur, uint8_t index);
static void ws_bootstrap_nw_frame_counter_set(protocol_interface_info_entry_t *cur, uint32_t counter);
static void ws_bootstrap_nw_frame_counter_read(protocol_interface_info_entry_t *cur, uint32_t *counter);
static void ws_bootstrap_authentication_completed(protocol_interface_info_entry_t *cur, bool success);
static void ws_bootstrap_pan_version_increment(protocol_interface_info_entry_t *cur);
static ws_nud_table_entry_t *ws_nud_entry_discover(protocol_interface_info_entry_t *cur, void *neighbor);
static void ws_nud_entry_remove(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *entry_ptr);
static bool ws_neighbor_entry_nud_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data);
static bool ws_rpl_dio_new_parent_accept(struct protocol_interface_info_entry *interface);

typedef enum {
    WS_PARENT_SOFT_SYNCH = 0,  /**< let FHSS make decision if synchronization is needed*/
    WS_PARENT_HARD_SYNCH,      /**< Synch FHSS with latest synch information*/
    WS_EAPOL_PARENT_SYNCH,  /**< Broadcast synch with EAPOL parent*/
} ws_parent_synch_e;


static void ws_bootsrap_create_ll_address(uint8_t *ll_address, const uint8_t *mac64)
{
    memcpy(ll_address, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(ll_address + 8, mac64, 8);
    ll_address[8] ^= 2;
}

mac_neighbor_table_entry_t *ws_bootstrap_mac_neighbor_add(struct protocol_interface_info_entry *interface, const uint8_t *src64)

{
    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), src64, MAC_ADDR_MODE_64_BIT);
    if (neighbor) {
        return neighbor;
    }

    neighbor = mac_neighbor_table_entry_allocate(mac_neighbor_info(interface), src64);
    if (!neighbor) {
        return NULL;
    }
    // TODO only call these for new neighbour
    mlme_device_descriptor_t device_desc;
    neighbor->lifetime = WS_NEIGHBOR_LINK_TIMEOUT;
    neighbor->link_lifetime = WS_NEIGHBOR_LINK_TIMEOUT;
    tr_debug("Added new neighbor %s : index:%u", trace_array(src64, 8), neighbor->index);
    mac_helper_device_description_write(interface, &device_desc, neighbor->mac64, neighbor->mac16, 0, false);
    mac_helper_devicetable_set(&device_desc, interface, neighbor->index, interface->mac_parameters->mac_default_key_index, true);
    return neighbor;
}

static void ws_bootstrap_neighbor_delete(struct protocol_interface_info_entry *interface, mac_neighbor_table_entry_t *entry_ptr)
{
    mac_helper_devicetable_remove(interface->mac_api, entry_ptr->index, entry_ptr->mac64);
    etx_neighbor_remove(interface->id, entry_ptr->index);
    ws_neighbor_class_entry_remove(&interface->ws_info->neighbor_storage, entry_ptr->index);
}

static void ws_bootstrap_neighbor_list_clean(struct protocol_interface_info_entry *interface)
{

    mac_neighbor_table_neighbor_list_clean(mac_neighbor_info(interface));
}

static void ws_bootstrap_address_notification_cb(struct protocol_interface_info_entry *interface, const struct if_address_entry *addr, if_address_callback_t reason)
{
    /* No need for LL address registration */
    if (addr->source == ADDR_SOURCE_UNKNOWN) {
        return;
    }
    if (reason == ADDR_CALLBACK_DAD_COMPLETE) {
        //Trig Address Registartion only when Bootstrap is ready
        if (interface->nwk_bootstrap_state == ER_BOOTSRAP_DONE && addr->source != ADDR_SOURCE_DHCP) {
            tr_debug("Address registration %s", trace_ipv6(addr->address));
            rpl_control_register_address(interface, addr->address);
        }
        if (addr_ipv6_scope(addr->address, interface) > IPV6_SCOPE_LINK_LOCAL) {
            // at least ula address available inside mesh.
            interface->global_address_available = true;
        }

    } else if (reason == ADDR_CALLBACK_DELETED) {
        // What to do?
        // Go through address list and check if there is global address still available
        if (addr->source == ADDR_SOURCE_DHCP) {
            //Deprecate dhcpv address
            uint8_t address[16];
            memcpy(address, addr->address, 16);
            dhcp_client_global_address_delete(interface->id, NULL, address);
        }
        //Discover prefix policy
        addr_policy_remove_by_label(WS_NON_PREFFRED_LABEL);

        interface->global_address_available = false;
        ns_list_foreach(if_address_entry_t, addr_str, &interface->ip_addresses) {
            if (addr_ipv6_scope(addr_str->address, interface) > IPV6_SCOPE_LINK_LOCAL) {
                // at least ula address available inside mesh.
                interface->global_address_available = true;
                break;
            }
        }
    } else if (reason == ADDR_CALLBACK_TIMER) {
        if (addr->source != ADDR_SOURCE_DHCP) {
            tr_debug("Address Re registration %s", trace_ipv6(addr->address));
            //Register
            rpl_control_register_address(interface, addr->address);
        }
    }
}

static int ws_bootstrap_tasklet_init(protocol_interface_info_entry_t *cur)
{
    if (cur->bootStrapId < 0) {
        cur->bootStrapId = eventOS_event_handler_create(&ws_bootstrap_event_handler, WS_INIT_EVENT);
        tr_debug("WS tasklet init");
    }

    if (cur->bootStrapId < 0) {
        tr_error("tasklet init failed");
        return -1;
    }


    return 0;
}
static void ws_nwk_event_post(protocol_interface_info_entry_t *cur, arm_nwk_interface_status_type_e posted_event)
{
    arm_event_s event = {
        .receiver = cur->net_start_tasklet,
        .sender = protocol_read_tasklet_id(), /**< Event sender Tasklet ID */
        .event_type = ARM_LIB_NWK_INTERFACE_EVENT,
        .event_data = posted_event,
        .event_id = (int8_t) cur->id,
        .data_ptr = NULL,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };
    if (eventOS_event_send(&event) != 0) {
        tr_error("nwk_net_event_post(): event send failed");
    }
}

static int8_t ws_bootsrap_event_trig(ws_bootsrap_event_type_e event_type, int8_t interface_id, arm_library_event_priority_e priority, void *event_data)
{
    arm_event_s event = {
        .receiver = interface_id,
        .sender = 0,
        .event_type = event_type,
        .priority = priority,
        .data_ptr = event_data,
    };
    return eventOS_event_send(&event);
}

static void ws_nud_table_reset(protocol_interface_info_entry_t *cur)
{
    //Empty active list
    ns_list_foreach_safe(ws_nud_table_entry_t, entry, &cur->ws_info->active_nud_process) {
        ns_list_remove(&cur->ws_info->active_nud_process, entry);
    }

    //Empty free list
    ns_list_foreach_safe(ws_nud_table_entry_t, entry, &cur->ws_info->free_nud_entries) {
        ns_list_remove(&cur->ws_info->free_nud_entries, entry);
    }
    //Add to free list to full
    for (int i = 0; i < ACTIVE_NUD_PROCESS_MAX; i++) {
        ns_list_add_to_end(&cur->ws_info->free_nud_entries, &cur->ws_info->nud_table_entrys[i]);
    }
}

static ws_nud_table_entry_t *ws_nud_entry_get_free(protocol_interface_info_entry_t *cur)
{
    ws_nud_table_entry_t *entry = ns_list_get_first(&cur->ws_info->free_nud_entries);
    if (entry) {
        entry->wait_response = false;
        entry->retry_count = 0;
        entry->nud_process = false;
        entry->timer = randLIB_get_random_in_range(1, 900);
        entry->neighbor_info = NULL;
        ns_list_remove(&cur->ws_info->free_nud_entries, entry);
        ns_list_add_to_end(&cur->ws_info->active_nud_process, entry);
    }
    return entry;
}


void ws_nud_entry_remove_active(protocol_interface_info_entry_t *cur, void *neighbor)
{
    ws_nud_table_entry_t *entry = ws_nud_entry_discover(cur, neighbor);

    if (entry) {
        mac_neighbor_table_entry_t *mac_neighbor = neighbor;
        ns_list_remove(&cur->ws_info->active_nud_process, entry);
        ns_list_add_to_end(&cur->ws_info->free_nud_entries, entry);
        if (mac_neighbor->nud_active) {
            mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), mac_neighbor, mac_neighbor->link_lifetime);
        }

        mac_neighbor_table_neighbor_connected(mac_neighbor_info(cur), mac_neighbor);
    }
}


static ws_nud_table_entry_t *ws_nud_entry_discover(protocol_interface_info_entry_t *cur, void *neighbor)
{
    ns_list_foreach(ws_nud_table_entry_t, entry, &cur->ws_info->active_nud_process) {
        if (entry->neighbor_info == neighbor) {
            return entry;
        }
    }
    return NULL;
}


static void ws_nud_state_clean(protocol_interface_info_entry_t *cur, ws_nud_table_entry_t *entry)
{
    mac_neighbor_table_entry_t *neighbor = entry->neighbor_info;
    ns_list_remove(&cur->ws_info->active_nud_process, entry);
    ns_list_add_to_end(&cur->ws_info->free_nud_entries, entry);
    if (neighbor->nud_active) {
        neighbor->nud_active = false;
        mac_neighbor_info(cur)->active_nud_process--;
    }
}

static void ws_nud_entry_remove(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *entry_ptr)
{
    ws_nud_table_entry_t *nud_entry = ws_nud_entry_discover(cur, entry_ptr);
    if (nud_entry) {
        ws_nud_state_clean(cur, nud_entry);
    }
}

static bool ws_nud_message_build(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *neighbor)
{
    //Send NS
    uint8_t ll_target[16];
    ws_bootsrap_create_ll_address(ll_target, neighbor->mac64);
    tr_info("NUD generate NS %u", neighbor->index);
    buffer_t *buffer = icmpv6_build_ns(cur, ll_target, NULL, true, false, NULL);
    if (buffer) {
        protocol_push(buffer);
        return true;
    }
    return false;
}

void ws_nud_active_timer(protocol_interface_info_entry_t *cur, uint16_t ticks)
{
    //Convert TICKS to real milliseconds
    if (ticks > 0xffff / 100) {
        ticks = 0xffff;
    } else if (ticks == 0) {
        ticks = 1;
    } else {
        ticks *= 100;
    }

    ns_list_foreach_safe(ws_nud_table_entry_t, entry, &cur->ws_info->active_nud_process) {
        if (entry->timer <= ticks) {
            //TX Process or timeout
            if (entry->wait_response) {
                //Timeout for NUD or Probe
                if (entry->nud_process) {
                    tr_debug("NUD NA timeout");
                    if (entry->retry_count < 2) {
                        entry->timer = randLIB_get_random_in_range(1, 900);
                        entry->wait_response = false;
                    } else {
                        //Clear entry from active list
                        ws_nud_state_clean(cur, entry);
                        //Remove whole entry
                        mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), entry->neighbor_info);
                    }
                } else {
                    ws_nud_state_clean(cur, entry);
                }

            } else {
                //Random TX wait period is over
                entry->wait_response = ws_nud_message_build(cur, entry->neighbor_info);
                if (!entry->wait_response) {
                    if (entry->nud_process && entry->retry_count < 2) {
                        entry->timer = randLIB_get_random_in_range(1, 900);
                    } else {
                        //Clear entry from active list
                        //Remove and try again later on
                        ws_nud_state_clean(cur, entry);
                    }
                } else {
                    entry->retry_count++;
                    entry->timer = 5001;
                }
            }
        } else {
            entry->timer -= ticks;
        }
    }
}

static fhss_ws_neighbor_timing_info_t *ws_get_neighbor_info(const fhss_api_t *api, uint8_t eui64[8])
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_fhss_api(api);
    if (!cur || !cur->mac_parameters || !mac_neighbor_info(cur)) {
        return NULL;
    }
    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(cur), eui64, MAC_ADDR_MODE_64_BIT);
    if (!mac_neighbor) {
        return NULL;
    }
    ws_neighbor_class_entry_t *ws_neighbor =  ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, mac_neighbor->index);
    if (!ws_neighbor) {
        return NULL;
    }
    return &ws_neighbor->fhss_data;
}
static void ws_bootstrap_llc_hopping_update(struct protocol_interface_info_entry *cur, const fhss_ws_configuration_t *fhss_configuration)
{
    memcpy(cur->ws_info->hopping_schdule.channel_mask, fhss_configuration->channel_mask, sizeof(uint32_t) * 8);
    cur->ws_info->hopping_schdule.uc_fixed_channel = fhss_configuration->unicast_fixed_channel;
    cur->ws_info->hopping_schdule.bc_fixed_channel = fhss_configuration->broadcast_fixed_channel;
    cur->ws_info->hopping_schdule.uc_channel_function = fhss_configuration->ws_uc_channel_function;
    cur->ws_info->hopping_schdule.bc_channel_function = fhss_configuration->ws_bc_channel_function;
    cur->ws_info->hopping_schdule.fhss_bc_dwell_interval = fhss_configuration->fhss_bc_dwell_interval;
    cur->ws_info->hopping_schdule.fhss_broadcast_interval = fhss_configuration->fhss_broadcast_interval;
    cur->ws_info->hopping_schdule.fhss_uc_dwell_interval = fhss_configuration->fhss_uc_dwell_interval;
    cur->ws_info->hopping_schdule.fhss_bsi = fhss_configuration->bsi;
}

static int8_t ws_fhss_initialize(protocol_interface_info_entry_t *cur)
{
    fhss_api_t *fhss_api = ns_sw_mac_get_fhss_api(cur->mac_api);

    if (!fhss_api) {
        // When FHSS doesn't exist yet, create one
        fhss_ws_configuration_t fhss_configuration;
        memset(&fhss_configuration, 0, sizeof(fhss_ws_configuration_t));
        ws_generate_channel_list(fhss_configuration.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

        // using bitwise AND operation for user set channel mask to remove channels not allowed in this device
        for (uint8_t n = 0; n < 8; n++) {
            fhss_configuration.channel_mask[n] &= cur->ws_info->fhss_channel_mask[n];
        }

        fhss_configuration.fhss_uc_dwell_interval = cur->ws_info->fhss_uc_dwell_interval;
        fhss_configuration.ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->fhss_uc_channel_function;
        fhss_configuration.ws_bc_channel_function = (fhss_ws_channel_functions)cur->ws_info->fhss_bc_channel_function;
        fhss_configuration.fhss_bc_dwell_interval = cur->ws_info->fhss_bc_dwell_interval;
        fhss_configuration.fhss_broadcast_interval = cur->ws_info->fhss_bc_interval;

        fhss_api = ns_fhss_ws_create(&fhss_configuration, cur->ws_info->fhss_timer_ptr);
        if (!fhss_api) {
            tr_error("fhss create failed");
            return -1;
        }
        ns_sw_mac_fhss_register(cur->mac_api, fhss_api);
    } else {
        // Read defaults from the configuration to help FHSS testing
        const fhss_ws_configuration_t *fhss_configuration = ns_fhss_ws_configuration_get(fhss_api);
        if (!fhss_configuration) {
            // no configuration set yet
            return 0;
        }
        memcpy(cur->ws_info->fhss_channel_mask, fhss_configuration->channel_mask, sizeof(uint32_t) * 8);
        cur->ws_info->fhss_uc_channel_function = fhss_configuration->ws_uc_channel_function;
        cur->ws_info->fhss_bc_channel_function = fhss_configuration->ws_bc_channel_function;
        cur->ws_info->fhss_bc_dwell_interval = fhss_configuration->fhss_bc_dwell_interval;
        cur->ws_info->fhss_bc_interval = fhss_configuration->fhss_broadcast_interval;
        cur->ws_info->fhss_uc_dwell_interval = fhss_configuration->fhss_uc_dwell_interval;
    }
    return 0;
}
static int8_t ws_fhss_set_defaults(protocol_interface_info_entry_t *cur, fhss_ws_configuration_t *fhss_configuration)
{
    fhss_configuration->fhss_uc_dwell_interval = cur->ws_info->fhss_uc_dwell_interval;
    fhss_configuration->ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->fhss_uc_channel_function;
    fhss_configuration->ws_bc_channel_function = (fhss_ws_channel_functions)cur->ws_info->fhss_bc_channel_function;
    fhss_configuration->fhss_bc_dwell_interval = cur->ws_info->fhss_bc_dwell_interval;
    fhss_configuration->fhss_broadcast_interval = cur->ws_info->fhss_bc_interval;
    if (cur->ws_info->fhss_uc_fixed_channel != 0xffff) {
        fhss_configuration->unicast_fixed_channel = cur->ws_info->fhss_uc_fixed_channel;
    }
    fhss_configuration->broadcast_fixed_channel = cur->ws_info->fhss_bc_fixed_channel;
    ws_generate_channel_list(fhss_configuration->channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

    // using bitwise AND operation for user set channel mask to remove channels not allowed in this device
    for (uint8_t n = 0; n < 8; n++) {
        fhss_configuration->channel_mask[n] &= cur->ws_info->fhss_channel_mask[n];
    }
    return 0;
}
static int8_t ws_fhss_border_router_configure(protocol_interface_info_entry_t *cur)
{
    // Read configuration of existing FHSS and start using the default values for any network
    fhss_ws_configuration_t fhss_configuration;
    memset(&fhss_configuration, 0, sizeof(fhss_ws_configuration_t));

    if (ns_fhss_ws_configuration_get(cur->ws_info->fhss_api)) {
        memcpy(&fhss_configuration, ns_fhss_ws_configuration_get(cur->ws_info->fhss_api), sizeof(fhss_ws_configuration_t));
    }
    ws_fhss_set_defaults(cur, &fhss_configuration);
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);

    return 0;
}

static uint16_t ws_randomize_fixed_channel(uint16_t configured_fixed_channel, uint8_t number_of_channels)
{
    if (configured_fixed_channel == 0xFFFF) {
        return randLIB_get_random_in_range(0, number_of_channels - 1);
    } else {
        return configured_fixed_channel;
    }
}

static int8_t ws_fhss_discovery_configure(protocol_interface_info_entry_t *cur)
{
    // Read configuration of existing FHSS and start using the default values for any network
    fhss_ws_configuration_t fhss_configuration;
    memset(&fhss_configuration, 0, sizeof(fhss_ws_configuration_t));

    if (ns_fhss_ws_configuration_get(cur->ws_info->fhss_api)) {
        memcpy(&fhss_configuration, ns_fhss_ws_configuration_get(cur->ws_info->fhss_api), sizeof(fhss_ws_configuration_t));
    }

    fhss_configuration.fhss_uc_dwell_interval = 0;
    fhss_configuration.ws_uc_channel_function = WS_FIXED_CHANNEL;
    fhss_configuration.ws_bc_channel_function = WS_FIXED_CHANNEL;
    fhss_configuration.fhss_bc_dwell_interval = 0;
    fhss_configuration.fhss_broadcast_interval = 0;
    uint8_t tmp_uc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels);
    uint8_t tmp_bc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels);
    fhss_configuration.unicast_fixed_channel = tmp_uc_fixed_channel;
    fhss_configuration.broadcast_fixed_channel = tmp_bc_fixed_channel;
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);

    return 0;
}

static int8_t ws_fhss_enable(protocol_interface_info_entry_t *cur)
{
    const fhss_ws_configuration_t *fhss_configuration = ns_fhss_ws_configuration_get(cur->ws_info->fhss_api);

    if (!cur->ws_info->fhss_api || !fhss_configuration) {
        return -1;
    }
    // Set the LLC information to follow the actual fhss settings
    ws_bootstrap_llc_hopping_update(cur, fhss_configuration);

    // Set neighbor info callback
    if (ns_fhss_set_neighbor_info_fp(cur->ws_info->fhss_api, &ws_get_neighbor_info)) {
        return -1;
    }
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        ns_fhss_ws_set_hop_count(cur->ws_info->fhss_api, 0);
    }
    return 0;
}

/* Sets the parent and broadcast schedule we are following
 *
 */
static void ws_bootstrap_primary_parent_set(struct protocol_interface_info_entry *cur, llc_neighbour_req_t *neighbor_info, ws_parent_synch_e synch_req)
{

    fhss_ws_configuration_t fhss_configuration;
    if (!neighbor_info->ws_neighbor->broadcast_timing_info_stored) {
        tr_error("No BC timing info for set new parent");
        return;
    }

    memcpy(&fhss_configuration, ns_fhss_ws_configuration_get(cur->ws_info->fhss_api), sizeof(fhss_ws_configuration_t));

    // Learning broadcast network configuration
    if (neighbor_info->ws_neighbor->broadcast_shedule_info_stored) {
        if (synch_req != WS_EAPOL_PARENT_SYNCH) {
            ws_fhss_set_defaults(cur, &fhss_configuration);
        }
        fhss_configuration.ws_bc_channel_function = (fhss_ws_channel_functions)neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_channel_function;
        if (fhss_configuration.ws_bc_channel_function == WS_FIXED_CHANNEL) {
            cur->ws_info->hopping_schdule.bc_fixed_channel = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.fixed_channel;
            cur->ws_info->fhss_bc_fixed_channel = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.fixed_channel;
        }
        fhss_configuration.bsi = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_schedule_id;
        fhss_configuration.fhss_bc_dwell_interval = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_dwell_interval;
        fhss_configuration.fhss_broadcast_interval = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_interval;
        fhss_configuration.broadcast_fixed_channel = cur->ws_info->fhss_bc_fixed_channel;
        neighbor_info->ws_neighbor->synch_done = true;
    }

    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);

    // We have broadcast schedule set up set the broadcast parent schedule
    ns_fhss_ws_set_parent(cur->ws_info->fhss_api, neighbor_info->neighbor->mac64, &neighbor_info->ws_neighbor->fhss_data.bc_timing_info, synch_req != WS_PARENT_SOFT_SYNCH);

    // Update LLC to follow updated fhss settings
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);
}

void ws_bootstrap_eapol_parent_synch(struct protocol_interface_info_entry *cur, llc_neighbour_req_t *neighbor_info)
{
    if (cur->ws_info->configuration_learned || !neighbor_info->ws_neighbor->broadcast_shedule_info_stored || !neighbor_info->ws_neighbor->broadcast_timing_info_stored) {
        return;
    }

    if (memcmp(neighbor_info->neighbor->mac64, cur->ws_info->parent_info.addr, 8)) {
        return;
    }

    //Store Brodacst Shedule
    if (!neighbor_info->ws_neighbor->synch_done) {
        ws_bootstrap_primary_parent_set(cur, neighbor_info, WS_EAPOL_PARENT_SYNCH);
    } else {
        ns_fhss_ws_set_parent(cur->ws_info->fhss_api, neighbor_info->neighbor->mac64, &neighbor_info->ws_neighbor->fhss_data.bc_timing_info, false);
    }
}

static void ws_bootstrap_ll_address_validate(struct protocol_interface_info_entry *cur)
{
    // Configure EUI64 for MAC if missing
    uint8_t mac64[8];
    if (!cur->mac_api) {
        return;
    }

    cur->mac_api->mac64_get(cur->mac_api, MAC_EXTENDED_DYNAMIC, mac64);

    if (memcmp(mac64, ADDR_UNSPECIFIED, 8) == 0) {
        cur->mac_api->mac64_get(cur->mac_api, MAC_EXTENDED_READ_ONLY, mac64);
    }

    if (memcmp(mac64, ADDR_UNSPECIFIED, 8) == 0) {
        // Generate random mac because it was not available
        randLIB_get_n_bytes_random(mac64, 8);
        mac64[0] |= 2; //Set Local Bit
        mac64[0] &= ~1; //Clear multicast bit

        tr_info("Generated random MAC %s", trace_array(mac64, 8));
    }
    mac_helper_mac64_set(cur, mac64);

    memcpy(cur->iid_eui64, mac64, 8);
    /* Invert U/L Bit */
    cur->iid_eui64[0] ^= 2;
    memcpy(cur->iid_slaac, cur->iid_eui64, 8);

}

/* \return 0x0100 to 0xFFFF ETX value (8 bit fraction)
 * \return 0xFFFF address not associated
 * \return 0x0000 address unknown or other error
 * \return 0x0001 no ETX statistics on this interface
 */
uint16_t ws_etx_read(protocol_interface_info_entry_t *interface, addrtype_t addr_type, const uint8_t *addr_ptr)
{

    uint16_t etx;
    if (!addr_ptr || !interface) {
        return 0;
    }

    uint8_t attribute_index;

    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), addr_ptr + PAN_ID_LEN, addr_type);
    if (!mac_neighbor) {
        return 0xffff;
    }
    attribute_index = mac_neighbor->index;
    ws_neighbor_class_entry_t *ws_neighbour = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, attribute_index);
    etx_storage_t *etx_entry = etx_storage_entry_get(interface->id, attribute_index);

    if (interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        if (!ws_neighbour || !etx_entry) {
            return 0xffff;
        }
    } else {

        if (!ws_neighbour || !etx_entry || etx_entry->etx_samples < 1 /*||
                !ws_neighbour->candidate_parent*/) {
            // if RSL value is not good enough candidate parent flag is removed and device not accepted as parent
            //tr_debug("ws_etx_read not valid params");
            return 0xffff;
        }

        //If we are not following gbobal Broadcast synch
        if (!interface->ws_info->pan_information.use_parent_bs) {
            //We must know both information's here
            if (!ws_neighbour->broadcast_shedule_info_stored ||
                    !ws_neighbour->broadcast_timing_info_stored) {
                return 0xffff;
            }
        } else {
            if (!ws_neighbour->broadcast_timing_info_stored) {
                //Global shedule is stored already
                tr_debug("ws_etx_read not valid NO BTI");
                return 0xffff;
            }
        }
    }

    etx = etx_local_etx_read(interface->id, attribute_index);
    if (etx == 0) {
        return 0xffff;
    }

    //tr_debug("ws_etx_read etx:%d", etx);
    return etx;
}
bool ws_bootstrap_nd_ns_transmit(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *entry,  bool unicast, uint8_t seq)
{
    (void)cur;
    (void)seq;

    if (unicast) {
        // Unicast NS is OK
        return false;
    }
    // Fail the resolution
    tr_warn("Link address lost for %s", trace_ipv6(entry->ip_address));
    ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, entry);
    // True means we skip the message sending
    return true;
}

static int8_t ws_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;

    if (!cur) {
        return -1;
    }

    if ((cur->configure_flags & INTERFACE_SETUP_MASK) != INTERFACE_SETUP_READY) {
        tr_error("Interface not yet fully configured");
        return -2;
    }
    if (ws_fhss_initialize(cur) != 0) {
        tr_error("fhss initialization failed");
        return -3;
    }

    // Save FHSS api
    cur->ws_info->fhss_api = ns_sw_mac_get_fhss_api(cur->mac_api);

    ws_bootstrap_ll_address_validate(cur);

    addr_interface_set_ll64(cur, NULL);
    cur->nwk_nd_re_scan_count = 0;
    //WS_interface_up(cur);
    // Trigger discovery for bootstrap
    ret_val = nwk_6lowpan_up(cur);
    if (ret_val) {
        goto cleanup;
    }

    /* Disable SLLAO send/mandatory receive with the ARO */
    cur->ipv6_neighbour_cache.use_eui64_as_slla_in_aro = true;
    /* Omit sending of NA if ARO SUCCESS */
    cur->ipv6_neighbour_cache.omit_na_aro_success = true;
    /* Omit sending of NA and consider ACK to be success */
    cur->ipv6_neighbour_cache.omit_na = true;
    // do not process AROs from NA. This is overriden by Wi-SUN specific failure handling
    cur->ipv6_neighbour_cache.recv_na_aro = false;
    /* Disable NUD Probes */
    cur->ipv6_neighbour_cache.send_nud_probes = false;
    cur->ipv6_neighbour_cache.probe_avoided_routers = true;
    /*Replace NS handler to disable multicast address queries */
    cur->if_ns_transmit = ws_bootstrap_nd_ns_transmit;

    dhcp_client_init(cur->id);
    dhcp_client_configure(cur->id, true, true, true); //RENEW uses SOLICIT, Interface will use 1 instance for address get, IAID address hint is not used.
    dhcp_client_solicit_timeout_set(cur->id, WS_DHCP_SOLICIT_TIMEOUT, WS_DHCP_SOLICIT_MAX_RT, WS_DHCP_SOLICIT_MAX_RC);


    ws_nud_table_reset(cur);

    blacklist_params_set(
        WS_BLACKLIST_ENTRY_LIFETIME,
        WS_BLACKLIST_TIMER_MAX_TIMEOUT,
        WS_BLACKLIST_TIMER_TIMEOUT,
        WS_BLACKLIST_ENTRY_MAX_NBR,
        WS_BLACKLIST_PURGE_NBR,
        WS_BLACKLIST_PURGE_TIMER_TIMEOUT);

    ws_bootstrap_event_discovery_start(cur);

    return 0;
cleanup:
    return ret_val;
}

static int8_t ws_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    tr_debug("Wi-SUN ifdown");

    // Reset WS information
    // ws_common_reset(cur)
    ws_llc_reset(cur);
    if (nd_proxy_downstream_interface_unregister(cur->id) != 0) {
        tr_warn("nd proxy unregister failed");
    }
    ws_nud_table_reset(cur);
    dhcp_client_delete(cur->id);
    ws_eapol_relay_delete(cur);
    ws_eapol_auth_relay_delete(cur);
    ws_pae_controller_stop(cur);
    blacklist_clear();

    return nwk_6lowpan_down(cur);
}

void ws_bootstrap_configuration_reset(protocol_interface_info_entry_t *cur)
{
    // Configure IP stack to operate as Wi-SUN node

    // Do not process beacons
    cur->mac_parameters->beacon_ind = NULL;
    cur->mac_parameters->mac_security_level = 0;

    // Set default parameters to interface
    cur->configure_flags = INTERFACE_BOOTSTRAP_DEFINED;
    cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
    cur->lowpan_info = 0;

    switch (cur->bootsrap_mode) {
        //        case NET_6LOWPAN_SLEEPY_HOST:
        case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST:
            break;

        case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER:
        case ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER:
            cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
            break;

        default:
            tr_err("Invalid bootstrap_mode");
    }

    cur->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    cur->ws_info->network_pan_id = 0xffff;
    cur->ws_info->trickle_pas_running = false;
    cur->ws_info->trickle_pa_running = false;
    cur->ws_info->trickle_pcs_running = false;
    cur->ws_info->trickle_pc_running = false;

    //cur->mac_security_key_usage_update_cb = ws_management_mac_security_key_update_cb;
    return;
}

static bool ws_bootstrap_network_name_matches(const struct mcps_data_ie_list *ie_ext, const char *network_name_ptr)
{
    if (!network_name_ptr || !ie_ext) {
        return false;
    }

    ws_wp_network_name_t network_name;
    if (!ws_wp_nested_network_name_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &network_name)) {
        tr_warn("No network name IE");
        return false;
    }

    if (network_name_ptr == NULL || strncmp(network_name_ptr, (char *)network_name.network_name, network_name.network_name_length) != 0) {
        return false;
    }
    return true;
}

static void ws_bootstrap_pan_advertisement_analyse_active(struct protocol_interface_info_entry *cur, ws_pan_information_t *pan_information)
{
    /* TODO In Active state
     *
     * A consistent transmission is defined as a PAN Advertisement received by a node with PAN ID and
     * NETNAME-IE / Network Name matching that of the receiving node, and with a PAN-IE / Routing Cost
     * the same or worse than (bigger than or equal to) that of the receiving node.
     *
     * Inconsistent:
     *
     * Received Routing Cost is smaller than stored one
     *
     * A PAN Advertisement received by a node with PAN ID and NETNAME-IE / Network name matching
     * that of the receiving node, and PAN-IE / Routing Cost better than (smaller than) that of the receiving node.
     *
     */

    if (pan_information->routing_cost >= cur->ws_info->pan_information.routing_cost) {
        trickle_consistent_heard(&cur->ws_info->trickle_pan_advertisement);
    } else {
        trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);
    }

    // automatic network size adjustment
    if (cur->ws_info->network_size_config == NETWORK_SIZE_AUTOMATIC &&
            cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER &&
            cur->ws_info->pan_information.pan_size != pan_information->pan_size) {
        ws_common_network_size_configure(cur, pan_information->pan_size);
    }
}

static void ws_bootstrap_pan_advertisement_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{

    //Validate Pan Conrfirmation is at packet
    ws_pan_information_t pan_information;
    if (!ws_wp_nested_pan_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &pan_information)) {
        // Corrupted
        tr_error("No pan information");
        return;
    }

    // if in active scan state
    if (!ws_bootstrap_state_discovery(cur)) {
        if (data->SrcPANId != cur->ws_info->network_pan_id) {
            return;
        }
    }


    // Check pan flags so that it is valid
    if (!pan_information.rpl_routing_method) {
        // NOT RPL routing
        tr_warn("Not supported routing");
        return;
    }

    /* TODO smart neighbour process
     *
     * Unsecure packet we cant trust the device?
     *
     * This message is received from tens of devices and we must select the best parent
     *
     * We save the best parent and create entry when we have selected the EAPOL target
     *
     */

    // Save route cost for all neighbours
    llc_neighbour_req_t neighbor_info;
    neighbor_info.neighbor = NULL;
    if (ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false)) {
        neighbor_info.ws_neighbor->routing_cost = pan_information.routing_cost;
    }

    // Save the best network parent

    if (ws_bootstrap_state_discovery(cur)) {
        // Discovery state processing
        tr_info("potential parent addr:%s panid:%x signal:%d", trace_array(data->SrcAddr, 8), data->SrcPANId, data->signal_dbm);

        // This parent is selected and used for authentication.
        if (memcmp(cur->ws_info->parent_info.addr, ADDR_UNSPECIFIED, 8) != 0) {

            // if we dont have higher than threshold signal only signal level decides parent
            if (ws_neighbor_class_rsl_from_dbm_calculate(cur->ws_info->parent_info.signal_dbm) < (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERISIS) &&
                    ws_neighbor_class_rsl_from_dbm_calculate(data->signal_dbm) > ws_neighbor_class_rsl_from_dbm_calculate(cur->ws_info->parent_info.signal_dbm)) {
                // automatically select the best quality link from the below threshold
                goto parent_selected;
            }
            // Drop if signal quality is not good enough
            if (ws_neighbor_class_rsl_from_dbm_calculate(data->signal_dbm) < (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERISIS)) {
                tr_info("EAPOL target dropped Link quality too low");
                return;
            }

            // Select the lowest PAN cost
            uint16_t pan_cost = (pan_information.routing_cost / PRC_WEIGHT_FACTOR) + (pan_information.pan_size / PS_WEIGHT_FACTOR);
            uint16_t current_pan_cost = (cur->ws_info->parent_info.pan_information.routing_cost / PRC_WEIGHT_FACTOR) + (cur->ws_info->parent_info.pan_information.pan_size / PS_WEIGHT_FACTOR);
            if (current_pan_cost < pan_cost) {
                tr_info("EAPOL target dropped Higher Pan cost %u > %u current", pan_cost, current_pan_cost);
                return;
            }

            // If pan cost is the same then we select the one we hear highest
            if (current_pan_cost == pan_cost &&
                    cur->ws_info->parent_info.signal_dbm > data->signal_dbm) {
                tr_info("EAPOL target dropped Lower link quality %d < %d current", data->signal_dbm, cur->ws_info->parent_info.signal_dbm);
                return;
            }
        }

parent_selected:
        // Parent valid store information
        cur->ws_info->parent_info.ws_utt = *ws_utt;
        // Saved from unicast IE
        cur->ws_info->parent_info.ws_us = *ws_us;

        // Saved from Pan information, do not overwrite pan_version as it is not valid here
        cur->ws_info->parent_info.pan_information.pan_size = pan_information.pan_size;
        cur->ws_info->parent_info.pan_information.routing_cost = pan_information.routing_cost;
        cur->ws_info->parent_info.pan_information.use_parent_bs = pan_information.use_parent_bs;
        cur->ws_info->parent_info.pan_information.rpl_routing_method = pan_information.rpl_routing_method;
        cur->ws_info->parent_info.pan_information.version = pan_information.version;

        // Saved from message
        cur->ws_info->parent_info.timestamp = data->timestamp;
        cur->ws_info->parent_info.pan_id = data->SrcPANId;
        cur->ws_info->parent_info.link_quality = data->mpduLinkQuality;
        cur->ws_info->parent_info.signal_dbm = data->signal_dbm;
        memcpy(cur->ws_info->parent_info.addr, data->SrcAddr, 8);

        tr_info("New parent addr:%s panid:%x signal:%d", trace_array(cur->ws_info->parent_info.addr, 8), cur->ws_info->parent_info.pan_id, cur->ws_info->parent_info.signal_dbm);
        return;
    }
    // Active state processing
    ws_bootstrap_pan_advertisement_analyse_active(cur, &pan_information);

    // Learn latest network information
    if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER && neighbor_info.neighbor) {
        uint8_t ll_address[16];
        ws_bootsrap_create_ll_address(ll_address, neighbor_info.neighbor->mac64);

        if (rpl_control_is_dodag_parent(cur, ll_address)) {
            cur->ws_info->pan_information.pan_size = pan_information.pan_size;
            cur->ws_info->pan_information.routing_cost = pan_information.routing_cost;
            cur->ws_info->pan_information.rpl_routing_method = pan_information.rpl_routing_method;
            cur->ws_info->pan_information.use_parent_bs = pan_information.use_parent_bs;
            cur->ws_info->pan_information.version = pan_information.version;
        }

    }
}

static void ws_bootstrap_pan_advertisement_solicit_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{

    (void)data;
    (void)ws_utt;
    (void)ws_us;
    /*
     * An inconsistent transmission is defined as:
     * A PAN Advertisement Solicit with NETNAME-IE matching that of the receiving node.
     */
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);
    /*
     *  A consistent transmission is defined as
     *  a PAN Advertisement Solicit with NETNAME-IE / Network Name matching that configured on the receiving node.
     */
    trickle_consistent_heard(&cur->ws_info->trickle_pan_advertisement_solicit);
}


static void ws_bootstrap_pan_config_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{

    uint16_t pan_version;
    ws_bs_ie_t ws_bs_ie;
    uint8_t *gtkhash_ptr;

    if (data->SrcPANId != cur->ws_info->network_pan_id) {
        return;
    }
    ws_bt_ie_t ws_bt_ie;
    if (!ws_wh_bt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_bt_ie)) {
        tr_warn("BT-IE");
        return;
    }

    /*
     * A consistent transmission is defined as a PAN Configuration with a PAN-ID matching that of the receiving node and
     * a PANVER-IE / PAN Version greater than or equal to the receiving node’s current PAN version.
     *
     * A inconsistent transmission is defined as:
     *
     *  A PAN Configuration with PAN-ID matching that of the receiving node and a
     *  PANVER-IE / PAN Version that is less than the receiving node’s current PAN version.
     */

    // TODO Add this to neighbor table
    // TODO save all information from config message if version number has changed

    if (!ws_wp_nested_pan_version_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &pan_version)) {
        // Corrupted
        tr_warn("no version");
        return;
    }

    gtkhash_ptr = ws_wp_nested_gtkhash_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength);

    if (!gtkhash_ptr) {
        // Corrupted
        tr_error("No gtk hash");
        return;
    }

    if (!ws_wp_nested_bs_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_bs_ie)) {
        // Corrupted
        tr_error("No broadcast schedule");
        return;
    }
    llc_neighbour_req_t neighbor_info;
    bool neighbour_pointer_valid;

    if (cur->ws_info->configuration_learned || cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //If we are border router or learned configuration we only update already learned neighbours.
        neighbour_pointer_valid = ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false);

    } else {
        neighbour_pointer_valid = ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, true);
        if (!neighbour_pointer_valid) {
            return;
        }
    }

    if (neighbour_pointer_valid) {
        etx_lqi_dbm_update(cur->id, data->mpduLinkQuality, data->signal_dbm, neighbor_info.neighbor->index);
        //Update Neighbor Broadcast and Unicast Parameters
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us);
        ws_neighbor_class_neighbor_broadcast_time_info_update(neighbor_info.ws_neighbor, &ws_bt_ie, data->timestamp);
        ws_neighbor_class_neighbor_broadcast_schedule_set(neighbor_info.ws_neighbor, &ws_bs_ie);
    }

    if (cur->ws_info->configuration_learned) {
        tr_info("PAN Config analyse own:%d, heard:%d", cur->ws_info->pan_information.pan_version, pan_version);
        if (cur->ws_info->pan_information.pan_version == pan_version) {
            // Same version heard so it is consistent
            trickle_consistent_heard(&cur->ws_info->trickle_pan_config);
            if (neighbour_pointer_valid && neighbor_info.neighbor->link_role == PRIORITY_PARENT_NEIGHBOUR) {
                ws_bootstrap_primary_parent_set(cur, &neighbor_info, WS_PARENT_SOFT_SYNCH);
            }
            // no need to process more
            return;
        } else  {
            // received version is different so we need to reset the trickle
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
            if (neighbour_pointer_valid && neighbor_info.neighbor->link_role == PRIORITY_PARENT_NEIGHBOUR) {
                ws_bootstrap_primary_parent_set(cur, &neighbor_info, WS_PARENT_HARD_SYNCH);
            }
            if (common_serial_number_greater_16(cur->ws_info->pan_information.pan_version, pan_version)) {
                // older version heard ignoring the message
                return;
            }
        }
    }

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //Border router does not learn network information
        return;
    }

    /*
     * Learn new information from neighbor
     */
    tr_info("Updated PAN configuration own:%d, heard:%d", cur->ws_info->pan_information.pan_version, pan_version);

    // restart PAN version timer
    cur->ws_info->pan_version_timeout_timer = ws_common_version_timeout_get(cur->ws_info->network_size_config);
    cur->ws_info->pan_information.pan_version = pan_version;

    ws_pae_controller_gtk_hash_update(cur, gtkhash_ptr);

    ws_pae_controller_nw_key_index_update(cur, data->Key.KeyIndex - 1);

    if (!cur->ws_info->configuration_learned) {
        // Generate own hopping schedules Follow first parent broadcast and plans and also use same unicast dwell
        tr_info("learn network configuration");
        cur->ws_info->configuration_learned = true;
        // return to state machine after 1-2 s
        cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(10, 20);
        // enable frequency hopping for unicast channel and start listening first neighbour
        ws_bootstrap_primary_parent_set(cur, &neighbor_info, WS_PARENT_HARD_SYNCH);
        // set neighbor as priority parent clear if there is others
        protocol_6lowpan_neighbor_priority_clear_all(cur->id, PRIORITY_1ST);
        neighbor_info.neighbor->link_role = PRIORITY_PARENT_NEIGHBOUR;
    }
}

static void ws_bootstrap_pan_config_solicit_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{
    if (data->SrcPANId != cur->ws_info->network_pan_id) {
        return;
    }

    /* TODO smart neighbour process
     *
     * Unsecure packet we cant trust the device?
     *
     * Question mark in specification also present, now we create neighbour.
     * this is moved in future to NS/ND processing triggered by RPL
     *
     */

    llc_neighbour_req_t neighbor_info;
    if (ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false)) {
        etx_lqi_dbm_update(cur->id, data->mpduLinkQuality, data->signal_dbm, neighbor_info.neighbor->index);
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us);
    }



    /*
     * A consistent transmission is defined as a PAN Configuration Solicit with
     * a PAN-ID matching that of the receiving node and a NETNAME-IE / Network Name
     * matching that configured on the receiving node.
     */
    trickle_consistent_heard(&cur->ws_info->trickle_pan_config_solicit);
    /*
     *  inconsistent transmission is defined as either:
     *  A PAN Configuration Solicit with a PAN-ID matching that of the receiving node and
     *  a NETNAME-IE / Network Name matching the network name configured on the receiving
     */
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
}
static bool ws_bootstrap_network_found(protocol_interface_info_entry_t *cur)
{
    tr_debug("analyze network discovery result");

    // This parent is used for authentication to the network
    if (memcmp(cur->ws_info->parent_info.addr, ADDR_UNSPECIFIED, 8) == 0) {
        // No parent found yet
        return false;
    }
    return true;
}

static bool ws_channel_plan_zero_compare(ws_channel_plan_zero_t *rx_plan, ws_hopping_schedule_t *hopping_schdule)
{
    if (rx_plan->operation_class != hopping_schdule->operating_class) {
        return false;
    } else if (rx_plan->regulator_domain != hopping_schdule->regulatory_domain) {
        return false;
    }
    return true;
}

static bool ws_channel_plan_one_compare(ws_channel_plan_one_t *rx_plan, ws_hopping_schedule_t *hopping_schdule)
{
    uint16_t num_of_channel = hopping_schdule->number_of_channels;
    if (rx_plan->ch0 != hopping_schdule->ch0_freq) {
        return false;
    } else if (rx_plan->channel_spacing != hopping_schdule->channel_spacing) {
        return false;
    } else if (rx_plan->number_of_channel != num_of_channel) {
        return false;
    }
    return true;
}




static void ws_bootstrap_asynch_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type)
{

    if (data->SrcAddrMode != MAC_ADDR_MODE_64_BIT) {
        // Not from long address
        return;
    }
    //Validate network name
    switch (message_type) {
        case WS_FT_PAN_ADVERT:
        case WS_FT_PAN_ADVERT_SOL:
        case WS_FT_PAN_CONF_SOL:
            //Check Network Name
            if (!ws_bootstrap_network_name_matches(ie_ext, cur->ws_info->network_name)) {
                // Not in our network
                return;
            }
            break;
        case WS_FT_PAN_CONF:
            break;
        default:
            return;
    }
    ws_stats_update(cur, STATS_WS_ASYNCH_RX, 1);
    //UTT-IE and US-IE are mandatory for all Asynch Messages
    ws_utt_ie_t ws_utt;
    if (!ws_wh_utt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_utt)) {
        // Corrupted
        return;
    }

    ws_us_ie_t ws_us;
    if (!ws_wp_nested_us_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_us)) {
        // Corrupted
        return;
    }

    //Compare Unicast channel Plan
    if (ws_us.channel_plan != cur->ws_info->hopping_schdule.channel_plan) {
        return;
    }

    if (ws_us.channel_plan == 0) {
        if (!ws_channel_plan_zero_compare(&ws_us.plan.zero, &cur->ws_info->hopping_schdule)) {
            return;
        }
    } else if (ws_us.channel_plan == 1) {
        if (!ws_channel_plan_one_compare(&ws_us.plan.one, &cur->ws_info->hopping_schdule)) {
            return;
        }
    }

    //Handle Message's
    switch (message_type) {
        case WS_FT_PAN_ADVERT:
            // Analyse Advertisement
            tr_info("received ADVERT Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_advertisement_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_ADVERT_SOL:
            tr_info("received ADVERT SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_advertisement_solicit_analyse(cur, data, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF:
            tr_info("received CONFIG Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_config_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        default:
            tr_info("received CONFIG SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_config_solicit_analyse(cur, data, &ws_utt, &ws_us);
            break;
    }
}

static void ws_bootstrap_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message)
{
    ws_stats_update(interface, STATS_WS_ASYNCH_TX, 1);
    (void)interface;
    (void)asynch_message;
}

uint32_t ws_time_from_last_unicast_traffic(uint32_t current_time_stamp, ws_neighbor_class_entry_t *ws_neighbor)
{
    uint32_t time_from_last_unicast_shedule = current_time_stamp;

    //Time from last RX unicast in us
    time_from_last_unicast_shedule -= ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp;
    time_from_last_unicast_shedule /= 1000000; //Convert to seconds
    return time_from_last_unicast_shedule;
}

static void ws_bootstrap_neighbor_table_clean(struct protocol_interface_info_entry *interface)
{
    uint8_t ll_target[16];

    if (mac_neighbor_info(interface)->neighbour_list_size <= mac_neighbor_info(interface)->list_total_size - WS_NON_CHILD_NEIGHBOUR_COUNT) {
        // Enough neighbor entries
        return;
    }
    memcpy(ll_target, ADDR_LINK_LOCAL_PREFIX, 8);

    uint32_t current_time_stamp = ns_sw_mac_read_current_timestamp(interface->mac_api);

    mac_neighbor_table_entry_t *neighbor_entry_ptr = NULL;
    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur, &mac_neighbor_info(interface)->neighbour_list) {
        ws_neighbor_class_entry_t *ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, cur->index);

        if (cur->link_role == PRIORITY_PARENT_NEIGHBOUR) {
            //This is our primary parent we cannot delete
            continue;
        }

        if (cur->nud_active || ws_neighbor->accelerated_etx_probe || ws_neighbor->negative_aro_send) {
            //If NUD process is active do not trig
            continue;
        }

        if (neighbor_entry_ptr && neighbor_entry_ptr->lifetime < cur->lifetime) {
            // We have already shorter link entry found this cannot replace it
            continue;
        }

        if (cur->trusted_device) {

            if (ipv6_neighbour_has_registered_by_eui64(&interface->ipv6_neighbour_cache, cur->mac64)) {
                // We have registered entry so we have been selected as parent
                continue;
            }

            memcpy(ll_target + 8, cur->mac64, 8);
            ll_target[8] ^= 2;

            if (rpl_control_is_dodag_parent(interface, ll_target)) {
                // Possible parent is limited to 3 by default?
                continue;
            }
        }

        //Read current timestamp
        uint32_t time_from_last_unicast_shedule = ws_time_from_last_unicast_traffic(current_time_stamp, ws_neighbor);

        if (time_from_last_unicast_shedule > WS_NEIGHBOR_TEMPORARY_LINK_MIN_TIMEOUT || !ws_neighbor->unicast_data_rx) {
            //Accept only Enough Old Device
            if (!neighbor_entry_ptr) {
                //Accept first compare
                neighbor_entry_ptr = cur;
            } else {
                uint32_t compare_neigh_time = ws_time_from_last_unicast_traffic(current_time_stamp, ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor_entry_ptr->index));
                if (compare_neigh_time < time_from_last_unicast_shedule)  {
                    //Accept older RX timeout allways
                    neighbor_entry_ptr = cur;
                }
            }
        }
    }
    if (neighbor_entry_ptr) {
        tr_info("dropped oldest neighbour %s", trace_array(neighbor_entry_ptr->mac64, 8));
        mac_neighbor_table_neighbor_remove(mac_neighbor_info(interface), neighbor_entry_ptr);
    }

}

static bool ws_bootstrap_neighbor_info_request(struct protocol_interface_info_entry *interface, const uint8_t *mac_64, llc_neighbour_req_t *neighbor_buffer, bool request_new)
{
    neighbor_buffer->ws_neighbor = NULL;
    neighbor_buffer->neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), mac_64, ADDR_802_15_4_LONG);
    if (neighbor_buffer->neighbor) {
        neighbor_buffer->ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor_buffer->neighbor->index);
        if (!neighbor_buffer->ws_neighbor) {
            return false;
        }
        return true;
    }
    if (!request_new) {
        return false;
    }

    uint8_t ll_target[16];
    ws_bootsrap_create_ll_address(ll_target, mac_64);

    if (blacklist_reject(ll_target)) {
        // Rejected by blacklist
        return false;
    }

    ws_bootstrap_neighbor_table_clean(interface);

    neighbor_buffer->neighbor = ws_bootstrap_mac_neighbor_add(interface, mac_64);

    if (!neighbor_buffer->neighbor) {
        return false;
    }

    neighbor_buffer->ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor_buffer->neighbor->index);
    if (!neighbor_buffer->ws_neighbor) {
        mac_neighbor_table_neighbor_remove(mac_neighbor_info(interface), neighbor_buffer->neighbor);
        return false;
    }
    return true;
}

static bool ws_rpl_dio_new_parent_accept(struct protocol_interface_info_entry *interface)
{
    uint16_t parent_candidate_size = rpl_control_parent_candidate_list_size(interface, false);
    //TODO check bootstarap state for review
    //if we have enough candidates at list do not accept new multicast neighbours
    if (parent_candidate_size > WS_NEIGHBOUR_MAX_CANDIDATE_PROBE) {
        return false;
    }

    parent_candidate_size = rpl_control_parent_candidate_list_size(interface, true);
    //If we have already enough parent selected Candidates count is bigger tahn 4
    if (parent_candidate_size >= 2) {
        return false;
    }

    return true;
}


static void ws_neighbor_entry_remove_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{

    protocol_interface_info_entry_t *cur = user_data;
    lowpan_adaptation_remove_free_indirect_table(cur, entry_ptr);
    // Sleepy host
    if (cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        mac_data_poll_protocol_poll_mode_decrement(cur);
    }

    //TODO State machine check here

    if (entry_ptr->ffd_device) {
        protocol_6lowpan_release_short_link_address_from_neighcache(cur, entry_ptr->mac16);
        protocol_6lowpan_release_long_link_address_from_neighcache(cur, entry_ptr->mac64);
    }

    //NUD Process Clear Here
    ws_nud_entry_remove(cur, entry_ptr);

    ws_bootstrap_neighbor_delete(cur, entry_ptr);
}

static bool ws_neighbor_entry_nud_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{
    uint32_t time_from_start = entry_ptr->link_lifetime - entry_ptr->lifetime;
    bool activate_nud = false;
    protocol_interface_info_entry_t *cur = user_data;

    ws_neighbor_class_entry_t *ws_neighbor = ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, entry_ptr->index);
    etx_storage_t *etx_entry = etx_storage_entry_get(cur->id, entry_ptr->index);

    if (!entry_ptr->trusted_device || !ws_neighbor || !etx_entry || ws_neighbor->negative_aro_send) {
        return false;
    }

    uint8_t ll_address[16];

    if (time_from_start > WS_NEIGHBOR_NUD_TIMEOUT) {

        ws_bootsrap_create_ll_address(ll_address, entry_ptr->mac64);

        if (!rpl_control_is_dodag_parent_candidate(cur, ll_address, WS_NEIGHBOUR_MAX_CANDIDATE_PROBE)) {
            if (!ipv6_neighbour_has_registered_by_eui64(&cur->ipv6_neighbour_cache, entry_ptr->mac64)) {
                //NUD Not needed for if neighbour is not child or parent candidate
                return false;
            }
        }

        if (time_from_start > WS_NEIGHBOR_NUD_TIMEOUT * 1.5) {
            activate_nud = true;
        } else {
            uint16_t switch_prob = randLIB_get_random_in_range(0, WS_NUD_RANDOM_SAMPLE_LENGTH - 1);
            //Take Random from time WS_NEIGHBOR_NUD_TIMEOUT - WS_NEIGHBOR_NUD_TIMEOUT*1.5
            if (switch_prob < WS_NUD_RANDOM_COMPARE) {
                activate_nud = true;
            }
        }
    } else if (etx_entry->etx_samples < WS_NEIGBOR_ETX_SAMPLE_MAX) {
        //Take Random number for trig a prope.
        //ETX Sample 0: random 1-8
        //ETX Sample 1: random 2-16
        //ETX Sample 2: random 4-32
        if (etx_entry->etx_samples == 0 && ws_neighbor->accelerated_etx_probe) {
            //Accept quick Probe for init ETX
            activate_nud = true;
        } else {

            ws_bootsrap_create_ll_address(ll_address, entry_ptr->mac64);
            if (!rpl_control_is_dodag_parent_candidate(cur, ll_address, WS_NEIGHBOUR_MAX_CANDIDATE_PROBE)) {
                return false;
            }

            uint32_t probe_period = WS_PROBE_INIT_BASE_SECONDS << etx_entry->etx_samples;
            uint32_t time_block = 1 << etx_entry->etx_samples;
            if (time_from_start >= probe_period) {
                //tr_debug("Link Probe test %u Sample trig", etx_entry->etx_samples);
                activate_nud = true;
            } else if (time_from_start > time_block) {
                uint16_t switch_prob = randLIB_get_random_in_range(0, probe_period - 1);
                //Take Random from time WS_NEIGHBOR_NUD_TIMEOUT - WS_NEIGHBOR_NUD_TIMEOUT*1.5
                if (switch_prob < 2) {
                    //tr_debug("Link Probe test with jitter %"PRIu32", sample %u", time_from_start, etx_entry->etx_samples);
                    activate_nud = true;
                }
            }
        }
    }

    if (!activate_nud) {
        return false;
    }

    ws_nud_table_entry_t *entry = ws_nud_entry_get_free(cur);
    if (!entry) {
        return false;
    }
    entry->neighbor_info = entry_ptr;
    if (ws_neighbor->accelerated_etx_probe) {
        ws_neighbor->accelerated_etx_probe = false;
        entry->timer = 1;
    }

    if (etx_entry->etx_samples >= WS_NEIGBOR_ETX_SAMPLE_MAX) {
        entry->nud_process = true;
    }
    return true;
}

int ws_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode)
{
    int ret_val = 0;

    ws_neighbor_class_t neigh_info;
    neigh_info.neigh_info_list = NULL;
    neigh_info.list_size = 0;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    mac_description_storage_size_t buffer;
    if (!cur->mac_api || !cur->mac_api->mac_storage_sizes_get || cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
        return -2;
    }

    if (buffer.key_description_table_size < 4) {
        tr_err("MAC key_description_table_size too short %d<4", buffer.key_description_table_size);
        return -2;
    }

    if (!etx_storage_list_allocate(cur->id, buffer.device_decription_table_size)) {
        return -1;
    }
    if (!etx_cached_etx_parameter_set(WS_ETX_MIN_WAIT_TIME, WS_ETX_MIN_SAMPLE_COUNT)) {
        etx_storage_list_allocate(cur->id, 0);
        return -1;
    }

    etx_max_update_set(WS_ETX_MAX_UPDATE);

    if (blacklist_init() != 0) {
        tr_err("MLE blacklist init failed.");
        return -1;
    }

    switch (bootstrap_mode) {
        //        case NET_6LOWPAN_SLEEPY_HOST:
        case NET_6LOWPAN_HOST:
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST;
            break;
        case NET_6LOWPAN_ROUTER:
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER;
            break;
        case NET_6LOWPAN_BORDER_ROUTER:
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER;
            break;
        default:
            return -3;
    }

    if (!ws_neighbor_class_alloc(&neigh_info, buffer.device_decription_table_size)) {
        ret_val = -1;
        goto init_fail;
    }

    //Disable allways by default
    lowpan_adaptation_interface_mpx_register(interface_id, NULL, 0);

    mac_neighbor_table_delete(mac_neighbor_info(cur));
    mac_neighbor_info(cur) = mac_neighbor_table_create(buffer.device_decription_table_size, ws_neighbor_entry_remove_notify
                                                       , ws_neighbor_entry_nud_notify, cur);
    if (!mac_neighbor_info(cur)) {
        ret_val = -1;
        goto init_fail;
    }

    ws_llc_create(cur, &ws_bootstrap_asynch_ind, &ws_bootstrap_asynch_confirm, &ws_bootstrap_neighbor_info_request);

    mpx_api_t *mpx_api = ws_llc_mpx_api_get(cur);
    if (!mpx_api) {
        ret_val =  -4;
        goto init_fail;
    }

    if (ws_common_allocate_and_init(cur) < 0) {
        ret_val =  -4;
        goto init_fail;
    }

    if (ws_bootstrap_tasklet_init(cur) != 0) {
        ret_val =  -4;
        goto init_fail;
    }

    //Register MPXUser to adapatation layer
    if (lowpan_adaptation_interface_mpx_register(interface_id, mpx_api, MPX_LOWPAN_ENC_USER_ID) != 0) {
        ret_val =  -4;
        goto init_fail;
    }

    //Init PAE controller and set callback
    if (ws_pae_controller_init(cur) < 0) {
        ret_val =  -4;
        goto init_fail;
    }
    if (ws_pae_controller_cb_register(cur, &ws_bootstrap_authentication_completed, &ws_bootstrap_nw_key_set, &ws_bootstrap_nw_key_clear, &ws_bootstrap_nw_key_index_set, &ws_bootstrap_nw_frame_counter_set, &ws_bootstrap_nw_frame_counter_read, &ws_bootstrap_pan_version_increment) < 0) {
        ret_val =  -4;
        goto init_fail;
    }

    //Init EAPOL PDU handler and register it to MPX
    if (ws_eapol_pdu_init(cur) < 0) {
        ret_val =  -4;
        goto init_fail;
    }
    if (ws_eapol_pdu_mpx_register(cur, mpx_api, MPX_KEY_MANAGEMENT_ENC_USER_ID != 0)) {
        ret_val =  -4;
        // add deallocs
        goto init_fail;
    }

    cur->if_up = ws_bootstrap_up;
    cur->if_down = ws_bootstrap_down;
    cur->ws_info->neighbor_storage = neigh_info;
    cur->etx_read_override = ws_etx_read;

    ws_bootstrap_configuration_reset(cur);
    addr_notification_register(ws_bootstrap_address_notification_cb);
    //Enable MAC Security by pass
    mlme_set_t set_req;
    bool state = true;
    set_req.attr = macAcceptByPassUnknowDevice;
    set_req.attr_index = 0;
    set_req.value_pointer = &state;
    set_req.value_size = sizeof(bool);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);

    // Set the default parameters for MPL
    cur->mpl_proactive_forwarding = true;

    cur->mpl_data_trickle_params.Imin = MPL_MS_TO_TICKS(DATA_MESSAGE_IMIN);
    cur->mpl_data_trickle_params.Imax = MPL_MS_TO_TICKS(DATA_MESSAGE_IMAX);
    cur->mpl_data_trickle_params.TimerExpirations = DATA_MESSAGE_TIMER_EXPIRATIONS;
    cur->mpl_data_trickle_params.k = 8;

    // Specification is ruling out the compression mode, but we are now doing it.
    cur->mpl_seed = true;
    cur->mpl_seed_id_mode = MULTICAST_MPL_SEED_ID_IPV6_SRC_FOR_DOMAIN;
    cur->mpl_seed_set_entry_lifetime = MPL_SEED_SET_ENTRY_TIMEOUT;

    cur->mpl_control_trickle_params.TimerExpirations = 0;

    mpl_domain_create(cur, ADDR_ALL_MPL_FORWARDERS, NULL, MULTICAST_MPL_SEED_ID_DEFAULT, -1, 0, NULL, NULL);
    addr_add_group(cur, ADDR_REALM_LOCAL_ALL_NODES);
    addr_add_group(cur, ADDR_REALM_LOCAL_ALL_ROUTERS);

    return 0;

    //Error handling and free memory
init_fail:
    lowpan_adaptation_interface_mpx_register(interface_id, NULL, 0);
    ws_eapol_pdu_mpx_register(cur, NULL, 0);
    mac_neighbor_table_delete(mac_neighbor_info(cur));
    etx_storage_list_allocate(cur->id, 0);
    ws_neighbor_class_dealloc(&neigh_info);
    ws_llc_delete(cur);
    ws_eapol_pdu_delete(cur);
    ws_pae_controller_delete(cur);
    return ret_val;
}

int ws_bootstrap_restart(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->ws_info) {
        return -1;
    }
    ws_bootstrap_event_discovery_start(cur);
    return 0;
}

int ws_bootstrap_set_rf_config(protocol_interface_info_entry_t *cur, phy_rf_channel_configuration_s rf_configs)
{
    mlme_set_t set_request;
    // Set RF configuration
    set_request.attr = macRfConfiguration;
    set_request.value_pointer = &rf_configs;
    set_request.value_size = sizeof(phy_rf_channel_configuration_s);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_request);
    // Set Ack wait duration
    uint16_t ack_wait_symbols = WS_ACK_WAIT_SYMBOLS + (WS_TACK_MAX_MS * (rf_configs.datarate / 1000));
    set_request.attr = macAckWaitDuration;
    set_request.value_pointer = &ack_wait_symbols;
    set_request.value_size = sizeof(ack_wait_symbols);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_request);
    // Set multi CSMA-CA configuration
    mlme_multi_csma_ca_param_t multi_csma_params = {WS_NUMBER_OF_CSMA_PERIODS, WS_CSMA_MULTI_CCA_INTERVAL};
    set_request.attr = macMultiCSMAParameters;
    set_request.value_pointer = &multi_csma_params;
    set_request.value_size = sizeof(mlme_multi_csma_ca_param_t);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_request);
    return 0;
}

int ws_bootstrap_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address)
{
    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), ll_address, false, NULL);

    if (mac_neighbor) {
        mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), mac_neighbor);
    }
    return 0;
}

int ws_bootstrap_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address)
{
    blacklist_update(ll_address, false);
    rpl_control_neighbor_delete(cur, ll_address);
    ws_bootstrap_neighbor_remove(cur, ll_address);
    return 0;
}

static void ws_bootstrap_mac_activate(protocol_interface_info_entry_t *cur, uint16_t channel, uint16_t panid, bool coordinator)
{
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));

    cur->mac_parameters->pan_id = panid;
    cur->mac_parameters->mac_channel = channel;

    start_req.PANId = panid;
    start_req.LogicalChannel = channel;
    start_req.BeaconOrder = 0x0f;
    start_req.SuperframeOrder = 0x0f;
    start_req.PANCoordinator = coordinator;

    if (cur->mac_api) {
        cur->mac_api->mlme_req(cur->mac_api, MLME_START, (void *)&start_req);
    }
}

static void ws_bootstrap_fhss_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("FHSS activate");
    ws_fhss_enable(cur);
    ws_llc_hopping_schedule_config(cur, &cur->ws_info->hopping_schdule);
    // Only supporting fixed channel

    tr_debug("MAC init");
    mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
    cur->lowpan_info &=  ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    ws_bootstrap_mac_security_enable(cur);
    ws_bootstrap_mac_activate(cur, cur->ws_info->fhss_uc_fixed_channel, cur->ws_info->network_pan_id, true);
    return;
}

static void ws_bootstrap_network_information_learn(protocol_interface_info_entry_t *cur)
{
    tr_debug("learn network information from parent");

    // Start following network broadcast timing schedules

    // Regulatory domain saving? cant change?

    // Save network information
    cur->ws_info->network_pan_id = cur->ws_info->parent_info.pan_id;
    cur->ws_info->pan_information = cur->ws_info->parent_info.pan_information;
    cur->ws_info->pan_information.pan_version = 0; // This is learned from actual configuration

    // TODO create parent neighbour table entry for unicast schedule to enable authentication

    return;
}
static void ws_bootstrap_network_configuration_learn(protocol_interface_info_entry_t *cur)
{
    tr_debug("Start using PAN configuration");

    // Timing information can be modified here
    ws_llc_set_pan_information_pointer(cur, &cur->ws_info->pan_information);
    uint8_t *gtkhash = ws_pae_controller_gtk_hash_ptr_get(cur);
    ws_llc_set_gtkhash(cur, gtkhash);
    // TODO update own fhss schedules we are starting to follow first parent

    return;
}

static void ws_bootstrap_ip_stack_addr_clear(protocol_interface_info_entry_t *cur)
{
    tr_debug("ip stack address clear");
    ns_list_foreach_safe(if_address_entry_t, addr, &cur->ip_addresses) {
        if (addr->source != ADDR_SOURCE_STATIC &&
                addr_ipv6_scope(addr->address, cur) > IPV6_SCOPE_LINK_LOCAL) {
            // Remove all exept User set address
            addr_delete_entry(cur, addr);
        }
    }
}

static void ws_bootstrap_ip_stack_reset(protocol_interface_info_entry_t *cur)
{
    tr_debug("ip stack reset");
    // Delete all temporary cached information
    ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);
    lowpan_context_list_free(&cur->lowpan_contexts);
}

static void ws_bootstrap_ip_stack_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("ip stack init");
    clear_power_state(ICMP_ACTIVE);
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;
    ws_bootstrap_ip_stack_reset(cur);
}

static void ws_set_fhss_hop(protocol_interface_info_entry_t *cur)
{
    uint16_t own_rank = ws_bootstrap_rank_get(cur);
    uint16_t rank_inc = ws_bootstrap_min_rank_inc_get(cur);
    if (own_rank == 0xffff || rank_inc == 0xffff) {
        return;
    }
    // Calculate own hop count. This method gets inaccurate when hop count increases.
    uint8_t own_hop = (own_rank - rank_inc) / rank_inc;
    ns_fhss_ws_set_hop_count(cur->ws_info->fhss_api, own_hop);
    tr_debug("own hop: %u, own rank: %u, rank inc: %u", own_hop, own_rank, rank_inc);
}

static void ws_address_registration_update(protocol_interface_info_entry_t *interface)
{
    rpl_control_register_address(interface, NULL);
    tr_info("RPL parent update ... register ARO");
}

static void ws_bootstrap_rpl_callback(rpl_event_t event, void *handle)
{

    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return;
    }
    // if waiting for RPL and
    if (event == RPL_EVENT_DAO_DONE) {
        // Trigger statemachine check
        cur->bootsrap_state_machine_cnt = 1;
        rpl_dodag_info_t dodag_info;
        struct rpl_instance *instance = rpl_control_enumerate_instances(cur->rpl_domain, NULL);

        if (instance && rpl_control_read_dodag_info(instance, &dodag_info)) {
            tr_debug("Enable DHCPv6 relay");
            dhcp_relay_agent_enable(cur->id, dodag_info.dodag_id);

            tr_debug("Start EAPOL relay");
            // Set both own port and border router port to 10253
            ws_eapol_relay_start(cur, EAPOL_RELAY_SOCKET_PORT, dodag_info.dodag_id, EAPOL_RELAY_SOCKET_PORT);
            // Set network information to PAE
            ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->network_name);
            // Network key is valid
            ws_pae_controller_nw_key_valid(cur);
        }

        ws_set_fhss_hop(cur);

    } else if (event == RPL_EVENT_LOCAL_REPAIR_NO_MORE_DIS) {
        /*
         * RPL goes to passive mode, but does not require any extra changed
         *
         * We could remove our current addresses learned from RPL
         * We could send solicit for configuration and then select new parent when those arrive
         *
         */

    } else if (event == RPL_EVENT_DAO_PARENT_SWITCH) {
        ws_address_registration_update(cur);
    }
    cur->ws_info->rpl_state = event;
    tr_info("RPL event %d", event);
}

static void ws_dhcp_client_global_adress_cb(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status)
{
    (void)prefix;
    (void)interface;
    //TODO add handler for negative status
    tr_debug("DHCPv6 %s status %u with link %s", trace_ipv6(prefix), register_status, trace_ipv6(dhcp_addr));
    if (register_status) {
        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface);
        if (cur) {
            rpl_control_register_address(cur, prefix);
        }
    }
}


void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local)
{
    if (dhcp_client_get_global_address(cur->id, parent_link_local, prefix, cur->mac, DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE, ws_dhcp_client_global_adress_cb) != 0) {
        tr_error("DHCPp client request fail");
    }
}

void ws_dhcp_client_address_delete(protocol_interface_info_entry_t *cur, uint8_t *prefix)
{
    dhcp_client_global_address_delete(cur->id, NULL, prefix);
}

bool ws_eapol_relay_state_active(protocol_interface_info_entry_t *cur)
{
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER || cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
        return true;
    }

    return false;
}

static void ws_rpl_prefix_callback(prefix_entry_t *prefix, void *handle, uint8_t *parent_link_local)
{
    protocol_interface_info_entry_t *cur = (protocol_interface_info_entry_t *) handle;
    /* Check if A-Flag.
     * A RPL node may use this option for the purpose of Stateless Address Autoconfiguration (SLAAC)
     * from a prefix advertised by a parent.
     */
    if (prefix->options & PIO_A) {

        if (parent_link_local) {
            if (icmpv6_slaac_prefix_update(cur, prefix->prefix, prefix->prefix_len, prefix->lifetime, prefix->preftime) != 0) {
                ipv6_interface_slaac_handler(cur, prefix->prefix, prefix->prefix_len, prefix->lifetime, prefix->preftime);
                /*
                 * Give SLAAC addresses a different label and low precedence to indicate that
                 * they probably shouldn't be used for external traffic. SLAAC use in Wi-SUN is non-standard,
                 * and we use it for mesh-local traffic we should prefer any DHCP-assigned addresses
                 * for talking to the outside world
                 *
                 */
                addr_policy_table_add_entry(prefix->prefix, prefix->prefix_len, 2, WS_NON_PREFFRED_LABEL);
            }
        } else {
            icmpv6_slaac_prefix_update(cur, prefix->prefix, prefix->prefix_len, 0, 0);
        }
    } else if (prefix->prefix_len) {
        // Create new address using DHCP
        if (parent_link_local) {
            ws_dhcp_client_address_request(cur, prefix->prefix, parent_link_local);
        } else {
            /* Deprecate address and remove client */
            tr_debug("Prefix invalidation %s", trace_ipv6(prefix->prefix));
            dhcp_client_global_address_delete(cur->id, NULL, prefix->prefix);
        }
    }
}

static bool ws_rpl_new_parent_callback_t(uint8_t *ll_parent_address, void *handle)
{

    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return false;
    }

    uint8_t mac64[8];
    memcpy(mac64, ll_parent_address + 8, 8);
    mac64[0] ^= 2;
    llc_neighbour_req_t neigh_buffer;
    if (ws_bootstrap_neighbor_info_request(cur, mac64, &neigh_buffer, false)) {
        return true;
    }

    if (!ws_rpl_dio_new_parent_accept(cur)) {
        return false;
    }

    //Discover Multicast temporary entry

    ws_neighbor_temp_class_t *entry = ws_llc_get_multicast_temp_entry(cur, mac64);
    if (!entry) {
        return false;
    }
    //Create entry
    bool create_ok = ws_bootstrap_neighbor_info_request(cur, mac64, &neigh_buffer, true);
    if (create_ok) {
        ws_neighbor_class_entry_t *ws_neigh = neigh_buffer.ws_neighbor;
        //Copy fhss temporary data
        *ws_neigh = entry->neigh_info_list;
        //ETX Create here
        etx_lqi_dbm_update(cur->id, entry->mpduLinkQuality, entry->signal_dbm, neigh_buffer.neighbor->index);
        mac_neighbor_table_trusted_neighbor(mac_neighbor_info(cur), neigh_buffer.neighbor, true);
    }
    ws_llc_free_multicast_temp_entry(cur, entry);


    return create_ok;
}

static void ws_bootstrap_rpl_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("RPL Activate");
    bool downstream = true;
    bool leaf = false;

    addr_add_router_groups(cur);
    rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, downstream);
    rpl_control_set_callback(protocol_6lowpan_rpl_domain, ws_bootstrap_rpl_callback, ws_rpl_prefix_callback, ws_rpl_new_parent_callback_t, cur);
    // If i am router I Do this
    rpl_control_force_leaf(protocol_6lowpan_rpl_domain, leaf);
    rpl_control_request_parent_link_confirmation(true);

    cur->ws_info->rpl_state = 0xff; // Set invalid state and learn from event
}

static void ws_bootstrap_network_start(protocol_interface_info_entry_t *cur)
{
    //Set Network names, Pan information configure, hopping schedule & GTKHash
    ws_llc_set_network_name(cur, (uint8_t *)cur->ws_info->network_name, strlen(cur->ws_info->network_name));
    ws_llc_set_pan_information_pointer(cur, &cur->ws_info->pan_information);
}

static void ws_bootstrap_network_discovery_configure(protocol_interface_info_entry_t *cur)
{
    // Reset information to defaults
    cur->ws_info->network_pan_id = 0xffff;

    ws_common_regulatory_domain_config(cur);
    ws_fhss_discovery_configure(cur);

    //Set Network names, Pan information configure, hopping schedule & GTKHash
    ws_llc_set_network_name(cur, (uint8_t *)cur->ws_info->network_name, strlen(cur->ws_info->network_name));
}


static void ws_bootstrap_advertise_start(protocol_interface_info_entry_t *cur)
{
    cur->ws_info->trickle_pa_running = true;
    trickle_start(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);
    cur->ws_info->trickle_pc_running = true;
    trickle_start(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
}

static void ws_bootstrap_pan_version_increment(protocol_interface_info_entry_t *cur)
{
    cur->ws_info->pan_version_timer = 1;
}

// Start network scan
static void ws_bootstrap_start_discovery(protocol_interface_info_entry_t *cur)
{
    tr_debug("router discovery start");
    ws_bootstrap_state_change(cur, ER_ACTIVE_SCAN);
    cur->nwk_nd_re_scan_count = 0;
    cur->ws_info->configuration_learned = false;
    cur->ws_info->pan_version_timeout_timer = 0;
    // Clear parent info
    memset(cur->ws_info->parent_info.addr, 0, 8);

    // Clear learned neighbours
    ws_bootstrap_neighbor_list_clean(cur);

    // Clear RPL information
    rpl_control_remove_domain_from_interface(cur);

    // Clear ip stack from old information
    ws_bootstrap_ip_stack_reset(cur);
    // New network scan started old addresses not assumed valid anymore
    ws_bootstrap_ip_stack_addr_clear(cur);

    if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) != INTERFACE_NWK_BOOTSRAP_ACTIVE) {
        // we have sent bootstrap ready event and now
        // restarted discovery so bootstrap down event is sent
        cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;
        ws_nwk_event_post(cur, ARM_NWK_NWK_CONNECTION_DOWN);
    }

    // Start advertisement solicit trickle and calculate when we are checking the status
    cur->ws_info->trickle_pas_running = true;
    if (cur->ws_info->trickle_pan_advertisement_solicit.I != cur->ws_info->trickle_params_pan_discovery.Imin) {
        // Trickle not reseted so starting a new interval
        trickle_start(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);
    }

    // Discovery statemachine is checkked after we have sent the Solicit
    uint16_t time_to_solicit = 0;
    if (cur->ws_info->trickle_pan_advertisement_solicit.t > cur->ws_info->trickle_pan_advertisement_solicit.now) {
        time_to_solicit = cur->ws_info->trickle_pan_advertisement_solicit.t - cur->ws_info->trickle_pan_advertisement_solicit.now;
    }

    tr_debug("Disc params imin %u, imax %u, expirations %u, k %u PAS Trickle I %u t %u, now %u, c %u",
             cur->ws_info->trickle_params_pan_discovery.Imin, cur->ws_info->trickle_params_pan_discovery.Imax, cur->ws_info->trickle_params_pan_discovery.TimerExpirations, cur->ws_info->trickle_params_pan_discovery.k,
             cur->ws_info->trickle_pan_advertisement_solicit.I, cur->ws_info->trickle_pan_advertisement_solicit.t, cur->ws_info->trickle_pan_advertisement_solicit.now, cur->ws_info->trickle_pan_advertisement_solicit.c);

    cur->bootsrap_state_machine_cnt = time_to_solicit + cur->ws_info->trickle_params_pan_discovery.Imin + randLIB_get_8bit() % 50;
}

// Start authentication
static void ws_bootstrap_start_authentication(protocol_interface_info_entry_t *cur)
{
    // Set PAN ID and network name to controller
    ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->network_name);

    ws_pae_controller_authenticate(cur);
}

static void ws_bootstrap_mac_security_enable(protocol_interface_info_entry_t *cur)
{
    mac_helper_default_security_level_set(cur, AES_SECURITY_LEVEL_ENC_MIC64);
    mac_helper_default_security_key_id_mode_set(cur, MAC_KEY_ID_MODE_IDX);
}

static void ws_bootstrap_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t slot, uint8_t index, uint8_t *key)
{
    mac_helper_security_key_to_descriptor_set(cur, key, index + 1, slot);
}

static void ws_bootstrap_nw_key_clear(protocol_interface_info_entry_t *cur, uint8_t slot)
{
    mac_helper_security_key_descriptor_clear(cur, slot);
}

static void ws_bootstrap_nw_key_index_set(protocol_interface_info_entry_t *cur, uint8_t index)
{
    // Set send key
    mac_helper_security_auto_request_key_index_set(cur, index + 1);
}

static void ws_bootstrap_nw_frame_counter_set(protocol_interface_info_entry_t *cur, uint32_t counter)
{
    // Set frame counter
    mac_helper_link_frame_counter_set(cur->id, counter);
}

static void ws_bootstrap_nw_frame_counter_read(protocol_interface_info_entry_t *cur, uint32_t *counter)
{
    // Read frame counter
    mac_helper_link_frame_counter_read(cur->id, counter);
}

static void ws_bootstrap_authentication_completed(protocol_interface_info_entry_t *cur, bool success)
{
    if (success) {
        tr_debug("authentication success");
        ws_bootstrap_event_configuration_start(cur);
    } else {
        tr_debug("authentication failed");
        // What else to do to start over again...
        // Trickle is reseted when entering to discovery from state 2
        trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);
        ws_bootstrap_event_discovery_start(cur);
    }
}

// Start configuration learning
static void ws_bootstrap_start_configuration_learn(protocol_interface_info_entry_t *cur)
{
    tr_debug("router configuration learn start");
    ws_bootstrap_state_change(cur, ER_SCAN);

    cur->ws_info->configuration_learned = false;
    // Clear parent info

    memset(cur->ws_info->parent_info.addr, 0, 8);

    // Clear all temporary information
    ws_bootstrap_ip_stack_reset(cur);

    cur->ws_info->pas_requests = 0;
    // Reset advertisement solicit trickle to start discovering network
    cur->ws_info->trickle_pcs_running = true;
    trickle_start(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery);
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery);
}
static void ws_bootstrap_rpl_scan_start(protocol_interface_info_entry_t *cur)
{
    tr_debug("Start RPL learn");
    // routers wait until RPL root is contacted
    ws_bootstrap_state_change(cur, ER_RPL_SCAN);
    // Set timeout for check to 30 -60 seconds
    cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(WS_RPL_DIS_INITIAL_TIMEOUT / 2, WS_RPL_DIS_INITIAL_TIMEOUT);
}

/*
 * Event transitions
 *
 * */
void ws_bootstrap_event_discovery_start(protocol_interface_info_entry_t *cur)
{
    ws_bootsrap_event_trig(WS_DISCOVERY_START, cur->bootStrapId, ARM_LIB_LOW_PRIORITY_EVENT, NULL);
}
void ws_bootstrap_event_configuration_start(protocol_interface_info_entry_t *cur)
{
    ws_bootsrap_event_trig(WS_CONFIGURATION_START, cur->bootStrapId, ARM_LIB_LOW_PRIORITY_EVENT, NULL);
}
void ws_bootstrap_event_authentication_start(protocol_interface_info_entry_t *cur)
{
    ws_bootstrap_state_change(cur, ER_PANA_AUTH);
}
void ws_bootstrap_event_operation_start(protocol_interface_info_entry_t *cur)
{
    ws_bootsrap_event_trig(WS_OPERATION_START, cur->bootStrapId, ARM_LIB_LOW_PRIORITY_EVENT, NULL);
}
void ws_bootstrap_event_routing_ready(protocol_interface_info_entry_t *cur)
{
    ws_bootsrap_event_trig(WS_ROUTING_READY, cur->bootStrapId, ARM_LIB_LOW_PRIORITY_EVENT, NULL);
}
void ws_bootstrap_configuration_trickle_reset(protocol_interface_info_entry_t *cur)
{
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
}


static void ws_bootstrap_pan_advert_solicit(protocol_interface_info_entry_t *cur)
{
    asynch_request_t async_req;
    memset(&async_req, 0, sizeof(asynch_request_t));
    async_req.message_type = WS_FT_PAN_ADVERT_SOL;
    //Request UTT Header and US and Net name from payload
    async_req.wh_requested_ie_list.utt_ie = true;
    async_req.wp_requested_nested_ie_list.us_ie = true;
    async_req.wp_requested_nested_ie_list.net_name_ie = true;

    ws_generate_channel_list(async_req.channel_list.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

    async_req.channel_list.channel_page = CHANNEL_PAGE_10;
    async_req.security.SecurityLevel = 0;

    ws_llc_asynch_request(cur, &async_req);
}

static void ws_bootstrap_pan_config_solicit(protocol_interface_info_entry_t *cur)
{
    asynch_request_t async_req;
    memset(&async_req, 0, sizeof(asynch_request_t));
    async_req.message_type = WS_FT_PAN_CONF_SOL;
    //Request UTT Header and US and Net name from payload
    async_req.wh_requested_ie_list.utt_ie = true;
    async_req.wp_requested_nested_ie_list.us_ie = true;
    async_req.wp_requested_nested_ie_list.net_name_ie = true;

    ws_generate_channel_list(async_req.channel_list.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

    async_req.channel_list.channel_page = CHANNEL_PAGE_10;
    async_req.security.SecurityLevel = 0;

    ws_llc_asynch_request(cur, &async_req);
}

static struct rpl_instance *ws_get_rpl_instance(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->rpl_domain) {
        return NULL;
    }
    struct rpl_instance *best_instance = NULL;
    ns_list_foreach(struct rpl_instance, instance, &cur->rpl_domain->instances) {
        best_instance = instance;
        // Select best grounded and lowest rank? But there should be only one really
    }
    return best_instance;
}

static uint16_t ws_bootstrap_routing_cost_calculate(protocol_interface_info_entry_t *cur)
{
    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_entry_get_priority(mac_neighbor_info(cur));
    if (!mac_neighbor) {
        return 0xffff;
    }
    ws_neighbor_class_entry_t *ws_neighbor =  ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, mac_neighbor->index);
    if (!ws_neighbor) {
        return 0xffff;
    }

    uint16_t etx = etx_local_etx_read(cur->id, mac_neighbor->index);
    if (etx == 0) {
        etx = 0xffff;
    }
    if (etx > 0x800) {
        // Wi-SUN section 6.2.3.1.6.1 says ETX can only be maximum of 1024 (8*128) in RPL units, ie 8.0.
        etx = 0x800;
    }
    etx = etx >> 1;

    return ws_neighbor->routing_cost + etx;
}

static uint16_t ws_bootstrap_rank_get(protocol_interface_info_entry_t *cur)
{
    struct rpl_instance *rpl_instance = ws_get_rpl_instance(cur);
    if (!rpl_instance) {
        return 0xffff;
    }
    return rpl_control_current_rank(rpl_instance);
}


static uint16_t ws_bootstrap_min_rank_inc_get(protocol_interface_info_entry_t *cur)
{
    struct rpl_instance *rpl_instance = ws_get_rpl_instance(cur);
    if (!rpl_instance) {
        return 0xffff;
    }
    struct rpl_dodag_info_t dodag_info;
    if (!rpl_control_read_dodag_info(rpl_instance, &dodag_info)) {
        return 0xffff;
    }
    return dodag_info.dag_min_hop_rank_inc;
}

static void ws_bootstrap_pan_advert(protocol_interface_info_entry_t *cur)
{
    asynch_request_t async_req;
    memset(&async_req, 0, sizeof(asynch_request_t));
    async_req.message_type = WS_FT_PAN_ADVERT;
    //Request UTT Header, Pan information and US and Net name from payload
    async_req.wh_requested_ie_list.utt_ie = true;
    async_req.wp_requested_nested_ie_list.us_ie = true;
    async_req.wp_requested_nested_ie_list.pan_ie = true;
    async_req.wp_requested_nested_ie_list.net_name_ie = true;

    ws_generate_channel_list(async_req.channel_list.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

    async_req.channel_list.channel_page = CHANNEL_PAGE_10;
    async_req.security.SecurityLevel = 0;

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        // Border routers write the NW size
        cur->ws_info->pan_information.pan_size = ws_bbr_pan_size(cur);
        cur->ws_info->pan_information.routing_cost = 0;
    } else {
        // Nodes need to calculate routing cost
        // PAN size is saved from latest PAN advertisement
        cur->ws_info->pan_information.routing_cost = ws_bootstrap_routing_cost_calculate(cur);
    }


    ws_llc_asynch_request(cur, &async_req);
}

static void ws_bootstrap_pan_config(protocol_interface_info_entry_t *cur)
{
    asynch_request_t async_req;
    memset(&async_req, 0, sizeof(asynch_request_t));
    async_req.message_type = WS_FT_PAN_CONF;
    //Request UTT Header, Pan information and US and Net name from payload
    async_req.wh_requested_ie_list.utt_ie = true;
    async_req.wh_requested_ie_list.bt_ie = true;
    async_req.wp_requested_nested_ie_list.us_ie = true;
    async_req.wp_requested_nested_ie_list.bs_ie = true;
    async_req.wp_requested_nested_ie_list.pan_version_ie = true;
    async_req.wp_requested_nested_ie_list.gtkhash_ie = true;
    async_req.wp_requested_nested_ie_list.vp_ie = true;

    ws_generate_channel_list(async_req.channel_list.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain);

    async_req.channel_list.channel_page = CHANNEL_PAGE_10;
    async_req.security.SecurityLevel = mac_helper_default_security_level_get(cur);
    async_req.security.KeyIdMode = mac_helper_default_security_key_id_mode_get(cur);
    async_req.security.KeyIndex = mac_helper_default_key_index_get(cur);
    ws_llc_asynch_request(cur, &async_req);
}

static void ws_bootstrap_event_handler(arm_event_s *event)
{
    ws_bootsrap_event_type_e event_type;
    event_type = (ws_bootsrap_event_type_e)event->event_type;
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_bootstrap_id(event->receiver);
    if (!cur) {
        return;
    }

    switch (event_type) {
        case WS_INIT_EVENT:
            tr_debug("tasklet init");
            break;
        case WS_DISCOVERY_START:
            tr_info("Discovery start");
            // All trickle timers stopped to allow entry from any state
            cur->ws_info->trickle_pa_running = false;
            cur->ws_info->trickle_pc_running = false;
            cur->ws_info->trickle_pas_running = false;
            cur->ws_info->trickle_pcs_running = false;

            if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                tr_debug("Border router start network");

                if (!ws_bbr_ready_to_start(cur)) {
                    // Wi-SUN not started yet we wait for Border router permission
                    ws_bootstrap_state_change(cur, ER_WAIT_RESTART);
                    cur->nwk_nd_re_scan_count = randLIB_get_random_in_range(40, 100);
                    return;
                }
                ws_pae_controller_auth_init(cur);

                // Randomize fixed channels. Only used if channel plan is fixed.
                cur->ws_info->fhss_uc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels);
                cur->ws_info->fhss_bc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels);
                cur->ws_info->network_pan_id = randLIB_get_random_in_range(0, 0xfffd);
                cur->ws_info->pan_information.pan_size = 0;
                cur->ws_info->pan_information.pan_version = randLIB_get_random_in_range(0, 0xffff);
                cur->ws_info->pan_information.routing_cost = 0;
                cur->ws_info->pan_information.rpl_routing_method = true;
                cur->ws_info->pan_information.use_parent_bs = true;
                cur->ws_info->pan_information.version = WS_FAN_VERSION_1_0;

                uint8_t *gtkhash = ws_pae_controller_gtk_hash_ptr_get(cur);
                ws_llc_set_gtkhash(cur, gtkhash);
                cur->ws_info->pan_version_timer = ws_common_version_lifetime_get(cur->ws_info->network_size_config);

                // Set default parameters for FHSS when starting a discovery
                ws_fhss_border_router_configure(cur);
                ws_bootstrap_fhss_activate(cur);
                ws_bootstrap_event_operation_start(cur);

                uint8_t ll_addr[16];
                addr_interface_get_ll_address(cur, ll_addr, 1);

                //SET EAPOL authenticator EUI64
                ws_pae_controller_border_router_addr_write(cur, cur->mac);

                // Set EAPOL relay to port 10255 and authenticator relay to 10253 (and to own ll address)
                ws_eapol_relay_start(cur, BR_EAPOL_RELAY_SOCKET_PORT, ll_addr, EAPOL_RELAY_SOCKET_PORT);

                // Set authenticator relay to port 10253 and PAE to 10254 (and to own ll address)
                ws_eapol_auth_relay_start(cur, EAPOL_RELAY_SOCKET_PORT, ll_addr, PAE_AUTH_SOCKET_PORT);

                // Set PAN ID and network name to controller
                ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->network_name);

                // Set PAE port to 10254 and authenticator relay to 10253 (and to own ll address)
                ws_pae_controller_authenticator_start(cur, PAE_AUTH_SOCKET_PORT, ll_addr, EAPOL_RELAY_SOCKET_PORT);
                break;
            }
            ws_pae_controller_supp_init(cur);

            // Configure LLC for network discovery
            ws_bootstrap_network_discovery_configure(cur);
            ws_bootstrap_fhss_activate(cur);
            // Start network scan
            ws_bootstrap_start_discovery(cur);
            break;

        case WS_CONFIGURATION_START:
            tr_info("Configuration start");
            // Old configuration is considered invalid stopping all
            cur->ws_info->trickle_pa_running = false;
            cur->ws_info->trickle_pc_running = false;
            cur->ws_info->trickle_pas_running = false;
            cur->ws_info->trickle_pcs_running = false;

            // Build list of possible neighbours and learn first broadcast schedule

            ws_bootstrap_start_configuration_learn(cur);
            break;
        case WS_OPERATION_START:
            tr_info("operation start");
            // Advertisements stopped during the RPL scan
            cur->ws_info->trickle_pa_running = false;
            cur->ws_info->trickle_pc_running = false;
            cur->ws_info->trickle_pas_running = false;
            cur->ws_info->trickle_pcs_running = false;
            // Activate RPL
            // Activate IPv6 stack
            ws_bootstrap_ip_stack_activate(cur);
            ws_bootstrap_rpl_activate(cur);
            ws_bootstrap_network_start(cur);
            // Wait for RPL start
            if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                ws_bootstrap_event_routing_ready(cur);
            } else {
                ws_bootstrap_rpl_scan_start(cur);
            }
            break;
        case WS_ROUTING_READY:
            tr_info("Routing ready");
            // stopped all to make sure we can enter here from any state
            cur->ws_info->trickle_pa_running = false;
            cur->ws_info->trickle_pc_running = false;
            cur->ws_info->trickle_pas_running = false;
            cur->ws_info->trickle_pcs_running = false;

            // Indicate PAE controller that bootstrap is ready
            ws_pae_controller_bootstrap_done(cur);

            ws_bootstrap_advertise_start(cur);
            ws_bootstrap_state_change(cur, ER_BOOTSRAP_DONE);
            break;

        default:
            tr_err("Invalid event received");
            break;
    }
}


/*
 * State machine
 *
 * */
void ws_bootstrap_network_scan_process(protocol_interface_info_entry_t *cur)
{

    if (!ws_bootstrap_network_found(cur)) {
        // Next check will be after one trickle
        cur->bootsrap_state_machine_cnt += cur->ws_info->trickle_params_pan_discovery.Imin + randLIB_get_8bit() % 50;
        return;
    }
    tr_info("select network");

    // Add EAPOL neighbour
    llc_neighbour_req_t neighbor_info;
    if (!ws_bootstrap_neighbor_info_request(cur, cur->ws_info->parent_info.addr, &neighbor_info, true)) {
        return;
    }

    ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, &cur->ws_info->parent_info.ws_utt, cur->ws_info->parent_info.timestamp);
    ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, &cur->ws_info->parent_info.ws_us);


    ws_bootstrap_network_information_learn(cur);
    ws_bootstrap_fhss_activate(cur);

    ws_pae_controller_set_target(cur, cur->ws_info->parent_info.pan_id, cur->ws_info->parent_info.addr); // temporary!!! store since auth
    ws_bootstrap_event_authentication_start(cur);
    return;
}

void ws_bootstrap_configure_process(protocol_interface_info_entry_t *cur)
{

    if (cur->ws_info->configuration_learned) {
        ws_bootstrap_network_configuration_learn(cur);


        ws_bootstrap_event_operation_start(cur);


        return;
    }
    return;
}
void ws_bootstrap_rpl_wait_process(protocol_interface_info_entry_t *cur)
{

    if (cur->ws_info->rpl_state == RPL_EVENT_DAO_DONE) {
        // RPL routing is ready
        ws_bootstrap_event_routing_ready(cur);
    } else if (!rpl_control_have_dodag(cur->rpl_domain)) {
        // RPL not ready send DIS message if possible
        if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER) {
            // TODO Multicast DIS should be sent only if no DIO heard for some time
            rpl_control_transmit_dis(cur->rpl_domain, cur, 0, 0, NULL, 0, ADDR_LINK_LOCAL_ALL_RPL_NODES);
        }
        // set timer for next DIS
        cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(WS_RPL_DIS_TIMEOUT / 2, WS_RPL_DIS_TIMEOUT);
    }
    return;
}

/*

 static bool ws_bootstrap_state_active(struct protocol_interface_info_entry *cur)
{
    if(cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_configure(struct protocol_interface_info_entry *cur)
{
    // Think about the state value
    if(cur->nwk_bootstrap_state == ER_SCAN) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_wait_rpl(struct protocol_interface_info_entry *cur)
{
    // Think about the state value
    if(cur->nwk_bootstrap_state == ER_RPL_SCAN) {
        return true;
    }
    return false;
}
*/
static bool ws_bootstrap_state_discovery(struct protocol_interface_info_entry *cur)
{
    if (cur->nwk_bootstrap_state == ER_ACTIVE_SCAN) {
        return true;
    }
    return false;
}

static void ws_bootstrap_state_change(protocol_interface_info_entry_t *cur, icmp_state_t nwk_bootstrap_state)
{
    cur->bootsrap_state_machine_cnt = 1;
    cur->nwk_bootstrap_state = nwk_bootstrap_state;
}

void ws_bootstrap_state_machine(protocol_interface_info_entry_t *cur)
{

    switch (cur->nwk_bootstrap_state) {
        case ER_WAIT_RESTART:
            tr_debug("WS SM:Wait for startup");
            ws_bootstrap_event_discovery_start(cur);
            break;
        case ER_ACTIVE_SCAN:
            tr_debug("WS SM:Active Scan");
            ws_bootstrap_network_scan_process(cur);
            break;
        case ER_SCAN:
            tr_debug("WS SM:configuration Scan");
            ws_bootstrap_configure_process(cur);
            break;
        case ER_PANA_AUTH:
            tr_info("authentication start");
            // Advertisements stopped during the EAPOL
            cur->ws_info->trickle_pa_running = false;
            cur->ws_info->trickle_pc_running = false;
            cur->ws_info->trickle_pas_running = false;
            cur->ws_info->trickle_pcs_running = false;

            ws_bootstrap_start_authentication(cur);
            break;
        case ER_RPL_SCAN:
            tr_debug("WS SM:Wait RPL to contact DODAG root");
            ws_bootstrap_rpl_wait_process(cur);
            break;
        case ER_BOOTSRAP_DONE:
            tr_debug("WS SM:Bootstrap Done");
            // Bootstrap_done event to application
            nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);
            break;
        default:
            tr_warn("WS SM:Invalid state %d", cur->nwk_bootstrap_state);
    }
}

void ws_bootstrap_trickle_timer(protocol_interface_info_entry_t *cur, uint16_t ticks)
{
    if (cur->ws_info->trickle_pas_running &&
            trickle_timer(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
        // send PAN advertisement solicit
        tr_info("Send PAN advertisement Solicit");
        ws_bootstrap_pan_advert_solicit(cur);
    }
    if (cur->ws_info->trickle_pcs_running &&
            trickle_timer(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
        // send PAN Configuration solicit
        if (cur->ws_info->pas_requests >= PCS_MAX) {
            // if MAX PCS sent restart discovery

            // Remove network keys from MAC
            ws_pae_controller_nw_keys_remove(cur);

            // Trickle is reseted when entering to discovery from state 3
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);
            ws_bootstrap_event_discovery_start(cur);
            return;
        }
        tr_info("Send PAN configuration Solicit");
        cur->ws_info->pas_requests++;
        ws_bootstrap_pan_config_solicit(cur);
    }
    if (cur->ws_info->trickle_pa_running &&
            trickle_timer(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
        // send PAN advertisement
        tr_info("Send PAN advertisement");
        ws_bootstrap_pan_advert(cur);
    }
    if (cur->ws_info->trickle_pc_running &&
            trickle_timer(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
        // send PAN Configuration
        tr_info("Send PAN configuration");
        ws_bootstrap_pan_config(cur);
    }
}


void ws_bootstrap_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    if (cur->ws_info->pan_version_timeout_timer) {
        // PAN version timer running
        if (cur->ws_info->pan_version_timeout_timer > seconds) {
            cur->ws_info->pan_version_timeout_timer -= seconds;
        } else {
            // Border router has timed out
            tr_warn("Border router has timed out");
            ws_bootstrap_event_discovery_start(cur);
        }
    }
}

void ws_primary_parent_update(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neighbor)
{
    if (interface->ws_info) {
        llc_neighbour_req_t neighbor_info;
        neighbor_info.neighbor = neighbor;
        neighbor_info.ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor->index);
        ws_bootstrap_primary_parent_set(interface, &neighbor_info, WS_PARENT_HARD_SYNCH);
        uint8_t link_local_address[16];
        ws_bootsrap_create_ll_address(link_local_address, neighbor->mac64);
        dhcp_client_server_address_update(interface->id, NULL, link_local_address);

        ws_secondary_parent_update(interface);
    }
}

void ws_secondary_parent_update(protocol_interface_info_entry_t *interface)
{
    if (interface->ws_info) {
        ns_list_foreach(if_address_entry_t, address, &interface->ip_addresses) {
            if (!addr_is_ipv6_link_local(address->address)) {
                ws_address_registration_update(interface);
            }
        }
    }
}

void ws_bootstrap_etx_accelerate(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neigh)
{
    ws_neighbor_class_entry_t *ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neigh->index);
    //Enable Faster ETX probing
    ws_neighbor->accelerated_etx_probe = true;
    //Move Neighbor to first to for accelerate Process
    mac_neighbor_table_t *table_class = mac_neighbor_info(interface);
    ns_list_remove(&table_class->neighbour_list, neigh);
    ns_list_add_to_start(&table_class->neighbour_list, neigh);
    //Try to Generate Active NUD Immediately
    if (!ws_neighbor_entry_nud_notify(neigh, interface)) {
        return;//Return if NUD active is full
    }
    table_class->active_nud_process++;
    neigh->nud_active = true;
    //Push NS to send
    ws_nud_active_timer(interface, 0);

}

#endif //HAVE_WS
