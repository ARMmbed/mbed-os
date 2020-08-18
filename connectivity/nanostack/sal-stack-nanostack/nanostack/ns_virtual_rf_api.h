/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

/** \file ns_virtual_rf_api.h
 * \brief Virtual RF interface API.
 */

#ifndef NS_VIRTUAL_RF_API_H_
#define NS_VIRTUAL_RF_API_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "platform/arm_hal_phy.h"


/**
 * @brief virtual_rf_device_register Creates virtual RF device
 * Emulates "radio" link like the real RF driver would do it. MAC does not know the difference.
 * Sends and listens data to/from serial MAC.
 * @param link_type Link type behind virtual device
 * @param mtu_size Maximum transmission unit size
 * @return Rf driver id
 */
int8_t virtual_rf_device_register(phy_link_type_e link_type, uint16_t mtu_size);

/**
 * @brief virtual_rf_client_register Creates virtual RF client
 * "Acts" in a role of upper layer for MAC (802.15.4 or ethernet) and in a role of RF driver for serial MAC.
 * Forwards data between serial MAC and either 802.15.4 or ethernet MAC.
 * @return Rf driver id
 */
int8_t virtual_rf_client_register(void);


#ifdef __cplusplus
}
#endif
#endif /* NS_VIRTUAL_RF_API_H_ */
