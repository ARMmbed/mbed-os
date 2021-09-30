/*
 * Copyright (c) 2021, Pelion and affiliates.
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

#include <string.h>
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "ws_management_api.h"

#ifdef HAVE_WS
#define TRACE_GROUP "wsph"

uint32_t ws_phy_decode_channel_spacing(uint8_t channel_spacing)
{
    if (CHANNEL_SPACING_100 == channel_spacing) {
        return 100000;
    } else if (CHANNEL_SPACING_200 == channel_spacing) {
        return 200000;
    } else if (CHANNEL_SPACING_250 == channel_spacing) {
        return 250000;
    } else if (CHANNEL_SPACING_400 == channel_spacing) {
        return 400000;
    } else if (CHANNEL_SPACING_600 == channel_spacing) {
        return 600000;
    } else if (CHANNEL_SPACING_800 == channel_spacing) {
        return 800000;
    } else if (CHANNEL_SPACING_1200 == channel_spacing) {
        return 1200000;
    }
    return 0;
}

uint32_t ws_phy_get_datarate_using_operating_mode(uint8_t operating_mode)
{
    if ((OPERATING_MODE_1a == operating_mode) || (OPERATING_MODE_1b == operating_mode)) {
        return 50000;
    } else if ((OPERATING_MODE_2a == operating_mode) || (OPERATING_MODE_2b == operating_mode)) {
        return 100000;
    } else if (OPERATING_MODE_3 == operating_mode) {
        return 150000;
    } else if ((OPERATING_MODE_4a == operating_mode) || (OPERATING_MODE_4b == operating_mode)) {
        return 200000;
    } else if (OPERATING_MODE_5 == operating_mode) {
        return 300000;
    }
    return 0;
}

uint8_t ws_phy_convert_operating_class_to_channel_plan_id(uint8_t operating_class, uint8_t regulatory_domain)
{
    if (regulatory_domain == REG_DOMAIN_KR) {

    } else if (regulatory_domain == REG_DOMAIN_EU) {
        if ((operating_class == 1) || (operating_class == 2) || (operating_class == 3) || (operating_class == 4)) {
            return operating_class + 31;
        }
    } else if (regulatory_domain == REG_DOMAIN_IN) {

    } else if ((regulatory_domain == REG_DOMAIN_NA) || (regulatory_domain == REG_DOMAIN_BZ)) {
        if ((operating_class == 1) || (operating_class == 2) || (operating_class == 3)) {
            return operating_class;
        }
    } else if (regulatory_domain == REG_DOMAIN_JP) {

    } else if (regulatory_domain == REG_DOMAIN_WW) {

    }

    return 0;
}

uint8_t ws_phy_convert_operating_mode_to_phy_mode_id(uint8_t operating_mode)
{
    if (OPERATING_MODE_1a == operating_mode) {
        return 1;
    } else if (OPERATING_MODE_1b == operating_mode) {
        return 2;
    } else if (OPERATING_MODE_2a == operating_mode) {
        return 3;
    } else if (OPERATING_MODE_2b == operating_mode) {
        return 4;
    } else if (OPERATING_MODE_3 == operating_mode) {
        return 5;
    } else if (OPERATING_MODE_4a == operating_mode) {
        return 6;
    } else if (OPERATING_MODE_4b == operating_mode) {
        return 7;
    } else if (OPERATING_MODE_5 == operating_mode) {
        return 8;
    }
    return 0;
}

uint32_t ws_phy_get_datarate_using_phy_mode_id(uint8_t phy_mode_id)
{
    if (84 == phy_mode_id) {
        return 150000;
    } else if (85 == phy_mode_id) {
        return 200000;
    } else if ((68 == phy_mode_id) || (86 == phy_mode_id)) {
        return 300000;
    } else if ((34 == phy_mode_id) || (51 == phy_mode_id) || (69 == phy_mode_id)) {
        return 400000;
    } else if ((52 == phy_mode_id) || (70 == phy_mode_id)) {
        return 600000;
    } else if ((35 == phy_mode_id) || (53 == phy_mode_id)) {
        return 800000;
    } else if ((36 == phy_mode_id) || (54 == phy_mode_id)) {
        return 1200000;
    } else if (37 == phy_mode_id) {
        return 1600000;
    } else if (38 == phy_mode_id) {
        return 2400000;
    } else if ((1 == phy_mode_id) || (2 == phy_mode_id) || (17 == phy_mode_id) || (18 == phy_mode_id)) {
        return 50000;
    } else if ((3 == phy_mode_id) || (4 == phy_mode_id) || (19 == phy_mode_id) || (20 == phy_mode_id)) {
        return 100000;
    } else if ((5 == phy_mode_id) || (21 == phy_mode_id)) {
        return 150000;
    } else if ((6 == phy_mode_id) || (7 == phy_mode_id) || (22 == phy_mode_id) || (23 == phy_mode_id)) {
        return 200000;
    } else if ((8 == phy_mode_id) || (24 == phy_mode_id)) {
        return 300000;
    }
    return 0;
}

uint8_t ws_phy_get_ofdm_option_using_phy_mode_id(uint8_t phy_mode_id)
{
    if ((phy_mode_id >= 34) && (phy_mode_id <= 38)) {
        return OFDM_OPTION_1;
    } else if ((phy_mode_id >= 51) && (phy_mode_id <= 54)) {
        return OFDM_OPTION_2;
    } else if ((phy_mode_id >= 68) && (phy_mode_id <= 70)) {
        return OFDM_OPTION_3;
    } else if ((phy_mode_id >= 84) && (phy_mode_id <= 86)) {
        return OFDM_OPTION_4;
    }
    return 0;
}

uint8_t ws_phy_get_ofdm_mcs_using_phy_mode_id(uint8_t phy_mode_id)
{
    if (34 == phy_mode_id) {
        return OFDM_MCS_2;
    } else if ((35 == phy_mode_id) || (51 == phy_mode_id)) {
        return OFDM_MCS_3;
    } else if ((36 == phy_mode_id) || (52 == phy_mode_id) || (68 == phy_mode_id) || (84 == phy_mode_id)) {
        return OFDM_MCS_4;
    } else if ((37 == phy_mode_id) || (53 == phy_mode_id) || (69 == phy_mode_id) || (85 == phy_mode_id)) {
        return OFDM_MCS_5;
    } else if ((38 == phy_mode_id) || (54 == phy_mode_id) || (70 == phy_mode_id) || (86 == phy_mode_id)) {
        return OFDM_MCS_6;
    }
    return 0;
}

uint8_t ws_phy_get_channel_plan_id_using_phy_mode_id(uint8_t phy_mode_id, uint8_t regulatory_domain, uint8_t base_channel_plan_id)
{
    if (regulatory_domain == REG_DOMAIN_KR) {

    } else if (regulatory_domain == REG_DOMAIN_EU) {
        // As on EU domain the PHY mode ID cannot unambiguously define the used channel plan id, use the one which best matches to the base channel plan id
        if ((base_channel_plan_id == 32) || (base_channel_plan_id == 33)) {
            if ((phy_mode_id == 1) || (phy_mode_id == 17)) {
                return 32;
            } else if ((phy_mode_id == 3) || (phy_mode_id == 5) || (phy_mode_id == 19) || (phy_mode_id == 21) || ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
                return 33;
            }
        } else if ((base_channel_plan_id == 36) || (base_channel_plan_id == 37)) {
            if ((phy_mode_id == 1) || (phy_mode_id == 17)) {
                return 36;
            } else if ((phy_mode_id == 3) || (phy_mode_id == 5) || (phy_mode_id == 19) || (phy_mode_id == 21) || ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
                return 37;
            }
        } else if ((base_channel_plan_id == 34) || (base_channel_plan_id == 35)) {
            if ((phy_mode_id == 1) || (phy_mode_id == 17)) {
                return 34;
            } else if ((phy_mode_id == 3) || (phy_mode_id == 5) || (phy_mode_id == 19) || (phy_mode_id == 21) || ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
                return 35;
            }
        }
    } else if (regulatory_domain == REG_DOMAIN_IN) {

    } else if ((regulatory_domain == REG_DOMAIN_NA) || (regulatory_domain == REG_DOMAIN_BZ)) {
        if ((phy_mode_id == 2) || (phy_mode_id == 3) || (phy_mode_id == 18) || (phy_mode_id == 19) || ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
            return 1;
        } else if ((phy_mode_id == 5) || (phy_mode_id == 6) || (phy_mode_id == 21) || (phy_mode_id == 22) || ((phy_mode_id >= 68) && (phy_mode_id <= 70))) {
            return 2;
        } else if ((phy_mode_id == 8) || (phy_mode_id == 24)) {
            return 3;
        } else if ((phy_mode_id >= 51) && (phy_mode_id <= 54)) {
            return 4;
        } else if ((phy_mode_id >= 34) && (phy_mode_id <= 38)) {
            return 5;
        }
    } else if (regulatory_domain == REG_DOMAIN_JP) {

    } else if (regulatory_domain == REG_DOMAIN_WW) {

    }

    return 0;
}

uint16_t ws_phy_get_number_of_channels_using_channel_plan_id(uint8_t channel_plan_id)
{
    if (channel_plan_id == 1) {
        return 129;
    } else if (channel_plan_id == 2) {
        return 64;
    } else if (channel_plan_id == 3) {
        return 42;
    } else if (channel_plan_id == 4) {
        return 32;
    } else if (channel_plan_id == 5) {
        return 21;
    } else if (channel_plan_id == 32) {
        return 69;
    } else if (channel_plan_id == 33) {
        return 35;
    } else if (channel_plan_id == 34) {
        return 55;
    } else if (channel_plan_id == 35) {
        return 27;
    } else if (channel_plan_id == 36) {
        return 125;
    } else if (channel_plan_id == 37) {
        return 62;
    }

    return 0;
}

uint32_t ws_phy_get_channel_spacing_using_channel_plan_id(uint8_t channel_plan_id)
{
    if (channel_plan_id == 1) {
        return 200000;
    } else if (channel_plan_id == 2) {
        return 400000;
    } else if (channel_plan_id == 3) {
        return 600000;
    } else if (channel_plan_id == 4) {
        return 800000;
    } else if (channel_plan_id == 5) {
        return 1200000;
    } else if (channel_plan_id == 32) {
        return 100000;
    } else if (channel_plan_id == 33) {
        return 200000;
    } else if (channel_plan_id == 34) {
        return 100000;
    } else if (channel_plan_id == 35) {
        return 200000;
    } else if (channel_plan_id == 36) {
        return 100000;
    } else if (channel_plan_id == 37) {
        return 200000;
    }

    return 0;
}

uint32_t ws_phy_get_channel_0_frequency_using_channel_plan_id(uint8_t channel_plan_id)
{
    if (channel_plan_id == 1) {
        return 902200000;
    } else if (channel_plan_id == 2) {
        return 902400000;
    } else if (channel_plan_id == 3) {
        return 902600000;
    } else if (channel_plan_id == 4) {
        return 902800000;
    } else if (channel_plan_id == 5) {
        return 903200000;
    } else if (channel_plan_id == 32) {
        return 863100000;
    } else if (channel_plan_id == 33) {
        return 863100000;
    } else if (channel_plan_id == 34) {
        return 870100000;
    } else if (channel_plan_id == 35) {
        return 870200000;
    } else if (channel_plan_id == 36) {
        return 863100000;
    } else if (channel_plan_id == 37) {
        return 863100000;
    }

    return 0;
}

bool ws_phy_get_fsk_fec_enabled_using_phy_mode_id(uint8_t phy_mode_id)
{
    if ((phy_mode_id >= 17) && (phy_mode_id <= 24)) {
        return true;
    }
    return false;
}

phy_modulation_e ws_phy_get_modulation_using_phy_mode_id(uint8_t phy_mode_id)
{
    if (((phy_mode_id >= 34) && (phy_mode_id <= 38)) ||
            ((phy_mode_id >= 51) && (phy_mode_id <= 54)) ||
            ((phy_mode_id >= 68) && (phy_mode_id <= 70)) ||
            ((phy_mode_id >= 84) && (phy_mode_id <= 86))) {
        return M_OFDM;
    } else if (((phy_mode_id >= 1) && (phy_mode_id <= 8)) || ((phy_mode_id >= 17) && (phy_mode_id <= 24))) {
        return M_2FSK;
    } else {
        return M_UNDEFINED;
    }
}

phy_modulation_index_e ws_phy_get_modulation_index_using_phy_mode_id(uint8_t phy_mode_id)
{
    if (ws_phy_get_modulation_using_phy_mode_id(phy_mode_id) != M_2FSK) {
        return MODULATION_INDEX_UNDEFINED;
    }

    if ((2 == phy_mode_id) || (18 == phy_mode_id) ||
            (4 == phy_mode_id) || (20 == phy_mode_id) ||
            (7 == phy_mode_id) || (23 == phy_mode_id)) {
        return MODULATION_INDEX_1_0;
    }
    return MODULATION_INDEX_0_5;
}

phy_modulation_index_e ws_phy_get_modulation_index_using_operating_mode(uint8_t operating_mode)
{
    if ((OPERATING_MODE_1b == operating_mode) || (OPERATING_MODE_2b == operating_mode) || (OPERATING_MODE_4b == operating_mode)) {
        return MODULATION_INDEX_1_0;
    } else {
        return MODULATION_INDEX_0_5;
    }
}


#endif //HAVE_WS
