/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file thread_host_bootstrap.c
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
#include "shalib.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_lowpower_private_api.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "MPL/mpl.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "Core/include/address.h"

#define TRACE_GROUP "tebs"

static bool thread_child_update_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRerties);
static bool thread_parent_discover_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries);
static bool thread_child_id_req_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries);

static void thread_child_synch_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);
static void thread_mle_child_request_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);

static int thread_parent_request_build(protocol_interface_info_entry_t *cur);

static int thread_attach_child_id_request_build(protocol_interface_info_entry_t *cur);
static int thread_end_device_synch_response_validate(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t data_length, uint8_t linkMargin, uint8_t *src_address, mle_security_header_t *securityHeader);

static uint8_t *thread_single_address_registration_tlv_write(uint8_t *ptr, lowpan_context_t *ctx, uint8_t *addressPtr);
static int8_t thread_end_device_synch_start(protocol_interface_info_entry_t *cur);


void thread_child_set_default_route(protocol_interface_info_entry_t *cur)
{
    thread_parent_info_t *parent = cur->thread_info->thread_endnode_parent;

    if (!parent) {
        tr_debug("child default route set fail");
        return;
    }
    cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
    uint8_t ula[16];
    // SET Default route to ::/0
    memcpy(ula, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&ula[8], parent->mac64 , 8);
    ula[8] ^= 2;
    if (ipv6_route_add(NULL, 0, cur->id, ula, ROUTE_THREAD, 0xffffffff, 0) == NULL) {
        tr_error("fail to add default route");
    }
}

static void thread_network_data_clean(protocol_interface_info_entry_t *cur)
{
    tr_debug("Clean network data");
    thread_network_data_router_id_mark_delete(&cur->thread_info->networkDataStorage, 0xffff, false);
    thread_network_data_router_id_free(&cur->thread_info->networkDataStorage, false, cur);
}

static void thread_merge_prepare(protocol_interface_info_entry_t *cur)
{
    thread_clean_old_16_bit_address_based_addresses(cur);
    mpl_clear_realm_scope_seeds(cur);
    ipv6_neighbour_cache_flush(&cur->ipv6_neighbour_cache);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_PROXIED_HOST, NULL);
    thread_routing_deactivate(&cur->thread_info->routing);
    thread_routing_init(&cur->thread_info->routing);
    cur->nwk_mode = ARM_NWK_GP_IP_MODE;
    thread_network_data_clean(cur);
}

//This function is for Thread Parent scan callback
static bool thread_parent_discover_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }

    cur->nwk_nd_re_scan_count++;

    tr_debug("Parse parent scan result");
    if (cur->thread_info->thread_attach_scanned_parent) {
        uint8_t ll64[16];
        thread_scanned_parent_t *parent = cur->thread_info->thread_attach_scanned_parent;
        link_configuration_s *linkConfiguration;
        mle_neigh_table_entry_t *entry_temp;

        linkConfiguration = thread_joiner_application_get_config(interface_id);
        if (!linkConfiguration) {
            return false;
        }

        /* SED requires that scanned parent is added to MLE table */
        memcpy(ll64, ADDR_LINK_LOCAL_PREFIX , 8);
        memcpy(&ll64[8], parent->mac64 , 8);
        ll64[8] ^= 2;

        entry_temp = mle_class_get_entry_by_ll64(interface_id, parent->linkMarginToParent,ll64, true);
        if (entry_temp == NULL) {
            return false;
        }
        entry_temp->threadNeighbor = true;
        entry_temp->short_adr = parent->shortAddress;
        entry_temp->priorityFlag = true;
        entry_temp->holdTime = 90;
        entry_temp->mle_frame_counter = parent->mleFrameCounter;

        thread_management_key_sets_calc(cur, linkConfiguration, cur->thread_info->thread_attach_scanned_parent->keySequence);
        thread_calculate_key_guard_timer(cur, linkConfiguration, true);

        mac_helper_devicetable_set(entry_temp, cur, parent->linLayerFrameCounter, mac_helper_default_key_index_get(cur));
        mle_entry_timeout_update(entry_temp, THREAD_DEFAULT_LINK_LIFETIME);

        if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
            nwk_thread_host_control(cur, NET_HOST_FAST_POLL_MODE, 50);
        }

        tr_debug("Parent found, send child ID req");
        thread_attach_child_id_request_build(cur);
        return false;

    } else {
        uint8_t *dataPtr;
        uint16_t dataLength;

        if (usedAllRetries) {
            if (thread_is_connected(cur)) {
                // Did not find any better partition to join. Be happy.
                cur->nwk_bootstrap_state = ER_MLE_ATTACH_READY;
                return true;
            }
            // Trig new state which call this
            thread_bootstrap_connection_error(cur->id, CON_ERROR_NETWORK_ATTACH_FAIL, NULL);
            return false;
        }

        //GET Data pointer
        dataPtr = mle_service_get_payload_start_point(msgId);
        dataLength = mle_service_get_payload_length(msgId);

        if (dataPtr) {
            mle_tlv_info_t tlv_info;
            //Scan MLE_TYPE_SCAN_MASK
            if (mle_tlv_option_discover(dataPtr, dataLength, MLE_TYPE_SCAN_MASK, &tlv_info)) {
                dataPtr = tlv_info.dataPtr;
                *dataPtr |= 0x40; //ADD REED Bit
            }
        }
    }
    return true;
}

