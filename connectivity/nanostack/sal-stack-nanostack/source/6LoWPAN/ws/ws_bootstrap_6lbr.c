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
#if defined(HAVE_WS) && defined(HAVE_WS_BORDER_ROUTER)
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
#include "6LoWPAN/ws/ws_bootstrap_ffn.h"
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

static int8_t ws_bootstrap_6lbr_fhss_configure(protocol_interface_info_entry_t *cur)
{
    // Read configuration of existing FHSS and start using the default values for any network
    fhss_ws_configuration_t fhss_configuration = ws_common_get_current_fhss_configuration(cur);
    //GET BSI from BBR module
    fhss_configuration.bsi = ws_bbr_bsi_generate(cur);
    ws_bootstrap_fhss_configure_channel_masks(cur, &fhss_configuration);
    // Randomize fixed channels. Only used if channel plan is fixed.
    cur->ws_info->cfg->fhss.fhss_uc_fixed_channel = ws_bootstrap_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_uc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    cur->ws_info->cfg->fhss.fhss_bc_fixed_channel = ws_bootstrap_randomize_fixed_channel(cur->ws_info->cfg->fhss.fhss_bc_fixed_channel, cur->ws_info->hopping_schdule.number_of_channels, fhss_configuration.channel_mask);
    ws_bootstrap_fhss_set_defaults(cur, &fhss_configuration);
    ns_fhss_ws_configuration_set(cur->ws_info->fhss_api, &fhss_configuration);
    ws_bootstrap_llc_hopping_update(cur, &fhss_configuration);

    return 0;
}

static int8_t ws_bootstrap_6lbr_backbone_ip_addr_get(protocol_interface_info_entry_t *interface_ptr, uint8_t *address)
{
    (void) interface_ptr;
    (void) address;

    if (ws_bbr_backbone_address_get(address)) {
        return 0;
    }

    return -1;
}

static void ws_bootstrap_6lbr_eapol_congestion_init(protocol_interface_info_entry_t *cur)
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

static void ws_bootstrap_6lbr_pan_config_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{
    ws_bs_ie_t ws_bs_ie;
    ws_bt_ie_t ws_bt_ie;
    llc_neighbour_req_t neighbor_info;

    if (data->SrcPANId != cur->ws_info->network_pan_id) {
        return;
    }
    if (!ws_wh_bt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_bt_ie)) {
        tr_warn("BT-IE");
        return;
    }

    if (!ws_wp_nested_bs_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_bs_ie)) {
        // Corrupted
        tr_error("No broadcast schedule");
        return;
    }

    //If we are border router or learned configuration we only update already learned neighbours.

    if (ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false)) {
        //Update Neighbor Broadcast and Unicast Parameters
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp, (uint8_t *) data->SrcAddr);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us, &cur->ws_info->hopping_schdule);
        ws_neighbor_class_neighbor_broadcast_time_info_update(neighbor_info.ws_neighbor, &ws_bt_ie, data->timestamp);
        ws_neighbor_class_neighbor_broadcast_schedule_set(neighbor_info.ws_neighbor, &ws_bs_ie);
    }
}

static void ws_bootstrap_6lbr_pan_config_solicit_analyse(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us)
{
    llc_neighbour_req_t neighbor_info;

    if (data->SrcPANId != cur->ws_info->network_pan_id) {
        return;
    }

    if (ws_bootstrap_neighbor_info_request(cur, data->SrcAddr, &neighbor_info, false)) {
        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, ws_utt, data->timestamp, (uint8_t *) data->SrcAddr);
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, ws_us, &cur->ws_info->hopping_schdule);
    }
}

