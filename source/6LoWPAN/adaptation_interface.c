/*
 * Copyright (c) 2016-2021, Pelion and affiliates.
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
#include "Core/include/ns_address_internal.h"
#include "Core/include/ns_socket.h"
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
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "lowpan_adaptation_interface.h"
#include "MLE/mle.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/ip.h"
#ifdef HAVE_RPL
#include "RPL/rpl_data.h"
#endif
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/ws/ws_common.h"
#include "Service_Libs/random_early_detection/random_early_detection_api.h"
#define TRACE_GROUP "6lAd"

typedef void (adaptation_etx_update_cb)(protocol_interface_info_entry_t *cur, buffer_t *buf, const mcps_data_conf_t *confirm);

// #define EXTRA_DEBUG_EXTRA
#ifdef EXTRA_DEBUG_EXTRA
#define tr_debug_extra(...) tr_debug(__VA_ARGS__)
#else
#define tr_debug_extra(...)
#endif

#define ADAPTION_DIRECT_TX_QUEUE_SIZE_THRESHOLD_TRACE 20

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
    bool fragmented_data: 1;
    bool first_fragment: 1;
    bool indirect_data: 1;
    bool indirect_data_cached: 1; /*!< Data cached for delayed transmission as mac request is already active */
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
    fragmenter_tx_entry_t active_broadcast_tx_buf; //Current active direct broadcast tx process
    fragmenter_tx_list_t activeUnicastList; //Unicast packets waiting data confirmation from MAC
    buffer_list_t directTxQueue; //Waiting free tx process
    uint16_t directTxQueue_size;
    uint16_t directTxQueue_level;
    uint16_t activeTxList_size;
    uint16_t indirect_big_packet_threshold;
    uint16_t max_indirect_big_packets_total;
    uint16_t max_indirect_small_packets_per_child;
    uint32_t last_rx_high_priority;
    bool fragmenter_active; /*!< Fragmenter state */
    adaptation_etx_update_cb *etx_update_cb;
    mpx_api_t *mpx_api;
    uint16_t mpx_user_id;
    ns_list_link_t      link; /*!< List link entry */
} fragmenter_interface_t;

#define LOWPAN_ACTIVE_UNICAST_ONGOING_MAX 10
#define LOWPAN_HIGH_PRIORITY_STATE_LENGTH 50 //5 seconds 100us ticks

/* Minimum buffer amount and memory size to ensure operation even in out of memory situation
 */
#define LOWPAN_MEM_LIMIT_MIN_QUEUE 10
#define LOWPAN_MEM_LIMIT_MIN_MEMORY 10000
#define LOWPAN_MEM_LIMIT_REMOVE_NORMAL 3000 // Remove when approaching memory limit
#define LOWPAN_MEM_LIMIT_REMOVE_MAX 10000 // Remove when at memory limit
#define LOWPAN_MEM_LIMIT_REMOVE_EF_MODE 20000 // Remove when out of memory and we are in EF mode

#define LOWPAN_TX_BUFFER_AGE_LIMIT_LOW_PRIORITY     30 // Remove low priority packets older than limit (seconds)
#define LOWPAN_TX_BUFFER_AGE_LIMIT_HIGH_PRIORITY    60 // Remove high priority packets older than limit (seconds)
#define LOWPAN_TX_BUFFER_AGE_LIMIT_EF_PRIORITY      120 // Remove expedited forwarding packets older than limit (seconds)



static NS_LIST_DEFINE(fragmenter_interface_list, fragmenter_interface_t, link);

/* Adaptation interface local functions */
static fragmenter_interface_t *lowpan_adaptation_interface_discover(int8_t interfaceId);

/* Interface direct message pending queue functions */
static void lowpan_adaptation_tx_queue_write(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, buffer_t *buf);
static buffer_t *lowpan_adaptation_tx_queue_read(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr);

/* Data direction and message length validation */
static bool lowpan_adaptation_indirect_data_request(mac_neighbor_table_entry_t *mle_entry);
static bool lowpan_adaptation_request_longer_than_mtu(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_interface_t *interface_ptr);

/* Common data tx request process functions */
static void lowpan_active_buffer_state_reset(fragmenter_tx_entry_t *tx_buffer);
static uint8_t lowpan_data_request_unique_handle_get(fragmenter_interface_t *interface_ptr);
static fragmenter_tx_entry_t *lowpan_indirect_entry_allocate(uint16_t fragment_buffer_size);
static fragmenter_tx_entry_t *lowpan_adaptation_tx_process_init(fragmenter_interface_t *interface_ptr, bool indirect, bool fragmented, bool is_unicast);
static void lowpan_adaptation_data_request_primitiv_set(const buffer_t *buf, mcps_data_req_t *dataReq, protocol_interface_info_entry_t *cur);
static void lowpan_data_request_to_mac(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_tx_entry_t *tx_ptr, fragmenter_interface_t *interface_ptr);

/* Tx confirmation local functions */
static bool lowpan_active_tx_handle_verify(uint8_t handle, buffer_t *buf);
static fragmenter_tx_entry_t *lowpan_listed_tx_handle_verify(uint8_t handle, fragmenter_tx_list_t *indirect_tx_queue);
static void lowpan_adaptation_data_process_clean(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr, uint8_t socket_event);
static uint8_t map_mlme_status_to_socket_event(uint8_t mlme_status);
static bool lowpan_adaptation_tx_process_ready(fragmenter_tx_entry_t *tx_ptr);

/* Fragmentation local functions */
static int8_t lowpan_message_fragmentation_init(buffer_t *buf, fragmenter_tx_entry_t *frag_entry, protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr);
static bool lowpan_message_fragmentation_message_write(const fragmenter_tx_entry_t *frag_entry, mcps_data_req_t *dataReq);
static bool lowpan_adaptation_indirect_queue_free_message(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr);

static fragmenter_tx_entry_t *lowpan_adaptation_indirect_mac_data_request_active(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr);

static bool lowpan_buffer_tx_allowed(fragmenter_interface_t *interface_ptr, buffer_t *buf);
static bool lowpan_adaptation_purge_from_mac(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr,  uint8_t msduhandle);

static void lowpan_adaptation_etx_update_cb(protocol_interface_info_entry_t *cur, buffer_t *buf, const mcps_data_conf_t *confirm)
{
    switch (confirm->status) {
        case MLME_TX_NO_ACK:
        case MLME_NO_DATA:
        case MLME_SUCCESS:
            if (buf->link_specific.ieee802_15_4.requestAck) {
                if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
                    bool success = false;
                    if (confirm->status == MLME_SUCCESS) {
                        success = true;
                    }
                    // Gets table entry
                    mac_neighbor_table_entry_t *neigh_table_ptr = mac_neighbor_table_address_discover(mac_neighbor_info(cur), buf->dst_sa.address + PAN_ID_LEN, buf->dst_sa.addr_type);
                    if (neigh_table_ptr) {
                        etx_transm_attempts_update(cur->id, 1 + confirm->tx_retries, success, neigh_table_ptr->index, neigh_table_ptr->mac64);
                        // Updates ETX statistics
                        etx_storage_t *etx_entry = etx_storage_entry_get(cur->id, neigh_table_ptr->index);
                        if (etx_entry) {
                            if (neigh_table_ptr->link_role == PRIORITY_PARENT_NEIGHBOUR) {
                                protocol_stats_update(STATS_ETX_1ST_PARENT, etx_entry->etx >> 4);
                            } else if (neigh_table_ptr->link_role == SECONDARY_PARENT_NEIGHBOUR) {
                                protocol_stats_update(STATS_ETX_2ND_PARENT, etx_entry->etx >> 4);
                            }
                        }
                    }
                }
            }
            break;
        default:

            break;

    }
}


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

