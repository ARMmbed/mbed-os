/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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

#ifndef MAC_DEFINES_H_
#define MAC_DEFINES_H_

#include "eventOS_event.h"
#include "mlme.h"
#include "mac_data_buffer.h"
#include "ns_list.h"

struct cca_structure_s;
struct buffer;
struct mac_pre_build_frame;
struct mlme_key_descriptor_s;
struct arm_device_driver_list;
struct fhss_api;

typedef enum mac_event_t {
    MAC_STATE_IDLE = 0,
    MAC_TX_DONE,
    MAC_TX_DONE_PENDING,
    MAC_CCA_FAIL,
    MAC_TIMER_ACK,
    MAC_TIMER_CCA,
    MAC_TX_FAIL,
    MAC_TX_TIMEOUT,
    MAC_UNKNOWN_DESTINATION,
    MAC_TX_PRECOND_FAIL
} mac_event_t;

typedef enum mac_tx_status_type_t {
    MAC_TX_STATUS_LEN = 0,
    MAC_TX_STATUS_CCA_CNT,
    MAC_TX_STATUS_RETRY_CNT,
} mac_tx_status_type_t;


typedef enum mac_int_event_t {
    MAC_INT_IDLE = 0,
    MAC_INT_TX,
    MAC_INT_TIMER
} mac_int_event_t;


typedef enum mac_ifs_event_t {
    MAC_IFS_IDLE = 0,
    MAC_IFS_CCA,
    MAC_IFS_TRIG_TX,
    MAC_IFS_TX_ACTIVE,
    MAC_IFS_CCA_FAILED,
    MAC_IFS_ACK_TIMEOUT,
} mac_ifs_event_t;



typedef enum mac_timer_event_t {
    MAC_INT_TIMER_IDLE = 0,
    MAC_INT_TIMER_ACK,
    MAC_INT_TIMER_CCA
} mac_timer_event_t;

typedef enum {
    MAC_FRAME_BEACON =      0,
    MAC_FRAME_DATA =            1,
    MAC_FRAME_ACK =             2,
    MAC_FRAME_CMD =             3,
    MAC_ILLEGAL_LEGACY  =       5,
    MAC_FRAME_RANGING =     14,
    MAC_FRAME_NONE =    15
} mac_frame_e;

typedef enum arm_nwk_mlme_event_type {
    ARM_NWK_MAC_MLME_IDLE = 0,
    ARM_NWK_MAC_MLME_SCAN = 1,
    ARM_NWK_MAC_MLME_ED_ANALYZE = 2,
    ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL = 4,
    ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL_AFTER_DATA = 5,
} arm_nwk_mlme_event_type_e;

#define ENHANCED_ACK_MAX_LENGTH 255

typedef struct dev_driver_tx_buffer {
    uint8_t *buf;
    uint8_t *enhanced_ack_buf;
    uint16_t ack_len;
    uint16_t len;
    unsigned priority: 2;
} dev_driver_tx_buffer_s;

/*
 *  Table 82-MAC command frames
 *
 *                      Command     Command name                    RFD       Subclause
 *                                                                Tx     Rx             */
#define MAC_ASSOC_REQ       0x01 // Association request           X           7.3.1
#define MAC_ASSOC_RESP      0x02 // Association response                 X    7.3.2
#define MAC_DISASSOC_NOTIFY 0x03 // Disassociation notification   X      X    7.3.3
#define MAC_DATA_REQ        0x04 // Data request                  X           7.3.4
#define MAC_PAN_CONFLICT    0x05 // PAN ID conflict notification  X           7.3.5
#define MAC_ORPHAN          0x06 // Orphan notification           X           7.3.6
#define MAC_BEACON_REQ      0x07 // Beacon request                            7.3.7
#define MAC_COORD_REALIGN   0x08 // Coordinator realignment              X    7.3.8
#define MAC_GTS_REQ         0x09 // GTS request                               7.3.9
//                          0x0a-0xff   Reserved

#define MAC_CCA_MAX 8
#define MAC_DEF_MIN_BE 3
#define MAC_DEF_MAX_BE 5

typedef struct mac_active_scan {
    uint8_t pan_id[2];
    uint8_t address[8];
    uint8_t lqi;
    uint8_t beacon_scan_ok;
} mac_active_scan;

