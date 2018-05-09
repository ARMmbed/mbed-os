/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "string.h"
#include "ns_trace.h"
#include "ns_list.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "Core/include/socket.h"
#include "mac_api.h"
#include "mac_mcps.h"
#include "mac_common_defines.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "lowpan_adaptation_interface.h"
#include "MLE/mle.h"
#ifdef HAVE_RPL
#include "RPL/rpl_data.h"
#endif

#define TRACE_GROUP "6lAd"

// #define EXTRA_DEBUG_EXTRA
#ifdef EXTRA_DEBUG_EXTRA
#define tr_debug_extra(...) tr_debug(__VA_ARGS__)
#else
#define tr_debug_extra(...)
#endif

typedef struct {
    uint16_t tag;   /*!< Fragmentation datagram TAG ID */
    uint16_t size;  /*!< Datagram Total Size (uncompressed) */
    uint16_t orig_size; /*!< Datagram Original Size (compressed) */
    uint16_t frag_max;  /*!< Maximum fragment size (MAC payload) */
    uint16_t offset; /*!< Data offset from datagram start */
    int16_t pattern; /*!< Size of compressed LoWPAN headers */
    uint16_t unfrag_ptr; /*!< Offset within buf of headers that precede the FRAG header */
    uint16_t frag_len;
    uint8_t unfrag_len; /*!< Length of headers that precede the FRAG header */
    bool fragmented_data:1;
    bool first_fragment:1;
    bool indirect_data:1;
    bool indirect_data_cached:1; /* Data cached for delayed transmission as mac request is already active */
    buffer_t *buf;
    uint8_t *fragmenter_buf;
    ns_list_link_t      link; /*!< List link entry */
} fragmenter_tx_entry_t;


typedef NS_LIST_HEAD(fragmenter_tx_entry_t, link) fragmenter_tx_list_t;

typedef struct {
    int8_t interface_id;
    uint16_t local_frag_tag;
    uint8_t msduHandle;
    fragmenter_tx_list_t indirect_tx_queue;
    uint8_t *fragment_indirect_tx_buffer; //Used for write fragmentation header
    uint16_t mtu_size;
    fragmenter_tx_entry_t active_unicast_tx_buf; //Current active direct unicast tx process
    fragmenter_tx_entry_t active_broadcast_tx_buf; //Current active direct broadcast tx process
    buffer_list_t directTxQueue; //Waiting free tx process
    uint16_t indirect_big_packet_threshold;
    uint16_t max_indirect_big_packets_total;
    uint16_t max_indirect_small_packets_per_child;
    bool fragmenter_active; /*!< Fragmenter state */
    ns_list_link_t      link; /*!< List link entry */
} fragmenter_interface_t;

static NS_LIST_DEFINE(fragmenter_interface_list, fragmenter_interface_t, link);

/* Adaptation interface local functions */
static fragmenter_interface_t *lowpan_adaptation_interface_discover(int8_t interfaceId);

/* Interface direct message pending queue functions */
static void lowpan_adaptation_tx_queue_write(fragmenter_interface_t *interface_ptr , buffer_t *buf);
static buffer_t * lowpan_adaptation_tx_queue_read(fragmenter_interface_t *interface_ptr, protocol_interface_info_entry_t *cur);

/* Data direction and message length validation */
static bool lowpan_adaptation_indirect_data_request(mle_neigh_table_entry_t *mle_entry);
static bool lowpan_adaptation_request_longer_than_mtu(protocol_interface_info_entry_t *cur, buffer_t *buf);

/* Common data tx request process functions */
static void lowpan_active_buffer_state_reset(fragmenter_tx_entry_t *tx_buffer);
static uint8_t lowpan_data_request_unique_handle_get(fragmenter_interface_t *interface_ptr);
static fragmenter_tx_entry_t *lowpan_indirect_entry_allocate(uint16_t fragment_buffer_size);
static fragmenter_tx_entry_t * lowpan_adaptation_tx_process_init(fragmenter_interface_t *interface_ptr, bool indirect, bool fragmented, bool is_unicast);
static void lowpan_adaptation_data_request_primitiv_set(const buffer_t *buf, mcps_data_req_t *dataReq, protocol_interface_info_entry_t *cur);
static void lowpan_data_request_to_mac(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_tx_entry_t *tx_ptr);

/* Tx confirmation local functions */
static bool lowpan_active_tx_handle_verify(uint8_t handle, buffer_t *buf);
static fragmenter_tx_entry_t * lowpan_indirect_tx_handle_verify(uint8_t handle, fragmenter_tx_list_t *indirect_tx_queue);
static void lowpan_adaptation_data_process_clean(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr, uint8_t socket_event);
static uint8_t map_mlme_status_to_socket_event(uint8_t mlme_status);
static bool lowpan_adaptation_tx_process_ready(fragmenter_tx_entry_t *tx_ptr);

/* Fragmentation local functions */
static int8_t lowpan_message_fragmentation_init(buffer_t *buf, fragmenter_tx_entry_t *frag_entry, protocol_interface_info_entry_t *cur);
static bool lowpan_message_fragmentation_message_write(const fragmenter_tx_entry_t *frag_entry, mcps_data_req_t *dataReq);
static void lowpan_adaptation_indirect_queue_free_message(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr);

static fragmenter_tx_entry_t* lowpan_adaptation_indirect_mac_data_request_active(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr);

//Discover
static fragmenter_interface_t *lowpan_adaptation_interface_discover(int8_t interfaceId)
{

    ns_list_foreach(fragmenter_interface_t, interface_ptr, &fragmenter_interface_list) {
        if (interfaceId == interface_ptr->interface_id) {
            return interface_ptr;
        }
    }

    return NULL;
}


