/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/*
 * \file protocol_6lowpan_bootstrap.c
 *
 */
#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_scheduler.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "net_pana_parameters_api.h"
#ifdef HAVE_RPL
#include "RPL/rpl_control.h"
#endif
#ifndef NO_MLE
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#endif
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif

#include "ccmLIB.h"
#include "shalib.h"
#include "net_nvm_api.h"
#include "common_functions.h"
#include "net_interface.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "BorderRouter/border_router.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/beacon_handler.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "libNET/src/net_load_balance_internal.h"
#include "6LoWPAN/NVM/nwk_nvm.h"


/* Fixed-point randomisation limits for randlib_randomise_base() - RFC 3315
 * says RAND is uniformly distributed between -0.1 and +0.1
 */
#define LOWPAN_RAND_LOW   0x7333 // 1 - 0.1; minimum for "1+RAND"
#define LOWPAN_RAND_HIGH  0x8CCD // 1 + 0.1; maximum for "1+RAND"

#define TRACE_GROUP_LOWPAN_BOOT "6Bo"
#define TRACE_GROUP "6Bo"

#ifdef HAVE_6LOWPAN_ND
#ifdef HAVE_RPL
static int protocol_6lowpan_router_multicast_synch(protocol_interface_info_entry_t *cur);
static void protocol_6lowpan_bootstrap_rpl_callback(rpl_event_t event, void *handle);
#else
#define protocol_6lowpan_router_multicast_synch(cur) (-1)
#endif

static void protocol_6lowpan_mle_purge_neighbors(struct protocol_interface_info_entry *cur_interface, uint8_t entry_count, uint8_t force_priority);
static uint8_t protocol_6lowpan_mle_order_last_entries(mle_neigh_table_list_t *mle_neigh_table, uint8_t entry_count);
static uint8_t protocol_6lowpan_mle_data_allocate(void);
static bool mle_accept_request_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries);
static void lowpan_comm_status_indication_cb(int8_t if_id, const mlme_comm_status_t* status);

static void protocol_6lowpan_priority_neighbor_remove(protocol_interface_info_entry_t *cur_interface, mle_neigh_table_entry_t *cur);
static void protocol_6lowpan_neighbor_information_remove(int8_t interface_id, mle_neigh_table_entry_t *cur);
static int8_t protocol_6lowpan_host_challenge(int8_t interface_id, const uint8_t *mac64);
static int8_t protocol_6lowpan_router_challenge(int8_t interface_id, const uint8_t *mac64);
static void protocol_6lowpan_address_reg_ready(protocol_interface_info_entry_t *cur_interface);
static void coordinator_black_list(protocol_interface_info_entry_t *cur);

static mle_6lowpan_data_t *mle_6lowpan_data;

#define MAX_MC_DIS_COUNT 3

static void lowpan_bootstrap_pan_control(protocol_interface_info_entry_t *cur, bool bootstrap_ready)
{
    if (cur->mac_api) {
        mlme_start_t start_req;
        memset(&start_req, 0, sizeof(mlme_start_t));
        start_req.BeaconOrder = 15;
        start_req.SuperframeOrder = 15;
        start_req.PANCoordinator = bootstrap_ready;
        start_req.LogicalChannel = cur->mac_parameters->mac_channel;
        start_req.PANId = cur->mac_parameters->pan_id;
        cur->mac_api->mlme_req(cur->mac_api, MLME_START , &start_req);
        net_load_balance_internal_state_activate(cur, bootstrap_ready);
    }
}

static uint8_t lowpan_mode_get_by_interface_ptr(protocol_interface_info_entry_t *cur)
{
    uint8_t mle_mode = 0;

    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        mle_mode |= MLE_RX_ON_IDLE;
    }


    if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        mle_mode |= (MLE_FFD_DEV);
    }

    return mle_mode;
}

uint8_t *mle_general_write_timeout(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{

    uint32_t timeout_value = 0;
    uint8_t mode = lowpan_mode_get_by_interface_ptr(cur);

    if (!mle_6lowpan_data) {
        return ptr;
    }

    if (!(mode & MLE_RX_ON_IDLE)) {
        timeout_value = mac_data_poll_host_timeout(cur);
    }
    if (timeout_value == 0) {
        timeout_value = mle_6lowpan_data->host_lifetime;
    }

    return mle_tlv_write_timeout(ptr, timeout_value);

}

static void protocol_6lowpan_priority_neighbor_remove(protocol_interface_info_entry_t *cur_interface, mle_neigh_table_entry_t *cur)
{
    if (!cur->priorityFlag ||
        !(cur_interface->lowpan_info & INTERFACE_NWK_ACTIVE) ||
        cur_interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return;
    }

    uint8_t mac64[8];

    if (cur_interface->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        // Coordinating parent has been lost during bootstrap
        if (!cur_interface->global_address_available) {
            tr_debug("bootstrap coordinator down");
            bootsrap_next_state_kick(ER_BOOTSTRAP_CONNECTION_DOWN, cur_interface);
        }
    } else {
        //Call Priority parent loose
        if (cur->short_adr != 0xffff) {
            memcpy(mac64, ADDR_SHORT_ADR_SUFFIC, 6);
            common_write_16_bit(cur->short_adr, &mac64[6]);
        } else {
            memcpy(mac64,cur->mac64 , 8);
            mac64[0] ^= 2;
        }

        if (nd_parent_loose_indcate(mac64, cur_interface) != 0) {
            //ND Router synch lost
            tr_debug("ND Router Synch Loose");
            bootsrap_next_state_kick(ER_PARENT_SYNCH_LOST, cur_interface);
        }
    }
}

static void protocol_6lowpan_neighbor_information_remove(int8_t interface_id, mle_neigh_table_entry_t *cur)
{
    protocol_interface_info_entry_t *cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur_interface) {
        return;
    }

    // Sleepy host
    if (cur_interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        mac_data_poll_protocol_poll_mode_decrement(cur_interface);
    }

    protocol_6lowpan_priority_neighbor_remove(cur_interface, cur);

    if (cur->mode & MLE_FFD_DEV) {
        protocol_6lowpan_release_short_link_address_from_neighcache(cur_interface, cur->short_adr);
        protocol_6lowpan_release_long_link_address_from_neighcache(cur_interface, cur->mac64);
    }
    mac_helper_devicetable_remove(cur_interface->mac_api, cur->attribute_index);
}

static bool protocol_6lowpan_challenge_callback(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    uint8_t mac64[8];
    uint8_t *ll64_ptr = mle_service_get_msg_destination_address_pointer(msgId);

    memcpy(mac64, ll64_ptr + 8, 8);
    mac64[0] ^= 2;

    mle_neigh_table_entry_t *neig_info = mle_class_get_by_link_address(interface_id, mac64, ADDR_802_15_4_LONG);

    if (!neig_info) {
        return false;//Why entry is removed before timeout??
    }


    if (neig_info->ttl > MLE_TABLE_CHALLENGE_TIMER) {
        return false;
    }


    if (usedAllRetries) {
        //GET entry
        mle_class_remove_entry(interface_id, neig_info);
        return false;
    }

    return true;
}

static int8_t protocol_6lowpan_host_challenge(int8_t interface_id, const uint8_t *mac64)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }


    uint16_t bufId;
    mle_message_timeout_params_t timeout;
    uint8_t ll64[16];


    //Challenge

    uint8_t security_level = mle_service_security_level_get(cur->id);
    tr_debug("Link REQUEST");
    bufId = mle_service_msg_allocate(cur->id, 32, true,MLE_COMMAND_REQUEST);
    if (bufId == 0) {
        return -1;
    }

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    ptr = mle_general_write_source_address(ptr, cur);
    ptr = mle_tlv_write_mode(ptr, lowpan_mode_get_by_interface_ptr(cur));
    if (security_level) {
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    }
    if (cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        ptr = mle_general_write_timeout(ptr, cur);
    }

    memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&ll64[8], mac64, 8);
    ll64[8] ^= 2;
    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 3;
    timeout.timeout_init = 2;
    timeout.timeout_max = 4;
    timeout.delay = MLE_NO_DELAY;

    //SET Destination address
    mle_service_set_msg_destination_address(bufId, ll64);
    //Set Callback
    mle_service_set_packet_callback(bufId, protocol_6lowpan_challenge_callback);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);

    mle_service_send_message(bufId);
    return 0;
}

static int8_t protocol_6lowpan_router_challenge(int8_t interface_id, const uint8_t *mac64)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }


    uint16_t bufId;
    mle_message_timeout_params_t timeout;
    uint8_t ll64[16];


    //Challenge

    uint8_t security_level = mle_service_security_level_get(cur->id);
    tr_debug("Link REQUEST");
    bufId = mle_service_msg_allocate(cur->id, 32, true,MLE_COMMAND_REQUEST);
    if (bufId == 0) {
        return -1;
    }

    uint8_t *ptr = mle_service_get_data_pointer(bufId);

    ptr = mle_general_write_source_address(ptr, cur);
    ptr = mle_tlv_write_mode(ptr, lowpan_mode_get_by_interface_ptr(cur));
    if (security_level) {
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    }

    memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(&ll64[8], mac64, 8);
    ll64[8] ^= 2;
    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }
    timeout.retrans_max = 2;
    timeout.timeout_init = 2;
    timeout.timeout_max = 4;
    timeout.delay = MLE_NO_DELAY;

    //SET Destination address
    mle_service_set_msg_destination_address(bufId, ll64);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);

    mle_service_send_message(bufId);
    return 0;
}


static uint8_t mle_advert_neigh_cnt(int8_t interface_id, bool short_adr) {

    uint8_t advert_neigh_cnt;
    uint8_t neighb_max;

    uint8_t mle_neigh_cnt = mle_class_active_neigh_counter(interface_id);

    if (short_adr == true) {
        neighb_max = 16;
    } else {
        neighb_max = 5;
    }

    if (mle_neigh_cnt > neighb_max) {
        advert_neigh_cnt = neighb_max;
    } else {
        advert_neigh_cnt = mle_neigh_cnt;
    }

    return (advert_neigh_cnt);
}

static uint8_t mle_link_quality_tlv_parse(uint8_t *mac64, uint16_t short_address, uint8_t *ptr, uint16_t data_length, uint8_t *iop_flags_ptr, uint8_t *link_idr_ptr)
{
    uint8_t entry_size;

    if (data_length) {
        entry_size = (*ptr++ & 0x0f) + 3;
        data_length--;

        // Supports 2 and 8 bytes long MAC addresses
        if ((entry_size == 4) || (entry_size == 10)) {

            uint8_t own_addr_match = false;

            // Searches own address from link quality TLV
            while (data_length >= entry_size ) {

                if (entry_size == 4){
                    if (common_read_16_bit(ptr + 2) == short_address) {
                        own_addr_match = true;
                    }
                } else {
                    if (memcmp(ptr + 2, mac64, 8) == 0) {
                        own_addr_match = true;
                    }
                }

                // If own address is found returns success
                if (own_addr_match) {
                    if (iop_flags_ptr) {
                        *iop_flags_ptr = ptr[0];
                    }
                    if (link_idr_ptr) {
                        *link_idr_ptr = ptr[1];
                    }
                    return 1;
                }

                ptr += entry_size;
                data_length -= entry_size;
            }
        }
    }
    return 0;
}

