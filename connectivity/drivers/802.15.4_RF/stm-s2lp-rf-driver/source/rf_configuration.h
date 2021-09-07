/*
 * Copyright (c) 2018-2020, Pelion and affiliates.
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

#ifndef RF_CONF_H_
#define RF_CONF_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RSSI_OFFSET     146

void rf_conf_calculate_datarate_registers(uint32_t datarate, uint16_t *datarate_mantissa, uint8_t *datarate_exponent);
void rf_conf_calculate_base_frequency_registers(uint32_t frequency, uint8_t *synt3, uint8_t *synt2, uint8_t *synt1, uint8_t *synt0);
void rf_conf_calculate_deviation_registers(uint32_t deviation, uint8_t *fdev_m, uint8_t *fdev_e);
int rf_conf_calculate_channel_spacing_registers(uint32_t channel_spacing, uint8_t *ch_space);
void rf_conf_calculate_rx_filter_bandwidth_registers(uint32_t rx_bandwidth, uint8_t *chflt_m, uint8_t *chflt_e);
void rf_conf_calculate_rssi_threshold_registers(int16_t rssi_threshold, uint8_t *rssi_th);
uint32_t rf_conf_calculate_deviation(phy_modulation_index_e modulation_index, uint32_t datarate);
int16_t rf_conf_cca_threshold_percent_to_rssi(uint8_t percent);

#ifdef __cplusplus
}
#endif

#endif /* RF_CONF_H_ */