static int thread_parent_request_build(protocol_interface_info_entry_t *cur)
{
    uint8_t *ptr;
    uint8_t mode;
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint8_t scanMask = 0x80;

    mle_service_interface_tx_queue_clean(cur->id);

    uint16_t buf_id = mle_service_msg_allocate(cur->id, 32, true, MLE_COMMAND_PARENT_REQUEST);
    if (buf_id == 0) {
        return -1;
    }

    if (cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH ||
        cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH_RETRY ||
        cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED ||
        cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        // When doing re-attach End devices are immediately accepted as parents
        scanMask |= 0x40;
    }
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);

    //SET Multicast to all Router ff02::2
    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_ROUTERS);
    tr_debug("Thread parent request");

    ptr = mle_service_get_data_pointer(buf_id);
    ptr = mle_tlv_write_scan_mask(ptr, scanMask);
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    mode = thread_mode_get_by_interface_ptr(cur);

    /* If we are a SLEEPY host, we do NOT set RX_ON_IDLE bit in parent requests */
    /*   NOTE: the RX_ON_IDLE is temporarily set on the interface during bootstrap */
    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        mode &= ~MLE_RX_ON_IDLE;
    }

    ptr = mle_tlv_write_mode(ptr, mode);
    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = THREAD_PARENT_REQUEST_MAX_RETRY_CNT;
    timeout.timeout_init = 1;
    timeout.timeout_max = 2;
    timeout.delay = MLE_NO_DELAY;
    cur->nwk_nd_re_scan_count = 1;
    mle_service_set_packet_callback(buf_id, thread_parent_discover_timeout_cb);
    if (cur->thread_info->thread_attached_state == THREAD_STATE_NETWORK_DISCOVER) {
        mle_service_interface_receiver_handler_update(cur->id, thread_mle_parent_discover_receive_cb);
    } else {
        mle_service_interface_receiver_handler_update(cur->id, thread_general_mle_receive_cb);
    }

    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_send_message(buf_id);
    return 0;
}

void thread_network_attach_start(protocol_interface_info_entry_t *cur)
{
    if (thread_parent_request_build(cur) == 0) {
        tr_debug("MLE Parent request");
        cur->nwk_bootstrap_state = ER_MLE_SCAN;
        cur->bootsrap_state_machine_cnt = 0;
    } else {
        cur->bootsrap_state_machine_cnt = 5;
    }
}

static int thread_end_device_synch_response_validate(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t data_length, uint8_t linkMargin, uint8_t *src_address, mle_security_header_t *securityHeader)
{
    uint8_t shortAddress[2];
    uint8_t status, mode;
    uint16_t srcAddress;
    uint16_t address16;
    uint32_t llFrameCounter;
    thread_leader_data_t leaderData;
    mle_neigh_table_entry_t *entry_temp;

    tr_debug("Validate Link Synch Response");
    //Check First Status
    if (mle_tlv_read_8_bit_tlv(MLE_TYPE_STATUS, ptr, data_length, &status)) {
        tr_debug("Synch status response %x",status);
        //Clean synch state and start
        return -1;
    }
    // Validate response
    // MLE_TYPE_MODE
    // Address
    // MLE_TYPE_SRC_ADDRESS
    // MLE_TYPE_LEADER_DATA
    if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, ptr, data_length, &mode) ||
            !mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, ptr, data_length, &srcAddress) ||
            !mle_tlv_read_16_bit_tlv(MLE_TYPE_ADDRESS16, ptr, data_length, &address16) ||
            !thread_leader_data_parse(ptr, data_length, &leaderData) ||
            !mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, ptr, data_length, &llFrameCounter)) {
        tr_debug("missing TLV's");
        return -1;
    }

    if (securityHeader->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
        thread_management_key_synch_req(cur->id, common_read_32_bit(securityHeader->Keysource));
    } else {
        tr_debug("Key ID Mode 2 not used; dropped.");
        return -3;
    }

    //Update parent link information
    entry_temp = mle_class_get_entry_by_ll64(cur->id, linkMargin, src_address, true);

    if (!entry_temp) {
        tr_debug("Neighbor allocate fail");
        return -2;
    }
/*

*/
    entry_temp->threadNeighbor = true;
    entry_temp->short_adr = srcAddress;
    entry_temp->handshakeReady = 1;
    entry_temp->holdTime = 90;
    entry_temp->priorityFlag = true; // Make this our parent
    common_write_16_bit(entry_temp->short_adr, shortAddress);

    mac_helper_coordinator_address_set(cur, ADDR_802_15_4_SHORT, shortAddress);
    mle_entry_timeout_update(entry_temp, thread_info(cur)->host_link_timeout);
    mac_helper_devicetable_set(entry_temp, cur, llFrameCounter, securityHeader->KeyIndex);

    thread_info(cur)->thread_attached_state = THREAD_STATE_CONNECTED;
    thread_bootstrap_update_ml16_address(cur, address16);
    //SET Parent Info and free scan response info
    thread_info(cur)->thread_endnode_parent->shortAddress = srcAddress;
    thread_info(cur)->thread_endnode_parent->router_id = srcAddress >> 10;
    memcpy(thread_info(cur)->thread_endnode_parent->mac64, entry_temp->mac64, 8);

    mle_tlv_info_t routing;
    if (mle_tlv_read_tlv(MLE_TYPE_ROUTE, ptr, data_length, &routing)) {
        thread_router_bootstrap_route_tlv_push(cur, routing.dataPtr, routing.tlvLen, linkMargin, entry_temp);
    }

    //Copy Leader Data
    *thread_info(cur)->thread_leader_data = leaderData;

    /*save the data version to one less than what was received so that any inconsistencies in network data is
     * fixed by a data request to parent
     */
    thread_info(cur)->thread_leader_data->stableDataVersion = leaderData.stableDataVersion - 1;
    thread_info(cur)->thread_leader_data->dataVersion = leaderData.dataVersion - 1;


    return 0;

}

