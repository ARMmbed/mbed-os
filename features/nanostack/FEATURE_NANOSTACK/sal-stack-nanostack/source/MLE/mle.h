/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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

#ifndef MLE_H_
#define MLE_H_

#include "nsconfig.h"
#include "Core/include/address.h"
#include "ns_list.h"

struct buffer;

#ifndef MAX_MLE_INFO_CNT
#ifdef CORTEXM3_STM32W108CC
#define MAX_MLE_INFO_CNT 40
#else
#define MAX_MLE_INFO_CNT 256
#endif
#endif

#define MLE_MAX_ROUTERS 64
/* Route option layout: 1 Sequence byte, ID mask (bit per router), Data (byte per valid ID) */
#define MLE_ROUTE_ID_MASK_SIZE   ((MLE_MAX_ROUTERS+7)/8)
#define MLE_ROUTE_MAX_DATA_SIZE  MLE_MAX_ROUTERS
#define MLE_ROUTE_MIN_OPTION_LEN (1 + MLE_ROUTE_ID_MASK_SIZE)

/* MLE messge types */
#define MLE_COMMAND_REQUEST                 0
#define MLE_COMMAND_ACCEPT                  1
#define MLE_COMMAND_ACCEPT_AND_REQUEST      2
#define MLE_COMMAND_REJECT                  3
#define MLE_COMMAND_ADVERTISEMENT           4
#define MLE_COMMAND_UPDATE                  5
#define MLE_COMMAND_UPDATE_REQUEST          6
/* New messages for Thread */
#define MLE_COMMAND_DATA_REQUEST            7
#define MLE_COMMAND_DATA_RESPONSE           8
#define MLE_COMMAND_PARENT_REQUEST          9  //Old MLE attache message number 1
#define MLE_COMMAND_PARENT_RESPONSE        10  //Old MLE attache message number 2
#define MLE_COMMAND_CHILD_ID_REQUEST       11  //Old MLE attache message number 3
#define MLE_COMMAND_CHILD_ID_RESPONSE      12  //Old MLE attache message number 4
#define MLE_COMMAND_CHILD_UPDATE_REQUEST   13
#define MLE_COMMAND_CHILD_UPDATE_RESPONSE  14
#define MLE_COMMAND_DATASET_ANNOUNCE       15
#define MLE_COMMAND_DISCOVERY_REQUEST      16
#define MLE_COMMAND_DISCOVERY_RESPONSE     17
#define MLE_COMMAND_METRIC_MANAGEMENT_REQUEST            18 // Experimental for BH
#define MLE_COMMAND_METRIC_MANAGEMENT_RESPONSE           19 // Experimental for BH
#define MLE_COMMAND_PROBE                                20 // Experimental for BH

#define MLE_UPDATE_CHANNEL          0
#define MLE_UPDATE_PAN_ID             1
#define MLE_UPDATE_PERMIT_MSG         2
#define MLE_UPDATE_BEACON_PAYLOAD   3

#define MLE_UPDATE_CHANNEL_FLAG          (1 << 0)
#define MLE_UPDATE_PAN_ID_FLAG           (1 << 1)
#define MLE_UPDATE_PERMIT_MSG_FLAG       (1 << 2)
#define MLE_UPDATE_BEACON_PAYLOAD_FLAG (1 << 3)

#define MLE_UPDATE_PROCESS_TRIG         0x80

#define MLE_ERROR_NO_ERROR            0
#define MLE_ERROR_BUFFER_ERROR        1
#define MLE_ERROR_ADDRESS_ERROR       2
#define MLE_ERROR_WRONG_TYPE          3
#define MLE_ERROR_ALLOCATION_ERROR    4
#define MLE_ERROR_DATA_TOO_LONG       5
#define MLE_ERROR_UNSUPPORTED_TYPE    6
#define MLE_ERROR_RESPONSE_NOT_WAITED 7

#define MLE_CHALLENGE_SIZE 4
#define MLE_RESPONSE_SIZE  32
#define MLE_MAX_SIZE       116