static uint8_t *mle_table_set_neighbours(int8_t interface_id, uint8_t *ptr)
{
    uint8_t *len_ptr = 0;
    uint8_t short_temp[2] = {0xff,0xff};
    uint8_t neigh_count = 0;
    uint8_t neigh_count_max = 0;
    uint8_t *link_flags_ptr;
    mle_neigh_table_entry_t *first_entry_ptr = NULL;
    bool loop_list = false;

    mle_neigh_table_list_t * neigh_list = mle_class_active_list_get(interface_id);
    if (!neigh_list) {
        return ptr;
    }

    *ptr++ = MLE_TYPE_LINK_QUALITY;
    len_ptr = ptr++;
    *len_ptr = 1;
    // defaults: complete, 2 bytes long link-layer address
    link_flags_ptr = ptr++;
    *link_flags_ptr = 0x81;

    if (mle_class_get_by_link_address(interface_id, short_temp,ADDR_802_15_4_SHORT)) {
        *link_flags_ptr |= 0x07;
        neigh_count_max = mle_advert_neigh_cnt(interface_id, false);
    } else {
        neigh_count_max = mle_advert_neigh_cnt(interface_id, true);
    }

    do {
        ns_list_foreach(mle_neigh_table_entry_t, cur, neigh_list)
        {

            loop_list = false;

            if ((cur->handshakeReady) && (cur->link_q_adv_sent == false)) {

                // If looping list, stops adding entries when at first sent entry again
                if (first_entry_ptr == cur) {
                    break;
                } else if (first_entry_ptr == NULL) {
                    first_entry_ptr = cur;
                }

                // Limits the number of entries that are sent
                if (++neigh_count > neigh_count_max) {
                    *link_flags_ptr &= 0x7f;
                    break;
                }

                if (cur->priorityFlag) {
                    *ptr++ = MLE_NEIGHBOR_PRIORITY_LINK | MLE_NEIGHBOR_INCOMING_LINK | MLE_NEIGHBOR_OUTGOING_LINK;
                } else {
                    *ptr++ = MLE_NEIGHBOR_INCOMING_LINK | MLE_NEIGHBOR_OUTGOING_LINK;
                }

                *ptr++ = etx_local_incoming_idr_read(interface_id, cur) >> 3;

                if ((*link_flags_ptr & 0x07) == 1) {
                    ptr = common_write_16_bit(cur->short_adr, ptr);
                    *len_ptr += 4;
                } else {
                    memcpy(ptr, cur->mac64, 8);
                    ptr += 8;
                    *len_ptr += 10;
                }

                // If end of the neighbor list, start adding entries from start again
                if (cur->link.next == 0) {
                    loop_list = true;
                    mle_neigh_table_list_t * neigh_temp = mle_class_active_list_get(interface_id);
                    ns_list_foreach(mle_neigh_table_entry_t, temp, neigh_temp)
                    {
                        // Marks entries not sent
                        temp->link_q_adv_sent = false;
                    }
                } else {
                    cur->link_q_adv_sent = true;
                }
            }
        }
    } while (loop_list);

    return ptr;
}

#ifndef NO_MLE
static int protocol_6lowpan_mle_neigh_advertise(protocol_interface_info_entry_t *cur)
{
    /* How was this 40 calculated? Seems to be more than enough, at least.
     * MODE = 2+1, SRC_ADDR = 2+2, LINK = 2+1+4*neighbours, ROUTE = 2+MLE_ROUTE_MIN_OPTION_LEN+routers
     * Total = 10 + neighbours * 4
     */
    uint16_t neig_cache_size = 40 + 7;
    uint8_t short_temp[2] = {0xff,0xff};
    uint8_t *ptr;
    mle_message_timeout_params_t timeout;

    if (!cur) {
        return 0;
    }

    if (mle_class_get_by_link_address(cur->id, short_temp,ADDR_802_15_4_SHORT)) {
        neig_cache_size += mle_advert_neigh_cnt(cur->id, false) * 10;
    } else {
        neig_cache_size += mle_advert_neigh_cnt(cur->id, true) << 2;
    }

    uint16_t bufId = mle_service_msg_allocate(cur->id, neig_cache_size, false, MLE_COMMAND_ADVERTISEMENT);

    if (bufId == 0) {
        return -1;
    }

    timeout.retrans_max = 0;
    timeout.timeout_init = 0;
    timeout.timeout_max = 0;
    timeout.delay = MLE_NO_DELAY;

    tr_debug("Send MLE Advertisement");
    mle_service_set_msg_destination_address(bufId,ADDR_LINK_LOCAL_ALL_ROUTERS);
    mle_service_set_msg_timeout_parameters(bufId, &timeout);

    ptr = mle_service_get_data_pointer(bufId);
    ptr = mle_general_write_source_address(ptr, cur);
    ptr = mle_tlv_write_mode(ptr, lowpan_mode_get_by_interface_ptr(cur));
    ptr = mle_table_set_neighbours(cur->id, ptr);

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
        tr_debug("Buffer overflow at message write");
    }

    return mle_service_send_message(bufId);
}
#endif

static uint8_t compute_link_margin(int8_t rssi)
{
    if (rssi < -94) {
        return 0;
    }

    return (rssi + 94);
}

static int mle_validate_6lowpan_link_request_message(uint8_t *ptr, uint16_t data_len, mle_tlv_info_t *tlv_info)
{
    /**
     * MLE Request need to include always challenge
     * - MLE_TYPE_CHALLENGE
     */
    if (mle_tlv_option_discover(ptr, data_len, MLE_TYPE_CHALLENGE, tlv_info) < 4) {
        // TLV not found or length is smaller than 4
        return -1;
    }
    return 0;
}

static void mle_neigh_time_and_mode_update(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length)
{
    mle_tlv_info_t mle_tlv_info;
    uint32_t timeout_tlv;

    if (!mle_6lowpan_data) {
        return;
    }

    if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_MODE, &mle_tlv_info) > 0) {
        uint8_t *t_ptr = mle_tlv_info.dataPtr;
        entry_temp->mode = *t_ptr;
    }

    if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_TIMEOUT, &mle_tlv_info) > 0) {
        timeout_tlv = common_read_32_bit(mle_tlv_info.dataPtr);
    } else {
        if (entry_temp->mode & MLE_FFD_DEV) {
            timeout_tlv = mle_6lowpan_data->router_lifetime;
        } else {
            timeout_tlv = mle_6lowpan_data->host_lifetime;
        }
    }
    mle_entry_timeout_update(entry_temp, timeout_tlv);
}

static void mle_neigh_entry_update_by_mle_tlv_list(int8_t interface_id, mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, uint8_t *mac64, uint16_t short_address)
{
    mle_tlv_info_t mle_tlv_info;

    if (tlv_length) {
        if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_SRC_ADDRESS, &mle_tlv_info) > 0) {
            entry_temp->short_adr = common_read_16_bit(mle_tlv_info.dataPtr);
        }

        if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_LINK_QUALITY, &mle_tlv_info) > 0) {
            uint8_t link_idr;
            uint8_t iop_flags;
            if (mle_link_quality_tlv_parse(mac64, short_address, mle_tlv_info.dataPtr, mle_tlv_info.tlvLen, &iop_flags, &link_idr)) {
                etx_remote_incoming_idr_update(interface_id, link_idr, entry_temp);

                if ((iop_flags & MLE_NEIGHBOR_PRIORITY_LINK) == MLE_NEIGHBOR_PRIORITY_LINK) {
                    entry_temp->priority_child_flag = true;
                } else {
                    entry_temp->priority_child_flag = false;
                }
            }
        }
    }

}

//Generate link request
static uint16_t mle_router_synch(protocol_interface_info_entry_t *cur, const uint8_t *destAddress, uint8_t delay, uint8_t incoming_idr, bool retrans, bool register_short)
{
    mle_message_timeout_params_t timeout;
    uint16_t bufId = mle_service_msg_allocate(cur->id, 64, true, MLE_COMMAND_REQUEST);
    if (bufId == 0) {
        return 0;
    }

    uint8_t security_level = mle_service_security_level_get(cur->id);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);
    if (register_short) {
        ptr = mle_tlv_write_source_address(ptr, cur->lowpan_desired_short_address);
    } else {
        ptr = mle_general_write_source_address(ptr, cur);
    }
    ptr = mle_tlv_write_mode(ptr, lowpan_mode_get_by_interface_ptr(cur));
    if (security_level) {
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    }
    if (cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
        ptr = mle_general_write_timeout(ptr, cur);
    }

    if (destAddress && incoming_idr != 0){
        uint8_t mac64[8];
        memcpy(mac64, &destAddress[8], 8);
        mac64[0] ^= 2;
        ptr = mle_tlv_write_link_quality(ptr, incoming_idr, mac64, 0, false);
    }

    if (destAddress) {
        mle_service_set_msg_destination_address(bufId,destAddress);
    } else {
        mle_service_set_msg_destination_address(bufId,ADDR_LINK_LOCAL_ALL_ROUTERS);
    }

    if (retrans) {
        if (destAddress) {
            timeout.retrans_max = 3;
            timeout.timeout_init = 2;
            timeout.timeout_max = 4;
        } else {
            timeout.retrans_max = 2;
            timeout.timeout_init = 4;
            timeout.timeout_max = 4;
        }
    } else {
        timeout.retrans_max = 1;
        timeout.timeout_init = 2;
        timeout.timeout_max = 4;
    }

    timeout.delay = delay;

    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
            tr_debug("Buffer overflow at message write");
        }

    mle_service_set_msg_timeout_parameters(bufId, &timeout);
    mle_service_send_message(bufId);
    return bufId;
}

static int mle_router_accept_request_build(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, uint8_t *challenge, uint8_t chalLen, uint8_t type, uint8_t incoming_idr, uint8_t priority_flag)
{
    uint16_t bufId;
    uint8_t mac64[8];
    mle_message_timeout_params_t timeout;

    uint8_t security_level = mle_service_security_level_get(cur->id);



    if (type == MLE_COMMAND_ACCEPT) {
        bufId =  mle_service_msg_allocate(cur->id, 64, false,type);
        timeout.retrans_max = 0;
        timeout.timeout_init = 0;
        timeout.timeout_max = 0;
    } else {
        bufId =  mle_service_msg_allocate(cur->id, 64, true,type);
        timeout.retrans_max = 2;
        timeout.timeout_init = 2;
        timeout.timeout_max = 4;
    }

    if (bufId == 0) {
        return -1;
    }

    if (addr_is_ipv6_multicast(mle_msg->packet_dst_address)) {
        timeout.delay = MLE_STANDARD_RESPONSE_DELAY;
    } else {
        timeout.delay = MLE_NO_DELAY;
    }

    tr_debug("MLE Router Link Request response");

    mle_service_set_msg_destination_address(bufId,mle_msg->packet_src_address);

    uint8_t *ptr = mle_service_get_data_pointer(bufId);
    ptr = mle_tlv_write_mode(ptr, lowpan_mode_get_by_interface_ptr(cur));

    if (security_level) {
        ptr = mle_general_write_link_layer_framecounter(ptr, cur);
    }

    if (challenge && chalLen) {

        ptr = mle_tlv_write_response(ptr, challenge, chalLen);
    }

    memcpy(mac64, &mle_msg->packet_src_address[8], 8);
    mac64[0] ^= 2;
    ptr = mle_tlv_write_link_quality(ptr, incoming_idr, mac64, 0, priority_flag);

    ptr = mle_general_write_source_address(ptr, cur);
    if (mle_service_update_length_by_ptr(bufId,ptr)!= 0) {
            tr_debug("Buffer overflow at message write");
        }
    mle_service_set_msg_timeout_parameters(bufId, &timeout);

    if (type == MLE_COMMAND_ACCEPT) {
        mle_service_set_msg_token_bucket_priority(bufId);
    } else {
        mle_service_set_packet_callback(bufId, mle_accept_request_cb);
    }

    mle_service_send_message(bufId);
    return 0;
}

static void protocol_6lowpan_link_reject_handler(protocol_interface_info_entry_t *cur, uint8_t *ll64)
{
    mle_neigh_table_entry_t *entry_temp = mle_class_get_entry_by_ll64(cur->id, 0, ll64, false, NULL);
    tr_debug("MLE link reject");
    if (entry_temp) {
        mle_class_remove_entry(cur->id, entry_temp);
    }
}

static bool mle_child_update_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }

    if (mle_service_check_msg_response(msgId)) {
        return false;
    }

    if (usedAllRetries) {
        tr_debug("Link Update fail-->Reset bootstrap");
        mac_data_poll_protocol_poll_mode_decrement(cur);
        bootsrap_next_state_kick(ER_BOOTSTRAP_CONNECTION_DOWN, cur);

        return false;
    }
    mac_data_poll_protocol_poll_mode_decrement(cur);
    return true;
}

