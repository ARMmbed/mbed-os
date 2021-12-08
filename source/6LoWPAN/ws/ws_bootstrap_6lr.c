/*
 * Copyright (c) 2021, Pelion and affiliates.
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
#if defined(HAVE_WS) && defined(HAVE_WS_ROUTER)
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
#include "6LoWPAN/ws/ws_bootstrap_6lr.h"
#include "6LoWPAN/ws/ws_bootstrap_ffn.h"
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

static void ws_bootstrap_6lr_address_update(protocol_interface_info_entry_t *interface);
static void ws_bootstrap_6lr_parent_confirm(protocol_interface_info_entry_t *cur, struct rpl_instance *instance);
static struct rpl_instance *ws_bootstrap_6lr_get_rpl_instance(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_6lr_rank_get(protocol_interface_info_entry_t *cur);
static uint16_t ws_bootstrap_6lr_min_rank_inc_get(protocol_interface_info_entry_t *cur);


static void ws_bootstrap_6lr_set_fhss_hop(protocol_interface_info_entry_t *cur)
{
    uint16_t own_rank = ws_bootstrap_6lr_rank_get(cur);
    uint16_t rank_inc = ws_bootstrap_6lr_min_rank_inc_get(cur);
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

static void ws_bootstrap_6lr_ip_stack_addr_clear(protocol_interface_info_entry_t *cur)
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

static void ws_bootstrap_6lr_pan_information_store(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us, ws_pan_information_t *pan_information)
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

static int8_t ws_bootstrap_6lr_fhss_configure(protocol_interface_info_entry_t *cur, bool discovery)
{
    // Read configuration of existing FHSS and start using the default values for any network
    fhss_ws_configuration_t fhss_configuration = ws_common_get_current_fhss_configuration(cur);
    ws_bootstrap_fhss_set_defaults(cur, &fhss_configuration);
    ws_bootstrap_fhss_configure_channel_masks(cur, &fhss_configuration);

    // Discovery is done using fixed channel
    if (discovery) {
        fhss_configuration.ws_uc_channel_function = WS_FIXED_CHANNEL;
    } else {
        fhss_configuration.ws_uc_channel_function = (fhss_ws_channel_functions)cur->ws_info->cfg->fhss.fhss_uc_channel_function;
    }
    fhss_configuration.ws_bc_channel_function = WS_FIXED_CHANNEL;
    fhss_configuration.fhss_broadcast_interval = 0;
    uint8_t tmp_uc_fixed_channel = ws_bootstrap_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    uint8_t tmp_bc_fixed_channel = ws_bootstrap_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    fhss_configuration.unicast_fixed_channel = tmp_uc_fixed_channel;
    fhss_configuration.broadcast_fixed_channel = tmp_bc_fixed_channel;
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ns_fhss_ws_set_hop_count(cur->ws_info->fhss_api, 0xff);
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);

    return 0;
}

void ws_bootstrap_6lr_network_discovery_configure(protocol_interface_info_entry_t *cur)
{
    // Reset information to defaults
    cur->ws_info->network_pan_id = 0xffff;

    ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule);
    ws_bootstrap_set_domain_rf_config(cur);
    ws_bootstrap_6lr_fhss_configure(cur, true);

    //Set Network names, Pan information configure, hopping schedule & GTKHash
    ws_llc_set_network_name(cur, (uint8_t *)cur->ws_info->cfg->gen.network_name, strlen(cur->ws_info->cfg->gen.network_name));
}

// Start network scan
static void ws_bootstrap_6lr_start_discovery(protocol_interface_info_entry_t *cur)
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
    ws_bootstrap_6lr_ip_stack_addr_clear(cur);

    if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) != INTERFACE_NWK_BOOTSRAP_ACTIVE) {
        // we have sent bootstrap ready event and now
        // restarted discovery so bootstrap down event is sent
        cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;
        ws_bootstrap_network_down(cur);
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

// Start configuration learning
static void ws_bootstrap_6lr_start_configuration_learn(protocol_interface_info_entry_t *cur)
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

static void ws_bootstrap_6lr_network_configuration_learn(protocol_interface_info_entry_t *cur)
{
    tr_debug("Start using PAN configuration");

    // Timing information can be modified here
    ws_llc_set_pan_information_pointer(cur, &cur->ws_info->pan_information);
    uint8_t *gtkhash = ws_pae_controller_gtk_hash_ptr_get(cur);
    ws_llc_set_gtkhash(cur, gtkhash);
    // TODO update own fhss schedules we are starting to follow first parent

    return;
}

static void ws_bootstrap_6lr_pan_advertisement_analyse_active(struct protocol_interface_info_entry *cur, ws_pan_information_t *pan_information)
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

static void ws_bootstrap_6lr_pan_advertisement_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
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
    ws_bootstrap_6lr_pan_information_store(cur, data, ws_utt, ws_us, &pan_information);

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
        //Store and search neighbour PCAP info
        if (ws_version_1_1(cur)) {
            ws_neighbor_class_pcap_ie_store(neighbor_info.ws_neighbor, ie_ext);
        }
    }

    ws_bootstrap_6lr_pan_advertisement_analyse_active(cur, &pan_information);

    // Learn latest network information
    if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER && neighbor_info.neighbor) {
        uint8_t ll_address[16];
        ws_common_create_ll_address(ll_address, neighbor_info.neighbor->mac64);

        if (rpl_control_is_dodag_parent(cur, ll_address)) {
            cur->ws_info->pan_information.pan_size = pan_information.pan_size;
            cur->ws_info->pan_information.routing_cost = pan_information.routing_cost;
            cur->ws_info->pan_information.rpl_routing_method = pan_information.rpl_routing_method;
            cur->ws_info->pan_information.use_parent_bs = pan_information.use_parent_bs;
        }
    }
}

static void ws_bootstrap_6lr_pan_advertisement_solicit_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
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
            ws_bootstrap_6lr_parent_confirm(cur, NULL);
        }
    }
}
#ifdef HAVE_WS_VERSION_1_1
static void ws_bootstrap_6lr_pan_config_lfn_analyze(struct protocol_interface_info_entry *cur, const struct mcps_data_ie_list *ie_ext)
{
    if (!ws_version_1_1(cur) || cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return;
    }

    ws_lfnver_ie_t lfn_version;
    if (!ws_wp_nested_lfn_version_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &lfn_version)) {
        return; // LFN version
    }

    //Read LFNGTKHASH
    ws_lgtkhash_ie_t ws_lgtkhash;
    if (!ws_wp_nested_lgtk_hash_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_lgtkhash)) {
        return;
    }

    if (!cur->ws_info->lfngtk.lfn_version_learned) {
        if (!cur->ws_info->configuration_learned) {
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
        }
    } else {
        if (cur->ws_info->lfngtk.lfn_version == lfn_version.lfn_version) {
            return;
        }

        if (common_serial_number_greater_16(cur->ws_info->lfngtk.lfn_version, lfn_version.lfn_version)) {
            // older version heard ignoring the message
            return;
        }
    }

    cur->ws_info->lfngtk.lfn_version = lfn_version.lfn_version;

    //Clear HASH allways at new first or for first leaned one's
    memset(cur->ws_info->lfngtk.lgtkhash, 0, 24);
    cur->ws_info->lfngtk.lfn_version_learned = true;

    //Set Active key index and hash inline bits
    cur->ws_info->lfngtk.active_key_index = ws_lgtkhash.active_lgtk_index;
    cur->ws_info->lfngtk.active_hash_1 = ws_lgtkhash.lgtk0;
    cur->ws_info->lfngtk.active_hash_2 = ws_lgtkhash.lgtk1;
    cur->ws_info->lfngtk.active_hash_3 = ws_lgtkhash.lgtk2;

    if (cur->ws_info->lfngtk.active_hash_1) {
        memcpy(cur->ws_info->lfngtk.lgtkhash, ws_lgtkhash.lgtk0_hash, 8);
    }

    if (cur->ws_info->lfngtk.active_hash_2) {
        memcpy(cur->ws_info->lfngtk.lgtkhash + 8, ws_lgtkhash.lgtk1_hash, 8);
    }

    if (cur->ws_info->lfngtk.active_hash_3) {
        memcpy(cur->ws_info->lfngtk.lgtkhash + 16, ws_lgtkhash.lgtk2_hash, 8);
    }
    //TODO Analyze HASH's and set LFN group key index
}
#endif


static void ws_bootstrap_6lr_pan_config_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
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
#ifdef HAVE_WS_VERSION_1_1
            ws_bootstrap_6lr_pan_config_lfn_analyze(cur, ie_ext);
#endif
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

#ifdef HAVE_WS_VERSION_1_1
    ws_bootstrap_6lr_pan_config_lfn_analyze(cur, ie_ext);
#endif

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

static void ws_bootstrap_6lr_pan_config_solicit_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
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
            ws_bootstrap_6lr_parent_confirm(cur, NULL);
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


void ws_bootstrap_6lr_asynch_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type)
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
            ws_bootstrap_6lr_pan_advertisement_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_ADVERT_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PAS, 1);
            tr_info("received ADVERT SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_6lr_pan_advertisement_solicit_analyse(cur, data, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PC, 1);
            tr_info("received CONFIG Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_6lr_pan_config_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PCS, 1);
            tr_info("received CONFIG SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_6lr_pan_config_solicit_analyse(cur, data, &ws_utt, &ws_us);
        default:
            // Unknown message do not process
            break;
    }
}

void ws_bootstrap_6lr_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message)
{
    (void)asynch_message;
    ws_stats_update(interface, STATS_WS_ASYNCH_TX, 1);
}

bool ws_bootstrap_6lr_eapol_relay_state_active(protocol_interface_info_entry_t *cur)
{
    if (cur->nwk_bootstrap_state == ER_BOOTSRAP_DONE) {
        return true;
    }

    return false;
}

static struct rpl_instance *ws_bootstrap_6lr_get_rpl_instance(protocol_interface_info_entry_t *cur)
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

static uint16_t ws_bootstrap_6lr_rank_get(protocol_interface_info_entry_t *cur)
{
    struct rpl_instance *rpl_instance = ws_bootstrap_6lr_get_rpl_instance(cur);
    if (!rpl_instance) {
        return 0xffff;
    }
    return rpl_control_current_rank(rpl_instance);
}

static uint16_t ws_bootstrap_6lr_min_rank_inc_get(protocol_interface_info_entry_t *cur)
{
    struct rpl_instance *rpl_instance = ws_bootstrap_6lr_get_rpl_instance(cur);
    if (!rpl_instance) {
        return 0xffff;
    }
    struct rpl_dodag_info_t dodag_info;
    if (!rpl_control_read_dodag_info(rpl_instance, &dodag_info)) {
        return 0xffff;
    }
    return dodag_info.dag_min_hop_rank_inc;
}

void ws_bootstrap_6lr_address_registration_update(protocol_interface_info_entry_t *interface, const uint8_t addr[16])
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

static void ws_bootstrap_6lr_address_update(protocol_interface_info_entry_t *interface)
{
    tr_info("RPL parent update ... register ARO");
    ws_bootstrap_6lr_address_registration_update(interface, NULL);
}

static void ws_bootstrap_6lr_parent_confirm(protocol_interface_info_entry_t *cur, struct rpl_instance *instance)
{
    /* Possible problem with the parent connection
     * Give some time for parent to rejoin and confirm the connection with ARO and DAO
     */
    const rpl_dodag_conf_int_t *config = NULL;
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


