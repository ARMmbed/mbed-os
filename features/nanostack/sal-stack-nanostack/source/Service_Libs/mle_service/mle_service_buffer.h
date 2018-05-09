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
 * \file mle_service_buffer.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MLE_SERVICE_BUFFER_H_
#define MLE_SERVICE_BUFFER_H_

#include <ns_types.h>
#include "ns_list.h"
#include "Service_Libs/mle_service/mle_service_api.h"

struct mle_security_header;

typedef struct mle_service_msg_buf {

    uint16_t msg_id; /*!< Buffer Id what could be used for update or free message */
    int8_t interfaceId; /*!< Which interface buffer is tiegted */
    mle_service_message_timeout_cb *timeout_cb; /*!< This will be called every time when timeout happen */
    uint8_t * buf; /*!< Payload buffer pointer */
    uint16_t size; /*!< Buffer total allocated size */
    uint16_t buf_end; /*!< End pointer in the buffer */
    uint8_t dst_address[16]; /*!< Messages IPv6 destination address */
    struct mle_security_header security_header; /*!< Messages security parameters */
    uint8_t *challengePtr; /*!< Messages challenge data */
    uint8_t challengeLen; /*!< Messages challenge data length */
    uint8_t delayed_response; /*!< Define delayed response message */
    uint16_t timeout; /*!< Messages active timeout */
    uint16_t timeout_init; /*!< Init timeout period length */
    uint16_t timeout_max; /*!< Define Max retransmission timeout */
    uint8_t retrans_max; /*!< Define max re transmission count */
    uint8_t retrans; /*!< Current retrans count */
    uint8_t selected_rf_channel; /*!< Packet selected channel (only valid when  selected_channel is true*/
    uint16_t packet_panid; /*!< Packet selected Pan-id (only valid when  selected_pan_id is true*/
    unsigned response_status:1;
    unsigned tokens_delay:1; /*!< Delay message because of tokens */
    unsigned tokens_priority:1; /*!< Message has priority over token bucket */
    unsigned message_sent:1; /* Message has been sent */
    unsigned selected_channel:1;
    unsigned selected_pan_id:1;
    unsigned enable_link_layer_security;
    unsigned psk_key_id_mode_2;
    ns_list_link_t link;
} mle_service_msg_buf_t;

/**
 *  MLE service buffer timeout callback
 *
 *  \param mle_msg mle buffer pointer
 */
typedef void (mle_service_buffer_timeout_cb)(mle_service_msg_buf_t *mle_msg);

/** get pointer to data end*/
#define mle_msg_data_pointer(x)  (&(x)->buf[(x)->buf_end])

/** set data length Set*/
#define mle_msg_length_set(x,z)  ((x)->buf_end = (x)->buf_end + (z))

/** set data length by updated data pointer*/
#define mle_msg_length_set_by_pointer(x,z)  ((x)->buf_end = (z) - (x)->buf)

/** get data length*/
#define mle_msg_data_length(x)  (uint16_t)(x->buf_end)

/**
 *  MLE service buffers free by interface id
 *
 *  Call this when unregister MLe service
 *
 *  \param interface_id interface id of mle service user
 *
 */
void mle_service_buffer_clean_buffers_by_interface_id(int8_t interface_id);

/**
 *  MLE service buffers count by interface id
 *
 *  get amount of active buffer count
 *
 *  \param interface_id interface id of mle service user
 *
 *  \return count of active buffers.
 */
uint16_t mle_service_buffer_count_by_interface_id(int8_t interface_id);

/**
 *  MLE service buffers allocate
 *
 *
 *  \param data_length mle message init length
 *
 *  \return a Pointer to mle message or
 *  \return NULL if allocate failed.
 *
 */
mle_service_msg_buf_t * mle_service_buffer_get(uint16_t data_length);

/**
 *  MLE service buffer pointer free.
 *
 * This function frees elements received from mle_service_buffer_get().
 * You should not feed in any other elements.
 *
 *  \param buf buffer pointer
 *
 *  \return 0 Free OK
 *  \return -1 Free error
 */
int mle_service_buffer_free(mle_service_msg_buf_t *buf);

/**
 *  MLE service buffers discover by buffer id
 *
 *
 *  \param id mle message id
 *
 *  \return > 0 Pointer to mle message
 *  \return NULL unknown id
 *
 */
mle_service_msg_buf_t *mle_service_buffer_find(uint16_t id);

