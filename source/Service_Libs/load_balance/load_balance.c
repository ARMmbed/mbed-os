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


#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "load_balance_api.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "mlme.h"

typedef enum {
    LB_NWK_SWITCH_IDLE = 0,
    LB_NWK_SWITCH_ROUTER_LEAVE,
    LB_NWK_SWITCH_NEIGHBOUR_LEAVE
} lb_nwk_switch_state_t;

typedef enum {
    LB_IDLE_STATE = 0,
    LB_BLOCK_STATE,
    LB_ACCEPT_STATE,
    LB_BLOCK_COMPARE,
    LB_BLOCK_NETWORK_SELECT,
} lb_state_t;

/**
 * @brief struct lb_network_s Load balance network information
 *
 */
typedef struct lb_network_s {
    struct mlme_pan_descriptor_s PANDescriptor;
    uint8_t priority;
    bool network_switch_accepted;
    lb_nwk_switch_state_t state;
    uint16_t state_timer;
    uint16_t beacon_data_length;
    uint8_t beacon_data[];
} lb_network_t;

/** get pointer to Mac header start point*/
#define beacon_payload_start_pointer(x)  (&(x)->beacon_data[0])

typedef struct lb_monitor_internal_s {
    load_balance_api_get_node_count *get_count_cb;
    load_balance_api_set_load_level *set_new_load_cb;
    uint16_t expected_node_count;
    uint8_t network_load_scaler;
    uint8_t last_load_level;
    uint16_t timer2update;
}lb_monitor_internal_t;


typedef struct lb_internal_s {
    load_balance_api_t *lb_api;
    lb_monitor_internal_t *lb_border_router;
    //user spesific callback
    load_balance_beacon_tx *load_balance_beacon_tx_cb;
    load_balance_priority_get *lb_priority_get_cb;
    load_balance_network_switch_req *lb_nwk_switch_cb;
    load_balance_network_switch_notify *lb_nwk_switch_notify; //Can be NULL if app not want to control
    //APP defined nwk switch callback
    net_load_balance_network_switch_notify *lb_access_switch_cb;
    void *lb_user_parent_id;
    lb_network_t *notified_network;
    uint32_t triggle_period;
    uint32_t lb_state_timer;
    uint32_t time_to_next_beacon;
    uint16_t beacon_max_payload_length;
    uint8_t nwk_switch_threshold_min;
    uint8_t nwk_switch_threshold_max;
    uint8_t nwk_maX_P;
    lb_state_t lb_state;
    bool load_balance_activate;
    bool periodic_beacon_activated;
}lb_internal_t;


/**
 * Load balance internal variables
 */
static load_balance_api_t lb_api;
static lb_internal_t *lb_store = NULL;

/**
 * Load balance internal used functions
 */
static lb_internal_t * load_balance_class_allocate(void);
static void load_balance_class_free(lb_internal_t *api);
static lb_internal_t *lb_api_get(const load_balance_api_t* api);
static bool load_balance_network_class_allocate(lb_internal_t *lb_store_ptr, uint16_t beacon_max_payload_length);
static void lb_network_switch_handle(lb_internal_t *this);
static void lb_load_level_poll(lb_internal_t * this, uint32_t trigle_period);

/**
 * Load balance shared functions to user
 */
static void lb_beacon_notify(const load_balance_api_t* api, const struct mlme_beacon_ind_s *beacon_ind, uint8_t priority);
static void lb_enable(const load_balance_api_t* api, bool active_state, uint32_t network_triggle_max_period, uint32_t network_route_life_time);
static int8_t lb_api_initialize(load_balance_api_t *api, load_balance_beacon_tx *lb_beacon_tx,
                                           load_balance_priority_get *priority_get_cb, load_balance_network_switch_req *lb_nwk_switch_cb, uint16_t baecon_max_payload_length, void *lb_user);
static void lb_second_ticks(const load_balance_api_t* api);