void ws_bootstrap_6lbr_asynch_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type)
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
            // Border routers do not do any analysing on the Advertisements heard from others
            // in future if we need PANID conflict detection we could use this
            break;
        case WS_FT_PAN_ADVERT_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PAS, 1);
            tr_info("received ADVERT SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_advertisement, &cur->ws_info->trickle_params_pan_discovery);

            break;
        case WS_FT_PAN_CONF:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PC, 1);
            tr_info("received CONFIG Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            ws_bootstrap_6lbr_pan_config_analyse(cur, data, ie_ext, &ws_utt, &ws_us);
            break;
        case WS_FT_PAN_CONF_SOL:
            ws_stats_update(cur, STATS_WS_ASYNCH_RX_PCS, 1);
            tr_info("received CONFIG SOL Src:%s rssi:%d", trace_array(data->SrcAddr, 8), data->signal_dbm);
            trickle_inconsistent_heard(&cur->ws_info->trickle_pan_config, &cur->ws_info->trickle_params_pan_discovery);
            ws_bootstrap_6lbr_pan_config_solicit_analyse(cur, data, &ws_utt, &ws_us);
        default:
            // Unknown message do not process
            break;
    }
}

void ws_bootstrap_6lbr_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message)
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

bool ws_bootstrap_6lbr_eapol_relay_state_active(protocol_interface_info_entry_t *cur)
{
    (void) cur;

    return true;
}

static void ws_bootstrap_6lbr_pan_version_increment(protocol_interface_info_entry_t *cur)
{
    (void)cur;
    ws_bbr_pan_version_increase(cur);
}

static void ws_bootstrap_6lbr_nw_info_updated(protocol_interface_info_entry_t *cur, uint16_t pan_id, uint16_t pan_version, char *network_name)
{
    /* For border router, the PAE controller reads PAN ID, PAN version and network name from storage.
     * If they are set, takes them into use here.
     */
    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        // Get network name
        ws_gen_cfg_t gen_cfg;
        if (ws_cfg_gen_get(&gen_cfg) < 0) {
            return;
        }

        // If PAN ID has not been set, set it
        if (cur->ws_info->network_pan_id == 0xffff) {
            cur->ws_info->network_pan_id = pan_id;
            // Sets PAN version
            cur->ws_info->pan_information.pan_version = pan_version;
            cur->ws_info->pan_information.pan_version_set = true;
        }

        // If network name has not been set, set it
        if (strlen(gen_cfg.network_name) == 0) {
            strncpy(gen_cfg.network_name, network_name, 32);
        }

        // Stores the settings
        ws_cfg_gen_set(cur, &gen_cfg, 0);
    }
}

static bool ws_bootstrap_6lbr_eapol_congestion_get(protocol_interface_info_entry_t *cur, uint16_t active_supp)
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

