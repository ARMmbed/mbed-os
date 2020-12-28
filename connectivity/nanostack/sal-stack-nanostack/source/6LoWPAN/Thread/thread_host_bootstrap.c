/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
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
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "MPL/mpl.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "Core/include/ns_address_internal.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"

#define TRACE_GROUP "tebs"

static bool thread_child_update_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRerties);
static bool thread_parent_discover_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries);
static bool thread_child_id_req_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries);

static void thread_child_synch_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);
static void thread_mle_child_request_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);

static int thread_parent_request_build(protocol_interface_info_entry_t *cur);

static int thread_attach_child_id_request_build(protocol_interface_info_entry_t *cur);
static int thread_end_device_synch_response_validate(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t data_length, uint8_t linkMargin, uint8_t *src_address, mle_security_header_t *securityHeader);

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
    memcpy(&ula[8], parent->mac64, 8);
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
    thread_partition_data_purge(cur);
    thread_clean_old_16_bit_address_based_addresses(cur);
    mpl_clear_realm_scope_seeds(cur);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_PROXIED_HOST, NULL);
    ipv6_route_table_remove_info(cur->id, ROUTE_THREAD_PROXIED_DUA_HOST, NULL);
    thread_network_data_clean(cur);
    cur->nwk_mode = ARM_NWK_GP_IP_MODE;
}

//This function is for Thread Parent scan callback
static bool thread_parent_discover_timeout_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur;
    bool new_entry_created;

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
        mac_neighbor_table_entry_t *entry_temp;

        linkConfiguration = thread_joiner_application_get_config(interface_id);
        if (!linkConfiguration) {
            return false;
        }

        /* SED requires that scanned parent is added to MLE table */
        memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(&ll64[8], parent->mac64, 8);
        ll64[8] ^= 2;
        entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), ll64, true, &new_entry_created);
        if (entry_temp == NULL) {
            return false;
        }
        thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, parent->linkMarginToParent, new_entry_created);
        thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);

        entry_temp->mac16 = parent->shortAddress;
        entry_temp->link_role = PRIORITY_PARENT_NEIGHBOUR;

        mle_service_frame_counter_entry_add(interface_id, entry_temp->index, parent->mleFrameCounter);

        thread_management_key_sets_calc(cur, linkConfiguration, cur->thread_info->thread_attach_scanned_parent->keySequence);
        thread_key_guard_timer_calculate(cur, linkConfiguration, true);

        mlme_device_descriptor_t device_desc;
        mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, parent->linLayerFrameCounter, false);
        mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, mac_helper_default_key_index_get(cur), new_entry_created);
        mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, THREAD_DEFAULT_LINK_LIFETIME);

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

    timeout.retrans_max = THREAD_PARENT_REQUEST_MAX_RETRY_CNT;
    timeout.timeout_init = THREAD_PARENT_REQ_SCANMASK_R_TIMEOUT;
    timeout.timeout_max = THREAD_PARENT_REQ_SCANMASK_RE_TIMEOUT;
    timeout.delay = MLE_STANDARD_RESPONSE_DELAY;

    if (cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH ||
            cur->thread_info->thread_attached_state == THREAD_STATE_REATTACH_RETRY ||
            cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED ||
            cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        // When doing re-attach End devices are immediately accepted as parents
        scanMask |= 0x40;
        timeout.timeout_init = THREAD_PARENT_REQ_SCANMASK_RE_TIMEOUT;
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
    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    cur->nwk_nd_re_scan_count = 1;

    mle_service_set_packet_callback(buf_id, thread_parent_discover_timeout_cb);
    if (cur->thread_info->thread_attached_state == THREAD_STATE_NETWORK_DISCOVER) {
        mle_service_interface_receiver_handler_update(cur->id, thread_mle_parent_discover_receive_cb);
    } else {
        mle_service_interface_receiver_handler_update(cur->id, thread_general_mle_receive_cb);
    }

    mle_service_set_msg_timeout_parameters_fast(buf_id, &timeout);
    mle_service_send_message(buf_id);
    return 0;
}

