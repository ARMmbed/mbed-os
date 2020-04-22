/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * \file thread_bootstrap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#ifdef HAVE_THREAD
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "ns_sha256.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "ns_trace.h"
#include "coap_service_api.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_bbr_commercial.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_beacon.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "6LoWPAN/Thread/thread_ccm.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/Thread/thread_mle_message_handler.h"
#include "mac_api.h"
#include "RPL/rpl_control.h" // insanity - bootstraps shouldn't be doing each others' clean-up
#include "thread_management_if.h"
#include "thread_border_router_api.h"
#include "thread_tmfcop_lib.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MPL/mpl.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "thread_config.h"
#include "thread_meshcop_lib.h"
#include "multicast_api.h"
#include "mlme.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "platform/topo_trace.h"

#define TRACE_GROUP "thbs"

//#define EXTRA_DEBUG_INFO
#ifdef EXTRA_DEBUG_INFO
#define tr_debug_extra(...) tr_debug(__VA_ARGS__)
#else
#define tr_debug_extra(...)
#endif
void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur);

static void thread_bootstrap_orphan_scan_start(struct protocol_interface_info_entry *cur_interface);
static int thread_configuration_security_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration);
static void thread_interface_bootsrap_mode_init(protocol_interface_info_entry_t *cur);
static void thread_bootstrap_generate_leader_and_link(protocol_interface_info_entry_t *cur);
static int thread_bootstrap_attach_start(int8_t interface_id, thread_bootsrap_state_type_e state);
static void thread_bootsrap_network_discovery_failure(int8_t interface_id);

static void thread_neighbor_remove(mac_neighbor_table_entry_t *entry_ptr, void *user_data);
static void thread_bootsrap_network_join_start(struct protocol_interface_info_entry *cur_interface, discovery_response_list_t *nwk_info);

#ifdef HAVE_THREAD_V2

static bool enabled = false;
static uint32_t addr_notificastion_timer;
static uint32_t mlr_timer;

static void thread_bootstrap_pbbr_network_data_process(struct protocol_interface_info_entry *cur);
static void thread_bootstrap_pbbr_update_done(struct protocol_interface_info_entry *cur);

#else
#define thread_bootstrap_pbbr_network_data_process(cur)
#define thread_bootstrap_pbbr_update_done(cur)

#endif

static void thread_neighbor_remove(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{
    protocol_interface_info_entry_t *cur = user_data;
    lowpan_adaptation_neigh_remove_free_tx_tables(cur, entry_ptr);

    thread_reset_neighbour_info(cur, entry_ptr);
    //Removes ETX neighbor
    etx_neighbor_remove(cur->id, entry_ptr->index);
    //Remove MLE frame counter info
    mle_service_frame_counter_entry_delete(cur->id, entry_ptr->index);
}

static bool thread_neighbor_entry_nud_notify(mac_neighbor_table_entry_t *entry_ptr, void *user_data)
{
    protocol_interface_info_entry_t *cur_interface = user_data;

    if (thread_am_router(cur_interface)) {
        return false; //Never do Keep alive with any one
    }

    if (entry_ptr->link_role != PRIORITY_PARENT_NEIGHBOUR) {
        return false; //Do not never challenge than priority parent
    }

    if (cur_interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        return false; //Sleepy end device should not never challenge
    }

    if (entry_ptr->lifetime  > MLE_TABLE_CHALLENGE_TIMER) {
        return false;
    }

    return thread_host_bootstrap_child_update(cur_interface, entry_ptr->mac64);
}

int8_t thread_mle_class_init(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    mac_description_storage_size_t buffer;
    //Read MAC device table sizes
    if (cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
        return -1;
    }

    if (buffer.key_description_table_size < 4) {
        return -1;
    }

    thread_neighbor_class_delete(&cur->thread_info->neighbor_class);

    if (!thread_neighbor_class_create(&cur->thread_info->neighbor_class, buffer.device_decription_table_size - 1)) {
        return -1;
    }

    if (!mac_neighbor_info(cur)) {
        mac_neighbor_info(cur) = mac_neighbor_table_create(buffer.device_decription_table_size - 1, thread_neighbor_remove
                                                           , thread_neighbor_entry_nud_notify, cur);
        if (!mac_neighbor_info(cur)) {
            return -1;
        }
    }

    if (mle_service_frame_counter_table_allocate(interface_id, buffer.device_decription_table_size - 1)) {
        return -1;
    }

    if (!etx_storage_list_allocate(cur->id, buffer.device_decription_table_size - 1)) {
        return -1;
    }

    lowpan_adaptation_interface_etx_update_enable(cur->id);

    //Defined well know neighbour for discovery

    return 0;
}



uint8_t thread_mode_get_by_interface_ptr(protocol_interface_info_entry_t *cur)
{
    uint8_t mle_mode = 0;
    if (!thread_info(cur)) {
        return 0;
    }
    if (cur->mac_parameters->RxOnWhenIdle) {
        mle_mode |= MLE_RX_ON_IDLE;
    }

    if (thread_info(cur)->requestFullNetworkData) {
        mle_mode |= (MLE_THREAD_REQ_FULL_DATA_SET);
    }
    if (thread_joiner_application_provisioning_get(cur->id) == PROVISIONING_STATUS_NOT_DONE) {
        // if provisioning is not done Sleepy devices need to temporarily request full network data
        // To receive commissioner information
        mle_mode |= (MLE_THREAD_REQ_FULL_DATA_SET);
    }

    /* We always send secured data requests */
    mle_mode |= MLE_THREAD_SECURED_DATA_REQUEST;

    switch (thread_info(cur)->thread_device_mode) {
        case THREAD_DEVICE_MODE_ROUTER:
        case THREAD_DEVICE_MODE_FULL_END_DEVICE:
            mle_mode |= MLE_FFD_DEV;
            break;

        default:
            break;
    }


    return mle_mode;
}

/**
 * Return lower (worse) of the two margins.
 */
uint8_t thread_parent_margin_calc(uint8_t marginFromParent, uint8_t marginToParent)
{
    if (marginFromParent > marginToParent) {
        return marginToParent;
    } else {
        return marginFromParent;
    }
}

uint8_t thread_compute_link_margin(int8_t rssi)
{
    if (rssi < -94) {
        return 0;
    }

    return (rssi + 94);
}

uint8_t thread_calculate_link_margin(int8_t dbm, uint8_t compLinkMarginFromParent)
{
    uint8_t compLinkMarginToParent;
    uint8_t newLqi;

    compLinkMarginToParent = thread_compute_link_margin(dbm);
    //Calculate New Combined LQI
    newLqi = thread_parent_margin_calc(compLinkMarginFromParent, compLinkMarginToParent);
    return newLqi;
}

bool thread_check_is_this_my_parent(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *entry_temp)
{
    if (entry_temp && thread_info(cur)->thread_endnode_parent) {
        if (memcmp(entry_temp->mac64, thread_info(cur)->thread_endnode_parent->mac64, 8) == 0) {
            return true;
        }
    }
    return false;
}

bool thread_bootstrap_request_network_data(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData, uint16_t short_address)
{
    bool requestNetworkdata = false;
    thread_leader_data_t *leaderInfo = thread_info(cur)->thread_leader_data;

    if (thread_info(cur)->thread_endnode_parent->shortAddress != short_address) {
        return false;
    }

    if (!thread_partition_match(cur, leaderData)) {
        tr_debug("Learn new Network Data");
        requestNetworkdata = true;
        thread_partition_info_update(cur, leaderData);
    } else if (common_serial_number_greater_8(leaderData->dataVersion, leaderInfo->dataVersion)) {
        requestNetworkdata = true;

    } else if (common_serial_number_greater_8(leaderData->stableDataVersion, leaderInfo->stableDataVersion)) {
        requestNetworkdata = true;

    }

    if (requestNetworkdata) {
        thread_bootstrap_parent_network_data_request(cur, true);
    }
    return true;
}

static int thread_router_check_previous_partition_info(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData, mle_tlv_info_t *routeTlv)
{
    if (!routeTlv || !routeTlv->dataPtr || !routeTlv->tlvLen || !leaderData) {
        //check for parameters
        return -1;
    }
    if ((leaderData->partitionId == cur->thread_info->previous_partition_info.partitionId) &&
            (leaderData->weighting == cur->thread_info->previous_partition_info.weighting) &&
            (routeTlv->dataPtr[0] == cur->thread_info->previous_partition_info.idSequence)) {
        //drop the advertisement from previuos partition
        return 1;
    } else {
        //do not drop the advertisement
        return 0;
    }
}
int thread_bootstrap_partition_process(protocol_interface_info_entry_t *cur, uint8_t heard_partition_routers, thread_leader_data_t *heard_partition_leader_data, mle_tlv_info_t *routeTlv)
{
    uint8_t active_routers = 0;
    thread_leader_data_t *current_leader_data = NULL;

    /* if there scanned parent, then the comparison is between parent responses so retrieve the previously scanned parent info
     * else comparison is between existing leader data and heard advertisement leader data so retrieve existing leader data
     */
    if (thread_info(cur)->thread_attach_scanned_parent) {
        current_leader_data = &thread_info(cur)->thread_attach_scanned_parent->leader_data;
        active_routers = thread_info(cur)->thread_attach_scanned_parent->activeRouters;
    } else {
        current_leader_data = thread_info(cur)->thread_leader_data;
        active_routers = thread_routing_count_active_routers(&thread_info(cur)->routing);
    }

    if (!current_leader_data)  {
        tr_warn("There is no leader data present");
        return -2;
    }

    if (1 == thread_router_check_previous_partition_info(cur, heard_partition_leader_data, routeTlv)) {
        tr_debug("Dropping advertisement from old partition without sequence number increase");
        return -2;
    }

    /*Rule 0: If we are going to form Higher partition than heard we dont try to attach to lower ones
     */
    if (thread_common_ccm_enabled(cur) &&
            thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        if (heard_partition_leader_data->weighting < thread_info(cur)->partition_weighting) {
            tr_debug("Heard a lower weight partition");
            return -2;
        }
        if (heard_partition_leader_data->weighting > thread_info(cur)->partition_weighting) {
            return 2;
        }
    }

    if ((heard_partition_routers == 0 && active_routers == 1) && thread_am_router(cur)) {
        //heard a REED and I am a lonely router in a singleton partition, so merge
        tr_debug("Heard a REED and I am a singleton - merge");
        return 2;
    }

    //Rule 1: A non-singleton Thread Network Partition always has higher priority than a singleton Thread Network Partition
    if (heard_partition_routers > 1 && active_routers == 1) {
        tr_debug("Heard a nonsingleton and i am a singleton");
        return 2;
    }
    if (active_routers > 1 && heard_partition_routers == 1) {
        return -2;
    }

    /*Rule 2: When comparing two singleton or two non-singleton Thread Network Partitions,
    the one with the higher 8-bit weight value has higher priority. */
    if (heard_partition_leader_data->weighting > current_leader_data->weighting) {
        tr_debug("Heard a greater weighting");
        return 2;
    }

    if (heard_partition_leader_data->weighting < current_leader_data->weighting) {
        return -2;
    }

    /*Rule 3: When comparing two singleton or two non-singleton Thread Network Partitions that have the same 8-bit weight value,
     * the one with the higher Partition ID, considered as unsigned 32-bit numbers, has higher priority.
     */
    if (heard_partition_leader_data->partitionId > current_leader_data->partitionId) {
        tr_debug("Heard a greater partition id");
        return 2;
    }

    if (heard_partition_leader_data->partitionId < current_leader_data->partitionId) {
        return -2;
    }

    return -2;
}

int thread_leader_data_validation(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData, mle_tlv_info_t *routeTlv)
{
    if (!thread_info(cur)->thread_leader_data) {
        return -1;
    }
    if (!thread_partition_match(cur, leaderData)) {
        uint8_t routers_in_route_tlv = thread_get_router_count_from_route_tlv(routeTlv);
        //partition checks
        return thread_bootstrap_partition_process(cur, routers_in_route_tlv, leaderData, routeTlv);
    }

    //Should check is there new version numbers
    if (common_serial_number_greater_8(leaderData->dataVersion, thread_info(cur)->thread_leader_data->dataVersion) ||
            common_serial_number_greater_8(leaderData->stableDataVersion, thread_info(cur)->thread_leader_data->stableDataVersion)) {
        // Version number increased by some-one else -> there is leader in the network
        if (thread_info(cur)->leader_private_data) {
            tr_error("Another leader detected -> bootstrap");
            thread_bootstrap_reset_restart(cur->id);
            return -1;
        }
        tr_debug("NEW Network Data available");
        return 1;
    }

    return 0;
}

void thread_bootstrap_all_nodes_address_generate(uint8_t multicast_address[16], uint8_t prefix[8], uint8_t scope)
{
    memset(multicast_address, 0, 16);
    multicast_address[0] = 0xff;
    multicast_address[1] = 0x30 | scope; //Thread specification says p and t bits are 1
    multicast_address[2] = 0x00; //Reserved
    multicast_address[3] = 0x40; //Prefix length 64 bits
    memcpy(&multicast_address[4], prefix, 8);
    multicast_address[15] = 1;
}

void thread_bootstrap_all_nodes_multicast_register(protocol_interface_info_entry_t *cur)
{
    uint8_t multicast_address[16];

    switch (cur->thread_info->thread_device_mode) {
#ifdef HAVE_THREAD_ROUTER
        case THREAD_DEVICE_MODE_ROUTER:
            cur->if_special_multicast_forwarding = thread_router_bootstrap_multicast_forwarder_enable;
            break;
#endif

        default:
            cur->if_special_multicast_forwarding = NULL;
            break;
    }

    // Register to link local all thread nodes multicast
    thread_bootstrap_all_nodes_address_generate(multicast_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_LINK_LOCAL);
    tr_debug("Register multicast address: %s", trace_ipv6(multicast_address));
    addr_add_group(cur, multicast_address);

    // Register to mesh local all thread nodes multicast
    thread_bootstrap_all_nodes_address_generate(multicast_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_REALM_LOCAL);
    tr_debug("Register multicast address: %s", trace_ipv6(multicast_address));
    addr_add_group(cur, multicast_address);
}

void thread_bootstrap_all_nodes_multicast_unregister(protocol_interface_info_entry_t *cur)
{
    uint8_t multicast_address[16];

    if (!cur->thread_info->threadPrivatePrefixInfo.ulaValid) {
        //Prefix not valid do not delete
        return;
    }
    // Unregister to link local all thread nodes multicast
    thread_bootstrap_all_nodes_address_generate(multicast_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 2);
    tr_debug("Free multicast address: %s", trace_ipv6(multicast_address));
    //multicast_free_address(multicast_address);
    addr_remove_group(cur, multicast_address);

    // Unregister to mesh local all thread nodes multicast
    thread_bootstrap_all_nodes_address_generate(multicast_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 3);
    tr_debug("Free multicast address: %s", trace_ipv6(multicast_address));
    //multicast_free_address(multicast_address);
    addr_remove_group(cur, multicast_address);
}

void thread_end_device_mode_set(protocol_interface_info_entry_t *cur, bool sleepy)
{
    if (sleepy) {
        cur->lowpan_info |= INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, false);
    } else {
        cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
    }
}


