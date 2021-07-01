/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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
#ifdef HAVE_WS
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "net_interface.h"
#include "eventOS_event.h"
#include "randLIB.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "sw_mac.h"
#include "ccmLIB.h"
#include "Core/include/ns_monitor.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/MAC/mac_ie_lib.h"
#include "MPL/mpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "RPL/rpl_policy.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/ip.h"
#include "Service_Libs/Trickle/trickle.h"
#include "Service_Libs/fhss/channel_list.h"
#include "Service_Libs/utils/ns_time.h"
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
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "platform/topo_trace.h"
#include "dhcp_service_api.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_vendordata.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "ws_management_api.h"
#include "net_rpl.h"
#include "mac_api.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"
#include "6LoWPAN/ws/ws_eapol_auth_relay.h"
#include "6LoWPAN/ws/ws_eapol_relay.h"
#include "libNET/src/net_dns_internal.h"
#include "Service_Libs/random_early_detection/random_early_detection_api.h"

#define TRACE_GROUP "wsbs"


static void ws_bootstrap_event_handler(arm_event_s *event);
static void ws_bootstrap_state_change(protocol_interface_info_entry_t *cur, icmp_state_t nwk_bootstrap_state);
static bool ws_bootstrap_state_discovery(struct protocol_interface_info_entry *cur);
static bool ws_bootstrap_state_active(struct protocol_interface_info_entry *cur);
static bool ws_bootstrap_state_wait_rpl(struct protocol_interface_info_entry *cur);
static int8_t ws_bootsrap_event_trig(ws_bootsrap_event_type_e event_type, int8_t interface_id, arm_library_event_priority_e priority, void *event_data);

static bool ws_bootstrap_neighbor_info_request(struct protocol_interface_info_entry *interface, const uint8_t *mac_64, llc_neighbour_req_t *neighbor_buffer, bool request_new);
static uint16_t ws_bootstrap_routing_cost_calculate(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_rank_get(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_min_rank_inc_get(protocol_interface_info_entry_t *cur);

static void ws_bootstrap_mac_security_enable(protocol_interface_info_entry_t *cur);
static void ws_bootstrap_nw_key_set(protocol_interface_info_entry_t *cur, uint8_t operation, uint8_t index, uint8_t *key);
static void ws_bootstrap_nw_key_clear(protocol_interface_info_entry_t *cur, uint8_t slot);
static void ws_bootstrap_nw_key_index_set(protocol_interface_info_entry_t *cur, uint8_t index);
static void ws_bootstrap_nw_frame_counter_set(protocol_interface_info_entry_t *cur, uint32_t counter, uint8_t slot);
static void ws_bootstrap_nw_frame_counter_read(protocol_interface_info_entry_t *cur, uint32_t *counter, uint8_t slot);
static void ws_bootstrap_nw_info_updated(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, uint16_t pan_version, char *network_name);
static void ws_bootstrap_authentication_completed(protocol_interface_info_entry_t *cur, auth_result_e result, uint8_t *target_eui_64);
static const uint8_t *ws_bootstrap_authentication_next_target(protocol_interface_info_entry_t *cur, const uint8_t *previous_eui_64, uint16_t *pan_id);
static bool ws_bootstrap_eapol_congestion_get(protocol_interface_info_entry_t *interface_ptr, uint16_t active_supp);
static void ws_bootstrap_pan_version_increment(protocol_interface_info_entry_t *cur);
static ws_nud_table_entry_t *ws_nud_entry_discover(protocol_interface_info_entry_t *cur, void *neighbor);
static void ws_nud_entry_remove(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *entry_ptr);
static bool ws_neighbor_entry_nud_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data);

static void ws_address_registration_update(protocol_interface_info_entry_t *interface, const uint8_t addr[16]);
static int8_t ws_bootstrap_neighbor_set(protocol_interface_info_entry_t *cur, parent_info_t *parent_ptr, bool clear_list);
static void ws_bootstrap_parent_confirm(protocol_interface_info_entry_t *cur, struct rpl_instance *instance);

static void ws_bootstrap_candidate_table_reset(protocol_interface_info_entry_t *cur);
static parent_info_t *ws_bootstrap_candidate_parent_get(struct protocol_interface_info_entry *cur, const uint8_t *addr, bool create);
static void ws_bootstrap_candidate_parent_sort(struct protocol_interface_info_entry *cur, parent_info_t *new_entry);
static void ws_bootstrap_packet_congestion_init(protocol_interface_info_entry_t *cur);

static void ws_bootstrap_asynch_trickle_stop(protocol_interface_info_entry_t *cur);
static void ws_bootstrap_advertise_start(protocol_interface_info_entry_t *cur);
static void ws_bootstrap_rpl_scan_start(protocol_interface_info_entry_t *cur);

static uint16_t ws_randomize_fixed_channel(uint16_t configured_fixed_channel, uint8_t number_of_channels, uint32_t *channel_mask);

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



static mac_neighbor_table_entry_t *ws_bootstrap_mac_neighbor_allocate(struct protocol_interface_info_entry *interface, const uint8_t *src64)
{
    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_entry_allocate(mac_neighbor_info(interface), src64);
    if (!neighbor) {
        return NULL;
    }
    // TODO only call these for new neighbour
    mlme_device_descriptor_t device_desc;
    neighbor->lifetime = ws_cfg_neighbour_temporary_lifetime_get();
    neighbor->link_lifetime = ws_cfg_neighbour_temporary_lifetime_get();
    mac_helper_device_description_write(interface, &device_desc, neighbor->mac64, neighbor->mac16, 0, false);
    mac_helper_devicetable_set(&device_desc, interface, neighbor->index, interface->mac_parameters->mac_default_key_index, true);

    return neighbor;
}

mac_neighbor_table_entry_t *ws_bootstrap_mac_neighbor_add(struct protocol_interface_info_entry *interface, const uint8_t *src64)

{
    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), src64, MAC_ADDR_MODE_64_BIT);
    if (neighbor) {
        return neighbor;
    }

    return ws_bootstrap_mac_neighbor_allocate(interface, src64);
}

void ws_bootstrap_neighbor_set_stable(struct protocol_interface_info_entry *interface, const uint8_t *src64)
{
    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), src64, MAC_ADDR_MODE_64_BIT);

    if (neighbor && neighbor->link_lifetime != WS_NEIGHBOR_LINK_TIMEOUT) {
        neighbor->lifetime = WS_NEIGHBOR_LINK_TIMEOUT;
        neighbor->link_lifetime = WS_NEIGHBOR_LINK_TIMEOUT;
        tr_info("Added new neighbor %s : index:%u", trace_array(src64, 8), neighbor->index);
    }
}

void ws_bootstrap_mac_neighbor_short_time_set(struct protocol_interface_info_entry *interface, const uint8_t *src64, uint32_t valid_time)
{
    mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), src64, MAC_ADDR_MODE_64_BIT);

    if (neighbor && neighbor->link_lifetime <= valid_time) {
        //mlme_device_descriptor_t device_desc;
        neighbor->lifetime = valid_time;
        neighbor->link_lifetime = valid_time;
        tr_debug("Set short response neighbor %s : index:%u", trace_array(src64, 8), neighbor->index);
    }
}

static void ws_bootstrap_neighbor_delete(struct protocol_interface_info_entry *interface, mac_neighbor_table_entry_t *entry_ptr)
{
    mac_helper_devicetable_remove(interface->mac_api, entry_ptr->index, entry_ptr->mac64);
    etx_neighbor_remove(interface->id, entry_ptr->index, entry_ptr->mac64);
    ws_neighbor_class_entry_remove(&interface->ws_info->neighbor_storage, entry_ptr->index);
}

static void ws_bootstrap_neighbor_list_clean(struct protocol_interface_info_entry *interface)
{

    mac_neighbor_table_neighbor_list_clean(mac_neighbor_info(interface));
}

static void ws_address_reregister_trig(struct protocol_interface_info_entry *interface)
{
    if (interface->ws_info->aro_registration_timer == 0) {
        interface->ws_info->aro_registration_timer = WS_NEIGHBOR_NUD_TIMEOUT;
    }
}

