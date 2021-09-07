/*
 * Copyright (c) 2016-2018, 2020, Pelion and affiliates.
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

/** \file mac_common_defines.h
 * \brief Common definitions for MAC layer.
 */

#ifndef MAC_COMMON_DEFINES_H_
#define MAC_COMMON_DEFINES_H_

#include "ns_types.h"

#define MAC_ADDR_MODE_NONE 0                    /**< Address mode for no address defined */
#define MAC_ADDR_MODE_16_BIT 2                  /**< Address mode for 16-bit addresses */
#define MAC_ADDR_MODE_64_BIT 3                  /**< Address mode for 64-bit addresses */

#define MAC_FRAME_VERSION_2003         0        /**< FCF - IEEE 802.15.4-2003 compatible */
#define MAC_FRAME_VERSION_2006         1        /**< FCF - IEEE 802.15.4-2006 (big payload or new security) */
#define MAC_FRAME_VERSION_2015         2        /**< FCF - IEEE 802.15.4-2015 (IE element support) */

//See IEEE standard 802.15.4-2006 (table 96) for more details about identifiers
#define MAC_KEY_ID_MODE_IMPLICIT    0           /**< Key identifier mode implicit */
#define MAC_KEY_ID_MODE_IDX         1           /**< Key identifier mode for 1-octet key index */
#define MAC_KEY_ID_MODE_SRC4_IDX    2           /**< Key identifier mode for combined 4-octet key source and 1-octet key index */
#define MAC_KEY_ID_MODE_SRC8_IDX    3           /**< Key identifier mode for combined 8-octet key source and 1-octet key index */

/* IEEE 802.15.4 constants */
#define MAC_IEEE_802_15_4_MAX_MPDU_UNSECURED_OVERHEAD   25      /**< Maximum for MAC protocol data unit unsecured overhead */
#define MAC_IEEE_802_15_4_MIN_MPDU_OVERHEAD             9       /**< Minimum overhead added by MAC to MPDU */
#define MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD           75      /**< Maximum overhead which is added by the MAC for beacon */
#define MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE           127     /**< Maximum number of octets PHY layer is able to receive */
#define MAC_IEEE_802_15_4G_MAX_PHY_PACKET_SIZE          2047    /**< Maximum number of octets PHY layer is able to receive */

#define MAC_IEEE_802_15_4_MAX_BEACON_PAYLOAD_LENGTH \
    (MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD) /**< Maximum size of beacon payload */

/** Maximum number of bytes which can be transmitted in the MAC payload of an unsecured MAC frame */
#define MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE \
    (MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - MAC_IEEE_802_15_4_MAX_MPDU_UNSECURED_OVERHEAD)

#define mac_unsecured_2003_compatibility false /**< For IEEE 802.15.4-2003 MAC compatibility, force max size to never exceed MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE */

/**
 * @brief struct mlme_security_t MLME/MCPS security structure
 * This structure encapsulates security related variables,
 * which are always used together if SecurityLevel > 0.
 *
 * See IEEE standard 802.15.4-2006 (e.g end of table 41) for more details
 */
typedef struct mlme_security_s {
    unsigned SecurityLevel: 3;  /**< Security level */
    unsigned KeyIdMode: 2;      /**< 2-bit value which define key source and ID use case */
    uint8_t KeyIndex;           /**< Key index */
    uint8_t Keysource[8];       /**< Key source */
} mlme_security_t;

#define MAC_HEADER_VENDOR_SPESIFIC_IE_ID          0x00  /**< Vendor specific Header IE element */
#define MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID    0x2a  /**< External organisation defined Header IE element */
#define MAC_HEADER_TERMINATION1_IE_ID             0x7e  /**< Header IE element termination when Payload element is following Header IE */
#define MAC_HEADER_TERMINATION2_IE_ID             0x7f  /**< Header IE element termination when no Payload element is following Header IE but normal payload is */


/**
 * @brief struct mac_header_IE_t Mac Header information element structure for parsing or write operation
 * This structure encapsulates security related variables,
 */
typedef struct mac_header_IE_s {
    uint8_t *content_ptr;   /**< Content data */
    unsigned length: 7;     /**< Element length 0- 127 */
    uint8_t id;             /**< Element ID */
} mac_header_IE_t;

#define MAC_PAYLOAD_IE_ESDU_GROUP_ID            0x00    /**< Encapsulated Service Data Unit (ESDU) Payload IE element's */
#define MAC_PAYLOAD_MLME_IE_GROUP_ID            0x01    /**< MLME nested Payload IE element's */
#define MAC_PAYLOAD_VENDOR_IE_GROUP_ID          0x02    /**< Vendor specific nested Payload IE element's */
#define MAC_PAYLOAD_MPX_IE_GROUP_ID             0x03    /**< MPX Payload IE element, IEEE 802.15.9 defined */
#define MAC_PAYLOAD_TERMINATION_IE_GROUP_ID     0x0f    /**< Payload IE element terminator. Mandatory when normal payload is coming after IE element part */

/**
 * @brief struct mac_header_IE_t Mac Payload information element structure for parsing or write operation
 * This structure encapsulates security related variables,
 */
typedef struct mac_payload_IE_s {
    uint8_t *content_ptr;   /**< Content data */
    unsigned length: 11;    /**< Element length 0- 2047 */
    unsigned id: 4;         /**< Group ID */
} mac_payload_IE_t;

#endif /* MAC_COMMON_DEFINES_H_ */
