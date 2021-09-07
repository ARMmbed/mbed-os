/*
 * Copyright (c) 2015-2019, Pelion and affiliates.
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

#ifndef __COAP_MESSAGE_HANDLER_H__
#define __COAP_MESSAGE_HANDLER_H__

#include <inttypes.h>
#include "mbed-coap/sn_coap_header.h"
#include "ns_list.h"

#define TRANSACTION_LIFETIME 180

/* Default value for CoAP duplicate message buffer (0 = disabled) */
#define DUPLICATE_MESSAGE_BUFFER_SIZE 0

/* Default value for CoAP blockwise data size  (0 = disabled) */
#define DEFAULT_BLOCKWISE_DATA_SIZE 0

/* Default values for CoAP resendings */
#define COAP_RESENDING_COUNT 3
#define COAP_RESENDING_INTERVAL 10

/**
 * \brief Service message response receive callback.
 *
 * Function that handles CoAP service message receiving and parsing
 *
 * \param msg_id           Id number of the current message.
 * \param response_ptr     Pointer to CoAP header structure.
 *
 * \return 0 for success / -1 for failure
  */
typedef int coap_message_handler_response_recv(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr);

typedef struct coap_msg_handler_s {
    void *(*sn_coap_service_malloc)(uint16_t);
    void (*sn_coap_service_free)(void *);
    uint8_t (*sn_coap_tx_callback)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *);
    struct coap_s *coap;
} coap_msg_handler_t;

typedef struct coap_transaction {
    uint8_t remote_address[16];
    uint8_t local_address[16];
    uint8_t token[8];
    uint32_t valid_until;
    uint8_t *data_ptr;
    coap_message_handler_response_recv *resp_cb;
    uint16_t remote_port;
    uint16_t msg_id;
    uint16_t data_len;
    int8_t service_id;
    uint8_t options;
    uint8_t token_len;
    sn_coap_msg_type_e req_msg_type;
    bool client_request: 1;

    ns_list_link_t link;
} coap_transaction_t;

/**
 * \brief Service message processing callback.
 *
 * Function that processes CoAP service message
 *
 * \param socket_id         Socket that receives the message.
 * \param recv_if_id        Interface where message is received.
 * \param coap_message      Actual CoAP message.
 * \param transaction_ptr   Message transaction.
 * \param local_addr        Address where message is received.
 *
 * \return 0 for success / -1 for failure
  */
typedef int16_t coap_msg_process_cb(int8_t socket_id, int8_t recv_if_id, sn_coap_hdr_s *coap_message, coap_transaction_t *transaction_ptr, const uint8_t *local_addr);

extern coap_msg_handler_t *coap_message_handler_init(void *(*used_malloc_func_ptr)(uint16_t), void (*used_free_func_ptr)(void *),
                                                     uint8_t (*used_tx_callback_ptr)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *));

extern int8_t coap_message_handler_destroy(coap_msg_handler_t *handle);

extern coap_transaction_t *coap_message_handler_transaction_valid(coap_transaction_t *tr_ptr);

extern coap_transaction_t *coap_message_handler_find_transaction(uint8_t *address_ptr, uint16_t port);

extern int16_t coap_message_handler_coap_msg_process(coap_msg_handler_t *handle, int8_t socket_id, int8_t recv_if_id, const uint8_t source_addr_ptr[static 16], uint16_t port,
                                                     const uint8_t dst_addr_ptr[static 16], uint8_t *data_ptr, uint16_t data_len, coap_msg_process_cb *msg_process_callback);

extern uint16_t coap_message_handler_request_send(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16],
                                                  uint16_t destination_port, sn_coap_msg_type_e msg_type, sn_coap_msg_code_e msg_code, const char *uri, sn_coap_content_format_e cont_type,
                                                  const uint8_t *payload_ptr, uint16_t payload_len, coap_message_handler_response_recv *request_response_cb);

extern int8_t coap_message_handler_response_send(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code,
                                                 sn_coap_content_format_e content_type, const uint8_t *payload_ptr, uint16_t payload_len);

extern int8_t coap_message_handler_request_delete(coap_msg_handler_t *handle, int8_t service_id, uint16_t msg_id);

extern int8_t coap_message_handler_request_delete_by_service_id(coap_msg_handler_t *handle, int8_t service_id);

extern int8_t coap_message_handler_exec(coap_msg_handler_t *handle, uint32_t current_time);

extern void transaction_delete(coap_transaction_t *this);

extern void transactions_delete_all(uint8_t *address_ptr, uint16_t port);

extern int8_t coap_message_handler_response_send_by_msg_id(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, uint16_t msg_id, sn_coap_msg_code_e message_code,
                                                           sn_coap_content_format_e content_type, const uint8_t *payload_ptr, uint16_t payload_len);

#endif
