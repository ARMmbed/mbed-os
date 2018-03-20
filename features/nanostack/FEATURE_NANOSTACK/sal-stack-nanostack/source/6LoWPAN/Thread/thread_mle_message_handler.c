/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#ifdef HAVE_THREAD

#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include <nsdynmemLIB.h>
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_lowpower_private_api.h"
#include "6LoWPAN/Thread/thread_mle_message_handler.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MLE/mle.h"
#include "mac_api.h"
#define TRACE_GROUP "thmh"
static int8_t thread_link_request_start(protocol_interface_info_entry_t *cur, uint8_t *router_ll64);
static bool thread_router_leader_data_process(protocol_interface_info_entry_t *cur, uint8_t *src_address, thread_leader_data_t *leaderData, mle_tlv_info_t *routeTlv, mle_neigh_table_entry_t *neighbor);
static void thread_parse_advertisement(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin);
static void thread_save_leader_data(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData);
static void thread_parse_accept(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin);
static void thread_parse_annoucement(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg);
static void thread_parse_data_response(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t linkMargin);
static void thread_host_child_update_request_process(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t linkMargin);
static void thread_parse_child_update_response(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin);

/* Public functions */
void thread_general_mle_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    /* TODO's:
     * - Any incoming link configuration or advertisement message, or an incoming update sent to a
     *   link-local address, whose IP Hop Limit is not 255, may have been forwarded by a Router
     *   and MUST be discarded.
     */

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return;
    }

    /* Check that message is from link-local scope */
    if(!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    uint8_t linkMargin = thread_compute_link_margin(mle_msg->dbm);

    // Always save lowpower queries
    thread_lowpower_process_request(mle_msg);

    switch (mle_msg->message_type) {
    case MLE_COMMAND_ACCEPT: {
        thread_parse_accept(cur, mle_msg, security_headers, linkMargin);
        break;
    }

    case MLE_COMMAND_METRIC_MANAGEMENT_REQUEST: {
        thread_lowpower_metrics_management_query_request_process(cur, mle_msg, security_headers, linkMargin);
        break;
    }

    case MLE_COMMAND_REJECT: {
        mle_neigh_table_entry_t *entry_temp;
        tr_warn("Reject Link");
        entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, false);
        if (entry_temp) {
            mle_class_remove_entry(cur->id, entry_temp);
        }
        break;
    }

    case MLE_COMMAND_DATASET_ANNOUNCE: {
        thread_parse_annoucement(cur, mle_msg);
        break;
    }

    case MLE_COMMAND_ADVERTISEMENT: {
        thread_parse_advertisement(cur, mle_msg, security_headers, linkMargin);
        break;
    }

    case MLE_COMMAND_DATA_RESPONSE: {
        thread_parse_data_response(cur, mle_msg, linkMargin);
        break;
    }

    case MLE_COMMAND_CHILD_UPDATE_RESPONSE:
    {
        thread_parse_child_update_response(cur, mle_msg, security_headers, linkMargin);
        break;
    }

    case MLE_COMMAND_PARENT_RESPONSE: {
        tr_debug("MLE parent response received");
        thread_mle_parent_discover_receive_cb(interface_id, mle_msg, security_headers);
        break;
    }
    default:
        if ((thread_am_host(cur) || thread_am_reed(cur)) && mle_msg->message_type == MLE_COMMAND_CHILD_UPDATE_REQUEST){
            // Thread host and router in REED mode answer the message same way. Routers only process messages from child
            thread_host_child_update_request_process(cur, mle_msg, linkMargin);
        } else if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
            thread_router_bootstrap_mle_receive_cb(interface_id, mle_msg, security_headers);
        } else {
            tr_warn("Not supported MLE message for host %d", mle_msg->message_type);
        }
    }

}

