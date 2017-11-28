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
#ifndef _NS_NWK_SCAN_H
#define _NS_NWK_SCAN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"
#include "net_interface.h" /* needed for channel_list_s */
#include "mlme.h"

/**
 * \file net_nwk_scan.h
 * \brief Link layer Scan API for Active and Energy Detection Scan API.
 *
 * - arm_net_energy_scan(), Energy scan start (Use only with ZIP node library).
 * - arm_net_nwk_scan(), Active scan for network (Use only with ZIP node library).
 * - arm_net_get_scanned_nwk_list(), Active scan result read.
 *
 * Scan API works only when stack is in idle state.
 *
 * The scan operation result is handled at callback function that is defined by scan function call.
 * The scan result format is as follows:
 *
 * | Data pointer | VALUE                                                 |
 * | :----------: | :---------------------------------------------------: |
 * | 1.Byte       | Scan Type: NET_NWK_ENERGY_SCAN or NET_NWK_ACTIVE_SCAN |
 * | 2.Byte       | Result length                                         |
 * | 3+n Bytes    | Payload :Read Only at Energy Detection type           |
 *
 * The result length indicates the scan response size as follows:
 *  - NET_NWK_ENERGY_SCAN Payload length is result length *2 bytes after length field.
 *   * 1.Byte Channel
 *   * 2.Byte Energy Level
 *  - NET_NWK_ACTIVE_SCAN result indicates the the number of networks.
 *   * The network list needs to be read by net_get_scanned_nwk_list().
 *
 */

/** Network energy detection scan type  */
#define NET_NWK_ENERGY_SCAN 0
/** Network active scan type  */
#define NET_NWK_ACTIVE_SCAN 1

/** Network parent address type 16-bit short */
#define NET_PARET_SHORT_16_BIT 2
/** Network parent address type 64-bit long */
#define NET_PARET_LONG_64_BIT 3

/* Active scan level definition */
/** List only PAN networks at channels */
#define NET_ACTIVE_SCAN_ONLY_NWK 0
/** List PAN networks with any beacon payload */
#define NET_ACTIVE_SCAN_ACCEPT_ANY_BEACON_PAYLOAD 1
/** List PAN networks with ZIP-specific payload that accept join */
#define NET_ACTIVE_SCAN_ACCEPT_ONLY_ZIP_SPESIFIC  2
/*!
 * \struct nwk_pan_alternative_parent_t
 * \brief Network alternative parent structure.
 */
typedef struct nwk_pan_alternative_parent_t {
    uint8_t CoordAddrMode; /**< Parent address mode NET_PARET_SHORT_16_BIT or NET_PARET_LONG_64_BIT. */
    uint8_t CoordAddress[8]; /**< Parent address based on CoordAddrMode. */
    uint8_t LinkQuality; /**< LQI to parent. */
} nwk_pan_alternative_parent_t;

/*!
 * \struct nwk_pan_descriptor_t
 * \brief Linked network response list.
 */
typedef struct nwk_pan_descriptor_t {
    mlme_pan_descriptor_t *pan_descriptor;              /**< Pan Description */
    uint8_t *beacon_payload;                            /**< Beacon Payload pointer */
    uint8_t beacon_length;                              /**< Beacon Payload length */
    nwk_pan_alternative_parent_t alternative_parent;   /**< Alternative Parent information pointer */
    struct nwk_pan_descriptor_t *next;                   /**< Link to next network result */
} nwk_pan_descriptor_t;

/**
 * \brief Energy detection scan start for configured channel with application-specific threshold.
 *
 * \param interface_id Interface id.
 * \param scan_list Channel list for scan operation.
 * \param passed_fptr A function pointer for scan result notify.
 * \param energy_tresshold Scan response lists all channels with smaller or equal level.
 *
 * \return 0 Scan operation started OK.
 * \return -1 Stack is active.
 * \return -2 Channel list not valid.
 * \return -3 Function not enabled at border router.
 *
 */
extern int8_t arm_net_energy_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t *conf), uint8_t energy_tresshold);
/**
 * \brief Active network scan for configured channels.
 *
 * \param interface_id Interface id.
 * \param scan_list Channel list for scan operation.
 * \param passed_fptr A function pointer for scan result notify.
 * \param scan_level NET_ACTIVE_SCAN_ONLY_NWK, NET_ACTIVE_SCAN_ACCEPT_ANY_BEACON_PAYLOAD,NET_ACTIVE_SCAN_ACCEPT_ONLY_ZIP_SPESIFIC
 *
 * \return 0 Scan operation started OK.
 * \return -1 Stack is active.
 * \return -2 Channel list not valid.
 * \return -3 Function not enabled at border router.
 *
 */
extern int8_t arm_net_nwk_scan(int8_t interface_id, channel_list_s *scan_list, void (*passed_fptr)(int8_t if_id, const mlme_scan_conf_t *conf), uint8_t scan_level);
/**
 * \brief Active scan result read.
 *
 * Note: The pointer is only valid at callback function call time. The application needs to allocate memory if it wants to save the result.
 *
 * \return >0 A pointer to scan result.
 * \return 0 No network results available.
 *
 */
extern nwk_pan_descriptor_t *arm_net_get_scanned_nwk_list(int8_t interface_id);
#ifdef __cplusplus
}
#endif
#endif /*_NS_NWK_SCAN_H*/
