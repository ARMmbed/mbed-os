/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/** \file mlme.h
 * \brief MLME API
 */

#ifndef MLME_H_
#define MLME_H_

#include <stdbool.h>
#include "mac_common_defines.h"
#include "net_interface.h"

/**
 * @brief struct mlme_pan_descriptor_t PAN descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 55) for more details
 */
typedef struct mlme_pan_descriptor_s {
    unsigned CoordAddrMode:2;
    uint16_t CoordPANId;
    uint8_t CoordAddress[8];
    uint8_t LogicalChannel;
    uint8_t ChannelPage;
    uint8_t SuperframeSpec[2];
    bool GTSPermit:1;
    uint8_t LinkQuality;
    uint32_t Timestamp;
    uint8_t SecurityFailure;
    mlme_security_t Key;
} mlme_pan_descriptor_t;

/**
 * @brief struct mlme_command_type_t Command type enumeration
 *
 * See IEEE standard 802.15.4-2006 (table 82) for more details
 */
typedef enum {
    ASSOCIATION_REQUEST = 1,
    ASSOCIATION_RESPONSE = 2,
    DISASSOCIATION_NOTIFICATION = 3,
    DATA_REQUEST = 4,
    PAN_ID_CONFLICT_NOTIFICATION = 5,
    ORPHAN_NOTIFICATION = 6,
    BEACON_REQUEST = 7,
    COORDINATOR_REALIGNMENT = 8,
    GTS_REQUEST = 9
    //Reserved
} mlme_command_type_t;

/**
 * @brief struct mlme_key_usage_descriptor_t Key usage descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 90) for more details
 */
typedef struct mlme_key_usage_descriptor_s {
    unsigned FrameType:3; //See mlme_frame_types
    unsigned CommandFrameIdentifier:4; //See mlme_command_type_t
} mlme_key_usage_descriptor_t;

/**
 * @brief struct mlme_key_device_descriptor_t Key usage descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 91) for more details
 */
typedef struct mlme_key_device_descriptor_s {
    uint8_t DeviceDescriptorHandle; //Index of an entry in macDeviceTable
    bool UniqueDevice:1;
    bool Blacklisted:1;
} mlme_key_device_descriptor_t;

/**
 * @brief enum mlme_security_type_t Security type enumeration
 *
 * See IEEE standard 802.15.4-2006 (table 95) for more details
 */
typedef enum {
    SEC_NONE = 0,
    SEC_MIC32 = 1,
    SEC_MIC64 = 2,
    SEC_MIC128 = 3,
    SEC_ENC = 4,
    SEC_ENC_MIC32 = 5,
    SEC_ENC_MIC64 = 6,
    SEC_ENC_MIC128 = 7
} mlme_security_type_t;

/**
 * @brief struct mlme_security_level_descriptor_t Security level descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 92) for more details
 */
typedef struct mlme_security_level_descriptor_s {
    unsigned FrameType:3; //See mlme_frame_types
    unsigned CommandFrameIdentifier:4; //See mlme_command_type_t
    unsigned SecurityMinimum:3; //See mlme_security_type_t
    bool DeviceOverrideSecurityMinimum:1;
} mlme_security_level_descriptor_t;

/**
 * @brief struct mlme_device_descriptor_t Device descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 93) for more details
 */
typedef struct mlme_device_descriptor_s {
    uint16_t PANId;
    uint16_t ShortAddress;
    uint8_t ExtAddress[8];
    uint32_t FrameCounter;
    bool Exempt:1;
} mlme_device_descriptor_t;

/**
 * @brief struct mlme_key_id_lookup_descriptor_t Key id lookup descriptor
 *
 * See IEEE standard 802.15.4-2006 (table 94) for more details
 */
typedef struct mlme_key_id_lookup_descriptor_s {
    uint8_t LookupData[9];
    unsigned LookupDataSize:1; //0 == 5, 1 == 9
} mlme_key_id_lookup_descriptor_t;


/**
 * @brief struct mlme_key_descriptor_entry_t Key descriptor entry
 *
 * See IEEE standard 802.15.4-2006 (table 89) for more details
 */
