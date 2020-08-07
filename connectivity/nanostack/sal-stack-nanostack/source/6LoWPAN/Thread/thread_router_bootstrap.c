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
 * \file thread_router_bootstrap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "shalib.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_beacon.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_lowpower_private_api.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "6LoWPAN/Thread/thread_ccm.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "multicast_api.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "DHCPv6_client/dhcpv6_client_api.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "thread_border_router_api.h"
#include "Core/include/ns_address_internal.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"

#ifdef HAVE_THREAD_ROUTER

#define TRACE_GROUP "trbs"

static uint8_t *thread_tlv_add(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint8_t tlv_type, uint8_t mode);

static void mle_multicast_push_to_sleep_child(protocol_interface_info_entry_t *cur, buffer_t *buf);

static void thread_bootstrap_client_router_id_cb(int8_t interface_id, int8_t status, uint16_t router_rloc, const uint8_t router_mask_ptr[9]);
static void thread_router_synch_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);
static void thread_bootstrap_client_router_id_release_cb(int8_t interface_id, int8_t status, uint16_t router_rloc, const uint8_t router_mask_ptr[9]);

static int thread_router_synch_accept_request_build(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint16_t shortAddress, uint8_t *challenge, uint8_t chalLen, uint8_t *tlvReq, uint8_t tlvReqLen);
static int thread_router_accept_to_endevice(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t *challenge, uint8_t chalLen);
static int thread_router_accept_request_build(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint16_t shortAddress, uint8_t *challenge, uint8_t chalLen, uint8_t type, bool rssi_tlv, uint8_t rssi);
static int thread_child_update_response(protocol_interface_info_entry_t *cur, uint8_t *dst_address, uint8_t mode, uint16_t short_address, uint32_t timeout, mle_tlv_info_t *addressRegisterTlv, mle_tlv_info_t *tlvReq, mle_tlv_info_t *challengeTlv, uint64_t active_timestamp, uint64_t pending_timestamp);
static int mle_build_and_send_data_response_msg(protocol_interface_info_entry_t *cur, uint8_t *dst_address, uint8_t *data_ptr, uint16_t data_len, mle_tlv_info_t *request_tlv, uint8_t mode);
static int thread_attach_parent_response_build(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, uint8_t *challenge, uint16_t chalLen, uint8_t linkMargin, uint8_t scanMask, uint8_t mode);
static int mle_attach_child_id_response_build(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, thread_pending_child_id_req_t *child_req, struct mac_neighbor_table_entry *neigh_info);

static int8_t thread_router_bootstrap_synch_request_send(protocol_interface_info_entry_t *cur);
static bool thread_child_id_request(protocol_interface_info_entry_t *cur, struct mac_neighbor_table_entry *entry_temp);


#ifdef HAVE_THREAD_V2
static bool thread_router_bootstrap_is_reed_upgrade_allowed(protocol_interface_info_entry_t *cur);
#else
#define thread_router_bootstrap_is_reed_upgrade_allowed(cur) (true)
#endif

static bool thread_router_parent_address_check(protocol_interface_info_entry_t *cur, uint8_t *source_addr)
{
    if (thread_info(cur)->thread_endnode_parent &&
            addr_iid_matches_eui64(source_addr + 8, thread_info(cur)->thread_endnode_parent->mac64)) {
        return true;
    }

    return false;
}

static void thread_send_proactive_an(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == ADDR_SOURCE_DHCP || e->source == ADDR_SOURCE_SLAAC) {
            uint8_t br_ml_addr[16];
            ns_list_foreach(thread_network_data_prefix_cache_entry_t, prefix, &cur->thread_info->networkDataStorage.localPrefixList) {
                uint8_t prefixBytesLen = prefixBits_to_bytes(prefix->servicesPrefixLen);

                if (memcmp(e->address, prefix->servicesPrefix, prefixBytesLen) != 0) {
                    continue;
                }

                ns_list_foreach(thread_network_server_data_entry_t, br, &prefix->borderRouterList) {
                    if (br->routerID == 0xfffe) {
                        continue;
                    }

                    uint16_t rloc16 = mac_helper_mac16_address_get(cur);

                    thread_addr_write_mesh_local_16(br_ml_addr, br->routerID, cur->thread_info);
                    tr_debug("Sending proactive AN to border router: %s", trace_ipv6(br_ml_addr));
                    thread_management_client_proactive_an(cur->id, e->address, rloc16, cur->iid_slaac, br_ml_addr);
                }
            }
        }
    }
}

int thread_router_bootstrap_mle_advertise(protocol_interface_info_entry_t *cur)
{
    /* How was this 40 calculated? Seems to be more than enough, at least.
     * MODE = 2+1, SRC_ADDR = 2+2, LINK = 2+1+4*neighbours, ROUTE = 2+MLE_ROUTE_MIN_OPTION_LEN+routers
     * Total = 10 + neighbours * 4
     */
    uint16_t neig_cache_size = 40 + 3;
    uint8_t *ptr;

    if (!cur) {
        return -1;
    }

    if (!thread_is_connected(cur)) {
        return -1;
    }

    if (thread_i_am_router(cur)) {
        neig_cache_size += thread_route_option_size(cur);
        neig_cache_size += thread_leader_data_tlv_size(cur);
    }

    uint16_t buf_id = mle_service_msg_allocate(cur->id, neig_cache_size, false, MLE_COMMAND_ADVERTISEMENT);
    if (buf_id == 0) {
        return -1;
    }

    uint32_t keySequence;
    if (thread_management_get_current_keysequence(cur->id, &keySequence) == 0) {
        mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    }

    //SET Destination address
    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_NODES);

    ptr = mle_service_get_data_pointer(buf_id);
    ptr = mle_general_write_source_address(ptr, cur);
    if (thread_i_am_router(cur)) {
        ptr = thread_route_option_write(cur, ptr);
    } else {
        *ptr++ = MLE_TYPE_ROUTE;
        *ptr++ = 0;
    }

    ptr = thread_leader_data_tlv_write(ptr, cur);

    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    tr_debug("Send MLE Secured Advertisement");
    return mle_service_send_message(buf_id);

}

static void clone_multicast_to_unicast(protocol_interface_info_entry_t *cur, mac_neighbor_table_entry_t *entry, buffer_t *buf)
{
    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return;
    }

    buffer_t *new_buf = buffer_clone(buf);
    if ((new_buf->info & B_DIR_MASK) == B_DIR_UP) {
        buffer_data_pointer(new_buf)[IPV6_HDROFF_HOP_LIMIT] = --new_buf->options.hop_limit;
    }

    new_buf->info = (buffer_info_t)(B_DIR_DOWN | B_TO_IPV6_TXRX | B_FROM_IPV6_FWD);
    uint8_t next_hop[16];
    memcpy(next_hop, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&next_hop[8], entry->mac64, 8);
    next_hop[8] ^= 2;

    ipv6_buffer_route_to(new_buf, next_hop, cur);
    protocol_push(new_buf);
    tr_debug("Cloned %s to %s", tr_ipv6(buf->dst_sa.address), tr_ipv6(next_hop));
}

static void mle_multicast_push_to_sleep_child(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (!cur_entry->rx_on_idle) {
            clone_multicast_to_unicast(cur, cur_entry, buf);
        }
    }
}

static void thread_multicast_forward_to_child(protocol_interface_info_entry_t *cur, buffer_t *buffer)
{
    mle_multicast_push_to_sleep_child(cur, buffer);
}

static void thread_registered_mcast_forward_to_child(protocol_interface_info_entry_t *cur, buffer_t *buffer)
{
    thread_registered_mcast_addr_t *addr = thread_registered_mcast_addr_entry_find(cur, buffer->dst_sa.address);

    if (addr) {
        tr_debug("Forwarding to registered multicast address: %s", trace_ipv6(addr->address));

        ns_list_foreach(thread_mcast_child_t, child, &addr->children) {
            mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur), child->mac64, ADDR_802_15_4_LONG);
            if (entry && !entry->rx_on_idle) {
                clone_multicast_to_unicast(cur, entry, buffer);
            }
        }
    }
}

void thread_router_bootstrap_multicast_forwarder_enable(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    //tr_debug("Considering forward %s (%s)", tr_ipv6(buf->dst_sa.address), (buf->info & B_DIR_MASK) == B_DIR_UP ? "up" : "down");
    if (addr_ipv6_multicast_scope(buf->dst_sa.address) <= IPV6_SCOPE_INTERFACE_LOCAL) {
        return;
    }

    // Safe guard: do NOT forward received link-local
    // multicast packets to our sleepy children
    if ((buf->info & B_DIR_MASK) == B_DIR_UP &&
            addr_ipv6_multicast_scope(buf->dst_sa.address) <= IPV6_SCOPE_LINK_LOCAL) {
        tr_debug("Received link-local multicast; return...");
        return;
    }

    uint8_t addr[16];
    thread_bootstrap_all_nodes_address_generate(addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_LINK_LOCAL);
    if (addr_ipv6_equal(buf->dst_sa.address, addr)) {
        thread_multicast_forward_to_child(cur, buf);
        return;
    }
    thread_bootstrap_all_nodes_address_generate(addr, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, IPV6_SCOPE_REALM_LOCAL);
    if (addr_ipv6_equal(buf->dst_sa.address, addr)) {
        thread_multicast_forward_to_child(cur, buf);
        return;
    }
    thread_registered_mcast_forward_to_child(cur, buf);
}

static void thread_router_synch_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    thread_leader_data_t leaderData;
    uint8_t linkMarginfronNeigh;
    uint16_t version, shortAddress, address16;
    uint32_t llFrameCounter;
    mle_tlv_info_t routing;
    mac_neighbor_table_entry_t *entry_temp;
    tr_debug("Thread MLE message router sync");
    //State machine What packet shuold accept in this case
    protocol_interface_info_entry_t *cur = mle_msg->interface_ptr;

    /* Check that message is from link-local scope */
    if (!addr_is_ipv6_link_local(mle_msg->packet_src_address)) {
        return;
    }

    if (!thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
        return;
    }

    //Validate ready TLV for router Synch
    if ((!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) ||
            (!mle_tlv_read_16_bit_tlv(MLE_TYPE_ADDRESS16, mle_msg->data_ptr, mle_msg->data_length, &address16)) ||
            (!mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &llFrameCounter)) ||
            (!mle_tlv_read_tlv(MLE_TYPE_ROUTE, mle_msg->data_ptr, mle_msg->data_length, &routing)) ||
            (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &shortAddress))) {
        return ;
    }

    uint8_t linkMargin = thread_compute_link_margin(mle_msg->dbm);

    switch (mle_msg->message_type) {
        case MLE_COMMAND_ACCEPT: {
            tr_info("Accept (ROUTER handler)");
            uint32_t mleFrameCounter;
            bool new_neigbour;
            uint16_t messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

            if (messageId == 0) {
                tr_debug("No matching challenge");
                return;
            }
            /*Link accept command has an optional MLE Frame counter TLV, if this is not present use link layer frame counter TLV
             * as MLE frame counter TLV*/
            if (!mle_tlv_read_32_bit_tlv(MLE_TYPE_MLE_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &mleFrameCounter)) {
                mleFrameCounter = llFrameCounter;
            }

            if (!thread_is_router_addr(address16)) {
                return;
            }
            //Allocate neighbor entry
            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, true, &new_neigbour);
            if (!entry_temp) {
                return;
            }

            if (security_headers->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
                thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));
                thread_key_guard_timer_reset(cur);
            } else {
                tr_error("Key ID Mode 2 not used; dropped.");
                return;
            }

            thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, new_neigbour);
            thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);

            //Free Response
            mle_service_msg_free(messageId);
            entry_temp->mac16 = shortAddress;

            //when allocating neighbour entry, use MLE Frame counter if present to validate further advertisements from the neighbour
            mle_service_frame_counter_entry_add(cur->id, entry_temp->index, mleFrameCounter);
            uint32_t timeout_tlv;

            mle_tlv_info_t mle_tlv_info;

            if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_TIMEOUT, &mle_tlv_info) > 0) {
                timeout_tlv = common_read_32_bit(mle_tlv_info.dataPtr);
            } else {
                if (new_neigbour) {
                    timeout_tlv = THREAD_DEFAULT_LINK_LIFETIME;
                } else {
                    timeout_tlv = entry_temp->link_lifetime;
                }
            }

            mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, timeout_tlv);

            if (thread_is_router_addr(shortAddress)) {
                entry_temp->connected_device = 1;
            }
            thread_neighbor_class_request_full_data_setup_set(&cur->thread_info->neighbor_class, entry_temp->index, true);

            if (mle_tlv_read_8_bit_tlv(MLE_TYPE_RSSI, mle_msg->data_ptr, mle_msg->data_length, &linkMarginfronNeigh)) {
                thread_routing_update_link_margin(cur, entry_temp->mac16, linkMargin, linkMarginfronNeigh);
            }

            mlme_device_descriptor_t device_desc;
            mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, llFrameCounter, false);
            mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, security_headers->KeyIndex, new_neigbour);

            //Copy Leader Data
            *cur->thread_info->thread_leader_data = leaderData;
            thread_bootstrap_update_ml16_address(cur, address16);
            thread_info(cur)->routerShortAddress = address16;
            thread_info(cur)->thread_attached_state = THREAD_STATE_CONNECTED_ROUTER;
            if (cur->thread_info->thread_leader_data->leaderRouterId == thread_router_id_from_addr(address16)) {
                if (thread_leader_service_thread_partitition_restart(interface_id, &routing)) {
                    return;
                }
                thread_network_data_request_send(cur, mle_msg->packet_src_address, false);
                // force leader to learn active/pending sets from data response
                cur->thread_info->leader_synced = true;
                // Prevent the Leader to learn network data from data response
                cur->thread_info->networkDataRequested = false;
                tr_info("Router synch OK as Leader");
            } else {
                // Decrement data version and request network data to be updated
                cur->thread_info->thread_leader_data->dataVersion--;
                cur->thread_info->thread_leader_data->stableDataVersion--;
                thread_network_data_request_send(cur, mle_msg->packet_src_address, true);
                // remove any existing rloc mapping in nvm
                thread_nvm_store_mleid_rloc_map_remove();
                tr_info("Router synch OK as Router");
            }

            thread_router_bootstrap_route_tlv_push(cur, routing.dataPtr, routing.tlvLen, linkMargin, entry_temp);
            thread_bootstrap_attached_ready(cur);
        }

        break;
        default:

            break;
    }
}