static void thread_child_synch_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    tr_debug("Thread MLE message child_synch handler");
    //State machine What packet shuold accept in this case
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    /* Check that message is from link-local scope */
    if(!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    uint16_t messageId;
    uint8_t linkMargin = thread_compute_link_margin(mle_msg->dbm);
    //State machine What packet shuold accept in this case
    switch (mle_msg->message_type) {
    case MLE_COMMAND_CHILD_UPDATE_RESPONSE:

        messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

        if (messageId == 0) {
            tr_debug("Not for me");
            return;
        }

        if (thread_end_device_synch_response_validate(cur, mle_msg->data_ptr, mle_msg->data_length, linkMargin, mle_msg->packet_src_address, security_headers) != 0) {
            tr_warn("End device synch failed");
            mle_service_msg_free(messageId);
            thread_bootsrap_device_synch_fail(cur);
            return;
        }
        mle_tlv_info_t networkDataTlv;
        thread_leader_data_t leaderData;
        tr_debug("End device synch Possible");

        cur->thread_info->thread_attached_state = THREAD_STATE_CONNECTED;
        // read network data, and leader data check. Send data request sent if pending set is not in sync
        if (mle_tlv_read_tlv(MLE_TYPE_NETWORK_DATA, mle_msg->data_ptr, mle_msg->data_length, &networkDataTlv) &&
            thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData) && thread_joiner_application_pending_delay_timer_in_sync(cur->id)) {
            thread_bootstrap_network_data_save(cur, &leaderData, networkDataTlv.dataPtr, networkDataTlv.tlvLen);
        } else {
            thread_bootstrap_parent_network_data_request(cur, true);
        }
        thread_bootstrap_attached_ready(cur);

        //SET For sleepy state and mode update
        if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
            nwk_thread_host_control(cur, NET_HOST_FAST_POLL_MODE, 1);
            //TRIG Child Update state
            thread_bootstrap_child_update_trig(cur);
        }

        mle_service_msg_free(messageId);
        break;
    }
}

static bool thread_host_prefer_parent_response(protocol_interface_info_entry_t *cur, thread_scanned_parent_t *scanned_parent, uint16_t version, thread_connectivity_t *connectivity)
{
    (void) connectivity;
    (void) cur;

    if (!thread_extension_version_check(thread_info(cur)->version)) {
        return false;
    }

    if (version < scanned_parent->version) {
        tr_debug("prefer existing parent response from %"PRIX16, scanned_parent->shortAddress);
        return true;
    }

    return false;
}

/* MLE callback for parent response messages.
 *
 * The Parent Response contains these TLVs:
 * - Source Address TLV
 * - Leader Data TLV
 * - Link-layer Frame Counter TLV
 * - MLE Frame Counter TLV (optional)
 * - Response TLV
 * - Challenge TLV
 * - Link Margin TLV
 * - Connectivity TLV
 * - Version TLV
 */
