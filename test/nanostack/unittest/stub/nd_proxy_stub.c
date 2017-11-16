/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#ifdef HAVE_ND_PROXY
#include "ns_types.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"


int nd_proxy_downstream_interface_register(int8_t interface_id, nd_proxy_req_cb *nd_proxy_req, bridge_state_update_cb * bridge_state_update)
{
    return 0;
}

int nd_proxy_downstream_interface_unregister(int8_t interface_id)
{
    return 0;
}

int nd_proxy_upstream_interface_register(int8_t interface_id, nd_proxy_req_cb *route_validation_req)
{
    return 0;
}

int nd_proxy_upstream_interface_unregister(int8_t interface_id)
{
    return 0;
}

bool nd_proxy_enabled_for_downstream(int8_t interface_id)
{
    return false;
}

bool nd_proxy_enabled_for_upstream(int8_t interface_id)
{
    return false;
}

bool nd_proxy_target_address_validation(int8_t upstream_id, uint8_t *address, int8_t *downstream_id_ptr)
{
    return false;
}

bool nd_proxy_upstream_route_onlink(int8_t downstream_id, uint8_t *address)
{
    return false;
}
#endif /* HAVE_ND_PROXY */

