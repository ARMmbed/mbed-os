/*
 * Copyright (c) 2013-2016, Arm Limited and affiliates.
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

int nd_proxy_downstream_interface_unregister(int8_t interface_id)
{
    return 0;
}

bool nd_proxy_enabled_for_upstream(int8_t interface_id)
{
    return false;
}

bool nd_proxy_upstream_route_onlink(int8_t downstream_id, uint8_t *address)
{
    return false;
}
