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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "mac_filter_api.h"
#include "mac_filter.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "mac_common_defines.h"

#include "mac_filter_stub.h"

mac_filter_stub_def_t mac_filter_stub_def;

int_fast8_t mac_filter_start(int8_t interface_id, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    return 0;
}

void mac_filter_stop(int8_t interface_id)
{
}

int_fast8_t mac_filter_set_address_mapping(int8_t interface_id, mac_filter_map_short_to_extended_cb *long_cb, mac_filter_map_extented_to_short_cb *short_cb)
{
    return 0;
}

int_fast8_t mac_filter_clear(int8_t interface_id)
{
    return 0;
}

int_fast8_t mac_filter_delete_short(int8_t interface_id, uint16_t mac16)
{
    return 0;
}

int_fast8_t mac_filter_delete_long(int8_t interface_id, uint8_t mac64[8])
{
    return 0;
}

int_fast8_t mac_filter_add_short(int8_t interface_id, uint16_t mac16, int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    return 0;
}

int_fast8_t mac_filter_add_long(int8_t interface_id, uint8_t mac64[8], int16_t lqi_m, int16_t lqi_add, int16_t dbm_m, int16_t dbm_add)
{
    return 0;
}

int_fast8_t mac_filter_modify_link_quality(int8_t interface_id, mac_pre_parsed_frame_t *mac_frame)
{
    return mac_filter_stub_def.value_int8;
}

