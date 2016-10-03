/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CppUTest/TestHarness.h"
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "sn_nsdl.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"

#include "sn_coap_builder_stub.h"
#include "sn_coap_parser_stub.h"
#include "sn_coap_header_check_stub.h"


int retCounter = 0;
static coap_s *coap_handle = NULL;
void myFree(void* addr);
void* myMalloc(uint16_t size);
uint8_t null_tx_cb(uint8_t *a, uint16_t b, sn_nsdl_addr_s *c, void *d);

TEST_GROUP(libCoap_protocol)
{
    void setup() {
        retCounter = 1;
        coap_handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);
    }

    void teardown() {
        retCounter = 0;
        sn_coap_protocol_destroy(coap_handle);
    }
};

void* myMalloc(uint16_t size)
{
    if( retCounter > 0 ){
        retCounter--;
        return malloc(size);
    }else {
        return NULL;
    }
}

void myFree(void* addr){
    if( addr ){
        free(addr);
    }
}

uint8_t null_tx_cb(uint8_t *a, uint16_t b, sn_nsdl_addr_s *c, void *d)
{
    return 0;
}

int8_t null_rx_cb(sn_coap_hdr_s *a, sn_nsdl_addr_s *b, void *c)
{

}

TEST(libCoap_protocol, sn_coap_protocol_destroy)
{
    CHECK( -1 == sn_coap_protocol_destroy(NULL));
    struct coap_s *handle = (struct coap_s *)malloc(sizeof(struct coap_s));
    handle->sn_coap_protocol_free = &myFree;
    handle->sn_coap_protocol_malloc = &myMalloc;
    ns_list_init(&handle->linked_list_resent_msgs);
    coap_send_msg_s *msg_ptr = (coap_send_msg_s*)malloc(sizeof(coap_send_msg_s));
    memset(msg_ptr, 0, sizeof(coap_send_msg_s));
    msg_ptr->send_msg_ptr = (sn_nsdl_transmit_s*)malloc(sizeof(sn_nsdl_transmit_s));
    memset(msg_ptr->send_msg_ptr, 0 , sizeof(sn_nsdl_transmit_s));
    msg_ptr->send_msg_ptr->uri_path_ptr = (uint8_t*)malloc(2);
    msg_ptr->send_msg_ptr->uri_path_len = 2;

    ns_list_add_to_end(&handle->linked_list_resent_msgs, msg_ptr);
#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT
    ns_list_init(&handle->linked_list_duplication_msgs);
#endif
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    ns_list_init(&handle->linked_list_blockwise_sent_msgs);
    ns_list_init(&handle->linked_list_blockwise_received_payloads);
#endif
    CHECK( 0 == sn_coap_protocol_destroy(handle));
}

TEST(libCoap_protocol, sn_coap_protocol_init_null_func_ptrs)
{
    POINTERS_EQUAL(NULL, sn_coap_protocol_init(NULL, NULL, NULL, NULL));
}

TEST(libCoap_protocol, sn_coap_protocol_init_null_malloc)
{
    POINTERS_EQUAL(NULL, sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL));

    retCounter = 1;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);
    CHECK(NULL != handle);

    sn_coap_protocol_destroy(handle);
}

TEST(libCoap_protocol, sn_coap_protocol_set_block_size)
{
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    CHECK( 0 == sn_coap_protocol_set_block_size(coap_handle,16) );
    CHECK( -1 == sn_coap_protocol_set_block_size(NULL,1) );
#endif
    CHECK( -1 == sn_coap_protocol_set_block_size(coap_handle,1) );
}

TEST(libCoap_protocol, sn_coap_protocol_set_duplicate_buffer_size)
{
#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT
    CHECK( 0 == sn_coap_protocol_set_duplicate_buffer_size(coap_handle,3));
    CHECK( -1 == sn_coap_protocol_set_duplicate_buffer_size(NULL,3));
#endif
    CHECK( -1 == sn_coap_protocol_set_duplicate_buffer_size(coap_handle,999));
}