int protocol_6lowpan_child_update(protocol_interface_info_entry_t *cur)
{
    uint16_t bufId;
    uint8_t cordAddress[16];
    if (protocol_6lowpan_interface_get_link_local_cordinator_address(cur, cordAddress) != 0) {
        return -1;
    }

    bufId = mle_router_synch(cur, cordAddress, MLE_NO_DELAY, 0, true, false);
    if (bufId == 0) {
        return -1;
    }

    return mle_service_set_packet_callback(bufId, mle_child_update_cb);

}

static bool mle_parent_link_req_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }

    if (mle_service_check_msg_response(msgId)) {
        if (cur->nwk_bootstrap_state == ER_MLE_LINK_REQ) {
            //Enter ND scan
            bootsrap_next_state_kick(ER_SCAN, cur);
            pan_coordinator_blacklist_free(&cur->pan_cordinator_black_list);
        }
#ifdef HAVE_RPL
        else if (cur->nwk_bootstrap_state == ER_ROUTER_SYNCH) {
            //Trig RPL multicast dis
            cur->nwk_bootstrap_state = ER_RPL_MC;
            cur->bootsrap_state_machine_cnt = 55;
        }
#endif
        else if (cur->nwk_bootstrap_state == ER_MLE_LINK_ADDRESS_SYNCH) {
            mac_data_poll_protocol_poll_mode_disable(cur);
            bootsrap_next_state_kick(ER_BOOTSRAP_DONE, cur);

        } else if (cur->nwk_bootstrap_state == ER_MLE_LINK_SHORT_SYNCH) {
            tr_debug("MAC16 address synch ready");
            //SET Here 16-bit
            //SET MAC16 Address & LL16
            mac_helper_mac16_address_set(cur, cur->lowpan_desired_short_address);
            protocol_6lowpan_set_ll16(cur, cur->lowpan_desired_short_address);
            protocol_6lowpan_address_reg_ready(cur);
        }
        return false;
    }

    if (usedAllRetries) {
        switch (cur->nwk_bootstrap_state) {
            case ER_MLE_LINK_REQ:
            case ER_MLE_LINK_ADDRESS_SYNCH:
            case ER_MLE_LINK_SHORT_SYNCH:
                if (cur->nwk_bootstrap_state == ER_MLE_LINK_REQ) {
                    coordinator_black_list(cur);
                }
                tr_debug("Link synch fail %u", cur->nwk_bootstrap_state);
                bootsrap_next_state_kick(ER_BOOTSTRAP_CONNECTION_DOWN, cur);
                break;
#ifdef HAVE_RPL
            case ER_ROUTER_SYNCH:
                bootsrap_next_state_kick(ER_RPL_MC, cur);
                break;
#endif // HAVE_RPL
            default:
                break;
        }
        return false;
    }
    return true;
}

static bool mle_accept_request_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }

    if (mle_service_check_msg_response(msgId)) {
        return false;
    }

    if (usedAllRetries) {
        //If message has been sent by MLE service sends MLE reject to clear link
        if (mle_service_check_msg_sent(msgId)) {
            uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(msgId);
            tr_debug("No accept for Accept/Request");
            mle_service_reject_message_build(cur->id, address_ptr, false);
        }
        return false;
    }
    return true;
}

int protocol_6lowpan_parent_link_req(protocol_interface_info_entry_t *cur)
{
    uint16_t bufId;
    uint8_t cordAddress[16];
    if (protocol_6lowpan_interface_get_link_local_cordinator_address(cur, cordAddress) != 0) {
        return -1;
    }

    bufId = mle_router_synch(cur, cordAddress, MLE_NO_DELAY, 0, true, false);
    if (bufId == 0) {
        tr_debug("No Buf");
        return -1;
    }
    cur->nwk_bootstrap_state = ER_MLE_LINK_REQ;

    return mle_service_set_packet_callback(bufId, mle_parent_link_req_cb);

}
#ifdef HAVE_RPL
static int protocol_6lowpan_router_multicast_synch(protocol_interface_info_entry_t *cur)
{
    uint16_t bufId;

    bufId = mle_router_synch(cur, NULL, MLE_NO_DELAY, 0, true, false);

    if (bufId == 0) {
        return -1;
    }
    cur->nwk_bootstrap_state = ER_ROUTER_SYNCH;

    return mle_service_set_packet_callback(bufId, mle_parent_link_req_cb);
}
#endif

static int protocol_6lowpan_parent_address_synch(protocol_interface_info_entry_t *cur, bool register_short)
{
    uint16_t bufId;

    uint8_t cordAddress[16];
    if (protocol_6lowpan_interface_get_link_local_cordinator_address(cur, cordAddress) != 0) {
        return -1;
    }

    bufId = mle_router_synch(cur, cordAddress, MLE_NO_DELAY, 0, true, register_short);
    if (bufId == 0) {
        return -1;
    }
    if (register_short) {
        cur->nwk_bootstrap_state = ER_MLE_LINK_SHORT_SYNCH;

    } else {
        cur->nwk_bootstrap_state = ER_MLE_LINK_ADDRESS_SYNCH;
    }

    return mle_service_set_packet_callback(bufId, mle_parent_link_req_cb);
}

static bool mle_new_link_req_cb(int8_t interface_id, uint16_t msgId, bool usedAllRetries)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }

    uint8_t *address_ptr = mle_service_get_msg_destination_address_pointer(msgId);

    if (mle_service_check_msg_response(msgId)) {
        if (address_ptr && !addr_is_ipv6_multicast(address_ptr)) {
            // Remove from blacklist
            blacklist_update(address_ptr, true);
        }
        return false;
    }

    if (usedAllRetries) {
        if (address_ptr && !addr_is_ipv6_multicast(address_ptr)) {
            tr_warn("Sync fail to new router");
            // Add to blacklist or update current entry
            blacklist_update(address_ptr, false);
        }
        return false;
    }
    return true;
}

int protocol_6lowpan_router_synch_to_new_router(protocol_interface_info_entry_t *cur, uint8_t *ll64, uint8_t incoming_idr, bool retrans)
{
    uint16_t bufId;

    bufId = mle_router_synch(cur, ll64, MLE_STANDARD_RESPONSE_DELAY, incoming_idr, retrans, false);
    if (bufId == 0) {
        return -1;
    }

    return mle_service_set_packet_callback(bufId, mle_new_link_req_cb);
}


static uint8_t mle_calculate_idr(int8_t interface_id, mle_message_t *mle_msg, mle_neigh_table_entry_t *entry_temp)
{

    return etx_lqi_dbm_update(interface_id, mle_msg->lqi, mle_msg->dbm, entry_temp) >> 3;

}

static bool mle_6lowpan_neighbor_limit_check(int8_t interface_id, mle_message_t *mle_msg, uint8_t only_max_limit_chk)
{
    uint16_t mle_neigh_cnt;
    bool link_quality = false;

    if (!mle_6lowpan_data || mle_6lowpan_data->nbr_of_neigh_max == 0) {
        return true;
    }

    mle_neigh_cnt = mle_class_active_neigh_counter(interface_id);

    // Neighbor max limit
    if (mle_neigh_cnt >= mle_6lowpan_data->nbr_of_neigh_max) {
        tr_debug("Number of neighbor max limit");
        return false;
    }

    if (only_max_limit_chk) {
        return true;
    }

    if (mle_msg->message_type == MLE_COMMAND_REQUEST) {
        mle_tlv_info_t mle_tlv_info;
        if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_LINK_QUALITY, &mle_tlv_info) > 0) {
           link_quality = true;
        }
    }

    // Lower threshold reached, start to limit answering to multicast messages
    if (mle_neigh_cnt >= mle_6lowpan_data->nbr_of_neigh_lower_threshold) {
        // If multicast link request or link request was triggered from advertisement
        if (addr_is_ipv6_multicast(mle_msg->packet_dst_address) || link_quality == true) {

            // Upper threshold reached, no responses to multicasts anymore
            if (mle_neigh_cnt >= mle_6lowpan_data->nbr_of_neigh_upper_threshold) {
                return false;
            }

            uint16_t ignore_prob = randLIB_get_random_in_range(1,
                mle_6lowpan_data->nbr_of_neigh_upper_threshold - mle_6lowpan_data->nbr_of_neigh_lower_threshold);

            if (ignore_prob < (mle_neigh_cnt - mle_6lowpan_data->nbr_of_neigh_lower_threshold)) {
                return false;
            }
        }
    }

    return true;
}