static void ws_bootstrap_address_notification_cb(struct protocol_interface_info_entry *interface, const struct if_address_entry *addr, if_address_callback_t reason)
{
    /* No need for LL address registration */
    if (addr->source == ADDR_SOURCE_UNKNOWN || !interface->ws_info) {
        return;
    }

    if (reason == ADDR_CALLBACK_DAD_COMPLETE) {
        //If address is generated manually we need to force registration
        if (addr->source != ADDR_SOURCE_DHCP) {
            //Trigger Address Registration only when Bootstrap is ready
            if (interface->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
                tr_debug("Address registration %s", trace_ipv6(addr->address));
                ws_address_registration_update(interface, addr->address);
            }
            ws_address_reregister_trig(interface);
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
    }

    // Addressing in Wi-SUN interface was changed for Border router send new event so Application can update the state
    if (interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER &&
            interface->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
        if (interface->bootsrap_state_machine_cnt == 0) {
            interface->bootsrap_state_machine_cnt = 10; //Re trigger state check
        }
    }
}

static void ws_bootstrap_configure_max_retries(protocol_interface_info_entry_t *cur, uint8_t max_mac_retries)
{
    mac_helper_mac_mlme_max_retry_set(cur->id, max_mac_retries);
}

static void ws_bootstrap_configure_csma_ca_backoffs(protocol_interface_info_entry_t *cur, uint8_t max_backoffs, uint8_t min_be, uint8_t max_be)
{
    mac_helper_mac_mlme_max_csma_backoffs_set(cur->id, max_backoffs);
    mac_helper_mac_mlme_be_set(cur->id, min_be, max_be);
}

static void ws_bootstrap_configure_data_request_restart(protocol_interface_info_entry_t *cur, uint8_t cca_failure_restart_max, uint8_t tx_failure_restart_max, uint16_t blacklist_min_ms, uint16_t blacklist_max_ms)
{
    mlme_request_restart_config_t request_restart_config;
    request_restart_config.cca_failure_restart_max = cca_failure_restart_max;
    request_restart_config.tx_failure_restart_max = tx_failure_restart_max;
    request_restart_config.blacklist_min_ms = blacklist_min_ms;
    request_restart_config.blacklist_max_ms = blacklist_max_ms;
    mac_helper_mac_mlme_data_request_restart_set(cur->id, &request_restart_config);
}

static int ws_bootstrap_tasklet_init(protocol_interface_info_entry_t *cur)
{
    if (cur->bootStrapId < 0) {
        cur->bootStrapId = eventOS_event_handler_create(&ws_bootstrap_event_handler, WS_INIT_EVENT);
        tr_info("WS tasklet init");
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
if_address_entry_t *ws_probe_aro_address(protocol_interface_info_entry_t *interface)
{
    if (interface->global_address_available) {
        ns_list_foreach(if_address_entry_t, address, &interface->ip_addresses) {
            if (addr_ipv6_scope(address->address, interface) > IPV6_SCOPE_LINK_LOCAL) {
                return address;
            }
        }
    }
    return NULL;
}


static bool ws_nud_message_build(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *neighbor, bool nud_process)
{
    //Send NS
    uint8_t ll_target[16];
    aro_t aro_temp;
    //SET ARO and src address pointer to NULL by default
    aro_t *aro_ptr = NULL;
    uint8_t *src_address_ptr = NULL;

    ws_bootsrap_create_ll_address(ll_target, neighbor->mac64);
    if (nud_process) {
        tr_info("NUD generate NS %u", neighbor->index);
    } else {
        tr_info("Probe generate NS %u", neighbor->index);
        if_address_entry_t *gp_address = ws_probe_aro_address(cur);
        if (gp_address) {
            src_address_ptr = gp_address->address;
            aro_temp.status = ARO_SUCCESS;
            aro_temp.present = true;
            memcpy(aro_temp.eui64, cur->mac, 8);
            //Just Short Test
            aro_temp.lifetime = 1;
            aro_ptr = &aro_temp;
        }
    }
    buffer_t *buffer = icmpv6_build_ns(cur, ll_target, src_address_ptr, true, false, aro_ptr);
    if (buffer) {
        buffer->options.traffic_class = IP_DSCP_CS6 << IP_TCLASS_DSCP_SHIFT;
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
                entry->wait_response = ws_nud_message_build(cur, entry->neighbor_info, entry->nud_process);
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
    if (mac_neighbor) {
        ws_neighbor_class_entry_t *ws_neighbor =  ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, mac_neighbor->index);
        if (!ws_neighbor) {
            return NULL;
        }
        return &ws_neighbor->fhss_data;
    }
    //Discover temporary entry
    ws_neighbor_temp_class_t *temp_entry = ws_llc_get_eapol_temp_entry(cur, eui64);
    if (!temp_entry) {
        return NULL;
    }
    return &temp_entry->neigh_info_list.fhss_data;
}
static void ws_bootstrap_llc_hopping_update(struct protocol_interface_info_entry *cur, const fhss_ws_configuration_t *fhss_configuration)
{
    cur->ws_info->hopping_schdule.uc_fixed_channel = fhss_configuration->unicast_fixed_channel;
    cur->ws_info->hopping_schdule.bc_fixed_channel = fhss_configuration->broadcast_fixed_channel;
    // Read UC channel function from WS info because FHSS might be temporarily configured to fixed channel during discovery.
    cur->ws_info->hopping_schdule.uc_channel_function = cur->ws_info->cfg->fhss.fhss_uc_channel_function;
    cur->ws_info->hopping_schdule.bc_channel_function = fhss_configuration->ws_bc_channel_function;
    cur->ws_info->hopping_schdule.fhss_bc_dwell_interval = fhss_configuration->fhss_bc_dwell_interval;
    cur->ws_info->hopping_schdule.fhss_broadcast_interval = fhss_configuration->fhss_broadcast_interval;
    cur->ws_info->hopping_schdule.fhss_uc_dwell_interval = fhss_configuration->fhss_uc_dwell_interval;
    cur->ws_info->hopping_schdule.fhss_bsi = fhss_configuration->bsi;
}

static uint8_t ws_generate_exluded_channel_list_from_active_channels(ws_excluded_channel_data_t *excluded_data, const uint32_t *selected_channel_mask, const uint32_t *global_channel_mask, uint16_t number_of_channels)
{
    bool active_range = false;

    //Clear Old Data
    memset(excluded_data, 0, sizeof(ws_excluded_channel_data_t));

    for (uint8_t i = 0; i < number_of_channels; i++) {
        if (!(global_channel_mask[0 + (i / 32)] & (1 << (i % 32)))) {
            //Global exluded channel
            if (active_range) {
                //Mark range stop here
                active_range = false;
            }
            continue;
        }

        if (selected_channel_mask[0 + (i / 32)] & (1 << (i % 32))) {
            if (active_range) {
                //Mark range stop here
                active_range = false;
            }
        } else {
            //Mark excluded channel
            //Swap Order already here
            excluded_data->channel_mask[0 + (i / 32)] |= 1 << (31 - (i % 32));
            excluded_data->excluded_channel_count++;

            if (excluded_data->excluded_range_length < WS_EXCLUDED_MAX_RANGE_TO_SEND) {
                if (!active_range) {
                    excluded_data->excluded_range_length++;
                    active_range = true;
                    //Set start channel
                    excluded_data->exluded_range[excluded_data->excluded_range_length - 1].range_start = i;
                } else {
                    excluded_data->exluded_range[excluded_data->excluded_range_length - 1].range_end = i;
                }
            }
        }
    }

    excluded_data->channel_mask_bytes_inline = ((number_of_channels + 7) / 8);

    uint8_t channel_plan = 0;
    if (excluded_data->excluded_range_length == 0) {
        excluded_data->excuded_channel_ctrl = WS_EXC_CHAN_CTRL_NONE;
    } else if (excluded_data->excluded_range_length <= WS_EXCLUDED_MAX_RANGE_TO_SEND) {

        uint8_t range_length = (excluded_data->excluded_range_length * 4) + 3;
        if (range_length <= ((number_of_channels + 7) / 8) + 6) {
            excluded_data->excuded_channel_ctrl = WS_EXC_CHAN_CTRL_RANGE;
        } else {
            excluded_data->excuded_channel_ctrl = WS_EXC_CHAN_CTRL_BITMASK;
            channel_plan = 1;
        }
    } else {
        excluded_data->excuded_channel_ctrl = WS_EXC_CHAN_CTRL_BITMASK;
        channel_plan = 1;
    }
    tr_debug("Excluded ctrl %u, exluded channel count %u, total domain channels %u", excluded_data->excuded_channel_ctrl, excluded_data->excluded_channel_count, number_of_channels);
    return channel_plan;
}

static void ws_fhss_configure_channel_masks(protocol_interface_info_entry_t *cur, fhss_ws_configuration_t *fhss_configuration)
{
    fhss_configuration->channel_mask_size = cur->ws_info->hopping_schdule.number_of_channels;
    ws_generate_channel_list(fhss_configuration->channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain, cur->ws_info->hopping_schdule.operating_class, cur->ws_info->hopping_schdule.channel_plan_id);
    ws_generate_channel_list(fhss_configuration->unicast_channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain, cur->ws_info->hopping_schdule.operating_class, cur->ws_info->hopping_schdule.channel_plan_id);
    // using bitwise AND operation for user set channel mask to remove channels not allowed in this device
    for (uint8_t n = 0; n < 8; n++) {
        fhss_configuration->unicast_channel_mask[n] &= cur->ws_info->cfg->fhss.fhss_channel_mask[n];
    }
    //Update Exluded channels
    cur->ws_info->hopping_schdule.channel_plan = ws_generate_exluded_channel_list_from_active_channels(&cur->ws_info->hopping_schdule.excluded_channels, fhss_configuration->unicast_channel_mask, fhss_configuration->channel_mask, cur->ws_info->hopping_schdule.number_of_channels);
}

static int8_t ws_fhss_initialize(protocol_interface_info_entry_t *cur)
{
    fhss_api_t *fhss_api = ns_sw_mac_get_fhss_api(cur->mac_api);
    fhss_ws_configuration_t fhss_configuration;
    memset(&fhss_configuration, 0, sizeof(fhss_ws_configuration_t));
    if (!fhss_api) {
        // When FHSS doesn't exist yet, create one
        ws_fhss_configure_channel_masks(cur, &fhss_configuration);

        fhss_configuration.fhss_uc_dwell_interval = cur->ws_info->cfg->fhss.fhss_uc_dwell_interval;
        fhss_configuration.ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_uc_channel_function;
        fhss_configuration.ws_bc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_bc_channel_function;
        fhss_configuration.fhss_bc_dwell_interval = cur->ws_info->cfg->fhss.fhss_bc_dwell_interval;
        fhss_configuration.fhss_broadcast_interval = cur->ws_info->cfg->fhss.fhss_bc_interval;
        fhss_api = ns_fhss_ws_create(&fhss_configuration, cur->ws_info->fhss_timer_ptr);

        if (!fhss_api) {
            return -1;
        }
        ns_sw_mac_fhss_register(cur->mac_api, fhss_api);
        // Allow transmitting unicast frames only on TX slots in normal and expedited forwarding mode
        ns_fhss_ws_set_tx_allowance_level(fhss_api, WS_TX_SLOT, WS_TX_SLOT);
    } else {
        // Read defaults from the configuration to help FHSS testing
        const fhss_ws_configuration_t *fhss_configuration_copy = ns_fhss_ws_configuration_get(fhss_api);
        if (!fhss_configuration_copy) {
            // no configuration set yet
            return 0;
        }
        fhss_configuration = *fhss_configuration_copy;
        //Overwrite domain channel setup this will over write a default 35 channel
        int num_of_channels = channel_list_count_channels(fhss_configuration_copy->unicast_channel_mask);
        cur->ws_info->hopping_schdule.number_of_channels = (uint8_t) num_of_channels;
        memcpy(cur->ws_info->cfg->fhss.fhss_channel_mask, fhss_configuration_copy->unicast_channel_mask, sizeof(uint32_t) * 8);
        cur->ws_info->cfg->fhss.fhss_uc_channel_function = fhss_configuration_copy->ws_uc_channel_function;
        cur->ws_info->cfg->fhss.fhss_bc_channel_function = fhss_configuration_copy->ws_bc_channel_function;
        cur->ws_info->cfg->fhss.fhss_bc_dwell_interval = fhss_configuration_copy->fhss_bc_dwell_interval;
        cur->ws_info->cfg->fhss.fhss_bc_interval = fhss_configuration_copy->fhss_broadcast_interval;
        cur->ws_info->cfg->fhss.fhss_uc_dwell_interval = fhss_configuration_copy->fhss_uc_dwell_interval;
        cur->ws_info->cfg->fhss.fhss_bc_fixed_channel = fhss_configuration_copy->broadcast_fixed_channel;
        cur->ws_info->cfg->fhss.fhss_uc_fixed_channel = fhss_configuration_copy->unicast_fixed_channel;
        ws_fhss_configure_channel_masks(cur, &fhss_configuration);
        ns_fhss_ws_configuration_set(fhss_api, &fhss_configuration);
    }

    return 0;
}

static int8_t ws_fhss_set_defaults(protocol_interface_info_entry_t *cur, fhss_ws_configuration_t *fhss_configuration)
{
    fhss_configuration->fhss_uc_dwell_interval = cur->ws_info->cfg->fhss.fhss_uc_dwell_interval;
    fhss_configuration->ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_uc_channel_function;
    fhss_configuration->ws_bc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_bc_channel_function;
    fhss_configuration->fhss_bc_dwell_interval = cur->ws_info->cfg->fhss.fhss_bc_dwell_interval;
    fhss_configuration->fhss_broadcast_interval = cur->ws_info->cfg->fhss.fhss_bc_interval;
    if (cur->ws_info->cfg->fhss.fhss_uc_fixed_channel != 0xffff) {
        fhss_configuration->unicast_fixed_channel = cur->ws_info->cfg->fhss.fhss_uc_fixed_channel;
    }
    fhss_configuration->broadcast_fixed_channel = cur->ws_info->cfg->fhss.fhss_bc_fixed_channel;
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

    //GET BSI from BBR module
    fhss_configuration.bsi = ws_bbr_bsi_generate(cur);
    ws_fhss_configure_channel_masks(cur, &fhss_configuration);
    // Randomize fixed channels. Only used if channel plan is fixed.
    cur->ws_info->cfg->fhss.fhss_uc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    cur->ws_info->cfg->fhss.fhss_bc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    ws_fhss_set_defaults(cur, &fhss_configuration);
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);

    return 0;
}

static bool ws_channel_allowed(uint8_t channel, uint32_t *channel_mask)
{
    if ((1 << (channel % 32)) & (channel_mask[channel / 32])) {
        return true;
    }
    return false;
}

static uint16_t ws_randomize_fixed_channel(uint16_t configured_fixed_channel, uint8_t number_of_channels, uint32_t *channel_mask)
{
    if (configured_fixed_channel == 0xFFFF) {
        uint16_t random_channel = randLIB_get_random_in_range(0, number_of_channels - 1);
        while (ws_channel_allowed(random_channel, channel_mask) == false) {
            random_channel = randLIB_get_random_in_range(0, number_of_channels - 1);
        }
        return random_channel;
    } else {
        return configured_fixed_channel;
    }
}

static int8_t ws_fhss_configure(protocol_interface_info_entry_t *cur, bool discovery)
{
    // Read configuration of existing FHSS and start using the default values for any network
    fhss_ws_configuration_t fhss_configuration;
    memset(&fhss_configuration, 0, sizeof(fhss_ws_configuration_t));

    if (ns_fhss_ws_configuration_get(cur->ws_info->fhss_api)) {
        memcpy(&fhss_configuration, ns_fhss_ws_configuration_get(cur->ws_info->fhss_api), sizeof(fhss_ws_configuration_t));
        ws_fhss_set_defaults(cur, &fhss_configuration);
        ws_fhss_configure_channel_masks(cur, &fhss_configuration);
    }
    // Discovery is done using fixed channel
    if (discovery) {
        fhss_configuration.ws_uc_channel_function = WS_FIXED_CHANNEL;
    } else {
        fhss_configuration.ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_uc_channel_function;
    }
    fhss_configuration.ws_bc_channel_function = WS_FIXED_CHANNEL;
    fhss_configuration.fhss_broadcast_interval = 0;
    uint8_t tmp_uc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    uint8_t tmp_bc_fixed_channel = ws_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    fhss_configuration.unicast_fixed_channel = tmp_uc_fixed_channel;
    fhss_configuration.broadcast_fixed_channel = tmp_bc_fixed_channel;
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ns_fhss_ws_set_hop_count(cur->ws_info->fhss_api, 0xff);
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
            cur->ws_info->cfg->fhss.fhss_bc_fixed_channel = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.fixed_channel;
        }
        fhss_configuration.bsi = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_schedule_id;
        fhss_configuration.fhss_bc_dwell_interval = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_dwell_interval;
        fhss_configuration.fhss_broadcast_interval = neighbor_info->ws_neighbor->fhss_data.bc_timing_info.broadcast_interval;
        fhss_configuration.broadcast_fixed_channel = cur->ws_info->cfg->fhss.fhss_bc_fixed_channel;
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
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER || cur->ws_info->configuration_learned || !neighbor_info->ws_neighbor->broadcast_shedule_info_stored || !neighbor_info->ws_neighbor->broadcast_timing_info_stored) {
        return;
    }

    if (ws_bootstrap_candidate_parent_get(cur, neighbor_info->neighbor->mac64, false) == NULL) {
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

        tr_info("Generated random MAC address");
    }
    tr_info("MAC address: %s", trace_array(mac64, 8));
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
uint16_t ws_local_etx_read(protocol_interface_info_entry_t *interface, addrtype_t addr_type, const uint8_t *mac_adddress)
{
    uint16_t etx;

    if (!mac_adddress || !interface) {
        return 0;
    }

    uint8_t attribute_index;

    mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(interface), mac_adddress, addr_type);
    if (!mac_neighbor) {
        return 0xffff;
    }
    attribute_index = mac_neighbor->index;
    ws_neighbor_class_entry_t *ws_neighbour = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, attribute_index);
    etx_storage_t *etx_entry = etx_storage_entry_get(interface->id, attribute_index);

    if (!ws_neighbour || !etx_entry) {
        return 0xffff;
    }

    etx = etx_local_etx_read(interface->id, attribute_index);

    // if we have a measurement ready then we will check the RSL validity
    if (etx != 0xffff && !ws_neighbour->candidate_parent) {
        // RSL value measured is lower than acceptable ETX will be given as MAX
        return WS_ETX_MAX << 1; // We use 8 bit fraction and ETX is usually 7 bit fraction
    }

    // If we dont have valid ETX for children we assume good ETX.
    // After enough packets is sent to children real calculated ETX is given.
    // This might result in ICMP source route errors returned to Border router causing secondary route uses
    if (etx == 0xffff && ipv6_neighbour_has_registered_by_eui64(&interface->ipv6_neighbour_cache, mac_neighbor->mac64)) {
        return 0x100;
    }

    return etx;
}

uint16_t ws_etx_read(protocol_interface_info_entry_t *interface, addrtype_t addr_type, const uint8_t *addr_ptr)
{
    if (!addr_ptr || !interface) {
        return 0;
    }
    return ws_local_etx_read(interface, addr_type, addr_ptr + PAN_ID_LEN);
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

static void ws_bootstrap_dhcp_neighbour_update_cb(int8_t interface_id, uint8_t ll_addr[static 16])
{
    if (memcmp(ll_addr, ADDR_LINK_LOCAL_PREFIX, 8)) {
        return;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    uint8_t mac64[8];
    memcpy(mac64, ll_addr + 8, 8);
    mac64[0] ^= 2;
    ws_bootstrap_mac_neighbor_short_time_set(cur, mac64, WS_NEIGHBOUR_DHCP_ENTRY_LIFETIME);
}

static void ws_bootstrap_dhcp_info_notify_cb(int8_t interface, dhcp_option_notify_t *options, dhcp_server_notify_info_t *server_info)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface);
    if (!cur) {
        return;
    }
    uint8_t server_ll64[16];
    memcpy(server_ll64, ADDR_LINK_LOCAL_PREFIX, 8);

    if (server_info->duid_length == 8) {
        memcpy(server_ll64 + 8, server_info->duid, 8);
    } else {
        server_ll64[8] = server_info->duid[0];
        server_ll64[9] = server_info->duid[1];
        server_ll64[10] = server_info->duid[2];
        server_ll64[11] = 0xff;
        server_ll64[12] = 0xfe;
        server_ll64[13] = server_info->duid[3];
        server_ll64[14] = server_info->duid[4];
        server_ll64[15] = server_info->duid[5];
    }
    server_ll64[8] ^= 2;

    switch (options->option_type) {
        case DHCPV6_OPTION_VENDOR_SPECIFIC_INFO:
            if (options->option.vendor_spesific.enterprise_number != ARM_ENTERPRISE_NUMBER) {
                break;
            }
            while (options->option.vendor_spesific.data_length) {
                uint16_t option_type;
                char *domain;
                uint8_t *address;
                uint16_t option_len;
                option_len = net_dns_option_vendor_option_data_get_next(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &option_type);
                tr_debug("DHCP vendor specific data type:%u length %d", option_type, option_len);
                //tr_debug("DHCP vendor specific data %s", trace_array(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length));

                if (option_len == 0) {
                    // Option fields were corrupted
                    break;
                }
                if (option_type == ARM_DHCP_VENDOR_DATA_DNS_QUERY_RESULT) {
                    // Process ARM DNS query result
                    domain = NULL;
                    address = NULL;
                    if (net_dns_option_vendor_option_data_dns_query_read(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &address, &domain) > 0 ||
                            domain || address) {
                        // Valid ARM DNS query entry
                        net_dns_query_result_set(interface, address, domain, server_info->life_time);
                    }
                }
                if (option_type == ARM_DHCP_VENDOR_DATA_NETWORK_TIME) {
                    // Process ARM Network Time
                    // Get Current time
                    // Get Round trip time of the DHCP request
                    // Estimated error is elapsed time of request
                    // If current time difference is larger than estimated error update current time
                    //     set the time for server time + *.5 RTT
                    int32_t era;
                    uint32_t offset;
                    if (net_vendor_option_current_time_read(options->option.vendor_spesific.data, options->option.vendor_spesific.data_length, &era, &offset, NULL)) {
                        uint64_t current_time;
                        uint64_t network_time = (era * (uint64_t)(4294967296)) + offset - 2208988800; //Convert to First day of Unix (1 Jan 1970)

                        tr_debug("Network Time option Era:%"PRId32" Offset:%"PRIu32" rtt: %"PRId32" time: %"PRIu64,  era, offset, server_info->rtt, network_time);
                        if (0 == ns_time_system_time_read(&current_time)) {
                            uint64_t difference;
                            // We only adjust clock if time has drifted more than 10 seconds to avoid constant changing of time
                            // If Round trip time is very high the accuracy is reduced.
                            uint32_t estimated_error = 10 + server_info->rtt / 10;
                            // Take into account the round trip time it took the response to arrive from the time server Write the time.
                            network_time += server_info->rtt / 20;

                            if (current_time > network_time) {
                                difference = current_time - network_time;
                            } else {
                                difference = network_time - current_time;
                            }
                            if (difference > estimated_error) {
                                // Larger than 10 second difference update the time
                                int ret = ns_time_system_time_write(network_time);
                                tr_info("Network Time %s: Era:%"PRId32" Offset:%"PRIu32" old time: %"PRIu64" time: %"PRIu64, ret == 0 ? "updated" : "update FAILED", era, offset, current_time, network_time);
                            }
                        }
                    }
                }

                options->option.vendor_spesific.data_length -= option_len;
                options->option.vendor_spesific.data += option_len;
            }
            break;

        case DHCPV6_OPTION_DNS_SERVERS:
            while (options->option.generic.data_length && options->option.generic.data_length >= 16 && options->option.generic.data_length % 16 == 0) {
                // Validate payload to have full 16 byte length addresses without any extra bytes
                net_dns_server_address_set(interface, server_ll64, options->option.generic.data, server_info->life_time);
                options->option.generic.data_length -= 16;
                options->option.generic.data += 16;
            }
            break;
        case DHCPV6_OPTION_DOMAIN_LIST:
            net_dns_server_search_list_set(interface, server_ll64, options->option.generic.data, options->option.generic.data_length, server_info->life_time);
            break;
        default:
            break;
    }

}

