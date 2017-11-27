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
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "mlme.h"
#include "load_balance_api.h"


load_balance_api_t *load_balance_create(load_balance_network_switch_notify *lb_notify_cb, bool actvated_beacon_periodic_tx)
{
    return NULL;
}

int load_balance_delete(load_balance_api_t *api)
{
    return 0;
}

int load_balance_network_threshold_set(load_balance_api_t *api, uint8_t threshold_min, uint8_t threshold_max)
{
    return 0;
}

int load_balance_network_load_monitor_enable(load_balance_api_t *api, uint16_t expected_node_count, uint8_t network_load_scaler, load_balance_api_get_node_count *get_count_cb, load_balance_api_set_load_level *set_new_load_cb)
{
    return 0;
}

int load_balance_network_load_monitor_disable(load_balance_api_t *api)
{
    return 0;
}

int load_balance_set_max_probability(load_balance_api_t *api, uint8_t max_p)
{
    return 0;
}