void mle_6lowpan_message_handler(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
    uint8_t *t_ptr;
    uint8_t response_type;
    uint8_t mode = 0x0a;
    mle_tlv_info_t mle_tlv_info;
    mle_tlv_info_t mle_challenge;
    mle_neigh_table_entry_t *entry_temp;
    uint8_t linkMargin;
    uint8_t incoming_idr;
    uint16_t responseId, own_mac16;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    //Calculate link margin
    linkMargin = compute_link_margin(mle_msg->dbm);

    own_mac16 = mac_helper_mac16_address_get(cur);

    switch (mle_msg->message_type) {
        case MLE_COMMAND_REQUEST:
            tr_debug("Link REQ");
            if (!cur->global_address_available) {
                return;
            } else if (mle_validate_6lowpan_link_request_message(mle_msg->data_ptr, mle_msg->data_length,&mle_challenge) !=0 ) {
                return;
            }

            entry_temp = NULL;
            //If link request frame counter is invalid do not update neighbor entry and use three way handshake
            if (security_headers->invalid_frame_counter) {
                //Limit rate for triggering link requests
                if (!mle_6lowpan_data->link_req_token_bucket) {
                    return;
                }
                mle_6lowpan_data->link_req_token_bucket--;
            } else {
                //Update only old information based on link request
                entry_temp = mle_class_get_entry_by_ll64(interface_id, linkMargin, mle_msg->packet_src_address, false, NULL);
                if (entry_temp) {
                    mle_neigh_time_and_mode_update(entry_temp,mle_msg->data_ptr, mle_msg->data_length);
                    mle_neigh_entry_update_by_mle_tlv_list(interface_id, entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur->mac, own_mac16);
                    mle_neigh_entry_frame_counter_update(entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur, security_headers->KeyIndex);
                } else {
                    if (!mle_6lowpan_neighbor_limit_check(interface_id, mle_msg, false)) {
                        return;
                    }
                }
            }

            incoming_idr = mle_calculate_idr(interface_id, mle_msg, entry_temp);

            if (entry_temp && entry_temp->handshakeReady) {
                response_type = MLE_COMMAND_ACCEPT;
            } else {
                response_type = MLE_COMMAND_ACCEPT_AND_REQUEST;
            }
            mle_router_accept_request_build(cur, mle_msg, mle_challenge.dataPtr, mle_challenge.tlvLen, response_type, incoming_idr, false);
            break;

        case MLE_COMMAND_ACCEPT_AND_REQUEST:
        case MLE_COMMAND_ACCEPT:
            if (mle_msg->message_type == MLE_COMMAND_ACCEPT_AND_REQUEST) {
                if (mle_validate_6lowpan_link_request_message(mle_msg->data_ptr, mle_msg->data_length, &mle_challenge) != 0) {
                    return;
                }
            }
            //Validate Response First
            responseId = mle_tlv_validate_response(mle_msg->data_ptr, mle_msg->data_length);
            if (responseId == 0) {
                return;
            }

            tr_debug("Accept & Request");

            entry_temp = mle_class_get_entry_by_ll64(interface_id, linkMargin, mle_msg->packet_src_address, false, NULL);

            if (!entry_temp) {
                // If there is space for neighbors try to allocate new entry
                if (mle_6lowpan_neighbor_limit_check(interface_id, mle_msg, true)) {
                    entry_temp = mle_class_get_entry_by_ll64(interface_id, linkMargin, mle_msg->packet_src_address, true, NULL);
                }
            }

            if (!entry_temp) {
                tr_debug("Reject by no space");
                mle_service_reject_message_build(cur->id, mle_msg->packet_src_address, false);
                return;
            }

            //Init mle security counters

            //Response state set now timeout know positive state
            mle_service_set_msg_response_true(responseId);

            entry_temp->threadNeighbor = false;
            entry_temp->handshakeReady = 1;

            mac_data_poll_protocol_poll_mode_decrement(cur);

            //Read Source address and Challenge
            mle_neigh_time_and_mode_update(entry_temp,mle_msg->data_ptr, mle_msg->data_length);
            if (mle_msg->message_type == MLE_COMMAND_ACCEPT_AND_REQUEST) {
                // If no global address set priority (bootstrap ongoing)
                if (!cur->global_address_available) {
                    entry_temp->priorityFlag = true;
                }

                mle_neigh_entry_update_by_mle_tlv_list(cur->id, entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur->mac, own_mac16);
                incoming_idr = mle_calculate_idr(cur->id, mle_msg, entry_temp);
                mle_router_accept_request_build(cur, mle_msg, mle_challenge.dataPtr, mle_challenge.tlvLen, MLE_COMMAND_ACCEPT, incoming_idr, entry_temp->priorityFlag);
            } else {
                mle_neigh_entry_update_by_mle_tlv_list(cur->id, entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur->mac, own_mac16);
                incoming_idr = mle_calculate_idr(cur->id, mle_msg, entry_temp);
            }
            mle_neigh_entry_frame_counter_update(entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur, security_headers->KeyIndex);
            // If MLE frame counter was invalid update its value since three way handshake is complete
            if (security_headers->invalid_frame_counter) {
                entry_temp->mle_frame_counter = security_headers->frameCounter;
            }
            break;

        case MLE_COMMAND_REJECT:
            if (security_headers->invalid_frame_counter) {
                return;
            }
            protocol_6lowpan_link_reject_handler(cur, mle_msg->packet_src_address);
            break;

        case MLE_COMMAND_ADVERTISEMENT:
            tr_info("Received MLE Advertisement from %s", trace_ipv6(mle_msg->packet_src_address));
            if (!cur->global_address_available || security_headers->invalid_frame_counter) {
                tr_error("MLE adv: Invalid frame counter: %s", security_headers->invalid_frame_counter ? "true" : "false");
                return;
            } else {
                uint8_t drop_advertisment = 0;

                if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_MODE, &mle_tlv_info) > 0) {
                    t_ptr = mle_tlv_info.dataPtr;
                    mode = *t_ptr;
                }

                entry_temp = mle_class_get_entry_by_ll64(interface_id, linkMargin, mle_msg->packet_src_address, false, NULL);
                if (!entry_temp) {
                    if ((mode & MLE_DEV_MASK) == MLE_FFD_DEV) {
                        // If there is space for neighbors synchronizes to new router
                        if (mle_6lowpan_neighbor_limit_check(interface_id, mle_msg, false)) {
                            // Checks blacklist
                            if (blacklist_reject(mle_msg->packet_src_address)) {
                                return;
                            }
                            incoming_idr = mle_calculate_idr(interface_id, mle_msg, NULL);
                            protocol_6lowpan_router_synch_to_new_router(cur, mle_msg->packet_src_address, 0, false);
                        }
                    }
                    tr_error("MLE adv: No MLE entry");
                    return;
                }

                //Verify Own Address
                drop_advertisment = 1;
                //Disvover own address only when we are aloocated address
                if (mle_tlv_option_discover(mle_msg->data_ptr, mle_msg->data_length, MLE_TYPE_LINK_QUALITY, &mle_tlv_info) > 0) {
                    uint8_t link_flags;
                    if (mle_tlv_info.tlvLen > 0) {
                        link_flags = *(mle_tlv_info.dataPtr);
                        if (mle_link_quality_tlv_parse(cur->mac,mac_helper_mac16_address_get(cur) , mle_tlv_info.dataPtr, mle_tlv_info.tlvLen, NULL, NULL))
                        {
                            drop_advertisment = 0;
                        }

                        if (drop_advertisment) {
                            if (link_flags & 0x80) {
                                //Total Entry at messgae
                                //Possible remove
                                if ((mode & MLE_DEV_MASK) == MLE_RFD_DEV) {
                                    //Remove Entry
                                    mle_class_remove_entry(cur->id, entry_temp);
                                    tr_error("MLE adv: Own address not found");
                                    return;
                                }
                            }
                        }
                    }
                }

                //UPDATE
                mle_neigh_entry_update_by_mle_tlv_list(cur->id,entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur->mac, own_mac16);
                mle_neigh_entry_frame_counter_update(entry_temp, mle_msg->data_ptr, mle_msg->data_length, cur, security_headers->KeyIndex);
                if (entry_temp->handshakeReady) {
                    mle_entry_timeout_refresh(entry_temp);
                }
            }
            break;

        default:
            break;
    }
}

int8_t arm_6lowpan_mle_service_ready_for_security_init(protocol_interface_info_entry_t *cur)
{
    //Verify MLE Service
    if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
        //validate MLE service
        if (!mle_service_interface_registeration_validate(cur->id)) {
            //Register
            if (mle_service_interface_register(cur->id,mle_6lowpan_message_handler, cur->mac,8) != 0) {
                tr_error("Mle Service init Fail");
                return -1;
            }
            if (mle_6lowpan_data) {
                if (mle_service_interface_token_bucket_settings_set(cur->id, mle_6lowpan_data->token_bucket_size,
                    mle_6lowpan_data->token_bucket_rate, mle_6lowpan_data->token_bucket_count) < 0) {
                    return -1;
                }
                mle_service_set_frame_counter_check(true);
                mle_service_set_accept_invalid_frame_counter(true);
            }
        }
    }
    return 0;
}

static uint8_t protocol_6lowpan_mle_data_allocate(void)
{
    if (mle_6lowpan_data) {
        return 0;
    }

    mle_6lowpan_data = ns_dyn_mem_alloc(sizeof(mle_6lowpan_data_t));

    if (!mle_6lowpan_data) {
        return 0;
    }

    mle_6lowpan_data->router_lifetime = MLE_ROUTER_DEFAULT_LIFETIME;
    mle_6lowpan_data->host_lifetime = MLE_ROUTER_HOST_LIFETIME;
    mle_6lowpan_data->nbr_of_neigh_lower_threshold = MLE_NBR_OF_NEIGHBOR_MAX_LIMIT;
    mle_6lowpan_data->nbr_of_neigh_upper_threshold = MLE_NBR_OF_NEIGHBOR_LOWER_THRESHOLD;
    mle_6lowpan_data->nbr_of_neigh_max = MLE_NBR_OF_NEIGHBOR_UPPER_THRESHOLD;

    mle_6lowpan_data->token_bucket_size = MLE_TOKEN_BUCKET_SIZE;
    mle_6lowpan_data->token_bucket_rate = MLE_TOKEN_BUCKET_RATE;
    mle_6lowpan_data->token_bucket_count = MLE_TOKEN_BUCKET_COUNT;

    mle_6lowpan_data->link_req_token_bucket = MLE_LINK_REQ_TOKEN_BUCKET_SIZE;

    return 0;
}

mle_6lowpan_data_t *protocol_6lowpan_mle_data_get(void)
{
    return mle_6lowpan_data;
}

static void protocol_6lowpan_mle_purge_neighbors(struct protocol_interface_info_entry *cur_interface, uint8_t entry_count, uint8_t force_priority)
{
    mle_neigh_table_list_t *mle_neigh_table;
    uint8_t count = 0;
    uint8_t ll64[16];

    if (!cur_interface) {
        return;
    }

    mle_neigh_table = mle_class_active_list_get(cur_interface->id);

    if (!mle_neigh_table) {
        return;
    }

    entry_count = protocol_6lowpan_mle_order_last_entries(mle_neigh_table, entry_count);

    ns_list_foreach_reverse_safe(mle_neigh_table_entry_t, entry, mle_neigh_table) {
        if (++count > entry_count) {
            break;
        }

        if (!force_priority) {
            if (entry->priorityFlag || entry->priority_child_flag) {
                break;
            }
        }

        memcpy(ll64, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(&ll64[8], entry->mac64, 8);
        ll64[8] ^= 2;

        tr_debug("MLE purge");

        // Sends REJECT
        mle_service_reject_message_build(cur_interface->id, ll64, false);
        mle_class_remove_entry(cur_interface->id, entry);

        // Adds purged neighbor to blacklist so that it is not added right away back from advertisement
        blacklist_update(ll64, false);
    }
}

static uint8_t protocol_6lowpan_mle_order_last_entries(mle_neigh_table_list_t *mle_neigh_table, uint8_t entry_count)
{
    mle_neigh_table_entry_t *last;
    mle_neigh_table_entry_t *first_ordered = NULL;
    uint8_t count = 0;

    do {
        last = NULL;

        ns_list_foreach(mle_neigh_table_entry_t, entry, mle_neigh_table) {

            if (entry == first_ordered) {
                break;
            }

            if (last == NULL) {
                last = entry;
                continue;
            }

            // Primary parent (parent selected for bootstrap or RPL primary parent)
            if (entry->priorityFlag && !last->priorityFlag) {
                continue;
            }

            // Secondary parent (RPL secondary parent)
            if (entry->second_priority_flag && !last->second_priority_flag) {
                continue;
            }

            // Uses this node as parent
            if (entry->priority_child_flag && !last->priority_child_flag) {
                continue;
            }

            // Better ETX
            if (entry->etx <= last->etx) {
                continue;
            }

            last = entry;
        }

        // Sets last to end of list
        if (last) {
            ns_list_remove(mle_neigh_table, last);

            if (first_ordered) {
                ns_list_add_before(mle_neigh_table, first_ordered, last);
            } else {
                ns_list_add_to_end(mle_neigh_table, last);
            }

            first_ordered = last;

            count++;

            if (count == entry_count) {
                break;
            }
        // If no lasts anymore then exits
        } else {
            break;
        }

    } while (true);

    return count;
}

static int8_t arm_6lowpan_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }
    mac_data_poll_disable(cur);
    /* Save security counter values to RAM and NVM */
    if (cur->nwk_wpan_nvm_api) {
        cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
    }
    cur->if_lowpan_security_params->mle_security_frame_counter = mle_service_security_get_frame_counter(cur->id);
    mle_service_interface_receiver_handler_update(cur->id, mle_6lowpan_message_handler);
    return nwk_6lowpan_down(cur);
}
#ifdef HAVE_6LOWPAN_ND

static void lowpan_mle_receive_security_bypass_cb(int8_t interface_id, mle_message_t *mle_msg)
{
#ifdef PANA
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    //Accept Only Link Reject
    if (interface && mle_msg->message_type == MLE_COMMAND_REJECT) {

        if ((interface->lowpan_info & (INTERFACE_NWK_BOOTSRAP_ACTIVE | INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION)) != (INTERFACE_NWK_BOOTSRAP_ACTIVE | INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION)) {
                return;
            }

            if (protocol_6lowpan_interface_compare_cordinator_netid(interface, mle_msg->packet_src_address + 8) != 0) {
                return;
            }

            if (interface->nwk_bootstrap_state != ER_PANA_AUTH) {
                return;
            }

            //Stop Pana and call ECC
            tr_debug("MLE Link reject from cordinator");
            pana_reset_client_session();
            bootsrap_next_state_kick(ER_PANA_AUTH_ERROR, interface);
    }
#endif
}

void arm_6lowpan_security_init_ifup(protocol_interface_info_entry_t *cur)
{
    if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {

        mle_service_security_init(cur->id, cur->if_lowpan_security_params->security_level,cur->if_lowpan_security_params->mle_security_frame_counter,  NULL, protocol_6lowpan_mle_service_security_notify_cb);
        switch (cur->if_lowpan_security_params->nwk_security_mode) {

            case NET_SEC_MODE_PSK_LINK_SECURITY:

            mle_service_security_set_security_key(cur->id, cur->if_lowpan_security_params->psk_key_info.security_key, cur->if_lowpan_security_params->psk_key_info.key_id, true);
            mle_service_security_set_frame_counter(cur->id, cur->if_lowpan_security_params->mle_security_frame_counter);
            break;
            case NET_SEC_MODE_PANA_LINK_SECURITY:
                mle_service_interface_receiver_bypass_handler_update(cur->id, lowpan_mle_receive_security_bypass_cb);
                break;
            default:
                break;
        }
    }

    cur->mac_parameters->mac_key_id_mode = MAC_KEY_ID_MODE_IDX;
    cur->mac_parameters->mac_configured_sec_level = cur->if_lowpan_security_params->security_level;
    switch (cur->if_lowpan_security_params->nwk_security_mode) {

        case NET_SEC_MODE_PANA_LINK_SECURITY:
            cur->lowpan_info |= (INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION);
            break;

        case NET_SEC_MODE_PSK_LINK_SECURITY:
            mac_helper_security_default_key_set(cur, cur->if_lowpan_security_params->psk_key_info.security_key, cur->if_lowpan_security_params->psk_key_info.key_id, MAC_KEY_ID_MODE_IDX);
            /* fall through */
        default:
            cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION;
            break;
    }
}
#endif

