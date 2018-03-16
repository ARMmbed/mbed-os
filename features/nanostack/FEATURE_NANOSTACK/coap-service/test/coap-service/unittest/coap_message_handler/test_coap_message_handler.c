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
#include "test_coap_message_handler.h"
#include <string.h>
#include "coap_message_handler.h"
#include "sn_coap_protocol_stub.h"
#include "nsdynmemLIB_stub.h"
#include "sn_coap_builder_stub.h"
#include "sn_coap_parser_stub.h"
#include "socket_api.h"
#include "coap_message_handler.c"

int retCounter = 0;
int retValue = 0;
int transaction_cb = 0;

static void *test_own_alloc(uint16_t size)
{
    if( retCounter > 0 ){
        retCounter--;
        return malloc(size);
    }
    return NULL;
}

static void test_own_free(void *ptr)
{
    if (ptr) {
        free(ptr);
    }
}

static uint8_t coap_tx_function(uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr, void *param)
{
    return 0;
}

int resp_recv(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr){
    return retValue;
}

int16_t process_cb(int8_t a, sn_coap_hdr_s *b, coap_transaction_t *c)
{
    return retValue;
}

static int transaction_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    transaction_cb = 1;
    return 1;
}

bool test_coap_message_handler_init()
{
    if( NULL != coap_message_handler_init(NULL, NULL, NULL) )
        return false;
    if( NULL != coap_message_handler_init(&test_own_alloc, NULL, NULL) )
        return false;
    if( NULL != coap_message_handler_init(&test_own_alloc, &test_own_free, NULL) )
        return false;
    if( NULL != coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function) )
        return false;
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = NULL;
    if( NULL != coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function) )
        return false;
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);
    if( NULL == handle )
        return false;
    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    free(handle);
    return true;
}

bool test_coap_message_handler_destroy()
{
    if( -1 != coap_message_handler_destroy(NULL) )
        return false;
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    if( 0 != coap_message_handler_destroy(handle) )
        return false;

    free(sn_coap_protocol_stub.expectedCoap);
    return true;
}

bool test_coap_message_handler_find_transaction()
{
    if( NULL != coap_message_handler_find_transaction(NULL, 0))
        return false;
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    uint8_t buf[16];
    memset(&buf, 1, 16);
    char uri[3];
    uri[0] = "r";
    uri[1] = "s";
    uri[2] = "\0";

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &resp_recv))
        return false;

    if( NULL == coap_message_handler_find_transaction(&buf, 24))
        return false;

    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}

bool test_coap_message_handler_coap_msg_process()
{
    uint8_t buf[16];
    memset(&buf, 1, 16);
    /*Handler is null*/
    if( -1 != coap_message_handler_coap_msg_process(NULL, 0, buf, 22, ns_in6addr_any, NULL, 0, NULL))
        return false;

    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    sn_coap_protocol_stub.expectedHeader = NULL;
    /* Coap parse returns null */
    if( -1 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = 66;
    /* Coap library responds */
    if( -1 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = COAP_STATUS_OK;
    sn_coap_protocol_stub.expectedHeader->msg_code = 1;
    retValue = 0;
    /* request received */
    if( 0 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = COAP_STATUS_OK;
    sn_coap_protocol_stub.expectedHeader->msg_code = 1;
    nsdynmemlib_stub.returnCounter = 1;
    retValue = -1;
    if( 0 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = COAP_STATUS_OK;
    sn_coap_protocol_stub.expectedHeader->msg_code = 333;

    if( -1 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

    sn_coap_protocol_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_protocol_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_protocol_stub.expectedHeader->coap_status = COAP_STATUS_OK;
    sn_coap_protocol_stub.expectedHeader->msg_code = 333;

    char uri[3];
    uri[0] = "r";
    uri[1] = "s";
    uri[2] = "\0";

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &resp_recv))
        return false;

    sn_coap_protocol_stub.expectedHeader->msg_id = 2;
//    sn_coap_protocol_stub.expectedHeader->token_ptr = (uint8_t*)malloc(4);
//    memset(sn_coap_protocol_stub.expectedHeader->token_ptr, 1, 4);
    if( -1 != coap_message_handler_coap_msg_process(handle, 0, buf, 22, ns_in6addr_any, NULL, 0, process_cb))
        return false;

//    free(sn_coap_protocol_stub.expectedHeader->token_ptr);

    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}

bool test_coap_message_handler_request_send()
{
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    uint8_t buf[16];
    memset(&buf, 1, 16);
    char uri[3];
    uri[0] = "r";
    uri[1] = "s";
    uri[2] = "\0";
    if( 0 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, NULL))
        return false;

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, NULL))
        return false;

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 0 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, NULL))
        return false;

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 0 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, NULL))
        return false;

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &resp_recv))
        return false;

    /* Clear all transactions */
    if( 0 != coap_message_handler_exec(handle, 0xffffffff))
        return false;

    sn_coap_protocol_stub.expectedInt16 = -4;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &transaction_recv_cb))
        return false;

    transaction_cb = 0;
    sn_coap_protocol_stub.expectedInt8 = 0;
    if( 0 != coap_message_handler_exec(handle, 12))
        return false;

    if (transaction_cb != 1)
        return false;

    sn_coap_protocol_stub.expectedInt16 = -2;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &transaction_recv_cb))
        return false;

    transaction_cb = 0;
    if( 0 != coap_message_handler_exec(handle, 2)) {
        return false;
    }
    if (transaction_cb != 1)
        return false;


    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}

