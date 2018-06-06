/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 * \file net_short_address_extension.c
 * \brief Network Short address extension API for library model
 *
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "net_address_extension.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/MAC/mac_helper.h"

static uint16_t net_ext_short_address = 0xfffe;
static uint8_t net_ext_gen_new = 1;

void net_ext_set_short_address_from_app(uint16_t short_id, uint8_t generate_dynamical_at_DAD)
{
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get(IF_6LoWPAN);

    net_ext_short_address = short_id;
    if (cur) {
        cur->lowpan_desired_short_address = short_id;
    }
    net_ext_gen_new = generate_dynamical_at_DAD;
}

uint8_t net_ext_DAD_event_generate(void)
{
    if (net_ext_gen_new) {
        net_ext_short_address = 0xfffe;
    }
    return net_ext_gen_new;
}

void net_ext_reset_short_address_from_app(void)
{
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get(IF_6LoWPAN);

    if (cur && cur->mac_api) {
        protocol_6lowpan_allocate_mac16(cur);
        mac_helper_mac16_address_set(cur, 0xfffe);
    }
    net_ext_gen_new = 1;
}

uint16_t net_ext_get_short_address(void)
{
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get(IF_6LoWPAN);
    if (net_ext_short_address == 0xfffe) {
        if (cur) {
            net_ext_short_address = mac_helper_mac16_address_get(cur);
        }
    }
    return net_ext_short_address;
}