#define TRACE_GROUP "lba"

/**
 * Load balance border router class allocate
 */
static lb_monitor_internal_t *lb_border_router_api_allocate(lb_internal_t* api)
{
    if (!api->lb_border_router) {
        api->lb_border_router = ns_dyn_mem_alloc(sizeof(lb_monitor_internal_t));
    }
    return api->lb_border_router;
}

/**
 * Load balance border router class free
 */
static int8_t lb_border_router_api_free(lb_internal_t* api)
{
    if (api->lb_border_router) {
        ns_dyn_mem_free(api->lb_border_router);
        api->lb_border_router = NULL;
        return 0;
    }
    return -1;
}

/**
 * Allocate Load balance class base
 */
static lb_internal_t * load_balance_class_allocate(void)
{
    if (lb_store) {
        if (lb_store->lb_user_parent_id) {
            return NULL;
        }
        return lb_store;
    }

    lb_internal_t *store = ns_dyn_mem_alloc(sizeof(lb_internal_t));
    if (store) {

        store->lb_api = &lb_api;
        lb_api.lb_beacon_notify = lb_beacon_notify;
        lb_api.lb_enable = lb_enable;
        lb_api.lb_initialize = lb_api_initialize;
        lb_api.lb_seconds_tick_update = lb_second_ticks;

        store->lb_border_router = NULL;
        store->load_balance_beacon_tx_cb = NULL;
        store->lb_nwk_switch_cb = NULL;
        store->lb_priority_get_cb = NULL;
        store->lb_user_parent_id = NULL;
        store->notified_network = NULL;
        store->lb_access_switch_cb = NULL;
        store->beacon_max_payload_length = 0;
        store->nwk_switch_threshold_max = 0;
        store->nwk_switch_threshold_min = 0;
        store->nwk_maX_P = 25;
        store->load_balance_activate = false;
        store->time_to_next_beacon = 0;
        store->triggle_period = 0;
        lb_store = store;
    }

    return store;
}

/**
 * Free allocated load balance class
 */
static void load_balance_class_free(lb_internal_t *api)
{
    //Clean heared networks
    ns_dyn_mem_free(api->notified_network);
    lb_border_router_api_free(api);
    ns_dyn_mem_free(api);
}

/**
 * Load balance class get by user API pointer
 */
static lb_internal_t *lb_api_get(const load_balance_api_t* api)
{
    if (!api || !lb_store || lb_store->lb_api != api) {
        return NULL;
    }
    return lb_store;
}



/**
 * Allocate notified network class
 */
static bool load_balance_network_class_allocate(lb_internal_t *lb_store_ptr, uint16_t beacon_max_payload_length)
{
    ns_dyn_mem_free(lb_store_ptr->notified_network);
    lb_store_ptr->beacon_max_payload_length = 0;
    if (beacon_max_payload_length) {
        lb_store_ptr->notified_network = ns_dyn_mem_alloc(sizeof(lb_network_t) + beacon_max_payload_length);
        if (!lb_store_ptr->notified_network) {
            return false;
        }
        lb_store_ptr->notified_network->network_switch_accepted = false;
        lb_store_ptr->notified_network->priority = 0xff;
        lb_store_ptr->notified_network->state = LB_NWK_SWITCH_IDLE;
        lb_store_ptr->notified_network->state_timer = 0;
        lb_store_ptr->beacon_max_payload_length = beacon_max_payload_length;

    }
    return true;
}

/**
 * Switch to notified and selected network
 */