void thread_network_attach_start(protocol_interface_info_entry_t *cur)
{
    if (thread_parent_request_build(cur) == 0) {
        tr_debug("MLE Parent request");
        cur->nwk_bootstrap_state = ER_MLE_SCAN;
        cur->bootsrap_state_machine_cnt = 0;
        /* advance trickle timer by 6 (in 100ms ticks) seconds if needed */
        thread_routing_trickle_advance(&cur->thread_info->routing, 6 * 10);
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
    mle_tlv_info_t addressRegisteredTlv;
    mac_neighbor_table_entry_t *entry_temp;
    bool new_entry_created;

    tr_debug("Validate Link Synch Response");
    //Check First Status
    if (mle_tlv_read_8_bit_tlv(MLE_TYPE_STATUS, ptr, data_length, &status)) {
        tr_debug("Synch status response %x", status);
        //Clean synch state and start
        return -1;
    }
    // Validate response
    // MLE_TYPE_MODE
    // Address
    // MLE_TYPE_SRC_ADDRESS
    // MLE_TYPE_LEADER_DATA
    // MLE_TYPE_ADDRESS_REGISTRATION
    if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, ptr, data_length, &mode) ||
            !mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, ptr, data_length, &srcAddress) ||
            !mle_tlv_read_16_bit_tlv(MLE_TYPE_ADDRESS16, ptr, data_length, &address16) ||
            !thread_leader_data_parse(ptr, data_length, &leaderData) ||
            !mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, ptr, data_length, &llFrameCounter)) {
        tr_debug("missing TLV's");
        return -1;
    }

    if (!(mode & THREAD_DEVICE_FED)) {
        // check for presence of Address registration TLV for MTDs
        if (!mle_tlv_read_tlv(MLE_TYPE_ADDRESS_REGISTRATION, ptr, data_length, &addressRegisteredTlv) ||
                (addressRegisteredTlv.tlvLen == 0)) {
            tr_debug("MTD missed address registration TLV - reattach");
            return -1;
        }
    }

    // check if the source address is a router address
    if (!thread_is_router_addr(srcAddress)) {
        return -1;
    }

    // check if the address16 is a valid child address
    if (!thread_addr_is_child(srcAddress, address16)) {
        return -1;
    }

    if (securityHeader->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
        thread_management_key_synch_req(cur->id, common_read_32_bit(securityHeader->Keysource));
        // if learning key sequence from link sync actual guard timer value is not known
        thread_key_guard_timer_reset(cur);
    } else {
        tr_debug("Key ID Mode 2 not used; dropped.");
        return -3;
    }

    //Update parent link information
    entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), src_address, true, &new_entry_created);

    if (!entry_temp) {
        tr_debug("Neighbor allocate fail");
        return -2;
    }
    thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, new_entry_created);
    thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
    /*

    */
    entry_temp->mac16 = srcAddress;
    entry_temp->connected_device = 1;
    entry_temp->link_role = PRIORITY_PARENT_NEIGHBOUR; // Make this our parent
    common_write_16_bit(entry_temp->mac16, shortAddress);

    mac_helper_coordinator_address_set(cur, ADDR_802_15_4_SHORT, shortAddress);
    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, thread_info(cur)->host_link_timeout);

    mlme_device_descriptor_t device_desc;
    mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, llFrameCounter, false);
    mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, securityHeader->KeyIndex, new_entry_created);

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
    (void) interface_id;
    tr_debug("Thread MLE message child_synch handler");
    //State machine What packet shuold accept in this case
    protocol_interface_info_entry_t *cur = mle_msg->interface_ptr;

    /* Check that message is from link-local scope */
    if (!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    uint16_t messageId;
    uint8_t linkMargin = thread_compute_link_margin(mle_msg->dbm);
    //State machine What packet shuold accept in this case
    switch (mle_msg->message_type) {
        case MLE_COMMAND_CHILD_UPDATE_RESPONSE:

            messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

            if (messageId == 0) {
                tr_debug("No matching challenge");
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

    if (thread_info(cur)->version < THREAD_VERSION_1_2) {
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
    (void) interface_id;
    protocol_interface_info_entry_t *cur = mle_msg->interface_ptr;

    /* Check that message is from link-local scope */
    if (!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    tr_debug("MLE Parent response handler");
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
                tr_debug("No matching challenge");
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
                    if (!thread_partition_match(cur, &leaderData)) {
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
                    if (thread_partition_match(cur, &leaderData)) {
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

            if (thread_common_ccm_enabled(cur) &&
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
                        int retVal = thread_bootstrap_partition_process(cur, connectivityTlv.activeRouters, &leaderData, NULL);
                        if (retVal > 0) {
                            // New partition is Higher
                            scan_result = thread_info(cur)->thread_attach_scanned_parent;
                        }
                    } else if (leaderData.partitionId == currentPartitionId) {
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
                                        !thread_is_router_addr(srcAddress)) {
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
                                    if (!scan_result) {
                                        tr_debug("Dropped Connectivity is not as good");
                                    }
                                }
                            } else {
                                scan_result = NULL;
                            }
                        }
                    }
                    if (scan_result && thread_host_prefer_parent_response(cur, scan_result, version, &connectivityTlv)) {
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
    mac_neighbor_table_entry_t *entry_temp;
    protocol_interface_info_entry_t *cur = mle_msg->interface_ptr;
    link_configuration_s *link_configuration;
    link_configuration = thread_joiner_application_get_config(cur->id);
    if (!link_configuration) {
        return;
    }

    /* Check that message is from link-local scope */
    if (!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    tr_debug("Thread MLE Child ID response handler");

    switch (mle_msg->message_type) {

        case MLE_COMMAND_CHILD_ID_RESPONSE: {
            uint8_t src_mac64[8];
            uint8_t shortAddress[2];
            uint16_t childId;
            mle_tlv_info_t routeTlv, addressRegisteredTlv, networkDataTlv;
            mle_tlv_info_t ConfigurationTlv;
            uint64_t pending_timestamp = 0;
            uint64_t active_timestamp;
            thread_scanned_parent_t *scan_result = thread_info(cur)->thread_attach_scanned_parent;
            bool new_entry_created;

            tr_info("Recv Child ID Response");

            // Validate that response is coming from the scanned parent candidate
            memcpy(src_mac64, (mle_msg->packet_src_address + 8), 8);
            src_mac64[0] ^= 2;
            if (memcmp(src_mac64, scan_result->mac64, 8) != 0) {
                tr_debug("Drop Child ID response from previous request");
                return;
            }

            // Clear old data
            if (cur->thread_info->releaseRouterId) {
                thread_bootstrap_clear_neighbor_entries(cur);
            }

            cur->thread_info->localServerDataBase.release_old_address = true;

            thread_neighbor_list_clean(cur);
            thread_leader_service_stop(interface_id);
            thread_leader_service_leader_data_free(cur->thread_info);
            thread_merge_prepare(cur);

            // Create entry for new parent
            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, true, &new_entry_created);
            if (entry_temp == NULL) {
                // todo: what to do here?
                return;
            }
            thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, thread_compute_link_margin(mle_msg->dbm), new_entry_created);
            thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);

            //Parse mandatory TLV's
            if (!thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
                return;
            }
            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &entry_temp->mac16)) {
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

            common_write_16_bit(entry_temp->mac16, shortAddress);
            //Update possible reed address by real router address
            scan_result->shortAddress = entry_temp->mac16;

            entry_temp->connected_device = 1;
            entry_temp->link_role = PRIORITY_PARENT_NEIGHBOUR;

            mac_helper_coordinator_address_set(cur, ADDR_802_15_4_SHORT, shortAddress);
            mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, thread_info(cur)->host_link_timeout);

            if (scan_result->security_key_index != security_headers->KeyIndex) {
                // KeyIndex has been changed between parent_response and child_id_response, reset link layer frame counter
                scan_result->linLayerFrameCounter = 0;
                scan_result->security_key_index = security_headers->KeyIndex;
            }
            mlme_device_descriptor_t device_desc;
            mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, scan_result->linLayerFrameCounter, false);
            mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, security_headers->KeyIndex, new_entry_created);

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
            parent->version = scan_result->version;
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
                thread_router_bootstrap_route_tlv_push(cur, routeTlv.dataPtr, routeTlv.tlvLen, thread_compute_link_margin(mle_msg->dbm), entry_temp);
            }
            thread_bootstrap_attached_ready(cur);

            break;
        }
        default:
            tr_debug("Skip msg type %d", mle_msg->message_type);
            break;

    }
}