static void ws_bootstrap_memory_configuration()
{
    /* Configure memory limits for garbage collection based on total memory size
     * Starting from these values
     *      5% for High mark
     *      2% for critical mark
     *      1% for Routing limit
     * Memory     High               Critical            Drop routing
     * 32K RAM    3200 bytes         1280 Bytes          1024 bytes
     * 64K RAM    3200 bytes         1280 Bytes          1024 bytes
     * 128K RAM   6400 bytes         2560 Bytes          1280 bytes
     * 320K RAM   16000 byte         6400 Bytes          3200 bytes
     * 640K RAM   32000 byte         12800 Bytes         6400 bytes
     * 1000K RAM  50000 bytes        20000 Bytes         10000 bytes
     * 4000K RAM  120000 bytes       40000 Bytes         10000 bytes
     * */
    // In small memory devices there needs to lower limit so that there some change to be usable
    // and there is no use for having very large values on high memory devices
    ns_monitor_packet_ingress_rate_limit_by_memory(1024, 10000, 1);

    ns_monitor_heap_gc_threshold_set(3200, 120000, 95, 1280, 40000, 98);
    return;
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
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //BBR init like NVM read
        ws_bbr_init(cur);
    }
    // Save FHSS api
    cur->ws_info->fhss_api = ns_sw_mac_get_fhss_api(cur->mac_api);

    ws_bootstrap_ll_address_validate(cur);

    addr_interface_set_ll64(cur, NULL);
    cur->nwk_nd_re_scan_count = 0;
    // Trigger discovery for bootstrap
    ret_val = nwk_6lowpan_up(cur);
    if (ret_val) {
        goto cleanup;
    }

    /* Wi-sun will trig event for stamechine this timer must be zero on init */
    cur->bootsrap_state_machine_cnt = 0;
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

    dhcp_client_init(cur->id, DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE);
    dhcp_service_link_local_rx_cb_set(cur->id, ws_bootstrap_dhcp_neighbour_update_cb);
    dhcp_client_configure(cur->id, true, true, true); //RENEW uses SOLICIT, Interface will use 1 instance for address get, IAID address hint is not used.

    dhcp_client_solicit_timeout_set(cur->id, WS_DHCP_SOLICIT_TIMEOUT, WS_DHCP_SOLICIT_MAX_RT, WS_DHCP_SOLICIT_MAX_RC, WS_DHCP_SOLICIT_MAX_DELAY);
    dhcp_client_option_notification_cb_set(cur->id, ws_bootstrap_dhcp_info_notify_cb);

    // Configure memory limits and garbage collection values;
    ws_bootstrap_memory_configuration();
    ws_nud_table_reset(cur);

    ws_bootstrap_candidate_table_reset(cur);
    // Zero uptime counters
    cur->ws_info->uptime = 0;
    cur->ws_info->authentication_time = 0;
    cur->ws_info->connected_time = 0;

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

void ws_bootstrap_disconnect(protocol_interface_info_entry_t *cur, ws_bootsrap_event_type_e event_type)
{
    if (cur->nwk_bootstrap_state == ER_RPL_NETWORK_LEAVING) {
        //Already moved to leaving state.
        return;
    }
    // We are no longer connected
    cur->ws_info->connected_time = 0;

    if (cur->rpl_domain && cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
        //Stop Asych Timer
        ws_bootstrap_asynch_trickle_stop(cur);
        tr_debug("Start Network soft leaving");
        if (event_type == WS_FAST_DISCONNECT) {
            rpl_control_instant_poison(cur, cur->rpl_domain);
            cur->bootsrap_state_machine_cnt = 80; //Give 8 seconds time to send Poison
        } else {
            rpl_control_poison(cur->rpl_domain, 1);
            cur->bootsrap_state_machine_cnt = 6000; //Give 10 minutes time for poison if RPL is not report
        }

    } else {
        ws_bootstrap_event_discovery_start(cur);
    }
    cur->nwk_bootstrap_state = ER_RPL_NETWORK_LEAVING;
}


static void ws_bootstrap_asynch_trickle_stop(protocol_interface_info_entry_t *cur)
{
    cur->ws_info->trickle_pas_running = false;
    cur->ws_info->trickle_pa_running = false;
    cur->ws_info->trickle_pcs_running = false;
    cur->ws_info->trickle_pc_running = false;
    cur->ws_info->trickle_pc_consistency_block_period = 0;
}

static int8_t ws_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    tr_info("Wi-SUN ifdown");
    // Reset MAC for safe upper layer memory free
    protocol_mac_reset(cur);
    ns_sw_mac_fhss_unregister(cur->mac_api);
    ns_fhss_delete(cur->ws_info->fhss_api);
    cur->ws_info->fhss_api = NULL;
    // Reset WS information
    ws_bootstrap_asynch_trickle_stop(cur);
    ws_llc_reset(cur);
    if (nd_proxy_downstream_interface_unregister(cur->id) != 0) {
        tr_warn("nd proxy unregister failed");
    }
    ws_nud_table_reset(cur);
    dhcp_client_delete(cur->id);
    ws_eapol_relay_delete(cur);
    ws_eapol_auth_relay_delete(cur);
    ws_pae_controller_stop(cur);
    ws_bootstrap_candidate_table_reset(cur);
    blacklist_clear();
    cur->if_common_forwarding_out_cb = NULL;

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
    ws_bootstrap_asynch_trickle_stop(cur);

    //cur->mac_security_key_usage_update_cb = ws_management_mac_security_key_update_cb;
    return;
}

static bool ws_bootstrap_network_name_matches(const struct mcps_data_ie_list *ie_ext, const char *network_name_ptr)
{
    ws_wp_network_name_t network_name;

    if (!network_name_ptr || !ie_ext) {
        return false;
    }

    if (!ws_wp_nested_network_name_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &network_name)) {
        tr_warn("No network name IE");
        return false;
    }

    if (network_name.network_name_length != strlen(network_name_ptr)) {
        return false;
    }

    if (strncmp(network_name_ptr, (char *)network_name.network_name, network_name.network_name_length) != 0) {
        return false;
    }

    // names have equal length and same characters
    return true;
}

static void ws_bootstrap_pan_advertisement_analyse_active(struct protocol_interface_info_entry *cur, ws_pan_information_t *pan_information)
{
    /* In Active state
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
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //Border router never set consistent that will guarantee that BR will send advertisment
        return;
    }
#ifdef WISUN_1_0_ERRATA_FIX
    if (pan_information->pan_size == cur->ws_info->pan_information.pan_size) {
        //If same pan size information then set consistent value
        trickle_consistent_heard(&cur->ws_info->trickle_pan_advertisement);
    }
#else
    // Wi-SUN 1.0 specified functionality, causes extra inconsistencies when we hear higher rank advertisements
    if (pan_information->routing_cost >= ws_bootstrap_routing_cost_calculate(cur)) {
        trickle_consistent_heard(&cur->ws_info->trickle_pan_advertisement);
    } else {
        trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);
    }
#endif
}

static parent_info_t *ws_bootstrap_candidate_parent_get_best(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_reserved) {
        tr_info("candidate list a:%s panid:%x cost:%d size:%d rssi:%d txFailure:%u age:%"PRIu32, trace_array(entry->addr, 8), entry->pan_id, entry->pan_information.routing_cost, entry->pan_information.pan_size, entry->signal_dbm, entry->tx_fail, protocol_core_monotonic_time - entry->age);
    }

    return ns_list_get_first(&cur->ws_info->parent_list_reserved);
}

static void ws_bootstrap_decode_exclude_range_to_mask_by_range(void *mask_buffer, ws_excluded_channel_range_t *range_info, uint16_t number_of_channels)
{
    uint16_t range_start, range_stop;
    uint8_t mask_index = 0;
    //uint8_t channel_index = 0;
    uint8_t *range_ptr = range_info->range_start;
    uint32_t *mask_ptr = mask_buffer;
    while (range_info->number_of_range) {
        range_start = common_read_16_bit_inverse(range_ptr);
        range_ptr += 2;
        range_stop = common_read_16_bit_inverse(range_ptr);
        range_ptr += 2;
        range_info->number_of_range--;
        for (uint16_t channel = 0; channel < number_of_channels; channel++) {

            if (channel && (channel % 32 == 0)) {
                mask_index++;
                //channel_index = 0;
            }
            if (channel >= range_start && channel <= range_stop) {
                //mask_ptr[mask_index] |= 1 << (31 - channel_index);
                mask_ptr[0 + (channel / 32)] |= 1 << (31 - (channel % 32));
            } else if (channel > range_stop) {
                break;
            }
        }
    }
}

static void ws_bootstrap_candidate_parent_store(parent_info_t *parent, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us, ws_pan_information_t *pan_information)
{
    parent->ws_utt = *ws_utt;
    // Saved from unicast IE
    parent->ws_us = *ws_us;

    //copy excluded channel here if it is inline
    if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
        memset(parent->excluded_channel_data, 0, 32);
        //Decode Range to mask here
        ws_bootstrap_decode_exclude_range_to_mask_by_range(parent->excluded_channel_data, &parent->ws_us.excluded_channels.range, 256);
        parent->ws_us.excluded_channels.mask.channel_mask = parent->excluded_channel_data;
        parent->ws_us.excluded_channels.mask.mask_len_inline = 32;
        parent->ws_us.excluded_channel_ctrl = WS_EXC_CHAN_CTRL_BITMASK;
    } else if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
        parent->ws_us.excluded_channels.mask.channel_mask = parent->excluded_channel_data;
        memcpy(parent->excluded_channel_data, ws_us->excluded_channels.mask.channel_mask, ws_us->excluded_channels.mask.mask_len_inline);
    }

    // Saved from Pan information, do not overwrite pan_version as it is not valid here
    parent->pan_information.pan_size = pan_information->pan_size;
    parent->pan_information.routing_cost = pan_information->routing_cost;
    parent->pan_information.use_parent_bs = pan_information->use_parent_bs;
    parent->pan_information.rpl_routing_method = pan_information->rpl_routing_method;
    parent->pan_information.version = pan_information->version;

    // Saved from message
    parent->timestamp = data->timestamp;
    parent->pan_id = data->SrcPANId;
    parent->link_quality = data->mpduLinkQuality;
    parent->signal_dbm = data->signal_dbm;
    memcpy(parent->addr, data->SrcAddr, 8);

    if (ws_neighbor_class_rsl_from_dbm_calculate(parent->signal_dbm) > (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERISIS)) {
        parent->link_acceptable = true;
    }
    if (ws_neighbor_class_rsl_from_dbm_calculate(parent->signal_dbm) < (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD - CAND_PARENT_HYSTERISIS)) {
        parent->link_acceptable = false;
    }
    parent->age = protocol_core_monotonic_time;
}

static void ws_bootstrap_candidate_table_reset(protocol_interface_info_entry_t *cur)
{
    //Empty active list
    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_free) {
        ns_list_remove(&cur->ws_info->parent_list_free, entry);
    }

    //Empty free list
    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_reserved) {
        ns_list_remove(&cur->ws_info->parent_list_reserved, entry);
    }
    //Add to free list to full
    for (int i = 0; i < WS_PARENT_LIST_SIZE; i++) {
        ns_list_add_to_end(&cur->ws_info->parent_list_free, &cur->ws_info->parent_info[i]);
    }
}

static parent_info_t *ws_bootstrap_candidate_parent_allocate(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    parent_info_t *entry = ns_list_get_first(&cur->ws_info->parent_list_free);
    if (entry) {
        memcpy(entry->addr, addr, 8);
        ns_list_remove(&cur->ws_info->parent_list_free, entry);
        ns_list_add_to_end(&cur->ws_info->parent_list_reserved, entry);
    } else {
        // If there is no free entries always allocate the last one of reserved as it is the worst
        entry = ns_list_get_last(&cur->ws_info->parent_list_reserved);

    }
    if (entry) {
        entry->tx_fail = 0;
        entry->link_acceptable = false;
    }
    return entry;
}

static parent_info_t *ws_bootstrap_candidate_parent_get(struct protocol_interface_info_entry *cur, const uint8_t *addr, bool create)
{
    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_reserved) {
        if (memcmp(entry->addr, addr, 8) == 0) {
            return entry;
        }
    }
    if (create) {
        return ws_bootstrap_candidate_parent_allocate(cur, addr);
    }
    return NULL;
}

static void ws_bootstrap_candidate_parent_mark_failure(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    parent_info_t *entry = ws_bootstrap_candidate_parent_get(cur, addr, false);
    if (entry) {
        if (entry->tx_fail >= 2) {
            ns_list_remove(&cur->ws_info->parent_list_reserved, entry);
            ns_list_add_to_end(&cur->ws_info->parent_list_free, entry);
        } else {
            entry->tx_fail++;
            ws_bootstrap_candidate_parent_sort(cur, entry);
        }

    }
}

static bool ws_bootstrap_candidate_parent_compare(parent_info_t *p1, parent_info_t *p2)
{
    // Return true if P2 is better
    // signal lower than threshold for both
    // pan_cost
    // signal quality

    if (p2->tx_fail > p1->tx_fail) {
        return false;
    }

    if (p2->tx_fail < p1->tx_fail) {
        return true;
    }

    if (p1->link_acceptable && !p2->link_acceptable) {
        // Link acceptable is always better than not
        return true;
    }
    if (!p1->link_acceptable && p2->link_acceptable) {
        // Link acceptable is always better than not
        return false;
    }

    // Select the lowest PAN cost
    uint16_t p1_pan_cost = (p1->pan_information.routing_cost / PRC_WEIGHT_FACTOR) + (p1->pan_information.pan_size / PS_WEIGHT_FACTOR);
    uint16_t p2_pan_cost = (p2->pan_information.routing_cost / PRC_WEIGHT_FACTOR) + (p2->pan_information.pan_size / PS_WEIGHT_FACTOR);
    if (p1_pan_cost > p2_pan_cost) {
        return true;
    } else if (p1_pan_cost < p2_pan_cost) {
        return false;
    }

    // If pan cost is the same then we select the one we hear highest
    if (p1->signal_dbm < p2->signal_dbm) {
        return true;
    }
    return false;
}

static void ws_bootstrap_candidate_list_clean(struct protocol_interface_info_entry *cur, uint8_t pan_max, uint32_t current_time, uint16_t pan_id)
{
    int pan_count = 0;

    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_reserved) {

        if ((current_time - entry->age) > WS_PARENT_LIST_MAX_AGE) {
            ns_list_remove(&cur->ws_info->parent_list_reserved, entry);
            ns_list_add_to_end(&cur->ws_info->parent_list_free, entry);
            continue;
        }
        if (entry->pan_id == pan_id) {
            // Same panid if there is more than limited amount free those
            pan_count++;
            if (pan_count > pan_max) {
                ns_list_remove(&cur->ws_info->parent_list_reserved, entry);
                ns_list_add_to_end(&cur->ws_info->parent_list_free, entry);
                continue;
            }
        }
    }
}

static void ws_bootstrap_candidate_parent_sort(struct protocol_interface_info_entry *cur, parent_info_t *new_entry)
{
    //Remove from the list

    ns_list_foreach_safe(parent_info_t, entry, &cur->ws_info->parent_list_reserved) {

        if (entry == new_entry) {
            // own entry skip it
            continue;
        }

        if (ws_bootstrap_candidate_parent_compare(entry, new_entry)) {
            // New entry is better
            //tr_debug("candidate list new is better");
            ns_list_remove(&cur->ws_info->parent_list_reserved, new_entry);
            ns_list_add_before(&cur->ws_info->parent_list_reserved, entry, new_entry);
            return;
        }
    }
    // This is the last entry
    ns_list_remove(&cur->ws_info->parent_list_reserved, new_entry);
    ns_list_add_to_end(&cur->ws_info->parent_list_reserved, new_entry);
}

static void ws_bootstrap_pan_information_store(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us, ws_pan_information_t *pan_information)
{

    parent_info_t *new_entry;
    /* Have List of 20 heard neighbours
     * Order those as best based on pan cost
     * In single pan order based on signal quality
     * in single PAN limit the amount of devices to 5
     * If there is no advertisement heard for last hour Clear the neigbour.
     */

    // Discovery state processing
    //tr_info("neighbour: addr:%s panid:%x signal:%d", trace_array(data->SrcAddr, 8), data->SrcPANId, data->signal_dbm);

    // Clean old entries
    ws_bootstrap_candidate_list_clean(cur, WS_PARENT_LIST_MAX_PAN_IN_DISCOVERY, protocol_core_monotonic_time, data->SrcPANId);

    new_entry = ws_bootstrap_candidate_parent_get(cur, data->SrcAddr, true);
    if (!new_entry) {
        tr_warn("neighbour creation fail");
        return;
    }
    // Safe the information
    ws_bootstrap_candidate_parent_store(new_entry, data, ws_utt, ws_us, pan_information);
    if (!new_entry->link_acceptable) {
        // This entry is either poor quality or changed to poor quality link so we will remove this
        // Todo in future possibility to try poor link parents if we have not found any good link parents
        tr_info("neighbour not accepted: addr:%s panid:%x rsl:%d device_min_sens: %d", trace_array(new_entry->addr, 8), new_entry->pan_id, ws_neighbor_class_rsl_from_dbm_calculate(new_entry->signal_dbm), DEVICE_MIN_SENS);
        ns_list_remove(&cur->ws_info->parent_list_reserved, new_entry);
        ns_list_add_to_end(&cur->ws_info->parent_list_free, new_entry);
        return;
    }
    // set to the correct place in list
    ws_bootstrap_candidate_parent_sort(cur, new_entry);

    return;
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

    if (ws_us->excluded_channel_ctrl) {
        //Validate that we can storage data
        if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK && ws_us->excluded_channels.mask.mask_len_inline > 32) {
            return;
        }
    }

    // Check pan flags so that it is valid
    if (!pan_information.rpl_routing_method) {
        // NOT RPL routing
        //tr_warn("Not supported routing");
        return;
    }

    // Store heard pans and possible candidate parents
    ws_bootstrap_pan_information_store(cur, data, ws_utt, ws_us, &pan_information);

    if (!(ws_bootstrap_state_active(cur) ||
            ws_bootstrap_state_wait_rpl(cur))) {
        // During discovery/eapol/config learn we dont do further processing for advertisements
        return;
    }
    // Active state processing
    //tr_debug("Advertisement active");

    // In active operation less neighbours per pan is allowed
    ws_bootstrap_candidate_list_clean(cur, WS_PARENT_LIST_MAX_PAN_IN_ACTIVE, protocol_core_monotonic_time, data->SrcPANId);

    // Check if valid PAN
    if (data->SrcPANId != cur->ws_info->network_pan_id) {
        return;
    }

    // Save route cost for all known neighbors
    llc_neighbour_req_t neighbor_info;
    neighbor_info.neighbor = NULL;
    if (ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false)) {
        neighbor_info.ws_neighbor->routing_cost = pan_information.routing_cost;
    }

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
    /*
     *  Optimized PAN discovery to select the parent faster if we hear solicit from someone else
     */

    if (ws_bootstrap_state_discovery(cur)  && ws_cfg_network_config_get(cur) <= CONFIG_MEDIUM &&
            cur->bootsrap_state_machine_cnt > cur->ws_info->trickle_params_pan_discovery.Imin * 2) {

        cur->bootsrap_state_machine_cnt = cur->ws_info->trickle_params_pan_discovery.Imin + randLIB_get_random_in_range(0, cur->ws_info->trickle_params_pan_discovery.Imin);

        tr_info("Making parent selection in %u s", (cur->bootsrap_state_machine_cnt / 10));
    }

    if (ws_bootstrap_state_active(cur) && cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(cur), data->SrcAddr, ADDR_802_15_4_LONG);
        if (neighbor && neighbor->link_role == PRIORITY_PARENT_NEIGHBOUR) {
            ws_bootstrap_parent_confirm(cur, NULL);
        }
    }
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

    //Validate BSI
    if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {

        if (cur->ws_info->ws_bsi_block.block_time && cur->ws_info->ws_bsi_block.old_bsi == ws_bs_ie.broadcast_schedule_identifier) {
            tr_debug("Do not accept a old BSI: %u in time %"PRIu32, cur->ws_info->ws_bsi_block.old_bsi, cur->ws_info->ws_bsi_block.block_time);
            //Refresh Block time when hear a old BSI
            cur->ws_info->ws_bsi_block.block_time = cur->ws_info->cfg->timing.pan_timeout;
            return;
        }

        //When Config is learned and USE Parent BS is enabled compare is this new BSI
        if (cur->ws_info->configuration_learned && cur->ws_info->pan_information.use_parent_bs && ws_bs_ie.broadcast_schedule_identifier != cur->ws_info->hopping_schdule.fhss_bsi) {
            //Accept only next possible BSI number
            if ((cur->ws_info->hopping_schdule.fhss_bsi + 1) != ws_bs_ie.broadcast_schedule_identifier) {
                tr_debug("Do not accept a unknown BSI: %u", ws_bs_ie.broadcast_schedule_identifier);
            } else {
                tr_debug("NEW Brodcast Schedule %u...BR rebooted", ws_bs_ie.broadcast_schedule_identifier);
                cur->ws_info->ws_bsi_block.block_time = cur->ws_info->cfg->timing.pan_timeout;
                cur->ws_info->ws_bsi_block.old_bsi = cur->ws_info->hopping_schdule.fhss_bsi;
                ws_bootstrap_event_disconnect(cur, WS_NORMAL_DISCONNECT);
            }
            return;
        }
    }


    if (cur->ws_info->configuration_learned || cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        //If we are border router or learned configuration we only update already learned neighbours.
        neighbour_pointer_valid = ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false);

    } else {
        neighbour_pointer_valid = ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, true);
        if (!neighbour_pointer_valid) {
            return;
        }
        ws_bootstrap_neighbor_set_stable(cur, data->SrcAddr);
    }

    if (neighbour_pointer_valid) {
        //Update Neighbor Broadcast and Unicast Parameters
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp, (uint8_t *) data->SrcAddr);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us, &cur->ws_info->hopping_schdule);
        ws_neighbor_class_neighbor_broadcast_time_info_update(neighbor_info.ws_neighbor, &ws_bt_ie, data->timestamp);
        ws_neighbor_class_neighbor_broadcast_schedule_set(neighbor_info.ws_neighbor, &ws_bs_ie);
    }

    if (cur->ws_info->configuration_learned) {
        tr_info("PAN Config analyse own:%d, heard:%d", cur->ws_info->pan_information.pan_version, pan_version);
        if (cur->ws_info->pan_information.pan_version == pan_version) {
            //Check if Trgigle have been resetted in short time skip this then
            if (cur->ws_info->trickle_pc_consistency_block_period == 0) {
                // Same version heard so it is consistent
                trickle_consistent_heard(&cur->ws_info->trickle_pan_config);
            }

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
            cur->ws_info->trickle_pc_consistency_block_period = WS_CONFIG_CONSISTENT_FILTER_PERIOD;
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
    //Check Here Do we have a selected Primary parent
    if (!cur->ws_info->configuration_learned || cur->ws_info->rpl_state == RPL_EVENT_DAO_DONE) {
        ws_common_border_router_alive_update(cur);
    }

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
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp, (uint8_t *) data->SrcAddr);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us, &cur->ws_info->hopping_schdule);
    }

    if (ws_bootstrap_state_active(cur) && cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(cur), data->SrcAddr, ADDR_802_15_4_LONG);
        if (neighbor && neighbor->link_role == PRIORITY_PARENT_NEIGHBOUR) {
            ws_bootstrap_parent_confirm(cur, NULL);
        }
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