static void lb_network_switch_handle(lb_internal_t *this)
{
    lb_network_t *network_class = this->notified_network;
    if (network_class->state_timer) {
        network_class->state_timer--;
        return;
    }

    switch (network_class->state) {
        case LB_NWK_SWITCH_IDLE:
            if (!this->notified_network->network_switch_accepted) {
                if (this->lb_access_switch_cb && !this->lb_access_switch_cb() ) {
                    return;
                }
                this->notified_network->network_switch_accepted = true;
            }
            tr_info("Start NWK switch statemachine!");
            this->lb_nwk_switch_notify(this->lb_user_parent_id, LB_ROUTER_LEAVE, &network_class->state_timer);
            network_class->state = LB_NWK_SWITCH_ROUTER_LEAVE;
            break;

        case LB_NWK_SWITCH_ROUTER_LEAVE:
            tr_info("Router leave state!");
            this->lb_nwk_switch_notify(this->lb_user_parent_id, LB_NEIGHBOUR_LEAVE, &network_class->state_timer);
            network_class->state = LB_NWK_SWITCH_NEIGHBOUR_LEAVE;
            break;

        case LB_NWK_SWITCH_NEIGHBOUR_LEAVE:
            tr_info("Neighbour leave state!");
            //Disable beacon send
            lb_enable(this->lb_api, false, 0, 0);
            this->lb_nwk_switch_cb(this->lb_user_parent_id, &network_class->PANDescriptor, beacon_payload_start_pointer(network_class), network_class->beacon_data_length);
            break;
    }

}

static bool lb_accept_beacon_state(lb_state_t lb_state)
{
    //Check state
    switch (lb_state) {
        case LB_IDLE_STATE:
        case LB_BLOCK_STATE:
        case LB_BLOCK_COMPARE:
            return false;
        default:
            break;

    }
    return true;
}

/**
 * Beacon notify handler
 */
static void lb_beacon_notify(const load_balance_api_t* api, const struct mlme_beacon_ind_s *beacon_ind, uint8_t priority)
{
    lb_internal_t * this = lb_api_get(api);
    if (!beacon_ind || !this || !this->nwk_switch_threshold_max || !this->nwk_switch_threshold_min) {
        return;
    } else if (!lb_accept_beacon_state(this->lb_state)) {
        return;
    } else if (beacon_ind->beacon_data_length > this->beacon_max_payload_length) {
        return;
    }

    lb_network_t *nwk = this->notified_network;
    //Get current global part joining priority
    uint8_t current_priority = this->lb_priority_get_cb(this->lb_user_parent_id);

    if (this->lb_state == LB_ACCEPT_STATE) {
        //Compare new network priority
        if (current_priority < priority) {
            return;
        }

        uint16_t diff = current_priority - priority;
        uint16_t switch_prob; // 256 * percent (0-25600)

        if (diff >= this->nwk_switch_threshold_max) {
            switch_prob = this->nwk_maX_P * 256;
            tr_debug("diff %"PRIu16", tmax %"PRIu8" %"PRIu16, diff, this->nwk_switch_threshold_max, switch_prob);
        } else if (diff <= this->nwk_switch_threshold_min) {
            tr_debug("diff %"PRIu16", tmin %"PRIu8, diff, this->nwk_switch_threshold_min);
            switch_prob = 0;
        } else {
            // The multiplication could overflow 16-bit, even though the final result will be 16-bit
            switch_prob = (uint32_t) this->nwk_maX_P * 256 * (diff - this->nwk_switch_threshold_min) / (this->nwk_switch_threshold_max - this->nwk_switch_threshold_min);
            tr_debug("diff between min & max, diff %"PRIu16", tmax %"PRIu8" prob%"PRIu16, diff, this->nwk_switch_threshold_max, switch_prob);
        }

        if (switch_prob > randLIB_get_random_in_range(0, 25599)) {
            this->lb_state_timer = randLIB_get_random_in_range(1, 32);
            this->lb_state = LB_BLOCK_NETWORK_SELECT;
        }
        else {
            //Enter Block state
            this->lb_state_timer = this->triggle_period;
            this->lb_state = LB_BLOCK_COMPARE;
            return;
        }

    } else {
        //Compare priority to saved network
        if (priority > nwk->priority) {
            return;
        } else if (priority == nwk->priority && nwk->PANDescriptor.LinkQuality > beacon_ind->PANDescriptor.LinkQuality) {
            return;
        }
    }

    //Copy network info
    nwk->priority = priority;
    nwk->PANDescriptor = beacon_ind->PANDescriptor;
    memcpy(beacon_payload_start_pointer(nwk), beacon_ind->beacon_data, beacon_ind->beacon_data_length);
    nwk->beacon_data_length = beacon_ind->beacon_data_length;
}
/**
 * Load balance activate or disable
 */