static int thread_router_synch_accept_request_build(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint16_t shortAddress, uint8_t *challenge, uint8_t chalLen, uint8_t *tlvReq, uint8_t tlvReqLen)
{
    uint16_t length = 127;
    uint8_t i, tlvType;
    mle_message_timeout_params_t timeout;
    struct link_configuration *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    tr_debug("MLE Router Link Synh response");

    for (i = 0; i < tlvReqLen; i++) {
        tlvType = tlvReq[i];
        if (tlvType == MLE_TYPE_NETWORK_DATA) {
            length += 2 + thread_network_data_tlv_size(cur, true);
            break;
        }
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, length, false, MLE_COMMAND_ACCEPT);
    if (bufId == 0) {
        return -1;
    }

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    ptr = thread_leader_data_tlv_write(ptr, cur);
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    //SET MLE Frame Counter
    ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));

    ptr = mle_tlv_write_response(ptr, challenge, chalLen);
    ptr = mle_general_write_source_address(ptr, cur);

    for (i = 0; i < tlvReqLen; i++) {
        tlvType = tlvReq[i];
        if (tlvType == MLE_TYPE_NETWORK_DATA) {
            ptr = thread_network_data_tlv_write(cur, ptr, true);
        } else if (tlvType == MLE_TYPE_ROUTE) {
            ptr = thread_route_option_write(cur, ptr);
        } else if (tlvType == MLE_TYPE_ADDRESS16) {
            ptr = mle_tlv_write_short_address(ptr, shortAddress);
        } else if (tlvType == MLE_TYPE_RSSI) {

        }
    }

    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    //SET Destination address
    mle_service_set_msg_destination_address(bufId, mle_msg->packet_src_address);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    mle_service_send_message(bufId);
    return 0;
}

static int thread_router_accept_to_endevice(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t *challenge, uint8_t chalLen)
{
    mle_message_timeout_params_t timeout;
    uint16_t bufId = mle_service_msg_allocate(cur->id, 64, false, MLE_COMMAND_ACCEPT);
    if (bufId == 0) {
        return -1;
    }

    if (addr_is_ipv6_multicast(mle_msg->packet_src_address)) {
        timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
    } else {
        timeout.delay = MLE_NO_DELAY;
    }
    tr_debug("MLE Router Link Request response to REED or ED");

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;

    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    //SET MLE Frame Counter
    ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
    ptr = mle_tlv_write_response(ptr, challenge, chalLen);
    ptr = thread_leader_data_tlv_write(ptr, cur);
    ptr = mle_general_write_source_address(ptr, cur);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    //SET Destination address
    mle_service_set_msg_destination_address(bufId, mle_msg->packet_src_address);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);
    mle_service_send_message(bufId);
    return 0;

}

static int thread_router_accept_request_build(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint16_t shortAddress, uint8_t *challenge, uint8_t chalLen, uint8_t type, bool rssi_tlv, uint8_t rssi)
{
    mle_message_timeout_params_t timeout;
    uint16_t bufId;
    (void)shortAddress;
    uint32_t keySequence;
    if (type == MLE_COMMAND_ACCEPT) {
        timeout.retrans_max = 0;
        timeout.timeout_init = 0;
        timeout.timeout_max = 0;
        bufId = mle_service_msg_allocate(cur->id, 128, false, type);
    } else {
        timeout.retrans_max = 1;
        timeout.timeout_init = 2;
        timeout.timeout_max = 3;
        bufId = mle_service_msg_allocate(cur->id, 128, true, type);
    }
    if (bufId == 0) {
        return -1;
    }

    if (addr_is_ipv6_multicast(mle_msg->packet_dst_address)) {
        timeout.delay = MLE_HALF_SECOND_MAX_DELAY;
    } else {
        timeout.delay = MLE_NO_DELAY;
    }

    tr_debug("MLE Router Link Request response");

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    ptr = thread_leader_data_tlv_write(ptr, cur);
    if (rssi_tlv) {
        ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
        ptr = mle_tlv_rssi_tlv(ptr, rssi);
        if (type != MLE_COMMAND_ACCEPT) {
            uint8_t req_tlv = MLE_TYPE_RSSI;
            ptr = mle_tlv_req_tlv(ptr, &req_tlv, 1);

        }
    }

    if (challenge && chalLen) {
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
        //SET MLE Frame Counter
        ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
        ptr = mle_tlv_write_response(ptr, challenge, chalLen);
    }

    ptr = mle_general_write_source_address(ptr, cur);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, mle_msg->packet_src_address);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return 0;

}

static uint16_t thread_tlv_len(protocol_interface_info_entry_t *cur, uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t mode)
{
    // Calculates the extra length for bigger TLVs it is assumed that smaller ones fit in extra reserved space
    uint16_t len = 0;
    for (uint16_t i = 0; i < tlv_len; i++) {
        if (*tlv_ptr == MLE_TYPE_ROUTE) {
            len +=  thread_route_option_size(cur);
        } else if (*tlv_ptr == MLE_TYPE_NETWORK_DATA) {
            len += 2 + thread_network_data_tlv_size(cur, mode & MLE_THREAD_REQ_FULL_DATA_SET);
        } else if (*tlv_ptr == MLE_TYPE_ADDRESS16) {
            //Short Address
            len += 4;
        }
        tlv_ptr++;
    }
    return len;
}

static uint8_t *thread_tlv_add(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint8_t tlv_type, uint8_t mode)
{
    struct link_configuration *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return ptr;
    }

    switch (tlv_type) {
        case MLE_TYPE_SRC_ADDRESS:
            /* Add a MLE Source Address TLV */
            ptr = mle_general_write_source_address(ptr, cur); /* TODO cur normally first in param list */
            break;
        case MLE_TYPE_MODE: {
            /* Add a MLE Mode TLV */
            *ptr++ = MLE_TYPE_MODE;
            *ptr++ = 1;
            *ptr++ = thread_mode_get_by_interface_ptr(cur);
        }
        break;
        case MLE_TYPE_TIMEOUT:
            /* Add a MLE Timeout TLV */
            ptr = mle_tlv_write_timeout(ptr, cur->thread_info->host_link_timeout);
            break;
        case MLE_TYPE_LL_FRAME_COUNTER:
            /* Add a MLE Timeout TLV */
            ptr = mle_general_write_link_layer_framecounter(ptr, cur);
            break;
        case MLE_TYPE_NWK_PARAM:
            /* Add a MLE Network Parameter TLV */
            /* TODO */
            break;
        case MLE_TYPE_MLE_FRAME_COUNTER:
            /* Add a MLE MLE Frame Counter TLV */
            /* TODO */
            /* Not ostensibly used in Thread */
            break;
        case MLE_TYPE_ROUTE:
            /* Add a MLE Router Table TLV */
            ptr = thread_route_option_write(cur, ptr);
            break;
        case MLE_TYPE_ADDRESS16:
            /* Add a MLE Short Address TLV */
            /* TODO */
            break;
        case MLE_TYPE_LEADER_DATA:
            ptr = thread_leader_data_tlv_write(ptr, cur);
            break;
        case MLE_TYPE_NETWORK_DATA:
            ptr = thread_network_data_tlv_write(cur, ptr, mode & MLE_THREAD_REQ_FULL_DATA_SET);
            break;
        case MLE_TYPE_SCAN_MASK:
            break;
        case MLE_TYPE_CONNECTIVITY:
            break;
        case MLE_TYPE_RSSI:
            break;
        default:
            /* No other TLV type can be requested */
            /* LQI possibly - but not for thread */
            break;
    }
    return ptr;
}

static int thread_child_update_response(protocol_interface_info_entry_t *cur, uint8_t *dst_address, uint8_t mode, uint16_t short_address, uint32_t timeout, mle_tlv_info_t *addressRegisterTlv, mle_tlv_info_t *tlvReq, mle_tlv_info_t *challengeTlv, uint64_t active_timestamp, uint64_t pending_timestamp)
{
    uint16_t i;
    uint16_t len = 64;
    uint32_t keySequence;
    bool add_active_configuration = false;
    bool add_pending_configuration = false;
    uint64_t own_pending_timestamp = 0;
    uint8_t *ptr;
    if (!thread_info(cur)) {
        return -1;
    }
    link_configuration_s *link_configuration;
    link_configuration = thread_joiner_application_get_config(cur->id);

    if (!link_configuration) {
        return -1;
    }

    len += 10; // active timestamp tlv size
    len += thread_pending_timestamp_tlv_size(cur);

    if (!active_timestamp || active_timestamp != link_configuration->timestamp) {
        len += thread_active_operational_dataset_size(cur);
        add_active_configuration = true;
    }
    own_pending_timestamp = thread_joiner_application_pending_config_timestamp_get(cur->id);
    // if pending config is not in sync from requested device
    if (!pending_timestamp ||
            (own_pending_timestamp && own_pending_timestamp != pending_timestamp)) {
        len += thread_pending_operational_dataset_size(cur);
        add_pending_configuration = true;
    }
    if (tlvReq && tlvReq->tlvLen) {
        mle_tlv_ignore(tlvReq->dataPtr, tlvReq->tlvLen, MLE_TYPE_LL_FRAME_COUNTER);
        len += thread_tlv_len(cur, tlvReq->dataPtr, tlvReq->tlvLen, mode);
    }
    if (addressRegisterTlv && addressRegisterTlv->tlvLen) {
        len += addressRegisterTlv->tlvLen;
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, len, false, MLE_COMMAND_CHILD_UPDATE_RESPONSE);

    if (bufId == 0) {
        return -1;
    }

    tr_debug("MLE Child synch response");

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    ptr = mle_service_get_data_pointer(bufId);

    ptr = mle_tlv_write_mode(ptr, mode); //Write Mode Allways

    if (addressRegisterTlv && addressRegisterTlv->tlvLen) {
        *ptr++ = MLE_TYPE_ADDRESS_REGISTRATION;
        *ptr++ = addressRegisterTlv->tlvLen;
        memcpy(ptr, addressRegisterTlv->dataPtr, addressRegisterTlv->tlvLen);
        ptr += addressRegisterTlv->tlvLen;
    }
    //Set SRC
    ptr = mle_general_write_source_address(ptr, cur);
    //SET leader data
    ptr = thread_leader_data_tlv_write(ptr, cur);
    if (timeout) {
        ptr = mle_tlv_write_timeout(ptr, timeout);
    }

    if (challengeTlv && challengeTlv->tlvLen) {
        ptr = mle_tlv_write_response(ptr, challengeTlv->dataPtr, challengeTlv->tlvLen);
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
        //SET MLE Frame Counter
        ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
    }

    if (tlvReq && tlvReq->tlvLen) {
        /* Go through the requested TLV mask */
        for (i = 0; i < tlvReq->tlvLen; i++) {
            ptr = thread_tlv_add(cur, ptr, tlvReq->dataPtr[i], mode);
        }
        if (mle_tlv_requested(tlvReq->dataPtr, tlvReq->tlvLen, MLE_TYPE_ADDRESS16)) {
            ptr = mle_tlv_write_short_address(ptr, short_address);
        }
        if (mle_tlv_requested(tlvReq->dataPtr, tlvReq->tlvLen, MLE_TYPE_NETWORK_DATA)) {
            ptr = thread_active_timestamp_write(cur, ptr);
            ptr = thread_pending_timestamp_write(cur, ptr);
            if (add_active_configuration) {
                ptr = thread_active_operational_dataset_write(cur, ptr);
            }
            if (add_pending_configuration) {
                ptr = thread_pending_operational_dataset_write(cur, ptr);
            }
        }
    }

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, dst_address);
    mle_service_send_message(bufId);
    return 0;

}