static int8_t arm_6lowpan_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;
    if ((cur->configure_flags & INTERFACE_SETUP_MASK) != INTERFACE_SETUP_READY) {
        tr_debug("Interface not yet fully configured");
        ret_val = -5;
    } else {

        //Verify MLE Service
        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
            //validate MLE service
            if (!mle_service_interface_registeration_validate(cur->id)) {
                //Register

                if (mle_service_interface_register(cur->id,mle_6lowpan_message_handler, cur->mac,8) != 0) {
                    tr_error("Mle Service init Fail");
                    return -1;
                }
            }

            if (mle_6lowpan_data) {
                if (mle_service_interface_token_bucket_settings_set(cur->id, mle_6lowpan_data->token_bucket_size,
                                                                    mle_6lowpan_data->token_bucket_rate, mle_6lowpan_data->token_bucket_count) < 0) {
                    tr_error("Mle Service tokens set Fail");
                    return -1;
                }
                mle_service_set_frame_counter_check(true);
                mle_service_set_accept_invalid_frame_counter(true);
            }
        }

        arm_6lowpan_security_init_ifup(cur);

        //SET 6lowpan default here
        mac_helper_mac_mlme_max_retry_set(cur->id, LOWPAN_MAX_FRAME_RETRIES);

        addr_interface_set_ll64(cur, NULL);
        if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER) {
            cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
            //rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, true);
            icmpv6_radv_enable(cur);
        }
        ret_val = nwk_6lowpan_up(cur);
    }
    return ret_val;
}
#endif

void arm_6lowpan_bootstrap_init(protocol_interface_info_entry_t *cur)
{
    //Init 6lowpan Bootsrap
    icmp_nd_routers_init();
    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ACTIVE;
    cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY;
    bootsrap_next_state_kick(ER_SCAN, cur);
    mac_helper_mac16_address_set(cur, 0xffff);
}

#ifdef HAVE_6LOWPAN_ND
static void arm_6lowpan_bootstrap_functions_set(protocol_interface_info_entry_t *cur)
{
    cur->if_up = arm_6lowpan_bootstrap_up;
    cur->if_down = arm_6lowpan_bootstrap_down;
}

static uint8_t protocol_6lowpan_analyze_beacon_payload(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    (void)len;
    nwk_filter_params_s *filter = &(cur->mac_parameters->nwk_filter_params);

    if (*ptr == filter->beacon_protocol_id_filter || filter->beacon_protocol_id_filter == 0xff) {
        ptr++;
        if (filter->nwk_active_scan_level == 2) {
            if ((*ptr & 1)) {
                if (filter->beacon_nwk_id_filter) {
                    ptr++;
                    if (memcmp(filter->beacon_nwk_id_filter, ptr, 16)) {
                        tr_debug("NWK ID filter");
                        return 0;
                    }
                }
                return 1;
            }
        } else {
            return 1;
        }
    }

    return 0;
}

int8_t arm_network_processor_up(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = -1;
    if ((cur->configure_flags & INTERFACE_SETUP_NETWORK_DRIVER_MASK) != INTERFACE_SETUP_NETWORK_DRIVER_READY) {
        tr_debug("Interface not yet fully configured\n");
        ret_val = -5;
    } else {
        protocol_6lowpan_register_handlers(cur);
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
        mle_class_mode_set(cur->id, MLE_CLASS_ROUTER);
        mac_helper_default_security_level_set(cur, SEC_NONE);

        if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_SNIFFER) {
            mac_helper_pib_boolean_set(cur, macAssociationPermit, false);
            mac_helper_pib_boolean_set(cur, macPromiscuousMode, true);
            lowpan_bootstrap_pan_control(cur, false);

        } else if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT) {
            // Updates beacon
            beacon_join_priority_update(cur->id);
            mac_helper_pib_boolean_set(cur, macAssociationPermit, true);
            net_load_balance_internal_state_activate(cur, true);
        }

        cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
        cur->interface_mode = INTERFACE_UP;
        cur->nwk_mode = ARM_NWK_RAW_PHY_MODE;
        cur->lowpan_info |= (INTERFACE_NWK_ACTIVE | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY);
        cur->bootsrap_state_machine_cnt = 0;
        nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);

        ret_val = 0;
    }
    return ret_val;
}

static bool lowpan_interface_is_active(int8_t interface_id) {
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return false;
    }

    return true;
}

static void arm_6lowpan_security_key_update_cb(protocol_interface_info_entry_t *cur, const mlme_security_t *security_params)
{
    if (cur->mac_parameters->mac_next_key_index && (security_params->KeyIndex == cur->mac_parameters->mac_next_key_index)) {
        tr_debug("Trig Next Key");
        mac_helper_security_key_swap_next_to_default(cur);
        mle_service_security_key_trig(cur->id, security_params->KeyIndex);
        if (cur->nwk_wpan_nvm_api) {
            cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
        }
    }
}
int8_t arm_6lowpan_bootstarp_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension)
{
    int8_t ret_val = -1;
    bool enable_mle_protocol = true;
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    arm_6lowpan_bootstrap_functions_set(cur);
    cur->mac_parameters->beacon_ind = protocol_6lowpan_analyze_beacon_payload;

    mac_beacon_link_beacon_join_priority_tx_callback_set(cur->id, protocol_6lowpan_beacon_join_priority_tx);
    mac_beacon_link_beacon_compare_rx_callback_set(cur->id, protocol_6lowpan_beacon_compare_rx);

    if (net_6lowpan_mode_extension == NET_6LOWPAN_ND_WITHOUT_MLE) {
        enable_mle_protocol = false;
    }

    cur->mac_security_key_usage_update_cb = arm_6lowpan_security_key_update_cb;
    //Allocate MLE class here
    //Deallocate old here

    mle_class_deallocate(interface_id);

    if (enable_mle_protocol) {

        mac_description_storage_size_t buffer;
        //Read MAC device table sizes
        if (cur->mac_api->mac_storage_sizes_get(cur->mac_api, &buffer) != 0) {
            return -1;
        }
        if (mle_class_init(interface_id, buffer.device_decription_table_size, &protocol_6lowpan_neighbor_information_remove, &protocol_6lowpan_host_challenge, &lowpan_interface_is_active) != 0) {
            return -1;
        }
        mle_class_router_challenge(interface_id, protocol_6lowpan_router_challenge);
    }

    mle_service_interface_unregister(cur->id);

    if (bootstrap_mode == NET_6LOWPAN_NETWORK_DRIVER || bootstrap_mode == NET_6LOWPAN_SNIFFER) {
        enable_mle_protocol = false;
        cur->if_up = arm_network_processor_up;
        if (bootstrap_mode == NET_6LOWPAN_NETWORK_DRIVER) {
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT;
            cur->mac_parameters->beacon_ind = NULL; //Drop beacons
        } else {
            cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_SNIFFER;
        }
        cur->lowpan_info &= ~INTERFACE_NWK_ROUTER_DEVICE;
        cur->lowpan_info |= INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
        arm_nwk_6lowpan_borderrouter_data_free(cur);

        ret_val = 0;
        goto bootstrap_finish_check;
    } else {
        if (enable_mle_protocol) {
#ifdef NO_MLE
            return -2;
#else
            cur->comm_status_ind_cb = lowpan_comm_status_indication_cb;
            if (mle_service_interface_register(cur->id,mle_6lowpan_message_handler, cur->mac,8) != 0) {
                tr_error("Mle Service init Fail");
                return -1;
            }

            if (protocol_6lowpan_mle_data_allocate() != 0) {
                tr_error("MLE data allocate Fail");
                return -1;
            }

            if (blacklist_init() != 0) {
                tr_error("Blacklist init Fail");
                return -1;
            }

            if (mle_6lowpan_data) {
                if (mle_service_interface_token_bucket_settings_set(cur->id, mle_6lowpan_data->token_bucket_size,
                    mle_6lowpan_data->token_bucket_rate, mle_6lowpan_data->token_bucket_count) < 0) {
                    tr_error("Mle Service tokens set Fail");
                    return -1;
                }
                mle_service_set_frame_counter_check(true);
                mle_service_set_accept_invalid_frame_counter(true);
            }
#endif
        }

        arm_6lowpan_bootstrap_functions_set(cur);
        cur->configure_flags &= ~INTERFACE_BOOTSTRAP_DEFINED;
        switch (bootstrap_mode) {
            case NET_6LOWPAN_HOST:
                protocol_6lowpan_host_init(cur, false);
                ret_val = 0;
                break;
            case NET_6LOWPAN_SLEEPY_HOST:
                protocol_6lowpan_host_init(cur, true);
                ret_val = 0;
                break;

            case NET_6LOWPAN_ROUTER:
                protocol_6lowpan_router_init(cur);
                ret_val = 0;
                break;

            case NET_6LOWPAN_BORDER_ROUTER:
                ret_val = arm_nwk_6lowpan_borderrouter_init(cur);
                cur->mac_parameters->beacon_ind = NULL; //Drop beacons
                break;
            default:
                break;

        }
    }

    bootstrap_finish_check:
    if (ret_val == 0) {
        /**
         *  Do Thread dealloc
         */
        thread_info_deallocate(cur);
        //ADD RPL Support if supported and device is router
#ifdef HAVE_RPL
        /**
         *  ADD RPL Flag If device is router
         */
        if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
            //rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, true);
            //rpl_control_set_callback(protocol_6lowpan_rpl_domain, protocol_6lowpan_bootstrap_rpl_callback, cur);
        }
#endif
        cur->configure_flags |= INTERFACE_BOOTSTRAP_DEFINED;
        if (enable_mle_protocol) {
            cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_MLE;
        } else {
            cur->lowpan_info &= ~INTERFACE_NWK_BOOTSRAP_MLE;
        }
    }

    return ret_val;
}


static void protocol_6lowpan_bootstrap_icmp_rs_msg_tx(protocol_interface_info_entry_t *cur)
{
    buffer_t *buf = icmpv6_build_rs(cur, NULL);

    protocol_push(buf);
}

void nwk_6lowpan_router_scan_state(protocol_interface_info_entry_t *cur)
{
    cur->nwk_rpl_scan_counter = 0;
    if (cur->nwk_nd_re_scan_count == 0) {
        if (cur->border_router_setup) {
            //Activate RS
            arm_border_router_ready(cur);
        } else {
            tr_warn("No ND Router");
            nwk_bootsrap_state_update(ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL, cur);
        }

    } else {
        //Verify is ND Object allocated already
        if (!cur->border_router_setup && nd_object_active()) {
            tr_debug("Wait response from  ND");
        } else {
            tr_debug("RS*");
            protocol_6lowpan_bootstrap_icmp_rs_msg_tx(cur);
            cur->nwk_nd_re_scan_count--;
            if (cur->border_router_setup) {
                cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(25, 50);
            } else {
                if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
                    cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(25, 40);
                } else {
                    cur->bootsrap_state_machine_cnt = randLIB_get_random_in_range(60, 70);
                }
            }
        }
    }
}