static struct protocol_interface_info_entry *lowpan_adaptation_network_interface_discover(const mpx_api_t *api)
{

    ns_list_foreach(fragmenter_interface_t, interface_ptr, &fragmenter_interface_list) {
        if (api == interface_ptr->mpx_api) {
            return protocol_stack_interface_info_get_by_id(interface_ptr->interface_id);
        }
    }

    return NULL;
}


static void lowpan_adaptation_tx_queue_level_update(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr)
{
    random_early_detetction_aq_calc(cur->random_early_detection, interface_ptr->directTxQueue_size);
    protocol_stats_update(STATS_AL_TX_QUEUE_SIZE, interface_ptr->directTxQueue_size);

    if (interface_ptr->directTxQueue_size == interface_ptr->directTxQueue_level + ADAPTION_DIRECT_TX_QUEUE_SIZE_THRESHOLD_TRACE ||
            interface_ptr->directTxQueue_size == interface_ptr->directTxQueue_level - ADAPTION_DIRECT_TX_QUEUE_SIZE_THRESHOLD_TRACE) {
        interface_ptr->directTxQueue_level = interface_ptr->directTxQueue_size;
        tr_info("Adaptation layer TX queue size %u Active MAC tx request %u", interface_ptr->directTxQueue_level, interface_ptr->activeTxList_size);
    }
}


static void lowpan_adaptation_tx_queue_write(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, buffer_t *buf)
{
    buffer_t *lower_priority_buf = NULL;

    ns_list_foreach(buffer_t, entry, &interface_ptr->directTxQueue) {

        if (entry->priority < buf->priority) {
            lower_priority_buf = entry;
            break;
        }
    }

    if (lower_priority_buf) {
        ns_list_add_before(&interface_ptr->directTxQueue, lower_priority_buf, buf);
    } else {
        ns_list_add_to_end(&interface_ptr->directTxQueue, buf);
    }
    interface_ptr->directTxQueue_size++;
    lowpan_adaptation_tx_queue_level_update(cur, interface_ptr);
}

static void lowpan_adaptation_tx_queue_write_to_front(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, buffer_t *buf)
{
    buffer_t *lower_priority_buf = NULL;

    ns_list_foreach(buffer_t, entry, &interface_ptr->directTxQueue) {

        if (entry->priority <= buf->priority) {
            lower_priority_buf = entry;
            break;
        }
    }

    if (lower_priority_buf) {
        ns_list_add_before(&interface_ptr->directTxQueue, lower_priority_buf, buf);
    } else {
        ns_list_add_to_end(&interface_ptr->directTxQueue, buf);
    }
    interface_ptr->directTxQueue_size++;
    lowpan_adaptation_tx_queue_level_update(cur, interface_ptr);
}

static buffer_t *lowpan_adaptation_tx_queue_read(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr)
{
    // Currently this function is called only when data confirm is received for previously sent packet.
    if (!interface_ptr->directTxQueue_size) {
        return NULL;
    }
    ns_list_foreach_safe(buffer_t, buf, &interface_ptr->directTxQueue) {

        if (buf->link_specific.ieee802_15_4.requestAck && interface_ptr->last_rx_high_priority &&  buf->priority < QOS_EXPEDITE_FORWARD) {
            //Stop reading at this point when Priority is not enough big
            return NULL;
        }

        if (lowpan_buffer_tx_allowed(interface_ptr, buf)) {
            ns_list_remove(&interface_ptr->directTxQueue, buf);
            interface_ptr->directTxQueue_size--;
            lowpan_adaptation_tx_queue_level_update(cur, interface_ptr);
            return buf;
        }
    }
    return NULL;
}

//fragmentation needed

static bool lowpan_adaptation_request_longer_than_mtu(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_interface_t *interface_ptr)
{
    uint_fast16_t overhead = mac_helper_frame_overhead(cur, buf);

    if (interface_ptr->mpx_api) {
        overhead += interface_ptr->mpx_api->mpx_headroom_size_get(interface_ptr->mpx_api, interface_ptr->mpx_user_id);
    }


    if (buffer_data_length(buf) > (int16_t)mac_helper_max_payload_size(cur, overhead)) {
        return true;
    } else {
        return false;
    }
}

static bool lowpan_adaptation_indirect_data_request(mac_neighbor_table_entry_t *entry_ptr)
{
    if (entry_ptr && !(entry_ptr->rx_on_idle)) {
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



static fragmenter_tx_entry_t *lowpan_listed_tx_handle_verify(uint8_t handle, fragmenter_tx_list_t *indirect_tx_queue)
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
    while (!valid_info) {
        handle = interface_ptr->msduHandle++;
        if (!lowpan_listed_tx_handle_verify(handle, &interface_ptr->activeUnicastList)
                && !lowpan_active_tx_handle_verify(handle, interface_ptr->active_broadcast_tx_buf.buf)
                && !lowpan_listed_tx_handle_verify(handle, &interface_ptr->indirect_tx_queue)) {
            valid_info = true;
        }
    }
    return handle;

}

static void lowpan_list_entry_free(fragmenter_tx_list_t *list, fragmenter_tx_entry_t *entry)
{
    ns_list_remove(list, entry);
    if (entry->buf) {
        buffer_free(entry->buf);
    }
    ns_dyn_mem_free(entry->fragmenter_buf);
    ns_dyn_mem_free(entry);
}

static void lowpan_list_free(fragmenter_tx_list_t *list, bool fragment_buf_free)
{
    while (!ns_list_is_empty(list)) {
        fragmenter_tx_entry_t *entry = ns_list_get_first(list);
        if (!fragment_buf_free) {
            //We can't free this pointer becuase it must be until interface is deleted
            entry->fragmenter_buf = NULL;
        }
        lowpan_list_entry_free(list, entry);
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
    if (!interface_ptr) {
        return -1;
    }

    memset(interface_ptr, 0, sizeof(fragmenter_interface_t));
    interface_ptr->interface_id = interface_id;
    interface_ptr->fragment_indirect_tx_buffer = NULL;
    interface_ptr->mtu_size = 0;
    interface_ptr->msduHandle = randLIB_get_8bit();
    interface_ptr->local_frag_tag = randLIB_get_16bit();

    ns_list_init(&interface_ptr->indirect_tx_queue);
    ns_list_init(&interface_ptr->directTxQueue);
    ns_list_init(&interface_ptr->activeUnicastList);
    interface_ptr->activeTxList_size = 0;
    interface_ptr->directTxQueue_size = 0;
    interface_ptr->directTxQueue_level = 0;

    ns_list_add_to_end(&fragmenter_interface_list, interface_ptr);

    return 0;
}

void lowpan_adaptation_interface_etx_update_enable(int8_t interface_id)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);
    if (interface_ptr) {
        interface_ptr->etx_update_cb = lowpan_adaptation_etx_update_cb;
    }
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
    lowpan_list_free(&interface_ptr->activeUnicastList, false);
    interface_ptr->activeTxList_size = 0;
    lowpan_active_buffer_state_reset(&interface_ptr->active_broadcast_tx_buf);

    //Free Indirect entry
    lowpan_list_free(&interface_ptr->indirect_tx_queue, true);

    buffer_free_list(&interface_ptr->directTxQueue);
    interface_ptr->directTxQueue_size = 0;
    interface_ptr->directTxQueue_level = 0;
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
    lowpan_list_free(&interface_ptr->activeUnicastList, false);
    interface_ptr->activeTxList_size  = 0;
    lowpan_active_buffer_state_reset(&interface_ptr->active_broadcast_tx_buf);
    //Clean fragmented message flag
    interface_ptr->fragmenter_active = false;

    //Free Indirect entry
    lowpan_list_free(&interface_ptr->indirect_tx_queue, true);

    buffer_free_list(&interface_ptr->directTxQueue);
    interface_ptr->directTxQueue_size = 0;
    interface_ptr->directTxQueue_level = 0;
    interface_ptr->last_rx_high_priority = 0;

    return 0;
}