/* MLE TLV types */
typedef enum mle_tlv_type_t_ {
    /* draft-kelsey-intarea-mesh-link-establishment-06 */
    MLE_TYPE_SRC_ADDRESS = 0,
    MLE_TYPE_MODE = 1,
    MLE_TYPE_TIMEOUT = 2,
    MLE_TYPE_CHALLENGE = 3,
    MLE_TYPE_RESPONSE = 4,
    MLE_TYPE_LL_FRAME_COUNTER = 5,
    MLE_TYPE_LINK_QUALITY = 6,
    MLE_TYPE_NWK_PARAM = 7,
    MLE_TYPE_MLE_FRAME_COUNTER = 8,
    /* Thread MLE Extensions (draft-kelsey-thread-mle-01) */
    MLE_TYPE_ROUTE = 9,
    MLE_TYPE_ADDRESS16 = 10,
    MLE_TYPE_LEADER_DATA = 11,
    MLE_TYPE_NETWORK_DATA = 12,
    MLE_TYPE_TLV_REQUEST = 13,
    MLE_TYPE_SCAN_MASK = 14,
    MLE_TYPE_CONNECTIVITY = 15,
    MLE_TYPE_RSSI = 16,
    MLE_TYPE_STATUS = 17,
    MLE_TYPE_VERSION = 18,
    MLE_TYPE_ADDRESS_REGISTRATION = 19,
    MLE_TYPE_CHANNEL = 20,
    MLE_TYPE_PANID = 21,
    MLE_TYPE_ACTIVE_TIMESTAMP = 22,
    MLE_TYPE_PENDING_TIMESTAMP = 23,
    MLE_TYPE_OPERATIONAL_DATASET = 24,
    MLE_TYPE_PENDING_OPERATIONAL_DATASET = 25,
    MLE_TYPE_DISCOVERY = 26,
	/* Thread low power TLVs*/
    MLE_TYPE_LINK_METRICS_QUERY = 87,
    MLE_TYPE_LINK_METRICS_MANAGEMENT = 88,
    MLE_TYPE_LINK_METRICS_REPORT = 89,
    MLE_TYPE_UNASSIGNED = 0xFF
} mle_tlv_type_t;

#define MLE_STATUS_ERROR 1
/* MLE Mode = IEEE 802.15.4 Capability Information */
#define MLE_DEV_MASK 2
#define MLE_FFD_DEV 2
#define MLE_RFD_DEV 0
#define MLE_RX_ON_IDLE 8
/** Thead Spesific ModeFlags */
#define MLE_THREAD_SECURED_DATA_REQUEST 0x04
#define MLE_THREAD_REQ_FULL_DATA_SET 0x01

#define MLE_TIMER_TICKS_SECONDS     4
#define MLE_TIMER_TICKS_MS (MLE_TIMER_TICKS_SECONDS*1000)

#define MLE_TABLE_CHALLENGE_TIMER       3

#define MLE_NEIGHBOR_PRIORITY_LINK (1 << 5)
#define MLE_NEIGHBOR_OUTGOING_LINK (1 << 6)
#define MLE_NEIGHBOR_INCOMING_LINK (1 << 7)

typedef struct mle_neigh_table_entry_t {
    uint8_t                             attribute_index;
    uint16_t                           ttl;                       /*!< destination TTL * 4 seconds */
    uint32_t                            last_contact_time;        /*!< monotonic time - hard to define "contact"; used for Thread Leasequery replies */
    uint32_t                           mle_frame_counter;
    uint16_t                           timeout_rx;
    uint16_t                            holdTime;
    uint16_t                            link_margin;
    uint8_t                            mac64[8];                  /*!< MAC64 */
    uint8_t                            mlEid[8];
    uint16_t                           short_adr;
    uint8_t                            mode;
    uint16_t                           etx;                       /*!< 12 bits fraction */
    uint16_t                           stored_diff_etx;           /*!< 12 bits fraction */
    uint8_t                            remote_incoming_idr;       /*!< 5 bits fraction */
    uint32_t                           last_key_sequence;
    unsigned                           accumulated_failures: 5;
    unsigned                           new_key_pending:1;
    unsigned                           link_q_adv_sent: 1;
    unsigned                           tmp_etx: 1;
    unsigned                           priority_child_flag: 1;    /* Is using our node as preferred parent */
    unsigned                           second_priority_flag: 1;   /* Is secondary parent */
    unsigned                            thread_commission: 1;
    unsigned                           threadNeighbor: 1;
    unsigned                            priorityFlag: 1;
    unsigned                            handshakeReady: 1;
    unsigned                            medium_ttl_challenge: 1;
    unsigned                            linkIdr: 4;
    ns_list_link_t                     link;
} mle_neigh_table_entry_t ;

