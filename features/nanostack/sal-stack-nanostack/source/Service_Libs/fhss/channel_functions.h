/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef CHANNEL_FUNC_H_
#define CHANNEL_FUNC_H_

/**
 * @brief Compute the unicast schedule channel index using tr51 channel function.
 * @param slot_number Current slot number.
 * @param mac MAC address of the node for which the index is calculated.
 * @param number_of_channels Number of channels.
 * @param excluded_channels Excluded channels.
 * @return Channel index.
 */
int32_t tr51_get_uc_channel_index(uint16_t slot_number, uint8_t *mac, int16_t number_of_channels, uint32_t *excluded_channels);

/**
 * @brief Compute the broadcast schedule channel index using tr51 channel function.
 * @param slot_number Current slot number.
 * @param bsi Broadcast schedule identifier of the node for which the index is calculated.
 * @param number_of_channels Number of channels.
 * @param excluded_channels Excluded channels.
 * @return Channel index.
 */
int32_t tr51_get_bc_channel_index(uint16_t slot_number, uint16_t bsi, int16_t number_of_channels, uint32_t *excluded_channels);

/**
 * @brief Compute the unicast schedule channel index using direct hash channel function.
 * @param slot_number Current slot number.
 * @param mac MAC address of the node for which the index is calculated.
 * @param number_of_channels Number of channels.
 * @return Channel index.
 */
int32_t dh1cf_get_uc_channel_index(uint16_t slot_number, uint8_t *mac, int16_t number_of_channels);

/**
 * @brief Compute the broadcast schedule channel index using direct hash channel function.
 * @param slot_number Current slot number.
 * @param bsi Broadcast schedule identifier of the node for which the index is calculated.
 * @param number_of_channels Number of channels.
 * @return Channel index.
 */
int32_t dh1cf_get_bc_channel_index(uint16_t slot_number, uint16_t bsi, int16_t number_of_channels);

#endif /*CHANNEL_FUNC_H_*/
