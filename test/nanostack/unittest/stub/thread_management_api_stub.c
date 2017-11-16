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
#include <inttypes.h>
#include "thread_management_api.h"
#include "ns_types.h"
#include "sn_coap_header.h"

int thread_management_recv_set_response_cb(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{
    return 0;
}

int thread_management_recv_get_response_cb(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{
    return 0;
}

int thread_management_register(int8_t interface_id)
{
    return 0;
}

int thread_management_unregister(int8_t interface_id)
{
    return 0;
}

int thread_management_set_security_policy(int8_t interface_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_set_steering_data(int8_t interface_id,uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_set_commissioning_data_timestamp(int8_t interface_id, uint64_t time, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_get(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr)
{
    return 0;
}
