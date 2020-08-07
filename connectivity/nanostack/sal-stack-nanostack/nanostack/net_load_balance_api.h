/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
 * \file net_load_balance_api.h
 * \brief 6LoWPAN network load balance control API.
 */

#ifndef NET_LOAD_BALANCE_API_H_
#define NET_LOAD_BALANCE_API_H_

#include "ns_types.h"

/**
 * \brief load_balance_network_switch_notify This function is called by the load balancer when it has detected a better network to switch to.
 *
 * \return true The network can be switched immediately.
 * \return false The load balancer will ask later a time for network switching if a better network is still available at that time.
 */
typedef bool net_load_balance_network_switch_notify(void);

/**
 * \brief Set user callback for accepting the network switch.
 * \param interface_id Interface ID.
 * \param network_switch_notify User callback.
 *
 * \return 0 Set OK.
 * \return -1 unknown Interface.
 */
int8_t net_load_balance_network_switch_cb_set(int8_t interface_id, net_load_balance_network_switch_notify *network_switch_notify);

/**
 * \brief Create and enable load balance to the selected interface.
 * \param interface_id Interface ID.
 * \param enable_periodic_beacon_interval Set True when you want to activate load balance periodic beacon; false will work only properly with the FHSS system.
 *
 * \return 0 Enable OK.
 * \return -1 unknown Interface or parameter error.
 * \return -2 Out of memory.
 * \return -3 Load balance already configured to this interface.
 */
int8_t net_load_balance_create(int8_t interface_id, bool enable_periodic_beacon_interval);

/**
 * \brief Disable and delete load balancing from the interface.
 * \param interface_id Interface ID.
 *
 * \return 0 Process OK.
 * \return -1 Unknown interface.
 */
int8_t net_load_balance_delete(int8_t interface_id);

/**
 * \brief Set load balance threshold min and max.
 *
 *  Network switch: diff_priority >= randLIB_get_random_in_range(threshold_min, threshold_max) --> switch network if true.
 *  For border router: Disable network compare by setting threshold_min and threshold_max to 0.
 *
 * \param interface_id Interface ID.
 * \param threshold_min Min value defines a random minimum value for compare (must be bigger than 0).
 * \param threshold_max Max value defines a random maximum value for compare (must be bigger than 0).
 *
 * \return 0 Process OK, -1 Unknown interface ID.
 */
int8_t net_load_balance_threshold_set(int8_t interface_id, uint8_t threshold_min, uint8_t threshold_max);


/**
 * \brief Set the network probability percent when the new network is better than threshold max.
 *
 * \param interface_id Interface ID.
 * \param max_p Probability percent to switch the network. Default is 25%. Accepted values are [1,100], recommend values are 10-25.
 *
 * \return 0 Process OK, -1 Unknown interface ID or parameter fail.
 */
int8_t net_load_balance_set_max_probability(int8_t interface_id, uint8_t max_p);

/**
 * \brief Set load balance expected device count and enable automatic network load level update.
 *
 * This feature is just for RPL DoDAG root device!
 *
 * \param interface_id Interface ID.
 * \param expected_device_count Device count that sets the max load level.
 *        - If the count is not divisible by 8, the function rounds the number up to reach that.
 *        - It is not hard limit it define max DoDAG preference.
 *        - For hard limit, check whiteboard_api.h.
 *
 * \return 0 Process OK, -1 Unknown interface ID, -2 Out of memory.
 */
int8_t net_load_balance_load_level_update_enable(int8_t interface_id, uint16_t expected_device_count);

/**
 * \brief Disable the automatic network load level update.
 *
 * \param interface_id Interface ID.
 *
 * \return 0 Process OK, -1 Unknown interface ID.
 */
int8_t net_load_balance_load_level_update_disable(int8_t interface_id);



#endif /* NET_LOAD_BALANCE_API_H_ */