int8_t nwk_thread_host_control(protocol_interface_info_entry_t *cur, net_host_mode_t mode, uint32_t delay)
{
    int8_t ret_val = 0;
    //Check IF Bootsrap Ready and type is Host

    if (cur == NULL || cur->rfd_poll_info == NULL) {
        return -1;
    }

    nwk_rfd_poll_setups_s *rf_ptr = cur->rfd_poll_info;
    switch (mode) {
        case NET_HOST_FAST_POLL_MODE:
            //fast mode
            //Check Host current sleep state
            if (rf_ptr->host_mode == NET_HOST_RX_ON_IDLE) {
                tr_debug("Enable Fast poll mode. Init Poll timer and period");
                thread_end_device_mode_set(cur, true);
                mac_poll_timer_trig(delay, cur);
                rf_ptr->nwk_app_poll_time = 300;
                rf_ptr->host_mode = NET_HOST_FAST_POLL_MODE;
            }
            break;

        case NET_HOST_RX_ON_IDLE:
            // Non-sleep mode
            thread_end_device_mode_set(cur, false);
            rf_ptr->host_mode = NET_HOST_RX_ON_IDLE;
            break;

        default:
            ret_val = -1;
            break;
    }

    return ret_val;
}

void thread_set_link_local_address(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach_safe(if_address_entry_t, addr, &cur->ip_addresses) {
        if (memcmp(addr->address, ADDR_LINK_LOCAL_PREFIX, 8) == 0) {
            tr_debug("deleting address %s", trace_ipv6(addr->address));
            ns_list_remove(&cur->ip_addresses, addr);
            ns_dyn_mem_free(addr);
        }
    }

    /* Fix EUId64 also  to start use 0*/
    memcpy(cur->iid_eui64, cur->mac, 8);
    cur->iid_eui64[0] ^= 2;
    addr_interface_set_ll64(cur, NULL);
}

static int thread_configuration_security_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    tr_debug("MAC SET Security Mode");

    if (!(cur->lowpan_info & INTERFACE_NWK_ACTIVE) || !(cur->configure_flags & INTERFACE_BOOTSTRAP_DEFINED)) {
        return -1;
    }
    mac_helper_security_key_clean(cur);
    mac_helper_default_security_level_set(cur, 5);
    mac_helper_default_security_key_id_mode_set(cur, MAC_KEY_ID_MODE_IDX);

    cur->if_lowpan_security_params->nwk_security_mode = NET_SEC_MODE_PSK_LINK_SECURITY;
    cur->mac_parameters->mac_configured_sec_level = 5;
    cur->thread_info->masterSecretMaterial.historyKeyValid = false;
    cur->thread_info->masterSecretMaterial.valid_Info = true;
    // Update the guard timer value
    thread_key_guard_timer_calculate(cur, linkConfiguration, true);
    //Define KEY's
    thread_security_prev_key_generate(cur, linkConfiguration->master_key, linkConfiguration->key_sequence);
    thread_security_key_generate(cur, linkConfiguration->master_key, linkConfiguration->key_sequence);
    thread_security_next_key_generate(cur, linkConfiguration->master_key, linkConfiguration->key_sequence);
    return 0;
}

void thread_bootstrap_mac_activate(protocol_interface_info_entry_t *cur, uint16_t channel, uint16_t panid, bool coordinator)
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

    thread_discovery_responser_enable(cur->id, coordinator);

    if (cur->mac_api) {
        cur->mac_api->mlme_req(cur->mac_api, MLME_START, (void *)&start_req);
    }
}

int thread_configuration_mac_activate(protocol_interface_info_entry_t *cur, uint16_t channel, uint16_t panid, uint8_t *extended_random_mac)
{
    ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);
    mac_helper_mac64_set(cur, extended_random_mac);
    thread_set_link_local_address(cur);

    //SET Thread default here
    mac_helper_mac_mlme_max_retry_set(cur->id, THREAD_MAX_FRAME_RETRIES);

    bool coordinator = (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER);
    thread_bootstrap_mac_activate(cur, channel, panid, coordinator);

    mac_data_poll_init(cur);

    return 0;
}

int thread_configuration_6lowpan_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("6lowpan configure");
    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
    cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
    return 0;
}

static void thread_bootstrap_ml_address_update(protocol_interface_info_entry_t *cur, const link_configuration_s *conf)
{
    tr_debug("Updating ML addresses and prefix information.");
    uint8_t address[16];

    if (cur->thread_info->threadPrivatePrefixInfo.ulaValid &&
            memcmp(cur->thread_info->threadPrivatePrefixInfo.ulaPrefix,
                   conf->mesh_local_ula_prefix, 8) != 0) {
        // Current prefix is valid and old vs. new different: update old addresses
        // Update the addresses in the neighbor cache (replace the old ULA prefix part)
        ns_list_foreach(ipv6_neighbour_t, entry, &cur->ipv6_neighbour_cache.list) {
            tr_debug("Neighbor cache address: %s", trace_ipv6(entry->ip_address));
            if (bitsequal(entry->ip_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 64)) {
                memcpy(entry->ip_address, conf->mesh_local_ula_prefix, 8);
                tr_debug("Updated to %s.", trace_ipv6(entry->ip_address));
            }
        }

        // Delete the ML64 address
        thread_delete_ml64_address(cur);

        // Free previously registered multicast addresses
        thread_bootstrap_all_nodes_multicast_unregister(cur);

        // In-place replace all other mesh local addresses...
        ns_list_foreach_safe(if_address_entry_t, e, &cur->ip_addresses) {
            tr_debug("IP address: %s", trace_ipv6(e->address));
            if (bitsequal(e->address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 64)) {
                memcpy(address, conf->mesh_local_ula_prefix, 8);
                memcpy(address + 8, e->address + 8, 8);
                tr_debug("Updated to: %s", trace_ipv6(address));
                addr_add(cur, address, e->prefix_len, e->source, e->valid_lifetime, e->preferred_lifetime, true);
                addr_delete_entry(cur, e);
            }
        }
    }

    // Set new ML-EID and ULA prefix
    uint8_t *ml_eid = thread_joiner_application_ml_eid_get(cur->id);
    memcpy(cur->iid_slaac, ml_eid, 8);

    arm_thread_private_ula_prefix_set(cur, conf->mesh_local_ula_prefix);

    // Generate new ML64 address
    thread_generate_ml64_address(cur);

    // Register multicast addresses
    thread_bootstrap_all_nodes_multicast_register(cur);

    // Register leader anycast address (if we are the leader)
    thread_router_bootstrap_anycast_address_register(cur);

    thread_bootstrap_routing_activate(cur);
}

int thread_configuration_thread_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    tr_debug("thread configure");

    // Update existing mesh-local addresses and the ML prefix
    thread_bootstrap_ml_address_update(cur, linkConfiguration);

    //Define Default Contexts
    lowpan_context_update(&cur->lowpan_contexts, LOWPAN_CONTEXT_C, 0xFFFF, linkConfiguration->mesh_local_ula_prefix, 64, true);

    thread_bbr_commercial_route_update(cur);

    blacklist_clear();

    blacklist_params_set(
        THREAD_BLACKLIST_ENTRY_LIFETIME,
        THREAD_BLACKLIST_TIMER_MAX_TIMEOUT,
        THREAD_BLACKLIST_TIMER_TIMEOUT,
        THREAD_BLACKLIST_ENTRY_MAX_NBR,
        THREAD_BLACKLIST_PURGE_NBR,
        THREAD_BLACKLIST_PURGE_TIMER_TIMEOUT);

    return 0;
}

int thread_configuration_mle_activate(protocol_interface_info_entry_t *cur)
{
    tr_debug("thread MLE configure");
    mle_service_interface_receiver_handler_update(cur->id, thread_general_mle_receive_cb);
    return 0;
}

int thread_configuration_mle_disable(protocol_interface_info_entry_t *cur)
{
    tr_debug("thread MLE disable");
    mle_service_interface_receiver_handler_update(cur->id, NULL);
    return 0;
}

static int thread_mle_service_register(protocol_interface_info_entry_t *cur, uint8_t *mac64)
{
    if (mle_service_interface_register(cur->id, cur, thread_mle_parent_discover_receive_cb, mac64, 8) != 0) {
        tr_error("Mle Service init Fail");
        return -1;
    }
    mle_service_set_frame_counter_check(true);
    mle_service_set_fragmented_msg_ll_security(true);
    return 0;
}

int thread_link_configuration_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    //Generate Beacon Payload from active configuration
    if (thread_beacon_create_payload(cur) != 0) {
        return -1;
    }

    if (thread_configuration_mac_activate(cur, linkConfiguration->rfChannel, linkConfiguration->panId, thread_joiner_application_random_mac_get(cur->id))) {
        return -1;
    }

    mac_helper_mac_device_description_pan_id_update(cur->id, linkConfiguration->panId);

    thread_configuration_thread_activate(cur, linkConfiguration);
    thread_configuration_security_activate(cur, linkConfiguration);
    thread_configuration_6lowpan_activate(cur);
    return 0;
}

int thread_bootstrap_announce_send(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp, uint16_t selected_channel)
{
    uint8_t *ptr;
    uint8_t channel_tlv[3];
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, 128, false, MLE_COMMAND_DATASET_ANNOUNCE);
    if (buf_id == 0) {
        return -1;
    }

    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_NODES);

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_set_msg_link_layer_security_mode(buf_id, true);

    ptr = mle_service_get_data_pointer(buf_id);

    ptr = thread_meshcop_tlv_data_write_uint64(ptr, MLE_TYPE_ACTIVE_TIMESTAMP, timestamp);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MLE_TYPE_PANID, panid);
    channel_tlv[0] = channel_page;
    common_write_16_bit(channel, &channel_tlv[1]);
    ptr = thread_meshcop_tlv_data_write(ptr, MLE_TYPE_CHANNEL, 3, channel_tlv);

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    //SET packet channel
    mle_service_set_msg_rf_channel(buf_id, selected_channel);

    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_set_msg_panid(buf_id, 0xffff);
    mle_service_send_message(buf_id);
    return 0;
}
static void thread_announce_ntf_cb(void *arg)
{
    if (!arg) {
        return;
    }
    protocol_interface_info_entry_t *cur = arg;
    cur->thread_info->announcement_info->timer = NULL;
    thread_bootsrap_event_trig(THREAD_ANNOUNCE_ACTIVE, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

static void thread_announce_success_cb(void *arg)
{
    // We come here when we have succesfully attached to announced channel and then we announce this back
    protocol_interface_info_entry_t *cur = arg;

    if (!cur || !cur->thread_info->announcement_info) {
        return;
    }

    cur->thread_info->announcement_info->timer = NULL;
    cur->thread_info->announcement_info->announce_success = false;
    thread_bootstrap_announcement_start(cur, cur->thread_info->announcement_info->channel_page, cur->thread_info->announcement_info->channel, 3, cur->thread_info->announcement_info->period);
}

void thread_bootstrap_announcement_start(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint8_t count, uint16_t period)
{
    if (!cur->thread_info->announcement_info) {
        cur->thread_info->announcement_info = ns_dyn_mem_alloc(sizeof(thread_announcement_t));
    }
    if (!cur->thread_info->announcement_info) {
        return;
    }
    tr_info("Start announcement ch: %d", channel);
    cur->thread_info->announcement_info->channel = channel;
    cur->thread_info->announcement_info->period = period;
    cur->thread_info->announcement_info->channel_page = channel_page;
    cur->thread_info->announcement_info->count = count;
    cur->thread_info->announcement_info->timer = NULL;
    cur->thread_info->announcement_info->announce_success = false;
    cur->thread_info->announcement_info->timestamp = 0;
    thread_bootsrap_event_trig(THREAD_ANNOUNCE_ACTIVE, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}
void thread_bootstrap_temporary_attach(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp)
{
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);

    if (!linkConfiguration) {
        tr_error("No link configuration!");
        return;
    }

    tr_debug("Attach to new channel %u", channel);

    /*
     * Save the old info and make a timer to announce it to old channels once if attachment is succesfull
     * When we receive attach fail
     */
    if (!cur->thread_info->announcement_info) {
        cur->thread_info->announcement_info = ns_dyn_mem_alloc(sizeof(thread_announcement_t));
    }
    if (!cur->thread_info->announcement_info) {
        return;
    }
    cur->thread_info->announcement_info->channel = linkConfiguration->rfChannel;
    cur->thread_info->announcement_info->channel_page = linkConfiguration->channel_page;
    cur->thread_info->announcement_info->panid = linkConfiguration->panId;
    cur->thread_info->announcement_info->count = 1;
    cur->thread_info->announcement_info->period = 1000;
    cur->thread_info->announcement_info->timestamp = timestamp;
    cur->thread_info->announcement_info->timer = eventOS_timeout_ms(thread_announce_success_cb, 20000, cur);
    // TODO check timer value
    cur->thread_info->announcement_info->announce_success = true;
    linkConfiguration->channel_page = channel_page;
    linkConfiguration->rfChannel = channel;
    linkConfiguration->panId = panid;
    thread_joiner_application_link_configuration_store(cur->id, linkConfiguration);
    thread_bootstrap_reset_restart(cur->id);
}

static const trickle_params_t thread_mpl_data_trickle_params = {
    .Imin = 1, /* 50ms */
    .Imax = 2, /* 100ms */
    .k = 0,
    .TimerExpirations = 2 /* MPL core knows to suppress to 0 for non-routers */
};

static const trickle_params_t thread_mpl_control_trickle_params = {
    .Imin = 11,
    .Imax = 5 * 60 * 20,
    .k = 0,
    .TimerExpirations = 0
};

void thread_interface_init(protocol_interface_info_entry_t *cur)
{
    thread_discovery_reset(cur->id);
    thread_routing_set_mesh_callbacks(cur);
    dhcp_client_init(cur->id, DHCPV6_DUID_HARDWARE_EUI64_TYPE);
    dhcp_client_configure(cur->id, false, false, false);
    thread_management_client_init(cur->id);
    thread_bootstrap_address_registration_init();
    cur->mpl_seed_id_mode = MULTICAST_MPL_SEED_ID_MAC_SHORT;
    cur->mpl_seed_set_entry_lifetime = 90;
    cur->mpl_proactive_forwarding = true;
    cur->mpl_control_trickle_params = thread_mpl_control_trickle_params;
    cur->mpl_data_trickle_params = thread_mpl_data_trickle_params;
    cur->mpl_seed = true;
    cur->mpl_treat_realm_domains_as_one = true;
    cur->if_ns_transmit = thread_nd_ns_transmit;
    cur->if_special_forwarding = thread_nd_special_forwarding;
    cur->if_snoop = thread_nd_snoop;
    cur->if_icmp_handler = thread_nd_icmp_handler;
    cur->ipv6_neighbour_cache.send_nud_probes = false;
    cur->ipv6_neighbour_cache.probe_avoided_routers = false;
    cur->ipv6_neighbour_cache.recv_addr_reg = true;
    cur->send_mld = false;
    cur->ip_multicast_as_mac_unicast_to_parent = true;
    if (!cur->thread_info->routerShortAddress) {
        cur->thread_info->routerShortAddress = 0xfffe;
    }
    if (cur->thread_info->thread_attach_scanned_parent) {
        mle_service_msg_free(cur->thread_info->thread_attach_scanned_parent->child_id_request_id);
        ns_dyn_mem_free(cur->thread_info->thread_attach_scanned_parent);
        cur->thread_info->thread_attach_scanned_parent = NULL;
    }
    //Disable Always RPL
    rpl_control_remove_domain_from_interface(cur);
    mpl_domain_create(cur, ADDR_ALL_MPL_FORWARDERS, NULL, MULTICAST_MPL_SEED_ID_DEFAULT, -1, 0, NULL, NULL);
    addr_add_group(cur, ADDR_REALM_LOCAL_ALL_NODES);
    cur->nwk_nd_re_scan_count = 5;

}

static void thread_interface_bootsrap_mode_init(protocol_interface_info_entry_t *cur)
{
    thread_routing_reset(&cur->thread_info->routing);
    mac_helper_mac16_address_set(cur, 0xffff);

    if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER) {
        tr_debug("Set ASPIRING Router Mode");
        cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_ROUTER;
        cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
    } else if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST &&
               cur->thread_info->end_device_link_synch) {
        tr_debug("Set FED Mode");
        cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_FULL_END_DEVICE;
    } else if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST) {
        tr_debug("Set ASPIRING Sleepy Host Mode");
        cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_SLEEPY_END_DEVICE;
        //SET Sleepy Host To RX on Idle mode for bootsrap
        nwk_thread_host_control(cur, NET_HOST_RX_ON_IDLE, 0);
        cur->thread_info->childUpdateReqTimer = 8 * cur->thread_info->host_link_timeout / 10;
    } else {
        tr_debug("Set End node Mode");
        cur->thread_info->thread_device_mode = THREAD_DEVICE_MODE_END_DEVICE;
    }

    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        // set router neighbour cache
        ipv6_neighbour_cache_configure(THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_SIZE,
                                       THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_SHORT_TERM,
                                       THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_LONG_TERM,
                                       THREAD_ROUTER_IPV6_NEIGHBOUR_CACHE_LIFETIME);
        // set router destination cache
        ipv6_destination_cache_configure(THREAD_ROUTER_IPV6_DESTINATION_CACHE_SIZE,
                                         THREAD_ROUTER_IPV6_DESTINATION_CACHE_SHORT_TERM,
                                         THREAD_ROUTER_IPV6_DESTINATION_CACHE_LONG_TERM,
                                         THREAD_ROUTER_IPV6_DESTINATION_CACHE_LIFETIME);
    } else {
        // device is some sort of end device
        ipv6_neighbour_cache_configure(THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_SIZE,
                                       THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_SHORT_TERM,
                                       THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_LONG_TERM,
                                       THREAD_END_DEVICE_IPV6_NEIGHBOUR_CACHE_LIFETIME);

        ipv6_destination_cache_configure(THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_SIZE,
                                         THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_SHORT_TERM,
                                         THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_LONG_TERM,
                                         THREAD_END_DEVICE_IPV6_DESTINATION_CACHE_LIFETIME);
    }

    cur->thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
}