static void lb_enable(const load_balance_api_t* api, bool active_state, uint32_t network_triggle_max_period, uint32_t network_route_life_time)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this || !this->lb_user_parent_id) {
        return;
    }

    this->load_balance_activate = active_state;

    if (active_state) {
        //Start Block_period
        this->triggle_period = network_triggle_max_period;
        this->lb_state = LB_BLOCK_STATE;
        tr_debug("Enable Network Triggle max time %"PRIu32" %"PRIu32" route life time", network_triggle_max_period, network_route_life_time);
        if (this->nwk_switch_threshold_min && this->nwk_switch_threshold_max) {
            this->lb_state_timer = network_route_life_time > network_triggle_max_period ? network_route_life_time : network_triggle_max_period;
        } else {
            this->lb_state_timer = 10;
            if (this->lb_border_router) {
                this->lb_border_router->timer2update = 0;
                lb_load_level_poll(this, this->triggle_period);
            }
        }

    } else {
        this->lb_state = LB_IDLE_STATE;
    }

    if (this->notified_network) {
        this->notified_network->priority = 0xff;
        this->notified_network->network_switch_accepted = false;
        this->notified_network->state = LB_NWK_SWITCH_IDLE;
        this->notified_network->state_timer = 0;
    }
}

/**
 * Load balance activate or disable
 */
static int8_t lb_api_initialize(load_balance_api_t *api, load_balance_beacon_tx *lb_beacon_tx,
                                           load_balance_priority_get *priority_get_cb, load_balance_network_switch_req *lb_nwk_switch_cb, uint16_t beacon_max_payload_length, void *lb_user)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this || !lb_beacon_tx || !priority_get_cb || !lb_nwk_switch_cb || !lb_user) {
        return -1;
    }

    //Allocate beacon payload here
    if (!load_balance_network_class_allocate(this, beacon_max_payload_length)) {
        return -2;
    }

    this->lb_user_parent_id = lb_user;
    this->load_balance_beacon_tx_cb = lb_beacon_tx;
    this->lb_nwk_switch_cb = lb_nwk_switch_cb;
    this->lb_priority_get_cb = priority_get_cb;
    return 0;
}

static void lb_load_level_poll(lb_internal_t * this, uint32_t trigle_period)
{
    if (!this->lb_border_router || !this->lb_user_parent_id) {
        return;
    }

    if (this->lb_border_router->timer2update) {
        this->lb_border_router->timer2update--;
        return;
    }

    uint16_t node_count;
    if (this->lb_border_router->get_count_cb(this->lb_user_parent_id, &node_count) != 0) {
        this->lb_border_router->last_load_level = 0xff;
        this->lb_border_router->timer2update = 10;
        return;
    }
    //Calculate
    uint8_t new_load;
    if (node_count >= this->lb_border_router->expected_node_count) {
        new_load = this->lb_border_router->network_load_scaler - 1;
    } else {
        uint16_t scaled_level = this->lb_border_router->expected_node_count / this->lb_border_router->network_load_scaler;
        new_load = node_count / scaled_level;
    }

    if (this->lb_border_router->last_load_level != new_load) {

        if (this->lb_border_router->set_new_load_cb(this->lb_user_parent_id, new_load) == 0) {
            this->lb_border_router->last_load_level = new_load;
        }
    }
    this->lb_border_router->timer2update = trigle_period >> 1; //Update Every block period / 2
}

