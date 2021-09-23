/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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

/**
 * \file mac_filter_api.h
 * \brief API to allow filtering of packets based upon link quality and received power levels.
 *
 * \section app-mac-fil Applying a  MAC filter
 *  - mac_filter_start(), Starts a MAC level filter.
 *  - mac_filter_add_short(), Adds a filter using short MAC address.
 *  - mac_filter_add_long(), Adds a filter using long MAC address.
 *
 * \section rem-mac-fil Removing a  MAC filter
 *  - mac_filter_stop(), Stops MAC level filter and clears all defaults.
 *  - mac_filter_clear(), Stops MAC level filter and leaves the default link configuration.
 *  - mac_filter_delete_short(), Deletes filter for a device specified by short MAC address.
 *  - mac_filter_delete_long(), Deletes filter for a device specified by long MAC address.
 *
 *  \section reg-mac-fil Setting up address mapping for filter
 *  -  mac_filter_set_address_mapping(), Registers address mapping functions.
 *
 * \section macr-help Helper macros
 * - mac_filter_start(interface_id, MAC_FILTER_BLOCKED), Black list filter.
 * - mac_filter_start(interface_id, MAC_FILTER_ALLOWED), White list filter not modifying the qualities.
 * - mac_filter_start(interface_id, MAC_FILTER_FIXED(0x01, -80)), Fixed value for default link quality (poor quality).
 * - mac_filter_start(interface_id, MAC_FILTER_FIXED(0xff, -20)), Fixed value for default link quality (good quality).
 * - mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_BLOCKED)
 * - mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_ALLOWED)
 * - mac_filter_add_short(interface_id, 0x0001, MAC_FILTER_FIXED(0xff,-30))
 * - mac_filter_add_long(interface_id, mac64, MAC_FILTER_BLOCKED)
 * - mac_filter_add_long(interface_id, mac64, MAC_FILTER_ALLOWED)
 * - mac_filter_add_long(interface_id, mac64, MAC_FILTER_FIXED(0x7f, -60))
 */


#ifndef MAC_FILTER_API_H_
#define MAC_FILTER_API_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Start MAC level filter.
 * This function can be called again if default values are modified.
 *
 *
 * \param interface_id Network interface ID.
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier).
 * \param lqi_add Value added to LQI.
 * \param dbm_m DBM multiplier (8.8 Fixed point multiplier).
 * \param dbm_add Value added to DBM.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */

int_fast8_t mac_filter_start(int8_t interface_id, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

/**
 * \brief Stop MAC level filter.
 *
 * Stops the filtering module and clears the default settings and all the filterings made.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */

void mac_filter_stop(int8_t interface_id);

/** White list filter not modifying the qualities.*/
#define MAC_FILTER_ALLOWED 0x100, 0, 0x100, 0
/** Black list filter.*/
#define MAC_FILTER_BLOCKED 0, 0, 0, 0
/** Fixed value for default link quality.*/
#define MAC_FILTER_FIXED(lqi,dbm) 0, lqi, 0, dbm

/**
 * \brief Delete all filters.
 *
 * Leaves the default link configuration.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */
int_fast8_t mac_filter_clear(int8_t interface_id);

/**
 * \brief Map the extended address to the short address.
 *
 * \param interface_id Network Interface.
 * \param mac64 Extended address.
 * \param mac16 Return the short address.
 *
 * \return 0, address resolved.
 * \return <0 No mapping found.
 */
typedef int_fast8_t (mac_filter_map_extented_to_short_cb)(int8_t interface_id, uint8_t mac64[8], uint16_t *mac16);

/**
 * \brief Map short address to extended address.
 *
 * \param interface_id Network interface ID.
 * \param mac64[out] Return buffer for the extended address.
 * \param mac16 Short address.
 *
 * return 0, address resolved.
 * return <0 no mapping found.
 */
typedef int_fast8_t (mac_filter_map_short_to_extended_cb)(int8_t interface_id, uint8_t mac64[8], uint16_t mac16);

/**
 * \brief Register address mapping functions.
 *
 * This function should be added for layer in stack that keeps up the list of address mapping functions.
 * If filters are enabled these functions are called if no match from filters was found.
 *
 * When this service is no longer provided call this function with NULL pointers.
 *
 * \param interface_id Network interface ID.
 * \param long_cb Address mapping to resolve long address from short address.
 * \param short_cb Address mapping to resolve short address from long address.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */
int_fast8_t mac_filter_set_address_mapping(int8_t interface_id, mac_filter_map_short_to_extended_cb *long_cb, mac_filter_map_extented_to_short_cb *short_cb);

/**
 * \brief Delete filter for device
 *
 * \param interface_id Network interface ID.
 * \param mac16 Short address.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */
int_fast8_t mac_filter_delete_short(int8_t interface_id, uint16_t mac16);

/**
 * \brief Delete filter for device.
 *
 * \param interface_id Network interface ID.
 * \param mac64 Long address.
 *
 * \return 0, OK
 * \return <0 Not OK
 */
int_fast8_t mac_filter_delete_long(int8_t interface_id, uint8_t mac64[8]);

/**
 * \brief Add MAC short address filter.
 *
 * \param interface_id Network interface ID.
 * \param mac16 Short address.
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier).
 * \param lqi_add Value added to LQI.
 * \param dbm_m DBM multiplier (8.8 Fixed point multiplier).
 * \param dbm_add Value added to DBM.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */
int_fast8_t mac_filter_add_short(int8_t interface_id, uint16_t mac16, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

/**
 * \brief Add MAC long address filter.
 *
 * \param interface_id Network interface ID.
 * \param mac64 Long address.
 * \param lqi_m LQI multiplier (8.8 Fixed point multiplier).
 * \param lqi_add Value added to LQI.
 * \param dbm_m DBM multiplier (8.8 Fixed point multiplier).
 * \param dbm_add Value added to DBM.
 *
 * \return 0, OK.
 * \return <0 Not OK.
 */
int_fast8_t mac_filter_add_long(int8_t interface_id, uint8_t mac64[8], int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add);

#ifdef __cplusplus
}
#endif

#endif /* MAC_FILTER_API_H_ */