static int mle_build_and_send_data_response_msg(protocol_interface_info_entry_t *cur, uint8_t *dst_address, uint8_t *data_ptr, uint16_t data_len, mle_tlv_info_t *request_tlv, uint8_t mode)
{
    uint16_t length = 64 + 20 + 4; // source address 4 bytes
    uint8_t *ptr;
    uint_fast16_t i;
    uint64_t active_timestamp = 0;
    uint64_t pending_timestamp = 0;
    bool add_active_configuration = false;
    bool add_pending_configuration = false;
    bool active_timestamp_present_in_request = false;
    bool pending_timestamp_present_in_request = false;
    uint64_t own_pending_timestamp = 0;
    link_configuration_s *link_configuration;
    link_configuration = thread_joiner_application_get_config(cur->id);

    if (!link_configuration) {
        return 0;
    }

    active_timestamp_present_in_request = mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, data_ptr, data_len, &active_timestamp);
    pending_timestamp_present_in_request = mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, data_ptr, data_len, &pending_timestamp);

    length += thread_pending_timestamp_tlv_size(cur);

    if (!active_timestamp_present_in_request || active_timestamp != link_configuration->timestamp) {
        length += thread_active_operational_dataset_size(cur);
        add_active_configuration = true;
    }
    own_pending_timestamp = thread_joiner_application_pending_config_timestamp_get(cur->id);
    // if pending config is not in sync from requested device
    if (!pending_timestamp_present_in_request ||
            (own_pending_timestamp && own_pending_timestamp != pending_timestamp)) {
        length += thread_pending_operational_dataset_size(cur);
        add_pending_configuration = true;
    }

    // Leader data is added by default
    mle_tlv_ignore(request_tlv->dataPtr, request_tlv->tlvLen, MLE_TYPE_LEADER_DATA);
    length += thread_tlv_len(cur, request_tlv->dataPtr, request_tlv->tlvLen, mode);

    //link metrics info
    length += thread_lowpower_link_metrics_length(cur, dst_address);

    uint16_t bufId =  mle_service_msg_allocate(cur->id, length, false, MLE_COMMAND_DATA_RESPONSE);

    if (bufId == 0) {
        return -1;
    }

    tr_debug("Send MLE data response, %s mode=%x", trace_ipv6(dst_address), mode);
    ptr = mle_service_get_data_pointer(bufId);

    ptr = thread_leader_data_tlv_write(ptr, cur);
    /* Go through the requested TLV mask */
    for (i = 0; i < request_tlv->tlvLen; i++) {
        ptr = thread_tlv_add(cur, ptr, request_tlv->dataPtr[i], mode);
    }

    ptr = thread_active_timestamp_write(cur, ptr);
    ptr = thread_pending_timestamp_write(cur, ptr);
    ptr = mle_general_write_source_address(ptr, cur);
    if (add_active_configuration) {
        ptr = thread_active_operational_dataset_write(cur, ptr);
    }
    if (add_pending_configuration) {
        ptr = thread_pending_operational_dataset_write(cur, ptr);
    }
    ptr = thread_lowpower_link_metrics_write(cur, dst_address, ptr);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, dst_address);
    //Set Security
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    mle_service_send_message(bufId);
    return 0;


}

//this responds with zeros currently, needs to be updated after results are computed

static int thread_attach_parent_response_build(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, uint8_t *challenge, uint16_t chalLen, uint8_t linkMargin, uint8_t scanMask, uint8_t mode)
{
    /**
     * This Message need to add Next Items:
     * - MLE_TYPE_SRC_ADDRESS
     * - MLE_TYPE_LEADER_DATA
     * - MLE_TYPE_LL_FRAME_COUNTER
     * - MLE_TYPE_MLE FRAME_COUNTER
     * - MLE_TYPE_CHALLENGE
     * - MLE_TYPE_RESPONSE
     * - MLE_TYPE_CONNECTIVITY
     * - MLE_TYPE_RSSI
     * - MLE_TYPE_VERSION
     */

    mle_message_timeout_params_t timeout;

    uint16_t bufId = mle_service_msg_allocate(cur->id, 128, true, MLE_COMMAND_PARENT_RESPONSE);
    if (bufId == 0) {
        return -1;
    }
    tr_debug("Build MLE Parent response");

    uint32_t keySequence;
    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    //Set Security
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    //SET Leader Data
    ptr = thread_leader_data_tlv_write(ptr, cur);

    // SET Connectivity TLV
    ptr = thread_connectivity_tlv_write(ptr, cur, mode);

    //SET LL Frame Counter
    ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    //SET MLE Frame Counter
    ptr = mle_tlv_write_framecounter(ptr, mle_service_security_get_frame_counter(cur->id));
    //SET Source Address
    ptr = mle_general_write_source_address(ptr, cur);
    //SET Response
    if (challenge != NULL) {
        ptr = mle_tlv_write_response(ptr, challenge, chalLen);
    }
    //RSSI
    ptr = mle_tlv_rssi_tlv(ptr, linkMargin);
    //Set Version
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 1;
    timeout.timeout_init = 6;
    timeout.timeout_max = 6;

    switch (scanMask & 0xc0) {
        case 0x80:
            timeout.delay = MLE_HALF_SECOND_MAX_DELAY;
            break;
        default:
            timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
            break;
    }

    //SET Destination address
    mle_service_set_msg_destination_address(bufId, dstAddress);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return 0;
}

int thread_router_bootstrap_reset_child_info(protocol_interface_info_entry_t *cur, struct mac_neighbor_table_entry *child)
{
    /* Cleanup occurs for /any/ link we lose to something that looks like a child address,
     * not just links that are /now/ our children.
     * Due to REED/partition transitions the address may not look like a current child address;
     * we could be holding a child entry for future repromotion to router with same ID.
     */
    if (thread_is_router_addr(child->mac16) || child->mac16 >= 0xfffe) {
        return -1;
    }
    tr_debug("Child free %x", child->mac16);
    thread_dynamic_storage_child_info_clear(cur->id, child);

    // If Child's RLOC16 appears in the Network Data send the RLOC16 to the Leader
    if (thread_network_data_services_registered(&cur->thread_info->networkDataStorage, child->mac16)) {
        tr_debug("Remove references to Child's RLOC16 from the Network Data");
        thread_management_client_network_data_unregister(cur->id, child->mac16);
    }

    // Clear all (sleepy) child registrations to multicast groups
    thread_child_mcast_entries_remove(cur, child->mac64);

    return 0;
}

void thread_router_bootstrap_child_information_clear(protocol_interface_info_entry_t *cur)
{
    /* make sure that the child info (from previous partition if any)
        is cleared if no router address is got from leader */

    if (!cur->thread_info) {
        return;
    }

    // Remove mle neighbour entries for children in previous partition
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach_safe(mac_neighbor_table_entry_t, table_entry, mac_table_list) {
        if (table_entry->mac16 < 0xfffe && !thread_is_router_addr(table_entry->mac16)) {
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), table_entry);
        }
    }
}
static void thread_router_bootstrap_invalid_child_information_clear(protocol_interface_info_entry_t *cur, uint16_t router_rloc)
{

    tr_debug("Thread Short address changed old: %x new: %x", cur->thread_info->routerShortAddress, router_rloc);

    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    // scrub neighbours with child addresses that are not ours
    ns_list_foreach_safe(mac_neighbor_table_entry_t, table_entry, mac_table_list) {
        if (table_entry->mac16 < 0xfffe &&
                !thread_is_router_addr(table_entry->mac16) &&
                thread_router_addr_from_addr(table_entry->mac16) != router_rloc) {
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), table_entry);
        }
    }
}

static void thread_bootstrap_client_router_id_cb(int8_t interface_id, int8_t status, uint16_t router_rloc, const uint8_t router_mask_ptr[9])
{
    uint8_t ml16[16];
    protocol_interface_info_entry_t *cur;
    uint8_t parent_router_id = FAST_ROUTE_NO_ROUTE;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    tr_debug("RouterID CB");
    if (!cur) {
        return;
    }
    if (!cur->thread_info->routerIdRequested) {
        return;
    }
    cur->thread_info->routerIdRequested = false;

    if (cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER ||
            cur->thread_info->leader_private_data) {
        // Test api can cause these states or some out of sync operation.
        tr_warn("Router address for non-REED");
        return;
    }

    if (status < 0) {
        thread_bootstrap_router_id_get_fail(cur);
        return;
    }

    uint8_t routeId = *router_mask_ptr++;

    if (cur->thread_info->thread_endnode_parent) {
        parent_router_id = cur->thread_info->thread_endnode_parent->router_id;
    }

    thread_router_bootstrap_invalid_child_information_clear(cur, router_rloc);

    // Release network data from old address
    cur->thread_info->localServerDataBase.release_old_address = true;

    //ADD New ML16
    // This should be used thread_bootstrap_update_ml16_address(cur, router_rloc);
    thread_clean_old_16_bit_address_based_addresses(cur);
    mac_helper_mac16_address_set(cur, router_rloc);
    cur->thread_info->routerShortAddress = router_rloc;
    memcpy(ml16, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(&ml16[8], ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(router_rloc, &ml16[14]);

    cur->global_address_available = true;
    //Register UL16
    if_address_entry_t *def_address = addr_add(cur, ml16, 64, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0xffffffff, true);
    if (!def_address) {
        return;
    }

    //  /* XXX Is short_src_adr ever reset? Is it undefined if info not in msg? */
    tr_debug("Route seq %d Router mask: %s", routeId, trace_array(router_mask_ptr, 8));
    cur->thread_info->routing.router_id_sequence_valid = false;
    if (parent_router_id != FAST_ROUTE_NO_ROUTE) {
        thread_routing_force_next_hop(cur, routeId, router_mask_ptr, parent_router_id);
    } else {
        tr_warn("No parent information available, no initial route set.");
        thread_routing_update_id_set(cur, routeId, router_mask_ptr);
    }
    thread_bootstrap_attached_active_router(cur);

}

void thread_router_bootstrap_router_id_request(protocol_interface_info_entry_t *cur, uint8_t status)
{
    int router_id_req_status;
    tr_debug("Router ID Request");
    if (cur->thread_info->routerIdRequested) {
        tr_warn("Router ID already requested");
        return;
    }

    router_id_req_status = thread_management_client_router_id_get(cur->id, cur->mac, cur->thread_info->routerShortAddress, thread_bootstrap_client_router_id_cb, status);
    tr_debug("RouterIDReq COAP ID=%d", router_id_req_status);
    if (router_id_req_status > 0) {
        cur->thread_info->routerIdRequested = true;
    }
}

static int mle_attach_child_id_response_build(protocol_interface_info_entry_t *cur, uint8_t *dstAddress, thread_pending_child_id_req_t *child_req, struct mac_neighbor_table_entry *neigh_info)
{
    uint16_t len = 12 + 4; //Leader data and short address
    uint8_t *ptr;
    bool fullList = false;
    uint64_t pending_timestamp;
    struct link_configuration *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    if (thread_neighbor_class_request_full_data_setup(&cur->thread_info->neighbor_class, neigh_info->index)) {
        fullList = true;
    }

    if (child_req->shortAddressReq) {
        //Short Address
        len += 4;
    }

    if (child_req->networkDataReq) {
        len += 2 + thread_network_data_tlv_size(cur, fullList);
    }

    if (child_req->routeReq) {
        len += thread_route_option_size(cur);
    }
    if (child_req->request_active_config || child_req->active_timestamp != linkConfiguration->timestamp) {
        len += thread_active_operational_dataset_size(cur);
    }

    //always put active timestamp
    len += 10;
    len += thread_pending_timestamp_tlv_size(cur);
    pending_timestamp = thread_joiner_application_pending_config_timestamp_get(cur->id);
    if (pending_timestamp && pending_timestamp != child_req->pending_timestamp) {
        len += thread_pending_operational_dataset_size(cur);
    }

    mle_message_timeout_params_t timeout;
    uint16_t bufId = mle_service_msg_allocate(cur->id, len, false, MLE_COMMAND_CHILD_ID_RESPONSE);
    if (bufId == 0) {
        return -1;
    }
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    tr_debug("Send MLE Child ID response");

    ptr = mle_service_get_data_pointer(bufId);

    ptr = thread_leader_data_tlv_write(ptr, cur);

    ptr = mle_general_write_source_address(ptr, cur);

    if (child_req->shortAddressReq) {
        ptr = mle_tlv_write_short_address(ptr, neigh_info->mac16);
    }

    if (child_req->routeReq) {
        ptr = thread_route_option_write(cur, ptr);
    }

    if (child_req->networkDataReq) {
        ptr = thread_network_data_tlv_write(cur, ptr, fullList);
    }

    if (child_req->active_timestamp != linkConfiguration->timestamp) {
        ptr = thread_active_operational_dataset_write(cur, ptr);
    }

    //always write active timestamp
    ptr = thread_active_timestamp_write(cur, ptr);
    if (pending_timestamp > 0 && pending_timestamp != child_req->pending_timestamp) {
        ptr = thread_pending_operational_dataset_write(cur, ptr);
    }

    ptr = thread_pending_timestamp_write(cur, ptr);


    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;
    mle_service_set_msg_destination_address(bufId, dstAddress);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return 0;


}

int thread_router_bootstrap_child_max_timeout_get(protocol_interface_info_entry_t *cur, uint32_t *max_child_timeout)
{
    uint16_t router_address = thread_info(cur)->routerShortAddress;
    uint32_t max_timeout = 0;
    if (router_address >= 0xfffe) {
        router_address = mac_helper_mac16_address_get(cur);
    }
    if (router_address & THREAD_CHILD_MASK) {
        return -1; //I am child
    }
    if (router_address >= 0xfffe) {
        return -1;
    }
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (thread_router_addr_from_addr(cur_entry->mac16) == router_address &&
                !cur_entry->ffd_device && cur_entry->lifetime > max_timeout) {
            max_timeout = cur_entry->lifetime;
        }
    }
    *max_child_timeout = max_timeout;
    return 0;
}

uint16_t thread_router_bootstrap_child_count_get(protocol_interface_info_entry_t *cur)
{
    uint16_t child_count = 0;
    uint16_t router_address = thread_info(cur)->routerShortAddress;
    if (router_address >= 0xfffe) {
        router_address = mac_helper_mac16_address_get(cur);
    }
    if (router_address & THREAD_CHILD_MASK) {
        return 0; //I am child
    }
    if (router_address >= 0xfffe) {
        return 0;
    }
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (thread_router_addr_from_addr(cur_entry->mac16) == router_address) {
            child_count++;
        }
    }
    return child_count;
}