typedef NS_LIST_HEAD(mle_neigh_table_entry_t, link) mle_neigh_table_list_t;


/* MLE TLV types */
typedef enum mle_class_user_mode {
    MLE_CLASS_ROUTER = 0,
    MLE_CLASS_END_DEVICE,
    MLE_CLASS_SLEEPY_END_DEVICE,
} mle_class_user_mode;

//MLE table class function pointer types
/**
 * Remove entry notify
 */
typedef void mle_entry_user_entry_remove_notify(int8_t interface_id, mle_neigh_table_entry_t *entry_ptr);

typedef int8_t mle_entry_link_keep_alive(int8_t interface_id, const uint8_t *mac64);

typedef bool mle_entry_interface_activate(int8_t interface_id);

int8_t mle_class_init(int8_t interface_id, uint8_t table_size, mle_entry_user_entry_remove_notify *remove_cb, mle_entry_link_keep_alive *keep_alive_cb, mle_entry_interface_activate *interface_is_active);

int8_t mle_class_router_challenge(int8_t interface_id,mle_entry_link_keep_alive *challenge_cb);

bool mle_class_exists_for_interface(int8_t interface_id);

int8_t mle_class_deallocate(int8_t interface_id);

int8_t mle_class_list_clean(int8_t interface_id);

int8_t mle_class_mode_set(int8_t interface_id,mle_class_user_mode mode);

int8_t mle_class_set_new_key_pending(int8_t interface_id);

int16_t mle_class_free_entry_count_get(int8_t interface_id);

mle_neigh_table_entry_t *mle_class_get_entry_by_ll64(int8_t interface_id, uint8_t linkMargin, const uint8_t *ipv6Address, bool allocateNew);

mle_neigh_table_entry_t *mle_class_discover_entry_by_ll64(int8_t interface_id, const uint8_t *ipv6Address);

mle_neigh_table_entry_t *mle_class_get_entry_by_mac64(int8_t interface_id, uint8_t linkMargin, const uint8_t *mac64, bool allocateNew);

mle_neigh_table_entry_t *mle_class_get_by_link_address(int8_t interface_id, const uint8_t *address, addrtype_t type);

mle_neigh_table_entry_t *mle_class_get_by_device_attribute_id(int8_t interface_id, uint8_t attribute_index);

int8_t mle_class_remove_entry(int8_t interface_id, mle_neigh_table_entry_t *entry);

int8_t mle_class_remove_neighbour(int8_t interface_id, const uint8_t *address, addrtype_t type);

mle_neigh_table_list_t *mle_class_active_list_get(int8_t interface_id);

int16_t mle_class_sleepy_entry_count_get(int8_t interface_id);
int16_t mle_class_rfd_entry_count_get(int8_t interface_id);

uint16_t mle_class_active_neigh_counter(int8_t interface_id);

/**
 *  Function to refresh Neigh Entry when get some trusted response or spesified place
 */
mle_neigh_table_entry_t *mle_refresh_entry_timeout(int8_t interfaceId, const uint8_t *addressPtr, addrtype_t addressType, bool dataPollConfirmation);

bool mle_neigh_entry_frame_counter_update(mle_neigh_table_entry_t *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, struct protocol_interface_info_entry *cur, uint8_t key_id);

uint8_t *mle_general_write_source_address(uint8_t *ptr, struct protocol_interface_info_entry *cur);

uint8_t *mle_general_write_link_layer_framecounter(uint8_t *ptr, struct protocol_interface_info_entry *cur);
void mle_entry_timeout_update(mle_neigh_table_entry_t *entry_temp, uint32_t timeout_tlv);
void mle_entry_timeout_refresh(mle_neigh_table_entry_t *entry_temp);

#endif /* MLE_H_ */
