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
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_management_server.h"

#define TRACE_GROUP "TMFs"

#include "net_interface.h"
#include "socket_api.h"
#include "thread_config.h"
#include "thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "thread_commissioning_if.h"
//#include "thread_tmf_data_if.h"

int thread_management_server_init(int8_t interface_id)
{
    return 0;
}

void thread_management_server_delete(int8_t interface_id)
{
}

void joiner_router_recv_commission_msg(void *cb_res)
{
}

int thread_management_server_joiner_router_init(int8_t interface_id)
{
    return 0;
}

void thread_management_server_joiner_router_deinit(int8_t interface_id)
{
}

int thread_management_server_commisoner_data_get(int8_t interface_id, thread_management_server_data_t *server_data)
{
    return 0;
}

bool thread_management_server_source_address_check(int8_t interface_id, uint8_t source_address[16])
{
    return false;
}
