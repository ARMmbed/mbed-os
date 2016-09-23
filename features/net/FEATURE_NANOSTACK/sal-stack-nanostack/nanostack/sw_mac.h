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


#ifdef __cplusplus
}
#endif

#endif // NS_SW_MAC_H
