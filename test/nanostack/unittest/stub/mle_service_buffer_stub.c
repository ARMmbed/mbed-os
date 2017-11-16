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

/*
 * \file mle_service_buffer.c
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
#include "randLIB.h"
#include "Service_Libs/mle_service/mle_service_buffer.h"
#include "Service_Libs/mle_service/mle_service_interface.h"

#define TRACE_GROUP "msbu"

typedef NS_LIST_HEAD(mle_service_msg_buf_t, link) mle_service_msg_list_t;

mle_service_msg_buf_t *mle_service_buffer_find(uint16_t id)
{
    return NULL;
}

mle_service_msg_buf_t * mle_service_buffer_find_for_response(uint8_t *responseData, uint16_t length)
{
    return NULL;
}

uint8_t * mle_service_buffer_get_data_pointer(uint16_t msgId)
{
    return NULL;
}

int mle_service_buffer_update_length(uint16_t msgId, uint16_t tail_length)
{
    return 0;
}

int mle_service_buffer_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr)
{
    return 0;
}

int mle_service_buffer_tail_get(uint16_t msgId, uint16_t tail_length)
{
    return 0;
}

int mle_service_buffer_set_msg_type(uint16_t msgId, uint8_t message_type)
{
    return 0;
}

int mle_service_buffer_set_response(uint16_t msgId)
{
    return 0;
}

bool mle_service_check_msg_response(uint16_t msgId)
{
    return false;
}

int mle_service_buffer_set_timeout_cb(uint16_t msgId, mle_service_message_timeout_cb *cb)
{
    return 0;
}

uint8_t * mle_service_buffer_get_msg_destination_address_pointer(uint16_t msgId)
{
    return NULL;
}

int mle_service_buffer_set_msg_destination_address(uint16_t msgId, uint8_t *address_ptr)
{
    return 0;
}

void mle_service_buffer_clean_buffers_by_interface_id(int8_t interface_id)
{
}


mle_service_msg_buf_t * mle_service_buffer_get(uint16_t data_length)
{
    return NULL;
}

int mle_service_buffer_free(mle_service_msg_buf_t *buf)
{
    return 0;
}

uint16_t mle_service_buffer_tokens_delay_count(void)
{
    return 0;
}

bool mle_service_buffer_timer_tick(uint16_t ticks, mle_service_buffer_timeout_cb *timeout_cb)
{
    return false;
}

int mle_service_buffer_set_msg_rf_channel(uint16_t msgId, uint8_t channel)
{
    return 0;
}