static int8_t thread_end_device_synch_start(protocol_interface_info_entry_t *cur)
{
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, 150 + 3 + 6 + 10, true, MLE_COMMAND_CHILD_UPDATE_REQUEST);
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

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
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
        bool new_entry_created;

        // Add the parent to the MLE neighbor table
        mac_neighbor_table_entry_t *mac_entry = mac_neighbor_entry_get_by_mac64(mac_neighbor_info(cur), cur->thread_info->thread_endnode_parent->mac64, true, &new_entry_created);
        if (mac_entry) {
            //Add link margin 64
            thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, mac_entry->index, 64, new_entry_created);
            thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, mac_entry->index);

            mac_entry->mac16 = cur->thread_info->thread_endnode_parent->shortAddress;
            mac_entry->connected_device = 1;

            // In case we don't get response to sync; use temporary timeout here,
            // Child ID Response handler will set correct value later
            mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), mac_entry, mac_entry->link_lifetime);

            // Add the parent to the MAC table (for e.g. secured/fragmented Child Update Response)
            mlme_device_descriptor_t device_desc;
            mac_helper_device_description_write(cur, &device_desc, mac_entry->mac64, mac_entry->mac16, 0, false);
            mac_helper_devicetable_set(&device_desc, cur, mac_entry->index, cur->mac_parameters->mac_default_key_index, new_entry_created);
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
    mac_neighbor_table_entry_t *entry_temp;
    thread_scanned_parent_t *scanned_parent;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

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
    tr_debug("Child ID Request timed out: %s", trace_ipv6(addr));

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
    if ((scanned_parent->leader_data.partitionId != thread_info(cur)->thread_leader_data->partitionId) ||
            (scanned_parent->leader_data.weighting != thread_info(cur)->thread_leader_data->weighting)) {
        entry_temp = mac_neighbor_table_address_discover(mac_neighbor_info(cur), scanned_parent->mac64, ADDR_802_15_4_LONG);
        if (entry_temp) {
            bool my_parent = thread_check_is_this_my_parent(cur, entry_temp);
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), entry_temp);
            if (my_parent) {
                tr_debug("No parent resp - any-attach");
                thread_bootstrap_connection_error(interface_id, CON_ERROR_NETWORK_ATTACH_FAIL, NULL);
                goto exit;
            }
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
    cur->thread_info->networkDataRequested = false;

    cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;