TEST(libCoap_protocol, sn_coap_protocol_set_retransmission_parameters)
{
#if ENABLE_RESENDINGS
    CHECK( 0 == sn_coap_protocol_set_retransmission_parameters(coap_handle,3,0) );
    CHECK( 0 == sn_coap_protocol_set_retransmission_parameters(coap_handle,3, 10) );
    CHECK( -1 == sn_coap_protocol_set_retransmission_parameters(NULL,3,0) );
#endif
    CHECK( -1 == sn_coap_protocol_set_retransmission_parameters(coap_handle,999,0) )
}

TEST(libCoap_protocol, sn_coap_protocol_set_retransmission_buffer)
{
#if ENABLE_RESENDINGS
    CHECK( 0 == sn_coap_protocol_set_retransmission_buffer(coap_handle,3,3) );
    CHECK( -1 == sn_coap_protocol_set_retransmission_buffer(NULL,3,3) );
#endif
    CHECK( -1 == sn_coap_protocol_set_retransmission_buffer(coap_handle,3,999) );
}

//TEST(libCoap_protocol, sn_coap_protocol_clear_retransmission_buffer)
//{
//    sn_coap_protocol_clear_retransmission_buffer();
//}
#include <stdio.h>

TEST(libCoap_protocol, sn_coap_protocol_delete_retransmission)
{
#if ENABLE_RESENDINGS
    retCounter = 6;
    sn_nsdl_addr_s dst_addr_ptr;
    sn_coap_hdr_s src_coap_msg_ptr;
    uint8_t temp_addr[4] = {0};
    uint8_t dst_packet_data_ptr[4] = {0x40, 0x00, 0x00, 0x63};

    memset(&dst_addr_ptr, 0, sizeof(sn_nsdl_addr_s));
    memset(&src_coap_msg_ptr, 0, sizeof(sn_coap_hdr_s));

    dst_addr_ptr.addr_ptr = temp_addr;
    dst_addr_ptr.addr_len = 4;
    dst_addr_ptr.type = SN_NSDL_ADDRESS_TYPE_IPV4;

    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    CHECK( -1 == sn_coap_protocol_delete_retransmission(NULL, 0));

    CHECK( -2 == sn_coap_protocol_delete_retransmission(handle, 0));

    sn_coap_builder_stub.expectedInt16 = 4;

    CHECK( 0 < sn_coap_protocol_build(handle, &dst_addr_ptr, dst_packet_data_ptr, &src_coap_msg_ptr, NULL));

    CHECK( 0 == sn_coap_protocol_delete_retransmission(handle, 99));

    sn_coap_protocol_destroy(handle);
#endif
}

