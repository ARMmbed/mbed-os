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

/*
 * mac_mcps_sap.h
 *
 *  Created on: 19 Jan 2016
 *      Author: juhhei01
 */

#ifndef MAC_IEEE802_15_4_MAC_MCPS_SAP_H_
#define MAC_IEEE802_15_4_MAC_MCPS_SAP_H_

#include "mlme.h"
#include "mac_common_defines.h"

struct protocol_interface_rf_mac_setup;
struct mcps_data_req_s;
struct arm_phy_sap_msg_s;
struct mcps_purge_s;

/** Address types */
typedef enum {
    MAC_FRAME_REQ, /*!< MAC upper layer data request message */
    MAC_PHY_RAW_REQ,
    MAC_PARSED_FRAME_IND, /*!< PD-SAP parsed MAC Frame */
}mac_mcps_primitiv_type;


#define MAC_PD_DATA_NORMAL_PRIORITY 0    //Normal MCPS DATA REQ
#define MAC_PD_DATA_MEDIUM_PRIORITY 1    //Indirect Data which is polled
#define MAC_PD_DATA_HIGH_PRIOTITY   2    //Beacon request Beacon response

#define MCPS_SAP_DATA_IND_EVENT 1
#define MCPS_SAP_DATA_CNF_EVENT 2
#define MAC_MLME_EVENT_HANDLER  3
#define MAC_MCPS_INDIRECT_TIMER_CB 4
#define MAC_MLME_SCAN_CONFIRM_HANDLER 5
#define MAC_SAP_TRIG_TX 6


/**
 * @brief struct mac_aux_security_header_t MAC auxiliarity security header structure
 * INTERNAL use only
 */
typedef struct mac_aux_security_header_s {
    unsigned securityLevel:3;
    unsigned KeyIdMode:2;
    uint32_t frameCounter;
    uint8_t Keysource[8];
    uint8_t KeyIndex;
} mac_aux_security_header_t;

typedef struct mac_fcf_sequence_s{
    unsigned frametype:3;
    bool securityEnabled:1;
    bool framePending :1;
    bool ackRequested:1;
    bool intraPan:1;
    unsigned DstAddrMode:2; /*0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    unsigned frameVersion:2;
    unsigned SrcAddrMode:2; /*0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    uint8_t DSN;
}mac_fcf_sequence_t;

typedef struct mac_pre_parsed_frame_s{
    void *mac_class_ptr;
    mlme_device_descriptor_t *neigh_info;
    uint8_t LQI;
    int8_t dbm;
    mac_fcf_sequence_t fcf_dsn;
    uint16_t frameLength; //Encoded or open payload length
    uint8_t mac_header_length;
    uint8_t security_aux_header_length;
    uint16_t mac_payload_length;
    uint32_t timestamp;
    bool ack_pendinfg_status;
    uint8_t buf[]; /*!< Trailing buffer data */
} mac_pre_parsed_frame_t;

typedef struct mac_pre_build_frame{
    mac_fcf_sequence_t fcf_dsn;
    uint16_t DstPANId;
    uint8_t DstAddr[8];
    uint16_t SrcPANId;
    uint8_t SrcAddr[8];
    mac_aux_security_header_t aux_header;
    uint8_t mac_command_id; //For MLME
    uint16_t mac_payload_length;
    uint8_t msduHandle;
    uint16_t buffer_ttl;
    uint8_t *mac_payload;
    uint8_t status;
    bool upper_layer_request;
    bool mac_allocated_payload_ptr:1;
    unsigned mac_header_length_with_security : 6; //Total max is 37
    unsigned security_mic_len:5;    //Max possible lengths 0, 4, 8, 16 bytes
    unsigned priority:2;
    struct mac_pre_build_frame *next; //Pointer for queue purpose
} mac_pre_build_frame_t;


void mac_generic_event_trig(uint8_t event_type, void *mac_ptr, bool low_latency);

/**
 * Create MAC MCPS event handler tasklet
 */
int8_t mac_mcps_sap_tasklet_init(void);

/**
 * Free MAC MCPS SAP layer pre parsed buffer
 */
void mcps_sap_pre_parsed_frame_buffer_free(mac_pre_parsed_frame_t *buf);


/**
 * Allocate MCPS SAP data request buffer frame
 *
 * \param  payload_size set 0 when MCPS-DATA-REQUSEST other wise MAC internal packet send need to set dynamic payload size!!
 */
mac_pre_build_frame_t * mcps_sap_prebuild_frame_buffer_get(uint16_t payload_size);

/**
 * Free MCPS SAP data request buffer frame
 *
 * \param  buffer pointer to allocated frame
 */
void mcps_sap_prebuild_frame_buffer_free(mac_pre_build_frame_t *buffer);

void mcps_sap_pd_req_queue_write(struct protocol_interface_rf_mac_setup *rf_mac_setup, mac_pre_build_frame_t *buffer);


/**
 * Allocate MAC MCPS SAP layer buffer for received PHY layer Data Indication
 */
mac_pre_parsed_frame_t * mcps_sap_pre_parsed_frame_buffer_get(const uint8_t *data_ptr, uint16_t frame_length);

/**
 * Forward Buffer for MAC MCPS SAP layer event handler
 */
int8_t mcps_sap_pd_ind(mac_pre_parsed_frame_t *buffer);

/**
 * MAC MCPS SAP layer data confirmation event trig
 */
void mcps_sap_pd_confirm(void *mac_ptr);

int8_t mac_virtual_sap_data_cb(void *identifier, struct arm_phy_sap_msg_s *message);

void mcps_sap_data_req_handler(struct protocol_interface_rf_mac_setup *rf_mac_setup , const struct mcps_data_req_s *data_req );

void mac_mcps_trig_buffer_from_queue(struct protocol_interface_rf_mac_setup *rf_mac_setup);

void mac_mcps_buffer_queue_free(struct protocol_interface_rf_mac_setup *rf_mac_setup);

bool mac_is_ack_request_set(mac_pre_build_frame_t *buffer);

int mac_convert_frame_type_to_fhss(uint8_t frame_type);

void mcps_sap_trig_tx(void *mac_ptr);

void mcps_sap_purge_reg_handler(struct protocol_interface_rf_mac_setup *rf_mac_setup, const struct mcps_purge_s *purge_req);

#endif /* MAC_IEEE802_15_4_MAC_MCPS_SAP_H_ */
