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
#include <stdint.h>
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol_internal.h"

#include "sn_coap_header_check_stub.h"
#include "sn_coap_parser_stub.h"

sn_coap_hdr_s coap_header;
sn_coap_options_list_s option_list;
uint8_t buffer[256];
uint8_t temp[10];

uint8_t retCounter = 0;

static void *own_alloc(uint16_t size)
{
    if( retCounter > 0 ){
        retCounter--;
        return malloc(size);
    }
    return NULL;
}

static void own_free(void *ptr)
{
    free(ptr);
}

TEST_GROUP(libCoap_builder)
{
    void setup() {
        sn_coap_header_check_stub.expectedInt8 = 0;
        retCounter = 0;
        memset(&coap_header, 0, sizeof(sn_coap_hdr_s));
        memset(&option_list, 0, sizeof(sn_coap_options_list_s));

        coap_header.options_list_ptr = &option_list;
        coap_header.msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
        coap_header.msg_code = COAP_MSG_CODE_RESPONSE_CREATED;
        coap_header.msg_id = 12;
    }

    void teardown() {

    }
};

TEST(libCoap_builder, build_confirmable_response)
{
    struct coap_s handle;
    sn_coap_hdr_s *response = NULL;

    handle.sn_coap_protocol_malloc = &own_alloc;
    handle.sn_coap_protocol_free = &own_free;

    coap_header.msg_type = COAP_MSG_TYPE_RESET;
    coap_header.msg_code = COAP_MSG_CODE_REQUEST_GET;
    coap_header.msg_id = 12;

    //NULL pointer
    CHECK(sn_coap_build_response(NULL, NULL, 0) == NULL);
    CHECK(sn_coap_build_response(&handle, NULL, 0) == NULL);
    CHECK(sn_coap_build_response(NULL, &coap_header, 0) == NULL);
    CHECK(sn_coap_build_response(&handle, &coap_header, COAP_MSG_CODE_RESPONSE_CONTENT) == NULL);

    retCounter = 1;
    CHECK(sn_coap_build_response(&handle, &coap_header, COAP_MSG_CODE_RESPONSE_CONTENT) == NULL);

    coap_header.msg_type = COAP_MSG_TYPE_CONFIRMABLE;

    retCounter = 2;
    response = sn_coap_build_response(&handle, &coap_header, COAP_MSG_CODE_RESPONSE_CONTENT);

    CHECK(response != NULL);
    CHECK(response->msg_type == COAP_MSG_TYPE_ACKNOWLEDGEMENT);
    CHECK(response->msg_id == 12);
    CHECK(response->msg_code == COAP_MSG_CODE_RESPONSE_CONTENT);

    own_free(response);
}

TEST(libCoap_builder, build_non_confirmable_response)
{
    struct coap_s handle;
    sn_coap_hdr_s *response = NULL;
    uint8_t token_val = 0x99;

    handle.sn_coap_protocol_malloc = &own_alloc;
    handle.sn_coap_protocol_free = &own_free;

    coap_header.msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    coap_header.msg_code = COAP_MSG_CODE_REQUEST_GET;
    coap_header.msg_id = 12;
    coap_header.token_len = 1;
    coap_header.token_ptr = &token_val;

    retCounter = 1;
    CHECK( NULL == sn_coap_build_response(&handle, &coap_header, COAP_MSG_CODE_RESPONSE_CONTENT));

    retCounter = 2;
    response = sn_coap_build_response(&handle, &coap_header, COAP_MSG_CODE_RESPONSE_CONTENT);

    CHECK(response != NULL);
    CHECK(response->msg_type == COAP_MSG_TYPE_NON_CONFIRMABLE);
    CHECK(response->msg_code == COAP_MSG_CODE_RESPONSE_CONTENT);
    CHECK(response->token_ptr != NULL);
    CHECK(memcmp(response->token_ptr, coap_header.token_ptr, coap_header.token_len) == 0);
    CHECK(response->token_len == coap_header.token_len);

    own_free(response->token_ptr);
    own_free(response);
}


