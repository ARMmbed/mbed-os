/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
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

#include "Core/include/ns_monitor.h"
#include "mac_api.h"    // for mcps_packet_ingress_rate_limit_by_memory
#include "MAC/IEEE802_15_4/mac_mcps_sap.h" // for mcps_packet_ingress_rate_limit_by_memory

int ns_conf_gc_threshold_set(uint8_t percentage_high, uint8_t percentage_critical)
{
    return ns_monitor_heap_gc_threshold_set(percentage_high, percentage_critical);
}

int ns_conf_packet_ingress_rate_limit_by_mem(uint8_t free_heap_percentage)
{
    return mcps_packet_ingress_rate_limit_by_memory(free_heap_percentage);
}