void thread_mle_parent_discover_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    /* Check that message is from link-local scope */
    if(!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    tr_debug("Thread MLE Parent request response Handler");
    //State machine What packet should accept in this case
    switch (mle_msg->message_type) {
        case MLE_COMMAND_PARENT_RESPONSE: {
            thread_scanned_parent_t *scan_result = NULL;
            uint16_t messageId;
            uint16_t srcAddress, version;
            uint32_t llFrameCounter;
            uint32_t mleFrameCounter;
            uint8_t rssiValue;
            mle_tlv_info_t challengeTlv;
            uint8_t currentMargin, newMargin;
            thread_link_quality_e newLqi;
            thread_leader_data_t leaderData;
            thread_connectivity_t connectivityTlv;
            bool accept_response;

            tr_info("Parent Response");
            if (security_headers->KeyIdMode != MAC_KEY_ID_MODE_SRC4_IDX) {
                tr_debug("Wrong key mode %u ", security_headers->KeyIdMode);
                return;
            }
            messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

            if (messageId == 0) {
                tr_debug("Not for me");
                return;
            }

            //Read Leader Data and verify connectivity
            if (!thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
                return;
            }
            if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_RSSI, mle_msg->data_ptr, mle_msg->data_length, &rssiValue)) {
                return;
            }
            if (!thread_connectivity_tlv_parse(mle_msg->data_ptr, mle_msg->data_length, &connectivityTlv)) {
                return;
            }
            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &srcAddress)) {
                return;
            }
            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) {
                return;
            }
            if (!mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &llFrameCounter)) {
                return;
            }
            if (!mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv)) {
                return;
            }
            //If MLE MLE_TYPE_MLE_FRAME_COUNTER TLV is present then use it for validating further messages else use link layer frame counter
            if (!mle_tlv_read_32_bit_tlv(MLE_TYPE_MLE_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &mleFrameCounter)) {
                mleFrameCounter = llFrameCounter;
            }

            /**
             * At THREAD_STATE_REATTACH state only accept same Partition ID and Higher ID than current
             */
            if (thread_info(cur)->thread_attached_state == THREAD_STATE_REATTACH || thread_info(cur)->thread_attached_state == THREAD_STATE_REATTACH_RETRY) {
                tr_debug("Reattach");
                if (thread_info(cur)->thread_leader_data) {
                    if ((thread_info(cur)->thread_leader_data->partitionId != leaderData.partitionId) ||
                        (thread_info(cur)->thread_leader_data->weighting != leaderData.weighting)) {
                        //accept only same ID at reattach phase
                        return;
                    }
                    //Compare ID - when downgrading, accept all
                    if (!thread_info(cur)->releaseRouterId) {
                        if (!common_serial_number_greater_8(connectivityTlv.idSequence, thread_info(cur)->routing.router_id_sequence)) {
                            tr_debug("Drop old partition by old ID");
                            return;
                        } else {
                            tr_debug("Accept old partition by new ID");
                        }
                    }
                }
            } else if (thread_info(cur)->thread_attached_state == THREAD_STATE_ATTACH_ANY ||
                    thread_info(cur)->thread_attached_state == THREAD_STATE_CONNECTED ||
                    thread_info(cur)->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
                if (thread_info(cur)->thread_leader_data) {
                    if ((thread_info(cur)->thread_leader_data->partitionId == leaderData.partitionId) &&
                        (thread_info(cur)->thread_leader_data->weighting == leaderData.weighting)) {
                        //accept only different ID at anyattach phase
                        tr_debug("Drop old partition");
                        return;
                    }
                } else {
                    //TODO Fix this
                    tr_error("No leader Data allocated for AnyAttach,Why?");
                }
            }

            if (blacklist_reject(mle_msg->packet_src_address)) {
                tr_debug("Drop Parent Response because blacklisted");
                return;
            }

            /* Calculate this parent link margin */
            newMargin = thread_calculate_link_margin(mle_msg->dbm, rssiValue);
            newLqi = thread_link_margin_to_quality(newMargin << THREAD_LINK_MARGIN_SCALING);
            tr_debug("Parent scan count %d addr:%04x Margin:%d  lqi:%d", cur->nwk_nd_re_scan_count, srcAddress, newMargin, newLqi);
            if (newLqi == QUALITY_BAD) {
                tr_debug("Drop Bad Link");
                accept_response = false;
            } else {
                if (cur->nwk_nd_re_scan_count <= 1) {
                    if (newLqi == QUALITY_20dB) {
                        accept_response = true;
                    } else {
                        tr_debug("Drop first time less than QUALITY_20dB");
                        accept_response = false;
                    }
                } else {
                    accept_response = true;
                }
            }

            if (thread_extension_enabled(cur) &&
                thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_ROUTER &&
                leaderData.weighting < thread_info(cur)->partition_weighting) {
                // Only applies to extensions and only routers that can form new partitions can ignore lower weight
                tr_debug("Drop parent due weighting %d<%d", leaderData.weighting, thread_info(cur)->partition_weighting);
                return;
            }

            if (accept_response) {
                if (thread_info(cur)->thread_attach_scanned_parent == NULL) {
                    thread_info(cur)->thread_attach_scanned_parent = ns_dyn_mem_temporary_alloc(sizeof(thread_scanned_parent_t));
                    if (!thread_info(cur)->thread_attach_scanned_parent) {
                        return;
                    }
                    thread_info(cur)->thread_attach_scanned_parent->child_id_request_id = 0;
                    scan_result = thread_info(cur)->thread_attach_scanned_parent;
                    tr_debug("Partition %"PRIu32, leaderData.partitionId);
                } else {
                    uint32_t currentPartitionId = thread_info(cur)->thread_attach_scanned_parent->leader_data.partitionId;
                    uint8_t currentWeighting = thread_info(cur)->thread_attach_scanned_parent->leader_data.weighting;
                    tr_debug("Current partition %"PRIu32" old:%"PRIu32" weighting %"PRIu8" old:%"PRIu8,
                        currentPartitionId, leaderData.partitionId, currentWeighting, leaderData.weighting);

                    if ((leaderData.partitionId != currentPartitionId) ||
                        (leaderData.weighting != currentWeighting)) {
                        int retVal = thread_bootstrap_partition_process(cur, connectivityTlv.activeRouters, &leaderData,NULL);
                        if (retVal > 0) {
                            // New partition is Higher
                            scan_result = thread_info(cur)->thread_attach_scanned_parent;
                        }
                    }
                    else if (leaderData.partitionId == currentPartitionId) {
                        thread_link_quality_e currentLqi;
                        //Calculate Current summed LQI
                        scan_result = thread_info(cur)->thread_attach_scanned_parent;
                        currentMargin = thread_parent_margin_calc(scan_result->linkMarginFromParent, scan_result->linkMarginToParent);
                        currentLqi = thread_link_margin_to_quality(currentMargin << THREAD_LINK_MARGIN_SCALING);
                        tr_debug("Compare LQI from margins:");
                        tr_debug("New Margin %d vs Cur %d", newMargin, currentMargin);
                        tr_debug("New Lqi %d vs Cur %d", newLqi, currentLqi);
                        if (newLqi > currentLqi) {
                            /*Override old parent data*/
                            tr_debug("Better Parent Lqi");
                        } else {
                            if (newLqi == currentLqi) {
                                //Compare if REED
                                if (thread_is_router_addr(scan_result->shortAddress) &&
                                        !thread_is_router_addr(srcAddress) ) {
                                    scan_result = NULL;
                                    tr_debug("Dropped Response from REED over router");
                                } else if (connectivityTlv.parentPriority > scan_result->parentPriority) {
                                    tr_debug("Better parent priority %d>%d", connectivityTlv.parentPriority, scan_result->parentPriority);
                                } else if (connectivityTlv.parentPriority < scan_result->parentPriority) {
                                    tr_debug("Dropped Response - lower parent priority %d<%d", connectivityTlv.parentPriority, scan_result->parentPriority);
                                    scan_result = NULL;
                                } else {
                                    tr_debug("Similar LQI check connectivity old: %d,%d,%d new:%d,%d,%d",
                                            scan_result->linkQuality3, scan_result->linkQuality2, scan_result->linkQuality1,
                                            connectivityTlv.linkQuality3, connectivityTlv.linkQuality2, connectivityTlv.linkQuality1);

                                    if (scan_result->linkQuality3 > connectivityTlv.linkQuality3) {
                                        scan_result = NULL;
                                    }
                                    if (scan_result &&
                                            scan_result->linkQuality3 == connectivityTlv.linkQuality3 &&
                                            scan_result->linkQuality2 > connectivityTlv.linkQuality2) {
                                        scan_result = NULL;
                                    }
                                    if (scan_result &&
                                            scan_result->linkQuality3 == connectivityTlv.linkQuality3 &&
                                            scan_result->linkQuality2 == connectivityTlv.linkQuality2 &&
                                            scan_result->linkQuality1 > connectivityTlv.linkQuality1) {
                                        scan_result = NULL;
                                    }
                                    if(!scan_result) {
                                        tr_debug("Dropped Connectivity is not as good");
                                    }
                                }
                            } else {
                                scan_result = NULL;
                            }
                        }
                    }
                    if (scan_result && thread_host_prefer_parent_response(cur, scan_result, version, &connectivityTlv)){
                        scan_result = NULL;
                    }

                }
            }

            if (scan_result) {
                // save the result
                thread_leader_data_t *leader;
                scan_result->linkMarginToParent = thread_compute_link_margin(mle_msg->dbm);
                scan_result->linkMarginFromParent = rssiValue;
                scan_result->linLayerFrameCounter = llFrameCounter;
                scan_result->mleFrameCounter = mleFrameCounter;
                scan_result->shortAddress = srcAddress;
                scan_result->version = version;
                memcpy(scan_result->challengeData, challengeTlv.dataPtr, challengeTlv.tlvLen);
                scan_result->chal_len = challengeTlv.tlvLen;
                //Save MAC address from LL64
                memcpy(scan_result->mac64, (mle_msg->packet_src_address + 8), 8);
                scan_result->mac64[0] ^= 2;
                leader = &scan_result->leader_data;
                //Copy Leader Data
                *leader = leaderData;
                scan_result->routeCostToLeader = connectivityTlv.leaderCost;
                scan_result->linkQuality3 = connectivityTlv.linkQuality3;
                scan_result->linkQuality2 = connectivityTlv.linkQuality2;
                scan_result->linkQuality1 = connectivityTlv.linkQuality1;
                scan_result->parentPriority = connectivityTlv.parentPriority;
                scan_result->activeRouters = connectivityTlv.activeRouters;
                scan_result->security_key_index = security_headers->KeyIndex;

                thread_routing_update_link_margin(cur, scan_result->shortAddress, scan_result->linkMarginToParent, scan_result->linkMarginFromParent);
                scan_result->keySequence = common_read_32_bit(security_headers->Keysource);
                tr_debug("Current %"PRIu32" RX %"PRIu32" Cnt%"PRIu32, scan_result->leader_data.partitionId, leaderData.partitionId, scan_result->linLayerFrameCounter);
            }
             break;
        }
        default:
            break;
    }

}