static void thread_save_leader_data(protocol_interface_info_entry_t *cur, thread_leader_data_t *leaderData)
{
    if (thread_info(cur)->thread_leader_data) {
        bool requestNetworkdata = false;
        if (common_serial_number_greater_8(leaderData->dataVersion, thread_info(cur)->thread_leader_data->dataVersion)) {
            requestNetworkdata = true;

        } else if (common_serial_number_greater_8(leaderData->stableDataVersion, thread_info(cur)->thread_leader_data->stableDataVersion)) {
            requestNetworkdata = true;
        }

        if (thread_info(cur)->thread_leader_data->partitionId != leaderData->partitionId) {
            requestNetworkdata = true;
            thread_info(cur)->thread_leader_data->stableDataVersion = leaderData->stableDataVersion - 1;
            thread_info(cur)->thread_leader_data->dataVersion = leaderData->dataVersion - 1;
        }

        thread_info(cur)->thread_leader_data->partitionId = leaderData->partitionId;
        thread_info(cur)->thread_leader_data->leaderRouterId = leaderData->leaderRouterId;
        thread_info(cur)->thread_leader_data->weighting = leaderData->weighting;

        if (requestNetworkdata) {
            thread_bootstrap_parent_network_data_request(cur, false);
        } else {
            thread_info(cur)->thread_leader_data->stableDataVersion = leaderData->stableDataVersion;
            thread_info(cur)->thread_leader_data->dataVersion = leaderData->dataVersion;
        }
    }
}

static int8_t thread_link_request_start(protocol_interface_info_entry_t *cur, uint8_t *router_ll64)
{
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t bufId = mle_service_msg_allocate(cur->id, 32, true, MLE_COMMAND_REQUEST);

    if (bufId == 0) {
        return -1;
    }

    timeout.retrans_max = 2;
    timeout.timeout_init = 2;
    timeout.timeout_max = 3;
    timeout.delay = MLE_STANDARD_RESPONSE_DELAY;

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    ptr = mle_general_write_source_address(ptr, cur);
    ptr = thread_leader_data_tlv_write(ptr, cur);

    tr_debug("Synch Req");

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_service_set_packet_callback(bufId, thread_link_request_timeout);
    mle_service_set_msg_destination_address(bufId, router_ll64);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return 0;
}

static bool thread_router_leader_data_process(protocol_interface_info_entry_t *cur, uint8_t *src_address, thread_leader_data_t *leaderData, mle_tlv_info_t *routeTlv, mle_neigh_table_entry_t *neighbor)
{
    int leaderDataUpdate = thread_leader_data_validation(cur, leaderData, routeTlv);

    if (leaderDataUpdate == 1) {
        if (neighbor && neighbor->handshakeReady == 1) {
            // Request network data if we have a 2-way link
            tr_debug("Request New Network Data from %s", trace_ipv6(src_address));
            thread_network_data_request_send(cur, src_address, true);
        } else {

        }
    } else if (leaderDataUpdate == 2) {
        tr_debug("Start Merge");
        thread_bootstrap_connection_error(cur->id, CON_ERROR_PARTITION_MERGE, NULL);
        return false;

    } else if (leaderDataUpdate < 0) {
        tr_warn("Leader data failure");
        return false;
    }
    return true;
}

static bool thread_reed_partitions_merge(protocol_interface_info_entry_t *cur, uint16_t shortAddress, thread_leader_data_t heard_partition_leader_data)
{
    if (thread_is_router_addr(shortAddress)) {
        return false;
    }
    if (thread_extension_version_check(thread_info(cur)->version)) {
        // lower weighting heard
        if (thread_info(cur)->thread_leader_data->weighting > heard_partition_leader_data.weighting) {
            return false;
        }
        // lower/same partition id heard
        if (thread_info(cur)->thread_leader_data->weighting == heard_partition_leader_data.weighting &&
                thread_info(cur)->thread_leader_data->partitionId >= heard_partition_leader_data.partitionId ) {
            return false;
        }
    } else if (thread_info(cur)->thread_leader_data->partitionId >= heard_partition_leader_data.partitionId){
        return false;
    }
    // can merge to a higher weighting/partition id
    thread_bootstrap_connection_error(cur->id, CON_ERROR_PARTITION_MERGE, NULL);
    return true;
}