static void lowpan_adaptation_tx_queue_write(fragmenter_interface_t *interface_ptr , buffer_t *buf)
{
    buffer_t *lower_priority_buf = NULL;

    ns_list_foreach(buffer_t, cur, &interface_ptr->directTxQueue) {
        if (cur->priority < buf->priority) {
            lower_priority_buf = cur;
            break;
        }
    }

    if (lower_priority_buf) {
        ns_list_add_before(&interface_ptr->directTxQueue, lower_priority_buf, buf);
    } else {
        ns_list_add_to_end(&interface_ptr->directTxQueue, buf);
    }
}

static buffer_t * lowpan_adaptation_tx_queue_read(fragmenter_interface_t *interface_ptr, protocol_interface_info_entry_t *cur)
{
    /* Currently this function is called only when data confirm is received for previously sent packet.
     * Data confirm has freed the corresponding "active buffer" and this function will look for new buffer to be set as active buffer.
     */
    ns_list_foreach_safe(buffer_t, buf, &interface_ptr->directTxQueue) {
        bool fragmented_needed = lowpan_adaptation_request_longer_than_mtu(cur, buf);
        //Check that we not trig second active fragmentation process
        if (fragmented_needed && interface_ptr->fragmenter_active) {
            tr_debug("Do not trig Second active fragmentation");
        } else if ((buf->link_specific.ieee802_15_4.requestAck && !interface_ptr->active_unicast_tx_buf.buf)
                || (!buf->link_specific.ieee802_15_4.requestAck && !interface_ptr->active_broadcast_tx_buf.buf)) {
            ns_list_remove(&interface_ptr->directTxQueue, buf);
            return buf;
        }
    }
    return NULL;
}

//fragmentation needed

static bool lowpan_adaptation_request_longer_than_mtu(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    uint_fast8_t overhead = mac_helper_frame_overhead(cur, buf);


    if (buffer_data_length(buf) > (int16_t)mac_helper_max_payload_size(cur, overhead)) {
        return true;
    } else {
        return false;
    }
}

static bool lowpan_adaptation_indirect_data_request(mle_neigh_table_entry_t *mle_entry)
{
    if (mle_entry && !(mle_entry->mode & MLE_RX_ON_IDLE)) {
        return true;
    }
    return false;
}


static void lowpan_active_buffer_state_reset(fragmenter_tx_entry_t *tx_buffer)
{
    if (tx_buffer->buf) {
        buffer_free(tx_buffer->buf);
        tx_buffer->buf = NULL;
    }
    tx_buffer->fragmented_data = false;
    tx_buffer->first_fragment = true;
}

static bool lowpan_active_tx_handle_verify(uint8_t handle, buffer_t *buf)
{

    if (buf && buf->seq == handle) {
        return true;
    }


    return false;
}



static fragmenter_tx_entry_t * lowpan_indirect_tx_handle_verify(uint8_t handle, fragmenter_tx_list_t *indirect_tx_queue)
{
    ns_list_foreach(fragmenter_tx_entry_t, entry, indirect_tx_queue) {
        if (entry->buf->seq == handle) {
            return entry;
        }
    }
    return NULL;
}



static uint8_t lowpan_data_request_unique_handle_get(fragmenter_interface_t *interface_ptr)
{
    bool valid_info = false;
    uint8_t handle;
    while(!valid_info) {
        handle = interface_ptr->msduHandle++;
        if (!lowpan_active_tx_handle_verify(handle,interface_ptr->active_unicast_tx_buf.buf)
                && !lowpan_active_tx_handle_verify(handle,interface_ptr->active_broadcast_tx_buf.buf)
                && !lowpan_indirect_tx_handle_verify(handle, &interface_ptr->indirect_tx_queue)) {
            valid_info = true;
        }
    }
    return handle;

}

static void lowpan_indirect_entry_free(fragmenter_tx_list_t *list , fragmenter_tx_entry_t *entry)
{
    ns_list_remove(list, entry);
    if (entry->buf) {
        buffer_free(entry->buf);
    }
    ns_dyn_mem_free(entry->fragmenter_buf);
    ns_dyn_mem_free(entry);
}

static void lowpan_indirect_queue_free(fragmenter_tx_list_t *list)
{
    while(!ns_list_is_empty(list)) {
        fragmenter_tx_entry_t *entry = ns_list_get_first(list);
        lowpan_indirect_entry_free(list, entry);
    }
}


int8_t lowpan_adaptation_interface_init(int8_t interface_id, uint16_t mac_mtu_size)
{
    if (mac_mtu_size == 0) {
        return -2;
    }
    //Remove old interface
    lowpan_adaptation_interface_free(interface_id);

    //Allocate new
    fragmenter_interface_t *interface_ptr = ns_dyn_mem_alloc(sizeof(fragmenter_interface_t));
    uint8_t *tx_buffer = ns_dyn_mem_alloc(mac_mtu_size);
    if (!interface_ptr  || !tx_buffer) {
        ns_dyn_mem_free(interface_ptr);
        ns_dyn_mem_free(tx_buffer);
        return -1;
    }

    memset(interface_ptr, 0 ,sizeof(fragmenter_interface_t));
    interface_ptr->interface_id = interface_id;
    interface_ptr->fragment_indirect_tx_buffer = tx_buffer;
    interface_ptr->mtu_size = mac_mtu_size;
    interface_ptr->msduHandle = randLIB_get_8bit();
    interface_ptr->local_frag_tag = randLIB_get_16bit();

    ns_list_init(&interface_ptr->indirect_tx_queue);
    ns_list_init(&interface_ptr->directTxQueue);

    ns_list_add_to_end(&fragmenter_interface_list, interface_ptr);

    return 0;
}