/* Callback for child ID request.
 *
 * Child ID Response contains these TLVs:
 *   - Source Address TLV
 *   - Leader Data TLV
 *   - Address16 TLV
 *   - Network Data TLV (optional)
 *   - Route64 TLV (optional)
 *   - Address Registration TLV (optional)
 *   - Active Operational Dataset TLV (optional)
 *   - Pending Operational Dataset TLV (optional)
*/
static void thread_mle_child_request_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    thread_leader_data_t leaderData;
    mle_neigh_table_entry_t *entry_temp;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    link_configuration_s *link_configuration;
    if (!cur) {
        return;
    }
    link_configuration = thread_joiner_application_get_config(cur->id);
    if (!link_configuration) {
        return;
    }

    /* Check that message is from link-local scope */
    if(!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    tr_debug("Thread MLE Child request response Handler");

    switch (mle_msg->message_type) {

        case MLE_COMMAND_CHILD_ID_RESPONSE: {
            uint8_t shortAddress[2];
            uint16_t childId;
            mle_tlv_info_t routeTlv, addressRegisteredTlv, networkDataTlv;
            mle_tlv_info_t ConfigurationTlv;
            uint64_t pending_timestamp = 0;
            uint64_t active_timestamp;
            thread_scanned_parent_t *scan_result = thread_info(cur)->thread_attach_scanned_parent;

            tr_info("Received Child ID Response");

            // Clear old data
            if (cur->thread_info->releaseRouterId) {
                thread_bootstrap_clear_neighbor_entries(cur);
                cur->thread_info->localServerDataBase.release_old_address = true;
            }

            thread_clean_all_routers_from_neighbor_list(cur->id);
            thread_leader_service_stop(interface_id);
            thread_leader_service_leader_data_free(cur->thread_info);
            thread_merge_prepare(cur);

            // Create entry for new parent
            entry_temp = mle_class_get_entry_by_ll64(cur->id, thread_compute_link_margin(mle_msg->dbm), mle_msg->packet_src_address, true);
            if (entry_temp == NULL) {
                // todo: what to do here?
                return;
            }

            //Parse mandatory TLV's
            if (!thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
                return;
            }
            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &entry_temp->short_adr)) {
                return;
            }

            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_ADDRESS16, mle_msg->data_ptr, mle_msg->data_length, &childId)) {
                return;
            }

            //Read Optional TLV's
            mle_tlv_read_tlv(MLE_TYPE_ADDRESS_REGISTRATION, mle_msg->data_ptr, mle_msg->data_length, &addressRegisteredTlv);
            mle_tlv_read_tlv(MLE_TYPE_NETWORK_DATA, mle_msg->data_ptr, mle_msg->data_length, &networkDataTlv);
            mle_tlv_read_tlv(MLE_TYPE_ROUTE, mle_msg->data_ptr, mle_msg->data_length, &routeTlv);
            // update operational datasets
            mle_tlv_read_tlv(MLE_TYPE_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
            if (mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &active_timestamp)) {
                thread_active_operational_dataset_process(cur, ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen, active_timestamp);
            }
            // TODO check if result is true then need to update all configurations
            mle_tlv_read_tlv(MLE_TYPE_PENDING_OPERATIONAL_DATASET, mle_msg->data_ptr, mle_msg->data_length, &ConfigurationTlv);
            if (mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &pending_timestamp)) {
                thread_pending_operational_dataset_process(cur, pending_timestamp, ConfigurationTlv.dataPtr, ConfigurationTlv.tlvLen);
            } else if (pending_timestamp < thread_joiner_application_pending_config_timestamp_get(cur->id)) {
                // parent did not have timestamp but we haver
                tr_info("save pending set for leader");
                thread_joiner_application_next_pending_config_save(cur->id);
                thread_joiner_application_pending_config_delete(cur->id);
            }

            *thread_info(cur)->thread_leader_data = leaderData;

            thread_parent_info_t *parent = thread_parent_data_allocate(cur->thread_info);
            if (!parent) {
                tr_debug("Parent allocate fail.");
                return;
            }

            common_write_16_bit(entry_temp->short_adr, shortAddress);
            //Update possible reed address by real router address
            scan_result->shortAddress = entry_temp->short_adr;

            entry_temp->holdTime = 90;
            entry_temp->handshakeReady = 1;
            entry_temp->priorityFlag = true;
            entry_temp->threadNeighbor = true;

            mac_helper_coordinator_address_set(cur, ADDR_802_15_4_SHORT, shortAddress);
            mle_entry_timeout_update(entry_temp, thread_info(cur)->host_link_timeout);
            mac_helper_devicetable_set(entry_temp, cur, scan_result->linLayerFrameCounter, security_headers->KeyIndex);

            thread_info(cur)->thread_attached_state = THREAD_STATE_CONNECTED;

            thread_bootstrap_update_ml16_address(cur, childId);
            if (!thread_is_router_addr(thread_info(cur)->routerShortAddress)) {
                thread_info(cur)->routerShortAddress = 0xfffe;
            }

            mle_service_msg_free(scan_result->child_id_request_id);

            scan_result->child_id_request_id = 0;
            //SET Parent Info and free scan response info
            parent->pathCostToLeader = thread_sum_rx_path_cost_and_link_cost(scan_result->linkMarginToParent, scan_result->linkMarginFromParent, scan_result->routeCostToLeader);
            parent->shortAddress = scan_result->shortAddress;
            parent->router_id = (scan_result->shortAddress >> 10);
            memcpy(parent->mac64, scan_result->mac64, 8);
            //Check Network Data TLV
            if (networkDataTlv.tlvType == MLE_TYPE_NETWORK_DATA) {
                thread_bootstrap_network_data_save(cur, &leaderData, networkDataTlv.dataPtr, networkDataTlv.tlvLen);
            } else {
                cur->thread_info->thread_leader_data->dataVersion--;
                cur->thread_info->thread_leader_data->stableDataVersion--;
            }

            blacklist_update(mle_msg->packet_src_address, true);

            //
            if (routeTlv.tlvType == MLE_TYPE_ROUTE && routeTlv.tlvLen) {
                thread_router_bootstrap_route_tlv_push(cur, routeTlv.dataPtr, routeTlv.tlvLen , thread_compute_link_margin(mle_msg->dbm), entry_temp);
            }
            thread_bootstrap_attached_ready(cur);

            break;
        }
        default:
            tr_debug("Unsupported TLV %d", mle_msg->message_type);
            break;

    }
}

