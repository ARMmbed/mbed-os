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

#ifndef WS_PHY_H_
#define WS_PHY_H_

uint32_t ws_phy_decode_channel_spacing(uint8_t channel_spacing);

uint32_t ws_phy_get_datarate_using_operating_mode(uint8_t operating_mode);

uint8_t ws_phy_convert_operating_class_to_channel_plan_id(uint8_t operating_class, uint8_t regulatory_domain);

uint8_t ws_phy_convert_operating_mode_to_phy_mode_id(uint8_t operating_mode);

uint8_t ws_phy_get_channel_plan_id_using_phy_mode_id(uint8_t phy_mode_id, uint8_t regulatory_domain, uint8_t base_channel_plan_id);

uint32_t ws_phy_get_datarate_using_phy_mode_id(uint8_t phy_mode_id);

uint8_t ws_phy_get_ofdm_option_using_phy_mode_id(uint8_t phy_mode_id);

uint8_t ws_phy_get_ofdm_mcs_using_phy_mode_id(uint8_t phy_mode_id);

uint16_t ws_phy_get_number_of_channels_using_channel_plan_id(uint8_t channel_plan_id);

uint32_t ws_phy_get_channel_spacing_using_channel_plan_id(uint8_t channel_plan_id);

uint32_t ws_phy_get_channel_0_frequency_using_channel_plan_id(uint8_t channel_plan_id);

bool ws_phy_get_fsk_fec_enabled_using_phy_mode_id(uint8_t phy_mode_id);

phy_modulation_e ws_phy_get_modulation_using_phy_mode_id(uint8_t phy_mode_id);

phy_modulation_index_e ws_phy_get_modulation_index_using_phy_mode_id(uint8_t phy_mode_id);

phy_modulation_index_e ws_phy_get_modulation_index_using_operating_mode(uint8_t operating_mode);

#endif //WS_PHY_H_