void nwk_6lowpan_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
    if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {
        uint8_t bootsrap_ready = 0;

        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {

            if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
                if (pana_ping_notify_msg_tx(cur->mac_parameters->pan_id) == 0) {
                    tr_warn("PING TX fail");
                } else {
                    bootsrap_ready = 1;
                }

            } else {
                if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
#ifndef NO_MLE
                    tr_debug("MLE Parent Advertisment");
                    if (protocol_6lowpan_mle_neigh_advertise(cur) == 0) {
                        bootsrap_ready = 1;
                    } else {
                        tr_warn("MLE Host Parent Advert TX fail");
                    }
#endif
                } else {
                    bootsrap_ready = 1;
                }
            }
        } else {
            bootsrap_ready = 1;

        }
        if (bootsrap_ready) {
            if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
                // Updates beacon
                beacon_join_priority_update(cur->id);
                lowpan_bootstrap_pan_control(cur, true);
            }
            nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);
        } else {
            cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
            cur->bootsrap_state_machine_cnt = 2;
        }
    }
}

void protocol_6lowpan_link_advertise_handle(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t tick)
{
    if ((cur_interface->lowpan_info & (INTERFACE_NWK_BOOTSRAP_MLE  | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) == (INTERFACE_NWK_BOOTSRAP_MLE  | INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
#ifndef NO_MLE
        if (cur->mle_advert_timer) {
            if (cur->mle_advert_timer > tick) {
                cur->mle_advert_timer -= tick;

            } else {
                if (protocol_6lowpan_mle_neigh_advertise(cur_interface) == 0) {

                    if (cur_interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
                        cur->mle_advert_timer = 0;
                    } else {
                        cur->mle_advert_timer = 155;

                        if (mle_6lowpan_data) {
                            uint16_t period = mle_6lowpan_data->router_lifetime / 4;
                            if (period > 640) {
                                period = 640;
                            }
                            period *= 10; //seconds ticks --> 100ms ticks
                            //set 0.9 - 1.1 * period
                            cur->mle_advert_timer = randLIB_randomise_base(period, LOWPAN_RAND_LOW, LOWPAN_RAND_HIGH);
                        }
                    }
                } else {
                    cur->mle_advert_timer = 2;
                }

                if (cur->mle_purge_timer) {
                    cur->mle_purge_timer -= 1;
                } else {
                    if (mle_6lowpan_data && mle_6lowpan_data->nbr_of_neigh_max != 0) {
                        uint16_t mle_neigh_cnt = mle_class_active_neigh_counter(cur_interface->id);
                        if (mle_neigh_cnt > (mle_6lowpan_data->nbr_of_neigh_max - MLE_NEIGHBOR_PURGE_NBR)) {
                            protocol_6lowpan_mle_purge_neighbors(cur_interface, MLE_NEIGHBOR_PURGE_NBR, true);
                        }

                        if (mle_neigh_cnt > (mle_6lowpan_data->nbr_of_neigh_upper_threshold - MLE_NEIGHBOR_PURGE_NBR)) {
                            protocol_6lowpan_mle_purge_neighbors(cur_interface, MLE_NEIGHBOR_PURGE_NBR, false);
                        }

                        uint16_t mle_purge_timer;
                        /* From 1.0 to 1.5 * MLE_NEIGHBOR_PURGE_TIMER */
                        mle_purge_timer = randLIB_randomise_base(MLE_NEIGHBOR_PURGE_TIMER_TIMEOUT, 0x8000, 0xC000);
                        cur->mle_purge_timer = mle_purge_timer;
                    }
                }
                // Updates blacklist timer
                blacklist_ttl_update(1);
            }
        }
#endif
    } else {
        cur->mle_advert_timer = 0;
    }
}

static void protocol_6lowpan_nd_ready(protocol_interface_info_entry_t *cur)
{
    if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE)) {
        tr_debug("ND BS ready");
        bootsrap_next_state_kick(ER_BIND_COMP, cur);
        clear_power_state(ICMP_ACTIVE);
        cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY;
    } else {
        tr_debug("RE ND ready");
        clear_power_state(ICMP_ACTIVE);
        mac_data_poll_protocol_poll_mode_disable(cur);
        //TRIG MLE Challenge for Normal Host
        if ((cur->lowpan_info & (INTERFACE_NWK_ROUTER_DEVICE | INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE | INTERFACE_NWK_BOOTSRAP_MLE)) == INTERFACE_NWK_BOOTSRAP_MLE) {
            //TRIG Only Normal Host
#ifndef NO_MLE
            //GET Cordinaotor MLE Entry
            addrtype_t addrType;
            uint8_t tempAddr[8];
            addrType = mac_helper_coordinator_address_get(cur, tempAddr);

            mle_neigh_table_entry_t *entry_t = mle_class_get_by_link_address(cur->id, tempAddr, addrType);
            if (entry_t) {
                if (entry_t->ttl > MLE_TABLE_CHALLENGE_TIMER) {
                    entry_t->ttl = (MLE_TABLE_CHALLENGE_TIMER + 1);
                }
            }
#endif
        }
    }
}

static void protocol_6lowpan_address_reg_ready(protocol_interface_info_entry_t *cur_interface)
{
    nd_router_t *cur;
    cur = nd_get_object_by_nwk_id(cur_interface->nwk_id);

    if(!cur) {
        return;
    }

    cur->nd_timer = 10;
    cur->ns_forward_timer = 0;

    uint16_t mle_timer = 0;
    protocol_6lowpan_nd_ready(cur_interface);
    if (cur_interface->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        addr_add_router_groups(cur_interface);
        addr_add_group(cur_interface, ADDR_REALM_LOCAL_ALL_ROUTERS);
        icmpv6_radv_enable(cur_interface);
        icmpv6_restart_router_advertisements(cur_interface, cur->border_router);
        /* Stop the ND revalidate timer - this means we don't do RS again */
        cur->nd_re_validate = 0;
        mle_timer = 300;
    } else {
        if (cur_interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
            mac_data_poll_protocol_poll_mode_decrement(cur_interface);
            mle_timer = 20;
        } else {
            mle_timer = 155;
        }
    }
    if (cur_interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
        if (cur->mle_advert_timer == 0) {
            cur->mle_advert_timer = mle_timer;
            cur->mle_purge_timer = MLE_NEIGHBOR_PURGE_TIMER_TIMEOUT;
        }
    } else {
        cur->mle_advert_timer = 0;
    }
}

void protocol_6lowpan_bootstrap_nd_ready(protocol_interface_info_entry_t *cur_interface)
{

    tr_debug("ND Ready");



    if (cur_interface->lowpan_address_mode == NET_6LOWPAN_GP64_ADDRESS) {
        protocol_6lowpan_address_reg_ready(cur_interface);
    } else {
        //Here we need to verify address mode
        tr_debug("Synch MAC16 with parent");
        if (protocol_6lowpan_parent_address_synch(cur_interface, true) != 0 ) {
            nwk_bootsrap_state_update(ARM_NWK_NWK_CONNECTION_DOWN, cur_interface);
        }
    }


}

#ifdef HAVE_RPL

static void protocol_6lowpan_bootstrap_rpl_callback(rpl_event_t event, void *handle)
{

    protocol_interface_info_entry_t *cur = handle;
    if (!cur->rpl_domain || cur->interface_mode != INTERFACE_UP) {
        return;
    }
    switch (event) {
        case RPL_EVENT_DAO_DONE:
            if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE)) {
                bootsrap_next_state_kick(ER_BOOTSRAP_DONE, cur);
                clear_power_state(ICMP_ACTIVE);
            } else if (cur->nwk_bootstrap_state == ER_RPL_LOCAL_REPAIR) {
                // Updates beacon
                cur->bootsrap_state_machine_cnt = 0;
                cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
                beacon_join_priority_update(cur->id);
                lowpan_bootstrap_pan_control(cur, true);
            }
            break;

        case RPL_EVENT_LOCAL_REPAIR_START:
            if (!(cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE)) {
                tr_error("RPL Local repair started");
                lowpan_bootstrap_pan_control(cur, false);
                cur->bootsrap_state_machine_cnt = 0;
                cur->nwk_bootstrap_state = ER_RPL_LOCAL_REPAIR;
            }
            break;

        case RPL_EVENT_LOCAL_REPAIR_NO_MORE_DIS:
            tr_error("RPL Local repair fail-->interface to idle");
            nwk_bootsrap_state_update(ARM_NWK_NWK_CONNECTION_DOWN, cur);
            break;
    }
}

/**
 * \brief Send ICMP RPL DIS message.
 *
 * \return 0 , buffer_allocation fail & 1 message sent
 */
uint8_t nwk_bootstrap_icmp_rpl_dis_msg_tx(protocol_interface_info_entry_t *cur)
{
    if (cur->rpl_domain) {
        rpl_control_transmit_dis(cur->rpl_domain, cur, 0, 0, NULL, 0, NULL);
        return 1;
    }

    return 0;
}

/**
 * \brief Send ICMP RPL DIS message to bootstrap coordinator
 *
 * \return 0 , buffer_allocation fail & 1 message sent
 */
static uint8_t nwk_bootstrap_icmp_rpl_dis_coord_msg_tx(protocol_interface_info_entry_t *cur)
{
    if (!cur->rpl_domain) {
        return 0;
    }

    uint8_t coord_address[16];
    if (protocol_6lowpan_interface_get_link_local_cordinator_address(cur, coord_address) != 0) {
        tr_debug("Unicast DIS no coord");
        return 0;
    }

    rpl_control_transmit_dis(cur->rpl_domain, cur, 0, 0, NULL, 0, coord_address);
    return 1;
}

static void nwk_rpl_dio_scan(protocol_interface_info_entry_t *cur)
{
    if (cur->nwk_rpl_scan_counter < MAX_MC_DIS_COUNT) {
        if (nwk_bootstrap_icmp_rpl_dis_msg_tx(cur)) {
            cur->bootsrap_state_machine_cnt = 45 << cur->nwk_rpl_scan_counter;
            cur->nwk_rpl_scan_counter++;
            tr_debug("MC_DIS\n");
            cur->nwk_bootstrap_state = ER_RPL_SCAN;
        } else {
            cur->bootsrap_state_machine_cnt = 3;
        }
    } else {
        //GivE Up Bootsrap
        nwk_bootsrap_state_update(ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL, cur);
    }
}


void nwk_6lowpan_rpl_router_discover(protocol_interface_info_entry_t *cur)
{
    if (cur->rpl_domain) {
        tr_debug("MC DIS Force");
        if (nwk_bootstrap_icmp_rpl_dis_msg_tx(cur)) {
            cur->nwk_bootstrap_state = ER_RPL_SCAN;
            cur->bootsrap_state_machine_cnt = 55;
        } else {
            cur->nwk_bootstrap_state = ER_RPL_MC;
            cur->bootsrap_state_machine_cnt = 15;
        }
    } else {
        cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
        nwk_6lowpan_bootstrap_ready(cur);
    }
}

void nwk_6lowpan_rpl_router_result_check(protocol_interface_info_entry_t *cur)
{
    if (cur->rpl_domain) {
        if (rpl_control_have_dodag(cur->rpl_domain)) {
            tr_debug("UNI DIS");
            cur->bootsrap_state_machine_cnt = 0;
        } else {
            nwk_rpl_dio_scan(cur);
        }
    } else {
        cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
        nwk_6lowpan_bootstrap_ready(cur);
    }
}
#endif

void nwk_6lowpan_nd_address_registartion_ready(protocol_interface_info_entry_t *cur)
{
    tr_debug("ND Ready!!");
    cur->nwk_rpl_scan_counter = 0;

#ifndef NO_MLE
    if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        if (protocol_6lowpan_router_multicast_synch(cur) != 0) {
            tr_debug("Router link request start fail");
            nwk_bootsrap_state_update(ARM_NWK_NWK_CONNECTION_DOWN, cur);
        }
#ifdef HAVE_RPL
        if (protocol_6lowpan_rpl_domain) {
            // arm_nwk_6lowpan_rpl_dodag_poison from a previous connection may have left force_leaf set
            rpl_control_force_leaf(protocol_6lowpan_rpl_domain, false);
            rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, true);
            rpl_control_set_callback(protocol_6lowpan_rpl_domain, protocol_6lowpan_bootstrap_rpl_callback, cur);
        }
        // Send unicast DIS to coordinator
        nwk_bootstrap_icmp_rpl_dis_coord_msg_tx(cur);
