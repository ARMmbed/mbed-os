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

/**
 * \file load_balance_api.h
 * \brief A API class to support 6lowpan network load balance which based on RPL routing.
 */

#ifndef LOAD_BALANCE_API_H
#define LOAD_BALANCE_API_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "net_load_balance_api.h"

typedef struct load_balance_api load_balance_api_t;

struct mlme_beacon_ind_s;
struct mlme_pan_descriptor_s;

typedef enum {
    LB_ROUTER_LEAVE,
    LB_NEIGHBOUR_LEAVE
} load_balance_nwk_switch_operation;

/**
 * Application which want enable load balance must first create load balance class, e.g 'load_balance_api_t* load_balance_create()'
 * In the function external Nanostack needs to fill necessary function pointers so that Upper layer can use it.
 * For load balance user define:
 *   - load_balance_beacon_notify: Beacon notify should call this api
 *   - load_balance_api_initialize: Initialise load balance class with next function pointer
 *   - load_balance_enable
 * Load Balance requested function pointers
 *   - load_balance_priority_get
 *   - load_balance_network_switch_req
 *   - load_balance_beacon_interval_set
 */

/**
 * \brief load_balance_beacon_notify
 * \param api load_balance_api_t pointer
 * \param beacon_ind standard beacon indication structure
 * \param priority Beacon parsed network priority
 */
typedef void load_balance_beacon_notify(const load_balance_api_t* api, const struct mlme_beacon_ind_s *beacon_ind, uint8_t priority);

/**
 * \brief load_balance_second_ticks update load balance state timer
 * \param load_balance_api_t pointer
 */
typedef void load_balance_second_ticks(const load_balance_api_t* api);

/**
 * \brief load_balance_enabled handler for enabled or disable beacon notify
 * \param load_balance_api_t pointer
 * \param active_state true enable load balancer and false disable
 * \param network_triggle_max_period time that load balance will send periodic beacon
 * \param network_route_life_time time for device route lifetime
 */
typedef void load_balance_enable(const load_balance_api_t* api,
		bool active_state, uint32_t network_triggle_max_period,
		uint32_t network_route_life_time);

/**
 * \brief load_balance_priority_get function for load balance to get current advertisement priority
 * \param load_balance_user pointer to load balancer user initialised data
 *
 * \return priority
 */
typedef uint8_t load_balance_priority_get(const void * load_balance_user);

/**
 * \brief load_balance_beacon_tx function for trig periodically beacon
 * \param load_balance_user pointer to load balancer user initialised data
 *
 */
typedef void load_balance_beacon_tx(const void *load_balance_user);

/**
 * \brief load_balance_network_switch_notify this function will be called by load balance when it have detected better network to switch change state
 * \param load_balance_user pointer to load balancer user initialised data
 * \param operation network switch operation
 * \param timeout pointer where operation time must be define NULL when load balance not need time out or user set 0
 *
 * \return true Network switching can be performed immediately
 * \return false Network switching will be performed later
 */
typedef bool load_balance_network_switch_notify(const void *load_balance_user, load_balance_nwk_switch_operation operation, uint16_t *timeout);

/**
 * \brief load_balance_network_switch_req this function will be called by load balance when it have detected better network to switch
 * \param load_balance_user pointer to load balancer user initialised data
 * \param PANDescriptor pointer to link and coordinator info
 * \param beacon_payload beacon payload pointer for new network
 * \param beacon_payload_length payload length
 *
 * \return true network switch started
 * \return false network switch fail
 */
typedef bool load_balance_network_switch_req(void * load_balance_user, struct mlme_pan_descriptor_s *PANDescriptor, const uint8_t *beacon_payload, uint16_t beacon_payload_length);


/**
 * \brief load_balance_api_get_node_count Get border router registered node count
 * \param lb_user Load balancing user defined unique data.
 * \param node_count Pointer where node count is stored
 * \return -1 Get fail, 0 node_count include current node count
 */
typedef int8_t load_balance_api_get_node_count(void *lb_user, uint16_t *node_count);

/**
 * \brief load_balance_api_set_load_level Update new load level to network
 * \param lb_user Load balancing user defined unique data.
 * \param load_level load level which could be [0,CONFIGURED_SCALER -1]
 * \return -1 Set fail, 0 New Network Load is updated
 */
typedef int8_t load_balance_api_set_load_level(void *lb_user, uint8_t load_level);