int8_t lowpan_adaptation_interface_free(int8_t interface_id)
{
    //Discover
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    ns_list_remove(&fragmenter_interface_list, interface_ptr);
    //free active tx process
    lowpan_active_buffer_state_reset(&interface_ptr->active_unicast_tx_buf);
    lowpan_active_buffer_state_reset(&interface_ptr->active_broadcast_tx_buf);

    //Free Indirect entry
    lowpan_indirect_queue_free(&interface_ptr->indirect_tx_queue);

    buffer_free_list(&interface_ptr->directTxQueue);

    //Free Dynamic allocated entries
    ns_dyn_mem_free(interface_ptr->fragment_indirect_tx_buffer);
    ns_dyn_mem_free(interface_ptr);

    return 0;
}


int8_t lowpan_adaptation_interface_reset(int8_t interface_id)
{
    //Discover
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    //free active tx process
    lowpan_active_buffer_state_reset(&interface_ptr->active_unicast_tx_buf);
    lowpan_active_buffer_state_reset(&interface_ptr->active_broadcast_tx_buf);
    //Clean fragmented message flag
    interface_ptr->fragmenter_active = false;

    //Free Indirect entry
    lowpan_indirect_queue_free(&interface_ptr->indirect_tx_queue);

    buffer_free_list(&interface_ptr->directTxQueue);

    return 0;
}


static fragmenter_tx_entry_t *lowpan_indirect_entry_allocate(uint16_t fragment_buffer_size)
{
    fragmenter_tx_entry_t *indirec_entry = ns_dyn_mem_temporary_alloc(sizeof(fragmenter_tx_entry_t));
    if (!indirec_entry) {
        return NULL;
    }

    if (fragment_buffer_size) {
        indirec_entry->fragmenter_buf = ns_dyn_mem_temporary_alloc(fragment_buffer_size);
        if (!indirec_entry->fragmenter_buf) {
            ns_dyn_mem_free(indirec_entry);
            return NULL;
        }
    } else {
        indirec_entry->fragmenter_buf = NULL;
    }


    indirec_entry->buf = NULL;
    indirec_entry->fragmented_data = false;
    indirec_entry->first_fragment = true;
    indirec_entry->indirect_data_cached = false;

    return indirec_entry;
}

static int8_t lowpan_message_fragmentation_init(buffer_t *buf, fragmenter_tx_entry_t *frag_entry, protocol_interface_info_entry_t *cur)
{
    uint8_t *ptr;
    uint16_t uncompressed_size;

    /* Look for pre-fragmentation headers - strip off and store away */
    frag_entry->unfrag_ptr = buf->buf_ptr;
    frag_entry->unfrag_len = 0;
    ptr = buffer_data_pointer(buf);

    if ((ptr[0] & LOWPAN_MESH_MASK) == LOWPAN_MESH) {
        uint_fast8_t size = mesh_header_len_from_type_byte(ptr[0]);
        ptr += size;
        buf->buf_ptr += size;
    }

    if (ptr[0] == LOWPAN_DISPATCH_BC0) {
        ptr += 2;
        buf->buf_ptr += 2;
    }

    frag_entry->unfrag_len = buf->buf_ptr - frag_entry->unfrag_ptr;

    frag_entry->pattern = iphc_header_scan(buf, &uncompressed_size);
    frag_entry->size = buffer_data_length(buf);
    frag_entry->orig_size = frag_entry->size;
    frag_entry->size += (uncompressed_size - frag_entry->pattern);

    uint_fast8_t overhead = mac_helper_frame_overhead(cur, buf);
    frag_entry->frag_max = mac_helper_max_payload_size(cur, overhead);


    /* RFC 4944 says MTU and hence maximum size here is 1280, but that's
     * arbitrary, and some have argued that 6LoWPAN should have a larger
     * MTU, to avoid the need for IP fragmentation. So we don't enforce
     * that, leaving MTU decisions to upper layer config, and only look
     * for the "real" MTU from the FRAG header format, which would allow up
     * to 0x7FF (2047).
     */
    if (frag_entry->size > LOWPAN_HARD_MTU_LIMIT) {
        tr_error("Packet too big");
        return -1;
    }

    frag_entry->offset = uncompressed_size / 8;
    frag_entry->frag_len = frag_entry->pattern;
    if (frag_entry->unfrag_len + 4 + frag_entry->frag_len > frag_entry->frag_max) {
        tr_error("Too long 6LoWPAN header for fragment");
        return -1;
    }

    /* Now, frag_len is compressed payload bytes (just IPHC headers), and
     * frag_ptr->offset is uncompressed payload 8-octet units (just uncompressed
     * IPHC headers). Add post-IPHC payload to bring total compressed size up
     * to maximum fragment size.
     */
    while (frag_entry->unfrag_len + 4 + frag_entry->frag_len + 8 <= frag_entry->frag_max) {
        frag_entry->offset++;
        frag_entry->frag_len += 8;
    }
    frag_entry->fragmented_data = true;

    return 0;

}

/**
 * Return true when there is more fragmented packet for this message
 */