typedef struct mlme_key_descriptor_entry_s {
    mlme_key_id_lookup_descriptor_t *KeyIdLookupList;
    uint8_t KeyIdLookupListEntries;
    mlme_key_device_descriptor_t *KeyDeviceList;
    uint8_t KeyDeviceListEntries;
    mlme_key_usage_descriptor_t *KeyUsageList;
    uint8_t KeyUsageListEntries;
    uint8_t Key[16];
}mlme_key_descriptor_entry_t;

/**
 * @brief MLME primitive error statuses
 *
 * See IEEE standard 802.15.4-2006 for more details
 */
#define MLME_SUCCESS                    0x00
#define MLME_BUSY_CHAN                  0xe1
#define MLME_BUSY_RX                    0x01
#define MLME_BUSY_TX                    0x02
#define MLME_FORCE_TRX_OFF              0x03
#define MLME_IDLE                       0x04
#define MLME_RX_ON                      0x06
#define MLME_TRX_OFF                    0x08
#define MLME_TX_ON                      0x09
#define MLME_COUNTER_ERROR              0xdb
#define MLME_IMPROPER_KEY_TYPE          0xdc
#define MLME_IMPROPER_SECURITY_LEVEL    0xdd
#define MLME_UNSUPPORTED_LEGACY         0xde
#define MLME_UNSUPPORTED_SECURITY       0xdf
#define MLME_SECURITY_FAIL              0xe4
#define MLME_FRAME_TOO_LONG             0xe5
#define MLME_INVALID_HANDLE             0xe7
#define MLME_INVALID_PARAMETER          0xe8
#define MLME_TX_NO_ACK                  0xe9
#define MLME_NO_BEACON                  0xea
#define MLME_NO_DATA                    0xeb
#define MLME_NO_SHORT_ADDRESS           0xec
#define MLME_PAN_ID_CONFLICT            0xee
#define MLME_TRANSACTION_EXPIRED        0xf0
#define MLME_TRANSACTION_OVERFLOW       0xf1
#define MLME_UNAVAILABLE_KEY            0xf3
#define MLME_UNSUPPORTED_ATTRIBUTE      0xf4
#define MLME_INVALID_ADDRESS            0xf5
#define MLME_INVALID_INDEX              0xf9
#define MLME_LIMIT_REACHED              0xfa
#define MLME_READ_ONLY                  0xfb
#define MLME_SCAN_IN_PROGRESS           0xfc
#define MLME_DATA_POLL_NOTIFICATION     0xff

/**
 * @brief enum mac_scan_type_t MAC scan type
 *
 * See IEEE standard 802.15.4-2006 (table 67) for more details
 */
typedef enum {
    MAC_ED_SCAN_TYPE = 0,
    MAC_ACTIVE_SCAN = 1,
    MAC_PASSIVE_SCAN = 2,
    MAC_ORPHAN_SCAN = 3
} mac_scan_type_t;

/**
 * @brief enum mlme_attr_t MLME attributes used with GET and SET primitives
 *
 * See IEEE standard 802.15.4-2006 (table 86) for more details
 */