TEST(libCoap_builder, build_message_negative_cases)
{
    // Null pointers as a parameter
    CHECK(sn_coap_builder(NULL, NULL) == -2);
    CHECK(sn_coap_builder(NULL, &coap_header) == -2);
    CHECK(sn_coap_builder(buffer, NULL) == -2);

    // Invalid option length
    coap_header.token_ptr = temp;
    CHECK(sn_coap_builder(buffer, &coap_header) == -1);
}

TEST(libCoap_builder, build_message_ok_cases)
{
    CHECK(sn_coap_builder(buffer, &coap_header) == 4);
}

TEST(libCoap_builder, build_message_options_token)
{
    coap_header.token_ptr = temp;
    coap_header.token_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 6);
}

TEST(libCoap_builder, build_message_options_uri_path)
{
    coap_header.uri_path_ptr = temp;
    coap_header.uri_path_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_content_type)
{
    coap_header.content_type_ptr = temp;
    coap_header.content_type_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_max_age)
{
    coap_header.options_list_ptr->max_age_ptr = temp;
    coap_header.options_list_ptr->max_age_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}

TEST(libCoap_builder, build_message_options_proxy_uri)
{
    coap_header.options_list_ptr->proxy_uri_ptr = temp;
    coap_header.options_list_ptr->proxy_uri_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}

TEST(libCoap_builder, build_message_options_etag)
{
    coap_header.options_list_ptr->etag_ptr = temp;
    coap_header.options_list_ptr->etag_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_uri_host)
{
    coap_header.options_list_ptr->uri_host_ptr = temp;
    coap_header.options_list_ptr->uri_host_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_location_path)
{
    coap_header.options_list_ptr->location_path_ptr = temp;
    coap_header.options_list_ptr->location_path_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_uri_port)
{
    coap_header.options_list_ptr->uri_port_ptr = temp;
    coap_header.options_list_ptr->uri_port_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}

TEST(libCoap_builder, build_message_options_location_query)
{
    coap_header.options_list_ptr->location_query_ptr = temp;
    coap_header.options_list_ptr->location_query_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}

TEST(libCoap_builder, build_message_options_observe)
{
    coap_header.options_list_ptr->observe_ptr = temp;
    coap_header.options_list_ptr->observe_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 7);
}


TEST(libCoap_builder, build_message_options_accept)
{
    coap_header.options_list_ptr->accept_ptr = temp;
    coap_header.options_list_ptr->accept_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}


TEST(libCoap_builder, build_message_options_uri_query)
{
    coap_header.options_list_ptr->uri_query_ptr = temp;
    coap_header.options_list_ptr->uri_query_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}


TEST(libCoap_builder, build_message_options_block1)
{
    coap_header.options_list_ptr->block1_ptr = temp;
    coap_header.options_list_ptr->block1_len = 2;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);
}

TEST(libCoap_builder, build_message_options_block2)
{
    coap_header.options_list_ptr->block2_ptr = temp;
    coap_header.options_list_ptr->block2_len = 2;

    sn_coap_header_check_stub.expectedInt8 = 44;
    CHECK(sn_coap_builder(buffer, &coap_header) == -1);

    sn_coap_header_check_stub.expectedInt8 = 0;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);

    coap_header.options_list_ptr = NULL; //return from sn_coap_builder_options_build immediately
    sn_coap_header_check_stub.expectedInt8 = 0;
    CHECK( 4 == sn_coap_builder(buffer, &coap_header) );
}