#define MAC_EXT_ADDLIST_SIZE 8
typedef struct mac_extented_address_table_t {
    uint8_t short_addr[2];
    uint8_t addr[8];
} mac_extented_address_table_t;

typedef struct mac_tx_status_t {
    uint16_t length;
    uint8_t cca_cnt;
    uint8_t retry;
} mac_tx_status_t;

typedef struct mac_mcps_data_conf_fail_s {
    uint8_t msduHandle;     /**< Handle associated with MSDU */
    uint8_t status;         /**< Status of the failing MSDU transmission */
} mac_mcps_data_conf_fail_t;

typedef struct protocol_interface_rf_mac_setup {
    int8_t mac_interface_id;
    bool macUpState: 1;
    bool shortAdressValid: 1;   //Define Dynamic src address to mac16 when it is true
    bool beaconSrcAddressModeLong: 1; //This force beacon src to mac64 otherwise shortAdressValid will define type
    bool secFrameCounterPerKey: 1;
    bool mac_extension_enabled: 1;
    bool mac_ack_tx_active: 1;
    bool mac_frame_pending: 1;
    /* MAC Capability Information */
    bool macCapRxOnIdle: 1;
    bool macCapCordinator: 1;
    bool macCapAssocationPermit: 1;
    bool macCapBatteryPowered: 1;
    bool macCapSecrutityCapability: 1;
    bool macProminousMode: 1;
    bool macGTSPermit: 1;
    bool mac_security_enabled: 1;
    /* Let trough packet which is secured properly (MIC authenticated group key)  and src address is 64-bit*/
    bool mac_security_bypass_unknow_device: 1;
    /* Load balancing need this feature */
    bool macAcceptAnyBeacon: 1;
    /* TX process Flag */
    bool macTxProcessActive: 1;
    bool macTxRequestAck: 1;
    /* Data Poll state's */
    bool macDataPollReq: 1;
    bool macWaitingData: 1;
    bool macRxDataAtPoll: 1;
    /* Radio State flags */
    bool macRfRadioOn: 1;
    bool macRfRadioTxActive: 1;
    bool macBroadcastDisabled: 1;
    bool scan_active: 1;
    bool rf_csma_extension_supported: 1;
    bool ack_tx_possible: 1;
    uint16_t mac_short_address;
    uint16_t pan_id;
    uint8_t mac64[8];
    uint16_t coord_short_address;
    uint8_t coord_long_address[8];

    /* CSMA Params */
    unsigned macMinBE: 4;
    unsigned macMaxBE: 4;
    unsigned macCurrentBE: 4;
    uint8_t macMaxCSMABackoffs;
    uint8_t backoff_period_in_10us; // max 2550us - it's 320us for standard 250kbps
    uint8_t mac_frame_filters;
    /* MAC channel parameters */
    channel_list_s mac_channel_list;
    uint8_t scan_duration; //Needed???
    mac_scan_type_t scan_type;

    uint8_t mac_channel;
    //uint8_t cca_failure;

    /* MAC TX Queue */
    uint16_t direct_queue_bytes;
    uint16_t unicast_queue_size;
    uint16_t broadcast_queue_size;
    struct mac_pre_build_frame *pd_data_request_queue_to_go;
    struct mac_pre_build_frame *pd_data_request_bc_queue_to_go;
    struct mac_pre_build_frame *active_pd_data_request;
    /* MAC Beacon info */
    uint16_t max_beacon_payload_length;
    uint8_t *mac_beacon_payload;
    uint8_t mac_beacon_payload_size;
    uint8_t mac_bea_sequence;
    uint8_t mac_sequence;
    uint8_t mac_tx_retry;
    uint8_t mac_cca_retry;
    uint16_t mac_ack_wait_duration;
    uint8_t mac_mlme_retry_max;
    uint8_t aUnitBackoffPeriod;
    uint8_t number_of_csma_ca_periods;  /**< Number of CSMA-CA periods */
    uint16_t multi_cca_interval;        /**< Length of the additional CSMA-CA period(s) in microseconds */
    /* Indirect queue parameters */
    struct mac_pre_build_frame *indirect_pd_data_request_queue;
    struct mac_pre_build_frame enhanced_ack_buffer;
    uint32_t enhanced_ack_handler_timestamp;
    arm_event_t mac_mcps_timer_event;
    uint16_t indirect_pending_bytes;
    arm_nwk_mlme_event_type_e mac_mlme_event;
    mac_event_t timer_mac_event;
    mac_event_t mac_tx_result;
    uint16_t active_mac_events;
    int8_t mac_tasklet_id;
    int8_t mac_mcps_timer;
    int8_t mac_timer_id;
    int8_t ifs_timer_id;
    int8_t mlme_timer_id;
    int8_t cca_timer_id;
    int8_t bc_timer_id;
    uint32_t mlme_tick_count;
    uint32_t symbol_rate;
    uint32_t symbol_time_us;
    uint8_t max_ED;
    uint16_t mlme_ED_counter;
    mac_tx_status_t mac_tx_status;
    mac_mcps_data_conf_fail_t mac_mcps_data_conf_fail;
    struct cca_structure_s *cca_structure;
    /* MAC Security components */
    struct mlme_device_descriptor_s *device_description_table;
    uint8_t device_description_table_size;
    struct mlme_key_descriptor_s *key_description_table;
    void *key_device_frame_counter_list_buffer;
    uint8_t key_description_table_size;
    uint8_t key_lookup_list_size;
    uint8_t key_usage_list_size;
    mlme_key_device_descriptor_t *key_device_desc_buffer;
    mlme_key_usage_descriptor_t *key_usage_list_buffer;
    mlme_key_id_lookup_descriptor_t *key_lookup_buffer;
    struct mlme_security_level_descriptor_s *security_level_descriptor_table;
    uint8_t security_level_descriptor_table_size;
    uint32_t security_frame_counter;
    uint8_t mac_default_key_source[8];
    mlme_security_t mac_auto_request;
    /* end of security part */
    //Device driver and buffer
    struct arm_device_driver_list *dev_driver;
    dev_driver_tx_buffer_s dev_driver_tx_buffer;
    struct arm_device_driver_list *tun_extension_rf_driver;
    /* End of API Control */
    struct mlme_scan_conf_s *mac_mlme_scan_resp;
    //beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr;
    struct mac_statistics_s *mac_statistics;
    /* FHSS API*/
    struct fhss_api *fhss_api;
} protocol_interface_rf_mac_setup_s;