static bool thread_router_advertiment_tlv_analyze(uint8_t *ptr, uint16_t data_length, thread_leader_data_t *leaderData, uint16_t *shortAddress, mle_tlv_info_t *routeTlv)
{
    //Read Leader Data and verify connectivity
    // Leader data is mandatory
    if (!thread_leader_data_parse(ptr, data_length, leaderData)) {
        return false;
    }

    if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, ptr, data_length, shortAddress)) {
        return false;
    }

    if (!mle_tlv_read_tlv(MLE_TYPE_ROUTE, ptr, data_length, routeTlv)) {
        if (thread_is_router_addr(*shortAddress)) {
            //Sender is reeds
            return false;
        }
        routeTlv->dataPtr = NULL;
        routeTlv->tlvLen = 0;
    }
    return true;
}

static void thread_update_mle_entry(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, mle_neigh_table_entry_t *entry_temp, uint16_t short_address)
{
    if (!entry_temp) {
        return;
    }

    mle_neigh_entry_frame_counter_update(entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur, security_headers->KeyIndex);

    if (!thread_attach_active_router(cur) && !thread_check_is_this_my_parent(cur, entry_temp)) {
        mle_entry_timeout_refresh(entry_temp);
    } else {
        entry_temp->last_contact_time = protocol_core_monotonic_time;
    }

    if (short_address != entry_temp->short_adr) {
        if (thread_router_addr_from_addr(entry_temp->short_adr) == cur->thread_info->routerShortAddress) {
            thread_dynamic_storage_child_info_clear(cur->id, entry_temp);
            protocol_6lowpan_release_short_link_address_from_neighcache(cur, entry_temp->short_adr);
        }
        entry_temp->short_adr = short_address;
        /* throw MLME_GET request, short address is changed automatically in get request callback */
        mlme_get_t get_req;
        get_req.attr = macDeviceTable;
        get_req.attr_index = entry_temp->attribute_index;
        cur->mac_api->mlme_req(cur->mac_api, MLME_GET, &get_req);
    }

    return;
}