TEST(libCoap_builder, sn_coap_builder_calc_needed_packet_data_size)
{
    CHECK(sn_coap_builder_calc_needed_packet_data_size(NULL) == 0);

    sn_coap_hdr_s header;
    memset(&header, 0, sizeof(sn_coap_hdr_s));
    header.msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    header.token_ptr = (uint8_t*)malloc(10);
    header.token_len = 10;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);
    free(header.token_ptr);
    header.token_ptr = (uint8_t*)malloc(6);
    header.token_len = 6;

    //Test variations of sn_coap_builder_options_calc_option_size here -->
    header.uri_path_ptr = (uint8_t*)malloc(290);
    memset(header.uri_path_ptr, '1', 290);
    header.uri_path_len = 290;
    header.uri_path_ptr[5] = '/';
    header.uri_path_ptr[285] = '/';

    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.uri_path_ptr[285] = '1';
    header.uri_path_ptr[170] = '/';

    header.content_type_ptr = (uint8_t*)malloc(6);
    header.content_type_len = 6;

    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.content_type_len = 2;
    sn_coap_options_list_s opt_list;
    memset(&opt_list, 0, sizeof(sn_coap_options_list_s));
    header.options_list_ptr = &opt_list;
    header.options_list_ptr->accept_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->accept_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->accept_len = 2;
    header.options_list_ptr->max_age_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->max_age_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    //proxy uri tests (4)
    header.options_list_ptr->max_age_len = 2;
    header.options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(270);
    header.options_list_ptr->proxy_uri_len = 1800;

    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);
    header.options_list_ptr->proxy_uri_len = 6;
    header.options_list_ptr->etag_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->etag_len = 0;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->proxy_uri_len = 14;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->proxy_uri_len = 281;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->etag_len = 4;
    header.options_list_ptr->uri_host_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->uri_host_len = 0;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->uri_host_len = 4;
    header.options_list_ptr->location_path_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->location_path_len = 270;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->uri_host_len = 44;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->location_path_len = 27;
    header.options_list_ptr->uri_port_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->uri_port_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->uri_port_len = 2;
    header.options_list_ptr->location_query_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->location_query_len = 277;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->location_query_len = 27;
    header.options_list_ptr->observe_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->observe_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->observe_len = 2;
    header.options_list_ptr->uri_query_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->uri_query_len = 0;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->observe = 1;

    header.options_list_ptr->uri_query_len = 4;
    header.options_list_ptr->block2_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->block2_len = 0;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->block2_len = 2;
    header.options_list_ptr->block1_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->block1_len = 0;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->block1_len = 2;
    header.payload_len = 1;
    CHECK(431 == sn_coap_builder_calc_needed_packet_data_size(&header));


    header.options_list_ptr->size1_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->size1_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    header.options_list_ptr->size2_ptr = (uint8_t*)malloc(6);
    header.options_list_ptr->size1_len = 2;
    header.options_list_ptr->size2_len = 6;
    CHECK(sn_coap_builder_calc_needed_packet_data_size(&header) == 0);

    free(header.options_list_ptr->observe_ptr);
    header.options_list_ptr->observe_ptr = NULL;
    header.options_list_ptr->observe_len = 0;
    header.options_list_ptr->block1_len = 2;
    header.options_list_ptr->size1_len = 2;
    header.options_list_ptr->size2_len = 2;
    header.payload_len = 1;
    CHECK(729 == sn_coap_builder_calc_needed_packet_data_size(&header));

    // <--

    //free(header.options_list_ptr->observe_ptr); Called earlier!
    free(header.options_list_ptr->location_query_ptr);
    free(header.options_list_ptr->uri_port_ptr);
    free(header.options_list_ptr->location_path_ptr);
    free(header.options_list_ptr->uri_host_ptr);
    free(header.options_list_ptr->etag_ptr);
    free(header.options_list_ptr->proxy_uri_ptr);
    free(header.options_list_ptr->max_age_ptr);
    free(header.options_list_ptr->accept_ptr);
    free(header.options_list_ptr->size1_ptr);
    free(header.options_list_ptr->block1_ptr);
    free(header.options_list_ptr->block2_ptr);
    header.options_list_ptr->location_query_ptr = NULL;
    header.options_list_ptr->uri_port_ptr = NULL;
    header.options_list_ptr->location_path_ptr = NULL;
    header.options_list_ptr->uri_host_ptr = NULL;
    header.options_list_ptr->etag_ptr = NULL;
    header.options_list_ptr->proxy_uri_ptr = NULL;
    header.options_list_ptr->max_age_ptr = NULL;
    header.options_list_ptr->accept_ptr = NULL;
    header.options_list_ptr->size1_ptr = NULL;
    header.options_list_ptr->block1_ptr = NULL;
    header.options_list_ptr->block2_ptr = NULL;

    CHECK(318 == sn_coap_builder_calc_needed_packet_data_size(&header));
    free(header.options_list_ptr->size2_ptr);
    free(header.options_list_ptr->uri_query_ptr);

    //Test sn_coap_builder_options_calculate_jump_need "else" case
    header.options_list_ptr = NULL;
    uint16_t val = sn_coap_builder_calc_needed_packet_data_size(&header);
    CHECK( 308 == val );

    free(header.content_type_ptr);
    free(header.uri_path_ptr);
    free(header.token_ptr);
}