static void lb_second_ticks(const load_balance_api_t* api)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this || this->lb_state == LB_IDLE_STATE) {
        return;
    }

    lb_load_level_poll(this, this->triggle_period);

    //Update Next beacon generation timer
    if (this->periodic_beacon_activated && this->lb_state != LB_BLOCK_STATE && --this->time_to_next_beacon == 0) {
        this->time_to_next_beacon = this->triggle_period;
        this->load_balance_beacon_tx_cb(this->lb_user_parent_id);
    }

    if (this->lb_state_timer == 0) {

        switch (this->lb_state) {
            case LB_BLOCK_STATE:
                //Enter block state to accept state
                this->lb_state = LB_ACCEPT_STATE;
                this->time_to_next_beacon = randLIB_get_random_in_range(1, 32);
                break;
            case LB_BLOCK_COMPARE:
                this->lb_state = LB_ACCEPT_STATE;
                break;
            case LB_BLOCK_NETWORK_SELECT:
                lb_network_switch_handle(this);
                break;
            default:
                break;
        }
    } else {
       this->lb_state_timer--;
    }
}

load_balance_api_t *load_balance_create(load_balance_network_switch_notify *lb_notify_cb, bool activate_periodic_beacon)
{
    //validate beacon period & beacon_notify
    if (!lb_notify_cb) {
        return NULL;
    }

    //allocate load balance class
    lb_internal_t * this = load_balance_class_allocate();
    if (!this) {
        return NULL;
    }
    this->lb_state = LB_IDLE_STATE;
    this->lb_nwk_switch_notify = lb_notify_cb;
    this->periodic_beacon_activated = activate_periodic_beacon;

    return this->lb_api;
}

int load_balance_delete(load_balance_api_t *api)
{
    if (!lb_store || lb_store->lb_api != api) {
        return -1;
    }

    load_balance_class_free(lb_store);
    lb_store = NULL;
    return 0;
}

int load_balance_network_threshold_set(load_balance_api_t *api, uint8_t threshold_min, uint8_t threshold_max)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this) {
        return -1;
    }

    this->nwk_switch_threshold_min = threshold_min;
    this->nwk_switch_threshold_max = threshold_max;
    return 0;
}

int load_balance_network_load_monitor_enable(load_balance_api_t *api, uint16_t expected_node_count, uint8_t network_load_scaler, load_balance_api_get_node_count *get_count_cb, load_balance_api_set_load_level *set_new_load_cb)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this) {
        return -1;
    }
    if (expected_node_count == 0 || network_load_scaler == 0 || expected_node_count < network_load_scaler) {
        return -1;
    }

    if (!get_count_cb || !set_new_load_cb) {
        return -1;
    }

    lb_monitor_internal_t *border_router = lb_border_router_api_allocate(this);
    if (!border_router) {
        return -2;
    }
    border_router->expected_node_count = expected_node_count;
    border_router->network_load_scaler = network_load_scaler;
    border_router->get_count_cb = get_count_cb;
    border_router->last_load_level = 0xff;
    border_router->set_new_load_cb = set_new_load_cb;
    border_router->timer2update = 10;
    return 0;
}

int load_balance_network_load_monitor_disable(load_balance_api_t *api)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this) {
        return -1;
    }
    return lb_border_router_api_free(this);
}

int load_balance_set_max_probability(load_balance_api_t *api, uint8_t max_p)
{
    lb_internal_t * this = lb_api_get(api);
    if (!this) {
        return -1;
    }
    if (max_p > 100 || max_p == 0) {
        return -1;
    }

    this->nwk_maX_P = max_p;

    return 0;

}

int load_balance_network_switch_cb_set(load_balance_api_t *api, net_load_balance_network_switch_notify *network_switch_notify)
{
	lb_internal_t * this = lb_api_get(api);
	if (!this) {
	    return -1;
	}

	this->lb_access_switch_cb = network_switch_notify;

	return 0;
}