void ws_bootstrap_6lr_primary_parent_update(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neighbor)
{
    if (interface->ws_info) {
        llc_neighbour_req_t neighbor_info;
        neighbor_info.neighbor = neighbor;
        neighbor_info.ws_neighbor = ws_neighbor_class_entry_get(&interface->ws_info->neighbor_storage, neighbor->index);
        ws_bootstrap_primary_parent_set(interface, &neighbor_info, WS_PARENT_HARD_SYNCH);
        uint8_t link_local_address[16];
        ws_common_create_ll_address(link_local_address, neighbor->mac64);
        dhcp_client_server_address_update(interface->id, NULL, link_local_address);

        ws_bootstrap_6lr_secondary_parent_update(interface);
    }
}

void ws_bootstrap_6lr_secondary_parent_update(protocol_interface_info_entry_t *interface)
{
    if (interface->ws_info) {
        ns_list_foreach(if_address_entry_t, address, &interface->ip_addresses) {
            if (!addr_is_ipv6_link_local(address->address)) {
                ws_bootstrap_6lr_address_update(interface);
            }
        }
    }
}

static void ws_bootstrap_6lr_rpl_scan_start(protocol_interface_info_entry_t *cur)
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


static void ws_bootstrap_6lr_rpl_callback(rpl_event_t event, void *handle)
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

        ws_bootstrap_6lr_set_fhss_hop(cur);
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
        ws_bootstrap_6lr_parent_confirm(cur, NULL);
        // Move to state 4 if we see issues with primary parent
        if (ws_bootstrap_state_active(cur)) {
            tr_info("Move state 4 to wait parent connection confirmation");
            ws_bootstrap_6lr_rpl_scan_start(cur);
            ws_bootstrap_network_down(cur);
        }
    } else if (event == RPL_EVENT_DAO_PARENT_ADD) {
        ws_bootstrap_6lr_address_update(cur);
    }
    cur->ws_info->rpl_state = event;
    tr_info("RPL event %d", event);
}