#define MAC_FCF_FRAME_TYPE_MASK             0x0007
#define MAC_FCF_FRAME_TYPE_SHIFT            0
#define MAC_FCF_SECURITY_BIT_MASK           0x0008
#define MAC_FCF_SECURITY_BIT_SHIFT          3
#define MAC_FCF_PENDING_BIT_MASK            0x0010
#define MAC_FCF_PENDING_BIT_SHIFT           4
#define MAC_FCF_ACK_REQ_BIT_MASK            0x0020
#define MAC_FCF_ACK_REQ_BIT_SHIFT           5
#define MAC_FCF_INTRA_PANID_MASK            0x0040
#define MAC_FCF_INTRA_PANID_SHIFT           6
#define MAC_FCF_SEQ_NUM_SUPPRESS_MASK       0x0100
#define MAC_FCF_SEQ_NUM_SUPPRESS_SHIFT      8
#define MAC_FCF_IE_PRESENTS_MASK            0x0200
#define MAC_FCF_IE_PRESENTS_SHIFT           9
#define MAC_FCF_DST_ADDR_MASK               0x0c00
#define MAC_FCF_DST_ADDR_SHIFT              10
#define MAC_FCF_VERSION_MASK                0x3000
#define MAC_FCF_VERSION_SHIFT               12
#define MAC_FCF_SRC_ADDR_MASK               0xc000
#define MAC_FCF_SRC_ADDR_SHIFT              14

/* MAC supported frame types */
#define FC_BEACON_FRAME         0x00
#define FC_DATA_FRAME           0x01
#define FC_ACK_FRAME            0x02
#define FC_CMD_FRAME            0x03

/* MAC virtual configuration types*/
#define MAC_BROADCAST_EVENT 0

#define MAC_DATA_PACKET_MIN_HEADER_LENGTH 11

#define MAC_DEFAULT_MAX_FRAME_RETRIES 3

// TODO: make this dynamic
#define FHSS_SYNCH_INFO_START   20

#endif /* MAC_DEFINES_H_ */