static void thread_parse_advertisement(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin)
{
    mle_tlv_info_t routeTlv;
    thread_leader_data_t leaderData;
    uint16_t shortAddress;
    mle_neigh_table_entry_t *entry_temp;

    // Check device mode & bootstrap state
    if ((thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) ||
        (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY)) {
        return;
    }

    tr_debug("Received Advertisement");

    // Validate and parse TLV's
    if (!thread_router_advertiment_tlv_analyze(mle_msg->data_ptr, mle_msg->data_length, &leaderData, &shortAddress, &routeTlv)) {
        return;
    }

    // Get MLE entry
    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, false);

    if ((security_headers->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX)
            && (thread_instance_id_matches(cur, &leaderData))) {
        thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));
    }

    // Check parent status
    if (!thread_attach_active_router(cur)) {
        //processing for non routers
        if (thread_check_is_this_my_parent(cur, entry_temp)) {
            //advertisement from parent
            if ((thread_info(cur)->thread_leader_data->partitionId != leaderData.partitionId) ||
                (thread_info(cur)->thread_leader_data->weighting != leaderData.weighting)) {
                //parent changed partition/weight - reset own routing information
                thread_old_partition_data_purge(cur);
            }
            //check if network data needs to be requested
            if (!thread_bootstrap_request_network_data(cur, &leaderData, shortAddress)) {
                tr_debug("Parent short address changed - re-attach");
                thread_bootstrap_connection_error(cur->id, CON_PARENT_CONNECT_DOWN, NULL);
                return;
            }
        }
    }

    // Update MLE entry
    thread_update_mle_entry(cur, mle_msg, security_headers, entry_temp, shortAddress);

    // Process advertisement
    if (thread_info(cur)->thread_device_mode != THREAD_DEVICE_MODE_END_DEVICE) {
        if (!thread_attach_active_router(cur)) {
            // REED and FED
            if (!entry_temp && thread_bootstrap_link_create_check(cur, shortAddress) && thread_bootstrap_link_create_allowed(cur, shortAddress, mle_msg->packet_src_address)) {
                if ((thread_info(cur)->thread_leader_data->partitionId == leaderData.partitionId) &&
                        (thread_info(cur)->thread_leader_data->weighting == leaderData.weighting)) {
                    // Create link to new neighbor no other processing allowed
                    thread_link_request_start(cur, mle_msg->packet_src_address);
                    return;
                }
                if (!thread_router_leader_data_process(cur, mle_msg->packet_src_address, &leaderData, &routeTlv, entry_temp)) {
                    // better partition found or new network data learn started
                    return;
                }
            }
            // process REED advertisement from higher partition
            if (thread_reed_partitions_merge(cur, shortAddress, leaderData)) {
                return;
            }
        } else {
            //Router
            if (!thread_router_leader_data_process(cur, mle_msg->packet_src_address, &leaderData, &routeTlv, entry_temp) ) {
                return;
            }
            thread_router_bootstrap_advertiment_analyze(cur, mle_msg->packet_src_address, entry_temp, shortAddress);
        }
    }

    // Process route TLV
    if ((entry_temp && routeTlv.dataPtr && routeTlv.tlvLen) &&
            (thread_info(cur)->thread_leader_data->partitionId == leaderData.partitionId)){
        tr_debug("Update Route TLV %x", entry_temp->short_adr);
        thread_router_bootstrap_route_tlv_push(cur, routeTlv.dataPtr, routeTlv.tlvLen , linkMargin, entry_temp);
    }
}
static void thread_parse_accept(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin)
{
    uint32_t llFrameCounter;
    uint32_t mleFrameCounter;
    uint16_t version, shortAddress;
    uint16_t messageId;
    uint8_t linkMarginfronNeigh;
    mle_neigh_table_entry_t *entry_temp;
    bool createNew;

    tr_info("MLE LINK ACCEPT");

    messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

    if (messageId == 0) {
        tr_debug("Not for me");
        return;
    }

    if (!addr_is_ipv6_multicast(mle_service_get_msg_destination_address_pointer(messageId))) {
        //Free Response only if it is unicast
        mle_service_msg_free(messageId);
    }

    // TODO: Check missing TLV's
    if ((!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) ||
            (!mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &llFrameCounter)) ||
            (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &shortAddress))) {
        return;
    }

    /* Call to determine whether or not we should create a new link */
    createNew = thread_bootstrap_link_create_check(cur, shortAddress);

    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, createNew);

    if (!entry_temp) {
        thread_link_reject_send(cur, mle_msg->packet_src_address);
        return;
    }

    if (security_headers->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
        thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));
    }

    //If MLE MLE_TYPE_MLE_FRAME_COUNTER TLV is present then use it for validating further messages else use link layer frame counter
    if ((!mle_tlv_read_32_bit_tlv(MLE_TYPE_MLE_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &mleFrameCounter))) {
        mleFrameCounter = llFrameCounter;
    }

    entry_temp->threadNeighbor = true;
    entry_temp->short_adr = shortAddress;
    entry_temp->mle_frame_counter = mleFrameCounter;
    // Set full data as REED needs full data and SED will not make links
    entry_temp->mode |= MLE_THREAD_REQ_FULL_DATA_SET;

    mac_helper_devicetable_set(entry_temp, cur, llFrameCounter, security_headers->KeyIndex);

    if (entry_temp->timeout_rx) {
        mle_entry_timeout_refresh(entry_temp);
    } else {
        mle_entry_timeout_update(entry_temp, THREAD_DEFAULT_LINK_LIFETIME);
    }

    if (thread_i_am_router(cur) && thread_is_router_addr(entry_temp->short_adr)) {
        // If we both are routers, mark the link as 2-way
        entry_temp->handshakeReady = 1;
        tr_debug("Marked link as 2-way, handshakeReady=%d", entry_temp->handshakeReady);
    } else {
        tr_debug("Marked link as 1-way, handshakeReady=%d", entry_temp->handshakeReady);
    }

    blacklist_update(mle_msg->packet_src_address, true);

    if (mle_tlv_read_8_bit_tlv(MLE_TYPE_RSSI, mle_msg->data_ptr, mle_msg->data_length, &linkMarginfronNeigh)) {
        thread_routing_update_link_margin(cur, entry_temp->short_adr, linkMargin, linkMarginfronNeigh);
    }
}
static void thread_parse_annoucement(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg)
{
    uint64_t timestamp;
    uint16_t panid;
    uint8_t *ptr;
    uint8_t channel_page;
    uint16_t channel;
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);


    tr_info("Recv Dataset Announce");
    if (8 > thread_tmfcop_tlv_data_get_uint64(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_ACTIVE_TIMESTAMP,&timestamp)) {
        tr_error("Missing timestamp TLV");
        return;
    }
    if (2 > thread_tmfcop_tlv_data_get_uint16(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_PANID,&panid)) {
        tr_error("Missing Panid TLV");
        return;
    }
    if (3 > thread_tmfcop_tlv_find(mle_msg->data_ptr, mle_msg->data_length,MLE_TYPE_CHANNEL,&ptr)) {
        tr_error("Missing Channel TLV");
        return;
    }
    channel_page = ptr[0];
    channel = common_read_16_bit(&ptr[1]);

    if (linkConfiguration->timestamp == timestamp) {
        // We received same timestamp
        tr_debug("Same timestamp");
        return;
    }

    if (cur->thread_info->announcement_info && cur->thread_info->announcement_info->timestamp == timestamp){
        // We received same timestamp again
        tr_debug("Processing announce with same timestamp");
        return;
    }


    if (linkConfiguration->timestamp > timestamp) {
        // We received older time stamp we just announce back to originator channel
        thread_bootstrap_announce_send(cur, linkConfiguration->channel_page, linkConfiguration->rfChannel, linkConfiguration->panId, linkConfiguration->timestamp, channel);
        return;
    }

    tr_debug("New configuration received");
    thread_bootstrap_temporary_attach(cur,channel_page, channel, panid, timestamp);
}