static void ws_bootstrap_6lr_rpl_prefix_callback(prefix_entry_t *prefix, void *handle, uint8_t *parent_link_local)
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

static bool ws_bootstrap_6lr_rpl_candidate_soft_filtering(protocol_interface_info_entry_t *cur, struct rpl_instance *instance)
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

static bool ws_bootstrap_6lr_rpl_new_parent_callback(uint8_t *ll_parent_address, void *handle, struct rpl_instance *instance, uint16_t candidate_rank)
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

    if (!ws_bootstrap_6lr_rpl_candidate_soft_filtering(cur, instance)) {

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

static void ws_bootstrap_6lr_rpl_parent_dis_callback(const uint8_t *ll_parent_address, void *handle, struct rpl_instance *instance)
{
    (void) ll_parent_address;
    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return;
    }
    //Multicast DIS from parent indicate that Parent is not valid in short time window possible
    ws_bootstrap_6lr_parent_confirm(cur, instance);
}


void ws_bootstrap_6lr_event_handler(protocol_interface_info_entry_t *cur, arm_event_s *event)
{
    ws_bootsrap_event_type_e event_type;
    event_type = (ws_bootsrap_event_type_e)event->event_type;

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

            ws_pae_controller_supp_init(cur);
            // Clear learned neighbours
            ws_bootstrap_neighbor_list_clean(cur);
            // Configure LLC for network discovery
            ws_bootstrap_6lr_network_discovery_configure(cur);
            ws_bootstrap_fhss_activate(cur);
            // Set retry configuration for discovery state
            ws_bootstrap_configure_max_retries(cur, WS_MAX_FRAME_RETRIES_BOOTSTRAP);
            // Set TX failure request restart configuration for discovery state
            ws_bootstrap_configure_data_request_restart(cur, WS_CCA_REQUEST_RESTART_MAX, WS_TX_REQUEST_RESTART_MAX_BOOTSTRAP, WS_REQUEST_RESTART_BLACKLIST_MIN, WS_REQUEST_RESTART_BLACKLIST_MAX);
            // Set CSMA-CA backoff configuration
            ws_bootstrap_configure_csma_ca_backoffs(cur, WS_MAX_CSMA_BACKOFFS, WS_MAC_MIN_BE, WS_MAC_MAX_BE);
            // Start network scan
            ws_bootstrap_6lr_start_discovery(cur);
            break;

        case WS_CONFIGURATION_START:
            tr_info("Configuration start");
            // Old configuration is considered invalid stopping all
            ws_bootstrap_asynch_trickle_stop(cur);

            // Build list of possible neighbours and learn first broadcast schedule

            ws_bootstrap_6lr_start_configuration_learn(cur);
            break;
        case WS_OPERATION_START:
            tr_info("operation start");
            // Advertisements stopped during the RPL scan
            ws_bootstrap_asynch_trickle_stop(cur);
            // Activate RPL
            // Activate IPv6 stack
            ws_bootstrap_ip_stack_activate(cur);
            ws_bootstrap_ffn_rpl_configure(cur);
            // Configure RPL for Node.
            rpl_control_set_callback(protocol_6lowpan_rpl_domain, ws_bootstrap_6lr_rpl_callback, ws_bootstrap_6lr_rpl_prefix_callback, ws_bootstrap_6lr_rpl_new_parent_callback, ws_bootstrap_6lr_rpl_parent_dis_callback, cur);
            rpl_control_set_memory_limits(WS_NODE_RPL_SOFT_MEM_LIMIT, WS_NODE_RPL_HARD_MEM_LIMIT);

            ws_bootstrap_network_start(cur);
            // Wait for RPL start
            ws_bootstrap_6lr_rpl_scan_start(cur);
            /* While in Join State 4, if a non Border Router determines it has been unable to communicate with the PAN Border
             * Router for an interval of PAN_TIMEOUT, a node MUST assume failure of the PAN Border Router and MUST
             * Transition to Join State 1
             */
            ws_common_border_router_alive_update(cur);
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
            ws_bootstrap_state_disconnect(cur, WS_FAST_DISCONNECT);
            break;
        case WS_NORMAL_DISCONNECT:
            ws_bootstrap_state_disconnect(cur, WS_NORMAL_DISCONNECT);
            break;

        case WS_TEST_PROC_TRIGGER:
            ws_bootstrap_test_procedure_trigger_exec(cur, (ws_bootsrap_procedure_t) event->data_ptr);
            break;

        default:
            tr_err("Invalid event received");
            break;
    }
}