int8_t thread_bootsrap_event_trig(thread_bootsrap_event_type_e event_type, int8_t Id, arm_library_event_priority_e priority)
{
    arm_event_s event = {
        .receiver = Id,
        .sender = 0,
        .event_type = event_type,
        .priority = priority,
    };
    return eventOS_event_send(&event);
}

void thread_bootstrap_reset_restart(int8_t interface)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface);
    if (!cur) {
        return;
    }

    if (cur->nwk_bootstrap_state == ER_MLE_SYNCH) {
        thread_network_synch_data_free(cur->id);
    }
    thread_nd_service_disable(interface);
    thread_routing_deactivate(&cur->thread_info->routing);
    //TODO: clear CoAP resending queue
    thread_bootsrap_event_trig(THREAD_BOOTSTRAP_RESET, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_tasklet(arm_event_s *event)
{

    protocol_interface_info_entry_t *cur = 0;
    thread_bootsrap_event_type_e event_type;
    event_type = (thread_bootsrap_event_type_e)event->event_type;
    cur = protocol_stack_interface_info_get_by_bootstrap_id(event->receiver);
    if (!cur) {
        tr_debug("Thread task unknown");
        return;
    }

    switch (event_type) {
        case THREAD_INIT_EVENT:
            tr_debug_extra("Thread SM THREAD_INIT_EVENT");
            tr_debug("Thread task Init");
            break;

        case THREAD_BOOTSTRAP_RESET:
            //Reset Current Thread state
            tr_debug_extra("Thread SM THREAD_BOOTSTRAP_RESET");
            if (thread_bootstrap_reset(cur) == 0) {
                tr_debug("Thread Attached");
            } else {
                tr_debug("Stop Bootsrap and send event");
            }
            break;

        case THREAD_ATTACH_READY:
            tr_debug_extra("Thread SM THREAD_ATTACH_READY");
            thread_bootstrap_attached_finish(cur);
            break;
        case THREAD_ATTACH_UPGRADE_REED:
            tr_debug_extra("Thread SM THREAD_ATTACH_UPGRADE_REED");
            if (thread_router_bootstrap_child_count_get(cur) > 0) {
                thread_router_bootstrap_router_id_request(cur, THREAD_COAP_STATUS_TLV_PARENT_PARTITION_CHANGE);
            } else {
                thread_router_bootstrap_router_id_request(cur, THREAD_COAP_STATUS_TLV_TOO_FEW_ROUTERS);
            }

            break;

        case THREAD_ATTACH_DOWNGRADE_ROUTER:
            tr_debug_extra("Thread SM THREAD_ATTACH_DOWNGRADE_ROUTER");
            thread_bootstrap_attach_start(cur->id, THREAD_REATTACH_REED);
            break;

        case THREAD_ATTACH_ACTIVE_ROUTER:
            tr_debug_extra("Thread SM THREAD_ATTACH_ACTIVE_ROUTER");
            thread_bootstrap_all_nodes_multicast_register(cur);
            thread_router_bootstrap_anycast_address_register(cur);
            thread_router_bootstrap_active_router_attach(cur);
            thread_bootstrap_child_id_request(cur);
            if (thread_nd_own_service_list_data_size(&cur->thread_info->localServerDataBase)) {
                // publish our services to allow leader to remove old ones
                thread_border_router_publish(cur->id);
            }
            thread_router_bootstrap_address_change_notify_send(cur);
            // Validate network data after a short period
            thread_border_router_resubmit_timer_set(cur->id, 5);
            break;
        case THREAD_ATTACH_ROUTER_ID_GET_FAIL:
            tr_debug_extra("Thread SM THREAD_ATTACH_ROUTER_ID_GET_FAIL");
            tr_debug("Thread Router Id request Fail");
            cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
            cur->thread_info->thread_attached_state = THREAD_STATE_CONNECTED;
            thread_router_bootstrap_child_information_clear(cur);
            thread_router_bootstrap_reed_advertisements_start(cur);
            thread_router_bootstrap_child_id_reject(cur);
            break;

        case THREAD_ATTACH_ROUTER_ID_RELEASED:
            tr_debug_extra("Thread SM THREAD_ATTACH_ROUTER_ID_RELEASED");
            if (thread_nd_own_service_list_data_size(&cur->thread_info->localServerDataBase)) {
                // publish our services to allow leader to remove old ones
                thread_border_router_publish(cur->id);
            }
            break;

        case THREAD_CHILD_ID_REQUEST:
            tr_debug_extra("Thread SM THREAD_CHILD_ID_REQUEST");
            thread_router_bootstrap_child_id_handler(cur);
            break;

        case THREAD_CHILD_UPDATE:
            tr_debug_extra("Thread SM THREAD_CHILD_UPDATE");
            thread_host_bootstrap_child_update(cur, cur->thread_info->thread_endnode_parent->mac64);
            break;
        case THREAD_ANNOUNCE_ACTIVE: {
            tr_debug_extra("Thread SM THREAD_ANNOUNCE_ACTIVE");

            if (cur->thread_info->announcement_info->count > 0) {
                cur->thread_info->announcement_info->count--;

                link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);
                if (!linkConfiguration) {
                    tr_error("No link configuration!");
                    break;
                }

                // New timeout needed
                cur->thread_info->announcement_info->timer = eventOS_timeout_ms(thread_announce_ntf_cb, cur->thread_info->announcement_info->period, cur);

                // Send announce_ntf
                thread_bootstrap_announce_send(cur, linkConfiguration->channel_page, linkConfiguration->rfChannel, linkConfiguration->panId, linkConfiguration->timestamp, cur->thread_info->announcement_info->channel);
            } else {
                // Last call, delete announcement info
                ns_dyn_mem_free(cur->thread_info->announcement_info);
                cur->thread_info->announcement_info = NULL;
            }

            break;
        }

        default:
            break;
    }
}


int thread_bootstrap_tasklet_init(protocol_interface_info_entry_t *cur)
{
    if (cur->bootStrapId < 0) {
        cur->bootStrapId = eventOS_event_handler_create(&thread_tasklet, THREAD_INIT_EVENT);
        tr_debug("Allocate Thread Tasklet");
    }
    if (cur->bootStrapId >= 0) {
        return 0;
    }
    return -1;
}

int thread_proxy_validate(int8_t interface_id, uint8_t *addrerss)
{
    ipv6_route_t *route;
    route = ipv6_route_choose_next_hop(addrerss, interface_id, NULL);
    if (!route) {
        return -1;
    }

    if (route->prefix_len < 128 || !route->on_link) {
        return -1;
    }

    return 0;
}

void thread_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
    cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;

    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        cur->ip_multicast_as_mac_unicast_to_parent = false;
        cur->ip_forwarding = true;
    } else {
        cur->ip_multicast_as_mac_unicast_to_parent = true;
        cur->ip_forwarding = false;
        if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
            /* REED should be able to IP forward */
            cur->ip_forwarding = true;
        } else {
            cur->ip_forwarding = false;
        }
    }

    tr_info("Set forwarding: ip=%d, multicast=%d", cur->ip_forwarding, thread_i_am_router(cur));

    if (cur->ip_forwarding) {
        addr_add_router_groups(cur);
    }
    // Bizarrely, Thread needs Realm-All-Routers active in FEDs for address resolution
    if (cur->ip_forwarding || cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE) {
        addr_add_group(cur, ADDR_REALM_LOCAL_ALL_ROUTERS);
    }

    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED) {
            thread_router_bootstrap_reed_advertisements_start(cur);
        }

        thread_bootstrap_mac_activate(cur, cur->mac_parameters->mac_channel, cur->mac_parameters->pan_id, true);

        if (nd_proxy_downstream_interface_register(cur->id, thread_proxy_validate, thread_bbr_proxy_state_update) != 0) {
            tr_debug("mesh proxy register fail");
        }
    }

    if (cur->thread_info->leader_private_data) {
        // Generate network data from network data structures
        thread_leader_service_generate_network_data(cur);
    }

    if (thread_addresses_needs_to_be_registered(cur)) {
        thread_info(cur)->childUpdateReqTimer = 1;
    }

    cur->bootsrap_state_machine_cnt = 0;
    mac_data_poll_protocol_poll_mode_decrement(cur);
}

void thread_neighbor_list_clean(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (!thread_addr_is_equal_or_child(cur->thread_info->routerShortAddress, cur_entry->mac16)) {
            tr_debug("Free ID %x", cur_entry->mac16);
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), cur_entry);
        }
    }
}

void thread_reed_fed_neighbour_links_clean(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    if (thread_i_am_router(cur)) {
        return;
    }

    if (thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE ||
            thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        return;
    }

    if (!thread_info(cur)->thread_endnode_parent) {
        return;
    }
    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        // do not remove parent entry
        if (memcmp(cur_entry->mac64, thread_info(cur)->thread_endnode_parent->mac64, 8) != 0) {
            tr_debug("Free short addr: %x", cur_entry->mac16);
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), cur_entry);
        }
    }
}

void thread_clean_old_16_bit_address_based_addresses(protocol_interface_info_entry_t *cur)
{
    uint8_t static_address[16];
    //Delete old ULA16
    memcpy(static_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&static_address[8], ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(mac_helper_mac16_address_get(cur), &static_address[14]);
    addr_delete(cur, static_address);
    mac_helper_mac16_address_set(cur, 0xffff);

}

static int thread_bootstrap_attach_start(int8_t interface_id, thread_bootsrap_state_type_e state)
{

    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    //Trigger the bootstrap

    nwk_thread_host_control(cur, NET_HOST_RX_ON_IDLE, 0);

    switch (state) {
        case THREAD_NORMAL_ATTACH:
            cur->thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
            mac_helper_default_security_level_set(cur, cur->mac_parameters->mac_configured_sec_level);
            mac_helper_default_security_key_id_mode_set(cur, MAC_KEY_ID_MODE_IDX);
            break;

        case THREAD_REATTACH:
            tr_debug("Thread ReAttach");
            //save the current partition id and sequence number before trying reattach
            cur->thread_info->previous_partition_info.partitionId = cur->thread_info->thread_leader_data->partitionId;
            cur->thread_info->previous_partition_info.weighting = cur->thread_info->thread_leader_data->weighting;
            cur->thread_info->previous_partition_info.idSequence = cur->thread_info->routing.router_id_sequence;
            cur->thread_info->routerShortAddress = mac_helper_mac16_address_get(cur);
            if (cur->thread_info->thread_attached_state != THREAD_STATE_REATTACH_RETRY) {
                cur->thread_info->thread_attached_state = THREAD_STATE_REATTACH;
            }
            break;
        case THREAD_PARTITION_MERGE:
            cur->thread_info->routerShortAddress = mac_helper_mac16_address_get(cur);
            break;
        case THREAD_ANY_ATTACH:
            cur->thread_info->routerShortAddress = mac_helper_mac16_address_get(cur);
            cur->thread_info->thread_attached_state = THREAD_STATE_ATTACH_ANY;
            break;

        case THREAD_REATTACH_REED:
            cur->thread_info->releaseRouterId = true;
            cur->thread_info->routerShortAddress = mac_helper_mac16_address_get(cur);

            if (cur->thread_info->thread_attached_state != THREAD_STATE_REATTACH_RETRY) {
                cur->thread_info->thread_attached_state = THREAD_STATE_REATTACH;
            }
            break;
    }

    // Set RX on idle
    thread_end_device_mode_set(cur, false);
    cur->nwk_nd_re_scan_count = 0;
    cur->nwk_bootstrap_state = ER_SCAN;
    cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(1, 10);
    return 0;
}

static void thread_bootsrap_network_discovery_failure(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return;
    }
//TODO we should send 3 in burst of 0.1 - 0.6 seconds and then do the exponential backup of 5s -- 80s
    uint32_t backof_delay = cur->nwk_nd_re_scan_count * 2;
    if (backof_delay > 600) {
        backof_delay = 600;    //TODO test this and check guess this is 100ms ticks
    }

    tr_debug("Continue network scan");
    cur->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    cur->bootsrap_state_machine_cnt = backof_delay + randLIB_get_random_in_range(1, 6);
}

static void thread_bootstrap_generate_leader_and_link(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH) {
        tr_debug("ReAttach Fail - retry");
        thread_bootstrap_attach_start(cur->id, THREAD_REATTACH);
        cur->thread_info->thread_attached_state = THREAD_STATE_REATTACH_RETRY;
    } else if (cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH_RETRY) {
        tr_warn("ReAttach Fail");
        thread_bootstrap_attach_start(cur->id, THREAD_ANY_ATTACH);
    } else {
        if (cur->thread_info->thread_attached_state == THREAD_STATE_NETWORK_DISCOVER) {
            bootsrap_next_state_kick(ER_BOOTSTRAP_LEADER_UP, cur);
        } else {
            bootsrap_next_state_kick(ER_BOOTSTRAP_NEW_FRAGMENT_START, cur);
        }
    }
}
static int8_t thread_bootstrap_attempt_attach_with_pending_set(protocol_interface_info_entry_t *cur)
{
    tr_debug("Attempting to attach with pending set");
    uint32_t pending_delay_timer = thread_joiner_application_pending_config_timeout_get(cur->id);
    if (pending_delay_timer > 0 && thread_joiner_application_pending_delay_timer_in_sync(cur->id)) {
        tr_debug("We have a pending delay timer running");
        //we already have a pending set that can be activated so return
        return -1;
    }

    if (!thread_joiner_application_pending_config_exists(cur->id)) {
        tr_debug("no pending configuration found after reset");
        return -1;
    }

    if (thread_joiner_application_old_config_exists(cur->id)) {
        //there is an existing old configuration so attempt to attach with it and set the pending timer to expire
        thread_joiner_application_old_config_activate(cur->id);
        thread_joiner_application_old_config_delete(cur->id);
        thread_joiner_application_pending_config_enable(cur->id, 20000);
    } else {
        thread_joiner_pending_config_activate(cur->id);
    }

    return 0;
}

