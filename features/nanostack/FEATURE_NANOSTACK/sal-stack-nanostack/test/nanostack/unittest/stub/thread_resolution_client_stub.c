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

#include "nsconfig.h"
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "thread_tmfcop_lib.h"

#include "coap_service_api.h"

#include "thread_config.h"
#include "thread_resolution_client.h"

/**
 * Public Api functions
 */
void thread_resolution_client_init(int8_t interface_id)
{
}

void thread_resolution_client_delete(int8_t interface_id)
{
}

void thread_resolution_client_set_notification_cb(int8_t interface_id, thread_resolution_client_notification_cb notification_cb)
{
}

void thread_resolution_client_set_error_cb(int8_t interface_id, thread_resolution_client_error_cb error_cb)
{
}

int thread_resolution_client_address_error(int8_t interface_id, const uint8_t dest_ip_addr[16], const uint8_t target_ip_addr[16], const uint8_t ml_eid[8])
{
    return 0;
}

int thread_resolution_client_resolve(int8_t interface_id, uint8_t ip_addr[16], thread_resolution_client_response_cb *resp_cb)
{
    return 0;
}

void thread_resolution_client_timer(int8_t interface_id, uint16_t seconds)
{
}

