/*
 * Copyright (c) 2016-2018, 2020-2021, Pelion and affiliates.
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

/** \file mac_mcps.h
 * \brief MAC Common Part Sublayer API
 */

#ifndef MAC_MCPS_H
#define MAC_MCPS_H

#include <inttypes.h>
#include "mac_common_defines.h"


/**
 * @brief struct mcps_data_req_t Data request structure
 *
 * See IEEE standard 802.15.4-2006 (table 41) for more details
 */
typedef struct mcps_data_req_s {
    unsigned SrcAddrMode: 2;        /**< Source address mode */
    unsigned DstAddrMode: 2;        /**< Destination address mode */
    uint16_t DstPANId;              /**< Destination PAN ID */
    uint8_t DstAddr[8];             /**< Destination address */
    uint16_t msduLength;            /**< Service data unit length */
    uint8_t *msdu;                  /**< Service data unit */
    uint8_t msduHandle;             /**< Handle associated with MSDU */
    bool TxAckReq: 1;               /**< Specifies whether ACK is needed or not */
    bool InDirectTx: 1;             /**< Specifies whether indirect or direct transmission is used */
    bool PendingBit: 1;             /**< Specifies whether more fragments are to be sent or not */
    bool SeqNumSuppressed: 1;       /**< True suppress sequence number from frame. This will be only checked when 2015 extension is enabled */
    bool PanIdSuppressed: 1;        /**< True suppress PAN-id is done when possible from frame. This will be only checked when 2015 extension is enabled */
    bool ExtendedFrameExchange: 1;  /**< True for Extended Frame change. This will be only checked when 2015 extension and enhanced frame is enabled */
    mlme_security_t Key;            /**< Security key */
} mcps_data_req_t;

/**
 * @brief struct mcps_data_conf_t Data confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 42) for more details
 */
typedef struct mcps_data_conf_s {
    uint8_t msduHandle;     /**< Handle associated with MSDU */
    uint8_t status;         /**< Status of the last MSDU transmission */
    uint32_t timestamp;     /**< Time, in symbols, at which the data were transmitted */
    //Non-standard extension
    uint8_t cca_retries;    /**< Number of CCA retries used during sending */
    uint8_t tx_retries;     /**< Number of retries done during sending, 0 means no retries */
} mcps_data_conf_t;


/**
 * @brief struct mcps_data_conf_payload_t Data confirmatio IE extension list and payload from enhanced ACK
 *
 */
typedef struct mcps_data_conf_payload_s {
    uint8_t *headerIeList;              /**< Header information IE's list without terminator*/
    uint8_t *payloadIeList;             /**< Payload information IE's list without terminator*/
    uint8_t *payloadPtr;                /**< Ack payload pointer */
    uint16_t headerIeListLength;        /**< Header information IE's list length in bytes */
    uint16_t payloadIeListLength;       /**< Payload information IE's list length in bytes */
    uint16_t payloadLength;             /**< Payload length in bytes */
} mcps_data_conf_payload_t;


/**
 * @brief struct mcps_data_ind_t Data indication structure
 *
 * See IEEE standard 802.15.4-2006 (table 43) for more details
 */
typedef struct mcps_data_ind_s {
    unsigned SrcAddrMode: 2;    /**< 0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    uint16_t SrcPANId;          /**< Source PAN ID */
    uint8_t SrcAddr[8];         /**< Source address */
    unsigned DstAddrMode: 2;    /**< Destination address mode */
    bool DSN_suppressed: 1;     /**< Indicate when DSN not include valid sequency id */
    uint16_t DstPANId;          /**< Destination PAN ID */
    uint8_t DstAddr[8];         /**< Destination address */
    uint8_t mpduLinkQuality;    /**< LQI value measured during reception of the MPDU */
    int8_t signal_dbm;          /**< This extension for normal IEEE 802.15.4 Data indication */
    uint32_t timestamp;         /**< The time, in symbols, at which the data were received */
    uint8_t DSN;                /**< Data sequence number */
    mlme_security_t Key;        /**< Security key */
    uint16_t msduLength;        /**< Data unit length */
    uint8_t *msdu_ptr;          /**< Data unit */
} mcps_data_ind_t;

/**
 * @brief struct mcps_data_ie_list_t MCPS data Information element list stucture
 *
 * Structure for IEEE 802.15.4-2015 MCPS data extension to Indication
 */
typedef struct mcps_data_ie_list {
    uint8_t *headerIeList;              /**< Header information IE's list without terminator*/
    uint8_t *payloadIeList;             /**< Payload information IE's list without terminator*/
    uint16_t headerIeListLength;        /**< Header information IE's list length in bytes */
    uint16_t payloadIeListLength;       /**< Payload information IE's list length in bytes */
} mcps_data_ie_list_t;