static void thread_parse_data_response(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t linkMargin)
{
    thread_leader_data_t leaderData;
    mle_tlv_info_t networkDataTlv;
    mle_tlv_info_t ConfigurationTlv;
    uint64_t active_timestamp = 0;
    uint64_t pending_timestamp = 0;// means no pending timestamp
    mle_neigh_table_entry_t *entry_temp;
    bool accept_new_data = false;
    bool leaderDataReceived;

    tr_debug("Data Response");

    if (thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
        leaderDataReceived = true;
    } else {
        leaderDataReceived = false;
    }
    if (!leaderDataReceived) {
        tr_warn("no leader TLV");
        return;
    }

    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, false);

    if(cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER ||
            cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE) {
        if (thread_info(cur)->thread_attached_state == THREAD_STATE_CONNECTED) {
            // We are attached as child and just learn new data
            if (!thread_check_is_this_my_parent(cur, entry_temp)) {
                return;
            }
        } else {
            if (thread_info(cur)->thread_leader_data->partitionId != leaderData.partitionId) {
                // if receiving data response from different partition it is dropped
                return;
            }
        }
    } else if (thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE ||
            thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        if (!entry_temp) {
            tr_debug("Unknown neighbor");
            return;
        }
    }

    if (thread_info(cur)->thread_leader_data->partitionId != leaderData.partitionId) {
        thread_info(cur)->thread_leader_data->leaderRouterId = leaderData.leaderRouterId;
        thread_info(cur)->thread_leader_data->partitionId = leaderData.partitionId;
        thread_old_partition_data_purge(cur);
        accept_new_data = true;
    }

    //check response is for link metrics query
    thread_lowpower_process_response(mle_msg->packet_src_address, cur->id, mle_msg->data_ptr, mle_msg->data_length);

    if (cur->thread_info->networkDataRequested) {
        tr_debug("Requested network data received");
        cur->thread_info->networkDataRequested = false;
        accept_new_data = true;
    }
    if (1 == thread_leader_data_validation(cur, &leaderData, NULL)) {
        tr_debug("Network data updated");
        accept_new_data = true;
    }
    // update operational datasets

    mle_tlv_read_tlv(MLE_TYPE_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
    if (mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &active_timestamp)) {
        thread_active_operational_dataset_process(cur,ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen, active_timestamp);
    }
    // TODO check if result is true then need to update all configurations
    mle_tlv_read_tlv(MLE_TYPE_PENDING_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
    if (mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &pending_timestamp)) {
        thread_pending_operational_dataset_process(cur, pending_timestamp, ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen);
    }
    // Check if we are now in sync
    // if not matching must send data request again
    if (thread_joiner_application_active_timestamp_get(cur->id) < active_timestamp
            || thread_joiner_application_pending_config_timestamp_get(cur->id) < pending_timestamp) {
        tr_debug("Request new network data with configurations active %"PRIX64", %"PRIX64" Pending %"PRIX64", %"PRIX64,
                active_timestamp, thread_joiner_application_active_timestamp_get(cur->id),
                pending_timestamp, thread_joiner_application_pending_config_timestamp_get(cur->id));
        thread_network_data_request_send(cur, mle_msg->packet_src_address, true);
        return;
    }
    //Check Network Data TLV
    if (accept_new_data) {
        if (mle_tlv_read_tlv(MLE_TYPE_NETWORK_DATA, mle_msg->data_ptr, mle_msg->data_length, &networkDataTlv)) {
           thread_bootstrap_network_data_save(cur, &leaderData, networkDataTlv.dataPtr, networkDataTlv.tlvLen);
           thread_bootstrap_network_data_update(cur);
        } else {
            tr_debug("SET NWK data Request state");
        }
    }

    // leader has finished synching network data after reset/restart
    if (cur->thread_info->leader_synced) {
        cur->thread_info->leader_synced = false;
        thread_leader_service_network_data_changed(cur, true, true);
    }
}
static int thread_host_child_update_response_send(protocol_interface_info_entry_t *cur, uint8_t *dst_address, mle_tlv_info_t *challengeTlv, mle_tlv_info_t *requestTlv)
{
    uint16_t len = 150 + 64;
    uint8_t mode;
    uint32_t keySequence;
    uint8_t *ptr;
    if (!thread_info(cur)) {
        return -1;
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, len, false, MLE_COMMAND_CHILD_UPDATE_RESPONSE);

    if (bufId == 0) {
        return -1;
    }

    tr_debug("MLE Child update response");

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    ptr = mle_service_get_data_pointer(bufId);

    mode = thread_mode_get_by_interface_ptr(cur);

    //Write Mode Allways
    ptr = mle_tlv_write_mode(ptr, mode);
    //Set SRC
    ptr = mle_general_write_source_address(ptr, cur);
    //SET leader data
    ptr = thread_leader_data_tlv_write(ptr, cur);

    //Set Addresss TLV
    if (requestTlv && mle_tlv_requested(requestTlv->dataPtr,requestTlv->tlvLen,MLE_TYPE_ADDRESS_REGISTRATION) &&
          (mode & MLE_FFD_DEV) == 0) {
        ptr = thread_address_registration_tlv_write(ptr, cur);
    }

    if (requestTlv && mle_tlv_requested(requestTlv->dataPtr,requestTlv->tlvLen,MLE_TYPE_TIMEOUT)) {
        ptr = mle_tlv_write_timeout(ptr, cur->thread_info->host_link_timeout);
    }

    if (challengeTlv && challengeTlv->tlvLen) {
        ptr = mle_tlv_write_response(ptr, challengeTlv->dataPtr, challengeTlv->tlvLen);
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
        //SET MLE Frame Counter
        ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
    }

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, dst_address);
    mle_service_send_message(bufId);
    return 0;
}