void ws_bootstrap_6lbr_event_handler(protocol_interface_info_entry_t *cur, arm_event_s *event)
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

            tr_info("Border router start network");


            if (!ws_bbr_ready_to_start(cur)) {
                // Wi-SUN not started yet we wait for Border router permission
                ws_bootstrap_state_change(cur, ER_WAIT_RESTART);
                cur->nwk_nd_re_scan_count = randLIB_get_random_in_range(40, 100);
                return;
            }
            // Clear Old information from stack
            cur->ws_info->network_pan_id = 0xffff;
            cur->ws_info->pan_information.pan_version_set = false;
            ws_nud_table_reset(cur);
            ws_bootstrap_neighbor_list_clean(cur);
            ws_bootstrap_ip_stack_reset(cur);
            ws_pae_controller_auth_init(cur);

            uint16_t pan_id = ws_bbr_pan_id_get(cur);
            if (pan_id != 0xffff) {
                cur->ws_info->network_pan_id = pan_id;
            } else {
                if (cur->ws_info->network_pan_id == 0xffff) {
                    cur->ws_info->network_pan_id = randLIB_get_random_in_range(0, 0xfffd);
                }
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
            // initialize for FAN 1.1 defaults
            if (ws_version_1_1(cur)) {
                cur->ws_info->pan_information.version = WS_FAN_VERSION_1_1;
#ifdef HAVE_WS_VERSION_1_1
                if (!cur->ws_info->lfngtk.lfn_version_learned) {
                    //Randomize LFN version
                    cur->ws_info->lfngtk.lfn_version = randLIB_get_random_in_range(0, 0xffff);
                    cur->ws_info->lfngtk.lfn_version_learned = true;
                }
#endif
            }

            uint8_t *gtkhash = ws_pae_controller_gtk_hash_ptr_get(cur);
            ws_llc_set_gtkhash(cur, gtkhash);
            ws_bbr_pan_version_increase(cur);

            // Set default parameters for FHSS when starting a discovery
            ws_common_regulatory_domain_config(cur, &cur->ws_info->hopping_schdule);
            ws_bootstrap_6lbr_fhss_configure(cur);
            ws_bootstrap_set_domain_rf_config(cur);
            ws_bootstrap_fhss_activate(cur);
            ns_fhss_ws_set_hop_count(cur->ws_info->fhss_api, 0);

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

            // Set information callbacks (backbone IP address get callback, network information, congestion)
            ws_pae_controller_information_cb_register(cur, ws_bootstrap_6lbr_backbone_ip_addr_get, &ws_bootstrap_6lbr_nw_info_updated, &ws_bootstrap_6lbr_eapol_congestion_get);

            // Set PAN version control callbacks
            ws_pae_controller_pan_version_cb_register(cur, &ws_bootstrap_6lbr_pan_version_increment);

            // Set PAE port to 10254 and authenticator relay to 10253 (and to own ll address)
            ws_pae_controller_authenticator_start(cur, PAE_AUTH_SOCKET_PORT, ll_addr, EAPOL_RELAY_SOCKET_PORT);

            // Initialize eapol congestion tracking
            ws_bootstrap_6lbr_eapol_congestion_init(cur);

            // Set retry configuration for bootstrap ready state
            ws_bootstrap_configure_max_retries(cur, WS_MAX_FRAME_RETRIES);

            // Set TX failure request restart configuration
            ws_bootstrap_configure_data_request_restart(cur, WS_CCA_REQUEST_RESTART_MAX, WS_TX_REQUEST_RESTART_MAX, WS_REQUEST_RESTART_BLACKLIST_MIN, WS_REQUEST_RESTART_BLACKLIST_MAX);

            // Set CSMA-CA backoff configuration
            ws_bootstrap_configure_csma_ca_backoffs(cur, WS_MAX_CSMA_BACKOFFS, WS_MAC_MIN_BE, WS_MAC_MAX_BE);

            ws_bootstrap_event_operation_start(cur);
            break;

        case WS_CONFIGURATION_START:
            tr_info("6LBR Configuration start");
            break;
        case WS_OPERATION_START:
            tr_info("operation start");
            // Advertisements stopped during the RPL scan
            ws_bootstrap_asynch_trickle_stop(cur);
            // Activate RPL
            // Activate IPv6 stack
            ws_bootstrap_ip_stack_activate(cur);
            ws_bootstrap_ffn_rpl_configure(cur);
            ws_bootstrap_network_start(cur);
            // Wait for RPL start
            ws_bootstrap_event_routing_ready(cur);
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

void ws_bootstrap_6lbr_state_machine(protocol_interface_info_entry_t *cur)
{

    switch (cur->nwk_bootstrap_state) {
        case ER_WAIT_RESTART:
            tr_debug("WS SM:Wait for startup");
            ws_bootstrap_event_discovery_start(cur);
            break;
        case ER_ACTIVE_SCAN:
            tr_debug("WS SM:Active Scan");
            break;
        case ER_SCAN:
            tr_debug("WS SM:configuration Scan");
            break;
        case ER_PANA_AUTH:
            tr_info("authentication start");
            break;
        case ER_RPL_SCAN:
            tr_debug("WS SM:Wait RPL to contact DODAG root");
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

void ws_bootstrap_6lbr_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds)
{
    (void)cur;
    (void)seconds;
}


#endif //HAVE_WS_BORDER_ROUTER && HAVE_WS