static uint16_t thread_router_bootstrap_child_address_generate(protocol_interface_info_entry_t *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    if (thread_router_bootstrap_child_count_get(cur) >= cur->thread_info->maxChildCount) {
        tr_info("Maximum count %d reached", cur->thread_info->maxChildCount);
        return 0xfffe;
    }

    bool address_allocated = false;
    for (uint16_t i = 0; i < cur->thread_info->maxChildCount; i++) {
        address_allocated = false;
        cur->thread_info->lastAllocatedChildAddress = (cur->thread_info->lastAllocatedChildAddress % THREAD_MAX_CHILD_ID_COUNT) + 1;
        ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
            if ((cur_entry->mac16 & THREAD_CHILD_MASK) == cur->thread_info->lastAllocatedChildAddress) {
                address_allocated = true;
                break;
            }
        }
        if (!address_allocated) {
            break;
        }
    }
    if (address_allocated) {
        // all possible addresses already allocated
        return 0xfffe;
    }
    return ((mac_helper_mac16_address_get(cur) & THREAD_ROUTER_MASK) | cur->thread_info->lastAllocatedChildAddress);
}

static bool thread_child_id_request(protocol_interface_info_entry_t *cur, struct mac_neighbor_table_entry *entry_temp)
{
    //Remove All Short address links from router
    if (entry_temp->mac16 < 0xfffe) {
        protocol_6lowpan_release_short_link_address_from_neighcache(cur, entry_temp->mac16);
    }

    //allocate child address if current is router, 0xffff or not our child
    if (!thread_addr_is_child(mac_helper_mac16_address_get(cur), entry_temp->mac16)) {
        entry_temp->mac16 = thread_router_bootstrap_child_address_generate(cur);
    }

    if (entry_temp->mac16 >= 0xfffe) {
        return false;
    }

    // Store this child info to the NVM
    thread_dynamic_storage_child_info_store(cur, entry_temp);
    //}
    return true;
}

void thread_router_bootstrap_child_id_handler(protocol_interface_info_entry_t *cur)
{
    thread_pending_child_id_req_t *req;
    bool new_neigbour = false;

    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED) {
        if (!cur->thread_info->routerIdRequested) {
            tr_info("Upgrade REED to Router");
            thread_router_bootstrap_router_id_request(cur, THREAD_COAP_STATUS_TLV_HAVE_CHILD_ID_REQUEST);
        }
        return;
    }
    req = thread_child_id_request_entry_get_from_the_list(cur);

    if (!req) {
        tr_debug("Child Id Req pending list empty");
        return;
    }

    if (cur->thread_info->thread_attached_state != THREAD_STATE_CONNECTED_ROUTER) {
        // We are not router can not process
        goto free_request;
    }

    uint8_t ll64[16];
    //set LL64 destination address
    memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&ll64[8], req->euid64, 8);
    ll64[8] ^= 2;
    //Allocate entry
    mac_neighbor_table_entry_t *entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), ll64, true, &new_neigbour);

    if (!entry_temp) {
        //Send link reject
        thread_link_reject_send(cur, ll64);
        goto free_request;
    }

    thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, req->linkMargin, new_neigbour);
    thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);

    // If mac frame couter is less than previous we need to leave the old one

    //Update or set neigh info
    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, req->timeout);
    mle_mode_parse_to_mac_entry(entry_temp, req->mode);
    thread_neighbor_class_mode_parse_to_entry(&cur->thread_info->neighbor_class, entry_temp->index, req->mode);
    entry_temp->connected_device = 1;
    mle_service_frame_counter_entry_add(cur->id, entry_temp->index, req->mleFrameCounter);

    if (req->shortAddressReq) {
        if (!thread_child_id_request(cur, entry_temp)) {
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), entry_temp);
            thread_link_reject_send(cur, ll64);
            goto free_request;
        }
    }
    if (new_neigbour) {
        mlme_device_descriptor_t device_desc;
        mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, req->frameCounter, false);
        mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, req->keyId, new_neigbour);
    } else {
        // in get response handler this will update the short address from MLE table
        mlme_get_t get_req;
        get_req.attr = macDeviceTable;
        get_req.attr_index = entry_temp->index;
        cur->mac_api->mlme_req(cur->mac_api, MLME_GET, &get_req);
    }

    //Register MLEID if RRFD device
    if (!entry_temp->ffd_device) {
        uint8_t tempIPv6Address[16];
        memcpy(tempIPv6Address, thread_info(cur)->threadPrivatePrefixInfo.ulaPrefix, 8);
        memcpy(&tempIPv6Address[8], req->eiid, 8);

        thread_neighbor_class_add_mleid(&cur->thread_info->neighbor_class, entry_temp->index, req->eiid);

        tr_debug("Register %s", trace_ipv6(tempIPv6Address));
        //Register GP --> 16
        thread_nd_address_registration(cur, tempIPv6Address, entry_temp->mac16, cur->mac_parameters->pan_id, entry_temp->mac64, NULL);
    }

    mle_attach_child_id_response_build(cur, ll64, req, entry_temp);

free_request:
    ns_dyn_mem_free(req);
    thread_bootstrap_child_id_request(cur);
}

static void thread_address_registration_tlv_parse(uint8_t *ptr, uint16_t data_length, protocol_interface_info_entry_t *cur, uint16_t mac16, uint8_t *mac64)
{
    lowpan_context_t *ctx;
    uint8_t tempIPv6Address[16];
    uint8_t ctxId;
    bool new_neighbour_created;
    thread_child_mcast_entries_remove(cur, mac64);
    while (data_length) {
        //Read
        ctxId = *ptr++;
        if (ctxId & 0x80) {
            ctxId &= 0x0f;
            ctx = lowpan_context_get_by_id(&cur->lowpan_contexts, ctxId);
            if (ctx) {
                memcpy(tempIPv6Address, ctx->prefix, 8);
                memcpy(&tempIPv6Address[8], ptr, 8);
                tr_debug("Register %s", trace_ipv6(tempIPv6Address));
                //Register GP --> 16
                int retVal = thread_nd_address_registration(cur, tempIPv6Address, mac16, cur->mac_parameters->pan_id, mac64, &new_neighbour_created);
                thread_bootstrap_address_registration(cur, tempIPv6Address, mac64, new_neighbour_created, retVal == -2);
                (void) retVal;
            } else {
                tr_debug("No Context %u", ctxId);
            }
            ptr += 8;
            data_length -= 9;
        } else {
            tr_debug("Register %s", trace_ipv6(ptr));

            if (addr_is_ipv6_multicast(ptr)) {
                // Register multicast address (link-local & higher)
                if (addr_ipv6_multicast_scope(ptr) >= IPV6_SCOPE_LINK_LOCAL) {
                    addr_add_group(cur, ptr);
                    if (thread_child_mcast_entry_get(cur, ptr, mac64)) {
                        tr_debug("Added sleepy multicast registration entry.");
                    }
                }
            } else {
                //Register GP --> 16
                int retVal = thread_nd_address_registration(cur, ptr, mac16, cur->mac_parameters->pan_id, mac64, &new_neighbour_created);
                thread_bootstrap_address_registration(cur, ptr, mac64, new_neighbour_created, retVal == -2);
                (void) retVal;
            }

            ptr += 16;
            data_length -= 17;
        }
    }
}