static int8_t thread_end_device_synch_start(protocol_interface_info_entry_t *cur)
{
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, 150 + 3 + 6 + 10, true,MLE_COMMAND_CHILD_UPDATE_REQUEST);
    if (buf_id == 0) {
        return -1;
    }
    uint8_t tlv_req[3];

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(buf_id);
    //Set Parent Address
    memcpy(address_ptr, ADDR_LINK_LOCAL_PREFIX, 8);
    address_ptr += 8;
    memcpy(address_ptr, cur->thread_info->thread_endnode_parent->mac64, 8);
    *address_ptr ^= 2;
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    //Add mode
    ptr = mle_tlv_write_mode(ptr, thread_mode_get_by_interface_ptr(cur));
    ptr = thread_address_registration_tlv_write(ptr, cur);
    tlv_req[0] = MLE_TYPE_ADDRESS16;
    tlv_req[1] = MLE_TYPE_NETWORK_DATA;

    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        // REEDs request Route64 as well
        tlv_req[2] = MLE_TYPE_ROUTE;
        ptr = mle_tlv_req_tlv(ptr, tlv_req, 3);
    } else {
        ptr = mle_tlv_req_tlv(ptr, tlv_req, 2);
    }

    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    tr_debug("Send MLE Child Update Request (Child synchronisation)");
    timeout.retrans_max = THREAD_REQUEST_MAX_RETRY_CNT;
    timeout.timeout_init = 1;
    timeout.timeout_max = 3;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_packet_callback(buf_id, thread_device_synch_timeout);
    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_send_message(buf_id);
    return 0;
}