static void lowpan_adaptation_mpx_data_confirm(const mpx_api_t *api, const struct mcps_data_conf_s *data)
{
    protocol_interface_info_entry_t *interface = lowpan_adaptation_network_interface_discover(api);

    lowpan_adaptation_interface_tx_confirm(interface, data);
}

static void lowpan_adaptation_mpx_data_indication(const mpx_api_t *api, const struct mcps_data_ind_s *data)
{
    protocol_interface_info_entry_t *interface = lowpan_adaptation_network_interface_discover(api);
    lowpan_adaptation_interface_data_ind(interface, data);
}




int8_t lowpan_adaptation_interface_mpx_register(int8_t interface_id, struct mpx_api_s *mpx_api, uint16_t mpx_user_id)
{
    //Discover
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);
    if (!interface_ptr) {
        return -1;
    }
    if (!mpx_api && interface_ptr->mpx_api) {
        //Disable Data Callbacks from MPX Class
        interface_ptr->mpx_api->mpx_user_registration(interface_ptr->mpx_api, NULL, NULL, interface_ptr->mpx_user_id);
    }

    interface_ptr->mpx_api = mpx_api;
    interface_ptr->mpx_user_id = mpx_user_id;

    if (interface_ptr->mpx_api) {
        //Register MPX callbacks: confirmation and indication
        interface_ptr->mpx_api->mpx_user_registration(interface_ptr->mpx_api, lowpan_adaptation_mpx_data_confirm, lowpan_adaptation_mpx_data_indication, interface_ptr->mpx_user_id);
    }
    return 0;
}

void lowpan_adaptation_free_heap(bool full_gc)
{
    ns_list_foreach(fragmenter_interface_t, interface_ptr, &fragmenter_interface_list) {
        // Go through all interfaces and free small amount of memory
        // This is not very radical, but gives time to recover without causing too harsh changes
        buffer_priority_t priority = QOS_NORMAL;
        uint32_t amount = LOWPAN_MEM_LIMIT_REMOVE_NORMAL;

        if (full_gc && interface_ptr->last_rx_high_priority) {
            // We have encountered out of memory in EF state We handle this as Critical state
            // Large amount of memory is freed and packets lower than EF priority can be dropped
            priority = QOS_NETWORK_CTRL;
            amount = LOWPAN_MEM_LIMIT_REMOVE_EF_MODE;
        } else if (interface_ptr->last_rx_high_priority) {
            // We are running out of memory and we are in EF state. We handle this as severe state
            // Some amount of memory is freed and also lower than EF priority packets are dropped
            priority = QOS_NETWORK_CTRL;
            amount = LOWPAN_MEM_LIMIT_REMOVE_MAX;
        } else if (full_gc) {
            // We have encountered out of memory. we need to remove more packets, but we only remove normal priority packets
            amount = LOWPAN_MEM_LIMIT_REMOVE_MAX;
        }

        lowpan_adaptation_free_low_priority_packets(interface_ptr->interface_id, priority, amount);
    }
}
buffer_t *lowpan_adaptation_get_oldest_packet(fragmenter_interface_t *interface_ptr, buffer_priority_t priority)
{
    ns_list_foreach(buffer_t, entry, &interface_ptr->directTxQueue) {
        if (entry->priority == priority) {
            // Only Higher priority packets left no need to go through list anymore
            return entry;
        }
    }
    return NULL;

}

int8_t lowpan_adaptation_free_low_priority_packets(int8_t interface_id, buffer_priority_t max_priority, uint32_t requested_amount)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!interface_ptr || !cur) {
        return -1;
    }
    uint32_t adaptation_memory = 0;
    uint16_t adaptation_packets = 0;
    uint32_t memory_freed = 0;
    uint16_t packets_freed = 0;

    ns_list_foreach(buffer_t, entry, &interface_ptr->directTxQueue) {
        adaptation_memory += sizeof(buffer_t) + entry->size;
        adaptation_packets++;
    }

    if (interface_ptr->directTxQueue_size < LOWPAN_MEM_LIMIT_MIN_QUEUE) {
        // Minimum reserved for operations
        return 0;
    }
    if (adaptation_memory < LOWPAN_MEM_LIMIT_MIN_MEMORY) {
        // Minimum reserved for operations
        return 0;
    }
    if (adaptation_memory - requested_amount < LOWPAN_MEM_LIMIT_MIN_MEMORY) {
        // only reduse to minimum
        requested_amount = adaptation_memory - LOWPAN_MEM_LIMIT_MIN_MEMORY;
    }

    /* Order of packets is
     * priority 4 oldest to newest
     * priority 3 oldest to newest
     * priority 2 oldest to newest
     * priority 1 oldest to newest
     * priority 0 oldest to newest
     * So we search oldest for lowest priority and delete that until that priority is empty
     * and then start deleting one higher priority packets from oldest first
     */

    buffer_priority_t priority = QOS_NORMAL;
    do {
        buffer_t *entry = lowpan_adaptation_get_oldest_packet(interface_ptr, priority);
        if (!entry) {
            if (priority < max_priority) {
                priority++;
                continue;
            }
            // No more packets available
            break;
        }
        // This packet can be deleted
        memory_freed += sizeof(buffer_t) + entry->size;
        packets_freed++;
        ns_list_remove(&interface_ptr->directTxQueue, entry);
        interface_ptr->directTxQueue_size--;
        lowpan_adaptation_tx_queue_level_update(cur, interface_ptr);
        socket_tx_buffer_event_and_free(entry, SOCKET_TX_FAIL);
    } while (memory_freed < requested_amount);

    tr_info("Adaptation Free low priority packets memory: %" PRIi32 " queue: %d deallocated %" PRIi32 " bytes, %d packets, %" PRIi32 " requested", adaptation_memory, adaptation_packets, memory_freed, packets_freed, requested_amount);
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