/*
 * Statemachine state functions
 * */

static void ws_bootstrap_6lr_network_scan_process(protocol_interface_info_entry_t *cur)
{

    parent_info_t *selected_parent_ptr;

    tr_debug("analyze network discovery result");

select_best_candidate:
    selected_parent_ptr = ws_bootstrap_candidate_parent_get_best(cur);

    if (!selected_parent_ptr) {
        // Configure LLC for network discovery
        ws_bootstrap_6lr_network_discovery_configure(cur);
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

static void ws_bootstrap_6lr_configure_process(protocol_interface_info_entry_t *cur)
{

    if (cur->ws_info->configuration_learned) {
        ws_bootstrap_6lr_network_configuration_learn(cur);
        ws_bootstrap_event_operation_start(cur);
        return;
    }
    return;
}

void ws_bootstrap_6lr_rpl_wait_process(protocol_interface_info_entry_t *cur)
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


/*
 * State machine
 */

void ws_bootstrap_6lr_state_machine(protocol_interface_info_entry_t *cur)
{

    switch (cur->nwk_bootstrap_state) {
        case ER_WAIT_RESTART:
            tr_debug("WS SM:Wait for startup");
            ws_bootstrap_event_discovery_start(cur);
            break;
        case ER_ACTIVE_SCAN:
            tr_debug("WS SM:Active Scan");
            ws_bootstrap_6lr_network_scan_process(cur);
            break;
        case ER_SCAN:
            tr_debug("WS SM:configuration Scan");
            ws_bootstrap_6lr_configure_process(cur);
            break;
        case ER_PANA_AUTH:
            tr_info("authentication start");
            // Advertisements stopped during the EAPOL
            ws_bootstrap_asynch_trickle_stop(cur);
            ws_bootstrap_6lr_fhss_configure(cur, false);
            int8_t new_default = cur->ws_info->weakest_received_rssi - 1;
            if ((new_default < CCA_DEFAULT_DBM) && (new_default >= CCA_LOW_LIMIT) && (new_default <= CCA_HIGH_LIMIT)) {
                // Restart automatic CCA threshold using weakest received RSSI as new default
                mac_helper_start_auto_cca_threshold(cur->id, cur->ws_info->hopping_schdule.number_of_channels, cur->ws_info->weakest_received_rssi - 1, CCA_HIGH_LIMIT, CCA_LOW_LIMIT);
            }
            ws_bootstrap_start_authentication(cur);
            break;
        case ER_RPL_SCAN:
            tr_debug("WS SM:Wait RPL to contact DODAG root");
            ws_bootstrap_6lr_rpl_wait_process(cur);
            break;
        case ER_BOOTSRAP_DONE:
            tr_info("WS SM:Bootstrap Done");
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

void ws_bootstrap_6lr_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
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
            ws_bootstrap_6lr_address_registration_update(cur, NULL);
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
}


#endif //HAVE_WS_BORDER_ROUTER && HAVE_WS
