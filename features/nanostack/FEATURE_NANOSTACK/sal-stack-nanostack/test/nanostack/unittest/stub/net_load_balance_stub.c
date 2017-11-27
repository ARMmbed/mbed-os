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
#include <string.h>
#include "net_load_balance_api.h"
#include "Service_Libs/load_balance/load_balance_api.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"



int8_t net_load_balance_network_switch_cb_set(int8_t interface_id,net_load_balance_network_switch_notify *network_switch_notify)
{
    return 0;
}


int8_t net_load_balance_create(int8_t interface_id, bool enable_periodic_beacon_interval)
{
    return 0;
}


int8_t net_load_balance_delete(int8_t interface_id)
{
    return 0;
}

int8_t net_load_balance_threshold_set(int8_t interface_id, uint8_t threshold_min, uint8_t threshold_max)
{
    return 0;
}

void net_load_balance_internal_state_activate(protocol_interface_info_entry_t *interface_ptr, bool state)
{

}