exit:
    mle_service_interface_receiver_handler_update(cur->id, thread_general_mle_receive_cb);

    ns_dyn_mem_free(cur->thread_info->thread_attach_scanned_parent);
    cur->thread_info->thread_attach_scanned_parent = NULL;

    return false;
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
    memcpy(address_ptr + 8, scan_parent->mac64, 8);
    address_ptr[8] ^= 2;

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);

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
    if ((mode & MLE_FFD_DEV) == 0) {
        ptr = thread_ml_address_tlv_write(ptr, cur);
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

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
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
    if (!cur || !cur->thread_info->thread_endnode_parent) {
        return false;
    }
    if (msgId != cur->thread_info->childUpdateReqMsgId) {
        //Wrong message id
        return false;
    }

    if (usedAllRerties) {
        tr_debug("Child Update timed out");
        cur->thread_info->thread_endnode_parent->childUpdatePending = false;
        cur->thread_info->thread_endnode_parent->childUpdateProcessActive = false;
        mac_data_poll_protocol_poll_mode_decrement(cur);
        thread_bootstrap_reset_restart(cur->id);
        tr_debug("Restart attachment");
        return false;
    }
    if (cur->thread_info->thread_endnode_parent->childUpdateProcessActive) {
        // we have not received response so re-send
        return true;
    }

    return false;

}

bool thread_host_bootstrap_child_update(protocol_interface_info_entry_t *cur, const uint8_t *mac64)
{
    mle_message_timeout_params_t timeout;
    uint8_t mode;
    uint32_t keySequence;


    if (!cur->thread_info->thread_endnode_parent) {
        tr_debug("Not end device parent info for NUD");
        return false;
    }

    if (cur->thread_info->thread_endnode_parent->childUpdateProcessActive) {
        //Set Pending if earlier process is already started
        cur->thread_info->thread_endnode_parent->childUpdatePending = true;
        return false;
    }
    //Trig event
    cur->thread_info->thread_endnode_parent->childUpdatePending = false;
    cur->thread_info->thread_endnode_parent->childUpdateProcessActive = true;


    tr_debug("Child Update Request");

    mode = thread_mode_get_by_interface_ptr(cur);

    //Build packet

    uint16_t bufId = mle_service_msg_allocate(cur->id, 150 + 3 + 6 + 10, false, MLE_COMMAND_CHILD_UPDATE_REQUEST);
    if (bufId == 0) {
        return false;
    }

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(bufId);
    memcpy(address_ptr, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(address_ptr + 8, mac64, 8);
    address_ptr[8] ^= 2;

    uint8_t *ptr = mle_service_get_data_pointer(bufId);
    ptr = mle_tlv_write_mode(ptr, mode);
    ptr = mle_general_write_source_address(ptr, cur);
    ptr = mle_tlv_write_timeout(ptr, cur->thread_info->host_link_timeout);
    ptr = thread_leader_data_tlv_write(ptr, cur);

    //Set Addresss TLV
    if ((mode & MLE_FFD_DEV) == 0) {
        ptr = thread_address_registration_tlv_write(ptr, cur);
    }

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 3;
    timeout.timeout_init = 1;
    timeout.timeout_max = 4;
    timeout.delay = MLE_NO_DELAY;
    net_host_mode_t macHostMode;
    if (mac_data_poll_host_mode_get(cur, &macHostMode) == 0 && macHostMode == NET_HOST_RX_ON_IDLE) {
        thread_end_device_mode_set(cur, false);
    }
    mac_data_poll_init_protocol_poll(cur);
    cur->thread_info->childUpdateReqMsgId = bufId;
    mle_service_set_packet_callback(bufId, thread_child_update_timeout_cb);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);

    return true;
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

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_service_set_msg_destination_address(bufId, dstAddress);
    mle_service_send_message(bufId);
    return 0;
}


#endif

