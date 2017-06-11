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
    unsigned SecurityLevel:3;   /**< Security level */
    unsigned KeyIdMode:2;       /**< 2-bit value which define key source and ID use case */
    uint8_t KeyIndex;           /**< Key index */
    uint8_t Keysource[8];       /**< Key source */
} mlme_security_t;

#endif /* MAC_COMMON_DEFINES_H_ */