void thread_endevice_synch_start(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->thread_endnode_parent) {
        mle_neigh_table_entry_t *entry_temp;

        // Add the parent to the MLE neighbor table
        entry_temp = mle_class_get_entry_by_mac64(cur->id, 64, cur->thread_info->thread_endnode_parent->mac64, true);

        if (entry_temp) {
            entry_temp->short_adr = cur->thread_info->thread_endnode_parent->shortAddress;
            entry_temp->handshakeReady = 1;
            entry_temp->threadNeighbor = true;

            // In case we don't get response to sync; use temporary timeout here,
            // Child ID Response handler will set correct value later
            mle_entry_timeout_update(entry_temp, 20);

            // Add the parent to the MAC table (for e.g. secured/fragmented Child Update Response)
            mac_helper_devicetable_set(entry_temp, cur, 0, cur->mac_parameters->mac_default_key_index);
        }
    }

    //Send Child Synch
    if (thread_end_device_synch_start(cur) == 0) {
        //SET Child synch receiver handler
        mle_service_interface_receiver_handler_update(cur->id, thread_child_synch_receive_cb);
        cur->nwk_bootstrap_state = ER_MLE_SYNCH;
    }
}

static bool thread_child_id_req_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    mle_neigh_table_entry_t *entry_temp;
    thread_scanned_parent_t *scanned_parent;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    uint8_t ll64[16];

    (void)msgId;

    if (!cur) {
        return false;
    }

    // Not last retry - keep trying
    if (!usedAllRetries) {
        return true;
    }

    //Do not release message inside this call MLE service will do that.
    cur->thread_info->thread_attach_scanned_parent->child_id_request_id = 0;

    uint8_t *addr = mle_service_get_msg_destination_address_pointer(msgId);
    tr_debug("Child ID Request timed out, address: %s", trace_ipv6(addr));

    blacklist_update(addr, false);

    scanned_parent = cur->thread_info->thread_attach_scanned_parent;

    /* Used all retries - if we are on any attach state or */
    /* if we don't have route to leader, start any-attach */
    if (thread_info(cur)->thread_attached_state == THREAD_STATE_NETWORK_DISCOVER ||
            -1 == thread_route_ready_to_leader(cur) || !scanned_parent) {
        tr_debug("start any-attach");
        thread_bootstrap_connection_error(interface_id, CON_ERROR_NETWORK_ATTACH_FAIL, NULL);
        goto exit;
    }

    /* Go back to old partition */
    tr_debug("Back to old partition");

    /* If scanned parent is from other partition, delete from MLE table */
    if (scanned_parent->leader_data.partitionId != thread_info(cur)->thread_leader_data->partitionId) {
        memcpy(ll64, ADDR_LINK_LOCAL_PREFIX , 8);
        memcpy(&ll64[8], scanned_parent->mac64 , 8);
        ll64[8] ^= 2;

        entry_temp = mle_class_get_entry_by_ll64(cur->id, scanned_parent->linkMarginToParent, ll64, false);
        if (entry_temp) {
            mle_class_remove_entry(cur->id, entry_temp);
        }
    }

    if (thread_info(cur)->thread_endnode_parent) {
    	mac_helper_coordinator_address_set(cur, ADDR_802_15_4_LONG, thread_info(cur)->thread_endnode_parent->mac64);
    }
    if (cur->thread_info->routerShortAddress == 0xfffe ||
            !thread_is_router_addr(cur->thread_info->routerShortAddress)) {
        thread_info(cur)->thread_attached_state = THREAD_STATE_CONNECTED;
    } else {
        thread_info(cur)->thread_attached_state = THREAD_STATE_CONNECTED_ROUTER;
    }

    cur->thread_info->localServerDataBase.release_old_address = false;
    cur->thread_info->releaseRouterId = false;

    cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
    mle_service_interface_receiver_handler_update(cur->id, thread_general_mle_receive_cb);

exit:
    ns_dyn_mem_free(cur->thread_info->thread_attach_scanned_parent);
    cur->thread_info->thread_attach_scanned_parent = NULL;

    return false;
}

static uint8_t *thread_single_address_registration_tlv_write(uint8_t *ptr, lowpan_context_t *ctx, uint8_t *addressPtr)
{
    *ptr++ = MLE_TYPE_ADDRESS_REGISTRATION;
    if (ctx) {
        *ptr++ = 9;
        //Write TLV to list
        *ptr++ = (ctx->cid | 0x80);
        memcpy(ptr, addressPtr + 8, 8);
        ptr += 8;
    } else {
        *ptr++ = 17;
        //Write TLV to list
        *ptr++ = 0;
        memcpy(ptr, addressPtr, 16);
        ptr += 16;
    }
    return ptr;
}

