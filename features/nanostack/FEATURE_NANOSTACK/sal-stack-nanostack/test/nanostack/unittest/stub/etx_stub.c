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
#include "common_functions.h"
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "platform/arm_hal_phy.h"
#include "net_interface.h"
#include "Service_Libs/etx/etx.h"
#include "Service_Libs/utils/isqrt.h"

#define TRACE_GROUP "etx"

void etx_transm_attempts_update(int8_t interface_id, uint8_t attempts, bool success, addrtype_t addr_type, const uint8_t *addr_ptr)
{
}

void etx_remote_incoming_idr_update(int8_t interface_id, uint8_t remote_incoming_idr, mle_neigh_table_entry_t *neigh_table_ptr)
{
}

uint16_t etx_read(int8_t nwk_id, addrtype_t addr_type, const uint8_t *addr_ptr)
{
    return 0;
}

uint16_t etx_local_incoming_idr_read(int8_t interface_id, mle_neigh_table_entry_t *neigh_table_ptr)
{
    return 0;
}

uint16_t etx_lqi_dbm_update(int8_t interface_id, uint8_t lqi, int8_t dbm, mle_neigh_table_entry_t *neigh_table_ptr)
{
    return 0;
}

uint8_t etx_value_change_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint16_t hysteresis, etx_value_change_handler_t *callback_ptr)
{
    return 0;
}

uint8_t etx_accum_failures_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint8_t threshold, etx_accum_failures_handler_t *callback_ptr)
{
    return 0;
}

void etx_neighbor_remove(int8_t interface_id, mle_neigh_table_entry_t *neigh_table_ptr)
{
}

void etx_neighbor_add(int8_t interface_id, mle_neigh_table_entry_t *neigh_table_ptr)
{

}
