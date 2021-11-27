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

#ifndef MAC_MODE_SWITCH_H_
#define MAC_MODE_SWITCH_H_

#define PHR_LEN 2

#define SHIFT_MODE_SWITCH_PARITY    (15)
#define SHIFT_MODE_SWITCH_CHECKSUM  (11)
#define SHIFT_MODE_SWITCH_PHY_MODE  (3)
#define SHIFT_MODE_SWITCH           (0)
#define MASK_MODE_SWITCH_PARITY     (0x8000)
#define MASK_MODE_SWITCH_CHECKSUM   (0x7800)
#define MASK_MODE_SWITCH_PHY_MODE   (0x07F8)
#define MASK_MODE_SWITCH            (0x0001)

/**
 * @brief Build mode switch PHR.
 * @param rf_ptr Pointer to MAC instance.
 * @param data_ptr Pointer to data buffer.
 * @param phy_mode_id Used PHY mode id.
 * @return 0 - success, -1 - failure.
 */
int8_t mac_build_mode_switch_phr(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t *data_ptr, uint8_t phy_mode_id);

/**
 * @brief Parse mode switch PHR.
 * @param rf_ptr Pointer to MAC instance.
 * @param data_ptr Pointer to data buffer.
 * @param data_len Data length.
 * @return 0 - mode switch PHR found, -1 - mode switch PHR not found.
 */
int8_t mac_parse_mode_switch_phr(protocol_interface_rf_mac_setup_s *rf_ptr, const uint8_t *data_ptr, uint16_t data_len);

/**
 * @brief Update mode switch state.
 * @param rf_ptr Pointer to MAC instance.
 */
int8_t mac_update_mode_switch_state(protocol_interface_rf_mac_setup_s *rf_ptr, mac_mode_switch_states_e state, uint8_t phy_mode_id);

#endif /* MAC_MODE_SWITCH_H_ */