TEST(libCoap_protocol, sn_coap_protocol_build)
{
    retCounter = 1;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);
    sn_nsdl_addr_s addr;
    memset(&addr, 0, sizeof(sn_nsdl_addr_s));
    sn_coap_hdr_s hdr;
    memset(&hdr, 0, sizeof(sn_coap_hdr_s));

    uint8_t* dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    CHECK( -2 == sn_coap_protocol_build(NULL, NULL, NULL, NULL, NULL));

    CHECK( -2 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    hdr.msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    hdr.msg_id = 0;

    addr.addr_ptr = (uint8_t*)malloc(5);
    memset(addr.addr_ptr, '1', 5);

    sn_coap_builder_stub.expectedInt16 = 0;

    CHECK( 0 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    hdr.payload_ptr = (uint8_t*)malloc(SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    memset(hdr.payload_ptr, '1', SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;

    sn_coap_builder_stub.expectedInt16 = -3;
//    CHECK( -2 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 0;
    hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    hdr.options_list_ptr->block1 = 67777;

    CHECK( -3 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));
    free(hdr.options_list_ptr);
    hdr.options_list_ptr = NULL;

    retCounter = 2;
    CHECK( -3 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    free(hdr.options_list_ptr);
    hdr.options_list_ptr = NULL;

    hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    hdr.msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    hdr.options_list_ptr->block2 = 1;
    retCounter = 0;

    CHECK( -3 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));
    free(hdr.options_list_ptr);
    hdr.options_list_ptr = NULL;

    free(hdr.payload_ptr);
    hdr.payload_ptr = NULL;
    hdr.payload_len = 0;

    //Test variations of sn_coap_convert_block_size here -->
    for( int i=0; i < 8; i++ ){
        uint16_t multiplier = 16*pow(2, i);
        sn_coap_protocol_set_block_size(handle,multiplier);
        hdr.payload_ptr = (uint8_t*)malloc(multiplier + 20);
        memset(hdr.payload_ptr, '1', multiplier + 20);
        hdr.payload_len = multiplier + 20;
        retCounter = 2;
        hdr.msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        CHECK( -3 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));
        hdr.msg_code = COAP_MSG_CODE_EMPTY;

        free(hdr.options_list_ptr);
        hdr.options_list_ptr = NULL;

        free(hdr.payload_ptr);
        hdr.payload_ptr = NULL;
        hdr.payload_len = 0;
    }
    sn_coap_protocol_set_block_size(handle,SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE);

    // <-- Test variations of sn_coap_convert_block_size here

    retCounter = 1;
    sn_coap_builder_stub.expectedInt16 = -1;
    CHECK( -1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 1;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    hdr.msg_code = COAP_MSG_CODE_EMPTY;

    retCounter = 2;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 3;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 4;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 5;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    // Test second SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE -->
    hdr.payload_ptr = (uint8_t*)malloc(SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    memset(hdr.payload_ptr, '1', SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;

    retCounter = 2;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));
    free(hdr.payload_ptr);

    hdr.payload_ptr = (uint8_t*)malloc(SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    memset(hdr.payload_ptr, '1', SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20);
    hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;

    retCounter = 4;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    free(hdr.payload_ptr);
    hdr.payload_ptr = (uint8_t*)malloc(UINT16_MAX);
    memset(hdr.payload_ptr, '1', UINT16_MAX);
    hdr.payload_len = UINT16_MAX;

    retCounter = 5;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    free(hdr.payload_ptr);
    hdr.payload_ptr = (uint8_t*)malloc(UINT16_MAX - 1);
    memset(hdr.payload_ptr, '1', UINT16_MAX - 1);
    hdr.payload_len = UINT16_MAX - 1;

    retCounter = 5;
    sn_coap_builder_stub.expectedInt16 = 1;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    sn_coap_protocol_destroy(handle);
    handle = NULL;
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);
    free(hdr.options_list_ptr);
    hdr.options_list_ptr = NULL;
    //Test sn_coap_protocol_copy_header here -->
    retCounter = 6;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    CHECK( -2 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    free(hdr.options_list_ptr);
    hdr.options_list_ptr = NULL;

    sn_coap_hdr_s* hdr2 = (sn_coap_hdr_s*)malloc(sizeof(sn_coap_hdr_s));
    memset(hdr2, 0, sizeof(sn_coap_hdr_s));
    hdr2->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    hdr2->uri_path_ptr = (uint8_t*)malloc(3);
    hdr2->uri_path_len = 3;
    hdr2->token_ptr = (uint8_t*)malloc(3);
    hdr2->token_len = 3;
    hdr2->content_format = COAP_CT_TEXT_PLAIN;

    hdr2->options_list_ptr = (sn_coap_options_list_s *)malloc(sizeof(sn_coap_options_list_s));
    memset(hdr2->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    hdr2->options_list_ptr->accept = COAP_CT_TEXT_PLAIN;
    hdr2->options_list_ptr->block1 = 67777;
    hdr2->options_list_ptr->block2 = 67777;
    hdr2->options_list_ptr->etag_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->etag_len = 3;
    hdr2->options_list_ptr->location_path_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->location_path_len = 3;
    hdr2->options_list_ptr->location_query_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->location_query_len = 3;
    hdr2->options_list_ptr->max_age = 3;
    hdr2->options_list_ptr->observe = 0;
    hdr2->options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->proxy_uri_len = 3;
    hdr2->options_list_ptr->uri_host_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->uri_host_len = 3;
    hdr2->options_list_ptr->uri_port = 3;
    hdr2->options_list_ptr->uri_query_ptr = (uint8_t*)malloc(3);
    hdr2->options_list_ptr->uri_query_len = 3;
    hdr2->options_list_ptr->use_size1 = true;
    hdr2->options_list_ptr->size1 = 0xFFFF01;

    hdr2->payload_ptr = (uint8_t*)malloc(3);

    for( int i=0; i < 8; i++ ){
        retCounter = 1 + i;
        sn_coap_builder_stub.expectedInt16 = 1;
        hdr2->payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
        int8_t rett = sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, hdr2, NULL);
        CHECK( -2 == rett );
    }

    retCounter = 11;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr2->payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, hdr2, NULL));

    retCounter = 19;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr2->payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, hdr2, NULL));

    free(hdr2->payload_ptr);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, hdr2);
    hdr2 = NULL;

    //<-- Test sn_coap_protocol_copy_header here

    hdr.msg_code = COAP_MSG_CODE_REQUEST_GET;
    retCounter = 5;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr.payload_len = 0;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    sn_coap_protocol_destroy(handle);
    handle = NULL;
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    retCounter = 6;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr.payload_len = 0;
    CHECK( -2 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    retCounter = 7;
    sn_coap_builder_stub.expectedInt16 = 1;
    hdr.payload_len = 0;
    CHECK( 1 == sn_coap_protocol_build(handle, &addr, dst_packet_data_ptr, &hdr, NULL));

    free(hdr.payload_ptr);
    hdr.payload_ptr = NULL;
    hdr.payload_len = 0;

    hdr.msg_code = COAP_MSG_CODE_EMPTY;

    // <-- Test second SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE

    free(addr.addr_ptr);
    free(dst_packet_data_ptr);
    sn_coap_protocol_destroy(handle);
}