bool ws_bootstrap_validate_channel_plan(ws_us_ie_t *ws_us, struct protocol_interface_info_entry *cur)
{
    if (ws_us->channel_plan == 0) {
        if (!ws_channel_plan_zero_compare(&ws_us->plan.zero, &cur->ws_info->hopping_schdule)) {
            return false;
        }
    } else if (ws_us->channel_plan == 1) {
        if (!ws_channel_plan_one_compare(&ws_us->plan.one, &cur->ws_info->hopping_schdule)) {
            return false;
        }
    }

    return true;
}

bool ws_bootstrap_validate_channel_function(ws_us_ie_t *ws_us, ws_bs_ie_t *ws_bs)
{
    if (ws_us) {
        if (ws_us->channel_function != WS_FIXED_CHANNEL &&
                ws_us->channel_function != WS_TR51CF &&
                ws_us->channel_function != WS_DH1CF) {
            return false;
        }
    }

    if (ws_bs) {
        if (ws_bs->channel_function != WS_FIXED_CHANNEL &&
                ws_bs->channel_function != WS_TR51CF &&
                ws_bs->channel_function != WS_DH1CF) {
            return false;
        }
    }

    return true;
}

static void ws_bootstrap_asynch_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type)
{
    // Store weakest heard packet RSSI
    if (cur->ws_info->weakest_received_rssi > data->signal_dbm) {
        cur->ws_info->weakest_received_rssi = data->signal_dbm;
    }

    if (data->SrcAddrMode != MAC_ADDR_MODE_64_BIT) {
        // Not from long address
        return;
    }
    ws_stats_update(cur, STATS_WS_ASYNCH_RX, 1);
    //Validate network name
    switch (message_type) {
        case WS_FT_PAN_ADVERT:
        case WS_FT_PAN_ADVERT_SOL:
        case WS_FT_PAN_CONF_SOL:
            //Check Network Name
            if (!ws_bootstrap_network_name_matches(ie_ext, cur->ws_info->cfg->gen.network_name)) {
                // Not in our network
                return;
            }
            break;
        case WS_FT_PAN_CONF:
            break;
        default:
            return;
    }
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

    if (!ws_bootstrap_validate_channel_plan(&ws_us, cur) ||
            !ws_bootstrap_validate_channel_function(&ws_us, NULL)) {
        return;
    }

    //Handle Message's
    switch (message_type) {
        case WS_FT_PAN_ADVERT:
            // Analyse Advertisement
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PA, 1);
            tr_info("received ADVERT Src:%s panid:%x rssi:%d", trace_array(data->SrcAddr, 8), data->SrcPANId, data->signal_dbm);
            ws_bootstrap_pan_advertisement_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_ADVERT_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PAS, 1);
            tr_info("received ADVERT SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_advertisement_solicit_analyse(cur, data, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PC, 1);
            tr_info("received CONFIG Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_config_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PCS, 1);
            tr_info("received CONFIG SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_pan_config_solicit_analyse(cur, data, &ws_utt, &ws_us);
        default:
            // Unknown message do not process
            break;
    }
}

static void ws_bootstrap_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message)
{
    ws_stats_update(interface, STATS_WS_ASYNCH_TX, 1);
    if (interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        if (asynch_message == WS_FT_PAN_CONF && interface->ws_info->pending_key_index_info.state == PENDING_KEY_INDEX_ACTIVATE) {
            interface->ws_info->pending_key_index_info.state = NO_PENDING_PROCESS;
            tr_info("Activate new default key %u", interface->ws_info->pending_key_index_info.index + 1);
            mac_helper_security_auto_request_key_index_set(interface, interface->ws_info->pending_key_index_info.index, interface->ws_info->pending_key_index_info.index + 1);
        }
    }
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

    if (mac_neighbor_info(interface)->neighbour_list_size <= mac_neighbor_info(interface)->list_total_size - ws_common_temporary_entry_size(mac_neighbor_info(interface)->list_total_size)) {
        // Enough neighbor entries
        return;
    }
    uint32_t temp_link_min_timeout;
    if (mac_neighbor_info(interface)->neighbour_list_size == mac_neighbor_info(interface)->list_total_size) {
        temp_link_min_timeout = 1; //Accept 1 second time from last
    } else {
        temp_link_min_timeout = interface->ws_info->cfg->timing.temp_link_min_timeout;
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

        if (cur->nud_active) {
            //If NUD process is active do not trig
            // or Negative ARO is active
            continue;
        }

        if (neighbor_entry_ptr && neighbor_entry_ptr->lifetime < cur->lifetime) {
            // We have already shorter link entry found this cannot replace it
            continue;
        }

        if (cur->link_lifetime > WS_NEIGHBOUR_TEMPORARY_ENTRY_LIFETIME && cur->link_lifetime <= WS_NEIGHBOUR_TEMPORARY_NEIGH_MAX_LIFETIME) {
            //Do not permit to remove configured temp life time
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
        if (time_from_last_unicast_shedule >= temp_link_min_timeout) {
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
    ws_stats_update(interface, STATS_WS_NEIGHBOUR_ADD, 1);
    return true;
}

static void ws_neighbor_entry_remove_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{

    protocol_interface_info_entry_t *cur = user_data;
    lowpan_adaptation_neigh_remove_free_tx_tables(cur, entry_ptr);
    // Sleepy host
    if (cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        mac_data_poll_protocol_poll_mode_decrement(cur);
    }

    //TODO State machine check here

    if (ipv6_neighbour_has_registered_by_eui64(&cur->ipv6_neighbour_cache, entry_ptr->mac64)) {
        // Child entry deleted
        ws_stats_update(cur, STATS_WS_CHILD_REMOVE, 1);
    }

    if (entry_ptr->ffd_device) {
        protocol_6lowpan_release_short_link_address_from_neighcache(cur, entry_ptr->mac16);
        protocol_6lowpan_release_long_link_address_from_neighcache(cur, entry_ptr->mac64);
    }

    //NUD Process Clear Here
    ws_nud_entry_remove(cur, entry_ptr);

    ws_bootstrap_neighbor_delete(cur, entry_ptr);
    ws_stats_update(cur, STATS_WS_NEIGHBOUR_REMOVE, 1);

}

static uint32_t ws_probe_init_time_get(protocol_interface_info_entry_t *cur)
{
    if (ws_cfg_network_config_get(cur) <= CONFIG_SMALL) {
        return WS_SMALL_PROBE_INIT_BASE_SECONDS;
    }

    return WS_NORMAL_PROBE_INIT_BASE_SECONDS;
}

static bool ws_neighbor_entry_nud_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{
    uint32_t time_from_start = entry_ptr->link_lifetime - entry_ptr->lifetime;
    uint8_t ll_address[16];
    bool nud_proces = false;
    bool activate_nud = false;
    bool child;
    bool candidate_parent;
    protocol_interface_info_entry_t *cur = user_data;

    ws_neighbor_class_entry_t *ws_neighbor = ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, entry_ptr->index);
    etx_storage_t *etx_entry = etx_storage_entry_get(cur->id, entry_ptr->index);

    if (!entry_ptr->trusted_device || !ws_neighbor || !etx_entry || entry_ptr->link_lifetime <= WS_NEIGHBOUR_TEMPORARY_NEIGH_MAX_LIFETIME) {
        return false;
    }

    if (lowpan_adaptation_expedite_forward_state_get(cur)) {
        //Do not send any probe or NUD when Expedite forward state is enabled
        return false;
    }

    ws_bootsrap_create_ll_address(ll_address, entry_ptr->mac64);

    if (time_from_start > WS_NEIGHBOR_NUD_TIMEOUT) {

        child = ipv6_neighbour_has_registered_by_eui64(&cur->ipv6_neighbour_cache, entry_ptr->mac64);
        candidate_parent = rpl_control_is_dodag_parent_candidate(cur, ll_address, cur->ws_info->cfg->gen.rpl_parent_candidate_max);
        /* For parents ARO registration is sent in link timeout times
         * For candidate parents NUD is needed
         * For children NUD is sent only at very close to end
         */
        if (!child && !candidate_parent) {
            // No need for keep alive
            return false;
        }
        if (child && (time_from_start < WS_NEIGHBOR_NUD_TIMEOUT * 1.8)) {
            /* This is our child with valid ARO registration send NUD if we are close to delete
             *
             * if ARO was received link is considered active so this is only in case of very long ARO registration times
             *
             * 1.8 means with link timeout of 30 minutes that NUD is sent 6 minutes before timeout
             *
             */
            return false;
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
        nud_proces = activate_nud;
    } else if (etx_entry->etx_samples < WS_NEIGHBOR_ETX_SAMPLE_MAX) {
        //Take Random number for trig a prope.
        //Small network
        //ETX Sample 0: random 1-4
        //ETX Sample 1: random 2-8
        //ETX Sample 2: random 4-16
        //Medium and large
        //ETX Sample 0: random 1-8
        //ETX Sample 1: random 2-16
        //ETX Sample 2: random 4-32

        ws_bootsrap_create_ll_address(ll_address, entry_ptr->mac64);
        if (!rpl_control_probe_parent_candidate(cur, ll_address)) {
            return false;
        }

        uint32_t probe_period = ws_probe_init_time_get(cur) << etx_entry->etx_samples;
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

    if (!activate_nud) {
        return false;
    }

    ws_nud_table_entry_t *entry = ws_nud_entry_get_free(cur);
    if (!entry) {
        return false;
    }
    entry->neighbor_info = entry_ptr;

    entry->nud_process = nud_proces;

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

    if (ns_sw_mac_enable_frame_counter_per_key(cur->mac_api, true)) {
        return -1;
    }

    if (!etx_storage_list_allocate(cur->id, buffer.device_decription_table_size)) {
        return -1;
    }
    if (!etx_cached_etx_parameter_set(WS_ETX_MIN_WAIT_TIME, WS_ETX_MIN_SAMPLE_COUNT, WS_NEIGHBOR_FIRST_ETX_SAMPLE_MIN_COUNT)) {
        etx_storage_list_allocate(cur->id, 0);
        return -1;
    }

    if (!etx_allow_drop_for_poor_measurements(WS_ETX_BAD_INIT_LINK_LEVEL, WS_ETX_MAX_BAD_LINK_DROP)) {
        etx_storage_list_allocate(cur->id, 0);
        return -1;
    }

    etx_max_update_set(WS_ETX_MAX_UPDATE);
    etx_max_set(WS_ETX_MAX);

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

    if (ws_cfg_settings_interface_set(cur) < 0) {
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
    if (ws_pae_controller_cb_register(cur, &ws_bootstrap_authentication_completed, &ws_bootstrap_authentication_next_target, &ws_bootstrap_nw_key_set, &ws_bootstrap_nw_key_clear, &ws_bootstrap_nw_key_index_set, &ws_bootstrap_nw_frame_counter_set, &ws_bootstrap_nw_frame_counter_read, &ws_bootstrap_pan_version_increment, &ws_bootstrap_nw_info_updated, &ws_bootstrap_eapol_congestion_get) < 0) {
        ret_val =  -4;
        goto init_fail;
    }
    if (ws_pae_controller_configure(cur, &cur->ws_info->cfg->sec_timer, &cur->ws_info->cfg->sec_prot, &cur->ws_info->cfg->timing) < 0) {
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

    cur->ipv6_neighbour_cache.link_mtu = cur->max_link_mtu = WS_MPX_MAX_MTU;

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

    // Specification is ruling out the compression mode, but we are now doing it.
    cur->mpl_seed = true;
    cur->mpl_seed_id_mode = MULTICAST_MPL_SEED_ID_IPV6_SRC_FOR_DOMAIN;

    cur->mpl_control_trickle_params.TimerExpirations = 0;

    cur->mpl_domain = mpl_domain_create(cur, ADDR_ALL_MPL_FORWARDERS, NULL, MULTICAST_MPL_SEED_ID_DEFAULT, -1, 0, NULL, NULL);
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

int ws_bootstrap_restart_delayed(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->ws_info) {
        return -1;
    }
    ws_bootstrap_state_change(cur, ER_WAIT_RESTART);
    cur->bootsrap_state_machine_cnt = 3;
    return 0;
}

int ws_bootstrap_set_rf_config(protocol_interface_info_entry_t *cur, phy_rf_channel_configuration_s rf_configs)
{
    mlme_set_t set_request;
    // Set MAC mode
    phy_802_15_4_mode_t mac_mode = IEEE_802_15_4G_2012;
    set_request.attr = mac802_15_4Mode;
    set_request.value_pointer = &mac_mode;
    set_request.value_size = sizeof(phy_802_15_4_mode_t);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_request);
    // Set RF configuration
    set_request.attr = macRfConfiguration;
    set_request.value_pointer = &rf_configs;
    set_request.value_size = sizeof(phy_rf_channel_configuration_s);
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_request);
    // Set Ack wait duration
    uint8_t bits_per_symbol = 1;
    if (rf_configs.modulation == M_OFDM) {
        bits_per_symbol = 4;
    }
    uint16_t ack_wait_symbols = WS_ACK_WAIT_SYMBOLS + (WS_TACK_MAX_MS * (rf_configs.datarate / 1000) / bits_per_symbol);
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
    // Start automatic CCA threshold
    mac_helper_start_auto_cca_threshold(cur->id, cur->ws_info->hopping_schdule.number_of_channels, CCA_DEFAULT_DBM, CCA_HIGH_LIMIT, CCA_LOW_LIMIT);
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
    rpl_control_neighbor_delete(cur, ll_address);
    ws_bootstrap_neighbor_remove(cur, ll_address);
    return 0;
}

static int ws_bootstrap_set_domain_rf_config(protocol_interface_info_entry_t *cur)
{
    phy_rf_channel_configuration_s rf_configs;
    memset(&rf_configs, 0, sizeof(phy_rf_channel_configuration_s));

    uint8_t phy_mode_id = cur->ws_info->hopping_schdule.phy_mode_id;
    if (((phy_mode_id >= 34) && (phy_mode_id <= 38)) ||
            ((phy_mode_id >= 51) && (phy_mode_id <= 54)) ||
            ((phy_mode_id >= 68) && (phy_mode_id <= 70)) ||
            ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
        rf_configs.modulation = M_OFDM;
        rf_configs.datarate = ws_get_datarate_using_phy_mode_id(cur->ws_info->hopping_schdule.phy_mode_id);
        rf_configs.ofdm_option = ws_get_ofdm_option_using_phy_mode_id(cur->ws_info->hopping_schdule.phy_mode_id);
        rf_configs.ofdm_mcs = ws_get_ofdm_mcs_using_phy_mode_id(cur->ws_info->hopping_schdule.phy_mode_id);
    } else {
        if ((phy_mode_id >= 17) && (phy_mode_id <= 24)) {
            rf_configs.fec = true;
        } else {
            rf_configs.fec = false;
        }
        rf_configs.modulation = M_2FSK;
        rf_configs.datarate = ws_get_datarate_using_operating_mode(cur->ws_info->hopping_schdule.operating_mode);
        rf_configs.modulation_index = ws_get_modulation_index_using_operating_mode(cur->ws_info->hopping_schdule.operating_mode);
    }

    rf_configs.channel_0_center_frequency = (uint32_t)cur->ws_info->hopping_schdule.ch0_freq * 100000;
    rf_configs.channel_spacing = ws_decode_channel_spacing(cur->ws_info->hopping_schdule.channel_spacing);
    rf_configs.number_of_channels = cur->ws_info->hopping_schdule.number_of_channels;
    ws_bootstrap_set_rf_config(cur, rf_configs);
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
    ws_bootstrap_mac_activate(cur, cur->ws_info->cfg->fhss.fhss_uc_fixed_channel, cur->ws_info->network_pan_id, true);
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
    if (own_hop == 1) {
        // Allow transmitting unicast frames only on TX slots in normal mode and always in expedited forwarding mode for first hop
        ns_fhss_ws_set_tx_allowance_level(cur->ws_info->fhss_api, WS_TX_SLOT, WS_TX_ALWAYS);
    } else {
        // Allow transmitting unicast frames only on TX slots in normal and expedited forwarding mode for other hops
        ns_fhss_ws_set_tx_allowance_level(cur->ws_info->fhss_api, WS_TX_SLOT, WS_TX_SLOT);
    }
    tr_debug("own hop: %u, own rank: %u, rank inc: %u", own_hop, own_rank, rank_inc);
}

static void ws_address_registration_update(protocol_interface_info_entry_t *interface, const uint8_t addr[16])
{
    rpl_control_register_address(interface, addr);
    // Timer is used only to track full registrations

    if (addr != NULL && interface->ws_info->aro_registration_timer) {
        // Single address update and timer is running
        return;
    }

    if (interface->ws_info->aro_registration_timer == 0) {
        // Timer expired and check if we have valid address to register
        ns_list_foreach(if_address_entry_t, address, &interface->ip_addresses) {
            if (!addr_is_ipv6_link_local(address->address)) {
                // We have still valid addresses let the timer run for next period
                tr_info("ARO registration timer start");
                interface->ws_info->aro_registration_timer = WS_NEIGHBOR_NUD_TIMEOUT;
                return;
            }
        }
    }
}

static void ws_address_parent_update(protocol_interface_info_entry_t *interface)
{
    tr_info("RPL parent update ... register ARO");
    ws_address_registration_update(interface, NULL);
}

static void ws_bootstrap_parent_confirm(protocol_interface_info_entry_t *cur, struct rpl_instance *instance)
{
    /* Possible problem with the parent connection
     * Give some time for parent to rejoin and confirm the connection with ARO and DAO
     */
    const rpl_dodag_conf_t *config = NULL;
    uint32_t Imin_secs = 0;

    if (!ws_bootstrap_state_active(cur)) {
        // If we are not in Active state no need to confirm parent
        return;
    }

    tr_info("RPL parent confirm");

    if (!instance) {
        // If we dont have instance we take any available to get reference
        instance = rpl_control_enumerate_instances(cur->rpl_domain, NULL);
    }

    if (instance) {
        config = rpl_control_get_dodag_config(instance);
    }

    if (config) {
        //dio imin Period caluclate in seconds
        uint32_t Imin_ms = config->dio_interval_min < 32 ? (1ul << config->dio_interval_min) : 0xfffffffful;
        //Covert to seconds and multiple by 2 so we give time to recovery so divide by 500 do that operation
        Imin_secs = (Imin_ms + 499) / 500;

        if (Imin_secs > 0xffff) {
            Imin_secs = 0xffff;
        }
    }
    if (Imin_secs == 0) {
        // If we dont have RPL configuration we assume conservative value
        Imin_secs = 60;
    }

    /*Speed up the ARO registration*/
    if (cur->ws_info->aro_registration_timer > Imin_secs) {
        cur->ws_info->aro_registration_timer = Imin_secs;
    }
}

static void ws_rpl_parent_dis_callback(const uint8_t *ll_parent_address, void *handle, struct rpl_instance *instance)
{
    (void) ll_parent_address;
    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return;
    }
    //Multicast DIS from parent indicate that Parent is not valid in short time window possible
    ws_bootstrap_parent_confirm(cur, instance);
}


static void ws_bootstrap_rpl_callback(rpl_event_t event, void *handle)
{

    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return;
    }

    if (event == RPL_EVENT_POISON_FINISHED) {
        //If we are waiting poison we will trig Discovery after couple seconds
        if (cur->nwk_bootstrap_state == ER_RPL_NETWORK_LEAVING) {
            cur->bootsrap_state_machine_cnt = 80; //Give 8 seconds time to send Poison
        }
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
            ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->pan_information.pan_version, cur->ws_info->cfg->gen.network_name);
            // Network key is valid, indicate border router IID to controller
            ws_pae_controller_nw_key_valid(cur, &dodag_info.dodag_id[8]);
            //Update here Suplikant target by validated Primary Parent
            if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                mac_neighbor_table_entry_t *mac_neighbor = mac_neighbor_entry_get_priority(mac_neighbor_info(cur));
                if (mac_neighbor) {
                    ws_pae_controller_set_target(cur, cur->ws_info->network_pan_id, mac_neighbor->mac64);
                }
            }

            // After successful DAO ACK connection to border router is verified
            ws_common_border_router_alive_update(cur);
        }

        if (!cur->ws_info->trickle_pa_running || !cur->ws_info->trickle_pc_running) {
            //Enable wi-sun asynch adverisment
            ws_bootstrap_advertise_start(cur);
        }

        ws_set_fhss_hop(cur);
        // Set retry configuration for bootstrap ready state
        ws_bootstrap_configure_max_retries(cur, WS_MAX_FRAME_RETRIES);
        // Set TX failure request restart configuration
        ws_bootstrap_configure_data_request_restart(cur, WS_CCA_REQUEST_RESTART_MAX, WS_TX_REQUEST_RESTART_MAX, WS_REQUEST_RESTART_BLACKLIST_MIN, WS_REQUEST_RESTART_BLACKLIST_MAX);
    } else if (event == RPL_EVENT_LOCAL_REPAIR_NO_MORE_DIS) {
        /*
         * RPL goes to passive mode, but does not require any extra changed
         *
         * We could remove our current addresses learned from RPL
         * We could send solicit for configuration and then select new parent when those arrive
         *
         */

    } else if (event == RPL_EVENT_LOCAL_REPAIR_START) {
        tr_debug("RPL local repair start");
        //Disable Async and go to state 4 to confirm parent connection
        ws_bootstrap_parent_confirm(cur, NULL);
        // Move to state 4 if we see issues with primary parent
        if (ws_bootstrap_state_active(cur)) {
            tr_info("Move state 4 to wait parent connection confirmation");
            ws_bootstrap_rpl_scan_start(cur);
            ws_nwk_event_post(cur, ARM_NWK_NWK_CONNECTION_DOWN);
        }
    } else if (event == RPL_EVENT_DAO_PARENT_ADD) {
        ws_address_parent_update(cur);
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
            ws_address_reregister_trig(cur);
        }
    } else {
        //Delete dhcpv6 client
        dhcp_client_global_address_delete(interface, dhcp_addr, prefix);
    }
}