void thread_router_bootstrap_mle_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    (void) interface_id;
    thread_leader_data_t leaderData;
    mac_neighbor_table_entry_t *entry_temp;
    bool rssiTLV;
    bool leaderDataReceived;
    protocol_interface_info_entry_t *cur = mle_msg->interface_ptr;

    //TLV REQUSTED
    if (mle_tlv_type_requested(MLE_TYPE_RSSI, mle_msg->data_ptr, mle_msg->data_length)) {
        rssiTLV = true;
    } else {
        rssiTLV = false;
    }

    if (thread_leader_data_parse(mle_msg->data_ptr, mle_msg->data_length, &leaderData)) {
        leaderDataReceived = true;
    } else {
        leaderDataReceived = false;
    }


    uint8_t linkMargin = thread_compute_link_margin(mle_msg->dbm);
    tr_debug("Thread MLE REED Handler");
    switch (mle_msg->message_type) {
        case MLE_COMMAND_PARENT_REQUEST: {
            uint8_t scanMask, mode;
            uint16_t version;
            mle_tlv_info_t challengeTlv;
            tr_info("Recv MLE Parent Request");

            if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY) {
                // If own attach is ongoing, do not process parent request
                return;
            }

            if (security_headers->KeyIdMode != MAC_KEY_ID_MODE_SRC4_IDX) {
                tr_debug("Wrong key mode %u ", security_headers->KeyIdMode);
                return;
            }

            if (!thread_router_bootstrap_routing_allowed(cur)) {
                tr_debug("R bit is off in security policy; drop packet");
                return;
            }

            // check if security policy prevents sending of parent response
            if (!thread_router_bootstrap_is_reed_upgrade_allowed(cur)) {
                tr_debug("Security policy prevents parent response; drop packet");
                return;
            }

            if (thread_am_reed(cur)) {
                // If we are in REED mode and receive PARENT_REQ from our parent, don't send response.
                if (thread_router_parent_address_check(cur, mle_msg->packet_src_address)) {
                    tr_debug("Drop PARENT_REQ from own parent");
                    return;
                }
            }

            // parent request received
            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, false, NULL);
            if (entry_temp) {
                //Set MAC modes
                mle_mode_parse_to_mac_entry(entry_temp, (MLE_FFD_DEV | MLE_RX_ON_IDLE));
                thread_neighbor_class_mode_parse_to_entry(&cur->thread_info->neighbor_class, entry_temp->index, MLE_THREAD_REQ_FULL_DATA_SET);
                thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, false);
                thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
            }
            if (!entry_temp) {
                if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, mle_msg->data_ptr, mle_msg->data_length, &mode)) {
                    tr_debug("NO Mode");
                    return;
                }

                // New child trying to attach check if we have room
                if (mle_service_interface_tx_queue_size(cur->id) > THREAD_MAX_PARALLEL_MLE_PARENT_REQUEST) {
                    tr_info("Drop MLE message: too many simultaneous MLE messages");
                    return;
                }

                if (mle_class_free_entry_count_get(cur) < 1) {
                    tr_info("Drop MLE message: no space left in the MLE table");
                    return;
                }

                if ((mode & MLE_DEV_MASK) == MLE_RFD_DEV && mle_class_rfd_entry_count_get(cur) >= THREAD_MAX_MTD_CHILDREN) {
                    tr_info("Drop MLE message: maximum MTD child count reached.");
                    return;
                }

                if (!(mode & MLE_RX_ON_IDLE) && mle_class_sleepy_entry_count_get(cur) >= THREAD_MAX_SED_CHILDREN) {
                    tr_info("Drop MLE message: maximum SED child count reached.");
                    return;
                }

                if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER &&
                        thread_router_bootstrap_child_count_get(cur) >= cur->thread_info->maxChildCount) {
                    tr_info("Drop MLE message: maximum Child count reached (%d)", cur->thread_info->maxChildCount);
                    return;
                }
            }

            if (thread_route_ready_to_leader(cur) != 0) {
                tr_debug("Leader Path infinite");
                return;
            }

            /**
                 * MLE attached First packet Mandatory TLV:s are validated at this function
                 * - MLE_TYPE_SCAN_MASK
                 * - MLE_TYPE_CHALLENGE
                 * - MLE_TYPE_VERSION
                 */
            //Validate Mask
            if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_SCAN_MASK, mle_msg->data_ptr, mle_msg->data_length, &scanMask)) {
                tr_debug("NO Scan mask");
                return;
            }

            if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, mle_msg->data_ptr, mle_msg->data_length, &mode)) {
                tr_debug("NO Mode");
                return;
            }

            if (!thread_scan_mask_validation(cur, scanMask)) {
                tr_debug("Not my type");
                return;
            }

            if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED &&
                    31 < thread_routing_count_active_routers(&cur->thread_info->routing)) {
                tr_info("No possibility to upgrade to router");
                return;
            }

            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) {
                tr_debug("NO version");
                return;
            }

            //Read Challenge for Build Response
            if (!mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv)) {
                tr_debug("No challenge");
                return;
            }

            if (thread_attach_parent_response_build(cur, mle_msg->packet_src_address, challengeTlv.dataPtr, challengeTlv.tlvLen, linkMargin, scanMask, mode) == 0) {
                tr_debug("Send MLE Parent response");
            }
            break;
        }

        case MLE_COMMAND_CHILD_ID_REQUEST:
            tr_info("Recv MLE Child ID Request from %s", trace_ipv6(mle_msg->packet_src_address));
            if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY) {
                // If own attach is ongoing, do not process child ID req
                return;
            }

            if (!thread_router_bootstrap_routing_allowed(cur)) {
                tr_debug("R bit is off in security policy; drop packet");
                return;
            }

            // check if security policy prevents sending of child id response
            if (!thread_router_bootstrap_is_reed_upgrade_allowed(cur)) {
                tr_debug("Security policy prevents child id response; drop packet");
                return;
            }

            if (thread_am_reed(cur)) {
                // If we are in REED mode and receive child ID request from our parent, call connection error.
                if (thread_router_parent_address_check(cur, mle_msg->packet_src_address)) {
                    tr_debug("Child ID req from own parent -> connection error");
                    entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, false, NULL);
                    if (entry_temp) {
                        mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), entry_temp);
                    }
                    thread_bootstrap_connection_error(cur->id, CON_PARENT_CONNECT_DOWN, NULL);
                    return;
                }
            }

            if (security_headers->KeyIdMode != MAC_KEY_ID_MODE_SRC4_IDX) {
                tr_debug("Wrong key mode %u ", security_headers->KeyIdMode);
                return;
            } else {
                thread_pending_child_id_req_t *id_req;
                uint8_t tlvType;
                mle_tlv_info_t tlvRequest, addressRegisteredTlv;
                uint8_t *t_ptr;

                uint16_t messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

                if (messageId == 0) {
                    tr_debug("No matching challenge");
                    return;
                }

                /* Perform key synchronization */
                thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));

                //Free Response
                mle_service_msg_free(messageId);

                mle_msg->packet_src_address[8]  ^= 2;
                id_req = thread_child_id_request_entry_get(cur, (mle_msg->packet_src_address + 8));
                mle_msg->packet_src_address[8]  ^= 2;

                if (!id_req) {
                    tr_debug("No room for child id req");
                    return;
                }

                /**
                     * MLE attached Tree packet Mandatory TLV:s are validated at this function
                     * - MLE_TYPE_TIMEOUT
                     * - MLE_TYPE_MODE
                     * - MLE_TYPE_LL_FRAME_COUNTER
                     * - MLE_TYPE_TLV_REQUEST
                     * - MLE_TYPE_VERSION
                     * Optional:
                     * - MLE_TYPE_ADDRESS_REGISTRATION
                     * - MLE_TYPE_MLE_FRAME_COUNTER
                     */

                if ((!mle_tlv_read_32_bit_tlv(MLE_TYPE_TIMEOUT, mle_msg->data_ptr, mle_msg->data_length, &id_req->timeout)) ||
                        (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, mle_msg->data_ptr, mle_msg->data_length, &id_req->mode)) ||
                        (!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &id_req->version)) ||
                        (!mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &id_req->frameCounter)) ||
                        (!mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &tlvRequest))) {
                    thread_child_id_request_entry_remove(cur, id_req);
                    tr_debug("Illegal child id req");
                    return;
                }
                //If MLE MLE_TYPE_MLE_FRAME_COUNTER TLV is present then use it for validating further messages else use link layer frame counter
                if (!mle_tlv_read_32_bit_tlv(MLE_TYPE_MLE_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &id_req->mleFrameCounter)) {
                    id_req->mleFrameCounter = id_req->frameCounter;
                }

                t_ptr = tlvRequest.dataPtr;

                for (uint8_t i = 0; i < tlvRequest.tlvLen; i++) {
                    tlvType = *t_ptr++;
                    if (tlvType == MLE_TYPE_ADDRESS16) {
                        id_req->shortAddressReq = true;
                    } else if (tlvType == MLE_TYPE_NETWORK_DATA) {
                        id_req->networkDataReq = true;
                    } else if (tlvType == MLE_TYPE_ROUTE) {
                        id_req->routeReq = true;
                    }
                }
                if (mle_tlv_read_tlv(MLE_TYPE_ADDRESS_REGISTRATION, mle_msg->data_ptr, mle_msg->data_length, &addressRegisteredTlv)) {
                    uint8_t context;
                    t_ptr = addressRegisteredTlv.dataPtr;
                    context = *t_ptr++;
                    if (context & 0x80) {
                        context &= 0x0f;
                        if (addressRegisteredTlv.tlvLen == 9 && (context == 0)) {
                            memcpy(id_req->eiid, t_ptr, 8);
                        }
                    } else {
                        t_ptr += 8;
                        memcpy(id_req->eiid, t_ptr, 8);
                    }
                }

                if (!(id_req->mode & MLE_FFD_DEV) && addressRegisteredTlv.tlvLen == 0) {
                    tr_debug("No address registration TLV in MTD child id request");
                    thread_child_id_request_entry_remove(cur, id_req);
                    return;
                }

                id_req->keyId = security_headers->KeyIndex;
                id_req->keySeq = common_read_32_bit(security_headers->Keysource);
                id_req->linkMargin = linkMargin;

                id_req->active_timestamp = 0;
                id_req->request_active_config = false;
                if (!mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &id_req->active_timestamp)) {
                    id_req->request_active_config = true;
                }
                if (!mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &id_req->pending_timestamp)) {
                    id_req->pending_timestamp = 0;
                }

                //Push Event to queue
                if (thread_bootstrap_child_id_request(cur) != 0) {
                    thread_child_id_request_entry_remove(cur, id_req);
                }

                break;
            }

        case MLE_COMMAND_REQUEST: {
            uint16_t shortAddress, version;
            mle_tlv_info_t addressRegisteredTlv, requestTlv, challengeTlv;
            uint8_t response_type = MLE_COMMAND_ACCEPT_AND_REQUEST;
            //Router attachment
            tr_info("Recv Router Link request");

            if (!thread_attach_active_router(cur)) {
                return; //Do respond until we are reed
            }

            if (!mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv)) {
                return;
            }
            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) {
                return;
            }
            if (!mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &requestTlv)) {
                /**
                 * This cuold cause problem if any dummy coder add this to message
                 * Validate is it REED or End device message link synch Request
                 * - MLE_TYPE_SRC_ADDRESS
                 * - MLE_TYPE_VERSION
                 * - MLE_TYPE_CHALLENGE
                 */
                if (mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &shortAddress)) {
                    //Correct TLV's lets response
                    if (!thread_is_router_addr(shortAddress)) {
                        if (leaderDataReceived && thread_partition_match(cur, &leaderData)) {
                            //REED or end device send response
                            thread_router_accept_to_endevice(cur, mle_msg, challengeTlv.dataPtr, challengeTlv.tlvLen);
                        } else {
                            tr_debug("Drop Link Request from REED/ED in a different partition.");
                            // Do nothing... ignore.
                        }
                    }
                }
                return;
            }

            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, false, NULL);

            if (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &shortAddress)) {

                /**
                 * MLE Router Synch mandatory options are
                 * - MLE_TYPE_VERSION
                 * - MLE_TYPE_CHALLENGE
                 * - MLE_TYPE_TLV_REQUEST
                 *
                 */

                if (entry_temp && entry_temp->connected_device) {
                    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, entry_temp->link_lifetime);
                    thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, false);
                    thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
                    thread_router_synch_accept_request_build(cur, mle_msg, entry_temp->mac16, challengeTlv.dataPtr, challengeTlv.tlvLen, requestTlv.dataPtr, requestTlv.tlvLen);
                }
            }
            bool update_mac_mib = false;
            /**
             * MLE attached First packet Mandatory TLV:s are validated at this function
             * - MLE_TYPE_SRC_ADDRESS
             * - MLE_TYPE_CHALLENGE
             * - MLE_TYPE_LEADER_DATA
             * - MLE_TYPE_VERSION
             * - MLE_TYPE_TLV_REQUEST
             */

            //Read Leader Data and verify connectivity
            if (!leaderDataReceived) {
                return;
            }

            //validate partition id
            if (!thread_partition_match(cur, &leaderData)) {
                tr_debug("Drop link request from wrong partition");
                return;
            }

            if (entry_temp) {
                /*remove from child list when becoming router*/
                // Was this previously our child? If yes, update.
                if ((entry_temp->mac16 & THREAD_CHILD_MASK) && thread_router_addr_from_addr(entry_temp->mac16) == cur->thread_info->routerShortAddress) {
                    thread_dynamic_storage_child_info_clear(cur->id, entry_temp);
                }
                update_mac_mib = true;
                entry_temp->mac16 = shortAddress; // short address refreshed

                if (thread_is_router_addr(shortAddress)) {
                    // Set full data as REED/Router needs full data (SED will not make links)
                    thread_neighbor_class_request_full_data_setup_set(&cur->thread_info->neighbor_class, entry_temp->index, true);
                }

                if (entry_temp->connected_device) {
                    if (mle_tlv_read_tlv(MLE_TYPE_ADDRESS_REGISTRATION, mle_msg->data_ptr, mle_msg->data_length, &addressRegisteredTlv)) {
                        if (!entry_temp->ffd_device) {
                            thread_address_registration_tlv_parse(addressRegisteredTlv.dataPtr, addressRegisteredTlv.tlvLen, cur, entry_temp->mac16, entry_temp->mac64);
                        }
                    }

                    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, entry_temp->link_lifetime);
                    thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, false);
                    thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
                    if (!mle_neigh_entry_frame_counter_update(entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur, security_headers->KeyIndex) &&  update_mac_mib) {
                        //GET
                        mlme_get_t get_req;
                        get_req.attr = macDeviceTable;
                        get_req.attr_index = entry_temp->index;
                        cur->mac_api->mlme_req(cur->mac_api, MLME_GET, &get_req);
                    }
                    response_type = MLE_COMMAND_ACCEPT;
                }
                response_type = MLE_COMMAND_ACCEPT;
            }


            bool accept_link = false;
            if (entry_temp) {
                // We know already so we can accept
                accept_link = true;
            } else if (addr_is_ipv6_multicast(mle_msg->packet_dst_address)) {
                if (thread_bootstrap_link_create_check(cur, shortAddress) &&
                        thread_bootstrap_link_create_allowed(cur, shortAddress, mle_msg->packet_src_address)) {
                    // Multicast and we can respond to message
                    accept_link = true;
                }
            } else if (thread_bootstrap_link_create_check(cur, shortAddress)) {
                // Unicast request and we should make link
                accept_link = true;
            }

            if (accept_link) {
                if (!thread_is_router_addr(shortAddress)) {
                    response_type = MLE_COMMAND_ACCEPT;
                }
                thread_router_accept_request_build(cur, mle_msg, shortAddress, challengeTlv.dataPtr, challengeTlv.tlvLen, response_type, rssiTLV, linkMargin);
            }

            break;
        }

        case MLE_COMMAND_ACCEPT_AND_REQUEST: {
            uint8_t linkMarginfronNeigh;
            uint16_t version, shortAddress;
            uint32_t llFrameCounter;
            mle_tlv_info_t requestTlv, challengeTlv;
            bool createNew, new_entry;
            tr_info("Recv Router Accept & Request");
            uint16_t messageId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);

            if (messageId == 0) {
                tr_debug("No matching challenge");
                return;
            }

            if (!addr_is_ipv6_multicast(mle_service_get_msg_destination_address_pointer(messageId))) {
                //Free Response only if it is unicast
                mle_service_msg_free(messageId);
            }

            if ((!mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv)) ||
                    (!mle_tlv_read_16_bit_tlv(MLE_TYPE_VERSION, mle_msg->data_ptr, mle_msg->data_length, &version)) ||
                    (!mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &requestTlv)) ||
                    (!mle_tlv_read_8_bit_tlv(MLE_TYPE_RSSI, mle_msg->data_ptr, mle_msg->data_length, &linkMarginfronNeigh)) ||
                    (!mle_tlv_read_32_bit_tlv(MLE_TYPE_LL_FRAME_COUNTER, mle_msg->data_ptr, mle_msg->data_length, &llFrameCounter)) ||
                    (!mle_tlv_read_16_bit_tlv(MLE_TYPE_SRC_ADDRESS, mle_msg->data_ptr, mle_msg->data_length, &shortAddress))) {
                thread_link_reject_send(cur, mle_msg->packet_src_address);
                return;
            }

            /* Call to determine whether or not we should create a new link */
            createNew = thread_bootstrap_link_create_check(cur, shortAddress);

            //Send Response

            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, createNew, &new_entry);
            if (entry_temp) {

                thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, new_entry);
                thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
                if (security_headers->KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
                    thread_management_key_synch_req(cur->id, common_read_32_bit(security_headers->Keysource));
                }

                entry_temp->mac16 = shortAddress;
                mlme_device_descriptor_t device_desc;
                mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, llFrameCounter, false);
                mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, security_headers->KeyIndex, new_entry);

                uint32_t link_lifetime;
                if (new_entry) {
                    link_lifetime = THREAD_DEFAULT_LINK_LIFETIME;

                } else {
                    link_lifetime = entry_temp->link_lifetime;
                }

                mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, link_lifetime);

                if (thread_is_router_addr(shortAddress)) {
                    entry_temp->connected_device = 1;
                }

                thread_neighbor_class_request_full_data_setup_set(&cur->thread_info->neighbor_class, entry_temp->index, true);

                thread_routing_update_link_margin(cur, entry_temp->mac16, linkMargin, linkMarginfronNeigh);
                //Read Source address and Challenge
                mac_data_poll_protocol_poll_mode_decrement(cur);
                thread_router_accept_request_build(cur, mle_msg, entry_temp->mac16, challengeTlv.dataPtr, challengeTlv.tlvLen, MLE_COMMAND_ACCEPT, rssiTLV, linkMargin);

                blacklist_update(mle_msg->packet_src_address, true);
            } else {
                thread_link_reject_send(cur, mle_msg->packet_src_address);
            }

            break;
        }

        case MLE_COMMAND_CHILD_UPDATE_REQUEST:
            tr_info("Recv Router Child Update request");
            {
                uint8_t mode;
                uint8_t status;
                uint32_t timeout = 0;
                uint64_t active_timestamp = 0;
                uint64_t pending_timestamp = 0;
                mle_tlv_info_t addressRegisterTlv = {MLE_TYPE_SRC_ADDRESS, 0, 0};
                mle_tlv_info_t challengeTlv = {MLE_TYPE_SRC_ADDRESS, 0, 0};
                mle_tlv_info_t tlv_req = {MLE_TYPE_SRC_ADDRESS, 0, 0};
                entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, false, NULL);

                if (mle_tlv_read_8_bit_tlv(MLE_TYPE_STATUS, mle_msg->data_ptr, mle_msg->data_length, &status)) {
                    if (1 == status && thread_check_is_this_my_parent(cur, entry_temp)) {
                        tr_debug("Parent has removed REED");
                        mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), entry_temp);
                        thread_bootstrap_connection_error(cur->id, CON_PARENT_CONNECT_DOWN, NULL);
                    }
                    return;
                }

                mle_tlv_read_tlv(MLE_TYPE_CHALLENGE, mle_msg->data_ptr, mle_msg->data_length, &challengeTlv);

                if (!entry_temp) {
                    tr_debug("Not Neighbor anymore");
                    thread_host_bootstrap_child_update_negative_response(cur, mle_msg->packet_src_address, &challengeTlv);
                    return;
                }

                if (!mle_tlv_read_8_bit_tlv(MLE_TYPE_MODE, mle_msg->data_ptr, mle_msg->data_length, &mode)) {
                    tr_debug("No Mode");
                    return;
                }

                //Keep alive updated
                thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, false);
                thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
                mle_mode_parse_to_mac_entry(entry_temp, mode);
                thread_neighbor_class_mode_parse_to_entry(&cur->thread_info->neighbor_class, entry_temp->index, mode);

                addressRegisterTlv.tlvType = MLE_TYPE_UNASSIGNED;
                mle_tlv_read_tlv(MLE_TYPE_ADDRESS_REGISTRATION, mle_msg->data_ptr, mle_msg->data_length, &addressRegisterTlv);
                mle_tlv_read_64_bit_tlv(MLE_TYPE_ACTIVE_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &active_timestamp);
                mle_tlv_read_64_bit_tlv(MLE_TYPE_PENDING_TIMESTAMP, mle_msg->data_ptr, mle_msg->data_length, &pending_timestamp);

                mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &tlv_req);

                if (addressRegisterTlv.tlvType == MLE_TYPE_ADDRESS_REGISTRATION &&
                        !entry_temp->ffd_device) {
                    tr_debug("Register child address");
                    thread_address_registration_tlv_parse(addressRegisterTlv.dataPtr, addressRegisterTlv.tlvLen, cur, entry_temp->mac16, entry_temp->mac64);
                }

                if (mle_tlv_read_32_bit_tlv(MLE_TYPE_TIMEOUT, mle_msg->data_ptr, mle_msg->data_length, &timeout)) {

                    tr_debug("Setting child timeout, value=%"PRIu32, timeout);
                    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, timeout);
                } else {
                    mac_neighbor_table_neighbor_refresh(mac_neighbor_info(cur), entry_temp, entry_temp->link_lifetime);
                }

                if (!leaderDataReceived) {
                    tr_debug("Child synch req");
                }

                tr_debug("Keep-Alive -->Respond Child");
                //Response
                thread_child_update_response(cur, mle_msg->packet_src_address, mode, entry_temp->mac16, timeout, &addressRegisterTlv, &tlv_req, &challengeTlv, active_timestamp, pending_timestamp);

            }
            break;

        case MLE_COMMAND_UPDATE:
            tr_info("Recv Router Update");
            break;

        case MLE_COMMAND_UPDATE_REQUEST:
            tr_info("Recv Router update Request");
            break;

        case MLE_COMMAND_DATA_REQUEST: {
            mle_tlv_info_t requestTlv;
            tr_info("Recv Router Data Request");

            entry_temp = mac_neighbor_entry_get_by_ll64(mac_neighbor_info(cur), mle_msg->packet_src_address, false, NULL);
            if (!entry_temp || !mle_tlv_read_tlv(MLE_TYPE_TLV_REQUEST, mle_msg->data_ptr, mle_msg->data_length, &requestTlv)) {
                return;
            }

            uint8_t mode = mle_mode_write_from_mac_entry(entry_temp);
            /* check if thread neighbor class is not initialized */
            if ((thread_neighbor_entry_linkmargin_get(&cur->thread_info->neighbor_class, entry_temp->index) == 0) &&
                    (thread_neighbor_last_communication_time_get(&cur->thread_info->neighbor_class, entry_temp->index) == 0)) {
                /*
                 * Thread neighbor class is not yet initialized and we receive data_request from such child.
                 * Always send full network data in this case
                 */
                mode |= MLE_THREAD_REQ_FULL_DATA_SET | MLE_THREAD_SECURED_DATA_REQUEST;
            } else {
                mode |= thread_neighbor_class_mode_write_from_entry(&cur->thread_info->neighbor_class, entry_temp->index);
            }

            thread_neighbor_class_update_link(&cur->thread_info->neighbor_class, entry_temp->index, linkMargin, false);
            thread_neighbor_last_communication_time_update(&cur->thread_info->neighbor_class, entry_temp->index);
            mle_build_and_send_data_response_msg(cur, mle_msg->packet_src_address, mle_msg->data_ptr, mle_msg->data_length, &requestTlv, mode);
        }
        break;
        default:
            tr_warn("Recv Router UNKNOWN message %d", mle_msg->message_type);
            break;

    }
}

