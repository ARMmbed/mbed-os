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

#include "string.h"
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "mac_api.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_mode_switch.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/rf_driver_storage.h"

#define TRACE_GROUP "mswc"

static uint8_t mac_calculate_mode_switch_parity(uint16_t mode_switch_phr)
{
    uint8_t counter = 0;
    // Calculate number of 1-bits in (15-bit) input and return modulo-2 of the sum
    for (int i = 0; i < 15; i++) {
        if (mode_switch_phr & (1 << i)) {
            counter++;
        }
    }
    return counter % 2;
}
/*
 * 11-bit input must be padded with 4 leading zeroes.
 * Reverse the 15-bit result and divide with polynomial X⁴ + X + 1 -> 10011
 * Return remainder as checksum
 *
 * Example:
 * Input:                       xxxxx01000000001
 * Padded input:                x000001000000001
 * Reversed and padded input:   100000000100000x
 * Calculated checksum:         0x0f (00001111)
 *
 * Division:
 *
 *             10011010101   <- Result
 *       -----------------
 * 10011 | 100000000100000   <- Highest bit (10000) is 1, add 1 in result
 *         10011             <- 10011 * 1
 *         -----
 *          00110            <- Highest bit (00110) is 0, add 0 in result
 *          00000            <- 10011 * 0
 *          -----
 *            .
 *               .
 *                  .
 *                   -----
 *                   11100
 *                   10011
 *                    ----
 *                    1111  <- Remainder
 *
 */
static uint8_t mac_calculate_mode_switch_checksum(uint16_t mode_switch_phr)
{
    // X⁴ + X + 1 -> 0b10011 -> 0x13
    uint8_t polynomial = 0x13;
    // Pad input with four leading zeroes
    mode_switch_phr &= ~0x7800;
    // Reverse input
    uint16_t phr_reversed = 0;
    for (int i = 0; i < 16; i++) {
        if (mode_switch_phr & (1 << i)) {
            phr_reversed |= 1 << (15 - i);
        }
    }
    // Divide 15-bit padded and reversed input, use polynomial 10011 as the divider
    uint8_t shift = 11;
    uint8_t remainder = phr_reversed >> shift;
    for (int i = 0; i < 11; i++) {
        // Check highest bit
        if (remainder & (1 << 4)) {
            remainder ^= polynomial;
        } else {
            remainder ^= 0;
        }
        // Division ready, return remainder as checksum
        if (!(--shift)) {
            return remainder;
        }
        remainder <<= 1;
        if (phr_reversed & (1 << shift)) {
            remainder |= 1;
        }
    }
    // Shouldn't go here
    return 0;
}

/*
 * Mode switch PHR format:
 *
 *  |     0     |   1-2  |      3-10     |  11-14 |  15  |
 *  |Mode Switch|Reserved|New Phy Mode ID|Checksum|Parity|
 *
 */
int8_t mac_build_mode_switch_phr(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t *data_ptr, uint8_t phy_mode_id)
{
    (void) rf_ptr;
    if (!data_ptr) {
        return -1;
    }
    // - Write mode switch and PHY mode id fields
    uint16_t mode_switch_phr = 1 << SHIFT_MODE_SWITCH;
    mode_switch_phr |= phy_mode_id << SHIFT_MODE_SWITCH_PHY_MODE;
    // - Calculate checksum
    mode_switch_phr |= mac_calculate_mode_switch_checksum(mode_switch_phr) << SHIFT_MODE_SWITCH_CHECKSUM;
    // - Calculate parity
    mode_switch_phr |= mac_calculate_mode_switch_parity(mode_switch_phr) << SHIFT_MODE_SWITCH_PARITY;

    common_write_16_bit_inverse(mode_switch_phr, data_ptr);

    // - With successful return value, MAC should start CSMA-CA for a mode switch PHR
    return 0;
}

static int8_t mac_change_mode_switch_configuration(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t phy_mode_id, mac_mode_switch_states_e new_state)
{
    mac_api_t *mac_api = get_sw_mac_api(rf_ptr);
    if (mac_api->mode_resolver_cb) {
        phy_rf_channel_configuration_s rf_config;
        memset(&rf_config, 0, sizeof(phy_rf_channel_configuration_s));
        if (!mac_api->mode_resolver_cb(mac_api, phy_mode_id, &rf_config)) {
            rf_ptr->dev_driver->phy_driver->extension(PHY_EXTENSION_SET_RF_CONFIGURATION, (uint8_t *) &rf_config);
            rf_ptr->mode_switch_state = new_state;
            return 0;
        } else {
            tr_error("Mode switch could not resolve PHY mode ID %u", phy_mode_id);
        }
    }
    return -1;
}