/** \brief Scatter-gather descriptor for MCPS request IE Element list
 *
 * Slightly optimised for small platforms - we assume we won't need any
 * element bigger than 64K.
 */
typedef struct ns_ie_iovec {
    void *ieBase;              /**< IE element pointer */
    uint_fast16_t iovLen;      /**< IE element length */
} ns_ie_iovec_t;


/**
 * @brief struct mcps_data_req_ie_list MCPS data Information element list stuctrure
 *
 * Structure for IEEE 802.15.4-2015 MCPS data extension to Request
 *
 * IE element could be divided to multiple vector which MAC just write to message direct.
 */
typedef struct mcps_data_req_ie_list {
    ns_ie_iovec_t *headerIeVectorList;   /**< Header IE element list */
    ns_ie_iovec_t *payloadIeVectorList;  /**< Payload IE element list */
    uint16_t headerIovLength;            /**< Header IE element list size, set 0 when no elements */
    uint16_t payloadIovLength;           /**< Payload IE element list size, set 0 when no elements */
} mcps_data_req_ie_list_t;


/**
 * @brief struct mcps_ack_data_payload_t IE extension list and payload for enhanced ACK
 *
 */
typedef struct mcps_ack_data_payload {
    struct mcps_data_req_ie_list ie_elements;   /**< IE hader and payload's elements */
    uint8_t *payloadPtr;                        /**< Ack payload pointer */
    uint16_t payloadLength;                     /**< Payload length in bytes */
} mcps_ack_data_payload_t;

/**
 * @brief struct mcps_purge_t Purge request structure
 *
 * See IEEE standard 802.15.4-2006 (table 44) for more details
 */
typedef struct mcps_purge_s {
    uint8_t msduHandle;         /**< Handle associated with MSDU */
} mcps_purge_t;

/**
 * @brief struct mcps_purge_conf_t Purge confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 45) for more details
 */
typedef struct mcps_purge_conf_s {
    uint8_t msduHandle;         /**< Handle associated with MSDU */
    uint8_t status;             /**< Status of the purge performed */
} mcps_purge_conf_t;

#define MCPS_EDFE_NORMAL_FRAME 0    /**< Normal Data message normal behaviour */
#define MCPS_EDFE_MALFORMED_FRAME 1 /**< Drop whole packet */
#define MCPS_EDFE_TX_FRAME  2       /**< Tx message send data if pending in 1ms -5ms time window */
#define MCPS_EDFE_RESPONSE_FRAME 3  /**< Response message send data if pending in 1ms -5ms time window */
#define MCPS_EDFE_FINAL_FRAME_TX 4  /**< Final response message send in 1ms -5ms time window */
#define MCPS_EDFE_FINAL_FRAME_RX 5  /**< EDFE session can be close at MAC side and drop this packet */

/**
 * @brief struct mcps_edfe_response_t EDFE detetction and response structure
 *
 */
typedef struct mcps_edfe_response_s {
    struct mcps_data_ie_list ie_elements;   /**< IE hader and payload's elements from Packet */
    struct mcps_data_req_ie_list ie_response; /**< IE hader and payload's elements for Response Packet */
    uint8_t edfe_message_status; /**< Indicate Packet handler status */
    uint8_t message_handle; /**< EDFE Data request message ID for detect pending data at LLC layer*/
    int8_t rssi;    /**< Received packet signal streng in dbm */
    unsigned SrcAddrMode: 2;        /**< Source address mode: used for RX validation and TX purpose */
    unsigned DstAddrMode: 2;        /**< Destination address mode: used for RX validation and TX purpose */
    uint8_t Address[8];             /**< RX: Packet Address Src & TX Response Destination address */
    bool SeqNumSuppressed: 1;       /**< True suppress sequence number from frame. This will be only checked when 2015 extension is enabled */
    bool PanIdSuppressed: 1;        /**< True suppress PAN-id is done when possible from frame.  */
    bool wait_response: 1;          /**< True enable response wait timer and re-send operation.  */
    bool use_message_handle_to_discover: 1; /**< EDFE Data request message ID is valid at message_handle.  */
} mcps_edfe_response_t;

/**
 * @brief enum mac_data_priority_t Data request priority level
 *
 * Data request priority level may affect CCA process and MAC queue process
 */
typedef enum mac_data_priority_e {
    MAC_DATA_NORMAL_PRIORITY = 0,   /**< Normal MCPS DATA REQ */
    MAC_DATA_MEDIUM_PRIORITY = 1,   /**< Indirect Data which is polled */
    MAC_DATA_HIGH_PRIORITY = 2,     /**< MAC command usually use this and beacon */
    MAC_DATA_EXPEDITE_FORWARD = 3   /**< Expedite forward level give highest priority */
} mac_data_priority_t;


#endif // MAC_MCPS_H