static void thread_bootstrap_orphan_scan_ready_cb(struct protocol_interface_info_entry *cur_interface, announce_discovery_response_t *discover_response)
{

    if (!discover_response) {
        thread_bootstrap_orphan_scan_start(cur_interface);
        return;
    }

    link_configuration_s *link_configuration = thread_joiner_application_get_config(cur_interface->id);

    if (!link_configuration) {
        tr_debug("no link configuration found after reset");
        return;
    }

    tr_debug("New configuration received channel %u, %x", discover_response->channel, discover_response->pan_id);
    link_configuration->panId = discover_response->pan_id;
    link_configuration->rfChannel = discover_response->channel;
    link_configuration->channel_page = 0;
    cur_interface->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    cur_interface->bootsrap_state_machine_cnt = 1;
    ns_dyn_mem_free(discover_response);
}

static void thread_bootstrap_orphan_scan_start(struct protocol_interface_info_entry *cur)
{
    thread_announce_discover_reques_t scan_req;
    //default channel mask for channels 11 to 26
    scan_req.channel_mask = 0x001fffe0;
#ifdef THREAD_THCI_SUPPORT
    //use active operational dataset's channel mask if available
    link_configuration_s *link_configuration = thread_joiner_application_get_config(cur->id);
    if (!link_configuration) {
        return;
    }
    scan_req.channel_mask = common_read_32_bit(link_configuration->channel_mask);
    //the current channel is added to the mask.
    scan_req.channel_mask |= (0x80000000 >> link_configuration->rfChannel);
#endif
    scan_req.pan_id = cur->mac_parameters->pan_id;
    scan_req.active_timestamp = 0;
    scan_req.active_timestamp |= MESHCOP_TLV_ACTIVE_TIME_STAMP_U_BIT;

    if (thread_discovery_announce_network_scan(cur->id, &scan_req, thread_bootstrap_orphan_scan_ready_cb) != 0) {

        tr_debug("announce discover start fail");
        thread_bootstrap_attach_start(cur->id, THREAD_NORMAL_ATTACH);
    } else {
        tr_debug("Orphan Host start announce scan");
    }
    return;
}

void thread_bootstrap_connection_error(int8_t interface_id, nwk_connect_error_types errorType, uint8_t *LinkId)
{
    (void)LinkId;
    protocol_interface_info_entry_t *cur;

    switch (errorType) {
        case CON_ERROR_POLL:
        case CON_PARENT_CONNECT_DOWN:
            thread_bootstrap_reset_restart(interface_id);
            break;

        case CON_ERROR_LINK_TX_FAIL:
            //thread_mle_challenge_trig(interface_id,LinkId);
            break;

        case CON_ERROR_NETWORK_ATTACH_FAIL:
            cur = protocol_stack_interface_info_get_by_id(interface_id);
            if (!cur || !cur->thread_info) {
                break;
            }
            if (thread_bootstrap_attempt_attach_with_pending_set(cur) == 0) {
                thread_bootstrap_attach_start(interface_id, THREAD_NORMAL_ATTACH);
                break;
            }

            if (cur->thread_info->announcement_info && cur->thread_info->announcement_info->timer &&
                    cur->thread_info->announcement_info->announce_success) {
                // Attachment to announce failed we return to previous channel
                link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);

                if (linkConfiguration) {
                    linkConfiguration->rfChannel = cur->thread_info->announcement_info->channel;
                    linkConfiguration->channel_page = cur->thread_info->announcement_info->channel_page;
                    linkConfiguration->panId = cur->thread_info->announcement_info->panid;
                    thread_joiner_application_link_configuration_store(cur->id, linkConfiguration);
                } else {
                    tr_error("No link configuration!");
                }
                //set announce success flag to false because attach to new channel failed
                cur->thread_info->announcement_info->announce_success = false;
                thread_bootstrap_attach_start(interface_id, THREAD_NORMAL_ATTACH);

                break;
            }

            if (cur->thread_info->thread_device_mode  == THREAD_DEVICE_MODE_ROUTER) {
                if (!thread_router_bootstrap_routing_allowed(cur)) {
                    thread_discovery_responser_enable(cur->id, false);
                    thread_bootstrap_orphan_scan_start(cur);
                } else {
                    thread_bootstrap_generate_leader_and_link(cur);
                }
            } else {
                thread_bootstrap_orphan_scan_start(cur);
            }
            break;

        case CON_ERROR_NO_THREAD_NETWORK_AVAILABLE:
            thread_bootsrap_network_discovery_failure(interface_id);
            break;
        case CON_ERROR_PARTITION_MERGE:
            thread_bootstrap_attach_start(interface_id, THREAD_PARTITION_MERGE);
            break;
        case CON_ERROR_NETWORK_REATTACH:
            thread_bootstrap_attach_start(interface_id, THREAD_REATTACH);
            break;
        case CON_ERROR_NEIGHBOR_UNREACHABLE:

            break;
        case CON_ERROR_NETWORK_KICK:
            thread_bootstrap_reset_restart(interface_id);
            break;
    }
}

void thread_interface_up(protocol_interface_info_entry_t *cur)
{
    thread_interface_init(cur);
    thread_interface_bootsrap_mode_init(cur);
    cur->nwk_nd_re_scan_count = 0;
}

int thread_bootstrap_reset(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->thread_info || (cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
        return -1;
    }

    if (cur->thread_info->thread_endnode_parent) {
        ns_dyn_mem_free(cur->thread_info->thread_endnode_parent);
        cur->thread_info->thread_endnode_parent = NULL;
    }

    neighbor_cache_flush(&cur->neigh_cache);
    thread_bootstrap_stop(cur);
    mac_neighbor_table_neighbor_list_clean(mac_neighbor_info(cur));
    cur->bootsrap_state_machine_cnt = 0;
    mac_helper_free_scan_confirm(&cur->mac_parameters->nwk_scan_params);
    //tr_debug( "--> idle");
    cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
    rpl_control_remove_domain_from_interface(cur);
    protocol_core_interface_info_reset(cur);
    mac_data_poll_disable(cur);

    //Prepare start Again

    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE; //Set Active Bootsrap
    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY; //Clear Bind
    protocol_6lowpan_interface_common_init(cur);
    addr_interface_set_ll64(cur, NULL);
    thread_interface_up(cur);

    cur->nwk_mode = ARM_NWK_GP_IP_MODE;
    cur->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    cur->nwk_nd_re_scan_count = 0;
    if (cur->thread_info->thread_attached_state != THREAD_STATE_REATTACH_RETRY) {
        cur->thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
    }
    cur->ipv6_neighbour_cache.send_nud_probes = false; //Disable NUD probing
    cur->ipv6_neighbour_cache.probe_avoided_routers = false;
    cur->ip_multicast_as_mac_unicast_to_parent = true;
    //Define Default Contexts
    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        nwk_thread_host_control(cur, NET_HOST_RX_ON_IDLE, 0);
    }
    thread_anycast_address_policy_update(cur->thread_info, true);
    thread_bootstrap_state_machine(cur);
    return 0;
}