static void thread_host_child_update_request_process(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t linkMargin)
{
    thread_leader_data_t leaderData;
    mle_tlv_info_t networkDataTlv;
    mle_tlv_info_t ConfigurationTlv;
    mle_tlv_info_t challengeTlv;
    mle_tlv_info_t tlv_req;
    uint64_t active_timestamp = 0;
    uint64_t pending_timestamp = 0;// means no pending timestamp
    mle_neigh_table_entry_t *entry_temp;
    bool data_request_needed = false;

    tr_debug("Child update request");
    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, false);

    if (!thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData) ||
        !entry_temp ||
        !thread_check_is_this_my_parent(cur, entry_temp)) {
        // Dropped if no leader data or not from parent
        tr_warn("invalid message");
        return;
    }
    mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv);
    mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &tlv_req);

    // Check if partition changed
    if (thread_info(cur)->thread_leader_data->partitionId != leaderData.partitionId) {
        thread_info(cur)->thread_leader_data->leaderRouterId = leaderData.leaderRouterId;
        thread_info(cur)->thread_leader_data->partitionId = leaderData.partitionId;
        thread_old_partition_data_purge(cur);
    }
    //Check Network Data TLV
    if (mle_tlv_read_tlv(MLE_TYPE_NETWORK_DATA, mle_msg->data_ptr, mle_msg->data_length, &networkDataTlv)) {
       thread_bootstrap_network_data_save(cur, &leaderData, networkDataTlv.dataPtr, networkDataTlv.tlvLen);
       thread_bootstrap_network_data_update(cur);
    }

    if (thread_info(cur)->thread_leader_data->stableDataVersion != leaderData.stableDataVersion ||
        thread_info(cur)->thread_leader_data->dataVersion != leaderData.dataVersion) {
        // version numbers not in sync need to send data request
        data_request_needed = true;
    }

    // update operational datasets
    mle_tlv_read_tlv(MLE_TYPE_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
    if (mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &active_timestamp)) {
        thread_active_operational_dataset_process(cur,ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen, active_timestamp);
    }
    mle_tlv_read_tlv(MLE_TYPE_PENDING_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
    if (mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &pending_timestamp)) {
        thread_pending_operational_dataset_process(cur, pending_timestamp, ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen);
    }
    // Check if operational datasets are in sync
    if (thread_joiner_application_active_timestamp_get(cur->id) < active_timestamp ||
        thread_joiner_application_pending_config_timestamp_get(cur->id) < pending_timestamp) {
        tr_debug("Request new network data with configurations active %"PRIX64", %"PRIX64" Pending %"PRIX64", %"PRIX64,
                active_timestamp, thread_joiner_application_active_timestamp_get(cur->id),
                pending_timestamp, thread_joiner_application_pending_config_timestamp_get(cur->id));
        data_request_needed = true;
    }
    thread_host_child_update_response_send(cur, mle_msg->packet_src_address, &challengeTlv, &tlv_req);

    if (data_request_needed) {
        thread_network_data_request_send(cur, mle_msg->packet_src_address, true);
    }
}

