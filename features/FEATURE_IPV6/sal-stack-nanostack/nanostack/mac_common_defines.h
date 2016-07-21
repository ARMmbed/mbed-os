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

/** \file mac_common_defines.h
 * \brief Common definitions for MAC layer.
 */

#ifndef MAC_COMMON_DEFINES_H_
#define MAC_COMMON_DEFINES_H_

#include "ns_types.h"

#define MAC_ADDR_MODE_NONE 0
#define MAC_ADDR_MODE_16_BIT 2
#define MAC_ADDR_MODE_64_BIT 3

#define MAC_FRAME_VERSION_2003         0    // FCF - IEEE 802.15.4-2003 compatible
#define MAC_FRAME_VERSION_2006         1    // FCF - IEEE 802.15.4-2006 (big payload or new security)


#define MAC_KEY_ID_MODE_IMPLICIT    0
#define MAC_KEY_ID_MODE_IDX         1
#define MAC_KEY_ID_MODE_SRC4_IDX    2
#define MAC_KEY_ID_MODE_SRC8_IDX    3

/* IEEE 802.15.4 constants */
#define MAC_IEEE_802_15_4_MAX_MPDU_UNSECURED_OVERHEAD   25
#define MAC_IEEE_802_15_4_MIN_MPDU_OVERHEAD             9
#define MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD           75
#define MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE           127

#define MAC_IEEE_802_15_4_MAX_BEACON_PAYLOAD_LENGTH \
    (MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD)
#define MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE \
    (MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - MAC_IEEE_802_15_4_MAX_MPDU_UNSECURED_OVERHEAD)

#define mac_unsecured_2003_compatibility false

/**
 * @brief struct mlme_security_t MLME/MCPS security structure
 * This structure encapsulates security related variables,
 * which are always used together if SecurityLevel > 0.
 *
 * See IEEE standard 802.15.4-2006 (e.g end of table 41) for more details
 */
typedef struct mlme_security_s {
    unsigned SecurityLevel:3;
    unsigned KeyIdMode:2; /* 2-bit value which define key source and ID use case */
    uint8_t KeyIndex; /* Key index */
    uint8_t Keysource[8];
} mlme_security_t;

#endif /* MAC_COMMON_DEFINES_H_ */