static uint32_t mac_calculate_timeout(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t phy_mode_id)
{
    mac_api_t *mac_api = get_sw_mac_api(rf_ptr);
    if (mac_api->mode_resolver_cb) {
        phy_rf_channel_configuration_s rf_config;
        memset(&rf_config, 0, sizeof(phy_rf_channel_configuration_s));
        if (!mac_api->mode_resolver_cb(mac_api, phy_mode_id, &rf_config)) {
            // Calculate transmission time of max size packet + Ack size + max settling time + Tack
            return ((uint64_t)(1200 + 100) * 8000000 / rf_config.datarate + 1500 + 5000);
        }
    }
    // If could not resolve configuration, use 300ms as default timeout
    return 300000;
}

int8_t mac_parse_mode_switch_phr(protocol_interface_rf_mac_setup_s *rf_ptr, const uint8_t *data_ptr, uint16_t data_len)
{
    if (data_len != PHR_LEN) {
        return -1;
    }
    if (!data_ptr) {
        return -1;
    }

    uint16_t mode_switch_phr = common_read_16_bit_inverse(data_ptr);
    if (!(mode_switch_phr & MASK_MODE_SWITCH)) {
        // Mode switch not enabled
        return -1;
    }

    // - Validate checksum
    if (mac_calculate_mode_switch_checksum(mode_switch_phr) != ((mode_switch_phr & MASK_MODE_SWITCH_CHECKSUM) >> SHIFT_MODE_SWITCH_CHECKSUM)) {
        // Invalid checksum, TODO: error correction
        return -1;
    }
    // - Validate parity
    if (mac_calculate_mode_switch_parity(mode_switch_phr) != (mode_switch_phr & MASK_MODE_SWITCH_PARITY) >> SHIFT_MODE_SWITCH_PARITY) {
        // Invalid parity
        return -1;
    }
    // - Read PHY mode id
    uint8_t phy_mode_id = (mode_switch_phr & MASK_MODE_SWITCH_PHY_MODE) >> SHIFT_MODE_SWITCH_PHY_MODE;

    // When mode switch PHR was received, change new configuration here to wait data packet and set mode switch state machine to 'PHR received'
    if (mac_change_mode_switch_configuration(rf_ptr, phy_mode_id, MAC_MS_PHR_RECEIVED)) {
        return -1;
    }
    // Backup timer, if reception of data packet fails
    timer_mac_start(rf_ptr, MAC_MODE_SWITCH_TIMEOUT, mac_calculate_timeout(rf_ptr, phy_mode_id) / 50);
    return 0;
}

int8_t mac_update_mode_switch_state(protocol_interface_rf_mac_setup_s *rf_ptr, mac_mode_switch_states_e state, uint8_t phy_mode_id)
{
    switch (state) {
        case MAC_MS_PHR_SEND_READY:
            // When mode switch PHR was sent, change new configuration here before transmitting data packet and set mode switch state machine to 'PHR send ready'
            if (!mac_change_mode_switch_configuration(rf_ptr, phy_mode_id, MAC_MS_PHR_SEND_READY)) {
                return 0;
            }
            break;
        case MAC_MS_DATA_SEND_READY:
            // When data packet was sent, switch back to base configuration and set mode switch state machine to 'idle'
            if ((rf_ptr->mode_switch_state == MAC_MS_PHR_SEND_READY) && !mac_change_mode_switch_configuration(rf_ptr, phy_mode_id, MAC_MS_IDLE)) {
                return 0;
            }
            break;
        case MAC_MS_DATA_RECEIVED:
            // When packet was received in new configuration, switch back to base configuration and set mode switch state machine to 'idle'
            if ((rf_ptr->mode_switch_state == MAC_MS_PHR_RECEIVED) && !mac_change_mode_switch_configuration(rf_ptr, phy_mode_id, MAC_MS_IDLE)) {
                // Packet was received, stop backup timer to not cause timeout event
                timer_mac_stop(rf_ptr);
                return 0;
            }
            break;
        case MAC_MS_TIMEOUT:
            // When reception timeout occurs, switch back to base configuration and set mode switch state machine to 'idle'
            if ((rf_ptr->mode_switch_state == MAC_MS_PHR_RECEIVED) && !mac_change_mode_switch_configuration(rf_ptr, phy_mode_id, MAC_MS_IDLE)) {
                return 0;
            }
            break;
        default:
            break;
    }
    return -1;
}