static bool lowpan_message_fragmentation_message_write(const fragmenter_tx_entry_t *frag_entry, mcps_data_req_t *dataReq)
{
    uint8_t *ptr = dataReq->msdu;
    if (frag_entry->unfrag_len) {
        memcpy(ptr, frag_entry->buf->buf  + frag_entry->unfrag_ptr, frag_entry->unfrag_len);
        ptr += frag_entry->unfrag_len;
    }
    if (frag_entry->first_fragment) {
        ptr = common_write_16_bit(((uint16_t) LOWPAN_FRAG1 << 8) | frag_entry->size, ptr);
        ptr = common_write_16_bit(frag_entry->tag, ptr);
    } else {
        ptr = common_write_16_bit(((uint16_t) LOWPAN_FRAGN << 8) | frag_entry->size, ptr);
        ptr = common_write_16_bit(frag_entry->tag, ptr);
        *ptr++ = frag_entry->offset;
    }
    memcpy(ptr, buffer_data_pointer(frag_entry->buf), frag_entry->frag_len);
    ptr += frag_entry->frag_len;
    dataReq->msduLength = ptr - dataReq->msdu;
    return frag_entry->offset * 8 + frag_entry->frag_len < frag_entry->size;
}

static fragmenter_tx_entry_t * lowpan_adaptation_tx_process_init(fragmenter_interface_t *interface_ptr, bool indirect, bool fragmented, bool is_unicast)
{
    fragmenter_tx_entry_t *tx_entry;
    if (!indirect) {
        if (is_unicast) {
            tx_entry = &interface_ptr->active_unicast_tx_buf;
        } else {
            tx_entry = &interface_ptr->active_broadcast_tx_buf;
        }
        tx_entry->fragmenter_buf = interface_ptr->fragment_indirect_tx_buffer;
    } else {
        if (fragmented) {
            tx_entry = lowpan_indirect_entry_allocate(interface_ptr->mtu_size);
        } else {
            tx_entry = lowpan_indirect_entry_allocate(0);
        }
    }

    if (!tx_entry) {
        return NULL;
    }

    lowpan_active_buffer_state_reset(tx_entry);

    tx_entry->indirect_data = indirect;

    return tx_entry;
}

buffer_t * lowpan_adaptation_data_process_tx_preprocess(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    //Validate is link known and set indirect, datareq and security key id mode
    if (buf->dst_sa.addr_type == ADDR_NONE) {
        goto tx_error_handler;
    }

    mle_neigh_table_entry_t *mle_entry = NULL;
    /* If MLE is enabled, we will talk if we have an MLE association */
    if (buf->dst_sa.addr_type == ADDR_802_15_4_LONG ) {
        mle_entry = mle_class_get_by_link_address(cur->id, buf->dst_sa.address + 2, buf->dst_sa.addr_type);

    } else if(buf->dst_sa.addr_type == ADDR_802_15_4_SHORT && (common_read_16_bit(buf->dst_sa.address + 2)) != 0xffff) {
        mle_entry = mle_class_get_by_link_address(cur->id, buf->dst_sa.address + 2, buf->dst_sa.addr_type);
    }

    //Validate neighbour
    if (!buf->options.ll_security_bypass_tx && mle_entry) {

        if (mle_entry->handshakeReady ||  mle_entry->thread_commission) {

        } else {
            //tr_warn("Drop TX to unassociated %s", trace_sockaddr(&buf->dst_sa, true));
            goto tx_error_handler;
        }
    }

    //Check indirect


    if (addr_check_broadcast(buf->dst_sa.address, buf->dst_sa.addr_type) == eOK) {
        buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;
        buf->dst_sa.address[2] = 0xff;
        buf->dst_sa.address[3] = 0xff;
        buf->link_specific.ieee802_15_4.indirectTxProcess = false;
        buf->link_specific.ieee802_15_4.requestAck = false;
    } else {
        buf->link_specific.ieee802_15_4.requestAck = true;
        buf->link_specific.ieee802_15_4.indirectTxProcess = lowpan_adaptation_indirect_data_request(mle_entry);
    }

    if (buf->link_specific.ieee802_15_4.key_id_mode != B_SECURITY_KEY_ID_2) {

        if (!buf->link_specific.ieee802_15_4.requestAck ) {
            buf->link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_MODE_DEFAULT;
        } else if (mle_entry && !mle_entry->thread_commission) {
            buf->link_specific.ieee802_15_4.key_id_mode  = B_SECURITY_KEY_ID_MODE_DEFAULT;
        } else {
            buf->link_specific.ieee802_15_4.key_id_mode  = B_SECURITY_KEY_ID_IMPLICIT;
        }
    }

    return buf;

    tx_error_handler:
    socket_tx_buffer_event_and_free(buf, SOCKET_TX_FAIL);
    return NULL;

}