/**
 *  MLE service buffers discover by resonse data
 *
 *  This function should call when mle service user need to verify response data to own challenge
 *
 *  \param responseData pointer to response data
 *  \param length length of response
 *
 *  \return > 0 Pointer to mle message
 *  \return NULL unknown response data
 *
 */
mle_service_msg_buf_t * mle_service_buffer_find_for_response(uint8_t *responseData, uint16_t length);

/**
 *  MLE service buffers data pointer get
 *
 *
 *  \param msgId mle message id
 *
 *  \return > 0 Pointer next data write
 *  \return NULL unknown message id
 *
 */
uint8_t * mle_service_buffer_get_data_pointer(uint16_t msgId);

/**
 *  MLE service buffers data length updated given length
 *
 *
 *  \param msgId mle message id
 *  \param tail_length how much length pointer will be moved ahead
 *
 *  \return 0 dat length update OK
 *  \return < 0 Unknown id or length update detect possible overwrite operation
 *
 */
int mle_service_buffer_update_length(uint16_t msgId, uint16_t tail_length);

/**
 *  MLE service buffers data length updated given updated data pointer
 *
 *
 *  \param msgId mle message id
 *  \param data_end_ptr new data end pointer
 *
 *  \return 0 dat length update OK
 *  \return < 0 Unknown id or length update detect possible overwrite operation
 *
 */
int mle_service_buffer_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr);

/**
 *  MLE service allocate more tail to current mle payload
 *
 *
 *  \param msgId mle message id
 *  \param tail_length new need data count
 *
 *  \return 0 dat length allocate ok OK
 *  \return < 0 Unknown id or allocate fail
 *
 */
int mle_service_buffer_tail_get(uint16_t msgId, uint16_t tail_length);

/**
 *  MLE service buffer timeout call back set
 *
 *  \param msgId mle message id
 *  \param cb new messgaes timeout call back function pointer
 *
 *  \return 0 set OK
 *  \return < 0 set fail
 *
 */
int mle_service_buffer_set_timeout_cb(uint16_t msgId, mle_service_message_timeout_cb *cb);

/**
 *  MLE service buffer response RX state set
 *
 *  \param msgId mle message id
 *
 *  \return 0 set OK
 *  \return < 0 set fail
 *
 */
int mle_service_buffer_set_response(uint16_t msgId);

/**
 *  MLE service mle message type write
 *
 *
 *  \param msgId mle message id
 *  \param message_type mle message type
 *
 *  \return 0 dat length allocate ok OK
 *  \return < 0 Unknown id
 *
 */
int mle_service_buffer_set_msg_type(uint16_t msgId, uint8_t message_type);

/**
 *  MLE service mle message address pointer get
 *
 *
 *  \param msgId mle message id
 *
 *  \return >0 Pointer to message destination address
 *  \return < 0 Unknown message id
 *
 */
uint8_t * mle_service_buffer_get_msg_destination_address_pointer(uint16_t msgId);

/**
 *  MLE service mle message address set
 *
 *
 *  \param msgId mle message id
 *  \param address_ptr destination address
 *
 *  \return 0 Write OK
 *  \return < 0 Unknown message id
 *
 */
int mle_service_buffer_set_msg_destination_address(uint16_t msgId, uint8_t *address_ptr);

/**
 * MLE service transaction timeout and retry handling
 */
bool mle_service_buffer_timer_tick(uint16_t ticks, mle_service_buffer_timeout_cb *timeout_cb);

/**
 *  Counts MLE service message buffers that tokens has delayed.
 *
 *  \return Buffer count
 *
 */
uint16_t mle_service_buffer_tokens_delay_count(void);

/**
 *  MLE service mle message rf channel set for this packet
 *
 *
 *  \param msgId mle message id
 *  \param channel selected rf channel
 *
 *  \return 0 Set OK
 *  \return < 0 Unknown message id
 *
 */
int mle_service_buffer_set_msg_rf_channel(uint16_t msgId, uint8_t channel);

/**
 *  MLE service mle message link layer security enable and set key id mode set for this packet
 *
 *
 *  \param msgId mle message id
 *  \param key_id_mode_2 true select psk key id mode 2 false slect mac default mode
 *
 *  \return 0 Set OK
 *  \return < 0 Unknown message id
 *
 */
int mle_service_buffer_set_msg_security_mode(uint16_t msgId, bool key_id_mode_2);

#endif /* MLE_SERVICE_BUFFER_H_ */
