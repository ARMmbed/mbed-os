/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
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

#include <string.h>
#include "nsdynmemLIB.h"
#include "coap_service_api_internal.h"
#include "coap_message_handler.h"
#include "mbed-coap/sn_coap_protocol.h"
#include "source/include/sn_coap_protocol_internal.h"
#include "socket_api.h"
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "randLIB.h"

#define TRACE_GROUP "CoSA"

static void *own_alloc(uint16_t size)
{
    if (size) {
        return ns_dyn_mem_temporary_alloc(size);
    } else {
        return 0;
    }
}

static void own_free(void *ptr)
{
    if (ptr) {
        ns_dyn_mem_free(ptr);
    }
}

static NS_LIST_DEFINE(request_list, coap_transaction_t, link);

static coap_transaction_t *transaction_find_client_by_token(uint8_t *token, uint8_t token_len, const uint8_t address[static 16], uint16_t port)
{
    (void) address;
    (void) port;
    coap_transaction_t *this = NULL;

    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if ((cur_ptr->token_len == token_len) && (memcmp(cur_ptr->token, token, token_len) == 0) && cur_ptr->client_request) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_transaction_t *transaction_find_server(uint16_t msg_id)
{
    coap_transaction_t *this = NULL;
    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if (cur_ptr->msg_id == msg_id && !cur_ptr->client_request) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_transaction_t *transaction_find_client(uint16_t msg_id)
{
    coap_transaction_t *this = NULL;
    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if (cur_ptr->msg_id == msg_id && cur_ptr->client_request) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_transaction_t *transaction_find_by_address(uint8_t *address_ptr, uint16_t port)
{
    coap_transaction_t *this = NULL;
    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if (cur_ptr->remote_port == port && memcmp(cur_ptr->remote_address, address_ptr, 16) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_transaction_t *transaction_find_by_service_id(int8_t service_id)
{
    coap_transaction_t *this = NULL;
    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if (cur_ptr->service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

/* retransmission valid time is calculated to be max. time that CoAP message sending can take: */
/* Number of retransmisisons, each retransmission is 2 * previous retransmisison time */
/* + random factor (max. 1.5) */
static uint32_t transaction_valid_time_calculate(void)
{
    int i;
    uint32_t time_valid = 0;

    for (i = 0; i <= COAP_RESENDING_COUNT; i++) {
        time_valid += (COAP_RESENDING_INTERVAL << i) * 1.5;
    }

    return time_valid + coap_service_get_internal_timer_ticks();
}

static coap_transaction_t *transaction_create(void)
{
    coap_transaction_t *this = ns_dyn_mem_alloc(sizeof(coap_transaction_t));
    if (this) {
        memset(this, 0, sizeof(coap_transaction_t));
        this->client_request = true;// default to client initiated method
        this->valid_until = transaction_valid_time_calculate();
        ns_list_add_to_start(&request_list, this);
    }

    return this;
}

static void transaction_free(coap_transaction_t *this)
{
    if (this->data_ptr) {
        ns_dyn_mem_free(this->data_ptr);
    }
    ns_dyn_mem_free(this);
}

void transaction_delete(coap_transaction_t *this)
{
    if (!coap_message_handler_transaction_valid(this)) {
        return;
    }
    ns_list_remove(&request_list, this);
    transaction_free(this);

    return;
}

void transactions_delete_all(uint8_t *address_ptr, uint16_t port)
{
    coap_transaction_t *transaction = transaction_find_by_address(address_ptr, port);

    while (transaction) {
        ns_list_remove(&request_list, transaction);
        if (transaction->resp_cb) {
            transaction->resp_cb(transaction->service_id, address_ptr, port, NULL);
        }
        sn_coap_protocol_delete_retransmission(coap_service_handle->coap, transaction->msg_id);
        transaction_free(transaction);
        transaction = transaction_find_by_address(address_ptr, port);
    }
}

static void transactions_delete_all_by_service_id(int8_t service_id)
{
    coap_transaction_t *transaction = transaction_find_by_service_id(service_id);

    while (transaction) {
        ns_list_remove(&request_list, transaction);
        if (transaction->resp_cb) {
            transaction->resp_cb(transaction->service_id, transaction->remote_address, transaction->remote_port, NULL);
        }
        sn_coap_protocol_delete_retransmission(coap_service_handle->coap, transaction->msg_id);
        transaction_free(transaction);
        transaction = transaction_find_by_service_id(service_id);
    }
}

static int8_t coap_rx_function(sn_coap_hdr_s *resp_ptr, sn_nsdl_addr_s *address_ptr, void *param)
{
    coap_transaction_t *this = NULL;
    (void)param;

    if (!resp_ptr || !address_ptr) {
        return -1;
    }

    tr_warn("transaction was not handled %d", resp_ptr->msg_id);

    if (resp_ptr->coap_status == COAP_STATUS_BUILDER_BLOCK_SENDING_DONE) {
        return 0;
    }

    if (resp_ptr->token_ptr) {
        this = transaction_find_client_by_token(resp_ptr->token_ptr, resp_ptr->token_len, address_ptr->addr_ptr, address_ptr->port);
    }
    if (!this) {
        return 0;
    }

    ns_list_remove(&request_list, this);
    if (this->resp_cb) {
        this->resp_cb(this->service_id, address_ptr->addr_ptr, address_ptr->port, NULL);
    }
    transaction_free(this);
    return 0;
}

coap_msg_handler_t *coap_message_handler_init(void *(*used_malloc_func_ptr)(uint16_t), void (*used_free_func_ptr)(void *),
                                              uint8_t (*used_tx_callback_ptr)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *))
{

    if ((used_malloc_func_ptr == NULL) || (used_free_func_ptr == NULL) || (used_tx_callback_ptr == NULL)) {
        return NULL;
    }

    coap_msg_handler_t *handle;
    handle = ns_dyn_mem_alloc(sizeof(coap_msg_handler_t));
    if (handle == NULL) {
        return NULL;
    }

    memset(handle, 0, sizeof(coap_msg_handler_t));

    handle->sn_coap_tx_callback = used_tx_callback_ptr;

    handle->sn_coap_service_free = used_free_func_ptr;
    handle->sn_coap_service_malloc = used_malloc_func_ptr;

    handle->coap = sn_coap_protocol_init(used_malloc_func_ptr, used_free_func_ptr, used_tx_callback_ptr, &coap_rx_function);
    if (!handle->coap) {
        ns_dyn_mem_free(handle);
        return NULL;
    }

    /* Set default buffer size for CoAP duplicate message detection */
    sn_coap_protocol_set_duplicate_buffer_size(handle->coap, DUPLICATE_MESSAGE_BUFFER_SIZE);

    /* Set default blockwise message size. */
    sn_coap_protocol_set_block_size(handle->coap, DEFAULT_BLOCKWISE_DATA_SIZE);

    /* Set default CoAP retransmission paramters */
    sn_coap_protocol_set_retransmission_parameters(handle->coap, COAP_RESENDING_COUNT, COAP_RESENDING_INTERVAL);

    return handle;
}

int8_t coap_message_handler_destroy(coap_msg_handler_t *handle)
{
    if (!handle) {
        return -1;
    }

    if (handle->coap) {
        sn_coap_protocol_destroy(handle->coap);
    }

    //Destroy transactions
    ns_list_foreach_safe(coap_transaction_t, cur_ptr, &request_list) {
        ns_list_remove(&request_list, cur_ptr);
        ns_dyn_mem_free(cur_ptr);
        cur_ptr = NULL;
    }

    handle->sn_coap_service_free(handle);
    return 0;
}

coap_transaction_t *coap_message_handler_transaction_valid(coap_transaction_t *tr_ptr)
{
    ns_list_foreach(coap_transaction_t, cur_ptr, &request_list) {
        if (cur_ptr == tr_ptr) {
            return tr_ptr;
        }
    }
    return NULL;
}

coap_transaction_t *coap_message_handler_find_transaction(uint8_t *address_ptr, uint16_t port)
{
    if (!address_ptr) {
        return NULL;
    }
    return transaction_find_by_address(address_ptr, port);
}

int16_t coap_message_handler_coap_msg_process(coap_msg_handler_t *handle, int8_t socket_id, int8_t recv_if_id, const uint8_t source_addr_ptr[static 16], uint16_t port, const uint8_t dst_addr_ptr[static 16],
                                              uint8_t *data_ptr, uint16_t data_len,  coap_msg_process_cb *msg_process_callback)
{
    sn_nsdl_addr_s src_addr;
    sn_coap_hdr_s *coap_message;
    int16_t ret_val = 0;
    coap_transaction_t *this = NULL;

    if (!msg_process_callback || !handle) {
        return -1;
    }

    src_addr.addr_ptr = (uint8_t *)source_addr_ptr;
    src_addr.addr_len  =  16;
    src_addr.type  =  SN_NSDL_ADDRESS_TYPE_IPV6;
    src_addr.port  =  port;

    coap_transaction_t *transaction_ptr = transaction_create();
    if (!transaction_ptr) {
        return -1;
    }
    transaction_ptr->service_id = coap_service_id_find_by_socket(socket_id);
    transaction_ptr->client_request = false;// this is server transaction
    memcpy(transaction_ptr->local_address, *(dst_addr_ptr) == 0xFF ? ns_in6addr_any : dst_addr_ptr, 16);
    memcpy(transaction_ptr->remote_address, source_addr_ptr, 16);
    transaction_ptr->remote_port = port;

    coap_message = sn_coap_protocol_parse(handle->coap, &src_addr, data_len, data_ptr, transaction_ptr);
    if (coap_message == NULL) {
        transaction_delete(transaction_ptr);
        tr_err("CoAP Parsing failed");
        return -1;
    }

    tr_debug("CoAP status:%d, type:%d, code:%d, id:%d", coap_message->coap_status, coap_message->msg_type, coap_message->msg_code, coap_message->msg_id);

    /* Check, if coap itself sends response, or block receiving is ongoing... */
    if (coap_message->coap_status != COAP_STATUS_OK && coap_message->coap_status != COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED) {
        tr_debug("CoAP library responds");
        transaction_delete(transaction_ptr);
        ret_val = -1;
        goto exit;
    }

    if (coap_message->msg_code > 0 && coap_message->msg_code < 32) {
        /* Request received */
        transaction_ptr->msg_id = coap_message->msg_id;
        transaction_ptr->req_msg_type = coap_message->msg_type;
        if (coap_message->token_len) {
            memcpy(transaction_ptr->token, coap_message->token_ptr, coap_message->token_len);
            transaction_ptr->token_len = coap_message->token_len;
        }
        if (msg_process_callback(socket_id, recv_if_id, coap_message, transaction_ptr, dst_addr_ptr) < 0) {
            // negative return value = message ignored -> delete transaction
            transaction_delete(transaction_ptr);
        }
        goto exit;
    } else {
        /* Response received */
        transaction_delete(transaction_ptr); // transaction_ptr not needed in response
        if (coap_message->token_ptr) {
            this = transaction_find_client_by_token(coap_message->token_ptr, coap_message->token_len, source_addr_ptr, port);
        }
        if (!this) {
            tr_error("client transaction not found");
            ret_val = -1;
            goto exit;
        }
        tr_debug("Service %d, response received", this->service_id);
        ns_list_remove(&request_list, this);
        if (this->resp_cb) {
            this->resp_cb(this->service_id, (uint8_t *)source_addr_ptr, port, coap_message);
        }
        transaction_free(this);
    }

exit:
    if (coap_message->coap_status == COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED) {
        handle->sn_coap_service_free(coap_message->payload_ptr);
    }

    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, coap_message);

    return ret_val;
}

uint16_t coap_message_handler_request_send(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16],
                                           uint16_t destination_port, sn_coap_msg_type_e msg_type, sn_coap_msg_code_e msg_code, const char *uri,
                                           sn_coap_content_format_e cont_type, const uint8_t *payload_ptr, uint16_t payload_len, coap_message_handler_response_recv *request_response_cb)
{
    coap_transaction_t *transaction_ptr;
    sn_coap_hdr_s request;
    sn_nsdl_addr_s dst_addr;
    uint8_t token[4];
    uint16_t data_len;
    uint8_t *data_ptr;

    tr_debug("Service %d, send CoAP request payload_len %d", service_id, payload_len);
    transaction_ptr = transaction_create();

    if (!uri || !transaction_ptr || !handle) {
        return 0;
    }

    transaction_ptr->service_id = service_id;
    transaction_ptr->client_request = true;
    transaction_ptr->resp_cb = request_response_cb;
    transaction_ptr->options = options;
    memcpy(transaction_ptr->remote_address, destination_addr, 16);
    transaction_ptr->remote_port = destination_port;
    transaction_ptr->req_msg_type = msg_type;
    memset(&request, 0, sizeof(request));
    dst_addr.addr_ptr = (uint8_t *) destination_addr; // Cast away const and trust that nsdl doesn't modify...
    dst_addr.addr_len  =  16;
    dst_addr.type  =  SN_NSDL_ADDRESS_TYPE_IPV6;
    dst_addr.port  =  destination_port;

    request.msg_type = msg_type;
    request.msg_code = msg_code;
    request.uri_path_ptr = (uint8_t *)uri;
    request.uri_path_len = strlen(uri);
    request.content_format = cont_type;

    do {
        randLIB_get_n_bytes_random(token, 4);
    } while (transaction_find_client_by_token(token, 4, destination_addr, destination_port));
    memcpy(transaction_ptr->token, token, 4);
    transaction_ptr->token_len = 4;
    request.token_ptr = transaction_ptr->token;
    request.token_len = 4;

    request.payload_len = payload_len;
    request.payload_ptr = (uint8_t *) payload_ptr;  // Cast away const and trust that nsdl doesn't modify...

    prepare_blockwise_message(handle->coap, &request);

    data_len = sn_coap_builder_calc_needed_packet_data_size_2(&request, sn_coap_protocol_get_configured_blockwise_size(handle->coap));
    data_ptr = own_alloc(data_len);
    if (data_len > 0 && !data_ptr) {
        transaction_delete(transaction_ptr);
        return 0;
    }
    int16_t sn_coap_ret = sn_coap_protocol_build(handle->coap, &dst_addr, data_ptr, &request, transaction_ptr);
    if (sn_coap_ret == -4) {
        /*
         * Not able to add message to resend queue, adjust message lifetime to one resending
         */
        transaction_ptr->valid_until = coap_service_get_internal_timer_ticks() + COAP_RESENDING_INTERVAL;
    } else if (sn_coap_ret < 0) {
        /*
         * Failed to build message, set transaction validity time to minimum to get this transaction cleared
         * immediately and callback called.
         */
        transaction_ptr->valid_until = coap_service_get_internal_timer_ticks();
    }

    transaction_ptr->msg_id = request.msg_id;
    handle->sn_coap_tx_callback(data_ptr, data_len, &dst_addr, transaction_ptr);

    // Free allocated data
    own_free(data_ptr);
    if (request.options_list_ptr) {
        own_free(request.options_list_ptr);
    }

    if (request_response_cb == NULL) {
        //No response expected
        return 0;
    }
    return request.msg_id;
}

static int8_t coap_message_handler_resp_build_and_send(coap_msg_handler_t *handle, sn_coap_hdr_s *coap_msg_ptr, coap_transaction_t *transaction_ptr)
{
    sn_nsdl_addr_s dst_addr;
    uint16_t data_len;
    uint8_t *data_ptr;


    dst_addr.addr_ptr  =  transaction_ptr->remote_address;
    dst_addr.addr_len  =  16;
    dst_addr.type  =  SN_NSDL_ADDRESS_TYPE_IPV6;
    dst_addr.port  =  transaction_ptr->remote_port;
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    prepare_blockwise_message(handle->coap, coap_msg_ptr);
#endif
    data_len = sn_coap_builder_calc_needed_packet_data_size_2(coap_msg_ptr, sn_coap_protocol_get_configured_blockwise_size(handle->coap));
    data_ptr = own_alloc(data_len);
    if (data_len > 0 && !data_ptr) {
        return -1;
    }
    sn_coap_protocol_build(handle->coap, &dst_addr, data_ptr, coap_msg_ptr, transaction_ptr);

    handle->sn_coap_tx_callback(data_ptr, data_len, &dst_addr, transaction_ptr);
    own_free(data_ptr);

    return 0;

}

int8_t coap_message_handler_response_send(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code, sn_coap_content_format_e content_type, const uint8_t *payload_ptr, uint16_t payload_len)
{
    sn_coap_hdr_s *response;
    coap_transaction_t *transaction_ptr;
    int8_t ret_val = 0;
    (void) options;
    (void)service_id;

    tr_debug("Service %d, send CoAP response", service_id);
    if (!request_ptr || !handle) {
        tr_error("invalid params");
        return -1;
    }

    transaction_ptr = transaction_find_server(request_ptr->msg_id);

    if (!transaction_ptr) {
        tr_error("response transaction not found");
        return -2;
    }

    response = sn_coap_build_response(handle->coap, request_ptr, message_code);
    if (!response) {
        return -1;
    }
    response->payload_len = payload_len;
    response->payload_ptr = (uint8_t *) payload_ptr;  // Cast away const and trust that nsdl doesn't modify...
    response->content_format = content_type;


    ret_val =  coap_message_handler_resp_build_and_send(handle, response, transaction_ptr);
    sn_coap_parser_release_allocated_coap_msg_mem(handle->coap, response);
    if (ret_val == 0) {
        transaction_delete(transaction_ptr);
    }

    return ret_val;
}

int8_t coap_message_handler_response_send_by_msg_id(coap_msg_handler_t *handle, int8_t service_id, uint8_t options, uint16_t msg_id, sn_coap_msg_code_e message_code, sn_coap_content_format_e content_type, const uint8_t *payload_ptr, uint16_t payload_len)
{
    sn_coap_hdr_s response;
    coap_transaction_t *transaction_ptr;
    int8_t ret_val;

    (void) options;
    (void)service_id;

    transaction_ptr = transaction_find_server(msg_id);
    if (!transaction_ptr || !handle) {
        return -1;
    }

    tr_debug("Service %d, send CoAP response", service_id);

    memset(&response, 0, sizeof(sn_coap_hdr_s));

    response.payload_len = payload_len;
    response.payload_ptr = (uint8_t *) payload_ptr;  // Cast away const and trust that nsdl doesn't modify...
    response.content_format = content_type;
    response.token_len = transaction_ptr->token_len;
    response.token_ptr = transaction_ptr->token;
    response.msg_code = message_code;
    if (transaction_ptr->req_msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        response.msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
        response.msg_id = msg_id;
    } else {
        response.msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    }

    ret_val = coap_message_handler_resp_build_and_send(handle, &response, transaction_ptr);
    if (ret_val == 0) {
        transaction_delete(transaction_ptr);
    }

    return ret_val;
}

int8_t coap_message_handler_request_delete(coap_msg_handler_t *handle, int8_t service_id, uint16_t msg_id)
{
    coap_transaction_t *transaction_ptr;
    (void)service_id;


    tr_debug("Service %d, delete CoAP request %d", service_id, msg_id);
    if (!handle) {
        tr_error("invalid params");
        return -1;
    }

    sn_coap_protocol_delete_retransmission(handle->coap, msg_id);

    transaction_ptr = transaction_find_client(msg_id);
    if (!transaction_ptr) {
        tr_error("response transaction not found");
        return -2;
    }

    if (transaction_ptr->resp_cb) {
        transaction_ptr->resp_cb(transaction_ptr->service_id, transaction_ptr->remote_address, transaction_ptr->remote_port, NULL);
    }
    transaction_delete(transaction_ptr);
    return 0;
}

int8_t coap_message_handler_request_delete_by_service_id(coap_msg_handler_t *handle, int8_t service_id)
{
    tr_debug("Service %d, delete all CoAP requests", service_id);

    if (!handle) {
        tr_error("invalid params");
        return -1;
    }

    transactions_delete_all_by_service_id(service_id);

    return 0;
}

int8_t coap_message_handler_exec(coap_msg_handler_t *handle, uint32_t current_time)
{

    if (!handle) {
        return -1;
    }

    // Remove outdated transactions from queue
    ns_list_foreach_safe(coap_transaction_t, transaction, &request_list) {
        if (transaction->valid_until < current_time) {
            tr_debug("transaction %d timed out", transaction->msg_id);
            ns_list_remove(&request_list, transaction);
            if (transaction->resp_cb) {
                transaction->resp_cb(transaction->service_id, transaction->remote_address, transaction->remote_port, NULL);
            }
            transaction_free(transaction);
        }
    }

    return sn_coap_protocol_exec(handle->coap, current_time);
}