void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local)
{
    if (dhcp_client_get_global_address(cur->id, parent_link_local, prefix, ws_dhcp_client_global_adress_cb) != 0) {
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

static bool ws_rpl_candidate_soft_filtering(protocol_interface_info_entry_t *cur, struct rpl_instance *instance)
{
    //Already many candidates
    uint16_t candidate_list_size = rpl_control_candidate_list_size(cur, instance);
    if (candidate_list_size >= cur->ws_info->cfg->gen.rpl_parent_candidate_max) {
        return false;
    }

    uint16_t selected_parents = rpl_control_selected_parent_count(cur, instance);

    //Already enough selected candidates
    if (selected_parents >= cur->ws_info->cfg->gen.rpl_selected_parent_max) {
        candidate_list_size -= selected_parents;
        if (candidate_list_size >= 2) {
            //We have more candidates than selected
            return false;
        }
    }

    return true;
}

static bool ws_rpl_new_parent_callback(uint8_t *ll_parent_address, void *handle, struct rpl_instance *instance, uint16_t candidate_rank)
{

    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return false;
    }

    if (blacklist_reject(ll_parent_address)) {
        // Rejected by blacklist
        return false;
    }

    uint8_t mac64[10];
    //bool replace_ok = false;
    //bool create_ok = false;
    llc_neighbour_req_t neigh_buffer;

    //Discover neigh ready here for possible ETX validate
    memcpy(mac64, ll_parent_address + 8, 8);
    mac64[0] ^= 2;


    ws_bootstrap_neighbor_info_request(cur, mac64, &neigh_buffer, false);
    //Discover Multicast temporary entry for create neighbour table entry for new candidate
    ws_neighbor_temp_class_t *entry = ws_llc_get_multicast_temp_entry(cur, mac64);

    if (!ws_rpl_candidate_soft_filtering(cur, instance)) {

        //Acept only better than own rank here
        if (candidate_rank >= rpl_control_current_rank(instance)) {
            //Do not accept no more siblings
            return false;
        }

        uint16_t candidate_list_size = rpl_control_candidate_list_size(cur, instance);
        if (candidate_list_size > cur->ws_info->cfg->gen.rpl_parent_candidate_max + 1) {
            //Accept only 1 better 1 time
            return false;
        }

        if (!neigh_buffer.neighbor) {
            //Do not accept any new in that Place
            return false;
        }

        uint8_t replacing[16];
        //Accept Know neighbour if it is enough good
        if (!rpl_control_find_worst_neighbor(cur, instance, replacing)) {
            return false;
        }
        // +2 Is for PAN ID space
        memcpy(mac64, replacing + 8, 8);
        mac64[0] ^= 2;

        if (ws_local_etx_read(cur, ADDR_802_15_4_LONG, mac64) == 0xffff) {
            //Not probed yet because ETX is 0xffff
            return false;
        }

        uint16_t etx = 0;
        if (neigh_buffer.neighbor) {
            etx = ws_local_etx_read(cur, ADDR_802_15_4_LONG, neigh_buffer.neighbor->mac64);
        }

        // Accept now only better one's when max candidates selected and max candidate list size is reached
        return rpl_possible_better_candidate(cur, instance, replacing, candidate_rank, etx);
    }

    //Neighbour allready
    if (neigh_buffer.neighbor) {
        return true;
    }

    if (!entry) {
        //No Multicast Entry Available
        return false;
    }

    //Create entry
    bool create_ok = ws_bootstrap_neighbor_info_request(cur, entry->mac64, &neigh_buffer, true);
    if (create_ok) {
        ws_neighbor_class_entry_t *ws_neigh = neigh_buffer.ws_neighbor;
        ws_bootstrap_neighbor_set_stable(cur, entry->mac64);
        //Copy fhss temporary data
        *ws_neigh = entry->neigh_info_list;
        mac_neighbor_table_trusted_neighbor(mac_neighbor_info(cur), neigh_buffer.neighbor, true);
    }
    ws_llc_free_multicast_temp_entry(cur, entry);

#if 0
neigh_create_ok:

    if (create_ok && replace_ok) {
        //Try remove here when accepted new better one possible
        tr_debug("Remove %s by %s", trace_ipv6(replacing), trace_ipv6(ll_parent_address));
        rpl_control_neighbor_delete_from_instance(cur, instance, replacing);
    }
#endif
    return create_ok;
}

static void ws_bootstrap_rpl_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("RPL Activate");
    bool downstream = true;
    bool leaf = false;

    addr_add_router_groups(cur);
    rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, downstream);
    rpl_control_set_callback(protocol_6lowpan_rpl_domain, ws_bootstrap_rpl_callback, ws_rpl_prefix_callback, ws_rpl_new_parent_callback, ws_rpl_parent_dis_callback, cur);
    // If i am router I Do this
    rpl_control_force_leaf(protocol_6lowpan_rpl_domain, leaf);
    rpl_control_process_routes(protocol_6lowpan_rpl_domain, false); // Wi-SUN assumes that no default route needed
    rpl_control_request_parent_link_confirmation(true);
    rpl_control_set_dio_multicast_min_config_advertisment_count(WS_MIN_DIO_MULTICAST_CONFIG_ADVERTISMENT_COUNT);
    rpl_control_set_address_registration_timeout((WS_NEIGHBOR_LINK_TIMEOUT / 60) + 1);
    rpl_control_set_dao_retry_count(WS_MAX_DAO_RETRIES);
    rpl_control_set_initial_dao_ack_wait(WS_MAX_DAO_INITIAL_TIMEOUT);
    rpl_control_set_mrhof_parent_set_size(WS_MAX_PARENT_SET_COUNT);
    rpl_control_set_force_tunnel(true);
    if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        rpl_control_set_memory_limits(WS_NODE_RPL_SOFT_MEM_LIMIT, WS_NODE_RPL_HARD_MEM_LIMIT);
    }
    // Set RPL Link ETX Validation Threshold to 2.5 - 33.0
    // This setup will set ETX 0x800 to report ICMP error 18% probability
    // When ETX start go over 0x280 forward dropping probability start increase  linear to 100% at 0x2100
    rpl_policy_forward_link_etx_threshold_set(0x280, 0x2100);

    // Set the minimum target refresh to sen DAO registrations before pan timeout
    rpl_control_set_minimum_dao_target_refresh(WS_RPL_DAO_MAX_TIMOUT);

    cur->ws_info->rpl_state = 0xff; // Set invalid state and learn from event
}