void thread_generate_ml64_address(protocol_interface_info_entry_t *cur)
{
    if_address_entry_t *def_address = NULL;
    uint8_t ula[16];
    memcpy(ula, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    //GENERATE ML-EID64
    memcpy(&ula[8], cur->iid_slaac, 8);
    def_address = addr_add(cur, ula, 64, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0xffffffff, true);
    if (def_address) {
        tr_debug("Generated UL64: %s", trace_ipv6(ula));
    }
}

void thread_delete_ml64_address(protocol_interface_info_entry_t *cur)
{
    uint8_t ula[16];
    memcpy(ula, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&ula[8], cur->iid_slaac, 8);
    addr_delete(cur, ula);

}

void thread_generate_ml16_address(protocol_interface_info_entry_t *cur)
{
    if_address_entry_t *def_address = NULL;
    uint8_t ula[16];
    uint16_t euid16 = mac_helper_mac16_address_get(cur);
    if (euid16 == 0xffff) { // Safe guard
        tr_debug("Do NOT generate ML16, mac16=%u", euid16);
        return;
    }
    //GENERATE ML-16
    memcpy(ula, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&ula[8], ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(euid16, &ula[14]);

    def_address = addr_add(cur, ula, 64, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0xffffffff, true);
    if (def_address) {
        tr_debug("Generated ML16: %s", trace_ipv6(ula));
        cur->global_address_available = true;
    }
}

void thread_delete_ml16_addresses(protocol_interface_info_entry_t *cur)
{
    uint8_t ula[16] = {0};
    memcpy(ula, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&ula[8], ADDR_SHORT_ADR_SUFFIC, 6);
    addr_delete_matching(cur, ula, 112, ADDR_SOURCE_UNKNOWN);
}

void thread_bootstrap_update_ml16_address(protocol_interface_info_entry_t *cur, uint16_t mac16)
{
    thread_delete_ml16_addresses(cur);
    mac_helper_mac16_address_set(cur, mac16);
    thread_generate_ml16_address(cur);
    thread_router_bootstrap_anycast_address_register(cur);
}
static void thread_meshlocal_route_set(protocol_interface_info_entry_t *cur)
{
    cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
    if (ipv6_route_add(cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 64, cur->id, NULL, ROUTE_THREAD, 0xffffffff, 0) == NULL) {
        tr_error("fail to add route");
    }
}

void thread_bootstrap_attached_ready(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_READY, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_attached_downgrade_router(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_DOWNGRADE_ROUTER, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_attched_upgrade_reed(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_UPGRADE_REED, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_attached_active_router(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_ACTIVE_ROUTER, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_router_id_release_ready(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_ROUTER_ID_RELEASED, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_router_id_get_fail(protocol_interface_info_entry_t *cur)
{
    thread_bootsrap_event_trig(THREAD_ATTACH_ROUTER_ID_GET_FAIL, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

int8_t thread_bootstrap_child_id_request(protocol_interface_info_entry_t *cur)
{
    return thread_bootsrap_event_trig(THREAD_CHILD_ID_REQUEST, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
}

void thread_bootstrap_routing_activate(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE ||
            cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        thread_meshlocal_route_set(cur);
        // FEDs and routers (REEDs) perform their own address resolution
        thread_nd_service_activate(cur->id);
    } else {
        thread_nd_client_service_activate(cur->id);
        thread_child_set_default_route(cur);
    }
}

void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur)
{
    cur->nwk_bootstrap_state = ER_MLE_ATTACH_READY;
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY;
    cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
    cur->bootsrap_state_machine_cnt = 10;
    cur->thread_info->routerIdRequested = false;
    cur->thread_info->networkDataRequested = false;
    clear_power_state(ICMP_ACTIVE);

    //Free scanned Result
    if (cur->thread_info->thread_attach_scanned_parent) {
        mle_service_msg_free(cur->thread_info->thread_attach_scanned_parent->child_id_request_id);
        ns_dyn_mem_free(cur->thread_info->thread_attach_scanned_parent);
        cur->thread_info->thread_attach_scanned_parent = NULL;
    }

    // Make local/nwk data check after 5s
    thread_border_router_resubmit_timer_set(cur->id, 5);
    //Generate UL16
    thread_generate_ml16_address(cur);
    //GENERATE ML-EID64
    thread_generate_ml64_address(cur);
    thread_bootstrap_routing_activate(cur);
    thread_bootstrap_network_data_update(cur);
    // After successful attach if there is announcement info present, send announcement back to previous channel
    if (cur->thread_info->announcement_info && cur->thread_info->announcement_info->announce_success == false) {
        cur->thread_info->announcement_info->timer = eventOS_timeout_ms(thread_announce_success_cb, 20000, cur);
    }
    thread_configuration_mle_activate(cur);

    if (cur->thread_info->releaseRouterId) {
        thread_router_bootstrap_router_id_release(cur);
    }
    uint8_t *parent_mac_addr = NULL;
    if (cur->thread_info->thread_endnode_parent) {
        parent_mac_addr = cur->thread_info->thread_endnode_parent->mac64;
    }
    thread_nvm_store_link_info_write(parent_mac_addr, mac_helper_mac16_address_get(cur));
    thread_bootstrap_ready(cur);

    if (thread_is_router_addr(mac_helper_mac16_address_get(cur))) {
        // Attached as router Trigger routter attach
        tr_info("Attaching directly to router");
        thread_bootstrap_attached_active_router(cur);
    }
}

//This function is for Thread Parent scan callback
bool thread_network_data_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur;
    (void)msgId;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return false;
    }

    /* If network data is not received, send again */
    if (thread_info(cur)->networkDataRequested && !usedAllRetries) {
        return true;
    }

    if (cur->thread_info->leader_synced) {
        if (usedAllRetries) {
            // could not learn network data from neighbour, everyone must reregister
            cur->thread_info->leader_synced = false;
            thread_leader_service_network_data_changed(cur, true, true);
            return false;
        } else {
            tr_debug("retrying as leader data not yet synced");
            return true;
        }
    }

    // if REED fails to get updated network data, it reattaches
    if (thread_info(cur)->networkDataRequested && !thread_attach_active_router(cur) && usedAllRetries) {
        thread_bootstrap_reset_restart(interface_id);
    }

    thread_info(cur)->networkDataRequested = false;
    mac_data_poll_protocol_poll_mode_decrement(cur);
    return false;
}


bool thread_tlv_request(int8_t interface_id, uint8_t *address, bool delayed_message, uint8_t *req_tlv, uint8_t req_len)
{
    mle_message_timeout_params_t timeout;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    uint16_t buf_id;

    buf_id = mle_service_msg_allocate(interface_id, 32 + 20 + thread_leader_data_tlv_size(cur), false, MLE_COMMAND_DATA_REQUEST);

    if (!cur || buf_id == 0) {
        return false;
    }

    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    ptr = mle_tlv_req_tlv(ptr, req_tlv, req_len);

    //SET Leader Data
    ptr = thread_leader_data_tlv_write(ptr, cur);

    ptr = thread_active_timestamp_write(cur, ptr);

    ptr = thread_pending_timestamp_write(cur, ptr);

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = THREAD_REQUEST_MAX_RETRY_CNT;
    timeout.timeout_init = 1;
    timeout.timeout_max = 3;
    if (delayed_message) {
        timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
    } else {
        timeout.delay = MLE_NO_DELAY;
    }
    mac_data_poll_init_protocol_poll(cur);
    mle_service_set_packet_callback(buf_id, thread_network_data_timeout);
    mle_service_set_msg_destination_address(buf_id, address);
    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    //Set Security
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_send_message(buf_id);
    return true;
}

void thread_bootstrap_parent_network_data_request(protocol_interface_info_entry_t *cur, bool delay_request)
{
    uint8_t dst_address[16];
    memcpy(dst_address, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&dst_address[8], thread_info(cur)->thread_endnode_parent->mac64, 8);
    dst_address[8] ^= 2;
    thread_network_data_request_send(cur, dst_address, delay_request);
}

void thread_parent_scan(protocol_interface_info_entry_t *cur)
{
    cur->thread_info->thread_attached_state = THREAD_STATE_NETWORK_DISCOVER;
    cur->nwk_nd_re_scan_count = 0;
    cur->nwk_bootstrap_state = ER_SCAN;
    cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(5, 15);
    nwk_thread_host_control(cur, NET_HOST_RX_ON_IDLE, 0);
}

void thread_bootstrap_joiner_application_commission_done_cb(int8_t interface_id)
{
    protocol_interface_info_entry_t *interface;
    interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return;
    }
    tr_debug("Commission Ready trig bootsrap to starting from init again");
    interface->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    interface->bootsrap_state_machine_cnt = randLIB_get_random_in_range(1, 4);
}

static int compare_steering_and_joiner_bloom(uint8_t *steering_bloom, uint8_t *joiner_bloom, uint8_t steering_tlv_length)
{
    //make sure s bit is not checked
    int loop_iterator;
    tr_debug("joiner bloom : %s", trace_array(joiner_bloom, steering_tlv_length));
    tr_debug("steering bloom : %s", trace_array(steering_bloom, steering_tlv_length));
    for (loop_iterator = 0; loop_iterator < steering_tlv_length; loop_iterator++) {
        if ((joiner_bloom[loop_iterator] != (joiner_bloom[loop_iterator] & steering_bloom[loop_iterator]))) {
            thci_trace("joinerDiscoveryFailedFiltered");
            return 0;
        }
    }
    return 1;
}

static bool thread_route_possible_add(thread_attach_device_mode_e threadMode)
{
    bool addRoute;
    if (threadMode == THREAD_DEVICE_MODE_ROUTER ||
            threadMode == THREAD_DEVICE_MODE_FULL_END_DEVICE) {
        addRoute = true;
    } else {
        addRoute = false;
    }

    return addRoute;
}

static void thread_dhcp_client_gua_error_cb(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status)
{
    if (register_status) {
        tr_debug("Get Address %s From %s", trace_ipv6(prefix), trace_ipv6(dhcp_addr));
    } else {
        tr_warn("Address Get fail: %s from: %s", trace_ipv6(prefix), trace_ipv6(dhcp_addr));
        if (prefix && dhcp_addr) {
            tr_debug("Delete Current Server data");
            dhcp_client_global_address_delete(interface, dhcp_addr, prefix);
        }
    }
}

bool thread_dhcpv6_address_entry_available(uint8_t *prefixPtr, if_address_list_t *list)
{
    bool addressReady = false;
    ns_list_foreach(if_address_entry_t, entry, list) {
        if (memcmp(entry->address, prefixPtr, 8) == 0) {
            addressReady = true;
            break;
        }
    }
    return addressReady;
}

static int thread_bloom_and_compare(uint8_t *steering_data_ptr, uint8_t steering_data_length, uint8_t *eui64, uint8_t eui64_length)
{
    if (steering_data_length == 0 || steering_data_length > 16) {
        return 0;
    }

    uint8_t joiner_bloom_calculated[16] = {0};
    uint8_t mac_extended_address[8];

    ns_sha256_nbits(eui64, eui64_length, mac_extended_address, 64);
    mac_extended_address[0] |= 2; //local administered bit is set

    thread_beacon_calculate_bloom_filter(joiner_bloom_calculated, steering_data_length, mac_extended_address, 8);

    return compare_steering_and_joiner_bloom(steering_data_ptr, joiner_bloom_calculated, steering_data_length);
}

static void thread_network_select_by_steering_data(device_configuration_s *device_configuration_ptr, thread_nwk_discovery_response_list_t *discover_response)
{
    ns_list_foreach_safe(discovery_response_list_t, cur_class, discover_response) {

        if (!thread_bloom_and_compare(cur_class->steering_data, cur_class->steering_data_valid, device_configuration_ptr->eui64, 8)) {
            ns_list_remove(discover_response, cur_class);
            ns_dyn_mem_free(cur_class);
        }

    }
}

static void thread_network_select(struct protocol_interface_info_entry *interface_ptr, device_configuration_s *device_configuration_ptr, thread_nwk_discovery_response_list_t *discover_response)
{
    (void) interface_ptr;

    discovery_response_list_t *discovered_network_ptr = thread_ccm_network_select(interface_ptr, discover_response);

    /* If network found */
    if (discovered_network_ptr) {
        /* Clear other networks from list */
        ns_list_foreach_safe(discovery_response_list_t, cur_class, discover_response) {
            if (cur_class != discovered_network_ptr) {
                ns_list_remove(discover_response, cur_class);
                ns_dyn_mem_free(cur_class);
            }
        }
    } else {
        thread_network_select_by_steering_data(device_configuration_ptr, discover_response);
    }
}

void thread_bootsrap_discovery_ready_cb(struct protocol_interface_info_entry *cur_interface, thread_nwk_discovery_response_list_t *discover_response)
{
    device_configuration_s *device_configuration_ptr = thread_joiner_application_get_device_config(cur_interface->id);
    if (!device_configuration_ptr) {
        tr_error("Mac scan confirm:Unknow Interface");
        thci_trace("joinerDiscoveryFailedNoBeacon");
        return;
    }

    thread_network_select(cur_interface, device_configuration_ptr, discover_response);

    if (!ns_list_count(discover_response)) {
        tr_debug("NO network available for scan");
        thci_trace("joinerDiscoveryFailedNoBeacon");
        goto exit_failure;
    }

    cur_interface->nwk_bootstrap_state = ER_ACTIVE_SCAN;
    cur_interface->bootsrap_state_machine_cnt = 1;
    return;



exit_failure:

    thread_bootstrap_connection_error(cur_interface->id, CON_ERROR_NO_THREAD_NETWORK_AVAILABLE, NULL);
}

static void thread_bootstrap_create_unsecure_link_to_parent(protocol_interface_info_entry_t *interface, discovery_response_list_t *nwk_info)
{
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));

    mac_helper_coordinator_address_set(interface, ADDR_802_15_4_LONG, nwk_info->extented_mac);

    interface->mac_parameters->pan_id = nwk_info->pan_id;
    interface->mac_parameters->mac_channel = nwk_info->channel;

    start_req.PANId = nwk_info->pan_id;
    start_req.LogicalChannel = nwk_info->channel;
    start_req.ChannelPage = 0;
    start_req.BeaconOrder = 0x0f;
    start_req.SuperframeOrder = 0x0f;
    //SET Beacon Payload
    mac_helper_beacon_payload_reallocate(interface, 0);
    interface->mac_api->mlme_req(interface->mac_api, MLME_START, (void *)&start_req);
    mac_data_poll_init(interface);
    mac_helper_mac16_address_set(interface, 0xffff);
    tr_debug("Mac Ready");
}

void thread_discover_native_commissioner_response(protocol_interface_info_entry_t *interface, thread_nwk_discovery_response_list_t *nwk_info)
{
    thread_commissioning_link_configuration_s *config_ptr;
    int n = 0;
    if (ns_list_count(nwk_info) == 0) {
        tr_debug("Thread discover:No nwk");
        goto exit_failure;
    }

    // create list of available networks for native commissioner interface
    config_ptr = ns_dyn_mem_alloc(sizeof(thread_commissioning_link_configuration_s) * ns_list_count(nwk_info));
    if (!config_ptr) {
        tr_debug("Mac scan confirm:out of resources");
        goto exit_failure;
    }

    ns_list_foreach(discovery_response_list_t, cur_class, nwk_info) {
        config_ptr[n].panId = cur_class->pan_id;
        config_ptr[n].Protocol_id = THREAD_PROTOCOL_ID;
        config_ptr[n].version = THREAD_PROTOCOL_VERSION;
        config_ptr[n].rfChannel = cur_class->channel;
        memcpy(config_ptr[n].name, cur_class->network_name, 16);
        memcpy(config_ptr[n].extented_pan_id, cur_class->extented_pan_id, 8);
        memcpy(config_ptr[n].destination_address, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(&config_ptr[n].destination_address[8], cur_class->extented_mac, 8);
        config_ptr[n].destination_address[8] ^= 2;
        config_ptr[n].destination_port = cur_class->commissioner_port;
        n++;
    }

    if (interface->thread_info->native_commissioner_cb) {
        interface->thread_info->native_commissioner_cb(interface->id, n, config_ptr);
    }
    ns_dyn_mem_free(config_ptr);

    if (!interface->thread_info->native_commissioner_link) {
        tr_debug("Mac scan confirm:continue scanning");
        goto exit_failure;
    }
    //Free if not matching network found
    ns_list_foreach_safe(discovery_response_list_t, cur_class, nwk_info) {
        if (cur_class->version != THREAD_PROTOCOL_VERSION ||
                cur_class->pan_id != interface->thread_info->native_commissioner_link->panId ||
                memcmp(interface->thread_info->native_commissioner_link->name, cur_class->network_name, 16) != 0  ||
                memcmp(interface->thread_info->native_commissioner_link->extented_pan_id, cur_class->extented_pan_id, 8) != 0) {

            ns_list_remove(nwk_info, cur_class);
            ns_dyn_mem_free(cur_class);

        }
    }

    if (ns_list_is_empty(nwk_info)) {
        tr_debug("Mac scan confirm:no networks available");
        goto exit_failure;
    }
    //select best parent link
    discovery_response_list_t *best = NULL;

    ns_list_foreach_safe(discovery_response_list_t, cur_class, nwk_info) {
        ns_list_remove(nwk_info, cur_class);
        if (!best || (best && best->dbm < cur_class->dbm)) {
            if (best) {
                ns_dyn_mem_free(cur_class);
            }
            best = cur_class;
        } else {
            ns_dyn_mem_free(cur_class);
        }
    }

    // best is always not null, because the not empty check above
    /* coverity[FORWARD_NULL] */
    interface->thread_info->native_commissioner_port = best->commissioner_port;
    thread_bootstrap_create_unsecure_link_to_parent(interface, best);
    ns_dyn_mem_free(best);

    /**
     *  we should stop scanning and saying that the interface is up
     */
    tr_debug("Native commissioning interface ready");
    interface->thread_info->thread_attached_state = THREAD_STATE_CONNECTED;
    interface->nwk_bootstrap_state = ER_MLE_ATTACH_READY;
    interface->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY;
    interface->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;

    interface->thread_info->routerIdRequested = false;
    interface->thread_info->networkDataRequested = false;

    interface->bootsrap_state_machine_cnt = 10;

    clear_power_state(ICMP_ACTIVE);
    thread_bootstrap_ready(interface);
    return;



exit_failure:

    thread_bootstrap_connection_error(interface->id, CON_ERROR_NO_THREAD_NETWORK_AVAILABLE, NULL);
}



static void thread_bootsrap_network_join_start(struct protocol_interface_info_entry *cur_interface, discovery_response_list_t *nwk_info)
{
    device_configuration_s *device_configuration_ptr = thread_joiner_application_get_device_config(cur_interface->id);

    uint8_t parent_ll_addr[16];
    memcpy(parent_ll_addr, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(parent_ll_addr + 8, nwk_info->extented_mac, 8);
    parent_ll_addr[8] ^= 2;

    if (!device_configuration_ptr || blacklist_reject(parent_ll_addr)) {
        ns_dyn_mem_free(nwk_info);
        thci_trace("joinerDiscoveryFailedNoBeacon");
        thread_bootstrap_connection_error(cur_interface->id, CON_ERROR_NO_THREAD_NETWORK_AVAILABLE, NULL);
        return;
    }

    uint8_t private_mac[8];
    ns_sha256_nbits(device_configuration_ptr->eui64, 8, private_mac, 64);
    private_mac[0] |= 2; //local administered bit is set

    tr_debug("joiner mac id : %s", trace_array(private_mac, 8));
    mac_helper_mac64_set(cur_interface, private_mac);

    thread_set_link_local_address(cur_interface); // only to generate IID

    thread_bootstrap_create_unsecure_link_to_parent(cur_interface, nwk_info);

    thci_trace("joinerDiscoverySuccess");
    uint8_t parentLLAddress[16];
    protocol_6lowpan_interface_get_link_local_cordinator_address(cur_interface, parentLLAddress);
    tr_debug("Start commission with %s", trace_ipv6(parentLLAddress));
    cur_interface->bootsrap_state_machine_cnt = 0;

    if (0 > thread_ccm_commission_start(cur_interface, parentLLAddress, nwk_info, thread_bootstrap_joiner_application_commission_done_cb)) {
        thread_joiner_application_pskd_commission_start(cur_interface->id, parentLLAddress, nwk_info->joiner_port, nwk_info->pan_id, nwk_info->extented_pan_id, nwk_info->channel, thread_bootstrap_joiner_application_commission_done_cb);
    }
    ns_dyn_mem_free(nwk_info);

}

void thread_bootsrap_device_synch_fail(protocol_interface_info_entry_t *cur)
{
    tr_debug("Link Synch Fail -->Parent Scan Start");
    thread_bootstrap_clear_neighbor_entries(cur);
    thread_network_synch_data_free(cur->id);
    thread_delete_ml64_address(cur);
    thread_parent_scan(cur);
}

bool thread_device_synch_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    (void)msgId;
    if (!interface) {
        return false;
    }

    if (usedAllRetries) {
        thread_bootsrap_device_synch_fail(interface);
        return false;
    }

    return true;
}

bool thread_link_request_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);

    if (!interface) {
        return false;
    }

    if (usedAllRetries) {
        uint8_t *addr = mle_service_get_msg_destination_address_pointer(msgId);
        tr_debug("Link Request timed out, address: %s", trace_ipv6(addr));
        blacklist_update(addr, false);
        return false;
    }

    return true;
}
static bool thread_bootstrap_sync_after_reset_start(protocol_interface_info_entry_t *cur)
{
    uint16_t my_short_address;
    uint8_t parent_mac64[8];

    // link sync is allowed only once in bootstrap start and we might get here in other cases also
    if (!cur->thread_info->link_sync_allowed) {
        return false;
    }
    cur->thread_info->link_sync_allowed = false;

    int link_info_err = thread_nvm_store_link_info_get(parent_mac64, &my_short_address);
    if (link_info_err != THREAD_NVM_FILE_SUCCESS) {
        tr_warning("thread_nvm_store_link_info_get returned %d", link_info_err);
        return false;
    }
    link_info_err = thread_nvm_store_link_info_clear();
    if (link_info_err != THREAD_NVM_FILE_SUCCESS) {
        tr_warning("thread_nvm_store_link_info_clear returned %d", link_info_err);
    }
    if (thread_is_router_addr(my_short_address)) {
        thread_info(cur)->routerShortAddress = my_short_address;
        thread_dynamic_storage_build_mle_table(cur->id);
        thread_router_bootstrap_link_synch_start(cur);
        return true;
    }
    if (!thread_parent_data_allocate(cur->thread_info)) {
        tr_info("parent alloc failed");
        return false;
    }

    cur->thread_info->thread_endnode_parent->shortAddress = 0xfffe;
    memcpy(cur->thread_info->thread_endnode_parent->mac64, parent_mac64, 8);
    thread_endevice_synch_start(cur);
    return true;
}
void thread_bootstrap_start_network_discovery(protocol_interface_info_entry_t *cur)
{
    thread_discover_reques_t scan_request;
    thread_discovery_ready_cb *discover_ready;
    cur->nwk_nd_re_scan_count++;
    cur->mac_parameters->nwk_scan_params.stack_chan_list = cur->mac_parameters->mac_channel_list;


    if (cur->thread_info->native_commissioner_cb) {
        tr_debug("native commissioner network scan start");

        discover_ready = thread_discover_native_commissioner_response;
        scan_request.joiner_flag = false;
        scan_request.native_commisioner = true;
    } else {
        discovery_response_list_t *thread_network = thread_discovery_network_description_get(cur->id);
        if (thread_network) {
            thread_bootsrap_network_join_start(cur, thread_network);
            return;
        }

        tr_debug("scan networks for joining application");
        thci_trace("joinerDiscoveryStarted");

        blacklist_params_set(
            THREAD_COMM_BLACKLIST_ENTRY_LIFETIME,
            THREAD_COMM_BLACKLIST_TIMER_MAX_TIMEOUT,
            THREAD_COMM_BLACKLIST_TIMER_TIMEOUT,
            THREAD_BLACKLIST_ENTRY_MAX_NBR,
            THREAD_BLACKLIST_PURGE_NBR,
            THREAD_BLACKLIST_PURGE_TIMER_TIMEOUT);

        scan_request.native_commisioner = false;
        scan_request.joiner_flag = true;

        discover_ready = thread_bootsrap_discovery_ready_cb;
    }

    scan_request.channel_mask = cur->mac_parameters->nwk_scan_params.stack_chan_list.channel_mask[0];
    scan_request.filter_tlv_data = NULL;
    scan_request.filter_tlv_length = 0;
    if (thread_discovery_network_scan(cur, &scan_request, discover_ready) != 0) {
        tr_error("Discovery scan start fail");
    }
}
void thread_bootstrap_state_machine(protocol_interface_info_entry_t *cur)
{
    link_configuration_s *linkConfiguration;

    switch (cur->nwk_bootstrap_state) {
        case ER_ACTIVE_SCAN:
            tr_debug("Thread SM:Active Scan");

            thread_joiner_application_nvm_link_configuration_load(cur->id);
            linkConfiguration = thread_joiner_application_get_config(cur->id);
            if (!linkConfiguration) {
                thread_bootstrap_start_network_discovery(cur);
                return;
            }

            //SET Link by Static configuration
            tr_info("thread network attach start");
            if (thread_mle_service_register(cur, thread_joiner_application_random_mac_get(cur->id)) != 0 ||
                    thread_link_configuration_activate(cur, linkConfiguration) != 0) {
                tr_error("Network Bootsrap Start Fail");
                bootsrap_next_state_kick(ER_BOOTSTRAP_SCAN_FAIL, cur);
                return;
            }
            if (thread_bootstrap_sync_after_reset_start(cur)) {
                // Link syncronisation started
                tr_debug("link synchronisation start");
                return;
            }
            tr_debug("Parent Scan Start");
            thread_parent_scan(cur);
            break;

        case ER_SCAN:
            tr_debug("Thread SM:Start ER Scan");
            cur->nwk_nd_re_scan_count = 0;
            thread_network_attach_start(cur);
            break;

        case ER_MLE_ATTACH_READY:
        case ER_MLE_SYNCH:
        case ER_MLE_SCAN:
        case ER_CHILD_ID_REQ:
            tr_debug("Thread SM:Attach Ready");
            break;

        case ER_BOOTSRAP_DONE:
            tr_debug("Thread SM:Bootstrap Done");
            cur->nwk_nd_re_scan_count = 0;
            break;
        case ER_BOOTSTRAP_SCAN_FAIL:
            tr_debug("Thread SM:Scan Failed");
            nwk_bootsrap_state_update(ARM_NWK_NWK_SCAN_FAIL, cur);
            break;

        case ER_BOOTSTRAP_LEADER_UP:
            tr_debug("Thread SM:Leader Start");
            thread_leader_service_thread_partitition_generate(cur->id, false);
            break;

        case ER_BOOTSTRAP_NEW_FRAGMENT_START:
            tr_debug("Thread SM:Create New fragment");
            thread_leader_service_thread_partitition_generate(cur->id, true);
            break;

        default:
            tr_warn("Thread SM:Invalid state");

    }
}
void thread_bootstrap_stop(protocol_interface_info_entry_t *cur)
{
    thread_bootstrap_address_registration_deinit();
    thread_anycast_address_policy_update(cur->thread_info, false);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD, NULL);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_BORDER_ROUTER, NULL);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_PROXIED_HOST, NULL);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_PROXIED_DUA_HOST, NULL);
    thread_leader_service_leader_data_free(cur->thread_info);
    thread_bootstrap_all_nodes_multicast_unregister(cur);
    thread_data_base_init(cur->thread_info, cur->id);
    dhcp_client_delete(cur->id);
    thread_nd_service_delete(cur->id);
    thread_child_id_request_entry_clean(cur);
    thread_registered_mcast_addr_entry_clean(cur);
    cur->mesh_callbacks = NULL;
}