static int8_t thread_router_bootstrap_synch_request_send(protocol_interface_info_entry_t *cur)
{
    uint8_t req_tlv[3];
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, 32, true, MLE_COMMAND_REQUEST);
    if (buf_id == 0) {
        return -1;
    }
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_set_msg_destination_address(buf_id, ADDR_LINK_LOCAL_ALL_ROUTERS);
    req_tlv[0] = MLE_TYPE_ADDRESS16;
    req_tlv[1] = MLE_TYPE_ROUTE;
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    ptr = mle_tlv_req_tlv(ptr, req_tlv, 2);
    if (mle_service_update_length_by_ptr(buf_id, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    // timeout set to two seconds, no retries
    timeout.retrans_max = 1;
    timeout.timeout_init = 2;
    timeout.timeout_max = 3;
    timeout.delay = MLE_NO_DELAY;

    mle_service_set_packet_callback(buf_id, thread_device_synch_timeout);
    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    mle_service_send_message(buf_id);
    return 0;
}

static int8_t thread_router_synch_new_router(protocol_interface_info_entry_t *cur, const uint8_t *destAddress)
{
    mle_message_timeout_params_t timeout;
    uint32_t keySequence;
    if (thread_info(cur)) {
        if (!thread_info(cur)->thread_leader_data) {
            return -1;
        }
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, 64, true, MLE_COMMAND_REQUEST);

    if (bufId == 0) {
        return -1;
    }
    uint8_t req_tlv = MLE_TYPE_RSSI;

    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    mle_service_set_msg_destination_address(bufId, destAddress);


    uint8_t *ptr = mle_service_get_data_pointer(bufId);
    ptr = mle_tlv_write_version(ptr, cur->thread_info->version);
    ptr = thread_leader_data_tlv_write(ptr, cur);
    ptr = mle_general_write_source_address(ptr, cur);
    ptr = mle_tlv_req_tlv(ptr, &req_tlv, 1);

    if (!addr_is_ipv6_multicast(destAddress)) {
        timeout.retrans_max = 2;
        timeout.timeout_init = 2;
        timeout.timeout_max = 3;
        timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
    } else {
        // Multicasts need transaction timer
        timeout.retrans_max = 1;
        timeout.timeout_init = 2;
        timeout.timeout_max = 3;
        timeout.delay = 0;
    }
    mle_service_set_packet_callback(bufId, thread_link_request_timeout);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }


    mle_service_send_message(bufId);
    return 0;
}

int thread_router_bootstrap_link_synch_start(protocol_interface_info_entry_t *cur)
{
    //Send Link Request for
    if (thread_router_bootstrap_synch_request_send(cur) == 0) {
        //SET Router synch receiver handler
        mle_service_interface_receiver_handler_update(cur->id, thread_router_synch_receive_cb);
        cur->nwk_bootstrap_state = ER_MLE_SYNCH;
        return 0;
    }
    return -1;
}
#ifdef HAVE_THREAD_V2

static bool thread_router_bootstrap_is_version_high_for_routing(uint8_t version_threshold, uint8_t thread_ver)
{

    if (thread_ver >= version_threshold + 3) {
        return true;
    }

    return false;
}

static void thread_router_bootstrap_pbbr_aloc_generate(struct protocol_interface_info_entry *cur)
{
    // Check if network data changed or bbr info and send proactive an if needed
    uint8_t bbr_anycast_addr[16];
    uint8_t bbr_rloc_addr[16];

    thread_addr_write_mesh_local_16(bbr_anycast_addr, 0xfc38, cur->thread_info);
    addr_delete_matching(cur, bbr_anycast_addr, 128, ADDR_SOURCE_THREAD_ALOC);

    if (0 != thread_common_primary_bbr_get(cur, bbr_rloc_addr, NULL, NULL, NULL)) {
        // Primary BBR not present
        return;
    }
    if (!addr_get_entry(cur, bbr_rloc_addr)) {
        // Not our address
        return;
    }
    tr_debug("generate primary BBR anycast address %s", trace_ipv6(bbr_anycast_addr));
    addr_add(cur, bbr_anycast_addr, 64, ADDR_SOURCE_THREAD_ALOC, 0xffffffff, 0, true);
    return;
}