static void thread_parse_child_update_response(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin)
{
    uint8_t mode;
    uint32_t timeout;
    mle_neigh_table_entry_t *entry_temp;
    thread_leader_data_t leaderData = {0};
    uint8_t status;
    bool leader_data_received;

    tr_debug("Child Update Response");

    leader_data_received = thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData);
    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, mle_msg->packet_src_address, false);

    if (mle_tlv_read_8_bit_tlv(MLE_TYPE_STATUS, mle_msg->data_ptr, mle_msg->data_length, &status) &&
        status == 1 && thread_check_is_this_my_parent(cur, entry_temp)) {
        tr_debug("parent has connection error");
        thread_bootstrap_connection_error(cur->id, CON_PARENT_CONNECT_DOWN, NULL);
        return;
    }

    if (!entry_temp) {
        tr_debug("Not Neighbor");
        mle_tlv_info_t challengeTlv;
        mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv);
        thread_host_bootstrap_child_update_negative_response(cur, mle_msg->packet_src_address, &challengeTlv);
        return;
    }

    if ((security_headers->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX)
            && (thread_instance_id_matches(cur, &leaderData))) {
        thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));
    } else {
        tr_debug("Key ID Mode 2 not used; dropped.");
        return;
    }

    if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, mle_msg->data_ptr, mle_msg->data_length, &mode)) {
        tr_debug("No Mode");
        return;
    }

    if (mle_tlv_read_32_bit_tlv(MLE_TYPE_TIMEOUT, mle_msg->data_ptr, mle_msg->data_length, &timeout)) {
        entry_temp->holdTime = 90;
        tr_debug("Setting child timeout, value=%"PRIu32, timeout);
        mle_entry_timeout_update(entry_temp, timeout);
        thread_info(cur)->thread_endnode_parent->childUpdateProcessStatus = true;
    }
    tr_debug("Keep-Alive -->Respond from Parent");
    mle_entry_timeout_refresh(entry_temp);

    //Save possible new Leader Data
    if (leader_data_received) {
        thread_save_leader_data(cur, &leaderData);
    }
    mac_data_poll_protocol_poll_mode_decrement(cur);
}

#endif