typedef enum {
    phyCurrentChannel = 0x00,       /*>Current RF channel*/
    macAckWaitDuration = 0x40,      /*>Integer, n. of symbols*/
    macAssociatedPANCoord = 0x56,   /*>Boolean, associated to PAN coordinator*/
    macAssociationPermit = 0x41,    /*>Boolean, if association is allowed (in coordinator)*/
    macAutoRequest = 0x42,          /*>Boolean, if device automatically sends data request on beacon*/
    macBattLifeExt = 0x43,          /*>Boolean, if BLE is enabled*/
    macBattLifeExtPeriods = 0x44,   /*>Integer 6-41, BLE backoff periods */
    macBeaconPayload = 0x45,        /*>Set of bytes, beacon payload*/
    macBeaconPayloadLength = 0x46,  /*>Integer 0-MaxPayLoadLen*/
    macBeaconOrder = 0x47,          /*>Integer 0–15, Beacon tx period, 15 = no periodic beacon*/
    macBeaconTxTime = 0x48,         /*>Integer 0x000000–0xffffff, symbols, when last beacon was transmitted*/
    macBSN = 0x49,                  /*>Integer 0x00–0xff, Beacon sequence number*/
    macCoordExtendedAddress = 0x4a, /*>64-bit IEEE of coordinator*/
    macCoordShortAddress = 0x4b,    /*>16-bit addr of coordinator*/
    macDSN = 0x4c,                  /*>Integer 0x00–0xff, Data frame sequence number*/
    macGTSPermit = 0x4d,            /*>Boolean, GTS allowed?*/
    macMaxBE = 0x57,                /*>Integer 3–8, max value of backoff exponent*/
    macMaxCSMABackoffs = 0x4e,      /*>Integer 0–5*/
    macMaxFrameTotalWaitTime = 0x58,/*>Integer, max of CAP symbols while waiting for data requested by DREQ or PEND*/
    macMaxFrameRetries = 0x59,      /*>Integer 0–7*/
    macMinBE = 0x4f,                /*>Integer 0–macMaxBE*/
    macPANId = 0x50,                /*>PAN ID, 16 bits*/
    macPromiscuousMode = 0x51,      /*>Boolean*/
    macResponseWaitTime = 0x5a,     /*>Integer 2–64  The maximum time in SuperFrameDurations to wait for responses*/
    macRxOnWhenIdle = 0x52,         /*>Boolean*/
    macSecurityEnabled = 0x5d,      /*>Boolean*/
    macShortAddress = 0x53,         /*>Short address, 16 bits*/
    macSuperframeOrder = 0x54,      /*>Integer 0–15, The length of the active portion of the outgoing superframe, 15 = none*/
    macSyncSymbolOffset = 0x5b,     /*>Integer 0x000–0x100 (symbols) timestamp offset*/
    macTimestampSupported = 0x5c,   /*>Boolean*/
    macTransactionPersistenceTime = 0x55, /*>Integer 0x0000–0xffff (unit periods)*/
    macKeyTable = 0x71,             /*>A table of KeyDescriptor entries, each containing keys and related information required for secured communications.*/
    macKeyTableEntries = 0x72,      /*>The number of entries in macKeyTable.*/
    macDeviceTable = 0x73,          /*>List of Descriptor entries, each indicating a remote device*/
    macDeviceTableEntries = 0x74,   /*>The number of entries in macDeviceTable.*/
    macSecurityLevelTable = 0x75,   /*>A table of SecurityLevelDescriptor entries*/
    macSecurityLevelTableEntries = 0x76,    /*>The number of entries in macSecurityLevelTable*/
    macFrameCounter = 0x77,         /*>The outgoing frame counter*/
    macAutoRequestSecurityLevel = 0x78, /*>0x00–0x07  The security level used for automatic data requests.*/
    macAutoRequestKeyIdMode = 0x79,      /*> The key identifier mode used for automatic data requests.*/
    macAutoRequestKeySource = 0x7a, /*>Key source for automatic data*/
    macAutoRequestKeyIndex = 0x7b,  /*>The index of the key used for automatic data*/
    macDefaultKeySource = 0x7c,      /*>Default key source*/
    //NON standard extension
    macLoadBalancingBeaconTx = 0xfd,  /*> Trig Beacon from load balance module periodic */
    macLoadBalancingAcceptAnyBeacon = 0xfe, /*>Beacon accept state control from other network. Value size bool, data true=Enable, false=disable .*/
    macThreadForceLongAddressForBeacon = 0xff /*>Thread standard force beacon source address for extended 64-bit*/
} mlme_attr_t;

//typedef struct mlme_associate_s {
//    uint8_t LogicalChannel;
//    uint8_t ChannelPage;
//    unsigned CoordAddrMode:2;
//    uint16_t CoordPANId;
//    uint8_t CoordAddress[8];
//    uint8_t CapabilityInformation;
//    mlme_security_t Key;
//} mlme_associate_t;