bool test_coap_message_handler_request_delete()
{
    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    uint8_t buf[16];
    memset(&buf, 1, 16);
    char uri[3];
    uri[0] = "r";
    uri[1] = "s";
    uri[2] = "\0";
    if( 0 == coap_message_handler_request_delete(NULL, 1, 1))
        return false;

    if( 0 == coap_message_handler_request_delete(handle, 1, 1))
        return false;

    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &resp_recv))
        return false;

    if( 0 != coap_message_handler_request_delete(handle, 1, 2))
        return false;

    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}

bool test_coap_message_handler_response_send()
{
    if( -1 != coap_message_handler_response_send(NULL, 2, 0, NULL, 1,3,NULL, 0))
        return false;

    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);
    sn_coap_hdr_s *header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(header, 0, sizeof(sn_coap_hdr_s));

    if( -2 != coap_message_handler_response_send(handle, 2, 0, header, 1,3,NULL, 0))
        return false;

    uint8_t buf[16];
    memset(&buf, 1, 16);
    char uri[3];
    uri[0] = "r";
    uri[1] = "s";
    uri[2] = "\0";
    sn_coap_builder_stub.expectedUint16 = 1;
    nsdynmemlib_stub.returnCounter = 3;
    if( 2 != coap_message_handler_request_send(handle, 3, 0, buf, 24, 1, 2, &uri, 4, NULL, 0, &resp_recv))
        return false;

    header->msg_id = 2;
    sn_coap_builder_stub.expectedUint16 = 2;
    coap_transaction_t * tx = coap_message_handler_find_transaction(&buf, 24);
    if( tx ){
        tx->client_request = false;
    }
    sn_coap_builder_stub.expectedHeader = NULL;
    if( -1 != coap_message_handler_response_send(handle, 2, 0, header, 1,3,NULL, 0))
        return false;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    nsdynmemlib_stub.returnCounter = 0;
    if( -1 != coap_message_handler_response_send(handle, 2, 0, header, 1,3,NULL, 0))
        return false;

    sn_coap_builder_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_builder_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != coap_message_handler_response_send(handle, 2, 0, header, 1,3,NULL, 0))
        return false;

    free(header);
    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}

bool test_coap_message_handler_exec()
{
    /* Null as a parameter */
    if( -1 != coap_message_handler_exec(NULL, 0))
        return false;

    retCounter = 1;
    sn_coap_protocol_stub.expectedCoap = (struct coap_s*)malloc(sizeof(struct coap_s));
    memset(sn_coap_protocol_stub.expectedCoap, 0, sizeof(struct coap_s));
    coap_msg_handler_t *handle = coap_message_handler_init(&test_own_alloc, &test_own_free, &coap_tx_function);

    if( 0 != coap_message_handler_exec(handle, 0))
        return false;

    nsdynmemlib_stub.returnCounter = 1;
    coap_transaction_t *transact_ptr = transaction_create();

    /* Transaction not timed out*/
    if( 0 != coap_message_handler_exec(handle, 0))
        return false;

    if (transaction_cb != 0)
        return false;

    /* Timed out, no CB */
    if( 0 != coap_message_handler_exec(handle, 300))
        return false;

    if (transaction_cb != 0)
        return false;

    nsdynmemlib_stub.returnCounter = 1;
    transact_ptr = transaction_create();
    transact_ptr->resp_cb = transaction_recv_cb;

    /* Transaction not timed out */
    if( 0 != coap_message_handler_exec(handle, 0))
        return false;

    if (transaction_cb != 0)
        return false;

    /* Transaction timed out */
    if( 0 != coap_message_handler_exec(handle, 300))
        return false;

    if (transaction_cb == 0)
        return false;

    /* Teardown */
    free(sn_coap_protocol_stub.expectedCoap);
    sn_coap_protocol_stub.expectedCoap = NULL;
    coap_message_handler_destroy(handle);
    return true;
}