TEST(libCoap_protocol, sn_coap_protocol_parse)
{
    CHECK( NULL == sn_coap_protocol_parse(NULL, NULL, 0, NULL, NULL) );

    retCounter = 1;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    addr->addr_ptr = (uint8_t*)malloc(5);

    uint8_t *packet_data_ptr = (uint8_t*)malloc(5);
    uint16_t packet_data_len = 5;

    sn_coap_parser_stub.expectedHeader = NULL;
    CHECK( NULL == sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL) );

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_header_check_stub.expectedInt8 = 1;
    sn_coap_parser_stub.expectedHeader->coap_status = COAP_STATUS_PARSER_ERROR_IN_HEADER;
    CHECK( NULL == sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL) );

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_header_check_stub.expectedInt8 = 1;
    sn_coap_parser_stub.expectedHeader->msg_code = sn_coap_msg_code_e(COAP_MSG_CODE_RESPONSE_PROXYING_NOT_SUPPORTED + 60);

    CHECK( NULL == sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL) );

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_header_check_stub.expectedInt8 = 0;

    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_EMPTY;

    CHECK( NULL == sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL) );

    //Test sn_coap_handle_blockwise_message, block1 != NULL -->
    sn_coap_protocol_set_duplicate_buffer_size(handle,1);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;

    sn_coap_options_list_s* list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;

    sn_coap_hdr_s *ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    uint8_t* payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 1;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( COAP_STATUS_PARSER_DUPLICATED_MSG == ret->coap_status );

    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 5;

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 6;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 4;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 4;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 7;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 5;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 8;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;

    retCounter = 6;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED == ret->coap_status );
    free(ret->payload_ptr);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    // block1 == 0x08 -->
    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 9;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 10;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 5;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 11;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 6;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    free(list);
//    free(sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 12;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x0F;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_POST;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;
    sn_coap_builder_stub.expectedUint16 = 1;

    retCounter = 7;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 13;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;
    sn_coap_builder_stub.expectedUint16 = 1;

    retCounter = 8;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    CHECK(COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING == ret->coap_status);
    free(payload);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 14;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 7;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    free(list);