static int8_t lowpan_message_fragmentation_init(buffer_t *buf, fragmenter_tx_entry_t *frag_entry, protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr)
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

    uint_fast16_t overhead = mac_helper_frame_overhead(cur, buf);
    if (interface_ptr->mpx_api) {
        overhead += interface_ptr->mpx_api->mpx_headroom_size_get(interface_ptr->mpx_api, interface_ptr->mpx_user_id);
    }

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

static fragmenter_tx_entry_t *lowpan_adaptation_tx_process_init(fragmenter_interface_t *interface_ptr, bool indirect, bool fragmented, bool is_unicast)
{
    // For broadcast, the active TX queue is only 1 entry. For unicast, using a list.
    fragmenter_tx_entry_t *tx_entry;
    if (!indirect) {
        if (is_unicast) {
            tx_entry = lowpan_indirect_entry_allocate(0);
            if (!tx_entry) {
                return NULL;
            }
            ns_list_add_to_end(&interface_ptr->activeUnicastList, tx_entry);
            interface_ptr->activeTxList_size++;
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

buffer_t *lowpan_adaptation_data_process_tx_preprocess(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    mac_neighbor_table_entry_t *neigh_entry_ptr = NULL;


    //Validate is link known and set indirect, datareq and security key id mode
    if (buf->dst_sa.addr_type == ADDR_NONE) {
        goto tx_error_handler;
    }

    if (addr_check_broadcast(buf->dst_sa.address, buf->dst_sa.addr_type) == eOK) {
        buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;
        buf->dst_sa.address[2] = 0xff;
        buf->dst_sa.address[3] = 0xff;
        buf->link_specific.ieee802_15_4.indirectTxProcess = false;
        buf->link_specific.ieee802_15_4.requestAck = false;
    } else {

        neigh_entry_ptr = mac_neighbor_table_address_discover(mac_neighbor_info(cur), buf->dst_sa.address + 2, buf->dst_sa.addr_type);

        //Validate neighbour
        if (!buf->options.ll_security_bypass_tx && neigh_entry_ptr) {

            if (neigh_entry_ptr->connected_device ||  neigh_entry_ptr->trusted_device) {

            } else {
                //tr_warn("Drop TX to unassociated %s", trace_sockaddr(&buf->dst_sa, true));
                goto tx_error_handler;
            }
        } else if (ws_info(cur) && !neigh_entry_ptr) {
            //Do not accept to send unknow device
            goto tx_error_handler;
        }
        buf->link_specific.ieee802_15_4.requestAck = true;
        buf->link_specific.ieee802_15_4.indirectTxProcess = lowpan_adaptation_indirect_data_request(neigh_entry_ptr);
    }

    if (buf->link_specific.ieee802_15_4.key_id_mode != B_SECURITY_KEY_ID_2) {

        if (!buf->link_specific.ieee802_15_4.requestAck) {
            buf->link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_MODE_DEFAULT;
        } else if (ws_info(cur) || (neigh_entry_ptr && !neigh_entry_ptr->trusted_device)) {
            buf->link_specific.ieee802_15_4.key_id_mode  = B_SECURITY_KEY_ID_MODE_DEFAULT;
        } else {
            buf->link_specific.ieee802_15_4.key_id_mode  = B_SECURITY_KEY_ID_IMPLICIT;
        }
    }

    return buf;

tx_error_handler:
    if (neigh_entry_ptr && neigh_entry_ptr->nud_active) {
        mac_neighbor_info(cur)->active_nud_process--;
        neigh_entry_ptr->nud_active = false;

    }
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
            lowpan_data_request_to_mac(cur, fragmenter_tx_entry->buf, fragmenter_tx_entry, interface_ptr);
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
                tr_debug_extra("Pushing seq %d to addr %s", fragmenter_tx_entry->buf->seq, trace_ipv6(fragmenter_tx_entry->buf->dst_sa.address));
                fragmenter_tx_entry->indirect_data_cached = false;
                lowpan_data_request_to_mac(cur, fragmenter_tx_entry->buf, fragmenter_tx_entry, interface_ptr);
                return true;
            }
        }
    }

    /* Sanity check, If nothing can be triggered from own address, check cache queue */
    return lowpan_adaptation_indirect_cache_sanity_check(cur, interface_ptr);
}