TEST(libCoap_builder, sn_coap_builder_options_build_add_one_option)
{
    coap_header.options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(280);
    memset(coap_header.options_list_ptr->proxy_uri_ptr, '1', 280);
    coap_header.options_list_ptr->proxy_uri_len = 2;
    sn_coap_header_check_stub.expectedInt8 = 0;
    CHECK(sn_coap_builder(buffer, &coap_header) == 8);

    coap_header.options_list_ptr->proxy_uri_len = 27;
    sn_coap_header_check_stub.expectedInt8 = 0;
    CHECK(34 == sn_coap_builder(buffer, &coap_header));

    coap_header.options_list_ptr->proxy_uri_len = 277;
    sn_coap_header_check_stub.expectedInt8 = 0;
    CHECK(285 == sn_coap_builder(buffer, &coap_header));

    free(coap_header.options_list_ptr->proxy_uri_ptr);
    coap_header.options_list_ptr->proxy_uri_ptr = NULL;
}

TEST(libCoap_builder, sn_coap_builder_options_build_add_zero_length_option)
{
    coap_header.options_list_ptr->proxy_uri_ptr = (uint8_t*)malloc(280);
    memset(coap_header.options_list_ptr->proxy_uri_ptr, '1', 280);
    coap_header.options_list_ptr->proxy_uri_len = 2;
    sn_coap_header_check_stub.expectedInt8 = 0;
    coap_header.options_list_ptr->observe = 1;
    int16_t val = sn_coap_builder(buffer, &coap_header);
    CHECK(val == 9);

    free(coap_header.options_list_ptr->proxy_uri_ptr);
}

TEST(libCoap_builder, sn_coap_builder_options_get_option_part_position)
{
    sn_coap_hdr_s header;
    memset(&header, 0, sizeof(sn_coap_hdr_s));
    sn_coap_options_list_s opt_list;
    memset(&opt_list, 0, sizeof(sn_coap_options_list_s));
    header.options_list_ptr = &opt_list;
    header.options_list_ptr->accept_ptr = (uint8_t*)malloc(20);
    memset(header.options_list_ptr->accept_ptr, '&', 20);
    header.options_list_ptr->accept_len = 20;
    uint16_t val = sn_coap_builder(buffer, &header);
    CHECK(val == 24);

    header.options_list_ptr->accept_ptr[0] = 'a';
    header.options_list_ptr->accept_ptr[1] = 'n';
    header.options_list_ptr->accept_ptr[19] = 'n';
    val = sn_coap_builder(buffer, &header);
    CHECK(val == 42);

    free(header.options_list_ptr->accept_ptr);
    header.options_list_ptr->accept_ptr = NULL;
}

TEST(libCoap_builder, sn_coap_builder_payload_build)
{
    sn_coap_hdr_s header;
    memset(&header, 0, sizeof(sn_coap_hdr_s));
    header.payload_ptr = (uint8_t*)malloc(5);
    header.payload_len = 5;
    sn_coap_options_list_s opt_list;
    memset(&opt_list, 0, sizeof(sn_coap_options_list_s));
    header.options_list_ptr = &opt_list;
    header.options_list_ptr->accept_ptr = (uint8_t*)malloc(20);
    memset(header.options_list_ptr->accept_ptr, '&', 20);
    header.options_list_ptr->accept_len = 20;
    uint16_t val = sn_coap_builder(buffer, &header);
    CHECK(val == 30);

    free(header.payload_ptr);
    free(header.options_list_ptr->accept_ptr);
    header.options_list_ptr->accept_ptr = NULL;
}
