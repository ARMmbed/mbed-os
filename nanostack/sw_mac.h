/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

/** \file sw_mac.h
 * \brief Software MAC API.
 */

#ifndef NS_SW_MAC_H
#define NS_SW_MAC_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct protocol_interface_rf_mac_setup;
struct mac_api_s;
struct mac_description_storage_size_s;
struct fhss_api;
struct mac_statistics_s;
struct phy_rf_statistics_s;

/**
 * @brief Creates 802.15.4 MAC API instance which will use RF driver given
 * @param rf_driver_id RF driver id. Must be valid
 * @param storage_sizes dynamic mac storage sizes DO NOT set any values to zero !!
 * @return New MAC instance if successful, NULL otherwise
 */
extern struct mac_api_s *ns_sw_mac_create(int8_t rf_driver_id, struct mac_description_storage_size_s *storage_sizes);

/**
 * @brief ns_sw_mac_virtual_client_register registers virtual driver to be used with 802.15.4 MAC.
 * This is always used with serial_mac_api
 * @param api API to start using virtual driver
 * @param virtual_driver_id
 * @return 0 if success, -1 if api or driver is invalid
 */
extern int8_t ns_sw_mac_virtual_client_register(struct mac_api_s *api, int8_t virtual_driver_id);

/**
 * @brief ns_sw_mac_virtual_client_unregister Unregisters virtual driver from 802.15.4 MAC
 * @param api API from which to unregister virtual driver
 * @return 0 if success, -1 if api is invalid
 */
extern int8_t ns_sw_mac_virtual_client_unregister(struct mac_api_s *api);

/**
 * @brief Registers created FHSS API instance to given software MAC instance.
 * @param mac_api MAC instance.
 * @param fhss_api FHSS instance.
 * @return 0 on success, -1 on fail.
 */
extern int ns_sw_mac_fhss_register(struct mac_api_s *mac_api, struct fhss_api *fhss_api);

/**
 * @brief Request registered FHSS API instance from software MAC instance.
 * @param mac_api MAC instance.
 * @return FHSS api.
 */
extern struct fhss_api *ns_sw_mac_get_fhss_api(struct mac_api_s *mac_api);

/**
 * @brief Start collecting statistics from software MAC.
 * @param mac_api MAC instance.
 * @param mac_statistics Statistics storage.
 * @return 0 on success, -1 on fail.
 */
extern int ns_sw_mac_statistics_start(struct mac_api_s *mac_api, struct mac_statistics_s *mac_statistics);

/**
 * @brief Start collecting statistics from PHY driver.
 * @param mac_api MAC instance.
 * @param phy_statistics Statistics storage.
 * @return 0 on success, -1 on fail.
 */
extern int ns_sw_mac_phy_statistics_start(struct mac_api_s *mac_api, struct phy_rf_statistics_s *phy_statistics);

/**
 * @brief Read current timestamp.
 * @param mac_api MAC instance.
 * @return Current timestamp in us
 */
extern uint32_t ns_sw_mac_read_current_timestamp(struct mac_api_s *mac_api);

#ifdef __cplusplus
}
#endif

#endif // NS_SW_MAC_H