static void lowpan_adaptation_data_request_primitiv_set(const buffer_t *buf, mcps_data_req_t *dataReq, protocol_interface_info_entry_t *cur)
{
    memset(dataReq, 0, sizeof(mcps_data_req_t));

    //Check do we need fragmentation

    dataReq->InDirectTx = buf->link_specific.ieee802_15_4.indirectTxProcess;
    dataReq->TxAckReq = buf->link_specific.ieee802_15_4.requestAck;
    dataReq->SrcAddrMode = buf->src_sa.addr_type;
    dataReq->DstAddrMode = buf->dst_sa.addr_type;
    memcpy(dataReq->DstAddr, &buf->dst_sa.address[2], 8);

    if (buf->link_specific.ieee802_15_4.useDefaultPanId) {
        dataReq->DstPANId = mac_helper_panid_get(cur);
    } else {
        dataReq->DstPANId = buf->link_specific.ieee802_15_4.dstPanId;
    }

    //Allocate message msdu handle
    dataReq->msduHandle = buf->seq;

    //Set Messages
    if (!buf->options.ll_security_bypass_tx) {
        dataReq->Key.SecurityLevel = mac_helper_default_security_level_get(cur);
        if (dataReq->Key.SecurityLevel) {
            switch (buf->link_specific.ieee802_15_4.key_id_mode) {
                case B_SECURITY_KEY_ID_MODE_DEFAULT:
                    dataReq->Key.KeyIndex = mac_helper_default_key_index_get(cur);
                    dataReq->Key.KeyIdMode = mac_helper_default_security_key_id_mode_get(cur);
                    break;
                case B_SECURITY_KEY_ID_IMPLICIT:
                    dataReq->Key.KeyIdMode = MAC_KEY_ID_MODE_IMPLICIT;
                    break;

                case B_SECURITY_KEY_ID_2:
                    dataReq->Key.KeyIndex = 0xff;
                    dataReq->Key.KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
                    common_write_32_bit(0xffffffff, dataReq->Key.Keysource);
                    break;
            }
        }
    }
}

static bool lowpan_adaptation_indirect_cache_sanity_check(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr)
{
    fragmenter_tx_entry_t *active_tx_entry;
    ns_list_foreach(fragmenter_tx_entry_t, fragmenter_tx_entry, &interface_ptr->indirect_tx_queue) {
        if (fragmenter_tx_entry->indirect_data_cached == false) {
            // active entry, jump to next one
            continue;
        }

        // cached entry found, check if it has pending data reguest
        active_tx_entry = lowpan_adaptation_indirect_mac_data_request_active(interface_ptr, fragmenter_tx_entry);

        if (active_tx_entry == NULL) {
            // entry is in cache and is not sent to mac => trigger this
            tr_debug_extra("sanity check, push seq %d to addr %s", fragmenter_tx_entry->buf->seq, trace_ipv6(fragmenter_tx_entry->buf->dst_sa.address));
            fragmenter_tx_entry->indirect_data_cached = false;
            lowpan_data_request_to_mac(cur, fragmenter_tx_entry->buf, fragmenter_tx_entry);
            return true;
        }
    }

    return false;
}