static void ws_bootstrap_network_start(protocol_interface_info_entry_t *cur)
{
    //Set Network names, Pan information configure, hopping schedule & GTKHash
    ws_llc_set_network_name(cur, (uint8_t *)cur->ws_info->cfg->gen.network_name, strlen(cur->ws_info->cfg->gen.network_name));
    ws_llc_set_pan_information_pointer(cur, &cur->ws_info->pan_information);
}

static void ws_bootstrap_network_discovery_configure(protocol_interface_info_entry_t *cur)
{
    // Reset information to defaults
    cur->ws_info->network_pan_id = 0xffff;

    ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule);
    ws_bootstrap_set_domain_rf_config(cur);
    ws_fhss_configure(cur, true);

    //Set Network names, Pan information configure, hopping schedule & GTKHash
    ws_llc_set_network_name(cur, (uint8_t *)cur->ws_info->cfg->gen.network_name, strlen(cur->ws_info->cfg->gen.network_name));
}


static void ws_bootstrap_advertise_start(protocol_interface_info_entry_t *cur)
{
    cur->ws_info->trickle_pa_running = true;
    trickle_start(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);
    cur->ws_info->trickle_pc_running = true;
    cur->ws_info->trickle_pc_consistency_block_period = 0;
    trickle_start(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
}

static void ws_bootstrap_pan_version_increment(protocol_interface_info_entry_t *cur)
{
    (void)cur;
    ws_bbr_pan_version_increase(cur);
}

// Start network scan
static void ws_bootstrap_start_discovery(protocol_interface_info_entry_t *cur)
{
    tr_debug("router discovery start");
    // Remove network keys from MAC
    ws_pae_controller_nw_keys_remove(cur);
    ws_bootstrap_state_change(cur, ER_ACTIVE_SCAN);
    cur->nwk_nd_re_scan_count = 0;
    cur->ws_info->configuration_learned = false;
    cur->ws_info->pan_timeout_timer = 0;
    cur->ws_info->weakest_received_rssi = 0;

    // Clear learned candidate parents
    ws_bootstrap_candidate_table_reset(cur);

    // Clear RPL information
    rpl_control_free_domain_instances_from_interface(cur);
    // Clear EAPOL relay address
    ws_eapol_relay_delete(cur);

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
    uint32_t time_to_solicit = 0;
    if (cur->ws_info->trickle_pan_advertisement_solicit.t > cur->ws_info->trickle_pan_advertisement_solicit.now) {
        time_to_solicit = cur->ws_info->trickle_pan_advertisement_solicit.t - cur->ws_info->trickle_pan_advertisement_solicit.now;
    }

    tr_debug("Disc params imin %u, imax %u, expirations %u, k %u PAS Trickle I %u t %u, now %u, c %u",
             cur->ws_info->trickle_params_pan_discovery.Imin, cur->ws_info->trickle_params_pan_discovery.Imax, cur->ws_info->trickle_params_pan_discovery.TimerExpirations, cur->ws_info->trickle_params_pan_discovery.k,
             cur->ws_info->trickle_pan_advertisement_solicit.I, cur->ws_info->trickle_pan_advertisement_solicit.t, cur->ws_info->trickle_pan_advertisement_solicit.now, cur->ws_info->trickle_pan_advertisement_solicit.c);

    time_to_solicit += cur->ws_info->trickle_params_pan_discovery.Imin + randLIB_get_random_in_range(0, cur->ws_info->trickle_params_pan_discovery.Imin);

    if (time_to_solicit > 0xffff) {
        time_to_solicit = 0xffff;
    }
    cur->bootsrap_state_machine_cnt = time_to_solicit;

    tr_info("Making parent selection in %u s", (cur->bootsrap_state_machine_cnt / 10));
}

// Start authentication
static void ws_bootstrap_start_authentication(protocol_interface_info_entry_t *cur)
{
    // Set PAN ID and network name to controller
    ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->pan_information.pan_version, cur->ws_info->cfg->gen.network_name);

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

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        if (cur->mac_parameters->mac_default_key_index != 0 && cur->mac_parameters->mac_default_key_index  != index + 1) {
            tr_info("New Pending key Request %u", index + 1);
            cur->ws_info->pending_key_index_info.state = PENDING_KEY_INDEX_ADVERTISMENT;
            cur->ws_info->pending_key_index_info.index = index;
            return;
        }
    }

    mac_helper_security_auto_request_key_index_set(cur, index, index + 1);
}

static void ws_bootstrap_nw_frame_counter_set(protocol_interface_info_entry_t *cur, uint32_t counter, uint8_t slot)
{
    // Set frame counter
    mac_helper_key_link_frame_counter_set(cur->id, counter, slot);
}

static void ws_bootstrap_nw_frame_counter_read(protocol_interface_info_entry_t *cur, uint32_t *counter, uint8_t slot)
{
    // Read frame counter
    mac_helper_key_link_frame_counter_read(cur->id, counter, slot);
}

static void ws_bootstrap_nw_info_updated(protocol_interface_info_entry_t *cur, uint16_t pan_id, uint16_t pan_version, char *network_name)
{
    /* For border router, the PAE controller reads PAN ID, PAN version and network name from storage.
     * If they are set, takes them into use here.
     */
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        // Get PAN ID and network name
        ws_gen_cfg_t gen_cfg;
        if (ws_cfg_gen_get(&gen_cfg, NULL) < 0) {
            return;
        }

        // If PAN ID has not been set, set it
        if (gen_cfg.network_pan_id == 0xffff) {
            gen_cfg.network_pan_id = pan_id;
            // Sets PAN version
            cur->ws_info->pan_information.pan_version = pan_version;
            cur->ws_info->pan_information.pan_version_set = true;
        }

        // If network name has not been set, set it
        if (strlen(gen_cfg.network_name) == 0) {
            strncpy(gen_cfg.network_name, network_name, 32);
        }

        // Stores the settings
        ws_cfg_gen_set(cur, NULL, &gen_cfg, 0);
    }
}

static void ws_bootstrap_authentication_completed(protocol_interface_info_entry_t *cur, auth_result_e result, uint8_t *target_eui_64)
{
    if (result == AUTH_RESULT_OK) {
        tr_debug("authentication success eui64:%s", trace_array(target_eui_64, 8));
        if (target_eui_64) {
            // Authentication was made contacting the authenticator
            cur->ws_info->authentication_time = cur->ws_info->uptime;
        }
        ws_bootstrap_event_configuration_start(cur);
    } else if (result == AUTH_RESULT_ERR_TX_ERR) {
        // eapol parent selected is not working
        tr_debug("authentication TX failed");

        ws_bootstrap_candidate_parent_mark_failure(cur, target_eui_64);
        // Go back for network scanning
        ws_bootstrap_state_change(cur, ER_ACTIVE_SCAN);

        // Start PAS interval between imin - imax.
        cur->ws_info->trickle_pas_running = true;
        trickle_start(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);

        // Parent selection is made before imin/2 so if there is parent candidates solicit is not sent
        cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(10, cur->ws_info->trickle_params_pan_discovery.Imin >> 1);
        tr_info("Making parent selection in %u s", (cur->bootsrap_state_machine_cnt / 10));
    } else {
        tr_debug("authentication failed");
        // What else to do to start over again...
        // Trickle is reseted when entering to discovery from state 2
        trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);
        ws_bootstrap_event_discovery_start(cur);
    }
}

static const uint8_t *ws_bootstrap_authentication_next_target(protocol_interface_info_entry_t *cur, const uint8_t *previous_eui_64, uint16_t *pan_id)
{
    ws_bootstrap_candidate_parent_mark_failure(cur, previous_eui_64);

    // Gets best target
    parent_info_t *parent_info = ws_bootstrap_candidate_parent_get_best(cur);
    if (parent_info) {
        /* On failure still continues with the new parent, and on next call,
           will try to set the neighbor again */
        ws_bootstrap_neighbor_set(cur, parent_info, true);
        *pan_id = parent_info->pan_id;
        return parent_info->addr;
    }

    // If no targets found, retries the last one
    return previous_eui_64;
}

static void ws_bootstrap_eapol_congestion_init(protocol_interface_info_entry_t *cur)
{
    random_early_detection_free(cur->llc_random_early_detection);
    cur->llc_random_early_detection = NULL;

    if (cur->llc_random_early_detection == NULL) {
        cur->llc_random_early_detection = random_early_detection_create(
                                              cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_min,
                                              cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_max,
                                              100, RED_AVERAGE_WEIGHT_EIGHTH);
    }

    random_early_detection_free(cur->llc_eapol_random_early_detection);
    cur->llc_eapol_random_early_detection = NULL;

    if (cur->llc_eapol_random_early_detection == NULL) {
        cur->llc_eapol_random_early_detection = random_early_detection_create(
                                                    cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_min,
                                                    cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_max,
                                                    100, RED_AVERAGE_WEIGHT_EIGHTH);
    }
}

static bool ws_bootstrap_eapol_congestion_get(protocol_interface_info_entry_t *cur, uint16_t active_supp)
{
    if (cur == NULL || cur->random_early_detection == NULL || cur->llc_random_early_detection == NULL || cur->llc_eapol_random_early_detection == NULL) {
        return false;
    }

    bool return_value = false;
    static struct red_info_s *red_info = NULL;
    uint16_t adaptation_average = 0;
    uint16_t llc_average = 0;
    uint16_t llc_eapol_average = 0;
    uint16_t average_sum = 0;
    uint8_t active_max = 0;

    //TODO implement API for HEAP info request
    uint32_t heap_size;
    const mem_stat_t *mem_stats = ns_dyn_mem_get_mem_stat();
    if (mem_stats) {
        heap_size = mem_stats->heap_sector_size;
    } else {
        heap_size = 0;
    }

    /*
      * For different memory sizes the max simultaneous authentications will be
      * 32k:    (32k / 50k) * 2 + 1 = 1
      * 65k:    (65k / 50k) * 2 + 1 = 3
      * 250k:   (250k / 50k) * 2 + 1 = 11
      * 1000k:  (1000k / 50k) * 2 + 1 = 41
      * 2000k:  (2000k / 50k) * 2 + 1 = 50 (upper limit)
      */
    active_max = (heap_size / 50000) * 2 + 1;
    if (active_max > 50) {
        active_max = 50;
    }

    // Read the values for adaptation and LLC queues
    adaptation_average = random_early_detetction_aq_read(cur->random_early_detection);
    llc_average = random_early_detetction_aq_read(cur->llc_random_early_detection);
    llc_eapol_average  = random_early_detetction_aq_read(cur->llc_eapol_random_early_detection);
    // Calculate combined average
    average_sum = adaptation_average + llc_average + llc_eapol_average;

    // Maximum for active supplicants based on memory reached, fail
    if (active_supp >= active_max) {
        return_value = true;
        goto congestion_get_end;
    }

    // Always allow at least five negotiations (if memory does not limit)
    if (active_supp < 5) {
        goto congestion_get_end;
    }

    if (red_info == NULL) {
        red_info = random_early_detection_create(
                       cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_min,
                       cur->ws_info->cfg->sec_prot.max_simult_sec_neg_tx_queue_max,
                       100, RED_AVERAGE_WEIGHT_DISABLED);
    }
    if (red_info == NULL) {
        goto congestion_get_end;
    }

    // Check drop probability
    average_sum = random_early_detetction_aq_calc(red_info, average_sum);
    return_value = random_early_detection_congestion_check(red_info);

congestion_get_end:
    tr_info("Active supplicant limit, active: %i max: %i summed averageQ: %i adapt averageQ: %i LLC averageQ: %i LLC EAPOL averageQ: %i drop: %s", active_supp, active_max, average_sum, adaptation_average, llc_average, llc_eapol_average, return_value ? "T" : "F");

    return return_value;
}

