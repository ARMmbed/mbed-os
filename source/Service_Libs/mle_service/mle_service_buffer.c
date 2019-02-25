/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
#include "Service_Libs/mle_service/mle_service_buffer.h"
#include "Service_Libs/mle_service/mle_service_interface.h"

#define TRACE_GROUP "msbu"

typedef NS_LIST_HEAD(mle_service_msg_buf_t, link) mle_service_msg_list_t;

static mle_service_msg_list_t srv_buf_list;

mle_service_msg_buf_t *mle_service_buffer_find(uint16_t id)
{
    ns_list_foreach(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        if (cur_ptr->msg_id == id) {
            return cur_ptr;
        }
    }
    return NULL;
}

mle_service_msg_buf_t *mle_service_buffer_find_for_response(uint8_t *responseData, uint16_t length)
{
    ns_list_foreach(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        if (cur_ptr->challengePtr && (cur_ptr->challengeLen == length)) {

            if (memcmp(cur_ptr->challengePtr, responseData, length) == 0) {
                return cur_ptr;
            }
        }
    }
    return NULL;
}


uint8_t *mle_service_buffer_get_data_pointer(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return NULL;
    }

    return mle_msg_data_pointer(buf);
}

int mle_service_buffer_update_length(uint16_t msgId, uint16_t tail_length)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    if (buf->size < (buf->buf_end + tail_length)) {
        return -2;
    }

    mle_msg_length_set(buf, tail_length);
    return 0;
}

int mle_service_buffer_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    uint16_t new_end;
    if (!buf) {
        return -1;
    }
    new_end = data_end_ptr - buf->buf;
    if (buf->size < new_end) {
        return -2;
    }

    buf->buf_end = new_end;
    return 0;
}

int mle_service_buffer_tail_get(uint16_t msgId, uint16_t tail_length)
{
    mle_service_msg_buf_t *buffer = mle_service_buffer_find(msgId);

    if (!buffer) {
        return -2;
    }

    if (buffer->size < (buffer->buf_end + tail_length)) {
        //Allocate new buffer
        uint8_t *ptr;

        if (tail_length % 64) {//Allocate min 64 bytes every timemore space
            tail_length += (64 - (tail_length % 64));
        }

        tail_length += buffer->buf_end;



        uint8_t *temporary_buf = ns_dyn_mem_alloc(tail_length);
        if (!temporary_buf) {
            return -1;
        }
        //Copy old data to to new buffer and set front headroon empyty
        ptr = (temporary_buf);
        memcpy(ptr, buffer->buf, buffer->buf_end);
        //Update new buffer params
        buffer->size = tail_length;
        ns_dyn_mem_free(buffer->buf);
        buffer->buf = temporary_buf;
        if (buffer->challengePtr) {
            buffer->challengePtr = buffer->buf + 3;
        }
    }
    return 0;
}

int mle_service_buffer_set_msg_type(uint16_t msgId, uint8_t message_type)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    uint8_t *ptr = buf->buf; // Message type is allways first byte
    *ptr = message_type;
    return 0;
}

int mle_service_buffer_set_response(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    buf->response_status = true;
    return 0;
}

bool mle_service_check_msg_response(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return false;
    }

    return buf->response_status;
}

int mle_service_buffer_set_timeout_cb(uint16_t msgId, mle_service_message_timeout_cb *cb)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    buf->timeout_cb = cb;
    return 0;
}

uint8_t *mle_service_buffer_get_msg_destination_address_pointer(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return NULL;
    }

    return buf->dst_address;
}

int mle_service_buffer_set_msg_destination_address(uint16_t msgId, uint8_t *address_ptr)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    memcpy(buf->dst_address, address_ptr, 16);
    return 0;
}

int mle_service_buffer_set_msg_rf_channel(uint16_t msgId, uint8_t channel)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }
    buf->selected_rf_channel = channel;
    buf->selected_channel = true;
    return 0;
}

int mle_service_buffer_set_msg_security_mode(uint16_t msgId, bool key_id_mode_2)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    buf->enable_link_layer_security = true;
    buf->psk_key_id_mode_2 = key_id_mode_2;
    return 0;
}


void mle_service_buffer_clean_buffers_by_interface_id(int8_t interface_id)
{
    ns_list_foreach_safe(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        if (cur_ptr->interfaceId == interface_id) {
            mle_service_buffer_free(cur_ptr);
        }
    }
}

uint16_t mle_service_buffer_count_by_interface_id(int8_t interface_id)
{
    uint16_t result = 0;
    ns_list_foreach_safe(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        if (cur_ptr->interfaceId == interface_id) {
            result++;
        }
    }
    return result;
}


mle_service_msg_buf_t *mle_service_buffer_get(uint16_t data_length)
{
    mle_service_msg_buf_t *buffer = ns_dyn_mem_temporary_alloc(sizeof(mle_service_msg_buf_t));
    if (!buffer) {
        return NULL;
    }

    //Allocate allways excat 64 byte blocks
    if (data_length % 64) {
        data_length += (64 - (data_length % 64));
    }


    buffer->buf = ns_dyn_mem_temporary_alloc(data_length);
    if (!buffer->buf) {
        //NOTE: We cannot call mle_service_buffer_free() here. Element is not yet fully constructed.
        ns_dyn_mem_free(buffer);
        return NULL;
    }

    buffer->size = data_length;
    buffer->timeout_cb = NULL;
    buffer->response_status = false;
    //NO retry by default
    buffer->timeout_init = 0;
    buffer->timeout = 0;
    buffer->timeout_max = 0;
    buffer->retrans_max = 0;
    buffer->retrans = 0;
    buffer->buf_end = 1;
    buffer->msg_id = 0;
    buffer->challengePtr = NULL;
    buffer->challengeLen = 0;
    buffer->delayed_response = 0;
    buffer->interfaceId = -1;
    buffer->tokens_delay = false;
    buffer->tokens_priority = false;
    buffer->message_sent = false;
    buffer->selected_channel = false;
    buffer->selected_pan_id = false;
    buffer->enable_link_layer_security = false;
    buffer->psk_key_id_mode_2 = false;
    memset(&buffer->security_header, 0, sizeof(mle_security_header_t));
    ns_list_add_to_start(&srv_buf_list, buffer);

    return buffer;
}

int mle_service_buffer_free(mle_service_msg_buf_t *buf)
{
    if (!buf) {
        return -1;
    }
    ns_list_remove(&srv_buf_list, buf); //Remove from the list
    ns_dyn_mem_free(buf->buf);//Free Allocated Data
    ns_dyn_mem_free(buf);// Free entry

    return 0;
}

uint16_t mle_service_buffer_tokens_delay_count(void)
{
    uint16_t buffer_count = 0;

    ns_list_foreach(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        if (cur_ptr->tokens_delay) {
            buffer_count++;
        }
    }

    return buffer_count;
}

/**
 * MLE service transaction timeout and retry handling
 */
bool mle_service_buffer_timer_tick(uint16_t ticks, mle_service_buffer_timeout_cb *timeout_cb)
{
    bool activeTimerNeed = false;

    ns_list_foreach_reverse_safe(mle_service_msg_buf_t, cur_ptr, &srv_buf_list) {
        activeTimerNeed = true;
        if (cur_ptr->timeout <= ticks) {
            cur_ptr->tokens_delay = false;
            timeout_cb(cur_ptr);
        } else {
            cur_ptr->timeout -= ticks;
            if (cur_ptr->tokens_delay) {
                timeout_cb(cur_ptr);
            }
        }
    }
    return activeTimerNeed;
}