static bool lowpan_adaptation_indirect_cache_trigger(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
{
    tr_debug_extra("lowpan_adaptation_indirect_cache_trigger()");

    if (ns_list_count(&interface_ptr->indirect_tx_queue) == 0) {
        return false;
    }

    /* Trigger first cached entry */
    ns_list_foreach(fragmenter_tx_entry_t, fragmenter_tx_entry, &interface_ptr->indirect_tx_queue) {
        if (fragmenter_tx_entry->indirect_data_cached) {
            if (addr_ipv6_equal(tx_ptr->buf->dst_sa.address, fragmenter_tx_entry->buf->dst_sa.address)) {
                tr_debug_extra("pushing seq %d to addr %s", fragmenter_tx_entry->buf->seq, trace_ipv6(fragmenter_tx_entry->buf->dst_sa.address));
                fragmenter_tx_entry->indirect_data_cached = false;
                lowpan_data_request_to_mac(cur, fragmenter_tx_entry->buf, fragmenter_tx_entry);
                return true;
            }
        }
    }

    /* Sanity check, If nothing can be triggered from own address, check cache queue */
    return lowpan_adaptation_indirect_cache_sanity_check(cur, interface_ptr);
}

static fragmenter_tx_entry_t* lowpan_adaptation_indirect_mac_data_request_active(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
{
    ns_list_foreach(fragmenter_tx_entry_t, fragmenter_tx_entry, &interface_ptr->indirect_tx_queue) {
        if (fragmenter_tx_entry->indirect_data_cached == false) {
            if (addr_ipv6_equal(tx_ptr->buf->dst_sa.address, fragmenter_tx_entry->buf->dst_sa.address)) {
                tr_debug_extra("active seq: %d", fragmenter_tx_entry->buf->seq);
                return fragmenter_tx_entry;
            }
        }
    }
    return NULL;
}

static fragmenter_tx_entry_t* lowpan_adaptation_indirect_first_cached_request_get(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
{
    ns_list_foreach(fragmenter_tx_entry_t, fragmenter_tx_entry, &interface_ptr->indirect_tx_queue) {
        if (fragmenter_tx_entry->indirect_data_cached == true) {
            if (addr_ipv6_equal(tx_ptr->buf->dst_sa.address, fragmenter_tx_entry->buf->dst_sa.address)) {
                tr_debug_extra("first cached seq: %d", fragmenter_tx_entry->buf->seq);
                return fragmenter_tx_entry;
            }
        }
    }
    return NULL;
}

static void lowpan_adaptation_make_room_for_small_packet(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, mle_neigh_table_entry_t *neighbour_to_count)
{
    if (interface_ptr->max_indirect_small_packets_per_child == 0) {
        return;
    }

    uint_fast16_t count = 0;

    ns_list_foreach_reverse_safe(fragmenter_tx_entry_t, tx_entry, &interface_ptr->indirect_tx_queue) {
        mle_neigh_table_entry_t *tx_neighbour = mle_class_get_by_link_address(cur->id, tx_entry->buf->dst_sa.address + 2, tx_entry->buf->dst_sa.addr_type);
        if (tx_neighbour == neighbour_to_count && buffer_data_length(tx_entry->buf) <= interface_ptr->indirect_big_packet_threshold) {
            if (++count >= interface_ptr->max_indirect_small_packets_per_child) {
                lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, tx_entry);
            }
        }
    }
}

static void lowpan_adaptation_make_room_for_big_packet(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr)
{
    if (interface_ptr->max_indirect_big_packets_total == 0) {
        return;
    }

    uint_fast16_t count = 0;

    ns_list_foreach_reverse_safe(fragmenter_tx_entry_t, tx_entry, &interface_ptr->indirect_tx_queue) {
        if (buffer_data_length(tx_entry->buf) > interface_ptr->indirect_big_packet_threshold) {
            if (++count >= interface_ptr->max_indirect_big_packets_total) {
                tr_debug_extra("free seq: %d", tx_entry->buf->seq);
                lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, tx_entry);
            }
        }
    }
}

static void lowpan_data_request_to_mac(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_tx_entry_t *tx_ptr)
{
    mcps_data_req_t dataReq;

    lowpan_adaptation_data_request_primitiv_set(buf, &dataReq, cur);
    if (tx_ptr->fragmented_data) {
        dataReq.msdu = tx_ptr->fragmenter_buf;
        //Call fragmenter
        bool more_fragments = lowpan_message_fragmentation_message_write(tx_ptr, &dataReq);
        if (dataReq.InDirectTx) {
            dataReq.PendingBit |= more_fragments;
        }
    } else {
        dataReq.msduLength = buffer_data_length(buf);
        dataReq.msdu = buffer_data_pointer(buf);
    }
    if (buf->link_specific.ieee802_15_4.rf_channel_switch) {
        //Switch channel if selected channel is different
        if (cur->mac_parameters->mac_channel != buf->link_specific.ieee802_15_4.selected_channel) {
            uint8_t channel = cur->mac_parameters->mac_channel;
            mac_helper_mac_channel_set(cur, buf->link_specific.ieee802_15_4.selected_channel);
            buf->link_specific.ieee802_15_4.selected_channel = channel;
        } else {
            buf->link_specific.ieee802_15_4.rf_channel_switch = false;
        }
    }

    cur->mac_api->mcps_data_req(cur->mac_api, &dataReq);
}

int8_t lowpan_adaptation_interface_tx(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    if (!buf) {
        return -1;
    }

    if (!cur || !cur->mac_api || !cur->mac_api->mcps_data_req) {
        goto tx_error_handler;
    }

    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr) {
        goto tx_error_handler;
    }

    //Check packet size
    bool fragmented_needed = lowpan_adaptation_request_longer_than_mtu(cur, buf);
    bool is_unicast = buf->link_specific.ieee802_15_4.requestAck;
    bool indirect = buf->link_specific.ieee802_15_4.indirectTxProcess;
    if (!indirect) {
        if (((is_unicast && interface_ptr->active_unicast_tx_buf.buf) || (!is_unicast && interface_ptr->active_broadcast_tx_buf.buf)) || (fragmented_needed && interface_ptr->fragmenter_active)) {
            lowpan_adaptation_tx_queue_write(interface_ptr, buf);
            return 0; //Return here
        }
    }

    //Allocate Handle
    buf->seq = lowpan_data_request_unique_handle_get(interface_ptr);

    if (buf->options.ll_sec_bypass_frag_deny && fragmented_needed) {
        // force security for fragmented packets
        buf->options.ll_security_bypass_tx = false;
    }

    fragmenter_tx_entry_t *tx_ptr = lowpan_adaptation_tx_process_init(interface_ptr, indirect, fragmented_needed, is_unicast);
    if (!tx_ptr) {
        goto tx_error_handler;
    }

    tx_ptr->buf = buf;

    if (fragmented_needed) {
        //Fragmentation init
        if (lowpan_message_fragmentation_init(buf, tx_ptr, cur) ) {
            tr_error("Fragment init fail");
            if (indirect) {
                ns_dyn_mem_free(tx_ptr->fragmenter_buf);
                ns_dyn_mem_free(tx_ptr);
            }
            goto tx_error_handler;
        }

        tx_ptr->tag = interface_ptr->local_frag_tag++;
        if (!indirect) {
            interface_ptr->fragmenter_active = true;
        }
    }

    if (indirect) {
        //Add to indirectQUue
        fragmenter_tx_entry_t *tx_ptr_cached;
        mle_neigh_table_entry_t *mle_entry = mle_class_get_by_link_address(cur->id, buf->dst_sa.address + 2, buf->dst_sa.addr_type);
        if (mle_entry) {
            buf->link_specific.ieee802_15_4.indirectTTL = (uint32_t) mle_entry->timeout_rx * MLE_TIMER_TICKS_MS;
        } else {
            buf->link_specific.ieee802_15_4.indirectTTL = cur->mac_parameters->mac_in_direct_entry_timeout;
        }

        tr_debug_extra("indirect seq: %d, addr=%s", tx_ptr->buf->seq, trace_ipv6(buf->dst_sa.address));

        // Make room for new message if needed */
        if (buffer_data_length(buf) <= interface_ptr->indirect_big_packet_threshold) {
            lowpan_adaptation_make_room_for_small_packet(cur, interface_ptr, mle_entry);
        } else {
            lowpan_adaptation_make_room_for_big_packet(cur, interface_ptr);
        }

        if (lowpan_adaptation_indirect_mac_data_request_active(interface_ptr, tx_ptr)) {
            // mac is handling previous data request, add new one to be cached */
            tr_debug_extra("caching seq: %d", tx_ptr->buf->seq);
            tx_ptr->indirect_data_cached = true;
        }

        ns_list_add_to_end(&interface_ptr->indirect_tx_queue, tx_ptr);

        // Check if current message can be delivered to MAC or should some cached message be delivered first
        tx_ptr_cached = lowpan_adaptation_indirect_first_cached_request_get(interface_ptr, tx_ptr);
        if (tx_ptr->indirect_data_cached == false && tx_ptr_cached) {
            tr_debug_extra("sending cached seq: %d", tx_ptr_cached->buf->seq);
            // set current message to cache
            tx_ptr->indirect_data_cached = true;
            // swap entries
            tx_ptr = tx_ptr_cached;
            tx_ptr->indirect_data_cached = false;
            buf = tx_ptr_cached->buf;
        } else if (tx_ptr->indirect_data_cached == true) {
            // There is mac data request ongoing and new req was sent to cache
            return 0;
        }
    }

    lowpan_data_request_to_mac(cur, buf, tx_ptr);
    return 0;


tx_error_handler:
    socket_tx_buffer_event_and_free(buf, SOCKET_NO_RAM);
    return -1;

}

