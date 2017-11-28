/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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

#include "Core/include/address.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#ifdef OLD_RPL
#include "6LoWPAN/RPL/rpl_definition.h"l
#include "6LoWPAN/RPL/rpl_obj.h"
#endif

#include "net_rpl.h"


int8_t arm_nwk_6lowpan_rpl_dodag_init(int8_t interface_id, const uint8_t *dodag_id, const dodag_config_t *config, uint8_t instance_id, uint8_t flags)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_remove(int8_t interface_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_start(int8_t interface_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_route_update(int8_t interface_id, uint8_t *route_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_poison(int8_t interface_id) {
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig(int8_t interface_id) {
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_version_increment(int8_t interface_id) {
    return -1;
}

uint8_t rpl_instance_list_read(uint8_t *cache_ptr, uint8_t cache_size) {
    return 0;
}

uint8_t rpl_read_dodag_info(rpl_dodag_info_t *dodag_ptr, uint8_t instance_id) {
    return 0;
}

int8_t arm_nwk_6lowpan_rpl_dodag_pref_set(int8_t interface_id, uint8_t preference) {
    return 0;
}
