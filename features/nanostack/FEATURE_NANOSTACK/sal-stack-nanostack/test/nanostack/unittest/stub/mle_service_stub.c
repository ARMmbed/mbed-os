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
#include "socket_api.h"
#include "Core/include/socket.h"
#include "net_interface.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h" // just for protocol_core_monotonic_time
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/mle_service/mle_service_security.h"
#include "Service_Libs/mle_service/mle_service_buffer.h"
#include "Service_Libs/mle_service/mle_service_interface.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "mle_service_stub.h"

#define TRACE_GROUP "mleS"

/* Fixed-point randomisation limits for randlib_randomise_base() - RFC 3315
 * says RAND is uniformly distributed between -0.1 and +0.1
 */
#define MLE_RAND_LOW   0x7333 // 1 - 0.1; minimum for "1+RAND"
#define MLE_RAND_HIGH  0x8CCD // 1 + 0.1; maximum for "1+RAND"

typedef struct {
    int8_t mle_socket;
    int8_t mle_socket_service_tasklet;
    uint8_t mle_adata[46];
    uint8_t mle_adata_length;
    bool mle_frame_counter_check;
} mle_service_class_t;

mle_service_msg_buf_def mle_service_msg_buf;
/**
 * MLE service transaction timeout and retry handling
 */
bool mle_service_timer_tick(uint16_t ticks)
{
    return false;

}

int mle_service_interface_register(int8_t interface_id, mle_service_receive_cb *receive_cb, uint8_t *mac64, uint8_t challengeLength)
{
    return 0;
}

bool mle_service_interface_registeration_validate(int8_t interface_id)
{
    return false;
}


int mle_service_interface_receiver_handler_update(int8_t interface_id, mle_service_receive_cb *receive_cb)
{
    return 0;
}

void mle_service_interface_unregister(int8_t interface_id)
{
}

void mle_service_interface_tx_queue_clean(int8_t interface_id)
{
}

uint16_t mle_service_interface_tx_queue_size(int8_t interface_id)
{
    return 0;
}

int mle_service_interface_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count)
{
    return 0;
}

uint16_t mle_service_msg_allocate(int8_t interface_id, uint16_t data_length, bool challengeAllocate, uint8_t type)
{
    return 0;
}

uint8_t * mle_service_get_data_pointer(uint16_t msgId)
{
    return NULL;
}

uint8_t * mle_service_get_payload_start_point(uint16_t msgId)
{
    return NULL;
}

uint16_t  mle_service_get_payload_length(uint16_t msgId)
{
    return 0;
}

int mle_service_update_length(uint16_t msgId, uint16_t tail_length)
{
    return 0;
}

int mle_service_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr)
{
    return 0;
}

int mle_service_msg_update_security_params(uint16_t msgId, uint8_t security_level, uint8_t key_id_mode, uint32_t keysequence)
{
    return 0;
}

int mle_service_set_packet_callback(uint16_t msgId, mle_service_message_timeout_cb *cb)
{
    return 0;
}

int mle_service_set_msg_response_true(uint16_t msgId)
{
    return 0;
}

uint8_t * mle_service_get_msg_destination_address_pointer(uint16_t msgId)
{
    return 0;
}

int mle_service_set_msg_destination_address(uint16_t msgId, const uint8_t *dstAddress)
{
    return 0;
}

int mle_service_set_msg_panid(uint16_t msgId, uint16_t panid)
{
    return 0;
}

int mle_service_msg_free(uint16_t msgId)
{
    return 0;
}

bool mle_service_transaction_buffer_get_for_response(uint8_t *responseData, uint16_t length, uint16_t *msgId)
{
    return false;
}

bool mle_service_check_msg_sent(uint16_t msgId)
{
    return false;
}

int mle_service_message_tail_get(uint16_t msgId, uint16_t tail_length)
{
    return 0;
}

int mle_service_set_msg_timeout_parameters(uint16_t msgId, mle_message_timeout_params_t *timeout_params)
{
    return 0;
}

int mle_service_set_msg_token_bucket_priority(uint16_t msgId)
{
    return 0;
}

int mle_service_send_message(uint16_t msgId)
{
    return 0;
}

int mle_service_security_init(int8_t interfaceId, uint8_t security_level, uint32_t security_frame_counter, mle_service_key_request_by_counter_cb * key_counter_req, mle_service_security_notify_cb *notify)
{
    return 0;
}

bool mle_service_security_set_frame_counter(int8_t interfaceId, uint32_t frame_counter)
{
    return false;
}

uint8_t * mle_service_security_default_key_get(int8_t interfaceId)
{
    return 0;
}

uint8_t  mle_service_security_default_key_id_get(int8_t interfaceId)
{
    return 0;
}

uint8_t * mle_service_security_next_key_get(int8_t interfaceId)
{
    return 0;
}

uint8_t  mle_service_security_next_key_id_get(int8_t interfaceId)
{
    return 0;
}

uint8_t  mle_service_security_level_get(int8_t interfaceId)
{
    return 0;
}

bool mle_service_security_key_trig(int8_t interfaceId, uint8_t keyId)
{
    return false;

}

int mle_service_security_instance_allocate(int8_t interface_id)
{
    return mle_service_msg_buf.msg_id;
}

bool mle_service_security_set_security_key(int8_t interfaceId, const uint8_t *security_key, uint8_t keyId, bool primary)
{
    return false;
}


int mle_service_reject_message_build(int8_t interfaceId, uint8_t *dstIpv6, bool force_unsecure)
{

    uint16_t buf_id = mle_service_msg_allocate(interfaceId, 32, false, MLE_COMMAND_REJECT);
    if (buf_id == 0) {
        return -1;
    }
    tr_debug("MLE Reject MSG Build");
    //SET Destination address
    mle_service_set_msg_destination_address(buf_id, dstIpv6);
    mle_service_send_message(buf_id);

    return 0;
}
uint32_t mle_service_security_get_frame_counter(int8_t interfaceId)
{
    return 0;
}
void mle_service_set_frame_counter_check(bool value)
{
}

int mle_service_interface_receiver_bypass_handler_update(int8_t interface_id, mle_service_receive_security_bypass_cb *receive_cb)
{
	return 0;
}

void mle_service_set_fragmented_msg_ll_security(bool value)
{
}

int mle_service_set_msg_rf_channel(uint16_t msgId, uint8_t channel)
{
    return 0;
}
int mle_service_set_msg_link_layer_security_mode(uint16_t msgId, bool use_key_id_mode_2)
{
    return 0;
}