void thread_bootstrap_child_update_trig(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED && cur->thread_info->thread_endnode_parent) {
        thread_bootsrap_event_trig(THREAD_CHILD_UPDATE, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
    }
}
static void thread_border_router_locator_copy(protocol_interface_info_entry_t *cur, thread_commissioner_t *registered_commissioner, uint8_t *data)
{
    memcpy(registered_commissioner->border_router_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&registered_commissioner->border_router_address[8], ADDR_SHORT_ADR_SUFFIC, 6);
    memcpy(&registered_commissioner->border_router_address[14], data, 2);
}
/* \return -1 Failed
 *          0 OK, data not changed
 *          1 OK, data changed */
static int thread_commission_data_tlv_parse(protocol_interface_info_entry_t *cur, uint8_t type, uint8_t length, uint8_t *data)
{
    thread_commissioner_t *registered_commissioner = &cur->thread_info->registered_commissioner;

    switch (type) {
        // This is set by leader
        case THREAD_TLV_COMMISSIONER_SESSION_ID:
            if (length != 2) {
                return -1;
            }
            if (registered_commissioner->session_id != common_read_16_bit(data)) {
                registered_commissioner->session_id = common_read_16_bit(data);
                return 1;
            }
            break;

        case THREAD_TLV_STEERING_DATA:
            if (length > 16) {
                return -1;
            }
            if (registered_commissioner->steering_data_len != length || memcmp(registered_commissioner->steering_data, data, length)) {
                memcpy(registered_commissioner->steering_data, data, length);
                registered_commissioner->steering_data_len = length;
                return 1;
            }
            break;

        case THREAD_TLV_BORDER_ROUTER_LOCATOR:
            if (length != 2) {
                return -1;
            }

            registered_commissioner->commissioner_valid = true;
            if (!registered_commissioner->commissioner_valid ||
                    memcmp(&registered_commissioner->border_router_address[14], data, 2) ||
                    memcmp(registered_commissioner->border_router_address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8)) {
                thread_border_router_locator_copy(cur, registered_commissioner, data);
                return 1;
            }
            break;

        default:
            break;

    }
    return 0;
}
static int thread_nd_prefix_context_allocate(protocol_interface_info_entry_t *cur, thread_prefix_tlv_t *prefixTlv, bool stableData)
{
    thread_network_local_data_context_entry_t context;
    uint8_t cid;
    //check possible context id or current allocated
    cid = thread_nd_context_id_allocate(&cur->thread_info->networkDataStorage, &cur->thread_info->localServerDataBase, prefixTlv->Prefix, prefixTlv->PrefixLen);
    if (cid == 16) {
        return -1;
    }

    context.cid = cid;
    context.compression = true;
    context.stableData = stableData;
    context.contextPrefixLength = prefixTlv->PrefixLen;
    if (lowpan_context_update(&cur->lowpan_contexts, cid, 0xffff, prefixTlv->Prefix, prefixTlv->PrefixLen, stableData) != 0) {
        return -1;
    }

    if (thread_nd_local_list_add_contexts(&cur->thread_info->networkDataStorage, prefixTlv, &context) != 0) {
        //Free context if allocation fail
        lowpan_context_update(&cur->lowpan_contexts, cid, 0, prefixTlv->Prefix,  prefixTlv->PrefixLen, stableData);
        return -1;
    }

    return 0;
}

int thread_bootstrap_network_data_process(protocol_interface_info_entry_t *cur, uint8_t *network_data_ptr, uint16_t network_data_length)
{
    int ret_val = 0;
    uint8_t addr[16];
    uint8_t *dptr;
    uint16_t length, data_length, flags;
    uint8_t prefix_bytes_len, subLength, preference;
    uint8_t type;
    bool stableData;
    bool update_data = false;
    thread_prefix_tlv_t prefixTlv;
    thread_network_local_data_context_entry_t context;
    thread_border_router_tlv_entry_t genericService;
    thread_network_data_cache_entry_t *networkDataStorage = &cur->thread_info->networkDataStorage;

    link_configuration_s *linkConfiguration  = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    data_length = network_data_length;
    dptr = network_data_ptr;
    tr_debug("Start Parsing TLV one by One");
    while (data_length) {
        tr_debug("Main TLV: %s", trace_array(dptr, data_length));

        prefixTlv.Prefix = NULL;
        prefixTlv.PrefixLen = 0;

        type = *dptr++;
        if (type & THREAD_NWK_STABLE_DATA) {
            stableData = true;
        } else {
            stableData = false;
        }

        type &= THREAD_NWK_DATA_TYPE_MASK;
        length = *dptr++;
        if ((length + 2) > data_length) {
            return -1;
        }

        //SET Pointer & Length ready for next check
        data_length -= (length + 2);
        if (!stableData && !cur->thread_info->requestFullNetworkData) {
            //Skip unstable data when it not requsted
            dptr += length;
        } else if (type == THREAD_NWK_DATA_TYPE_PREFIX) {
            prefixTlv.domainId = *dptr++;
            prefixTlv.PrefixLen = *dptr++;
            //Decrement length by prefix length
            length -= 2;
            prefixTlv.Prefix = dptr;
            prefix_bytes_len = prefixBits_to_bytes(prefixTlv.PrefixLen);

            if (prefix_bytes_len > length) {
                return -1;
            }
            length -= prefix_bytes_len;
            dptr += prefix_bytes_len;
            tr_debug("Prefix: %s", trace_ipv6_prefix(prefixTlv.Prefix, prefixTlv.PrefixLen));

            if (thread_network_data_sub_tlv_malformed_check(dptr, length) == 0) {

                while (length > 2) {
                    type = *dptr++;
                    subLength = *dptr++;
                    length -= 2;
                    if (type & THREAD_NWK_STABLE_DATA) {
                        stableData = true;
                    } else {
                        stableData = false;
                    }
                    type &= THREAD_NWK_DATA_TYPE_MASK;

                    tr_debug("SubType: %02x, %s", type, trace_array(dptr, subLength));

                    if (subLength <=  length) {
                        length -= subLength;
                        if (type == THREAD_NWK_DATA_TYPE_6LOWPAN_ID) {
                            while (subLength) {
                                if (*dptr & THREAD_NWK_CONTEXT_COMPRESS_ENABLED) {
                                    context.compression = true;
                                } else {
                                    context.compression = false;
                                }

                                context.cid = (*dptr++ & 0x0f);
                                context.contextPrefixLength = *dptr++;
                                context.stableData = stableData;
                                subLength -= 2;

                                if (thread_nd_verify_contex_id_is_free(networkDataStorage, prefixTlv.Prefix, &context) == 0) {
                                    thread_nd_local_list_add_contexts(networkDataStorage, &prefixTlv, &context);

                                } else {
                                    tr_debug("CID reserved already");
                                }
                            }
                        } else {

                            while (subLength) {
                                genericService.routerID = common_read_16_bit(dptr);
                                dptr += 2;
                                if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                                    flags = common_read_16_bit(dptr);
                                    dptr += 2;
                                    subLength -= THREAD_BORDER_ROUTER_TLV_LENGTH;
                                    genericService.Prf = ((flags >> THREAD_PRF_BIT_MOVE) & 3);
                                } else {
                                    /* HAS_ROUTE */
                                    preference = *dptr++;
                                    subLength -= THREAD_HAS_ROUTE_TLV_LENGTH;
                                    genericService.Prf = ((preference >> THREAD_HAS_ROUTE_PRF_BIT_MOVE) & 3);
                                }

                                genericService.stableData = stableData;

                                if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                                    genericService.P_configure = ((flags >> THREAD_P_CONFIGURE_BIT_MOVE) & 1);
                                    genericService.P_default_route = ((flags >> THREAD_P_DEF_ROUTE_BIT_MOVE) & 1);
                                    genericService.P_dhcp = ((flags >> THREAD_P_DHCP_BIT_MOVE) & 1);
                                    genericService.P_preferred = ((flags >> THREAD_P_PREFERRED_BIT_MOVE) & 1);
                                    genericService.P_slaac = ((flags >> THREAD_P_SLAAC_BIT_MOVE) & 1);
                                    genericService.P_on_mesh = ((flags >> THREAD_P_ON_MESH_BIT_MOVE) & 1);
                                    genericService.P_nd_dns = ((flags >> THREAD_P_ND_DNS_BIT_MOVE) & 1);
                                    genericService.P_res1 = ((flags >> THREAD_P_ND_RES_BIT_MOVE) & 1);
                                    if (thread_nd_local_list_add_on_mesh_prefix(networkDataStorage, &prefixTlv, &genericService) == 0) {
                                        if (networkDataStorage->stableUpdatePushed || networkDataStorage->temporaryUpdatePushed) {
                                            if (!genericService.P_slaac) {
                                                //Delete Address by this Entry
                                                tr_debug("Delete SLAAC address because not valid.");
                                                addr_delete_matching(cur, prefixTlv.Prefix, 64, ADDR_SOURCE_SLAAC);
                                            } else {
                                                ns_list_foreach_safe(if_address_entry_t, e, &cur->ip_addresses) {
                                                    if (e->source == ADDR_SOURCE_SLAAC &&
                                                            bitsequal(e->address, prefixTlv.Prefix, 64)) {
                                                        if (genericService.P_preferred) {
                                                            tr_debug("SLAAC address set as preferred.");
                                                        } else {
                                                            tr_debug("SLAAC address set as NOT preferred.");
                                                        }
                                                        addr_set_preferred_lifetime(cur, e, genericService.P_preferred ? 0xffffffff : 0);
                                                    }
                                                }
                                            }

                                            if (!genericService.P_dhcp) {
                                                //Delete DHCPv6 client
                                                memcpy(addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
                                                memcpy(&addr[8], ADDR_SHORT_ADR_SUFFIC, 6);
                                                common_write_16_bit(genericService.routerID, &addr[14]);
                                                tr_debug("Delete DHCPv6 given address");
                                                dhcp_client_global_address_delete(cur->id, addr, prefixTlv.Prefix);
                                            }
                                        }


                                        //Allocate Context
                                        if (cur->thread_info->leader_private_data) {
                                            if (thread_nd_prefix_context_allocate(cur, &prefixTlv, stableData) != 0) {
                                                //Mark Delete for this setup if context allocate fail
                                                thread_nd_local_list_del_on_mesh_server(networkDataStorage, &prefixTlv, &genericService);
                                            }
                                        }
                                    }
                                } else if (type == THREAD_NWK_DATA_TYPE_ROUTE) {
                                    if (genericService.routerID < 0xfffe) {
                                        thread_nd_local_list_add_route(networkDataStorage, &prefixTlv, &genericService);
                                        tr_debug("added route");
                                    }
                                }
                            }
                        }
                    } else {
                        tr_debug("SERVER Parse fail");
                        break;
                    }
                }
            } else {
                tr_debug("Malform Prefix sub TLV");
                dptr += length;
            }

        } else if (type == THREAD_NWK_DATA_TYPE_COMMISSION_DATA && stableData) {
            if (thread_network_data_sub_tlv_malformed_check(dptr, length) == 0) {
                tr_debug("Stable Commisssion TLV: %i", length);
                // Stable commissioning data has the Timestamps and is not processed here
            } else {
                tr_debug("Malformed stable Commisssion TLV: %i", length);
            }
            dptr += length;

        } else if (type == THREAD_NWK_DATA_TYPE_COMMISSION_DATA && !stableData) {
            if (thread_network_data_sub_tlv_malformed_check(dptr, length) == 0) {
                int data_changed;
                tr_debug("Unstable Commisssion TLV: %i", length);
                // in thread 1.1 unstable has only commissioner information
                cur->thread_info->registered_commissioner.commissioner_valid = false;
                while (length > 2) {
                    type = *dptr++;
                    subLength = *dptr++;
                    length -= 2;
                    tr_debug("SubType: %02x, %s", type, trace_array(dptr, subLength));

                    if (subLength <= length) {
                        length -= subLength;

                        data_changed = thread_commission_data_tlv_parse(cur, type, subLength, dptr);
                        if (data_changed < 0) {
                            tr_debug("Fail");
                        } else {
                            if (data_changed == 1) {
                                update_data = true;
                                tr_debug("Changed");
                            } else {
                                tr_debug("OK");
                            }
                        }
                        dptr += subLength;
                    } else {
                        tr_debug("SERVER Parse fail");
                        break;
                    }
                }
            } else {
                tr_debug("Malformed unstable Commisssion TLV: %i", length);
                dptr += length;
            }
        } else if (type == THREAD_NWK_DATA_TYPE_SERVICE_DATA) {
            thread_network_data_service_entry_t service_entry;

            service_entry.T = (*dptr) >> 7;
            service_entry.S_id = (*dptr++) & 0x0f;
            service_entry.S_stable = stableData;

            if (!service_entry.T) {
                service_entry.S_enterprise_number = common_read_32_bit(dptr);
                dptr += 4;
                length -= 4;
            } else {
                service_entry.S_enterprise_number = THREAD_ENTERPRISE_NUMBER;
            }

            service_entry.S_service_data_length = *dptr++;
            service_entry.S_service_data = dptr;
            dptr += service_entry.S_service_data_length;

            tr_debug("Service data: %s, enterprise number: %"PRIu32, trace_array(service_entry.S_service_data,
                                                                                 service_entry.S_service_data_length), service_entry.S_enterprise_number);

            length -= 2 + service_entry.S_service_data_length;

            while (length > 2) {
                type = *dptr++;
                subLength = *dptr++;
                length -= 2;

                if (subLength <= length) {
                    tr_debug("SubType: %02x, length: %d, data: %s", type, length, trace_array(dptr, subLength));

                    length -= subLength;

                    thread_network_data_service_server_entry_t server = {0};

                    if (type & THREAD_NWK_STABLE_DATA) {
                        server.stable = true;
                    }

                    server.router_id = common_read_16_bit(dptr);
                    subLength -= 2;
                    dptr += 2;

                    server.server_data = dptr;
                    server.server_data_length = subLength;
                    tr_debug("Router ID: %04x, Server data: %s", server.router_id, trace_array(server.server_data, server.server_data_length));

                    thread_nd_local_list_add_service(networkDataStorage, &service_entry, &server);
                    dptr += subLength;
                } else {
                    tr_debug("Server sub-TLV parse fail!");
                    return -1;
                }
            }
        } else {
            return -1;
        }
    }

    if (update_data) {
        ret_val = 1;
    }
    return ret_val;
}