#endif /* HAVE_RPL */
    } else {
        //No point to update link
        if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST || cur->lowpan_address_mode != NET_6LOWPAN_GP64_ADDRESS) {
            if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST) {
                cur->lowpan_info |= INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
                tr_debug("Enable Poll state");
                mle_class_mode_set(cur->id, MLE_CLASS_SLEEPY_END_DEVICE);
                mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, false);
                mac_data_poll_init(cur);
                mac_data_poll_init_protocol_poll(cur);
            }
            if (protocol_6lowpan_parent_address_synch(cur, false) != 0) {
                nwk_bootsrap_state_update(ARM_NWK_NWK_CONNECTION_DOWN, cur);
            }
        } else {
            cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
            nwk_6lowpan_bootstrap_ready(cur);
        }
    }
#else
#ifdef HAVE_RPL
    cur->nwk_bootstrap_state = ER_RPL_SCAN;
    nwk_6lowpan_rpl_router_result_check(cur);
#else
    cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
    nwk_6lowpan_bootstrap_ready(cur);
#endif
#endif
}
#ifdef PANA
void nwk_6lowpan_pana_key_pull(protocol_interface_info_entry_t *cur)
{
    //REG GP16 address
    if (pana_ping_notify_msg_tx(cur->mac_parameters->pan_id) == 0) {
        tr_warn("PING TX fail");
        cur->nwk_bootstrap_state = ER_PANA_PING;
        cur->bootsrap_state_machine_cnt = 2;
    }
}
#endif

#ifndef NO_MLE

#ifdef PANA
void nwk_6lowpan_bootsrap_pana_authentication_cb(bool processSuccesfully, protocol_interface_info_entry_t *cur)
{
    if (processSuccesfully) {
        bootsrap_next_state_kick(ER_PANA_AUTH_DONE, cur);
    } else {
        bootsrap_next_state_kick(ER_PANA_AUTH_ERROR, cur);

    }
}

static void nwk_6lowpan_bootsrap_pana_authentication_start(protocol_interface_info_entry_t *cur)
{
    uint8_t temp_coordinator_address[16];
    pana_tls_setup_s setup;
    sec_suite_t *suite = 0;
    tr_debug("Wake Pana by Bootsrap");
    protocol_6lowpan_interface_get_link_local_cordinator_address(cur, temp_coordinator_address);
    //Release old before copy new
    if (cur->pana_sec_info_temp == 0) {
        tr_debug("Allocate Pana auth Info");
        cur->pana_sec_info_temp = ns_dyn_mem_alloc(sizeof(auth_info_t));
    }
    if (cur->if_lowpan_security_params->pana_params) {
        setup.psk_key_id = cur->if_lowpan_security_params->pana_params->psk_key_id;

        switch (cur->if_lowpan_security_params->pana_params->nwk_chipher_mode) {
            case NET_TLS_PSK_CIPHER:        /**< Network Authentication support only PSK */
                setup.security_support = SEC_CIPHERSUITE_PSK;
                break;

            case NET_TLS_ECC_CIPHER:        /**< Network Authentication support only ECC */
                setup.security_support = SEC_CIPHERSUITE_ECC;
                break;
            case NET_TLS_PSK_AND_ECC_CIPHER:
                setup.security_support = SEC_CIPHERSUITE_PSK | SEC_CIPHERSUITE_ECC;
                break;
        }

        setup.pan_id = cur->mac_parameters->pan_id;
        suite = pana_client_init(cur->pana_sec_info_temp, temp_coordinator_address, &setup);

    }
    if (suite) {
        //SET address
        //SET CORD Address
        nd_router_t   *object = nd_get_pana_address();
        cur->nwk_bootstrap_state = ER_PANA_AUTH;
        cur->bootsrap_state_machine_cnt = 0;
        if (object) {
            icmp_nd_set_nd_def_router_address(suite->session_address, object);

            tr_debug("ND Router adr: %s", trace_ipv6(suite->session_address));

            //SET CORD ADDRESS
            if (memcmp(&suite->session_address[8], ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
                mac_helper_coordinator_address_set(cur, ADDR_802_15_4_SHORT, &(suite->session_address[14]));
            } else {
                suite->session_address[8] ^= 2;
                mac_helper_coordinator_address_set(cur, ADDR_802_15_4_LONG, &(suite->session_address[8]));
                suite->session_address[8] ^= 2;
            }
        } else {
            tr_debug("Use Mac Coordinator");
        }
        suite->session_port = UDP_PORT_PANA;
        suite->interface = cur;
    } else {
        cur->nwk_bootstrap_state = ER_PANA_AUTH_ERROR;
        cur->bootsrap_state_machine_cnt = 1;
    }
}
#endif

#endif

static void coordinator_black_list(protocol_interface_info_entry_t *cur)
{
    uint8_t coord_pan_address[10];
    addrtype_t cord_adr_type = mac_helper_coordinator_address_get(cur, coord_pan_address +2);

    if (cord_adr_type != ADDR_NONE) {
        uint16_t pana_id = mac_helper_panid_get(cur);
        common_write_16_bit(pana_id, coord_pan_address);
        if (cord_adr_type == ADDR_802_15_4_SHORT) {
            memset(coord_pan_address + 4, 0, 6);
        }

        pan_cordinator_blacklist_pan_set(&cur->pan_cordinator_black_list, coord_pan_address, 300);
    }
}

static void nwk_6lowpan_network_authentication_fail(protocol_interface_info_entry_t *cur)
{
    nwk_scan_params_t *scan_params =
            &cur->mac_parameters->nwk_scan_params;

    tr_warn("Pana Auhth er");

    scan_params->nwk_cur_active = mac_helper_free_pan_descriptions(scan_params->nwk_cur_active);
    //Black List coordinator
    coordinator_black_list(cur);

    nwk_bootsrap_state_update(ARM_NWK_AUHTENTICATION_FAIL, cur);
}


static void nwk_protocol_network_key_set_from_pana(protocol_interface_info_entry_t *cur)
{
    uint8_t *key_ptr = pana_key_get(cur->pana_sec_info_temp->network_key);

    if (key_ptr) {
        mac_helper_security_default_key_set(cur, (key_ptr + 16), cur->pana_sec_info_temp->key_id, MAC_KEY_ID_MODE_IDX);
        mle_service_security_set_security_key(cur->id, key_ptr, cur->pana_sec_info_temp->key_id, true);
        if (cur->nwk_wpan_nvm_api) {
            cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
        }
    }
}

uint8_t *protocol_6lowpan_mle_service_security_notify_cb(int8_t interface_id, mle_security_event_t event, uint8_t keyId)
{
    (void)keyId;
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!interface) {
        return NULL;
    }
    switch (event) {
        case MLE_SEC_MAX_FRAME_COUNTER_REACHED:

            break;

        case MLE_SEC_KEY_UPDATE_NOTIFY:
            //Call MAC update
            mac_helper_security_key_swap_next_to_default(interface);

            break;

        case MLE_SEC_UNKNOWN_KEY:
            break;
    }
    return NULL;
}

static void nwk_protocol_network_key_init_from_pana(protocol_interface_info_entry_t *cur)
{
    uint8_t *key_ptr = pana_key_get(cur->pana_sec_info_temp->network_key);

    if (key_ptr) {
        mac_helper_security_default_key_set(cur, (key_ptr + 16), cur->pana_sec_info_temp->key_id, MAC_KEY_ID_MODE_IDX);
        //mac_security_interface_link_frame_counter_reset(cur->id);
        mac_helper_default_security_level_set(cur, SEC_ENC_MIC32);
        mac_helper_default_security_key_id_mode_set(cur,MAC_KEY_ID_MODE_IDX);
        //Init MLE Frame counter and key's and security
        mle_service_security_init(cur->id, SEC_ENC_MIC32,cur->if_lowpan_security_params->mle_security_frame_counter,  NULL, protocol_6lowpan_mle_service_security_notify_cb);
        mle_service_security_set_security_key(cur->id, key_ptr, cur->pana_sec_info_temp->key_id, true);
        mle_service_security_set_frame_counter(cur->id, cur->if_lowpan_security_params->mle_security_frame_counter);
    }
}

static void nwk_6lowpan_network_authentication_done(protocol_interface_info_entry_t *cur)
{
    if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {
        mac_helper_free_scan_confirm(&cur->mac_parameters->nwk_scan_params);

        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {
            nwk_protocol_network_key_init_from_pana(cur);
        } else {
            tr_debug("SET NO security");
            mac_helper_default_security_level_set(cur, SEC_NONE);
        }

#ifndef NO_MLE
        if (protocol_6lowpan_parent_link_req(cur) != 0) {
            tr_debug("Link request start fail");
        }
#else
        pan_coordinator_blacklist_free(&cur->pan_cordinator_black_list);
        cur->nwk_bootstrap_state = ER_SCAN;
        nwk_6lowpan_router_scan_state(cur);
#endif
    } else {
        mac_data_poll_protocol_poll_mode_disable(cur);
        if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
            tr_debug("PULL kEY Done by Host");
            cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
            nwk_6lowpan_bootstrap_ready(cur);
        } else {
            tr_debug("PULL kEY Done by Router");
#ifdef PANA
            cur->nwk_bootstrap_state = ER_PANA_PING;
            nwk_6lowpan_pana_key_pull(cur);
#endif
        }

        nwk_protocol_network_key_set_from_pana(cur);
#ifndef NO_TLS
#endif

    }
}


bool protocol_6lowpan_bootsrap_link_set(protocol_interface_info_entry_t *interface, mlme_pan_descriptor_t *pan_descriptor, const uint8_t *beacon_payload, uint8_t beacon_length)
{
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));
    mac_helper_coordinator_address_set(interface, (addrtype_t)pan_descriptor->CoordAddrMode, pan_descriptor->CoordAddress);

    interface->mac_parameters->mac_channel = pan_descriptor->LogicalChannel;
    interface->mac_parameters->pan_id = pan_descriptor->CoordPANId;
    if (interface->nwk_wpan_nvm_api) {
        wpan_nvm_params_t *params = interface->nwk_wpan_nvm_api->nvm_params_get_cb(interface->nwk_wpan_nvm_api, pan_descriptor->CoordPANId);
        interface->if_lowpan_security_params->mle_security_frame_counter = params->mle_securit_counter;
        //SET MAC and MLE security frame counters
        mle_service_security_set_frame_counter(interface->id, params->mle_securit_counter);
        mac_helper_link_frame_counter_set(interface->id, params->mac_security_frame_counter);
    }

    start_req.PANId = pan_descriptor->CoordPANId;
    start_req.LogicalChannel = pan_descriptor->LogicalChannel;
    start_req.ChannelPage = 0;
    start_req.BeaconOrder = pan_descriptor->SuperframeSpec[0] >> 4;
    start_req.SuperframeOrder = pan_descriptor->SuperframeSpec[0] & 0x0f;
    //SET Beacon Payload
    uint8_t *b_ptr = mac_helper_beacon_payload_reallocate(interface, beacon_length);
    if (!b_ptr) {
        tr_error("Beacon Payload allocate Fail");
        bootsrap_next_state_kick(ER_BOOTSTRAP_SCAN_FAIL, interface);
        return false;
    }
    memcpy(b_ptr, beacon_payload, beacon_length);
    mac_helper_beacon_payload_register(interface);
    //Start and set pan-id
    interface->mac_api->mlme_req(interface->mac_api, MLME_START, &start_req);
    mac_helper_panid_set(interface, pan_descriptor->CoordPANId);

    return true;
}