static fragmenter_tx_entry_t *lowpan_adaptation_indirect_mac_data_request_active(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
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

static fragmenter_tx_entry_t *lowpan_adaptation_indirect_first_cached_request_get(fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
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

static bool lowpan_adaptation_is_priority_message(buffer_t *buf)
{
    // Mle messages
    if (buf->dst_sa.port == MLE_ALLOCATED_PORT || buf->src_sa.port == MLE_ALLOCATED_PORT) {
        return true;
    }

    // Management messages: address solicit, response, query, notification
    if (buf->dst_sa.port  == THREAD_MANAGEMENT_PORT || buf->src_sa.port == THREAD_MANAGEMENT_PORT) {
        return true;
    }

    // dhcp messages
    if (buf->dst_sa.port == DHCPV6_SERVER_PORT || buf->src_sa.port == DHCPV6_SERVER_PORT) {
        return true;
    }

    if (buf->dst_sa.port == DHCPV6_CLIENT_PORT || buf->src_sa.port == DHCPV6_CLIENT_PORT) {
        return true;
    }

    // ICMPv6 messages
    if (buf->options.type == ICMPV6_TYPE_ERROR_DESTINATION_UNREACH ||
            buf->options.type == ICMPV6_TYPE_ERROR_PACKET_TOO_BIG ||
            buf->options.type == ICMPV6_TYPE_ERROR_TIME_EXCEEDED ||
            buf->options.type == ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM) {
        return true;
    }
    return false;
}

static bool lowpan_adaptation_make_room_for_small_packet(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, mac_neighbor_table_entry_t *neighbour_to_count, fragmenter_tx_entry_t *new_entry)
{
    if (interface_ptr->max_indirect_small_packets_per_child == 0) {
        // this means there is always space for small packets - no need to check further
        return true;
    }

    uint_fast16_t count = 0;
    fragmenter_tx_entry_t *low_priority_msg_ptr = NULL;

    ns_list_foreach_reverse_safe(fragmenter_tx_entry_t, tx_entry, &interface_ptr->indirect_tx_queue) {
        mac_neighbor_table_entry_t *tx_neighbour = mac_neighbor_table_address_discover(mac_neighbor_info(cur), tx_entry->buf->dst_sa.address + 2, tx_entry->buf->dst_sa.addr_type);
        if (tx_neighbour == neighbour_to_count && buffer_data_length(tx_entry->buf) <= interface_ptr->indirect_big_packet_threshold) {
            if (!lowpan_adaptation_is_priority_message(tx_entry->buf)) {
                // if there is sub priorities inside message example age here you could compare
                low_priority_msg_ptr = tx_entry;
            }
            if (++count >= interface_ptr->max_indirect_small_packets_per_child) {
                if (!low_priority_msg_ptr) {
                    // take last entry if no low priority entry found
                    if (lowpan_adaptation_is_priority_message(new_entry->buf)) {
                        low_priority_msg_ptr = tx_entry;
                    } else {
                        return false;
                    }
                }
                tr_debug_extra("Purge seq: %d", low_priority_msg_ptr->buf->seq);
                if (lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, low_priority_msg_ptr) == false) {
                    /* entry could not be purged from mac, try next entry */
                    tr_debug_extra("Purge failed, try next");
                    count--;
                }
                low_priority_msg_ptr = NULL;
            }
        }
    }
    return true;
}

static bool lowpan_adaptation_make_room_for_big_packet(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *new_entry)
{
    if (interface_ptr->max_indirect_big_packets_total == 0) {
        // this means there is always space for big packets - no need to check further
        return true;
    }

    uint_fast16_t count = 0;
    fragmenter_tx_entry_t *low_priority_msg_ptr = NULL;

    ns_list_foreach_reverse_safe(fragmenter_tx_entry_t, tx_entry, &interface_ptr->indirect_tx_queue) {
        if (buffer_data_length(tx_entry->buf) > interface_ptr->indirect_big_packet_threshold) {
            if (!lowpan_adaptation_is_priority_message(tx_entry->buf)) {
                // if there is sub priorities inside message example age here you could compare
                low_priority_msg_ptr = tx_entry;
            }
            if (++count >= interface_ptr->max_indirect_big_packets_total) {
                if (!low_priority_msg_ptr) {
                    // take last entry if no low priority entry found
                    if (lowpan_adaptation_is_priority_message(new_entry->buf)) {
                        low_priority_msg_ptr = tx_entry;
                    } else {
                        return false;
                    }
                }
                tr_debug_extra("Purge seq: %d", low_priority_msg_ptr->buf->seq);
                if (lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, low_priority_msg_ptr) == false) {
                    tr_debug_extra("Purge failed, try next entry");
                    /* entry could not be purged from mac, try next entry */
                    count--;
                }
                low_priority_msg_ptr = NULL;
            }
        }
    }
    return true;
}

static void lowpan_data_request_to_mac(protocol_interface_info_entry_t *cur, buffer_t *buf, fragmenter_tx_entry_t *tx_ptr, fragmenter_interface_t *interface_ptr)
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
    //Define data priority
    mac_data_priority_t data_priority;

    switch (buf->priority) {
        case QOS_HIGH:
            data_priority = MAC_DATA_MEDIUM_PRIORITY;
            break;
        case QOS_NETWORK_CTRL:
            data_priority = MAC_DATA_HIGH_PRIORITY;
            break;
        case QOS_EXPEDITE_FORWARD:
            data_priority = MAC_DATA_EXPEDITE_FORWARD;
            break;
        case QOS_MAC_BEACON:
            data_priority = MAC_DATA_HIGH_PRIORITY;
            break;
        default:
            data_priority = MAC_DATA_NORMAL_PRIORITY;
            break;
    }

    if (interface_ptr->mpx_api) {
        dataReq.ExtendedFrameExchange = buf->options.edfe_mode;
        interface_ptr->mpx_api->mpx_data_request(interface_ptr->mpx_api, &dataReq, interface_ptr->mpx_user_id, data_priority);
    } else {
        mcps_data_req_ie_list_t ie_list;
        memset(&ie_list, 0, sizeof(mcps_data_req_ie_list_t));
        cur->mac_api->mcps_data_req_ext(cur->mac_api, &dataReq, &ie_list, NULL, data_priority, 0);
    }
}

static bool lowpan_adaptation_is_destination_tx_active(fragmenter_tx_list_t *list, buffer_t *buf)
{
    ns_list_foreach(fragmenter_tx_entry_t, entry, list) {
        if (entry->buf) {
            if (!memcmp(&entry->buf->dst_sa.address[2], &buf->dst_sa.address[2], 8)) {
                return true;
            }
        }
    }
    return false;
}

static bool lowpan_buffer_tx_allowed(fragmenter_interface_t *interface_ptr, buffer_t *buf)
{
    bool is_unicast = buf->link_specific.ieee802_15_4.requestAck;
    // Indirect allowed always
    if (buf->link_specific.ieee802_15_4.indirectTxProcess) {
        return true;
    }
    // Do not accept any other TX when fragmented TX active. Prevents other frames to be sent in between two fragments.
    if (interface_ptr->fragmenter_active) {
        return false;
    }
    // Do not accept more than one active broadcast TX
    if (!is_unicast && interface_ptr->active_broadcast_tx_buf.buf) {
        return false;
    }

    if (is_unicast && interface_ptr->activeTxList_size >= LOWPAN_ACTIVE_UNICAST_ONGOING_MAX) {
        //New TX is not possible there is already too manyactive connecting
        return false;
    }


    // Do not accept more than one active unicast TX per destination
    if (is_unicast && lowpan_adaptation_is_destination_tx_active(&interface_ptr->activeUnicastList, buf)) {
        return false;
    }

    if (is_unicast && interface_ptr->last_rx_high_priority &&  buf->priority < QOS_EXPEDITE_FORWARD) {
        return false;
    }
    return true;
}

static bool lowpan_adaptation_high_priority_state_exit(fragmenter_interface_t *interface_ptr)
{
    if (!interface_ptr->last_rx_high_priority || ((protocol_core_monotonic_time - interface_ptr->last_rx_high_priority) < LOWPAN_HIGH_PRIORITY_STATE_LENGTH)) {
        return false;
    }

    //Check First buffer_from tx queue
    buffer_t *buf = ns_list_get_first(&interface_ptr->directTxQueue);
    if (buf && buf->priority == QOS_EXPEDITE_FORWARD) {
        //TX queue must not include any
        return false;
    }

    //Check If we have a Any active TX process still active
    ns_list_foreach(fragmenter_tx_entry_t, entry, &interface_ptr->activeUnicastList) {
        if (entry->buf->priority == QOS_EXPEDITE_FORWARD) {
            return false;
        }
    }

    //Disable High Priority Mode
    if (interface_ptr->mpx_api) {
        interface_ptr->mpx_api->mpx_priority_mode_set(interface_ptr->mpx_api, false);
    }
    interface_ptr->last_rx_high_priority = 0;
    return true;
}