int thread_bootstrap_network_data_activate(protocol_interface_info_entry_t *cur)
{
    //Mark old data to be be freed
    thread_network_data_router_id_mark_delete(&cur->thread_info->networkDataStorage, 0xffff, false);

    int retVal = thread_bootstrap_network_data_process(cur, cur->thread_info->networkDataStorage.network_data, cur->thread_info->networkDataStorage.network_data_len);

    if (retVal < 0) {
        tr_warn("network data update failed: %s", trace_array(cur->thread_info->networkDataStorage.network_data, cur->thread_info->networkDataStorage.network_data_len));
        return retVal;
    }

    // delete marked data
    if (thread_network_data_router_id_free(&cur->thread_info->networkDataStorage, false, cur)) {
        thread_bootstrap_child_update_trig(cur);
    }

    // Learn the routes and dhcp addresses from prefixes
    thread_bootstrap_network_prefixes_process(cur);
    //Add new anycast addressess learned from network data.
    thread_router_bootstrap_anycast_address_register(cur);
    // Update joiner router status
    thread_management_server_joiner_router_init(cur->id);
    thread_management_server_ccm_service_init(cur->id);

    // Update border router relay
    thread_bbr_commissioner_proxy_service_update(cur->id);
    // update beacons
    thread_beacon_create_payload(cur);

    // Indicate network data change to other modules
    thread_bootstrap_pbbr_network_data_process(cur);
    thread_border_router_network_data_update_notify(cur);
    thread_bbr_network_data_update_notify(cur);

    thread_maintenance_timer_set(cur);

    return 0;
}

int thread_bootstrap_network_data_save(protocol_interface_info_entry_t *cur, thread_leader_data_t *leader_data, uint8_t *network_data_ptr, uint16_t network_data_len)
{
    if (!cur || !cur->thread_info || !leader_data || network_data_len > THREAD_MAX_NETWORK_DATA_SIZE) {
        tr_warn("Network data saving failed");
        return -1;
    }

    if (thread_attach_ready(cur) != 0) {
        return -2;
    }

    if (thread_network_data_malformed_check(network_data_ptr, network_data_len) != 0) {
        tr_warn("Malformed nw data: %s", trace_array(network_data_ptr, network_data_len));
        return -3;
    }

    // Do not process the network data until we are synchronized
    tr_debug("learn new network data");

    if (thread_info(cur)->thread_leader_data->stableDataVersion != leader_data->stableDataVersion) {
        thread_info(cur)->thread_leader_data->stableDataVersion = leader_data->stableDataVersion;
        cur->thread_info->networkDataStorage.stableUpdatePushed = true;
    }
    if (thread_info(cur)->thread_leader_data->dataVersion != leader_data->dataVersion) {
        thread_info(cur)->thread_leader_data->dataVersion = leader_data->dataVersion;
        cur->thread_info->networkDataStorage.temporaryUpdatePushed = true;
    }
    if ((network_data_len != cur->thread_info->networkDataStorage.network_data_len ||
            memcmp(cur->thread_info->networkDataStorage.network_data, network_data_ptr, network_data_len) != 0)) {
        // Network data was changed so at least it will be unstable change
        cur->thread_info->networkDataStorage.temporaryUpdatePushed = true;
        tr_debug("Network data changed; size %d stable:%d, unstable:%d", network_data_len, cur->thread_info->networkDataStorage.stableUpdatePushed, cur->thread_info->networkDataStorage.temporaryUpdatePushed);
        memcpy(cur->thread_info->networkDataStorage.network_data, network_data_ptr, network_data_len);
        cur->thread_info->networkDataStorage.network_data_len = network_data_len;
    }

    return 0;
}

void thread_bootstrap_network_prefixes_process(protocol_interface_info_entry_t *cur)
{
    // Route prefix is variable-length, so need to zero pad for ip6tos
    uint8_t addr[16];
    bool validToLearnRoutes, validToLearOnMeshRoute;
    thread_network_server_data_entry_t *weHostService = NULL;
    uint16_t routerId;
    tr_debug("Network Data process:");
    routerId = cur->mac_parameters->mac_short_address;
    thread_network_data_cache_entry_t *networkData;
    networkData = &cur->thread_info->networkDataStorage;
    validToLearnRoutes = thread_route_possible_add(cur->thread_info->thread_device_mode);
    validToLearOnMeshRoute = thread_on_mesh_route_possible_add(cur->thread_info->thread_device_mode);

    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_BORDER_ROUTER, NULL);

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, curPrefix, &networkData->localPrefixList) {
        weHostService = thread_nd_hosted_by_this_routerid(routerId, &curPrefix->routeList);
        tr_debug("Local ServicePrefix: %s", trace_ipv6_prefix(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen));

        if (!weHostService && validToLearnRoutes) {
            ns_list_foreach(thread_network_server_data_entry_t, curRouteItem, &curPrefix->routeList) {
                thread_addr_write_mesh_local_16(addr, curRouteItem->routerID, cur->thread_info);
                tr_debug("Add new route via: %s", trace_ipv6(addr));
                ipv6_route_add(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, addr, ROUTE_THREAD_BORDER_ROUTER, 0xffffffff, curRouteItem->Prf);
            }
        }

        weHostService = thread_nd_hosted_by_this_routerid(routerId, &curPrefix->borderRouterList);
        if (weHostService) {
            tr_debug("I'm Hosting BR");
        }

        ns_list_foreach(thread_network_server_data_entry_t, curBorderRouter, &curPrefix->borderRouterList) {
            //Set Default route ::/0
            if (curBorderRouter->P_default_route) {
                if (!(weHostService && weHostService->P_default_route) && validToLearnRoutes) {
                    thread_addr_write_mesh_local_16(addr, curBorderRouter->routerID, cur->thread_info);
                    tr_debug("Add default route via: %s", trace_ipv6(addr));
                    ipv6_route_add(NULL, 0, cur->id, addr, ROUTE_THREAD_BORDER_ROUTER, 0xffffffff, curBorderRouter->Prf);
                }
            }
            ipv6_stack_route_advert_update(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, RA_PRF_LOW);

            if (thread_nd_on_mesh_address_valid(curBorderRouter)) {
                if (validToLearOnMeshRoute) {
                    if (curBorderRouter->P_dhcp && weHostService && nd_proxy_enabled_for_upstream(cur->id) && nd_proxy_upstream_route_onlink(cur->id, curPrefix->servicesPrefix)) {
                        // don't add
                        tr_debug("Suppressing onlink %s for proxy", trace_ipv6_prefix(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen));
                    } else if (curBorderRouter->P_res1) {
                        ipv6_route_add(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, NULL, ROUTE_THREAD_PROXIED_DUA_HOST, 0xffffffff, 0);
                    } else {
                        //add
                        tr_debug("Adding onlink %s", trace_ipv6_prefix(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen));
                        ipv6_route_add(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, NULL, ROUTE_THREAD_BORDER_ROUTER, 0xffffffff, curBorderRouter->Prf);
                    }
                }
            }

            if (curBorderRouter->P_dhcp) {
                /* All end device types perform BR RLOC16 -> ALOC16
                   replacement if stable network data was requested. */
                if ((cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST ||
                        cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST) &&
                        cur->thread_info->requestFullNetworkData == false) {
                    ns_list_foreach(thread_network_data_context_entry_t, curRoute, &curPrefix->contextList) {
                        curBorderRouter->routerID = 0xfc00;
                        curBorderRouter->routerID |= curRoute->cid;
                        tr_debug("Replaced router ID with ALOC16: %04x", curBorderRouter->routerID);
                    }
                }

                if (!thread_dhcpv6_address_entry_available(curPrefix->servicesPrefix, &cur->ip_addresses)) {
                    thread_addr_write_mesh_local_16(addr, curBorderRouter->routerID, cur->thread_info);
                    /* Do not allow multiple DHCP solicits from one prefix => delete previous */
                    dhcp_client_global_address_delete(cur->id, NULL, curPrefix->servicesPrefix);
                    if (dhcp_client_get_global_address(cur->id, addr, curPrefix->servicesPrefix, thread_dhcp_client_gua_error_cb) == 0) {
                        tr_debug("GP Address Requested");
                    }
                }
            } else {
                /* All end device types perform RLOC16 -> 0xfffe
                   replacement if stable network data was requested. */
                if ((cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST ||
                        cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST) &&
                        cur->thread_info->requestFullNetworkData == false) {
                    tr_debug("Invalidate router ID: %04x", curBorderRouter->routerID);
                    curBorderRouter->routerID = 0xfffe;
                }
            }

            if (curBorderRouter->P_preferred) {
                if (!thread_dhcpv6_address_entry_available(curPrefix->servicesPrefix, &cur->ip_addresses)) {
                    icmpv6_slaac_address_add(cur, curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, 0xffffffff, 0xffffffff, true, SLAAC_IID_DEFAULT);
                }
            }
            // generate address based on res1 bit
            if (curBorderRouter->P_res1) {
                if (!thread_dhcpv6_address_entry_available(curPrefix->servicesPrefix, &cur->ip_addresses)) {
                    thread_bootstrap_dua_address_generate(cur, curPrefix->servicesPrefix, 64);
                }
            }

        } // for each borderRouterList

        ns_list_foreach(thread_network_data_context_entry_t, curRoute, &curPrefix->contextList) {
            uint8_t flags;
            flags = curRoute->cid;
            if (curRoute->compression) {
                flags |= 0x10;
            }

            lowpan_context_update(&cur->lowpan_contexts, flags, 0xFFFF, curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, curRoute->stableData);
        }
    } // for each localPrefixList
}

void thread_bootstrap_network_data_update(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->thread_info) {
        return;
    }

    if (cur->thread_info->networkDataStorage.stableUpdatePushed) {
        tr_debug("Stable Network Data Update");
    } else if (cur->thread_info->networkDataStorage.temporaryUpdatePushed) {
        tr_debug("Temporary Network Data Update");
    } else {
        // No changes in network data detected so no processing done
        return;
    }
    tr_info("Network data updated");
    thread_bootstrap_network_data_activate(cur);
    thread_router_bootstrap_network_data_distribute(cur);

    cur->thread_info->networkDataStorage.stableUpdatePushed = false;
    cur->thread_info->networkDataStorage.temporaryUpdatePushed = false;
}

void thread_bootstrap_clear_neighbor_entries(protocol_interface_info_entry_t *cur)
{
    if (cur == NULL || cur->thread_info == NULL) {
        return;
    }

    // Remove registered entries in the IP neighbor cache
    ns_list_foreach_safe(ipv6_neighbour_t, neighbour, &cur->ipv6_neighbour_cache.list) {
        if (neighbour->type == IP_NEIGHBOUR_REGISTERED) {
            ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour);
        }
    }
    mac_neighbor_table_neighbor_list_clean(mac_neighbor_info(cur));
}

void thread_bootstrap_dynamic_configuration_save(protocol_interface_info_entry_t *cur)
{
    uint32_t mac_frame_counter;
    if (!cur->thread_info) {
        return;
    }

    if (thread_i_am_router(cur)) {
        /* Store information of our children to the dynamic storage */
        mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;
        ns_list_foreach_safe(mac_neighbor_table_entry_t, entry, mac_table_list) {
            if (thread_addr_is_child(mac_helper_mac16_address_get(cur), entry->mac16)) {
                thread_dynamic_storage_child_info_store(cur, entry);
            }
        }
    }
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);
    mac_helper_link_frame_counter_read(cur->id, &mac_frame_counter);
    // in error situation this returns 0 !!!!
    uint32_t mle_frame_counter = mle_service_security_get_frame_counter(cur->id);
    if (linkConfiguration) {
        thread_nvm_store_fast_data_check_and_write(mac_frame_counter, mle_frame_counter, linkConfiguration->key_sequence);
    } else {
        thread_nvm_store_frame_counters_check_and_write(mac_frame_counter, mle_frame_counter);
    }
}

bool thread_bootstrap_link_create_check(protocol_interface_info_entry_t *interface, uint16_t short_address)
{
    if (interface->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE ||
            interface->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE) {
        // MED's never make any links so return false
        return false;
    }

    if (mle_class_free_entry_count_get(interface) < 1) {
        // We dont have room for any new links
        tr_warn("Link ignore no room for addr:%x", short_address);
        return false;
    }
    // TODO should check that we have enough room left for children
    // TODO Need to drop links for bad routers if no room

    if (interface->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        //routers always make links
        return true;
    }

    if (thread_is_router_addr(short_address) == false) {
        //incoming advertisement is not from router so return false
        return false;
    }

    if (mle_class_active_neigh_counter(interface) < THREAD_REED_AND_END_DEVICE_NEIGHBOR_LINKS + 1) {
        return true;
    }

    return false;
}

