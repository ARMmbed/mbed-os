/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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
    unsigned SrcAddrMode:2;
    unsigned DstAddrMode:2;
    uint16_t DstPANId;
    uint8_t DstAddr[8];
    uint16_t msduLength;
    uint8_t *msdu;
    uint8_t msduHandle;
    bool TxAckReq: 1;
    bool InDirectTx:1;
    bool PendingBit: 1;
    mlme_security_t Key;
} mcps_data_req_t;

/**
 * @brief struct mcps_data_conf_t Data confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 42) for more details
 */
typedef struct mcps_data_conf_s {
    uint8_t msduHandle;
    uint8_t status;
    uint32_t timestamp;
    //Non-standard extension
    uint8_t cca_retries;
    uint8_t tx_retries; //0 means no retry
} mcps_data_conf_t;

/**
 * @brief struct mcps_data_ind_t Data indication structure
 *
 * See IEEE standard 802.15.4-2006 (table 43) for more details
 */
typedef struct mcps_data_ind_s {
    unsigned SrcAddrMode:2; /*0x00 = no address 0x01 = reserved 0x02 = 16-bit short address 0x03 = 64-bit extended address */
    uint16_t SrcPANId;
    uint8_t SrcAddr[8];
    unsigned DstAddrMode:2;
    uint16_t DstPANId;
    uint8_t DstAddr[8];
    uint8_t mpduLinkQuality;
    int8_t signal_dbm; /* This extension for normal IEEE 802.15.4 Data indication */
    uint32_t timestamp;
    uint8_t DSN;
    mlme_security_t Key;
    uint16_t msduLength;
    uint8_t *msdu_ptr;
} mcps_data_ind_t;

/**
 * @brief struct mcps_purge_t Purge request structure
 *
 * See IEEE standard 802.15.4-2006 (table 44) for more details
 */
typedef struct mcps_purge_s {
    uint8_t msduHandle;
} mcps_purge_t;

/**
 * @brief struct mcps_purge_conf_t Purge confirm structure
 *
 * See IEEE standard 802.15.4-2006 (table 45) for more details
 */
typedef struct mcps_purge_conf_s {
    uint8_t msduHandle;
    uint8_t status;
} mcps_purge_conf_t;


#endif // MAC_MCPS_H