/**
 * \brief load_balance_api_initialize Initialises load balancer needed callback.
 * \param api load_balance_api_t pointer, which is created by application.
 * \param lb_beacon_tx_period_set_cb Upper layer function to handle MCPS confirmations
 * \param priority_get_cb Upper layer function to handle MCPS indications
 * \param lb_nwk_switch_cb
 * \param  beacon_max_payload_length Set 0 if network switch is disabled otherwise give max supported beacon payload.
 * \param lb_user Load balancing user defined unique data
 * \return -1 if error, -2 if Out of Memory, 0 otherwise
 */
typedef int8_t load_balance_api_initialize(load_balance_api_t *api, load_balance_beacon_tx *lb_beacon_tx,
                                           load_balance_priority_get *priority_get_cb, load_balance_network_switch_req *lb_nwk_switch_cb, uint16_t beacon_max_payload_length, void *lb_user);

/**
 * \brief Struct load_balance_api_s defines functions for two-way communications between load balance and user.
 * Nanostack creates load_balance_api_s object by calling load_balance_create() function.
 * Then object is passed to Upper layer which then initialises it's own callback functions.
 */
struct load_balance_api {
    load_balance_api_initialize *lb_initialize;
    load_balance_enable         *lb_enable;
    load_balance_beacon_notify  *lb_beacon_notify;
    load_balance_second_ticks   *lb_seconds_tick_update;
};

/**
 * \brief Create load balance class
 * \param lb_notify_cb user callback for accept network switch
 * \param activate_periodic_beacon True define that load balance will generate periodic beacon based configured value at enable
 *
 * \return Pointer to load balance class
 */
load_balance_api_t *load_balance_create(load_balance_network_switch_notify *lb_notify_cb, bool activate_periodic_beacon);

/**
 * \brief Delete load balance class and free memory
 * \param api Class pointer to deleted class
 *
 * \return 0 process ok -1 Unknown class
 */
int load_balance_delete(load_balance_api_t *api);

/**
 * \brief Set load balance threshold min and max
 *
 * Nework switch will work next  diff_priority >= randLIB_get_random_in_range(threshold_min, threshold_max) --> switch network if true
 * For border router Disable Network compare set threshold_min and threshold_max to 0.
 *
 * \param api Class pointer to deleted class
 * \param threshold_min minimal threshold value for network switch
 * \param threshold_max max threshold value for network switch
 *
 * \return 0 process ok -1 Unknown class or unsupported threshold
 */
int load_balance_network_threshold_set(load_balance_api_t *api, uint8_t threshold_min, uint8_t threshold_max);

/**
 * \brief Enable load balance load monitor and update feature
 *
 * This feature is for border router's defined function for give network scale and load parameter what load balancer will follow automatically.
 *
 * \param api Class pointer to deleted class
 * \param expected_node_count node which define max load to network
 * \param network_load_scaler Define max load value
 * \param get_count_cb function pointer for read current node count
 * \param set_new_load_cb function pointer what is called when load level must be update
 *
 * \return 0 process ok -1 Unknown class or unsupported parameter
 */
int load_balance_network_load_monitor_enable(load_balance_api_t *api, uint16_t expected_node_count, uint8_t network_load_scaler, load_balance_api_get_node_count *get_count_cb, load_balance_api_set_load_level *set_new_load_cb);

/**
 * \brief Disable load balance load monitor and update feature
 *
 * \param api Class pointer to deleted class
 *
 * \return 0 process ok -1 Unknown class
 */
int load_balance_network_load_monitor_disable(load_balance_api_t *api);

/**
 * \brief Set network probability percent when new network is better than threshold max
 *
 * \param api Class pointer to deleted class
 * \param max_p is percent probability to switch network. Default is 25%. Accepted values are [1,100] recommend values are 10-25.
 *
 * \return 0 process ok -1 Unknown class or unsupported parameter
 */
int load_balance_set_max_probability(load_balance_api_t *api, uint8_t max_p);

/**
 * \brief Set user callback for accepting the network switch.
 * \param api Class pointer to deleted class.
 * \param network_switch_notify User callback.
 *
 * \return 0 Set OK.
 * \return -1 unknown Interface.
 */
int load_balance_network_switch_cb_set(load_balance_api_t *api, net_load_balance_network_switch_notify *network_switch_notify);

#ifdef __cplusplus
}
#endif

#endif // LOAD_BALANCE_API_H