// Start configuration learning
static void ws_bootstrap_start_configuration_learn(protocol_interface_info_entry_t *cur)
{
    tr_debug("router configuration learn start");
    ws_bootstrap_state_change(cur, ER_SCAN);

    cur->ws_info->configuration_learned = false;

    // Clear all temporary information
    ws_bootstrap_ip_stack_reset(cur);

    cur->ws_info->pas_requests = 0;
    //Calculate max time for config learn state
    cur->ws_info->pan_config_sol_max_timeout = trickle_timer_max(&cur->ws_info->trickle_params_pan_discovery, PCS_MAX);
    // Reset advertisement solicit trickle to start discovering network
    cur->ws_info->trickle_pcs_running = true;
    trickle_start(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery);
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery);
}
static void ws_bootstrap_rpl_scan_start(protocol_interface_info_entry_t *cur)
{
    tr_debug("Start RPL learn");
    // Stop Trickle timers
    ws_bootstrap_asynch_trickle_stop(cur);

    // routers wait until RPL root is contacted
    ws_bootstrap_state_change(cur, ER_RPL_SCAN);
    // Change state as the state is checked in state machine
    cur->ws_info->rpl_state = RPL_EVENT_LOCAL_REPAIR_START;
    //For Large network and medium should do passive scan
    if (ws_cfg_network_config_get(cur) > CONFIG_SMALL) {
        // Set timeout for check to 30 - 60 seconds
        cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(WS_RPL_DIS_INITIAL_TIMEOUT / 2, WS_RPL_DIS_INITIAL_TIMEOUT);
    }
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

void ws_bootstrap_event_disconnect(protocol_interface_info_entry_t *cur, ws_bootsrap_event_type_e event_type)
{
    ws_bootsrap_event_trig(event_type, cur->bootStrapId, ARM_LIB_LOW_PRIORITY_EVENT, NULL);
}

void ws_bootstrap_configuration_trickle_reset(protocol_interface_info_entry_t *cur)
{
    trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
}

static void ws_set_asynch_channel_list(protocol_interface_info_entry_t *cur, asynch_request_t *async_req)
{
    memset(&async_req->channel_list, 0, sizeof(channel_list_s));
    if (cur->ws_info->cfg->fhss.fhss_uc_channel_function == WS_FIXED_CHANNEL) {
        //SET 1 Channel only
        uint16_t channel_number = cur->ws_info->cfg->fhss.fhss_uc_fixed_channel;
        async_req->channel_list.channel_mask[0 + (channel_number / 32)] = (1 << (channel_number % 32));
    } else {
        ws_generate_channel_list(async_req->channel_list.channel_mask, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->hopping_schdule.regulatory_domain, cur->ws_info->hopping_schdule.operating_class, cur->ws_info->hopping_schdule.channel_plan_id);
    }

    async_req->channel_list.channel_page = CHANNEL_PAGE_10;
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

    ws_set_asynch_channel_list(cur, &async_req);


    async_req.security.SecurityLevel = 0;

    ws_stats_update(cur, STATS_WS_ASYNCH_TX_PAS, 1);
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

    ws_set_asynch_channel_list(cur, &async_req);
    async_req.security.SecurityLevel = 0;

    ws_stats_update(cur, STATS_WS_ASYNCH_TX_PCS, 1);
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

    uint16_t etx = ws_local_etx_read(cur, ADDR_802_15_4_LONG, mac_neighbor->mac64);
    if (etx == 0) {
        etx = WS_ETX_MAX; //SET maximum value here if ETX is unknown
    } else {
        //Scale to 128 based ETX (local read return 0x100 - 0xffff
        etx = etx >> 1;
    }
    // Make the 0xffff as maximum value
    if (ws_neighbor->routing_cost + etx > 0xffff) {
        return 0xffff;
    }

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

    ws_set_asynch_channel_list(cur, &async_req);
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

    ws_stats_update(cur, STATS_WS_ASYNCH_TX_PA, 1);
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

    ws_set_asynch_channel_list(cur, &async_req);

    async_req.security.SecurityLevel = mac_helper_default_security_level_get(cur);
    async_req.security.KeyIdMode = mac_helper_default_security_key_id_mode_get(cur);
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER && cur->ws_info->pending_key_index_info.state == PENDING_KEY_INDEX_ADVERTISMENT) {
        async_req.security.KeyIndex =  cur->ws_info->pending_key_index_info.index + 1;
        cur->ws_info->pending_key_index_info.state = PENDING_KEY_INDEX_ACTIVATE;
    } else {
        async_req.security.KeyIndex = mac_helper_default_key_index_get(cur);
    }

    ws_stats_update(cur, STATS_WS_ASYNCH_TX_PC, 1);
    ws_llc_asynch_request(cur, &async_req);
}

static int8_t ws_bootstrap_backbone_ip_addr_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *address)
{
    (void) interface_ptr;
    (void) address;

    if (ws_bbr_backbone_address_get(address)) {
        return 0;
    }

    return -1;
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
            protocol_mac_reset(cur);
            ws_llc_reset(cur);
            lowpan_adaptation_interface_reset(cur->id);
            //Clear Pending Key Index State
            cur->ws_info->pending_key_index_info.state = NO_PENDING_PROCESS;
            cur->mac_parameters->mac_default_key_index = 0;

            ipv6_destination_cache_clean(cur->id);

            // Clear parent blacklist
            blacklist_clear();

            // All trickle timers stopped to allow entry from any state
            ws_bootstrap_asynch_trickle_stop(cur);
            //Init Packet congestion
            ws_bootstrap_packet_congestion_init(cur);

            if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                tr_info("Border router start network");


                if (!ws_bbr_ready_to_start(cur)) {
                    // Wi-SUN not started yet we wait for Border router permission
                    ws_bootstrap_state_change(cur, ER_WAIT_RESTART);
                    cur->nwk_nd_re_scan_count = randLIB_get_random_in_range(40, 100);
                    return;
                }
                // Clear Old information from stack

                ws_nud_table_reset(cur);
                ws_bootstrap_neighbor_list_clean(cur);
                ws_bootstrap_ip_stack_reset(cur);
                ws_pae_controller_auth_init(cur);

                if (cur->ws_info->cfg->gen.network_pan_id == 0xffff) {
                    cur->ws_info->network_pan_id = randLIB_get_random_in_range(0, 0xfffd);
                } else {
                    cur->ws_info->network_pan_id = cur->ws_info->cfg->gen.network_pan_id;
                }
                if (!cur->ws_info->pan_information.pan_version_set) {
                    cur->ws_info->pan_information.pan_version = randLIB_get_random_in_range(0, 0xffff);
                    cur->ws_info->pan_information.pan_version_set = true;
                }
                cur->ws_info->pan_information.pan_size = 0;
                cur->ws_info->pan_information.routing_cost = 0;
                cur->ws_info->pan_information.rpl_routing_method = true;
                cur->ws_info->pan_information.use_parent_bs = true;
                cur->ws_info->pan_information.version = WS_FAN_VERSION_1_0;

                uint8_t *gtkhash = ws_pae_controller_gtk_hash_ptr_get(cur);
                ws_llc_set_gtkhash(cur, gtkhash);
                ws_bbr_pan_version_increase(cur);

                // Set default parameters for FHSS when starting a discovery
                ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule);
                ws_fhss_border_router_configure(cur);
                ws_bootstrap_set_domain_rf_config(cur);
                ws_bootstrap_fhss_activate(cur);

                uint8_t ll_addr[16];
                addr_interface_get_ll_address(cur, ll_addr, 1);

                //SET EAPOL authenticator EUI64
                ws_pae_controller_border_router_addr_write(cur, cur->mac);

                // Set EAPOL relay to port 10255 and authenticator relay to 10253 (and to own ll address)
                ws_eapol_relay_start(cur, BR_EAPOL_RELAY_SOCKET_PORT, ll_addr, EAPOL_RELAY_SOCKET_PORT);

                // Set authenticator relay to port 10253 and PAE to 10254 (and to own ll address)
                ws_eapol_auth_relay_start(cur, EAPOL_RELAY_SOCKET_PORT, ll_addr, PAE_AUTH_SOCKET_PORT);

                // Set PAN ID and network name to controller
                ws_pae_controller_nw_info_set(cur, cur->ws_info->network_pan_id, cur->ws_info->pan_information.pan_version, cur->ws_info->cfg->gen.network_name);

                // Set backbone IP address get callback
                ws_pae_controller_auth_cb_register(cur, ws_bootstrap_backbone_ip_addr_get);

                // Set PAE port to 10254 and authenticator relay to 10253 (and to own ll address)
                ws_pae_controller_authenticator_start(cur, PAE_AUTH_SOCKET_PORT, ll_addr, EAPOL_RELAY_SOCKET_PORT);

                // Initialize eapol congestion tracking
                ws_bootstrap_eapol_congestion_init(cur);

                // Set retry configuration for bootstrap ready state
                ws_bootstrap_configure_max_retries(cur, WS_MAX_FRAME_RETRIES);

                // Set TX failure request restart configuration
                ws_bootstrap_configure_data_request_restart(cur, WS_CCA_REQUEST_RESTART_MAX, WS_TX_REQUEST_RESTART_MAX, WS_REQUEST_RESTART_BLACKLIST_MIN, WS_REQUEST_RESTART_BLACKLIST_MAX);

                // Set CSMA-CA backoff configuration
                ws_bootstrap_configure_csma_ca_backoffs(cur, WS_MAX_CSMA_BACKOFFS, WS_MAC_MIN_BE, WS_MAC_MAX_BE);

                ws_bootstrap_event_operation_start(cur);
                break;
            }
            ws_pae_controller_supp_init(cur);
            // Clear learned neighbours
            ws_bootstrap_neighbor_list_clean(cur);
            // Configure LLC for network discovery
            ws_bootstrap_network_discovery_configure(cur);
            ws_bootstrap_fhss_activate(cur);
            // Set retry configuration for discovery state
            ws_bootstrap_configure_max_retries(cur, WS_MAX_FRAME_RETRIES_BOOTSTRAP);
            // Set TX failure request restart configuration for discovery state
            ws_bootstrap_configure_data_request_restart(cur, WS_CCA_REQUEST_RESTART_MAX, WS_TX_REQUEST_RESTART_MAX_BOOTSTRAP, WS_REQUEST_RESTART_BLACKLIST_MIN, WS_REQUEST_RESTART_BLACKLIST_MAX);
            // Set CSMA-CA backoff configuration
            ws_bootstrap_configure_csma_ca_backoffs(cur, WS_MAX_CSMA_BACKOFFS, WS_MAC_MIN_BE, WS_MAC_MAX_BE);
            // Start network scan
            ws_bootstrap_start_discovery(cur);
            break;

        case WS_CONFIGURATION_START:
            tr_info("Configuration start");
            // Old configuration is considered invalid stopping all
            ws_bootstrap_asynch_trickle_stop(cur);

            // Build list of possible neighbours and learn first broadcast schedule

            ws_bootstrap_start_configuration_learn(cur);
            break;
        case WS_OPERATION_START:
            tr_info("operation start");
            // Advertisements stopped during the RPL scan
            ws_bootstrap_asynch_trickle_stop(cur);
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
                /* While in Join State 4, if a non Border Router determines it has been unable to communicate with the PAN Border
                 * Router for an interval of PAN_TIMEOUT, a node MUST assume failure of the PAN Border Router and MUST
                 * Transition to Join State 1
                 */
                ws_common_border_router_alive_update(cur);
            }
            break;
        case WS_ROUTING_READY:
            tr_info("Routing ready");
            // stopped all to make sure we can enter here from any state
            ws_bootstrap_asynch_trickle_stop(cur);

            // Indicate PAE controller that bootstrap is ready
            ws_pae_controller_bootstrap_done(cur);

            ws_bootstrap_advertise_start(cur);
            ws_bootstrap_state_change(cur, ER_BOOTSRAP_DONE);
            break;
        case WS_FAST_DISCONNECT:
            ws_bootstrap_disconnect(cur, WS_FAST_DISCONNECT);
            break;
        case WS_NORMAL_DISCONNECT:
            ws_bootstrap_disconnect(cur, WS_NORMAL_DISCONNECT);
            break;

        default:
            tr_err("Invalid event received");
            break;
    }
}

static int8_t ws_bootstrap_neighbor_set(protocol_interface_info_entry_t *cur, parent_info_t *parent_ptr, bool clear_list)
{
    uint16_t pan_id = cur->ws_info->network_pan_id;

    // Add EAPOL neighbor
    cur->ws_info->network_pan_id = parent_ptr->pan_id;
    cur->ws_info->pan_information = parent_ptr->pan_information;
    cur->ws_info->pan_information.pan_version = 0; // This is learned from actual configuration

    // If PAN ID changes, clear learned neighbors and activate FHSS
    if (pan_id != cur->ws_info->network_pan_id) {
        if (clear_list) {
            ws_bootstrap_neighbor_list_clean(cur);
        }
        ws_bootstrap_fhss_activate(cur);
    }

    llc_neighbour_req_t neighbor_info;
    if (!ws_bootstrap_neighbor_info_request(cur, parent_ptr->addr, &neighbor_info, true)) {
        //Remove Neighbour and set Link setup back
        ns_list_remove(&cur->ws_info->parent_list_reserved, parent_ptr);
        ns_list_add_to_end(&cur->ws_info->parent_list_free, parent_ptr);
        return -1;
    }
    ws_bootstrap_neighbor_set_stable(cur, parent_ptr->addr);
    ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, &parent_ptr->ws_utt, parent_ptr->timestamp, parent_ptr->addr);
    ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, &parent_ptr->ws_us, &cur->ws_info->hopping_schdule);
    return 0;
}

/*
 * State machine
 *
 * */
