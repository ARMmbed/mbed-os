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
#include <string.h>
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"

#define TRACE_GROUP "mPKe"

int mac_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, const uint8_t eui64[static 8], const uint8_t key[static 16])
{
    return 0;

}

int mac_pairwise_key_del(int8_t interface_id, const uint8_t eui64[static 8])
{
    return 0;
}


int mac_pairwise_key_flush_list(int8_t interface_id)
{
    return 0;
}


int mac_pairwise_key_interface_register(int8_t interface_id, uint8_t mac_supported_key_decription_size, uint8_t network_key_size) {
    return 0;
}

int mac_pairwise_key_interface_unregister(int8_t interface_id) {
    return 0;
}