bool thread_bootstrap_link_create_allowed(protocol_interface_info_entry_t *interface, uint16_t short_address, const uint8_t *src_addr)
{
#ifndef HAVE_DEBUG
    (void) short_address;
#endif
    // Add blacklist of routers that newer answer to us
    if (mle_service_interface_tx_queue_size(interface->id) > THREAD_MAX_PARALLEL_MLE_LINK_REQUEST) {
        // Maximum parallel link requests
        tr_warn("Link ignore too many req addr:%x", short_address);
        return false;
    }

    if (blacklist_reject(src_addr)) {
        return false;
    }

    return true;
}

bool thread_bootstrap_should_register_address(protocol_interface_info_entry_t *cur)
{
    // If we are a MTD, send requests to register addresses
    // Neither REEDs nor FEDs need to register their addresses
    uint8_t mode = thread_mode_get_by_interface_ptr(cur);
    return (mode & MLE_DEV_MASK) == MLE_RFD_DEV;
}

/**
 * Thread 1.2 support
 */

#ifdef HAVE_THREAD_V2

static void thread_bootstrap_mcast_subscrition_change(protocol_interface_info_entry_t *interface)
{
    uint8_t *addr;
    uint8_t *ptr;
    uint8_t addr_len;
    uint8_t br_ml_addr[16];

    if (0 != thread_common_primary_bbr_get(interface, br_ml_addr, NULL, NULL, NULL)) {
        return;
    }
    addr = ns_dyn_mem_temporary_alloc(16 * thread_max_mcast_addr + 2);
    if (!addr) {
        return;
    }
    // MLR is sent only for primary BBR for now, but this might change
    ptr = addr;
    addr_len = 0;
    *ptr++ = TMFCOP_TLV_IPV6_ADDRESS;
    *ptr++ = 0; // Length will be updated later
    ns_list_foreach(if_group_entry_t, cur, &interface->ip_groups) {
        if (addr_ipv6_multicast_scope(cur->group) < IPV6_SCOPE_ADMIN_LOCAL) {
            continue;
        }
        if (addr_ipv6_equal(cur->group, ADDR_SITE_LOCAL_ALL_ROUTERS)) {
            continue;
        }
        memcpy(ptr, cur->group, 16);
        ptr += 16;
        addr_len = ptr - addr;
        addr[1] = addr_len - 2;// Fill in the length
        if (addr_len > 16 * thread_max_mcast_addr) {
            // Send first patch
            thread_management_client_mlr_req_send(interface->id, br_ml_addr, addr, addr_len);
            // Reset the packet to start
            ptr = addr;
            ptr += 2;
            addr_len = 0;
        }
    }
    if (addr_len != 0) {
        // Send rest
        thread_management_client_mlr_req_send(interface->id, br_ml_addr, addr, addr_len);
    }
    ns_dyn_mem_free(addr);
}
static int thread_bootstrap_dua_registration_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) source_address;
    (void) source_port;

    uint16_t addr_len;
    uint8_t bbr_status;
    uint8_t *addr_data_ptr = NULL;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_client_get_interface_id_by_service_id(service_id));

    if (!cur) {
        return -1;
    }

    if (!response_ptr) {
        tr_warn("DUA.resp failed");
        return -2;
    }

    if (1 > thread_meshcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_STATUS, &bbr_status)) {
        return 0;
    }

    addr_len = thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &addr_data_ptr);

    if (addr_len < 16) {
        tr_warn("Invalid target eid in DUA.rsp cb message");
        return 0;
    }

    if_address_entry_t *addr_entry = addr_get_entry(cur, addr_data_ptr);
    if (addr_entry) {
        // Own processing
        if (bbr_status == THREAD_ST_DUA_SUCCESS) {
            addr_entry->preferred_lifetime = 0xffffffff;
        } else if (bbr_status == THREAD_ST_DUA_DUPLICATE) {
            cur->dad_failures++;
            thread_bootstrap_dua_address_generate(cur, addr_data_ptr, 64);
        } else if (bbr_status == THREAD_ST_DUA_INVALID) {
            addr_delete(cur, addr_data_ptr);
        } else {
            addr_entry->preferred_lifetime = 0;
        }
        return 0;
    }

    // processing for MTD children
    ipv6_neighbour_t *neighbour_entry;
    uint16_t nce_short_addr;
    uint8_t destination_address[16] = {0};

    if (bbr_status == THREAD_ST_DUA_SUCCESS) {
        // registration successful
        return 0;
    }

    neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, addr_data_ptr);
    if (!neighbour_entry) {
        return 0;
    }

    nce_short_addr = common_read_16_bit(neighbour_entry->ll_address + 2);
    if (!thread_addr_is_child(cur->thread_info->routerShortAddress, nce_short_addr)) {
        ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
        return 0;
    }

    if (bbr_status == THREAD_ST_DUA_DUPLICATE || bbr_status == THREAD_ST_DUA_INVALID) {
        // remove invalid or duplicate child entry
        ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
    }

    thread_addr_write_mesh_local_16(destination_address, nce_short_addr, cur->thread_info);

    thread_management_client_addr_ntf_send(cur->id, destination_address, addr_data_ptr, bbr_status);

    return 0;
}

static void thread_bootstrap_dua_registration_send(protocol_interface_info_entry_t *cur, const uint8_t address[16], const uint8_t ml_eid[8], const uint8_t dst_addr[16])
{

    tr_debug("domain address registration (DUA.req send)");
    thread_bootstrap_pbbr_update_done(cur);

    uint8_t payload[2 + 16 + 2 + 8];
    uint8_t *ptr;

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, address);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, ml_eid);

    coap_service_request_send(thread_management_server_service_id_get(cur->id), COAP_REQUEST_OPTIONS_NONE,
                              dst_addr, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_BBR_DOMAIN_ADDRESS_REGISTRATION, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, thread_bootstrap_dua_registration_cb);
}

static int thread_bootstrap_pbbr_update_needed(struct protocol_interface_info_entry *cur, uint16_t rloc, uint8_t seq)
{
    if (!cur->thread_info->ccm_info) {
        return 0;
    }
    if (cur->thread_info->ccm_info->update_needed  ||
            seq != cur->thread_info->ccm_info->sequence_number ||
            rloc != cur->thread_info->ccm_info->rloc) {
        return 1;
    }
    return 0;
}

static void thread_bootstrap_pbbr_update_done(struct protocol_interface_info_entry *cur)
{
    // Check if network data changed or bbr info and send proactive an if needed
    uint8_t addr[16];
    uint8_t sequence_number;
    uint32_t delay_timer;
    uint32_t br_mlr_timer;
    if (!cur->thread_info->ccm_info) {
        return;
    }
    if (0 == thread_common_primary_bbr_get(cur, addr, &sequence_number, &br_mlr_timer, &delay_timer)) {
        // BBR updated
        cur->thread_info->ccm_info->rloc = common_read_16_bit(&addr[14]);
        cur->thread_info->ccm_info->sequence_number = sequence_number;
        cur->thread_info->ccm_info->delay_timer = delay_timer;
        cur->thread_info->ccm_info->mlr_timer = br_mlr_timer;
        cur->thread_info->ccm_info->update_needed = false;
    }
}

static void thread_bootstrap_pbbr_network_data_process(struct protocol_interface_info_entry *cur)
{
    // Check if network data changed or bbr info and send proactive an if needed
    uint8_t addr[16];
    uint8_t sequence_number;
    uint32_t delay_timer;
    uint32_t mlr_timeout;

    if (0 != thread_common_primary_bbr_get(cur, addr, &sequence_number, &mlr_timeout, &delay_timer)) {
        // BBR not present
        return;
    }
    if (1 == thread_bootstrap_pbbr_update_needed(cur, common_read_16_bit(&addr[14]), sequence_number)) {
        thread_bootstrap_address_registration_timer_set(cur, 1 + randLIB_get_random_in_range(0, delay_timer / 1000), randLIB_get_random_in_range(1, 5));
    }
}

static void thread_bootstrap_address_registration_trigger(protocol_interface_info_entry_t *interface)
{
    // Address notification is sent only for DUA address
    thread_bootstrap_pbbr_update_done(interface);
    // Send notification on behalf of children
    ns_list_foreach(ipv6_neighbour_t, n, &interface->ipv6_neighbour_cache.list) {
        if (n->type == IP_NEIGHBOUR_REGISTERED && addr_ipv6_scope(n->ip_address, interface) > IPV6_SCOPE_REALM_LOCAL) {
            mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(interface), &n->ll_address[2], ADDR_802_15_4_SHORT);
            if (entry) {
                thread_bootstrap_address_registration(interface, n->ip_address, entry->mac64, true, false);
            }
        }
    }

    // Send own Address
    ns_list_foreach(if_address_entry_t, e, &interface->ip_addresses) {
        if (addr_ipv6_scope(e->address, interface) > IPV6_SCOPE_REALM_LOCAL) {
            thread_bootstrap_address_registration(interface, e->address, NULL, false, false);
        }
    }
}

void thread_bootstrap_address_registration_init()
{
    enabled = true;
    addr_notificastion_timer = THREAD_PROACTIVE_AN_INTERVAL;
    mlr_timer = 0;
}

bool thread_bootstrap_address_registration_running()
{
    return enabled;
}

void thread_bootstrap_address_registration_deinit()
{
    enabled = false;
}

bool thread_bootstrap_is_domain_prefix(protocol_interface_info_entry_t *interface, const uint8_t *addr)
{
    uint8_t address[16];
    memcpy(address, addr, 16);
    thread_prefix_tlv_t prefixTlv;
    prefixTlv.domainId = 0;
    prefixTlv.Prefix = address;
    prefixTlv.PrefixLen = 64;
    thread_network_data_prefix_cache_entry_t *prefix_entry = NULL;
    thread_network_data_cache_entry_t *networkDataCache = &interface->thread_info->networkDataStorage;
    prefix_entry = thread_prefix_entry_find(&networkDataCache->localPrefixList, &prefixTlv);
    if (!prefix_entry) {
        return false;
    }
    ns_list_foreach(thread_network_server_data_entry_t, brEntry, &prefix_entry->borderRouterList) {
        if (brEntry->P_res1) {
            return true;
        }
    }
    return false;
}

void thread_bootstrap_dua_address_generate(protocol_interface_info_entry_t *cur, const uint8_t *domain_prefix, uint8_t domain_prefix_len)
{
    if_address_entry_t *def_address = NULL;
    uint8_t domain_address[16];
    if (thread_info(cur)->version < THREAD_VERSION_1_2) {
        return;
    }
    addr_delete_matching(cur, NULL, 0, ADDR_SOURCE_THREAD_DOMAIN);
    memcpy(domain_address, domain_prefix, 8);
    def_address = icmpv6_slaac_address_add(cur, domain_address, domain_prefix_len, 0xffffffff, 0xffffffff, true, SLAAC_IID_DEFAULT);
    if (def_address) {
        tr_info("Generated domain address 64: %s", trace_ipv6(def_address->address));
        def_address->source = ADDR_SOURCE_THREAD_DOMAIN;
    } else {
        tr_error("Domain address creation failed");
    }
}

void thread_bootstrap_address_registration(struct protocol_interface_info_entry *interface, const uint8_t *addr, const uint8_t *child_mac64, bool refresh_child_entry, bool duplicate_child_detected)
{
    uint8_t *ml_eid;
    uint8_t br_ml_addr[16];
    uint8_t seq;
    uint32_t delay_timer;

    if (thread_info(interface)->version < THREAD_VERSION_1_2) {
        return;
    }

    if (!thread_bootstrap_is_domain_prefix(interface, addr)) {
        return;
    }
    tr_debug("domain prefix address");
    if (0 != thread_common_primary_bbr_get(interface, br_ml_addr, &seq, &delay_timer, NULL)) {
        // No pBBR present
        return;
    }
    tr_info("domain prefix Primary bbr present");

    if (child_mac64) {
        mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(interface), child_mac64, ADDR_802_15_4_LONG);

        if (!entry) {
            tr_error("No MLE entry.");
            return;
        }

        ml_eid = thread_neighbor_class_get_mleid(&interface->thread_info->neighbor_class, entry->index);
        if (!ml_eid) {
            tr_error("No Thread neighbor.");
            return;
        }

        if (duplicate_child_detected) {
            uint8_t dest_address[16] = {0};
            thread_addr_write_mesh_local_16(dest_address, entry->mac16, interface->thread_info);
            thread_management_client_addr_ntf_send(interface->id, dest_address, addr, 3);
            return;
        }
        if (!refresh_child_entry) {
            // don't send n/dr for existing entry
            return;
        }
    } else {
        link_configuration_s *link_config = thread_joiner_application_get_config(interface->id);

        if (!link_config) {
            tr_error("No link configuration.");
            return;
        }
        ml_eid = thread_joiner_application_ml_eid_get(interface->id);
    }

    // Register the DUA address
    thread_bootstrap_dua_registration_send(interface, addr, ml_eid, br_ml_addr);

    return;
}


void thread_bootstrap_address_registration_timer_set(protocol_interface_info_entry_t *interface, uint16_t dua_delay_seconds, uint16_t mlr_refresh_seconds)
{
    (void)interface;
    if (dua_delay_seconds != 0) {
        addr_notificastion_timer = dua_delay_seconds;
    }
    if (mlr_refresh_seconds != 0) {
        mlr_timer = mlr_refresh_seconds;
    }

}
void thread_bootstrap_address_registration_timer(protocol_interface_info_entry_t *interface, uint16_t seconds)
{
    uint32_t mlr_timeout;
    uint32_t delay_timer;
    if (!enabled || interface->thread_info->version < THREAD_VERSION_1_2) {
        return;
    }
    if (thread_bootstrap_should_register_address(interface)) {
        if (!interface->thread_info->thread_endnode_parent) {
            // We dont have parent so skip
            return;
        }
        if (interface->thread_info->thread_endnode_parent->version >= THREAD_VERSION_1_2) {
            // Parent supports all features
            return;
        }
    }

    if (0 != thread_common_primary_bbr_get(interface, NULL, NULL, &mlr_timeout, &delay_timer)) {
        // BBR not present
        return;
    }
    /* Update multicast addresses */
    if (mlr_timer > seconds) {
        mlr_timer -= seconds;
    } else {
        mlr_timer = mlr_timeout - randLIB_get_random_in_range(30, 50);
        thread_bootstrap_mcast_subscrition_change(interface);
    }
    /* Update global scope addresses (children's as well) */
    if (addr_notificastion_timer > seconds) {
        addr_notificastion_timer -= seconds;
    } else {
        addr_notificastion_timer = THREAD_PROACTIVE_AN_INTERVAL + randLIB_get_random_in_range(0, delay_timer / 1000);
        thread_bootstrap_address_registration_trigger(interface);
    }

}
void thread_bootstrap_child_address_registration_response_process(struct protocol_interface_info_entry *interface)
{
    //  if we are version 3 and parent is lower we need to send registrations for them

    if (interface->thread_info->version < THREAD_VERSION_1_2) {
        thread_bootstrap_address_registration_deinit();
        return;
    }
    if (!interface->thread_info->thread_endnode_parent) {
        // We dont have parent return
        thread_bootstrap_address_registration_deinit();
        return;
    }
    if (interface->thread_info->thread_endnode_parent->version >= THREAD_VERSION_1_2) {
        //parent supports 1.2
        thread_bootstrap_address_registration_deinit();
        return;
    }
    // We will start address registration timers
    if (!thread_bootstrap_address_registration_running()) {
        uint32_t dua_delay;
        if (0 != thread_common_primary_bbr_get(interface, NULL, NULL, NULL, &dua_delay)) {
            dua_delay = randLIB_get_random_in_range(1, 5);
        }
        thread_bootstrap_address_registration_init();
        thread_bootstrap_address_registration_timer_set(interface, 1 + randLIB_get_random_in_range(0, dua_delay / 1000), randLIB_get_random_in_range(1, 5));
    }
}


#endif
#endif