//typedef struct mlme_associate_ind_s {
//    uint8_t DeviceAddress[8];
//    uint8_t CapabilityInformation;
//    mlme_security_t Key;
//} mlme_associate_ind_t;

//typedef struct mlme_associate_resp_s {
//    uint8_t DeviceAddress[8];
//    uint16_t AssocShortAddress;
//    uint8_t status;
//    mlme_security_t Key;
//} mlme_associate_resp_t;

//typedef struct mlme_associate_conf_s {
//    uint16_t AssocShortAddress;
//    uint8_t status;
//    mlme_security_t Key;
//} mlme_associate_conf_t;

//typedef struct mlme_disassociate_s {
//    unsigned DeviceAddrMode:2;
//    uint16_t DevicePANId;
//    uint8_t DeviceAddress[8];
//    uint8_t DisassociateReason;
//    bool TxIndirect:1;
//    mlme_security_t Key;
//} mlme_disassociate_t;

//typedef struct mlme_disassociate_ind_s {
//    uint8_t DeviceAddress[8];
//    uint8_t DisassociateReason;
//    mlme_security_t Key;
//} mlme_disassociate_ind_t;

//typedef struct mlme_disassociate_conf_s {
//    uint8_t status;
//    unsigned DeviceAddrMode:2;
//    uint16_t DevicePANId;
//    uint8_t DeviceAddress[8];
//} mlme_disassociate_conf_t;

/**
 * @brief struct mlme_beacon_pending_address_spec_t Pending address specification field
 *
 * See IEEE standard 802.15.4-2006 (figure 51) for more details
 */
typedef struct mlme_beacon_pending_address_spec_s{
    unsigned short_address_count:3;
    unsigned extended_address_count:3;
}mlme_beacon_pending_address_spec_t;

/**
 * @brief struct mlme_beacon_gts_spec_t Format of GTS specification field
 *
 * See IEEE standard 802.15.4-2006 (figure 48) for more details
 */
typedef struct mlme_beacon_gts_spec_s{
    unsigned description_count:3;
    unsigned gts_permit:1;
}mlme_beacon_gts_spec_t;

/**
 * @brief struct mlme_beacon_ind_t Beacon notify structure
 *
 * See IEEE standard 802.15.4-2006 (table 54) for more details
 */
typedef struct mlme_beacon_ind_s {
    uint8_t BSN;
    mlme_pan_descriptor_t PANDescriptor;
    mlme_beacon_pending_address_spec_t PendAddrSpec;
    uint8_t *AddrList;
    uint16_t beacon_data_length;
    uint8_t *beacon_data;
} mlme_beacon_ind_t;

/**
 * @brief struct mlme_scan_t Scan request structure
 *
 * See IEEE standard 802.15.4-2006 (table 67) for more details
 */
typedef struct mlme_scan_s {
    mac_scan_type_t     ScanType;       /*> ED=0, active=1, passive=2, orphan=3*/
    channel_list_s	ScanChannels;	/*>bit field, low 27 bits used*/
    uint8_t     ScanDuration;   /*>0-14, scan duration/channel*/
    uint8_t     ChannelPage;    /*>0-31*/
    mlme_security_t Key;
} mlme_scan_t;

/**
 * @brief struct mlme_set_t Set request structure
 *
 * See IEEE standard 802.15.4-2006 (table 70) for more details
 */
typedef struct mlme_set_s {
    mlme_attr_t attr;
    uint8_t attr_index;
    const void *value_pointer;
    uint8_t value_size;
} mlme_set_t;

/**
 * @brief struct mlme_get_t Get request structure
 *
 * See IEEE standard 802.15.4-2006 (table 56) for more details
 */
typedef struct mlme_get_s {
    mlme_attr_t attr;
    uint8_t attr_index;
} mlme_get_t;

/**
 * @brief struct mlme_get_conf_t Get confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 57) for more details
 */
typedef struct mlme_get_conf_s {
    uint8_t status;
    mlme_attr_t attr;
    uint8_t attr_index;
    void *value_pointer;
    uint8_t value_size;
} mlme_get_conf_t;

/**
 * @brief struct mlme_set_conf_t Set confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 71) for more details
 */
