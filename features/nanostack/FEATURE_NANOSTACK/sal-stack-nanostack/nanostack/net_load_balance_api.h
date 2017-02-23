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

/**
 * \file net_load_balance_api.h
 * \brief 6Lowpan network load balance control API.
 */

#ifndef NET_LOAD_BALANCE_API_H_
#define NET_LOAD_BALANCE_API_H_

#include "ns_types.h"

/**
 * \brief load_balance_network_switch_notify this function will be called by load balance when it have detected better network to switch
 * \param interface id
 *
 * \return true Network switching can be performed immediately
 * \return false means that load balance will ask at a later time for network switching, if a better network is still available at that time
 */
typedef bool net_load_balance_network_switch_notify(int8_t interface_id);

/**
 * \brief Set user callback for accept network switch.
 * \param network_switch_notify user callback
 */
void net_load_balance_network_switch_cb_set(net_load_balance_network_switch_notify *network_switch_notify);

/**
 * \brief Create and enable load balance to selected interface.
 * \param interface_id interface id
 * \param enable_periodic_beacon_interval Set True when want activate load balance periodic beacon, false will work only properly with fhss system
 *
 * \return 0 Enable ok
 * \return -1 unknown Interface or parameter error
 * \return -2 Out of memory
 * \return -3 Load balance already configured to this interface
 */
int8_t net_load_balance_create(int8_t interface_id, bool enable_periodic_beacon_interval);

/**
 * \brief Disable and delete load balansing from interface
 * \param interface_id interface id
 *
 * \return 0 Process ok
 * \return -1 unknown Interface
 */
int8_t net_load_balance_delete(int8_t interface_id);

/**
 * \brief Set Load balance threshold min and max
 *
 *  Nework switch will work next  diff_priority >= randLIB_get_random_in_range(threshold_min, threshold_max) --> switch network if true
 *  For border router Disable Network compare set threshold_min and threshold_max to 0.
 * \param threshold_min min value define random minimal value for compare
 * \param threshold_max max value for define random max value for compare
 *
 * \return 0 process ok -1 Unknown interface id
 */
int8_t net_load_balance_threshold_set(int8_t interface_id, uint8_t threshold_min, uint8_t threshold_max);

/**
 * \brief Set Load balance expected device count and enable automatic network load level update
 *
 * This feature is just for RPL DoDAG root device!
 *
 * \param interface_id interface id
 * \param expected_device_count Device count which will set max load level. If count is not expected_device_count % 8 it not zero function update number up to reach that.It is not hard limit it define max DoDAG preference
 *
 * \return 0 process ok -1 Unknown interface id -2 Out of memory
 */
int8_t net_load_balance_load_level_update_enable(int8_t interface_id, uint16_t expected_device_count);

/**
 * \brief Disable automatic network load level update
 *
 * \param interface_id interface id
 *
 * \return 0 process ok -1 Unknown interface id
 */
int8_t net_load_balance_load_level_update_disable(int8_t interface_id);

/**
 * \brief Set network probability percent when new network is better than threshold max
 *
 * \param interface_id interface id
 * \param max_p is percent probability to switch network. Default is 40%. Accepted values are [1,100] recommend values are 15-50.
 *
 * \return 0 process ok -1 Unknown interface id or parameter fail
 */
int8_t net_load_balance_set_max_probability(int8_t interface_id , uint8_t max_p);


#endif /* NET_LOAD_BALANCE_API_H_ */