bool protocol_6lowpan_bootsrap_start(protocol_interface_info_entry_t *interface)
{
    net_load_balance_internal_state_activate(interface, false);

    //SET allways RX ON Idle device by default
    mac_helper_pib_boolean_set(interface, macRxOnWhenIdle, true);
    interface->lowpan_info &=  ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;

    mac_data_poll_init(interface);
    mac_helper_mac16_address_set(interface, 0xffff);
    tr_debug("Mac Ready");
    interface->nwk_nd_re_scan_count = 2;

    if (interface->if_lowpan_security_params->nwk_security_mode == NET_SEC_MODE_PSK_LINK_SECURITY) {
        tr_debug("SET Security Mode");
        mac_helper_default_security_level_set(interface, interface->mac_parameters->mac_configured_sec_level);
        mac_helper_default_security_key_id_mode_set(interface,MAC_KEY_ID_MODE_IDX);
    }

    //Check first pana and then MLE and else start RS scan pahse
    if (interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {
#ifdef PANA
        nwk_6lowpan_bootsrap_pana_authentication_start(interface);
        tr_debug("Pana auth");
#else
        bootsrap_next_state_kick(ER_BOOTSTRAP_SCAN_FAIL, interface);
        return false;
#endif
    } else if (interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
        if (protocol_6lowpan_parent_link_req(interface) != 0) {
            bootsrap_next_state_kick(ER_BOOTSTRAP_SCAN_FAIL, interface);
            return false;
        }
    } else {
        bootsrap_next_state_kick(ER_SCAN, interface);
    }
    return true;
}


void protocol_6lowpan_mac_scan_confirm(int8_t if_id, const mlme_scan_conf_t* conf)
{
    nwk_pan_descriptor_t *result;
    nwk_pan_descriptor_t *best;
    protocol_interface_info_entry_t *interface = NULL;

    if (conf->ScanType != MAC_ACTIVE_SCAN) {
        return;
    }

    interface = protocol_stack_interface_info_get_by_id(if_id);
    if (!interface) {
        tr_debug("Mac scan confirm:Unknow Interface");
        return;
    }
    bool is_border_router = false;
    if (interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        is_border_router = true;
    }

    interface->mac_parameters->nwk_scan_params.active_scan_active = false;

    result = arm_net_get_scanned_nwk_list(if_id);
    if (!result || !conf->ResultListSize) {
        tr_debug("Mac scan confirm:No Beacons");
        if (is_border_router == false) {
            bootsrap_next_state_kick(ER_BOOTSTRAP_SCAN_FAIL, interface);
            return;
        }
    }

    //Analyze Best Result
    best = mac_helper_select_best_lqi(result);
    mac_helper_drop_selected_from_the_scanresult(&interface->mac_parameters->nwk_scan_params, best);

    bool link_start_ok = false;
    if (is_border_router == false) {
        link_start_ok = protocol_6lowpan_bootsrap_link_set(interface, best->pan_descriptor, best->beacon_payload, best->beacon_length);
    }

    mac_helper_free_scan_confirm(&interface->mac_parameters->nwk_scan_params);

    best = mac_helper_free_pan_descriptions(best);

    if (link_start_ok) {
        protocol_6lowpan_bootsrap_start(interface);
    }

    if (is_border_router == true) {
        if (interface->nwk_bootstrap_state == ER_WARM_ACTIVE_SCAN) {
            border_router_start(interface, true);
            interface->bootsrap_state_machine_cnt = 0;
            interface->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
        } else {
            border_router_start(interface, false);
        }

    }
}

void bootstrap_timer_handle(uint16_t ticks)
{
    (void)ticks;
    ns_list_foreach(protocol_interface_info_entry_t, cur, &protocol_interface_info_list) {
        if (cur->nwk_id == IF_6LoWPAN) {
            if (cur->nwk_bootstrap_state == ER_ACTIVE_SCAN || cur->nwk_bootstrap_state == ER_WARM_ACTIVE_SCAN) {
                // Retransmit Scan request
                bootsrap_next_state_kick(cur->nwk_bootstrap_state, cur);
                tr_error("Restart active scan");
            } else {
                // Retransmit Start request
                mlme_start_t start_req;
                memset(&start_req, 0, sizeof(mlme_start_t));
                start_req.PANId = cur->border_router_setup->mac_panid;
                start_req.LogicalChannel = cur->mac_parameters->mac_channel;
                start_req.ChannelPage = 0;
                start_req.BeaconOrder = 0x0f;
                start_req.SuperframeOrder = 0x0f;
                start_req.PANCoordinator = 1;
                if( cur->mac_api ){
                    cur->mac_api->mlme_req(cur->mac_api, MLME_START, (void*)&start_req);
                    tr_error("Restart MAC");
                }
            }
        }
    }
}

void protocol_6lowpan_bootstrap(protocol_interface_info_entry_t *cur)
{
    switch (cur->nwk_bootstrap_state) {
        case ER_ACTIVE_SCAN:
        case ER_WARM_ACTIVE_SCAN:
            tr_debug("Start Active Scan");
            cur->mac_parameters->nwk_scan_params.stack_chan_list = cur->mac_parameters->mac_channel_list;

            mlme_scan_t req;
            mac_create_scan_request(MAC_ACTIVE_SCAN, &cur->mac_parameters->mac_channel_list, cur->mac_parameters->nwk_scan_params.scan_duration, &req);
            if( cur->mac_api ){
                cur->scan_cb = protocol_6lowpan_mac_scan_confirm;
                cur->mac_parameters->nwk_scan_params.active_scan_active = true;
                if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                    protocol_timer_start(PROTOCOL_TIMER_BOOTSTRAP_TIM, bootstrap_timer_handle, BOOTSTRAP_SCAN_TIMEOUT);
                }
                cur->mac_api->mlme_req(cur->mac_api, MLME_SCAN, &req);
            }
            break;

        case ER_SCAN:
            //LED1_TOGGLE();
            nwk_6lowpan_router_scan_state(cur);
            break;

        case ER_PANA_AUTH_ERROR:
            nwk_6lowpan_network_authentication_fail(cur);
            break;

        case ER_PANA_AUTH_DONE:
            nwk_6lowpan_network_authentication_done(cur);
            break;

        case ER_BIND_COMP:
            nwk_6lowpan_nd_address_registartion_ready(cur);
            break;

#ifdef HAVE_RPL

        case ER_RPL_MC:
            nwk_6lowpan_rpl_router_discover(cur);
            break;

        case ER_RPL_SCAN:
            nwk_6lowpan_rpl_router_result_check(cur);
            break;
#endif
        case ER_BOOTSRAP_DONE:
            nwk_6lowpan_bootstrap_ready(cur);
            break;

        case ER_PARENT_SYNCH_LOST:
            tr_debug("-->Parent synch Lose");
            nwk_bootsrap_state_update(ARM_NWK_NWK_PARENT_POLL_FAIL, cur);
            break;

        case ER_BOOTSTRAP_CONNECTION_DOWN:
            nwk_bootsrap_state_update(ARM_NWK_NWK_CONNECTION_DOWN, cur);
            break;

        case ER_BOOTSTRAP_IP_ADDRESS_ALLOC_FAIL:
            nwk_bootsrap_state_update(ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL, cur);
            tr_info("-->idle");
            break;

        case ER_BOOTSTRAP_DAD_FAIL:
            nwk_bootsrap_state_update(ARM_NWK_DUPLICATE_ADDRESS_DETECTED, cur);
            break;

        case ER_BOOTSTRAP_SCAN_FAIL:
            tr_debug("Network Bootsrap Start Fail");
            nwk_bootsrap_state_update(ARM_NWK_NWK_SCAN_FAIL, cur);
            break;
#ifdef PANA
        case ER_PANA_PING:
            nwk_6lowpan_pana_key_pull(cur);
            break;
#endif
        case ER_MLE_LINK_REQ:
            //No need to do anything in this case
            break;
        default:
            tr_error("Unknow state %d", cur->nwk_bootstrap_state);

    }
}

void protocol_6lowpan_nd_borderrouter_connection_down(protocol_interface_info_entry_t *interface)
{
    /*if (rpl_object_poisons() == 0) ??? */ {
        mac_helper_mac16_address_set(interface, 0xffff);

        //TRIG Event for ND connection Down
        bootsrap_next_state_kick(ER_BOOTSTRAP_IP_ADDRESS_ALLOC_FAIL, interface);
    }
}

void protocol_6lowpan_bootstrap_re_start(protocol_interface_info_entry_t *interface)
{
    mac_helper_mac16_address_set(interface, 0xffff);
    arm_6lowpan_bootstrap_init(interface);
    tr_info("-->Bootsrap");
}

uint8_t *protocol_6lowpan_nd_border_router_address_get(nwk_interface_id nwk_id)
{
    nd_router_t   *object = nd_get_object_by_nwk_id(nwk_id);
    if (object) {
        return object->border_router;
    }
    return 0;
}

uint8_t protocol_6lowpan_rf_link_scalability_from_lqi(uint8_t lqi)
{
    uint8_t i = 16;
    if (lqi >= 240) {
        i = 1;
    } else {
        lqi /= 16;
        if (lqi) {
            i = (16 - lqi);
        }
    }
    return i;
}

int protocol_6lowpan_del_ll16(protocol_interface_info_entry_t *cur, uint16_t mac_short_address)
{
    uint8_t address[16];
    memcpy(address, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(address + 8, ADDR_SHORT_ADR_SUFFIC, 6);
    common_write_16_bit(mac_short_address, &address[14]);

    return addr_delete(cur, address);
}

int protocol_6lowpan_set_ll16(protocol_interface_info_entry_t *cur, uint16_t mac_short_address)
{
    if_address_entry_t *address_entry;
    uint8_t address[16];
    memcpy(address, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(address + 8, ADDR_SHORT_ADR_SUFFIC , 6);
    common_write_16_bit(mac_short_address, &address[14]);

    address_entry = addr_add(cur, address, 64, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0xffffffff, false);
    if (address_entry) {
        return 0;
    }
    return -1;
}

static void protocol_6lowpan_generate_link_reject(protocol_interface_info_entry_t *cur, const mlme_comm_status_t *status)
{
    uint8_t address[16];
    memcpy(address, ADDR_LINK_LOCAL_PREFIX, 8);
    if (status->SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
        memcpy(address + 8, ADDR_SHORT_ADR_SUFFIC , 6);
        memcpy(address + 14,status->SrcAddr, 2);
    } else {
        memcpy(address + 8,status->SrcAddr, 8);
        address[8] ^= 2;
    }
    if (mac_helper_default_security_level_get(cur)) {
        tr_debug("Drop link by asymmetric security");
        mle_service_reject_message_build(cur->id, address, false);
        return;
    }

}

static void lowpan_comm_status_indication_cb(int8_t if_id, const mlme_comm_status_t* status)
{
#ifndef NO_MLE
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(if_id);
    if (!cur) {
        return;
    }

    switch (status->status) {
        case MLME_UNSUPPORTED_SECURITY:
        case MLME_UNAVAILABLE_KEY:
            /* Generate MLE Link Reject to destination */
            if (status->DstAddrMode == MAC_ADDR_MODE_16_BIT && status->DstAddr[0] == 0xff && status->DstAddr[1] == 0xff) {
                return; //Drop brodcast security failure
            }

            if (!cur->mle_link_reject_tokens) {
                return;
            }
            cur->mle_link_reject_tokens--;
            protocol_6lowpan_generate_link_reject(cur, status);

            break;
        case MLME_DATA_POLL_NOTIFICATION:
            mle_refresh_entry_timeout(if_id, status->SrcAddr, (addrtype_t)status->SrcAddrMode, false);
            break;
        default:
            break;
    }
#endif
}

bool lowpan_neighbour_data_clean(int8_t interface_id, const uint8_t *link_local_address)
{
    bool return_value = false;
#ifndef NO_MLE
    mle_neigh_table_entry_t * neigh_entry = mle_class_get_entry_by_ll64(interface_id, 0, link_local_address, false, NULL);
    if (neigh_entry) {
        //Remove entry
        if (neigh_entry->priorityFlag) {
            return_value = true;
        } else if (neigh_entry->second_priority_flag) {
            return_value = true;
        }
        mle_class_remove_entry(interface_id, neigh_entry);
    }
#endif
    return return_value;
}

void protocol_6lowpan_mle_timer(uint16_t ticks_update)
{
#ifndef NO_MLE
    if (mle_6lowpan_data) {
        /* Three request in burst and after that one link request per second */
        mle_6lowpan_data->link_req_token_bucket += ticks_update;
        if (mle_6lowpan_data->link_req_token_bucket > MLE_LINK_REQ_TOKEN_BUCKET_SIZE) {
            mle_6lowpan_data->link_req_token_bucket = MLE_LINK_REQ_TOKEN_BUCKET_SIZE;
        }
    }
#endif
}

#endif
