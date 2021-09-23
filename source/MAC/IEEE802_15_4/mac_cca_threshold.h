/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef MAC_CCA_THRESHOLD_H_
#define MAC_CCA_THRESHOLD_H_

#define CCA_THRESHOLD_STEP  1
#define CCA_FAILED_DBM      0x7F

typedef struct mac_cca_threshold {
    int8_t *ch_thresholds;
    int8_t high_limit;
    int8_t low_limit;
    int8_t default_dbm;
    uint8_t number_of_channels;
} mac_cca_threshold_s;

/**
 * @brief Initialize automatic CCA threshold.
 * @param rf_ptr Pointer to MAC instance.
 * @param number_of_channels Number of MAC channels.
 * @param default_dbm Default threshold.
 * @param high_limit Highest allowed CCA threshold.
 * @param low_limit Lowest allowed CCA threshold.
 * @return 0 Success, negative Failed.
 */
int8_t mac_cca_thr_init(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t number_of_channels, int8_t default_dbm, int8_t high_limit, int8_t low_limit);

/**
 * @brief Deinitialize automatic CCA threshold.
 * @param rf_ptr Pointer to MAC instance.
 * @return 0 Success, negative Not found.
 */
int8_t mac_cca_thr_deinit(protocol_interface_rf_mac_setup_s *rf_ptr);

/**
 * @brief Read CCA threshold of specific channel.
 * @param rf_ptr Pointer to MAC instance.
 * @param channel Channel.
 * @return CCA threshold (dBm), CCA_FAILED_DBM Feature not enabled.
 */
int8_t mac_cca_thr_get_dbm(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel);

/**
 * @brief Update CCA threshold of specific channel.
 * @param rf_ptr Pointer to MAC instance.
 * @param channel Channel.
 * @param dbm CCA threshold (dBm).
 * @return 0 Updated, negative Already using this value.
 */
int8_t mac_cca_threshold_update(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel, int8_t dbm);

/**
 * @brief Get pointer to CCA threshold table.
 * @param rf_ptr Pointer to MAC instance.
 * @return CCA threshold table.
 */
mac_cca_threshold_s *mac_get_cca_threshold_table(protocol_interface_rf_mac_setup_s *rf_ptr);

#endif /* MAC_CCA_THRESHOLD_H_ */