static bool lowpan_adaptation_tx_process_ready(fragmenter_tx_entry_t *tx_ptr)
{
    if (!tx_ptr->fragmented_data) {
        if (tx_ptr->buf->ip_routed_up) {
            protocol_stats_update(STATS_IP_ROUTE_UP, buffer_data_length(tx_ptr->buf));
        } else {
            protocol_stats_update(STATS_IP_TX_COUNT, buffer_data_length(tx_ptr->buf));
        }
        return true;
    }



    //Update data pointer by last packet length
    buffer_data_strip_header(tx_ptr->buf, tx_ptr->frag_len);
    //Update offset
    if (!tx_ptr->first_fragment) {
        tx_ptr->offset += tx_ptr->frag_len / 8;
    } else {
        tx_ptr->first_fragment = false;
    }

    /* Check Is still Data what have to send */
    tx_ptr->frag_len = buffer_data_length(tx_ptr->buf);


    if (tx_ptr->frag_len == 0) {
        //Release current data
        if (tx_ptr->buf->ip_routed_up) {
            protocol_stats_update(STATS_IP_ROUTE_UP, tx_ptr->orig_size);
        } else {
            protocol_stats_update(STATS_IP_TX_COUNT, tx_ptr->orig_size);
        }
        return true;
    }

    //Continue Process

    if (tx_ptr->unfrag_len + 5 + tx_ptr->frag_len > tx_ptr->frag_max) {
        tx_ptr->frag_len = tx_ptr->frag_max - 5 - tx_ptr->unfrag_len;
        tx_ptr->frag_len &= ~7;
    }

    return false;
}

static void lowpan_adaptation_data_process_clean(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr, uint8_t socket_event)
{
    buffer_t *buf = tx_ptr->buf;
    tx_ptr->buf = NULL;
    if (buf->link_specific.ieee802_15_4.indirectTxProcess) {
        //release from list and free entry
        lowpan_indirect_entry_free(&interface_ptr->indirect_tx_queue, tx_ptr);
    }

    socket_tx_buffer_event_and_free(buf, socket_event);
}


int8_t lowpan_adaptation_interface_tx_confirm(protocol_interface_info_entry_t *cur, const mcps_data_conf_t *confirm)
{
    if( !cur || !confirm ){
        return -1;
    }

    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr) {
        return -1;
    }

    //Check first
    fragmenter_tx_entry_t *tx_ptr;
    bool active_direct_confirm;
    bool is_unicast = true;

    if (lowpan_active_tx_handle_verify(confirm->msduHandle,interface_ptr->active_unicast_tx_buf.buf)) {
        active_direct_confirm = true;
        tx_ptr = &interface_ptr->active_unicast_tx_buf;
    } else if (lowpan_active_tx_handle_verify(confirm->msduHandle,interface_ptr->active_broadcast_tx_buf.buf)) {
        active_direct_confirm = true;
        tx_ptr = &interface_ptr->active_broadcast_tx_buf;
        is_unicast = false;
    } else {
        active_direct_confirm = false;
        tx_ptr = lowpan_indirect_tx_handle_verify(confirm->msduHandle, &interface_ptr->indirect_tx_queue);
    }

    if (!tx_ptr) {
        tr_error("No data request for this confirmation %u", confirm->msduHandle);
        return -1;
    }

    //Check status for
    buffer_t *buf = tx_ptr->buf;

    //Indirect data expiration
    if (confirm->status == MLME_TRANSACTION_EXPIRED && !active_direct_confirm) {
        if (buf->link_specific.ieee802_15_4.indirectTTL > 7000)
        {
            buf->link_specific.ieee802_15_4.indirectTTL -= 7000;
            //Push Back to MAC
            lowpan_data_request_to_mac(cur, buf, tx_ptr);
            return 0;
        }
    }

    switch (confirm->status) {
        case MLME_TX_NO_ACK:
        case MLME_NO_DATA:
        case MLME_SUCCESS:
            if (buf->link_specific.ieee802_15_4.requestAck) {
                bool success = false;
                if (confirm->status == MLME_SUCCESS) {
                    success = true;
                }
                etx_transm_attempts_update(cur->id, 1 + confirm->tx_retries , success, buf->dst_sa.addr_type, buf->dst_sa.address);
            }
            break;
        default:

            break;

    }
    //Switch original channel back
    if (buf->link_specific.ieee802_15_4.rf_channel_switch) {
        mac_helper_mac_channel_set(cur, buf->link_specific.ieee802_15_4.selected_channel);
        buf->link_specific.ieee802_15_4.rf_channel_switch = false;
    }

    switch (confirm->status) {

        case MLME_BUSY_CHAN:
            lowpan_data_request_to_mac(cur, buf, tx_ptr);
            break;
        case MLME_SUCCESS:

            //Check is there more packets
            if (lowpan_adaptation_tx_process_ready(tx_ptr)) {
                bool triggered_from_indirect_cache = false;
                if (tx_ptr->fragmented_data && active_direct_confirm) {
                    //Clean
                    interface_ptr->fragmenter_active = false;
                }

                if (tx_ptr->buf->link_specific.ieee802_15_4.indirectTxProcess) {
                    triggered_from_indirect_cache = lowpan_adaptation_indirect_cache_trigger(cur, interface_ptr, tx_ptr);
                }

                lowpan_adaptation_data_process_clean(interface_ptr, tx_ptr, map_mlme_status_to_socket_event(confirm->status));

                if (triggered_from_indirect_cache) {
                    return 0;
                }
            } else {
                lowpan_data_request_to_mac(cur, buf, tx_ptr);
            }

            break;
        case MLME_TX_NO_ACK:
        case MLME_SECURITY_FAIL:
        case MLME_TRANSACTION_EXPIRED:
        default:
            tr_error("MCPS Data fail by status %u", confirm->status);
#ifdef HAVE_RPL
            if (confirm->status == MLME_TX_NO_ACK) {
                if (buf->route && rpl_data_is_rpl_parent_route(buf->route->route_info.source)) {
                    protocol_stats_update(STATS_RPL_PARENT_TX_FAIL, 1);
                }
            }
#endif
            if (tx_ptr->fragmented_data) {
                tx_ptr->buf->buf_ptr = tx_ptr->buf->buf_end;
                tx_ptr->buf->buf_ptr -= tx_ptr->orig_size;
                if (active_direct_confirm) {
                    interface_ptr->fragmenter_active = false;
                }
            }

            lowpan_adaptation_data_process_clean(interface_ptr, tx_ptr, map_mlme_status_to_socket_event(confirm->status));
            break;

    }

    if ((is_unicast && !interface_ptr->active_unicast_tx_buf.buf) || (!is_unicast && !interface_ptr->active_broadcast_tx_buf.buf)) {
        //Read Buffer and trig next direct request
        lowpan_adaptation_interface_tx(cur, lowpan_adaptation_tx_queue_read(interface_ptr, cur));
    }

    return 0;

}