typedef struct mlme_set_conf_s {
    uint8_t status;
    mlme_attr_t attr;
    uint8_t attr_index;
} mlme_set_conf_t;


#define MLME_MAC_RES_SIZE_MAX 16

/**
 * @brief struct mlme_scan_conf_t Scan confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 68) for more details
 */
typedef struct mlme_scan_conf_s {
    uint8_t status;
    unsigned ScanType:2;
    uint8_t ChannelPage;
    channel_list_s UnscannedChannels;
    uint8_t ResultListSize;
    uint8_t *ED_values;
    mlme_pan_descriptor_t *PAN_values[MLME_MAC_RES_SIZE_MAX];
} mlme_scan_conf_t;

/**
 * @brief struct mlme_reset_t Reset request structure
 *
 * See IEEE standard 802.15.4-2006 (table 63) for more details
 */
typedef struct mlme_reset_s {
    bool SetDefaultPIB;
} mlme_reset_t;

/**
 * @brief struct mlme_reset_conf_t Reset confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 64) for more details
 */
typedef struct mlme_reset_conf_s {
    uint8_t status;
} mlme_reset_conf_t;

/**
 * @brief struct mlme_rx_enable_t Rx enable request structure
 *
 * See IEEE standard 802.15.4-2006 (table 65) for more details
 */
//typedef struct mlme_rx_enable_s {
//    bool DeferPermit;
//    uint32_t RxOnTime;
//    uint32_t RxOnDuration;
//} mlme_rx_enable_t;

/**
 * @brief struct mlme_rx_enable_conf_t Rx enable confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 66) for more details
 */
//typedef struct mlme_rx_enable_conf_s {
//    uint8_t status;
//} mlme_rx_enable_conf_t;

/**
 * @brief struct mlme_comm_status_t Comm status indication structure
 *
 * See IEEE standard 802.15.4-2006 (table 69) for more details
 */
typedef struct mlme_comm_status_s {
    uint16_t PANId;
    unsigned SrcAddrMode:2;
    uint8_t SrcAddr[8];
    unsigned DstAddrMode:2;
    uint8_t DstAddr[8];
    uint8_t status;
    mlme_security_t Key;
} mlme_comm_status_t;

/**
 * @brief struct mlme_start_t Start request structure
 *
 * See IEEE standard 802.15.4-2006 (table 72) for more details
 */
typedef struct mlme_start_s {
    uint16_t PANId;
    uint8_t LogicalChannel;
    uint8_t ChannelPage;
    uint32_t StartTime;
    unsigned BeaconOrder:4;
    unsigned SuperframeOrder:4;
    bool PANCoordinator:1;
    bool BatteryLifeExtension:1;
    bool CoordRealignment:1;
    mlme_security_t CoordRealignKey;
    mlme_security_t BeaconRealignKey;
} mlme_start_t;

/**
 * @brief struct mlme_start_conf_t Start confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 73) for more details
 */
typedef struct mlme_start_conf_s {
    uint8_t status;
} mlme_start_conf_t;

//typedef struct mlme_sync_s {
//    uint8_t LogicalChannel;
//    uint8_t ChannelPage;
//    bool TrackBeacon;
//} mlme_sync_t;

//typedef struct mlme_sync_loss_s {
//    uint8_t LossReason;
//    uint16_t PANId;
//    uint8_t LogicalChannel;
//    uint8_t ChannelPage;
//    mlme_security_t Key;
//} mlme_sync_loss_t;

/**
 * @brief struct mlme_poll_t Poll request structure
 *
 * See IEEE standard 802.15.4-2006 (table 76) for more details
 */
typedef struct mlme_poll_s {
    unsigned CoordAddrMode:2;
    uint16_t CoordPANId;
    uint8_t CoordAddress[8];
    mlme_security_t Key;
} mlme_poll_t;

/**
 * @brief struct mlme_poll_conf_t Poll confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 77) for more details
 */
typedef struct mlme_poll_conf_s {
    uint8_t status;
} mlme_poll_conf_t;

#endif /* MLME_H_ */