static void lowpan_adaptation_high_priority_state_enable(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr)
{

    if (!interface_ptr->last_rx_high_priority) {
        // MPX enaled stack must inform MPX to priority enable
        if (interface_ptr->mpx_api) {
            interface_ptr->mpx_api->mpx_priority_mode_set(interface_ptr->mpx_api, true);
        }
        //Purge Active tx queue's all possible's
        if (!interface_ptr->fragmenter_active) {
            //Purge Only When Fragmenter is not active
            ns_list_foreach_reverse_safe(fragmenter_tx_entry_t, entry, &interface_ptr->activeUnicastList) {

                if (lowpan_adaptation_purge_from_mac(cur, interface_ptr, entry->buf->seq)) {
                    buffer_t *buf = entry->buf;
                    ns_list_remove(&interface_ptr->activeUnicastList, entry);
                    interface_ptr->activeTxList_size--;
                    ns_dyn_mem_free(entry);
                    //Add message to tx queue front based on priority. Now same priority at buf is prioritised at order
                    lowpan_adaptation_tx_queue_write_to_front(cur, interface_ptr, buf);
                }
            }
        }
    }

    //Store timestamp for indicate last RX High Priority message
    interface_ptr->last_rx_high_priority = protocol_core_monotonic_time ? protocol_core_monotonic_time : 1;

}


static void lowpan_adaptation_priority_status_update(protocol_interface_info_entry_t *cur, fragmenter_interface_t *interface_ptr, buffer_priority_t priority)
{
    if (priority == QOS_EXPEDITE_FORWARD) {
        lowpan_adaptation_high_priority_state_enable(cur, interface_ptr);
    } else {
        //Let check can we disable possible High Priority state
        lowpan_adaptation_high_priority_state_exit(interface_ptr);
    }
}

void lowpan_adaptation_expedite_forward_enable(protocol_interface_info_entry_t *cur)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr) {
        return;
    }
    lowpan_adaptation_high_priority_state_enable(cur, interface_ptr);
}

bool lowpan_adaptation_expedite_forward_state_get(protocol_interface_info_entry_t *cur)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr || !interface_ptr->last_rx_high_priority) {
        return false;
    }
    return true;
}

void lowpan_adaptation_interface_slow_timer(protocol_interface_info_entry_t *cur)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr) {
        return;
    }

    if (lowpan_adaptation_high_priority_state_exit(interface_ptr)) {
        //Activate Packets from TX queue
        buffer_t *buf_from_queue = lowpan_adaptation_tx_queue_read(cur, interface_ptr);
        while (buf_from_queue) {
            lowpan_adaptation_interface_tx(cur, buf_from_queue);
            buf_from_queue = lowpan_adaptation_tx_queue_read(cur, interface_ptr);
        }
    }
}

static bool lowpan_adaptation_interface_check_buffer_timeout(buffer_t *buf)
{
    // Convert from 100ms slots to seconds
    uint32_t buffer_age_s = (protocol_core_monotonic_time - buf->adaptation_timestamp) / 10;

    if ((buf->priority == QOS_NORMAL) && (buffer_age_s > LOWPAN_TX_BUFFER_AGE_LIMIT_LOW_PRIORITY)) {
        return true;
    } else if ((buf->priority == QOS_HIGH) && (buffer_age_s > LOWPAN_TX_BUFFER_AGE_LIMIT_HIGH_PRIORITY)) {
        return true;
    } else if ((buf->priority == QOS_NETWORK_CTRL) && (buffer_age_s > LOWPAN_TX_BUFFER_AGE_LIMIT_HIGH_PRIORITY)) {
        return true;
    } else if ((buf->priority == QOS_EXPEDITE_FORWARD) && (buffer_age_s > LOWPAN_TX_BUFFER_AGE_LIMIT_EF_PRIORITY)) {
        return true;
    }
    return false;
}

