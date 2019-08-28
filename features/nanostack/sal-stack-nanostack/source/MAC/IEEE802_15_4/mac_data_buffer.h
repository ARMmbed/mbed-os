/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef MAC_DATA_BUFFER_H_
#define MAC_DATA_BUFFER_H_

#include "mac_mcps.h"

/**
 * @brief struct mac_aux_security_header_t MAC auxiliarity security header structure
 * INTERNAL use only
 */
typedef struct mac_aux_security_header_s {
    uint32_t frameCounter;
    uint8_t Keysource[8];
    uint8_t KeyIndex;
    unsigned securityLevel: 3;
    unsigned KeyIdMode: 2;
} mac_aux_security_header_t;

typedef struct mac_fcf_sequence_s {
    unsigned frametype: 3;
    bool securityEnabled: 1;
    bool framePending : 1;
    bool ackRequested: 1;
    bool intraPan: 1;
    bool sequenceNumberSuppress: 1;
    bool informationElementsPresets: 1;
    bool DstPanPresents: 1;
    bool SrcPanPresents: 1;
    unsigned DstAddrMode: 2; /*0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    unsigned frameVersion: 2;
    unsigned SrcAddrMode: 2; /*0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    uint8_t DSN;
} mac_fcf_sequence_t;

typedef struct mac_pre_parsed_frame_s {
    void *mac_class_ptr;
    uint8_t *payloadsIePtr;
    uint8_t *headerIePtr;
    uint8_t *macPayloadPtr;
    mlme_device_descriptor_t *neigh_info;
    uint32_t timestamp;
    mac_fcf_sequence_t fcf_dsn;
    uint16_t frameLength; //Encoded or open payload length
    uint16_t payloadsIeLength;
    uint16_t headerIeLength;
    uint16_t mac_header_length;
    uint16_t header_ie_length;
    uint16_t mac_payload_length;
    uint8_t security_aux_header_length;
    uint8_t LQI;
    int8_t dbm;
    bool ack_pendinfg_status;
    uint8_t buf[]; /*!< Trailing buffer data */
} mac_pre_parsed_frame_t;

typedef struct mac_pre_build_frame {
    mac_fcf_sequence_t fcf_dsn;
    uint16_t DstPANId;
    uint8_t DstAddr[8];
    uint16_t SrcPANId;
    uint8_t SrcAddr[8];
    mac_aux_security_header_t aux_header;
    uint8_t mac_command_id; //For MLME
    uint16_t payloadsIeLength;
    uint16_t headerIeLength;
    uint16_t mac_payload_length;
    uint16_t mac_header_length_with_security;
    uint8_t msduHandle;
    uint16_t buffer_ttl;
    struct mcps_data_req_ie_list ie_elements;
    struct channel_list_s asynch_channel_list;
    uint8_t *mac_payload;
    uint8_t status;
    uint8_t asynch_channel;
    uint8_t csma_periods_left;
    uint8_t fhss_retry_count;
    uint8_t fhss_cca_retry_count;
    uint32_t tx_time;
    bool upper_layer_request: 1;
    bool mac_allocated_payload_ptr: 1;
    bool asynch_request: 1;
    bool message_builded: 1;
    unsigned security_mic_len: 5;   //Max possible lengths 0, 4, 8, 16 bytes
    unsigned priority: 2;
    struct mac_pre_build_frame *next; //Pointer for queue purpose
} mac_pre_build_frame_t;


#endif /* MAC_DATA_BUFFER_H_ */