//    free(sn_coap_parser_stub.expectedHeader);

    /* Size is more than we can handle */
    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 100;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->use_size1 = true;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->size1 = 0xFFFFFF01;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    payload = (uint8_t*)malloc(65535);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 65535;

    retCounter = 10;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );

    free(payload);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    // received_coap_msg_ptr->msg_code > COAP_MSG_CODE_REQUEST_DELETE -->

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 15;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x00;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    CHECK( COAP_STATUS_OK == ret->coap_status );
    free(payload);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 16;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;


    sn_nsdl_addr_s tmp_addr;
    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    sn_coap_hdr_s tmp_hdr;
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    uint8_t* dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 16;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 17;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block1 = -1;
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 17;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;

    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 18;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0xe808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;


    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
//    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
//    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
//    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    tmp_hdr.options_list_ptr = NULL;

    tmp_hdr.payload_len = UINT16_MAX;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);
    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 19;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0xeee808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
//    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
//    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
//    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 19;
    tmp_hdr.msg_code = COAP_MSG_CODE_REQUEST_GET;
    tmp_hdr.payload_len = 1;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 20;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0xeee808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
//    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
//    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
//    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 20;
    tmp_hdr.msg_code = COAP_MSG_CODE_REQUEST_GET;
    tmp_hdr.payload_len = 1;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 5;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    CHECK( COAP_STATUS_PARSER_BLOCKWISE_ACK == ret->coap_status );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    //<-- block1 == 0x08
    //<-- Test sn_coap_handle_blockwise_message, block1 != -1

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    //Test sn_coap_handle_blockwise_message, block2 != -1 -->
    sn_coap_protocol_set_duplicate_buffer_size(handle,1);

    // block2 length == 1,2,3 -->

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 16;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;
    sn_coap_parser_stub.expectedHeader->uri_path_ptr = NULL;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 16;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 17;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block1 = -1;
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 17;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;

    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 18;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xe808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;


    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->use_size2 = true;
    tmp_hdr.options_list_ptr->size2 = 0xFF01;
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 19;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xeee808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
//    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
//    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
//    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 19;
    tmp_hdr.msg_code = COAP_MSG_CODE_REQUEST_GET;
    tmp_hdr.payload_len = 1;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 200;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xeee808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 21;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 200;
    tmp_hdr.msg_code = COAP_MSG_CODE_REQUEST_GET;
    tmp_hdr.payload_len = 1;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);


    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 20;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xeee808;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_DELETE;
    payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);

    tmp_hdr.msg_id = 20;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    tmp_hdr.payload_ptr = NULL;
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 6;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    CHECK( COAP_STATUS_PARSER_BLOCKWISE_ACK == ret->coap_status );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    //<-- block2 length == 1,2,3

    // block2 == 0x08 -->

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_id = 41;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_id = 41;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 41;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 0;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_id = 42;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 42;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 5;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    free(list);
    free(sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_id = 43;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 43;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 6;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
//    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_id = 44;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 44;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    retCounter = 7;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xa8a8;
    sn_coap_parser_stub.expectedHeader->msg_id = 45;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 45;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_builder_stub.expectedInt16 = -1;
    retCounter = 8;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xc8c8c8;
    sn_coap_parser_stub.expectedHeader->msg_id = 46;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 46;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_builder_stub.expectedInt16 = 1;
    retCounter = 8;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xc8c8c8;
    sn_coap_parser_stub.expectedHeader->msg_id = 47;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 47;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_builder_stub.expectedInt16 = 1;
    retCounter = 9;
    sn_coap_protocol_set_retransmission_buffer(handle,0,0);
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 0xc8c8c8;
    sn_coap_parser_stub.expectedHeader->msg_id = 47;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 1;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(tmp_hdr.options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    tmp_hdr.options_list_ptr->block2 = 1;
    tmp_hdr.msg_id = 47;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_builder_stub.expectedInt16 = 1;
    retCounter = 9;
    sn_coap_protocol_set_retransmission_buffer(handle,2,1);
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    // <-- block2 == 0x08

    //<-- Test sn_coap_handle_blockwise_message, block2 != -1

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 41;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 42;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 1;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 43;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 2;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 44;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 3;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 45;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 4;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL == ret );
    free(payload);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, sn_coap_parser_stub.expectedHeader);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

    list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = -1;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block2 = 1;
    sn_coap_parser_stub.expectedHeader->msg_id = 46;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    payload = (uint8_t*)malloc(5);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 5;

    retCounter = 6;
    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( NULL != ret );
    free(payload);
    free(ret->payload_ptr);
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    sn_coap_protocol_destroy(handle);
    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 18;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);
    dst_packet_data_ptr[2]=0;
    dst_packet_data_ptr[3]=18;

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);
    tmp_addr.port = 0;

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 5;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( ret != NULL );
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    sn_coap_protocol_destroy(handle);

    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_RESET;
    sn_coap_parser_stub.expectedHeader->msg_id = 18;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);
    dst_packet_data_ptr[2]=0;
    dst_packet_data_ptr[3]=18;

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);
    tmp_addr.port = 0;

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 5;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    tmp_hdr.uri_path_ptr = (uint8_t*)malloc(7);
    snprintf((char *)tmp_hdr.uri_path_ptr, 7, "13/0/1");
    tmp_hdr.uri_path_len = 7;

    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.uri_path_ptr);
    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( ret != NULL );
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    sn_coap_protocol_destroy(handle);

    retCounter = 1;
    handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 18;

    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);
    dst_packet_data_ptr[2]=0;
    dst_packet_data_ptr[3]=185;

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);
    tmp_addr.port = 0;

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 5;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    tmp_hdr.uri_path_ptr = (uint8_t*)malloc(7);
    snprintf((char *)tmp_hdr.uri_path_ptr, 7, "13/0/1");
    tmp_hdr.uri_path_len = 7;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    ret = sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK( ret != NULL );
    sn_coap_parser_release_allocated_coap_msg_mem(handle, ret);

    free(tmp_hdr.uri_path_ptr);
    free(packet_data_ptr);
    free(addr->addr_ptr);
    free(addr);
    sn_coap_protocol_destroy(handle);
}