static uint8_t map_mlme_status_to_socket_event(uint8_t mlme_status)
{
    uint8_t socket_event;

    switch (mlme_status) {
        case MLME_SUCCESS:
            socket_event = SOCKET_TX_DONE;
            break;
        case MLME_TX_NO_ACK:
        case MLME_SECURITY_FAIL:
        case MLME_TRANSACTION_EXPIRED:
        default:
            socket_event = SOCKET_TX_FAIL;
            break;
    }

    return (socket_event);
}

bool lowpan_adaptation_tx_active(int8_t interface_id)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);

    if (!interface_ptr || (!interface_ptr->active_unicast_tx_buf.buf && !interface_ptr->active_broadcast_tx_buf.buf)) {
        return false;
    }
    return true;
}

static bool lowpan_tx_buffer_address_compare(sockaddr_t *dst_sa, uint8_t *address_ptr, addrtype_t adr_type)
{

    if (dst_sa->addr_type != adr_type) {
        return false;
    }

    uint8_t compare_length;
    switch (adr_type) {
        case ADDR_802_15_4_SHORT:
            compare_length = 2;
            break;
        case ADDR_802_15_4_LONG:
            compare_length = 8;
            break;
        default:
            return false;
    }


    if (memcmp(&dst_sa->address[2], address_ptr, compare_length)) {
        return false;
    }
    return true;
}

static void lowpan_adaptation_purge_from_mac(struct protocol_interface_info_entry *cur, uint8_t msduhandle)
{
    mcps_purge_t purge_req;
    purge_req.msduHandle = msduhandle;
    cur->mac_api->mcps_purge_req(cur->mac_api, &purge_req);
}

static void lowpan_adaptation_indirect_queue_free_message(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
{
    tr_debug("Purge from indirect handle %u", tx_ptr->buf->seq);
    if (cur->mac_api->mcps_purge_req) {
        lowpan_adaptation_purge_from_mac(cur, tx_ptr->buf->seq);
    }
    lowpan_adaptation_data_process_clean(interface_ptr, tx_ptr, SOCKET_TX_FAIL);
}

int8_t lowpan_adaptation_indirect_free_messages_from_queues_by_address(struct protocol_interface_info_entry *cur, uint8_t *address_ptr, addrtype_t adr_type)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);

    if (!interface_ptr ) {
        return -1;
    }

    //Check first indirect queue
    ns_list_foreach_safe(fragmenter_tx_entry_t, entry, &interface_ptr->indirect_tx_queue) {

        if (lowpan_tx_buffer_address_compare(&entry->buf->dst_sa, address_ptr, adr_type)) {
            //Purge from mac
            lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, entry);
        }
    }

    return 0;

}

int8_t lowpan_adaptation_indirect_queue_params_set(struct protocol_interface_info_entry *cur, uint16_t indirect_big_packet_threshold, uint16_t max_indirect_big_packets_total, uint16_t max_indirect_small_packets_per_child)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);

    if (!interface_ptr) {
        return -1;
    }

    interface_ptr->indirect_big_packet_threshold = indirect_big_packet_threshold;
    interface_ptr->max_indirect_big_packets_total = max_indirect_big_packets_total;
    interface_ptr->max_indirect_small_packets_per_child = max_indirect_small_packets_per_child;

    return 0;
}
