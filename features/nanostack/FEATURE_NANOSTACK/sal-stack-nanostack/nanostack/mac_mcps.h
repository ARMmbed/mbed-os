/*
 * Copyright (c) 2016-2017 ARM Limited. All rights reserved.
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
    unsigned SrcAddrMode:2;         /**< Source address mode */
    unsigned DstAddrMode:2;         /**< Destination address mode */
    uint16_t DstPANId;              /**< Destination PAN ID */
    uint8_t DstAddr[8];             /**< Destination address */
    uint16_t msduLength;            /**< Service data unit length */
    uint8_t *msdu;                  /**< Service data unit */
    uint8_t msduHandle;             /**< Handle associated with MSDU */
    bool TxAckReq: 1;               /**< Specifies whether ACK is needed or not */
    bool InDirectTx:1;              /**< Specifies whether indirect or direct transmission is used */
    bool PendingBit: 1;             /**< Specifies whether more fragments are to be sent or not */
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
 * @brief struct mcps_data_ind_t Data indication structure
 *
 * See IEEE standard 802.15.4-2006 (table 43) for more details
 */
typedef struct mcps_data_ind_s {
    unsigned SrcAddrMode:2;     /**< 0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    uint16_t SrcPANId;          /**< Source PAN ID */
    uint8_t SrcAddr[8];         /**< Source address */
    unsigned DstAddrMode:2;     /**< Destination address mode */
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


#endif // MAC_MCPS_H