static int thread_attach_child_id_request_build(protocol_interface_info_entry_t *cur)
{
    uint8_t *ptr, *address_ptr;
    uint8_t mode;
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, 128, false, MLE_COMMAND_CHILD_ID_REQUEST);
    if (buf_id == 0) {
        return -1;
    }

    thread_scanned_parent_t *scan_parent = thread_info(cur)->thread_attach_scanned_parent;

    //Set ll64
    address_ptr = mle_service_get_msg_destination_address_pointer(buf_id);
    memcpy(address_ptr, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(address_ptr + 8 , scan_parent->mac64, 8);
    address_ptr[8] ^= 2;

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);

    lowpan_context_t *ctx;
    uint8_t ml64[16];
    uint8_t request_tlv_list[3];
    uint8_t macShort[2];
    uint8_t reqTlvCnt;
    mode = thread_mode_get_by_interface_ptr(cur);
    common_write_16_bit(scan_parent->shortAddress, macShort);
    //SET Coordinator Address
    mac_helper_coordinator_address_set(cur, ADDR_802_15_4_LONG, scan_parent->mac64);

    ptr = mle_service_get_data_pointer(buf_id);
    /* Allocate a new challenge */
    ptr = mle_tlv_write_mode(ptr, thread_mode_get_by_interface_ptr(cur));
    ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    //SET MLE Frame Counter
    ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
    ptr = mle_tlv_write_response(ptr, scan_parent->challengeData, scan_parent->chal_len);

    //Add ML-EID
    memcpy(ml64, thread_info(cur)->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&ml64[8], cur->iid_slaac, 8);
    if ((mode & MLE_FFD_DEV) == 0) {
        ctx = lowpan_context_get_by_address(&cur->lowpan_contexts, ml64);
        if (ctx) {
            //Write TLV to list
            ptr = thread_single_address_registration_tlv_write(ptr, ctx, ml64);
        }
    }
    reqTlvCnt = 2;
    request_tlv_list[0] = MLE_TYPE_NETWORK_DATA;

    request_tlv_list[1] = MLE_TYPE_ADDRESS16;
    if (thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        request_tlv_list[2] = MLE_TYPE_ROUTE;
        reqTlvCnt = 3;
    }
    ptr = mle_tlv_req_tlv(ptr, request_tlv_list, reqTlvCnt);
    ptr = mle_tlv_write_timeout(ptr, cur->thread_info->host_link_timeout);
    //Set Version
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    //add only active timestamp to the child id request
    ptr = thread_active_timestamp_write(cur, ptr);

    if (mle_service_update_length_by_ptr(buf_id,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = THREAD_CHILD_ID_REQUEST_MAX_RETRY_CNT;
    timeout.timeout_init = 1;
    timeout.timeout_max = 3;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_packet_callback(buf_id, thread_child_id_req_timeout);
    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_send_message(buf_id);
    //Update MLE handler state
    mle_service_interface_receiver_handler_update(cur->id, thread_mle_child_request_receive_cb);
    cur->nwk_bootstrap_state = ER_CHILD_ID_REQ;
    scan_parent->child_id_request_id = buf_id;
    return 0;
}

static bool thread_child_update_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRerties)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    (void)msgId;
    if (!cur || !cur->thread_info->thread_endnode_parent) {
        return false;
    }
    tr_debug("Child Update CB");
    if (cur->thread_info->thread_endnode_parent->childUpdateProcessStatus) {
        //This process is ready
        cur->thread_info->thread_endnode_parent->childUpdateProcessStatus = false;
        if (!cur->thread_info->thread_endnode_parent->childUpdatePending) {

            cur->thread_info->thread_endnode_parent->childUpdateProcessActive = false;
            //Disable Poll
            mac_data_poll_protocol_poll_mode_decrement(cur);
            tr_debug("Child Update ready");
        } else {
            cur->thread_info->thread_endnode_parent->childUpdatePending = false;
            tr_debug("Child Update Pending");
            thread_bootsrap_event_trig(THREAD_CHILD_UPDATE, cur->bootStrapId, ARM_LIB_HIGH_PRIORITY_EVENT);
        }
        return false;
    }

    if (usedAllRerties) {

        cur->thread_info->thread_endnode_parent->childUpdatePending = false;
        cur->thread_info->thread_endnode_parent->childUpdateProcessActive = false;
        cur->thread_info->thread_endnode_parent->childUpdateProcessStatus = false;
        mac_data_poll_protocol_poll_mode_decrement(cur);
        thread_bootstrap_reset_restart(cur->id);
        tr_debug("Restart attachment");
        return false;
    }

    return true;

}

int8_t thread_bootstrap_child_update(protocol_interface_info_entry_t *cur)
{
    mle_message_timeout_params_t timeout;
    uint8_t mode;
    uint32_t keySequence;

    if (!cur->thread_info->thread_endnode_parent) {
        return -1;
    }

    tr_debug("Child Update Request");

    mode = thread_mode_get_by_interface_ptr(cur);

    //Build packet

    uint16_t bufId = mle_service_msg_allocate(cur->id, 150 + 3 + 6 + 10, false, MLE_COMMAND_CHILD_UPDATE_REQUEST);
    if (bufId == 0) {
        return -1;
    }

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(bufId);
    memcpy(address_ptr, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(address_ptr + 8, cur->thread_info->thread_endnode_parent->mac64, 8);
    address_ptr[8] ^= 2;

    uint8_t *ptr = mle_service_get_data_pointer(bufId);
    ptr = mle_tlv_write_mode(ptr, mode);
    ptr = mle_tlv_write_timeout(ptr, cur->thread_info->host_link_timeout);
    ptr = thread_leader_data_tlv_write(ptr, cur);

    //Set Addresss TLV
    if ((mode & MLE_FFD_DEV) == 0) {
        ptr = thread_address_registration_tlv_write(ptr, cur);
    }

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 3;
    timeout.timeout_init = 1;
    timeout.timeout_max = 4;
    timeout.delay = MLE_NO_DELAY;
    net_host_mode_t macHostMode;
    if (mac_data_poll_host_mode_get(cur,&macHostMode) == 0 && macHostMode == NET_HOST_RX_ON_IDLE) {
        thread_end_device_mode_set(cur, false);
    }
    mac_data_poll_init_protocol_poll(cur);
    mle_service_set_packet_callback(bufId, thread_child_update_timeout_cb);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return 0;
}
int thread_host_bootstrap_child_update_negative_response(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, mle_tlv_info_t *challenge)
{
    uint32_t keySequence;

    if (!challenge) {
        return -2;
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, 16 + challenge->tlvLen, false, MLE_COMMAND_CHILD_UPDATE_RESPONSE);

    if (bufId == 0) {
        return -1;
    }
    tr_debug("MLE Child negative Response");

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    *ptr++ = MLE_TYPE_STATUS;
    *ptr++ = 1;
    *ptr++ = MLE_STATUS_ERROR;

    if (challenge->tlvLen) {
        //Add response
        ptr = mle_tlv_write_response(ptr, challenge->dataPtr, challenge->tlvLen);
    }

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_service_set_msg_destination_address(bufId, dstAddress);
    mle_service_send_message(bufId);
    return 0;
}


#endif