static bool thread_router_bootstrap_is_reed_upgrade_allowed(protocol_interface_info_entry_t *cur)
{
    link_configuration_s *link_conf_ptr = thread_management_configuration_get(cur->id);

    if (!link_conf_ptr) {
        return true;
    }

    if (thread_info(cur)->version < THREAD_VERSION_1_2) {
        return true;
    }

    // VR check
    if (!(link_conf_ptr->securityPolicy & SECURITY_POLICY_ALL_ROUTERS_JOIN_ALLOWED)) {
        uint8_t vr_threshold = link_conf_ptr->securityPolicyExt & THREAD_SECURITY_POLICY_VR_VALUE;
        if (!thread_router_bootstrap_is_version_high_for_routing(vr_threshold, cur->thread_info->version)) {
            return false;
        }
    }

    if (!(link_conf_ptr->securityPolicy & THREAD_SECURITY_POLICY_CCM_DISABLED)) {
        // ccm network
        if (link_conf_ptr->securityPolicyExt & THREAD_SECURITY_POLICY_NCR_DISABLED) {
            // NCR bit '1' Non-CCM Routers disabled
            if (!thread_info(cur)->ccm_credentials_ptr) {
                // Device does not have domain certificate
                return false;
            }
        }
    }

    return true;
}
#else
#define thread_router_bootstrap_pbbr_aloc_generate(cur)
#endif


bool thread_router_bootstrap_router_downgrade(protocol_interface_info_entry_t *cur)
{
    uint8_t activeRouterCount;
    uint8_t goodLinks;

    if (cur->thread_info->thread_attached_state != THREAD_STATE_CONNECTED_ROUTER) {
        return false;
    }

    // if we are commissioner border router we have registered our address and cant upgrade or downgrade
    if (cur->thread_info->registered_commissioner.commissioner_valid &&
            addr_get_entry(cur, cur->thread_info->registered_commissioner.border_router_address)) {
        return false;
    }

    // spec: Not be the Leader
    if (cur->thread_info->leader_private_data) {
        if (!thread_router_bootstrap_routing_allowed(cur)) {
            tr_debug("Cannot be leader anymore");
            // Settings have changed, cannot be leader anymore
            thread_bootstrap_reset_restart(cur->id);
        }
        // Leader can not downgrade
        return false;
    }

    if (!thread_router_bootstrap_routing_allowed(cur)) {
        return true;
    }

    //spec: If the number of active Routers on the network exceeds ROUTER_DOWNGRADE_THRESHOLD
    activeRouterCount = thread_routing_count_active_routers(&cur->thread_info->routing);
    if (activeRouterCount <= cur->thread_info->routerSelectParameters.routerDowngradeThresHold) {
        return false;
    }

    uint_fast8_t asGood;
    goodLinks = thread_routing_count_neighbours_for_downgrade(&cur->thread_info->routing, &asGood);

    //spec: Have at least MIN_DOWNGRADE_NEIGHBORS neighbors in set M.
    if (goodLinks < MIN_DOWNGRADE_NEIGHBORS) {
        return false;
    }

    //spec: Have at least one neighbor that has as good or better quality links to all Routers in M.
    if (asGood == 0) {
        return false;
    }

    //spec: Not be the sole Border Router for a particular Provisioning Domain
    if (!ns_list_is_empty(&cur->thread_info->localServerDataBase.prefix_list)) {
        return false;
    }

    uint16_t child_count = thread_router_bootstrap_child_count_get(cur);
    //Cant downgrade if child count is larger than 3 times exceed routers
    if (child_count > (3 * (activeRouterCount - cur->thread_info->routerSelectParameters.routerDowngradeThresHold))) {
        return false;
    }
    tr_info("Reed downgrade:ChildCount %u Active Routers %u good links %u downgrade threshold %u", child_count, activeRouterCount, goodLinks, cur->thread_info->routerSelectParameters.routerDowngradeThresHold);

    return true;
}

bool thread_router_bootstrap_reed_upgrade(protocol_interface_info_entry_t *cur)
{
    uint8_t activeRouterCount;

    if (!thread_router_bootstrap_routing_allowed(cur)) {
        return false;
    }

    if (!thread_router_bootstrap_is_reed_upgrade_allowed(cur)) {
        return false;
    }

    if (thread_am_router(cur)) {
        return false;
    }

    // if we are commissioner border router we have registered our address and cant upgrade or downgrade
    if (cur->thread_info->registered_commissioner.commissioner_valid &&
            addr_get_entry(cur, cur->thread_info->registered_commissioner.border_router_address)) {
        return false;
    }

    if (!cur->thread_info->routing.router_id_sequence_valid) {
        // In case routing information is not up-to-date yet...
        return false;
    }

    activeRouterCount = thread_routing_count_active_routers(&cur->thread_info->routing);

    if (activeRouterCount >= cur->thread_info->routerSelectParameters.routerUpgradeThresHold) {
        return false;
    }
    tr_info("Reed Upgrade:Active Routers %u upgrade threshold %u", activeRouterCount, cur->thread_info->routerSelectParameters.routerUpgradeThresHold);

    return true;
}

void thread_router_bootstrap_child_id_reject(protocol_interface_info_entry_t *cur)
{
    thread_pending_child_id_req_t *req;
    req = thread_child_id_request_entry_get_from_the_list(cur);
    while (req) {
        tr_debug("Remove entry from list");
        //Remove entry from list
        mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(cur), req->euid64, ADDR_802_15_4_LONG);
        if (neighbor) {
            mac_neighbor_table_neighbor_remove(mac_neighbor_info(cur), neighbor);
        }

        ns_dyn_mem_free(req);
        req = thread_child_id_request_entry_get_from_the_list(cur);
    }
}

void thread_router_bootstrap_active_router_attach(protocol_interface_info_entry_t *cur)
{
    uint8_t *parent_mac_addr = NULL;
    arm_nwk_6lowpan_thread_test_print_routing_database(cur->id);
    uint16_t address16 = mac_helper_mac16_address_get(cur);
    cur->thread_info->thread_attached_state = THREAD_STATE_CONNECTED_ROUTER;

    if (cur->thread_info->thread_leader_data->leaderRouterId == address16 &&
            !cur->thread_info->leader_private_data) {
        // Error we are set up as leader but no private data
        tr_error("Leader setup error");
    }
    cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
    thread_routing_activate(&cur->thread_info->routing);
    thread_router_synch_new_router(cur, ADDR_LINK_LOCAL_ALL_ROUTERS);
    thread_bootstrap_ready(cur);
    thread_bootstrap_network_prefixes_process(cur);
    thread_nd_service_activate(cur->id);
    thread_router_bootstrap_mle_advertise(cur);
    if (cur->thread_info->thread_endnode_parent) {
        parent_mac_addr = cur->thread_info->thread_endnode_parent->mac64;
    }
    thread_nvm_store_link_info_write(parent_mac_addr, mac_helper_mac16_address_get(cur));
}

static int thread_validate_own_routeid_from_new_mask(const uint8_t *master_router_id_mask, uint8_t router_id)
{
    int ret_val = -1;
    if (bit_test(master_router_id_mask, router_id)) {
        ret_val = 0;
    }
    return ret_val;
}

int thread_router_bootstrap_route_tlv_push(protocol_interface_info_entry_t *cur, uint8_t *route_tlv, uint8_t route_len, uint8_t linkMargin, struct mac_neighbor_table_entry *entry)
{
    (void) route_len;

    uint8_t route_id_seq;
    const uint8_t *router_id_mask, *route_data;

    route_id_seq = *route_tlv++;
    router_id_mask = route_tlv;
    route_tlv += 8;
    route_data = route_tlv;
    uint16_t mac16 = mac_helper_mac16_address_get(cur);

    if (!thread_is_router_addr(entry->mac16)) {
        // Received route tlv from non router ignore
        tr_info("drop route Processing from end device %x", entry->mac16);
        return 0;
    }

    if (thread_i_am_router(cur)) {
        thread_routing_info_t *routing = &cur->thread_info->routing;
        if (routing->router_id_sequence_valid && common_serial_number_greater_8(route_id_seq, routing->router_id_sequence)) {
            thread_routing_leader_connection_validate(cur->thread_info, routing->networkFragmentationTimer);
            routing->networkFragmentationTimer = 0;
            if (thread_validate_own_routeid_from_new_mask(router_id_mask, thread_router_id_from_addr(mac16)) != 0) {
                tr_debug("RouterID not valid any More");
                thread_bootstrap_connection_error(cur->id, CON_ERROR_NETWORK_KICK, NULL);
                return 0;
            }
        }
    } else if (!thread_info(cur)->thread_endnode_parent ||
               thread_info(cur)->thread_endnode_parent->shortAddress != entry->mac16) {
        return 0;
    }

    /* XXX Is short_src_adr ever reset? Is it undefined if info not in msg? */
    /* Don't add routing link if MLE link is NOT bi-directional (i.e. we can only hear) */
    if (entry->connected_device) {
        thread_routing_add_link(cur, entry->mac16, linkMargin, route_id_seq, router_id_mask, route_data, false);
    }

    return 0;
}


static void thread_bootstrap_client_router_id_release_cb(int8_t interface_id, int8_t status, uint16_t router_rloc, const uint8_t router_mask_ptr[9])
{
    protocol_interface_info_entry_t *cur;
    (void) status;
    (void)router_mask_ptr;
    (void)router_rloc;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    tr_debug("RouterID ReleaseCB");
    if (!cur) {
        return;
    }

    if (!cur->thread_info->releaseRouterId) {
        return;
    }
    cur->thread_info->releaseRouterId = false;

    thread_bootstrap_router_id_release_ready(cur);
    return;

}
static uint32_t thread_reed_timeout_calculate(thread_router_select_t *routerSelect)
{
    return randLIB_get_random_in_range(routerSelect->reedAdvertisementInterval, (routerSelect->reedAdvertisementInterval) + (routerSelect->reedAdvertisementJitterInterval));
}


static int thread_reed_advertise(protocol_interface_info_entry_t *cur)
{
    uint32_t keySequence;
    struct link_configuration *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    if (!thread_info(cur)) {
        return -1;
    }

    // FED not allowed to send advertisements
    if (thread_info(cur)->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE) {
        return -1;
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, 16, false, MLE_COMMAND_ADVERTISEMENT);
    if (bufId == 0) {
        return -1;
    }

    tr_debug("MLE REED ADVERTISEMENT STARTED");
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    /*Add Leader Data & Source Address TLVs */
    ptr = thread_leader_data_tlv_write(ptr, cur);
    ptr = mle_general_write_source_address(ptr, cur);
    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }

    mle_service_set_msg_destination_address(bufId, ADDR_LINK_LOCAL_ALL_NODES);
    mle_service_send_message(bufId);
    return 0;
}

static void thread_reed_advertisements_cb(void *arg)
{
    if (!arg) {
        return;
    }
    protocol_interface_info_entry_t *cur = arg;

    cur->thread_info->routerSelectParameters.reedAdvertisementTimeout = NULL;

    if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE && cur->nwk_bootstrap_state != ER_MLE_ATTACH_READY) {
        /* Own attach is ongoing, try to send advertisement after few seconds */
        cur->thread_info->routerSelectParameters.reedAdvertisementTimeout = eventOS_timeout_ms(thread_reed_advertisements_cb, 2 * 1000, cur);
        return;
    }

    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED &&
            cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        thread_reed_advertise(cur);
        thread_router_bootstrap_child_information_clear(cur);
        cur->thread_info->routerSelectParameters.reedAdvertisementTimeout = eventOS_timeout_ms(thread_reed_advertisements_cb, thread_reed_timeout_calculate(&cur->thread_info->routerSelectParameters) * 1000, cur);
    }
}

void thread_router_bootstrap_reed_advertisements_start(protocol_interface_info_entry_t *cur)
{
    uint32_t timeout = THREAD_REED_ADVERTISEMENT_DELAY;
    if (!cur) {
        return;
    }
    eventOS_timeout_cancel(cur->thread_info->routerSelectParameters.reedAdvertisementTimeout);
    cur->thread_info->routerSelectParameters.reedAdvertisementTimeout = NULL;

    cur->thread_info->reedJitterTimer = thread_router_bootstrap_random_upgrade_jitter();

    if (!thread_is_connected(cur)) {
        return;
    }
    if (cur->thread_info->releaseRouterId  ||
            thread_router_bootstrap_child_count_get(cur) == 0) {
        // If we dont have any children or are are downgrading send REED advertisement immediately
        timeout = 1;
    }
    cur->thread_info->routerSelectParameters.reedAdvertisementTimeout = eventOS_timeout_ms(thread_reed_advertisements_cb, timeout, cur);
}

void thread_router_bootstrap_reed_merge_advertisement(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->reedMergeAdvTimer > 1) {
        return;
    }
    thread_reed_advertise(cur);
    // 120s second timer reinitialised before next merge advertisement
    cur->thread_info->reedMergeAdvTimer = THREAD_REED_MERGE_ADVERTISEMENT_INTERVAL;

}
void thread_router_bootstrap_router_id_release(protocol_interface_info_entry_t *cur)
{
    tr_debug("Router ID Release");
    if (thread_management_client_router_id_release(cur->id, cur->mac, cur->thread_info->routerShortAddress, thread_bootstrap_client_router_id_release_cb) == 0) {
        // Release message sent succesfully
        cur->thread_info->routerShortAddress = 0xfffe;
        cur->thread_info->releaseRouterId = true;
    }
}