TEST(libCoap_protocol, sn_coap_protocol_exec)
{
    CHECK(-1 == sn_coap_protocol_exec(NULL, 0));

    retCounter = 1;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);

    sn_nsdl_addr_s tmp_addr;
    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    sn_coap_hdr_s tmp_hdr;
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    uint8_t* dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 5;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 7;

    sn_coap_options_list_s* list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    uint8_t* payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    uint8_t *packet_data_ptr = (uint8_t*)malloc(5);
    uint16_t packet_data_len = 5;

    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    addr->addr_ptr = (uint8_t*)malloc(5);

    retCounter = 5;
    sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    free(payload);
    free(packet_data_ptr);
    free(addr->addr_ptr);
    free(addr);

    CHECK(0 == sn_coap_protocol_exec(handle, 600));

    sn_coap_builder_stub.expectedInt16 = 0;
    retCounter = 0;
    sn_coap_protocol_destroy(handle);
}

TEST(libCoap_protocol, sn_coap_protocol_exec2)
{
    retCounter = 1;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, null_rx_cb);

    sn_nsdl_addr_s tmp_addr;
    memset(&tmp_addr, 0, sizeof(sn_nsdl_addr_s));
    sn_coap_hdr_s tmp_hdr;
    memset(&tmp_hdr, 0, sizeof(sn_coap_hdr_s));

    uint8_t* dst_packet_data_ptr = (uint8_t*)malloc(5);
    memset(dst_packet_data_ptr, '1', 5);

    tmp_addr.addr_ptr = (uint8_t*)malloc(5);
    memset(tmp_addr.addr_ptr, '1', 5);

    retCounter = 20;
    sn_coap_builder_stub.expectedInt16 = 5;
    tmp_hdr.payload_ptr = (uint8_t*)malloc(3);
    tmp_hdr.msg_id = 18;
    tmp_hdr.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
    tmp_hdr.payload_len = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE + 20;
    sn_coap_protocol_build(handle, &tmp_addr, dst_packet_data_ptr, &tmp_hdr, NULL);

    free(tmp_hdr.options_list_ptr);
    free(tmp_hdr.payload_ptr);
    free(tmp_addr.addr_ptr);
    free(dst_packet_data_ptr);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 7;

    sn_coap_options_list_s* list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 1;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_GET;
    uint8_t* payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = payload;
    sn_coap_parser_stub.expectedHeader->payload_len = 17;

    uint8_t *packet_data_ptr = (uint8_t*)malloc(5);
    uint16_t packet_data_len = 5;

    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));

    addr->addr_ptr = (uint8_t*)malloc(5);

    retCounter = 5;
    sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    free(payload);
    free(packet_data_ptr);
    free(addr->addr_ptr);
    free(addr);

    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));

    sn_coap_protocol_set_retransmission_parameters(handle,0, 5);
    CHECK(0 == sn_coap_protocol_exec(handle, 600));

    sn_coap_builder_stub.expectedInt16 = 0;
    retCounter = 0;
    sn_coap_protocol_destroy(handle);
}