void ws_bootstrap_network_scan_process(protocol_interface_info_entry_t *cur)
{

    parent_info_t *selected_parent_ptr;

    tr_debug("analyze network discovery result");

select_best_candidate:
    selected_parent_ptr = ws_bootstrap_candidate_parent_get_best(cur);

    if (!selected_parent_ptr) {
        // Configure LLC for network discovery
        ws_bootstrap_network_discovery_configure(cur);
        // randomize new channel and start MAC
        ws_bootstrap_fhss_activate(cur);
        // Next check will be after one trickle
        uint32_t random_start = cur->ws_info->trickle_params_pan_discovery.Imin + randLIB_get_random_in_range(0, cur->ws_info->trickle_params_pan_discovery.Imin);
        if (random_start > 0xffff) {
            random_start = 0xffff;
        }
        cur->bootsrap_state_machine_cnt = random_start;

        tr_info("Making parent selection in %u s", (cur->bootsrap_state_machine_cnt / 10));
        return;
    }
    tr_info("selected parent:%s panid %u", trace_array(selected_parent_ptr->addr, 8), selected_parent_ptr->pan_id);

    if (ws_bootstrap_neighbor_set(cur, selected_parent_ptr, false) < 0) {
        goto select_best_candidate;
    }

    ws_pae_controller_set_target(cur, selected_parent_ptr->pan_id, selected_parent_ptr->addr); // temporary!!! store since auth
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
        cur->ws_info->connected_time = cur->ws_info->uptime;
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

static bool ws_bootstrap_state_discovery(struct protocol_interface_info_entry *cur)
{
    if (cur->nwk_bootstrap_state == ER_ACTIVE_SCAN) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_authenticate(struct protocol_interface_info_entry *cur)
{
    // Think about the state value
    if (cur->nwk_bootstrap_state == ER_PANA_AUTH) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_configure(struct protocol_interface_info_entry *cur)
{
    // Think about the state value
    if (cur->nwk_bootstrap_state == ER_SCAN) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_wait_rpl(struct protocol_interface_info_entry *cur)
{
    // Think about the state value
    if (cur->nwk_bootstrap_state == ER_RPL_SCAN) {
        return true;
    }
    return false;
}

static bool ws_bootstrap_state_active(struct protocol_interface_info_entry *cur)
{
    if (cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
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
            ws_bootstrap_asynch_trickle_stop(cur);
            ws_fhss_configure(cur, false);
            int8_t new_default = cur->ws_info->weakest_received_rssi - 1;
            if ((new_default < CCA_DEFAULT_DBM) && (new_default >= CCA_LOW_LIMIT) && (new_default <= CCA_HIGH_LIMIT)) {
                // Restart automatic CCA threshold using weakest received RSSI as new default
                mac_helper_start_auto_cca_threshold(cur->id, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->weakest_received_rssi - 1, CCA_HIGH_LIMIT, CCA_LOW_LIMIT);
            }
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
        case ER_RPL_NETWORK_LEAVING:
            tr_debug("WS SM:RPL Leaving ready trigger discovery");
            ws_bootstrap_event_discovery_start(cur);
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
    if (cur->ws_info->trickle_pcs_running) {

        //Update MAX config sol timeout timer
        if (cur->ws_info->pan_config_sol_max_timeout > ticks) {
            cur->ws_info->pan_config_sol_max_timeout -= ticks;
        } else {
            //Config sol state timeout
            cur->ws_info->pan_config_sol_max_timeout = 0;
        }

        if (trickle_timer(&cur->ws_info->trickle_pan_config_solicit, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
            if (cur->ws_info->pas_requests < PCS_MAX) {
                // send PAN Configuration solicit
                tr_info("Send PAN configuration Solicit");
                ws_bootstrap_pan_config_solicit(cur);
            }
            //Update counter every time reason that we detect PCS_MAX higher state
            cur->ws_info->pas_requests++;
        }

        if (cur->ws_info->pas_requests > PCS_MAX || cur->ws_info->pan_config_sol_max_timeout == 0) {
            // if MAX PCS sent or max waited timeout restart discovery
            // Trickle is reseted when entering to discovery from state 3
            tr_info("PAN configuration Solicit timeout");
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement_solicit, &cur->ws_info->trickle_params_pan_discovery);
            ws_bootstrap_event_discovery_start(cur);
            return;
        }
    }
    if (cur->ws_info->trickle_pa_running &&
            trickle_timer(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
        // send PAN advertisement
        tr_info("Send PAN advertisement");
        ws_bootstrap_pan_advert(cur);
    }
    if (cur->ws_info->trickle_pc_running) {

        if (cur->ws_info->trickle_pc_consistency_block_period) {
            if (ticks >= cur->ws_info->trickle_pc_consistency_block_period) {
                cur->ws_info->trickle_pc_consistency_block_period = 0;
            } else {
                cur->ws_info->trickle_pc_consistency_block_period -= ticks;
            }
        }

        if (trickle_timer(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery, ticks)) {
            // send PAN Configuration
            tr_info("Send PAN configuration");
            ws_bootstrap_pan_config(cur);
        }
    }
}


void ws_bootstrap_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    /* Border router keep alive check
     */
    if (cur->ws_info->pan_timeout_timer) {
        // PAN version timer running
        if (cur->ws_info->pan_timeout_timer > seconds) {
            cur->ws_info->pan_timeout_timer -= seconds;
            if (cur->ws_info->pan_timeout_timer < cur->ws_info->cfg->timing.pan_timeout / 10) {
                /* pan timeout is closing need to verify that DAO is tested before the pan times out.
                   This will give some extra time for RPL to find better parents.
                   Border router liveliness can be checked from version number change or from successful DAO registrations
                   in this case there has not been any version number changes during this PAN lifetime.
                */
                rpl_control_dao_timeout(cur->rpl_domain, 20);
            }
        } else {
            // Border router has timed out
            //Clear Timeout timer
            cur->ws_info->pan_timeout_timer = 0;
            tr_warn("Border router has timed out");
            ws_bootstrap_event_disconnect(cur, WS_FAST_DISCONNECT);
        }
    }
    if (cur->ws_info->aro_registration_timer) {
        if (cur->ws_info->aro_registration_timer > seconds) {
            cur->ws_info->aro_registration_timer -= seconds;
        } else {
            // Update all addressess. This function will update the timer value if needed
            cur->ws_info->aro_registration_timer = 0;
            ws_address_registration_update(cur, NULL);
        }
    }

    if (cur->ws_info->ws_bsi_block.block_time) {
        if (cur->ws_info->ws_bsi_block.block_time > seconds) {
            cur->ws_info->ws_bsi_block.block_time -= seconds;
        } else {
            //Clear A BSI blokker
            cur->ws_info->ws_bsi_block.block_time = 0;
            cur->ws_info->ws_bsi_block.old_bsi = 0;
        }
    }
    /*Update join state statistics*/
    if (ws_bootstrap_state_discovery(cur)) {
        ws_stats_update(cur, STATS_WS_STATE_1, 1);
    } else if (ws_bootstrap_state_authenticate(cur)) {
        ws_stats_update(cur, STATS_WS_STATE_2, 1);
    } else if (ws_bootstrap_state_configure(cur)) {
        ws_stats_update(cur, STATS_WS_STATE_3, 1);
    } else if (ws_bootstrap_state_wait_rpl(cur)) {
        ws_stats_update(cur, STATS_WS_STATE_4, 1);
    } else if (ws_bootstrap_state_active(cur)) {
        ws_stats_update(cur, STATS_WS_STATE_5, 1);
    }
    cur->ws_info->uptime++;

    ws_llc_timer_seconds(cur, seconds);

}

void ws_bootstrap_primary_parent_update(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neighbor)
{
    if (interface->ws_info) {
        llc_neighbour_req_t neighbor_info;
        neighbor_info.neighbor = neighbor;
        neighbor_info.ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor->index);
        ws_bootstrap_primary_parent_set(interface, &neighbor_info, WS_PARENT_HARD_SYNCH);
        uint8_t link_local_address[16];
        ws_bootsrap_create_ll_address(link_local_address, neighbor->mac64);
        dhcp_client_server_address_update(interface->id, NULL, link_local_address);

        ws_bootstrap_secondary_parent_update(interface);
    }
}

void ws_bootstrap_secondary_parent_update(protocol_interface_info_entry_t *interface)
{
    if (interface->ws_info) {
        ns_list_foreach(if_address_entry_t, address, &interface->ip_addresses) {
            if (!addr_is_ipv6_link_local(address->address)) {
                ws_address_parent_update(interface);
            }
        }
    }
}

int ws_bootstrap_stack_info_get(protocol_interface_info_entry_t *cur, struct ws_stack_info *info_ptr)
{

    ws_neighbor_class_entry_t *ws_neighbour = NULL;

    memset(info_ptr, 0, sizeof(struct ws_stack_info));
    mac_neighbor_table_entry_t *mac_parent = mac_neighbor_entry_get_priority(mac_neighbor_info(cur));

    if (mac_parent) {
        ws_neighbour = ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, mac_parent->index);
        ws_bootsrap_create_ll_address(info_ptr->parent, mac_parent->mac64);
    }
    if (ws_neighbour) {
        info_ptr->rsl_in = ws_neighbor_class_rsl_in_get(ws_neighbour);
        info_ptr->rsl_out = ws_neighbor_class_rsl_out_get(ws_neighbour);
        info_ptr->routing_cost = ws_neighbour->routing_cost;
    }
    info_ptr->device_min_sens = DEVICE_MIN_SENS;
    if (ws_bootstrap_state_discovery(cur)) {
        info_ptr->join_state = 1;
    } else if (ws_bootstrap_state_authenticate(cur)) {
        info_ptr->join_state = 2;
    } else if (ws_bootstrap_state_configure(cur)) {
        info_ptr->join_state = 3;
    } else if (ws_bootstrap_state_wait_rpl(cur)) {
        info_ptr->join_state = 4;
    } else if (ws_bootstrap_state_active(cur)) {
        info_ptr->join_state = 5;
    }
    info_ptr->pan_id = cur->ws_info->network_pan_id;

    return 0;
}
int ws_bootstrap_neighbor_info_get(protocol_interface_info_entry_t *cur, ws_neighbour_info_t *neighbor_ptr, uint16_t table_max)
{
    uint8_t count = 0;
    if (!neighbor_ptr) {
        // Return the aount of neighbors.
        for (int n = 0; n < mac_neighbor_info(cur)->list_total_size; n++) {
            mac_neighbor_table_entry_t *mac_entry = mac_neighbor_table_attribute_discover(mac_neighbor_info(cur), n);
            if (mac_entry && mac_entry->lifetime && mac_entry->lifetime != 0xffffffff) {
                count++;
            }
        }
        return count;
    }

    if (table_max > mac_neighbor_info(cur)->list_total_size) {
        table_max = mac_neighbor_info(cur)->list_total_size;
    }

    for (int n = 0; n < mac_neighbor_info(cur)->list_total_size; n++) {
        if (count > table_max) {
            break;
        }

        mac_neighbor_table_entry_t *mac_entry = mac_neighbor_table_attribute_discover(mac_neighbor_info(cur), n);
        ws_neighbor_class_entry_t *ws_neighbor =  ws_neighbor_class_entry_get(&cur->ws_info->neighbor_storage, n);
        if (mac_entry && ws_neighbor && mac_entry->lifetime && mac_entry->lifetime != 0xffffffff) {
            // Active neighbor entry
            uint8_t ll_address[16];
            memset(neighbor_ptr + count, 0, sizeof(ws_neighbour_info_t));
            neighbor_ptr[count].lifetime = mac_entry->lifetime;

            neighbor_ptr[count].rsl_in = ws_neighbor_class_rsl_in_get(ws_neighbor);
            neighbor_ptr[count].rsl_out = ws_neighbor_class_rsl_out_get(ws_neighbor);

            // ETX is shown calculated as 8 bit integer, but more common way is to use 7 bit such that 128 means ETX:1.0
            neighbor_ptr[count].etx = ws_local_etx_read(cur, ADDR_802_15_4_LONG, mac_entry->mac64);
            if (neighbor_ptr[count].etx != 0xffff) {
                neighbor_ptr[count].etx = neighbor_ptr[count].etx >> 1;
            }
            ws_bootsrap_create_ll_address(ll_address, mac_entry->mac64);
            memcpy(neighbor_ptr[count].link_local_address, ll_address, 16);

            if (rpl_control_is_dodag_parent_candidate(cur, ll_address, cur->ws_info->cfg->gen.rpl_parent_candidate_max)) {
                neighbor_ptr[count].type = WS_CANDIDATE_PARENT;
            }
            neighbor_ptr[count].rpl_rank = rpl_control_neighbor_info_get(cur, ll_address, neighbor_ptr[count].global_address);

            if (mac_entry->link_role == PRIORITY_PARENT_NEIGHBOUR) {
                neighbor_ptr[count].type = WS_PRIMARY_PARENT;
            }
            if (mac_entry->link_role == SECONDARY_PARENT_NEIGHBOUR) {
                neighbor_ptr[count].type = WS_SECONDARY_PARENT;
            }
            if (mac_entry->link_role == CHILD_NEIGHBOUR) {
                neighbor_ptr[count].type = WS_CHILD;
            }

            ipv6_neighbour_t *IPv6_neighbor = ipv6_neighbour_get_registered_by_eui64(&cur->ipv6_neighbour_cache, mac_entry->mac64);
            if (IPv6_neighbor) {
                //This is a child
                neighbor_ptr[count].type = WS_CHILD;
                memcpy(neighbor_ptr[count].global_address, IPv6_neighbor->ip_address, 16);
                // Child lifetimes are based on Registration times not a link time
                neighbor_ptr[count].lifetime = IPv6_neighbor->lifetime;
            }
            count++;
        }
    }

    // Go through list
    return count;
}

//Calculate max_packet queue size
static uint16_t ws_bootstrap_define_congestin_max_threshold(uint32_t heap_total_size, uint16_t packet_size, uint16_t packet_per_seconds, uint32_t max_delay, uint16_t min_packet_queue_size, uint16_t max_packet_queue_size)
{
    uint32_t max_packet_count = 0;
    if (heap_total_size) {
        //Claculate how many packet can be max queue to half of heap
        max_packet_count = (heap_total_size / 2) / packet_size;
    }

    //Calculate how many packet is possible to queue for guarantee given max delay
    uint32_t max_delayded_queue_size = max_delay * packet_per_seconds;

    if (max_packet_count > max_delayded_queue_size) {
        //Limit queue size by MAX delay
        max_packet_count = max_delayded_queue_size;
    }

    if (max_packet_count > max_packet_queue_size) {
        //Limit queue size by Max
        max_packet_count = max_packet_queue_size;
    } else if (max_packet_count < min_packet_queue_size) {
        //Limit queue size by Min
        max_packet_count = min_packet_queue_size;
    }
    return (uint16_t)max_packet_count;
}

static uint16_t ws_bootstrap_packet_per_seconds(protocol_interface_info_entry_t *cur, uint16_t packet_size)
{
    uint32_t data_rate = ws_common_datarate_get(cur);

    //calculate how many packet is possible send in paper
    data_rate /= 8 * packet_size;

    //Divide optimal  by / 5 because we split TX / RX slots and BC schedule
    //With Packet size 500 it should return
    //Return 15 for 300kBits
    //Return 7 for 150kBits
    //Return 2 for 50kBits
    return data_rate / 5;
}



static void ws_bootstrap_packet_congestion_init(protocol_interface_info_entry_t *cur)
{
    random_early_detection_free(cur->random_early_detection);
    cur->random_early_detection = NULL;

    //TODO implement API for HEAP info request
    uint32_t heap_size;
    const mem_stat_t *mem_stats = ns_dyn_mem_get_mem_stat();
    if (mem_stats) {
        heap_size = mem_stats->heap_sector_size;
    } else {
        heap_size = 0;
    }

    uint16_t packet_per_seconds = ws_bootstrap_packet_per_seconds(cur, WS_CONGESTION_PACKET_SIZE);

    uint16_t min_th, max_th;

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        max_th = ws_bootstrap_define_congestin_max_threshold(heap_size, WS_CONGESTION_PACKET_SIZE, packet_per_seconds, WS_CONGESTION_QUEUE_DELAY, WS_CONGESTION_BR_MIN_QUEUE_SIZE, WS_CONGESTION_BR_MAX_QUEUE_SIZE);
    } else {
        max_th = ws_bootstrap_define_congestin_max_threshold(heap_size, WS_CONGESTION_PACKET_SIZE, packet_per_seconds, WS_CONGESTION_QUEUE_DELAY, WS_CONGESTION_NODE_MIN_QUEUE_SIZE, WS_CONGESTION_NODE_MAX_QUEUE_SIZE);
    }

    min_th = max_th / 2;
    tr_info("Wi-SUN packet congestion minTh %u, maxTh %u, drop probability %u weight %u, Packet/Seconds %u", min_th, max_th, WS_CONGESTION_RED_DROP_PROBABILITY, RED_AVERAGE_WEIGHT_EIGHTH, packet_per_seconds);
    cur->random_early_detection = random_early_detection_create(min_th, max_th, WS_CONGESTION_RED_DROP_PROBABILITY, RED_AVERAGE_WEIGHT_EIGHTH);

}

#endif //HAVE_WS
