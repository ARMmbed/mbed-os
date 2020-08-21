/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/*
 * \file mle_service_interface.c
 * \brief Add short description about this file!!!
 *
 */

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "common_functions.h"
#include "Service_Libs/mle_service/mle_service_interface.h"

#define TRACE_GROUP "mleS"

static service_instance_list_t srv_interface_list;

service_instance_t *mle_service_interface_get(int8_t interface_id)
{
    service_instance_t *srv_ptr = mle_service_interface_find(interface_id);

    if (!srv_ptr) {
        srv_ptr = ns_dyn_mem_alloc(sizeof(service_instance_t));
        if (srv_ptr) {
            ns_list_add_to_start(&srv_interface_list, srv_ptr);
            srv_ptr->interface_id = interface_id;
            srv_ptr->URT = 10; //1second
            srv_ptr->MRT = 50; //5 seconds
            srv_ptr->MRC = 3; //3 retry
            srv_ptr->token_bucket = 0;
            srv_ptr->token_bucket_ticks = 0;
            srv_ptr->token_bucket_size = 0;
            srv_ptr->token_bucket_rate = 0;
            srv_ptr->token_bucket_count = 0;
            srv_ptr->recv_security_bypass_cb = NULL;
        }
    }

    return srv_ptr;
}


service_instance_t *mle_service_interface_find(int8_t interface_id)
{
    ns_list_foreach(service_instance_t, cur_ptr, &srv_interface_list) {
        if (cur_ptr->interface_id == interface_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

int mle_service_interface_delete(int8_t interface_id)
{
    ns_list_foreach(service_instance_t, cur_ptr, &srv_interface_list) {
        if (cur_ptr->interface_id == interface_id) {
            ns_list_remove(&srv_interface_list, cur_ptr);
            ns_dyn_mem_free(cur_ptr);
            return 0;
        }
    }
    return -1;
}

bool mle_service_interface_list_empty(void)
{
    return ns_list_is_empty(&srv_interface_list);
}

bool mle_service_interface_timer_tick(uint16_t ticks, mle_service_interface_timer_cb *timeout_cb)
{
    bool active_timer_needed = false;

    ns_list_foreach(service_instance_t, cur_ptr, &srv_interface_list) {
        if (timeout_cb(ticks, cur_ptr)) {
            active_timer_needed = true;
        }
    }

    return (active_timer_needed);
}