TEST(libCoap_protocol, sn_coap_protocol_block_remove)
{
    sn_coap_protocol_block_remove(0,0,0,0);
    retCounter = 9;
    sn_nsdl_addr_s* addr = (sn_nsdl_addr_s*)malloc(sizeof(sn_nsdl_addr_s));
    memset(addr, 0, sizeof(sn_nsdl_addr_s));
    addr->addr_ptr = (uint8_t*)malloc(5);
    memset(addr->addr_ptr,'a',5);
    addr->addr_len = 5;
    struct coap_s * handle = sn_coap_protocol_init(myMalloc, myFree, null_tx_cb, NULL);
    uint8_t *packet_data_ptr = (uint8_t*)malloc(5);
    memset(packet_data_ptr,'x',5);
    uint16_t packet_data_len = 5;
    sn_coap_parser_stub.expectedHeader = NULL;
    sn_coap_parser_stub.expectedHeader = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(sn_coap_parser_stub.expectedHeader, 0, sizeof(sn_coap_hdr_s));
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    sn_coap_parser_stub.expectedHeader->msg_id = 13;

    sn_coap_options_list_s* list = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    memset(list, 0, sizeof(sn_coap_options_list_s));
    sn_coap_parser_stub.expectedHeader->options_list_ptr = list;
    sn_coap_parser_stub.expectedHeader->options_list_ptr->block1 = 0x08;
    sn_coap_parser_stub.expectedHeader->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    sn_coap_parser_stub.expectedHeader->msg_code = COAP_MSG_CODE_REQUEST_PUT;
    uint8_t* payload = (uint8_t*)malloc(17);
    sn_coap_parser_stub.expectedHeader->payload_ptr = packet_data_ptr;
    sn_coap_parser_stub.expectedHeader->payload_len = packet_data_len;
    sn_coap_builder_stub.expectedUint16 = 1;

    // Success
    retCounter = 9;
    sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 1);
    sn_coap_protocol_block_remove(handle,addr,packet_data_len,packet_data_ptr);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 0);

    // Ports does not match
    retCounter = 18;
    sn_coap_parser_stub.expectedHeader->msg_id = 14;
    addr->port = 5600;
    sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 1);
    addr->port = 5601;
    sn_coap_protocol_block_remove(handle,addr,packet_data_len,packet_data_ptr);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 1);

    // Addresses does not match
    retCounter = 18;
    sn_coap_parser_stub.expectedHeader->msg_id = 15;
    sn_coap_protocol_parse(handle, addr, packet_data_len, packet_data_ptr, NULL);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 2);
    addr->addr_ptr[0] = 'x';
    sn_coap_protocol_block_remove(handle,addr,packet_data_len,packet_data_ptr);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 2);

    // Payload length does not match
    addr->addr_ptr[0] = 'a';
    sn_coap_protocol_block_remove(handle,addr,packet_data_len+1,packet_data_ptr);
    CHECK(ns_list_count(&handle->linked_list_blockwise_received_payloads) == 2);

    free(sn_coap_parser_stub.expectedHeader->options_list_ptr);
    free(sn_coap_parser_stub.expectedHeader);
    free(addr->addr_ptr);
    free(addr);
    free(payload);
    free(packet_data_ptr);
    sn_coap_protocol_destroy(handle);
}

