/*
 * Copyright (c) 2013-2019, Arm Limited and affiliates.
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
#include "Core/include/ns_address_internal.h"
#include "ns_list.h"

struct buffer;
struct mac_neighbor_table_entry;

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

#define MLE_TABLE_CHALLENGE_TIMER       12

#define MLE_NEIGHBOR_PRIORITY_LINK (1 << 5)
#define MLE_NEIGHBOR_OUTGOING_LINK (1 << 6)
#define MLE_NEIGHBOR_INCOMING_LINK (1 << 7)


int8_t mle_class_set_new_key_pending(struct protocol_interface_info_entry *cur);
int16_t mle_class_free_entry_count_get(struct protocol_interface_info_entry *cur);
int16_t mle_class_sleepy_entry_count_get(struct protocol_interface_info_entry *cur);
int16_t mle_class_rfd_entry_count_get(struct protocol_interface_info_entry *cur);
uint16_t mle_class_active_neigh_counter(struct protocol_interface_info_entry *cur);
bool mle_neigh_entry_frame_counter_update(struct mac_neighbor_table_entry *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, struct protocol_interface_info_entry *cur, uint8_t key_id);
uint8_t *mle_general_write_source_address(uint8_t *ptr, struct protocol_interface_info_entry *cur);

uint8_t *mle_general_write_link_layer_framecounter(uint8_t *ptr, struct protocol_interface_info_entry *cur);

void mle_mode_parse_to_mac_entry(struct mac_neighbor_table_entry *mac_entry, uint8_t mode);
uint8_t  mle_mode_write_from_mac_entry(struct mac_neighbor_table_entry *mac_entry);

#endif /* MLE_H_ */