uint32_t thread_router_bootstrap_random_upgrade_jitter()
{
    if (thread_router_selection_jitter <= 2) {
        return 1;
    }
    return randLIB_get_random_in_range(2, thread_router_selection_jitter);
}

void thread_router_bootstrap_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
    thread_info_t *thread_info = cur->thread_info;

    if (thread_am_host(cur)) {
        return;
    }

    //Do we have a childs and we are REED state
    if (cur->thread_info->thread_attached_state == THREAD_STATE_CONNECTED && thread_router_bootstrap_child_count_get(cur)) {
        tr_debug("Trig quick router ID request");
        thread_bootstrap_attched_upgrade_reed(cur);
        return;
    }

    if (thread_info->reedJitterTimer > ticks) {
        // Reed status is checked every random jitter values
        thread_info->reedJitterTimer -= ticks;
    } else {
        thread_info->reedJitterTimer = thread_router_bootstrap_random_upgrade_jitter();
        if (thread_router_bootstrap_reed_upgrade(cur)) {
            //Check UpGrade possibility
            tr_debug("REED Upgrade to router");
            thread_bootstrap_attched_upgrade_reed(cur);
        }
        if (thread_router_bootstrap_router_downgrade(cur)) {
            tr_debug("Router downgrade to REED");
            thread_bootstrap_attached_downgrade_router(cur);
        }
    }

    if (cur->thread_info->reedMergeAdvTimer > ticks) {
        cur->thread_info->reedMergeAdvTimer -= ticks;
    } else {
        cur->thread_info->reedMergeAdvTimer = 0;
    }

    if (!thread_info->leader_private_data && thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER) {
        // Non leader router checks
        if (thread_info->routing.activated) {
            if (thread_info->routing.networkFragmentationTimer++ >= thread_info->routing.networkIdTimeout) {
                tr_debug("Leader Connection Lost");
                thread_bootstrap_connection_error(cur->id, CON_ERROR_NETWORK_REATTACH, NULL);
            }
        } else {
            tr_debug("Routing Disabled?");
        }
    }

    if (thread_info->proactive_an_timer) {
        if (thread_info->proactive_an_timer > ticks) {
            thread_info->proactive_an_timer -= ticks;
        } else {
            thread_send_proactive_an(cur);
            thread_info->proactive_an_timer = 0;
        }
    }

    thread_leader_service_timer(cur, ticks);
    return;
}

void thread_router_bootstrap_advertiment_analyze(protocol_interface_info_entry_t *cur, uint8_t *src_address, struct mac_neighbor_table_entry *entry_temp, uint16_t shortAddress)
{
    if (entry_temp) {

        if (thread_is_router_addr(shortAddress)) {
            entry_temp->link_lifetime = THREAD_DEFAULT_LINK_LIFETIME;
            entry_temp->link_lifetime++;
        }

        if (thread_is_router_addr(shortAddress)) {
            //Update MAC Security PIB table by get & set Operation
            mlme_get_t get_req;
            get_req.attr = macDeviceTable;
            get_req.attr_index = entry_temp->index;
            cur->mac_api->mlme_req(cur->mac_api, MLME_GET, &get_req);
            entry_temp->lifetime = entry_temp->link_lifetime;
        }
    } else {
        //
        if (!thread_attach_active_router(cur)) {
            // We are not router
            return;
        }
        if (!thread_is_router_addr(shortAddress)) {
            // Reed advertisement
            return;
        }
        if (thread_bootstrap_link_create_check(cur, shortAddress) &&
                thread_bootstrap_link_create_allowed(cur, shortAddress, src_address)) {
            //Challenge new router neighbor
            tr_debug("Synch new neighbor");
            thread_router_synch_new_router(cur, src_address);
        }
    }
}
static void thread_router_bootstrap_dhcp_server_any_cast_address_update(protocol_interface_info_entry_t *cur, uint16_t anycastAddress)
{
    uint8_t ipv6_address[16];

    thread_addr_write_mesh_local_16(ipv6_address, anycastAddress, cur->thread_info);
    tr_debug("generate DHCP anycast address %s", trace_ipv6(ipv6_address));
    addr_add(cur, ipv6_address, 124, ADDR_SOURCE_THREAD_ALOC, 0xffffffff, 0, true);
}

static void thread_bootstrap_dhcp_anycast_address_generate(protocol_interface_info_entry_t *cur)
{
    uint8_t ipv6_address[16];
    thread_addr_write_mesh_local_16(ipv6_address, 0xfc00, cur->thread_info);
    addr_delete_matching(cur, ipv6_address, 124, ADDR_SOURCE_THREAD_ALOC);

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, curPrefix, &cur->thread_info->networkDataStorage.localPrefixList) {
        // Go through all prefixes
        ns_list_foreach(thread_network_server_data_entry_t, curBorderRouter, &curPrefix->borderRouterList) {
            if (curBorderRouter->P_dhcp &&
                    curBorderRouter->routerID == cur->mac_parameters->mac_short_address) {
                // We host this dhcp service we must add anycast address
                ns_list_foreach(thread_network_data_context_entry_t, curRoute, &curPrefix->contextList) {
                    // Search what is the context id for this prefix
                    uint16_t anycastAddress = 0xfc00;
                    anycastAddress |= curRoute->cid;
                    thread_router_bootstrap_dhcp_server_any_cast_address_update(cur, anycastAddress);
                }
            }
        }
    }
}

static void thread_bootstrap_service_anycast_address_generate(protocol_interface_info_entry_t *cur)
{
    uint8_t ipv6_address[16];

    // Delete old address
    thread_addr_write_mesh_local_16(ipv6_address, 0xfc10, cur->thread_info);
    addr_delete_matching(cur, ipv6_address, 124, ADDR_SOURCE_THREAD_ALOC);

    ns_list_foreach(thread_network_data_service_cache_entry_t, curService, &cur->thread_info->networkDataStorage.service_list) {
        // Go through all prefixes
        ns_list_foreach(thread_network_data_service_server_entry_t, curServiceServer, &curService->server_list) {
            if (curServiceServer->router_id == cur->mac_parameters->mac_short_address) {
                // We host this service we must add anycast address
                // Search what is the context id for this prefix
                thread_addr_write_mesh_local_16(ipv6_address, 0xfc10 | curService->S_id, cur->thread_info);
                tr_debug("generate Service anycast address %s", trace_ipv6(ipv6_address));
                addr_add(cur, ipv6_address, 124, ADDR_SOURCE_THREAD_ALOC/*?*/, 0xffffffff, 0, true);
            }
        }
    }
}


static void thread_router_bootstrap_commissioner_aloc_generate(protocol_interface_info_entry_t *cur)
{
    uint8_t commissioner_anycast[16];

    // Delete old address
    thread_addr_write_mesh_local_16(commissioner_anycast, 0xfc30 + (cur->thread_info->registered_commissioner.session_id % 8), cur->thread_info);
    addr_delete_matching(cur, commissioner_anycast, 125, ADDR_SOURCE_THREAD_ALOC);

    if (!cur->thread_info->registered_commissioner.commissioner_valid) {
        // No commissioner available
        return;
    }

    if (!addr_get_entry(cur, cur->thread_info->registered_commissioner.border_router_address)) {
        // Not our address
        return;
    }
    // Add commissioning border router address
    tr_debug("generate commissioner anycast address %s", trace_ipv6(commissioner_anycast));
    addr_add(cur, commissioner_anycast, 64, ADDR_SOURCE_THREAD_ALOC, 0xffffffff, 0, true);
}

void thread_router_bootstrap_anycast_address_register(protocol_interface_info_entry_t *cur)
{
    uint8_t leader_anycast_address[16];

    tr_debug("Register anycast address:");

    // Generate leader ALOC address
    thread_addr_write_mesh_local_16(leader_anycast_address, 0xfc00, cur->thread_info);

    if (cur->thread_info->leader_private_data) {
        tr_debug("Register leader anycast address: %s", trace_ipv6(leader_anycast_address));
        addr_add(cur, leader_anycast_address, 128, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0, true);
    } else {
        // Delete Leader ALOC if ml prefix was changed address Source is the defining rule
        addr_delete_matching(cur, leader_anycast_address, 128, ADDR_SOURCE_UNKNOWN);

    }
    thread_bootstrap_dhcp_anycast_address_generate(cur);
    thread_bootstrap_service_anycast_address_generate(cur);
    thread_router_bootstrap_commissioner_aloc_generate(cur);
    thread_router_bootstrap_pbbr_aloc_generate(cur);
}

static int thread_router_bootstrap_network_data_propagation(protocol_interface_info_entry_t *cur, uint8_t *childUnicastAddress, bool fullList)
{
    uint8_t *payload_ptr;
    uint16_t payload_size = 16 + 4 + 20 + 4; //Version 4 bytes and Source address 4 bytes
    struct link_configuration *linkConfiguration;
    linkConfiguration = thread_joiner_application_get_config(cur->id);
    if (!linkConfiguration) {
        return -1;
    }

    if (!thread_info(cur)) {
        return -1;
    }

    payload_size += 2 + thread_network_data_tlv_size(cur, fullList);
    payload_size += thread_pending_timestamp_tlv_size(cur);

    mle_message_timeout_params_t timeout;
    uint16_t buf_id = mle_service_msg_allocate(cur->id, payload_size, false, MLE_COMMAND_DATA_RESPONSE);
    if (buf_id == 0) {
        return -1;
    }

    tr_debug("Send MLE data response network data changed");
    payload_ptr = mle_service_get_data_pointer(buf_id);
    uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(buf_id);

    /* Send to ULA16 of DATA_REQUEST originator. */
    /* Current_gp_prefix should have ULA prefix in */
    if (childUnicastAddress) {
        memcpy(address_ptr, childUnicastAddress, 16);
    } else {
        memcpy(address_ptr, ADDR_LINK_LOCAL_ALL_NODES, 16);
    }

    /*Add Leader Data & Network Data */
    payload_ptr = thread_leader_data_tlv_write(payload_ptr, cur);
    payload_ptr = thread_network_data_tlv_write(cur, payload_ptr, fullList);

    payload_ptr = thread_active_timestamp_write(cur, payload_ptr);
    payload_ptr = thread_pending_timestamp_write(cur, payload_ptr);
    payload_ptr = mle_general_write_source_address(payload_ptr, cur);

    if (mle_service_update_length_by_ptr(buf_id, payload_ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_HALF_SECOND_MAX_DELAY;
    mle_service_set_msg_timeout_parameters(buf_id, &timeout);
    //Set Security
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);

    mle_service_send_message(buf_id);
    return 0;

}

static void thread_router_bootstrap_network_data_push_to_sleep_child(protocol_interface_info_entry_t *cur, bool stableDataUpdate)
{
    uint8_t childLinkLocalAddress[16];
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    memcpy(childLinkLocalAddress, ADDR_LINK_LOCAL_PREFIX, 8);
    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (!cur_entry->rx_on_idle) {
            memcpy(&childLinkLocalAddress[8], cur_entry->mac64, 8);
            childLinkLocalAddress[8] ^= 2;
            if (thread_neighbor_class_request_full_data_setup(&cur->thread_info->neighbor_class, cur_entry->index)) {
                thread_router_bootstrap_network_data_propagation(cur, childLinkLocalAddress, true);
            } else {
                if (stableDataUpdate) {
                    thread_router_bootstrap_network_data_propagation(cur, childLinkLocalAddress, false);
                }
            }
        }
    }
}

void thread_router_bootstrap_network_data_distribute(protocol_interface_info_entry_t *cur)
{
    if (!thread_i_am_router(cur)) {
        return;
    }
    tr_debug("Propagate New Network Data");
    thread_router_bootstrap_network_data_propagation(cur, NULL, true);
    thread_router_bootstrap_network_data_push_to_sleep_child(cur, cur->thread_info->networkDataStorage.stableUpdatePushed);
}

bool thread_router_bootstrap_routing_allowed(struct protocol_interface_info_entry *cur)
{
    link_configuration_s *link_conf_ptr = thread_management_configuration_get(cur->id);

    if (!link_conf_ptr) {
        return true;
    }

    return !(thread_info(cur)->version < THREAD_VERSION_1_2 && !(link_conf_ptr->securityPolicy & SECURITY_POLICY_ALL_ROUTERS_JOIN_ALLOWED));
}

void thread_router_bootstrap_address_change_notify_send(protocol_interface_info_entry_t *cur)
{
    thread_info(cur)->proactive_an_timer = THREAD_PROACTIVE_AN_SEND_DELAY;
}

void thread_router_bootstrap_delay_reed_jitter(int8_t interface_id, uint16_t delay)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }
    if (cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER) {
        return;
    }
    // delay reed jitter timer to allow for settings changes to distribute
    thread_info(cur)->reedJitterTimer += delay;
    return;
}

#endif /* HAVE_THREAD_ROUTER */