int8_t lowpan_adaptation_interface_tx(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    bool is_room_for_new_message;
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

    uint8_t traffic_class = buf->options.traffic_class >> IP_TCLASS_DSCP_SHIFT;

    if (traffic_class == IP_DSCP_EF) {
        buffer_priority_set(buf, QOS_EXPEDITE_FORWARD);
    } else if (traffic_class == IP_DSCP_CS6) {
        //Network Control
        buffer_priority_set(buf, QOS_NETWORK_CTRL);
    } else if (traffic_class) {
        buffer_priority_set(buf, QOS_HIGH);
    }

    if (!buf->adaptation_timestamp) {
        // Set TX start timestamp
        buf->adaptation_timestamp = protocol_core_monotonic_time;
        if (!buf->adaptation_timestamp) {
            buf->adaptation_timestamp--;
        }
    } else if (lowpan_adaptation_interface_check_buffer_timeout(buf)) {
        // Remove old buffers
        socket_tx_buffer_event_and_free(buf, SOCKET_TX_FAIL);
        return -1;
    }

    //Update priority status
    lowpan_adaptation_priority_status_update(cur, interface_ptr, buf->priority);

    //Check packet size
    bool fragmented_needed = lowpan_adaptation_request_longer_than_mtu(cur, buf, interface_ptr);
    if (fragmented_needed) {
        // If fragmentation TX buffer not allocated, do it now.
        if (!interface_ptr->fragment_indirect_tx_buffer && !interface_ptr->mtu_size) {
            interface_ptr->fragment_indirect_tx_buffer = ns_dyn_mem_alloc(cur->mac_api->phyMTU);
            if (interface_ptr->fragment_indirect_tx_buffer) {
                interface_ptr->mtu_size = cur->mac_api->phyMTU;
            } else {
                tr_error("Failed to allocate fragmentation buffer");
                goto tx_error_handler;
            }
        }
    }
    bool is_unicast = buf->link_specific.ieee802_15_4.requestAck;
    bool indirect = buf->link_specific.ieee802_15_4.indirectTxProcess;

    if (!lowpan_buffer_tx_allowed(interface_ptr, buf)) {

        if (random_early_detection_congestion_check(cur->random_early_detection)) {
            // If we need to drop packet we drop oldest normal Priority packet.
            buffer_t *dropped = lowpan_adaptation_get_oldest_packet(interface_ptr, QOS_NORMAL);
            if (dropped) {
                ns_list_remove(&interface_ptr->directTxQueue, dropped);
                interface_ptr->directTxQueue_size--;
                socket_tx_buffer_event_and_free(dropped, SOCKET_TX_FAIL);
                protocol_stats_update(STATS_AL_TX_CONGESTION_DROP, 1);
            }
        }
        lowpan_adaptation_tx_queue_write(cur, interface_ptr, buf);
        return 0;
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
        if (lowpan_message_fragmentation_init(buf, tx_ptr, cur, interface_ptr)) {
            tr_error("Fragment init fail");
            if (indirect) {
                ns_dyn_mem_free(tx_ptr->fragmenter_buf);
                ns_dyn_mem_free(tx_ptr);
            } else {
                tx_ptr->buf = NULL;
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
        mac_neighbor_table_entry_t *neigh_entry_ptr = mac_neighbor_table_address_discover(mac_neighbor_info(cur), buf->dst_sa.address + PAN_ID_LEN, buf->dst_sa.addr_type);
        if (neigh_entry_ptr) {
            buf->link_specific.ieee802_15_4.indirectTTL = (uint32_t) neigh_entry_ptr->link_lifetime * 1000;
        } else {
            buf->link_specific.ieee802_15_4.indirectTTL = cur->mac_parameters->mac_in_direct_entry_timeout;
        }

        tr_debug_extra("indirect seq: %d, addr=%s", tx_ptr->buf->seq, trace_ipv6(buf->dst_sa.address));

        // Make room for new message if needed */
        if (buffer_data_length(buf) <= interface_ptr->indirect_big_packet_threshold) {
            is_room_for_new_message = lowpan_adaptation_make_room_for_small_packet(cur, interface_ptr, neigh_entry_ptr, tx_ptr);
        } else {
            is_room_for_new_message = lowpan_adaptation_make_room_for_big_packet(cur, interface_ptr, tx_ptr);
        }

        if (lowpan_adaptation_indirect_mac_data_request_active(interface_ptr, tx_ptr)) {
            // mac is handling previous data request, add new one to be cached */
            tr_debug_extra("caching seq: %d", tx_ptr->buf->seq);
            tx_ptr->indirect_data_cached = true;
        }

        if (is_room_for_new_message) {
            ns_list_add_to_end(&interface_ptr->indirect_tx_queue, tx_ptr);
        } else {
            if (tx_ptr->fragmenter_buf) {
                ns_dyn_mem_free(tx_ptr->fragmenter_buf);
            }
            ns_dyn_mem_free(tx_ptr);
            goto tx_error_handler;
        }

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

    lowpan_data_request_to_mac(cur, buf, tx_ptr, interface_ptr);
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
        lowpan_list_entry_free(&interface_ptr->indirect_tx_queue, tx_ptr);
    } else if (buf->link_specific.ieee802_15_4.requestAck) {
        ns_list_remove(&interface_ptr->activeUnicastList, tx_ptr);
        ns_dyn_mem_free(tx_ptr);
        interface_ptr->activeTxList_size--;
    }

    socket_tx_buffer_event_and_free(buf, socket_event);
}


int8_t lowpan_adaptation_interface_tx_confirm(protocol_interface_info_entry_t *cur, const mcps_data_conf_t *confirm)
{
    if (!cur || !confirm) {
        return -1;
    }

    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);
    if (!interface_ptr) {
        return -1;
    }

    //Check first
    fragmenter_tx_entry_t *tx_ptr;
    bool active_direct_confirm;
    if (lowpan_active_tx_handle_verify(confirm->msduHandle, interface_ptr->active_broadcast_tx_buf.buf)) {
        active_direct_confirm = true;
        tx_ptr = &interface_ptr->active_broadcast_tx_buf;
    } else {
        tx_ptr = lowpan_listed_tx_handle_verify(confirm->msduHandle, &interface_ptr->activeUnicastList);
        if (tx_ptr) {
            active_direct_confirm = true;
        } else {
            tx_ptr = lowpan_listed_tx_handle_verify(confirm->msduHandle, &interface_ptr->indirect_tx_queue);
            if (tx_ptr) {
                active_direct_confirm = false;
            }
        }
    }

    if (!tx_ptr) {
        tr_error("No data request for this confirmation %u", confirm->msduHandle);
        return -1;
    }
    buffer_t *buf = tx_ptr->buf;

    // Update adaptation layer latency for unicast packets. Given as seconds.
    if (buf->link_specific.ieee802_15_4.requestAck && buf->adaptation_timestamp) {
        protocol_stats_update(STATS_AL_TX_LATENCY, ((protocol_core_monotonic_time - buf->adaptation_timestamp) + 5) / 10);
    }

    //Indirect data expiration
    if (confirm->status == MLME_TRANSACTION_EXPIRED && !active_direct_confirm) {
        if (buf->link_specific.ieee802_15_4.indirectTTL > 7000) {
            buf->link_specific.ieee802_15_4.indirectTTL -= 7000;
            //Push Back to MAC
            lowpan_data_request_to_mac(cur, buf, tx_ptr, interface_ptr);
            return 0;
        }
    }

    if (interface_ptr->etx_update_cb) {
        interface_ptr->etx_update_cb(cur, buf, confirm);
    }

    //Switch original channel back
    if (buf->link_specific.ieee802_15_4.rf_channel_switch) {
        mac_helper_mac_channel_set(cur, buf->link_specific.ieee802_15_4.selected_channel);
        buf->link_specific.ieee802_15_4.rf_channel_switch = false;
    }

    if (confirm->status == MLME_SUCCESS) {
        //Check is there more packets
        if (lowpan_adaptation_tx_process_ready(tx_ptr)) {
            bool triggered_from_indirect_cache = false;
            if (tx_ptr->fragmented_data && active_direct_confirm) {
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
            lowpan_data_request_to_mac(cur, buf, tx_ptr, interface_ptr);
        }
    } else if ((confirm->status == MLME_BUSY_CHAN) && !ws_info(cur)) {
        lowpan_data_request_to_mac(cur, buf, tx_ptr, interface_ptr);
    } else if ((buf->link_specific.ieee802_15_4.requestAck) && (confirm->status == MLME_TRANSACTION_EXPIRED)) {
        lowpan_adaptation_tx_queue_write_to_front(cur, interface_ptr, buf);
        ns_list_remove(&interface_ptr->activeUnicastList, tx_ptr);
        ns_dyn_mem_free(tx_ptr);
        interface_ptr->activeTxList_size--;
    } else {


        if (confirm->status == MLME_TRANSACTION_OVERFLOW) {
            tr_error("MCPS Data fail by MLME_TRANSACTION_OVERFLOW");
        }

        tr_error("MCPS Data fail by status %u", confirm->status);
        if (buf->dst_sa.addr_type == ADDR_802_15_4_SHORT) {
            tr_info("Dest addr: %x", common_read_16_bit(buf->dst_sa.address + 2));
        } else if (buf->dst_sa.addr_type == ADDR_802_15_4_LONG) {
            tr_info("Dest addr: %s", trace_array(buf->dst_sa.address + 2, 8));
        }

#ifdef HAVE_RPL
        if (confirm->status == MLME_TX_NO_ACK || confirm->status == MLME_UNAVAILABLE_KEY) {
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
    }
    // When confirmation is for direct transmission, push all allowed buffers to MAC
    if (active_direct_confirm == true) {
        //Check Possibility for exit from High Priority state
        lowpan_adaptation_high_priority_state_exit(interface_ptr);
        buffer_t *buf_from_queue = lowpan_adaptation_tx_queue_read(cur, interface_ptr);
        while (buf_from_queue) {
            lowpan_adaptation_interface_tx(cur, buf_from_queue);
            buf_from_queue = lowpan_adaptation_tx_queue_read(cur, interface_ptr);
        }
    }
    return 0;
}

static bool mac_data_is_broadcast_addr(const sockaddr_t *addr)
{
    return (addr->addr_type == ADDR_802_15_4_SHORT) &&
           (addr->address[2] == 0xFF && addr->address[3] == 0xFF);
}

static bool mcps_data_indication_neighbor_validate(protocol_interface_info_entry_t *cur, const sockaddr_t *addr)
{
    if (thread_info(cur) || ws_info(cur) || (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE)) {
        mac_neighbor_table_entry_t *neighbor = mac_neighbor_table_address_discover(mac_neighbor_info(cur), addr->address + 2, addr->addr_type);
        if (neighbor && (neighbor->connected_device ||  neighbor->trusted_device)) {
            return true;
        }

        /* Otherwise, we don't know them */
        return false;
    } else {
        //6lowpan without MLE don't can't do validation
        return true;
    }

}

void lowpan_adaptation_interface_data_ind(protocol_interface_info_entry_t *cur, const mcps_data_ind_t *data_ind)
{
    buffer_t *buf = buffer_get(data_ind->msduLength);
    if (!buf || !cur) {
        return;
    }
    uint8_t *ptr;
    buffer_data_add(buf, data_ind->msdu_ptr, data_ind->msduLength);
    //tr_debug("MAC Paylod size %u %s",data_ind->msduLength, trace_array(data_ind->msdu_ptr, 8));
    buf->options.lqi = data_ind->mpduLinkQuality;
    buf->options.dbm = data_ind->signal_dbm;
    buf->src_sa.addr_type = (addrtype_t)data_ind->SrcAddrMode;
    ptr = common_write_16_bit(data_ind->SrcPANId, buf->src_sa.address);
    memcpy(ptr, data_ind->SrcAddr, 8);
    buf->dst_sa.addr_type = (addrtype_t)data_ind->DstAddrMode;
    ptr = common_write_16_bit(data_ind->DstPANId, buf->dst_sa.address);
    memcpy(ptr, data_ind->DstAddr, 8);
    //Set Link spesific stuff to seperately
    buf->link_specific.ieee802_15_4.srcPanId = data_ind->SrcPANId;
    buf->link_specific.ieee802_15_4.dstPanId = data_ind->DstPANId;

    if (mac_data_is_broadcast_addr(&buf->dst_sa)) {
        buf->options.ll_broadcast_rx = true;
    }
    buf->interface = cur;
    if (data_ind->Key.SecurityLevel) {
        buf->link_specific.ieee802_15_4.fc_security = true;

        if (cur->mac_security_key_usage_update_cb) {
            cur->mac_security_key_usage_update_cb(cur, &data_ind->Key);
        }
    } else {
        buf->link_specific.ieee802_15_4.fc_security = false;
        if (mac_helper_default_security_level_get(cur) ||
                !mcps_data_indication_neighbor_validate(cur, &buf->src_sa)) {
            //SET By Pass
            buf->options.ll_security_bypass_rx = true;
        }
    }

    buf->info = (buffer_info_t)(B_TO_IPV6_TXRX | B_FROM_MAC | B_DIR_UP);
    protocol_push(buf);
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

    if (!interface_ptr || (!ns_list_count(&interface_ptr->activeUnicastList) && !interface_ptr->active_broadcast_tx_buf.buf)) {
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

static bool lowpan_adaptation_purge_from_mac(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr,  uint8_t msduhandle)
{
    mcps_purge_t purge_req;
    purge_req.msduHandle = msduhandle;
    bool mac_purge_success = false;
    if (interface_ptr->mpx_api) {
        if (interface_ptr->mpx_api->mpx_data_purge(interface_ptr->mpx_api, &purge_req, interface_ptr->mpx_user_id) == 0) {
            mac_purge_success = true;
        }
    } else {
        if (cur->mac_api->mcps_purge_req) {
            if (cur->mac_api->mcps_purge_req(cur->mac_api, &purge_req) == 0) {
                mac_purge_success = true;
            }
        }
    }

    return mac_purge_success;
}

static bool lowpan_adaptation_indirect_queue_free_message(struct protocol_interface_info_entry *cur, fragmenter_interface_t *interface_ptr, fragmenter_tx_entry_t *tx_ptr)
{
    tr_debug("Purge from indirect handle %u, cached %d", tx_ptr->buf->seq, tx_ptr->indirect_data_cached);
    if (tx_ptr->indirect_data_cached == false) {
        if (lowpan_adaptation_purge_from_mac(cur, interface_ptr, tx_ptr->buf->seq) == false) {
            // MAC purge failed
            return false;
        }
    }

    lowpan_adaptation_data_process_clean(interface_ptr, tx_ptr, SOCKET_TX_FAIL);

    return true;
}

void lowpan_adaptation_neigh_remove_free_tx_tables(protocol_interface_info_entry_t *cur_interface, mac_neighbor_table_entry_t *entry_ptr)
{
    //Free first by defined short address
    if (entry_ptr->mac16 < 0xfffe) {
        uint8_t temp_address[2];
        common_write_16_bit(entry_ptr->mac16, temp_address);
        lowpan_adaptation_free_messages_from_queues_by_address(cur_interface, temp_address, ADDR_802_15_4_SHORT);
    }
    lowpan_adaptation_free_messages_from_queues_by_address(cur_interface, entry_ptr->mac64, ADDR_802_15_4_LONG);
}


int8_t lowpan_adaptation_free_messages_from_queues_by_address(struct protocol_interface_info_entry *cur, uint8_t *address_ptr, addrtype_t adr_type)
{
    fragmenter_interface_t *interface_ptr = lowpan_adaptation_interface_discover(cur->id);

    if (!interface_ptr) {
        return -1;
    }

    //Check first indirect queue
    ns_list_foreach_safe(fragmenter_tx_entry_t, entry, &interface_ptr->indirect_tx_queue) {
        if (lowpan_tx_buffer_address_compare(&entry->buf->dst_sa, address_ptr, adr_type)) {
            //Purge from mac
            lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, entry);
        }
    }

    //Check next direct queue
    ns_list_foreach_safe(fragmenter_tx_entry_t, entry, &interface_ptr->activeUnicastList) {
        if (lowpan_tx_buffer_address_compare(&entry->buf->dst_sa, address_ptr, adr_type)) {
            //Purge from mac
            lowpan_adaptation_indirect_queue_free_message(cur, interface_ptr, entry);
        }
    }

    //Check next directTxQueue there may be pending packets also
    ns_list_foreach_safe(buffer_t, entry, &interface_ptr->directTxQueue) {
        if (lowpan_tx_buffer_address_compare(&entry->dst_sa, address_ptr, adr_type)) {
            ns_list_remove(&interface_ptr->directTxQueue, entry);
            interface_ptr->directTxQueue_size--;
            //Update Average QUEUE
            lowpan_adaptation_tx_queue_level_update(cur, interface_ptr);
            socket_tx_buffer_event_and_free(entry, SOCKET_TX_FAIL);
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
