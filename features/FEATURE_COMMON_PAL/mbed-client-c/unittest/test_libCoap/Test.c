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
/**
 * \file \test_libCoap\Test.c
 *
 * \brief Unit tests for libCoap
 */
#include "ns_types.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_coap_header_internal.h"

#include "unity.h"
#include "string.h"
#include "stdlib.h"

static uint8_t *message_ptr = 0;
static uint16_t message_len = 0;
static sn_coap_hdr_s coap_header;
sn_nsdl_addr_s coap_address;
static uint8_t address[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

/* CoAP parameters to be used */
static uint8_t option_short[3] = {0x61, 0x62, 0x63};
static uint32_t option_short_value = 0x616263;
static uint8_t option_path[5] = {0x62, 0x63, 0x2f, 0x61, 0x62};
static uint8_t option_30[30];
static uint8_t option_300[300];
static uint8_t option_600[600];
static uint8_t option_800[800];

/* Resultat sju rätt är: */
static uint8_t coap_message_no_options[4]       = {0x60, 0x44, 0x12, 0x34};
static uint8_t coap_message_token_payload[11]   = {0x63, 0x44, 0x12, 0x34, 0x61, 0x62, 0x63, 0xff, 0x61, 0x62, 0x63};
static uint8_t coap_message_wrong_version[4]    = {0xe0, 0x44, 0x12, 0x34};
static uint8_t coap_message_malformed[4]        = {0x60, 0xae, 0x43, 0x11};
static uint8_t coap_message_option_short[9]     = {0x60, 0x44, 0x12, 0x34, 0xd3, 0x16, 0x61, 0x62, 0x63};               // option number 35
static uint8_t coap_message_option_30[7]        = {0x60, 0x44, 0x12, 0x34, 0xdd, 0x16, 0x11};                           // option number 35, length 30
static uint8_t coap_message_option_300[8]       = {0x60, 0x44, 0x12, 0x34, 0xde, 0x16, 0x00, 0x1f};                     // option number 35, length 300
static uint8_t coap_message_option_600[8]       = {0x60, 0x44, 0x12, 0x34, 0xde, 0x16, 0x01, 0x4b};                     // option number 35, length 600
static uint8_t coap_message_option_800[8]       = {0x60, 0x44, 0x12, 0x34, 0xde, 0x16, 0x02, 0x13};                     // option number 35, length 800
/* Options = Token - 3 bytes, max age - 14 - 3 bytes, Uri path - 11 - 2 x 2 bytes, Uri host - 3 - 3 bytes */
static uint8_t coap_message_multiple_options[21] = {0x63, 0x44, 0x12, 0x34, 0x61, 0x62, 0x63, 0x33, 0x61, 0x62, 0x63, 0x82, 0x62, 0x63, 0x02, 0x61, 0x62, 0x33, 0x61, 0x62, 0x63};

static uint8_t coap_message_empty_con[4]        = {0x40, 0x00, 0x12, 0x34};
static uint8_t coap_message_wrong_code_1[4]     = {0x40, 0x24, 0x12, 0x34};
static uint8_t coap_message_wrong_code_6[4]     = {0x40, 0xc4, 0x12, 0x34};
static uint8_t coap_message_wrong_code_7[4]     = {0x40, 0xe4, 0x12, 0x34};


void *own_alloc(uint16_t size);
void own_free(void *ptr);

struct coap_s *handle;

uint8_t tx_function(uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr, void *param);
int8_t rx_callback(sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *address_ptr, void *param);

/* non-test function declarations */
void fill_with_random(uint8_t *ptr, uint16_t len);

/* Unity test code starts */
void setUp(void)
{
    //This is run before EACH TEST

}

void tearDown(void)
{

}

void test_libcoap_init(void)
{
    handle = sn_coap_protocol_init(&own_alloc, &own_free, &tx_function, &rx_callback);

    TEST_ASSERT_NOT_NULL(handle);
}


/**
 * \fn test_libcoap_builder_no_options(void)
 *
 * \brief Build CoAP message - ACK - Changed
 *
 */
void test_libcoap_builder_no_options(void)
{
    memset(&coap_header, 0, sizeof(sn_coap_hdr_s));
    coap_header.msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    coap_header.msg_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    coap_header.msg_id = 0x1234;

    coap_header.options_list_ptr = malloc(sizeof(sn_coap_options_list_s));
    memset(coap_header.options_list_ptr, 0, sizeof(sn_coap_options_list_s));

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_no_options), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_no_options, message_ptr, sizeof(coap_message_no_options));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_token_and_payload(void)
 *
 * \brief Build CoAP message - ACK - changed - token - payload
 *
 */
void test_libcoap_builder_token_and_payload(void)
{
    memset(&coap_header, 0, sizeof(sn_coap_hdr_s));
    coap_header.msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
    coap_header.msg_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    coap_header.msg_id = 0x1234;
    coap_header.token_len = sizeof(option_short);
    coap_header.token_ptr = option_short;
    coap_header.payload_len = sizeof(option_short);
    coap_header.payload_ptr = option_short;

    coap_header.options_list_ptr = malloc(sizeof(sn_coap_options_list_s));
    memset(coap_header.options_list_ptr, 0, sizeof(sn_coap_options_list_s));

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_token_payload), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_token_payload, message_ptr, sizeof(coap_message_token_payload));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_short_option(void)
 *
 * \brief Build CoAP message - ACK - changed - short proxy URI option
 *
 */
void test_libcoap_builder_short_option(void)
{
    coap_header.token_len = 0;
    coap_header.token_ptr = 0;
    coap_header.payload_len = 0;
    coap_header.payload_ptr = 0;

    coap_header.options_list_ptr->proxy_uri_len = sizeof(option_short);
    coap_header.options_list_ptr->proxy_uri_ptr = option_short;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_option_short), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_option_short, message_ptr, sizeof(coap_message_option_short));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_option_30(void)
 *
 * \brief Build CoAP message - ACK - changed - proxy URI option, length 30 bytes
 *
 */
void test_libcoap_builder_option_30(void)
{
    fill_with_random(option_30, sizeof(option_30));
    coap_header.options_list_ptr->proxy_uri_len = sizeof(option_30);
    coap_header.options_list_ptr->proxy_uri_ptr = option_30;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL((sizeof(coap_message_option_30) + sizeof(option_30)), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_option_30, message_ptr, sizeof(coap_message_option_30));

    free(message_ptr);
    message_ptr = 0;

}

/**
 * \fn test_libcoap_builder_option_300(void)
 *
 * \brief Build CoAP message - ACK - changed - proxy URI option, length 300 bytes
 *
 */
void test_libcoap_builder_option_300(void)
{
    fill_with_random(option_300, sizeof(option_300));
    coap_header.options_list_ptr->proxy_uri_len = sizeof(option_300);
    coap_header.options_list_ptr->proxy_uri_ptr = option_300;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_option_300) + sizeof(option_300), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_option_300, message_ptr, sizeof(coap_message_option_300));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_option_600(void)
 *
 * \brief Build CoAP message - ACK - changed - proxy URI option, length 600 bytes
 *
 */

void test_libcoap_builder_option_600(void)
{
    fill_with_random(option_600, sizeof(option_600));
    coap_header.options_list_ptr->proxy_uri_len = sizeof(option_600);
    coap_header.options_list_ptr->proxy_uri_ptr = option_600;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_option_600) + sizeof(option_600), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_option_600, message_ptr, sizeof(coap_message_option_600));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_option_800(void)
 *
 * \brief Build CoAP message - ACK - changed - proxy URI option, length 800 bytes
 *
 */
void test_libcoap_builder_option_800(void)
{
    fill_with_random(option_800, sizeof(option_800));
    coap_header.options_list_ptr->proxy_uri_len = sizeof(option_800);
    coap_header.options_list_ptr->proxy_uri_ptr = option_800;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_option_800) + sizeof(option_800), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_option_800, message_ptr, sizeof(coap_message_option_800));

    free(message_ptr);
    message_ptr = 0;
}

/**
 * \fn test_libcoap_builder_message_with_multiple_options(void)
 *
 * \brief Build CoAP message - ACK - changed - URI host - URI path - max age - token
 *
 */
void test_libcoap_builder_message_with_multiple_options(void)
{
    coap_header.options_list_ptr->proxy_uri_len = 0;
    coap_header.options_list_ptr->proxy_uri_ptr = 0;

    coap_header.options_list_ptr->uri_host_len = sizeof(option_short);
    coap_header.options_list_ptr->uri_host_ptr = option_short;

    coap_header.uri_path_len = sizeof(option_path);
    coap_header.uri_path_ptr = option_path;

    coap_header.options_list_ptr->max_age = option_short_value;

    coap_header.token_len = sizeof(option_short);
    coap_header.token_ptr = option_short;

    message_len = sn_coap_builder_calc_needed_packet_data_size(&coap_header);
    message_ptr = malloc(message_len);
    sn_coap_builder(message_ptr, &coap_header);

    TEST_ASSERT_EQUAL(sizeof(coap_message_multiple_options), message_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(coap_message_multiple_options, message_ptr, sizeof(coap_message_multiple_options));

    free(message_ptr);
    message_ptr = 0;
}

/*******************************************************************************/
/***                        CoAP PARSER TEST                                 ***/
/*******************************************************************************/

/**
 * \fn test_libcoap_parser_parse_message_without_options(void)
 *
 * \brief call coap protocol parser with message without options (ACK - changed)
 *
 */
void test_libcoap_parser_parse_message_without_options(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_no_options), coap_message_no_options, NULL);

    TEST_ASSERT_NOT_NULL(coap_header_ptr);

    TEST_ASSERT_EQUAL(COAP_MSG_TYPE_ACKNOWLEDGEMENT, coap_header_ptr->msg_type);
    TEST_ASSERT_EQUAL(COAP_MSG_CODE_RESPONSE_CHANGED, coap_header_ptr->msg_code);
    TEST_ASSERT_EQUAL(0x1234, coap_header_ptr->msg_id);

    sn_coap_parser_release_allocated_coap_msg_mem(handle, coap_header_ptr);
}

/**
 * \fn test_libcoap_parser_parse_message_with_payload_and_token(void)
 *
 * \brief call coap protocol parser with message with token option and small payload (ACK - changed)
 *
 */
void test_libcoap_parser_parse_message_with_payload_and_token(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_token_payload), coap_message_token_payload, NULL);

    TEST_ASSERT_NOT_NULL(coap_header_ptr);

    TEST_ASSERT_EQUAL(COAP_MSG_TYPE_ACKNOWLEDGEMENT, coap_header_ptr->msg_type);
    TEST_ASSERT_EQUAL(COAP_MSG_CODE_RESPONSE_CHANGED, coap_header_ptr->msg_code);
    TEST_ASSERT_EQUAL(0x1234, coap_header_ptr->msg_id);

    TEST_ASSERT_EQUAL(3, coap_header_ptr->token_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_short, coap_header_ptr->token_ptr, sizeof(option_short));

    TEST_ASSERT_EQUAL(3, coap_header_ptr->payload_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_short, coap_header_ptr->payload_ptr, sizeof(option_short));

    sn_coap_parser_release_allocated_coap_msg_mem(handle, coap_header_ptr);
}

/**
 * \fn test_libcoap_parser_parse_message_with_small_option(void)
 *
 * \brief call coap protocol parser with message with small option (ACK - changed - URI query)
 *
 */
void test_libcoap_parser_parse_message_with_small_option(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_option_short), coap_message_option_short, NULL);

    TEST_ASSERT_NOT_NULL(coap_header_ptr);
    TEST_ASSERT_NOT_NULL(coap_header_ptr->options_list_ptr);

    TEST_ASSERT_EQUAL(COAP_MSG_TYPE_ACKNOWLEDGEMENT, coap_header_ptr->msg_type);
    TEST_ASSERT_EQUAL(COAP_MSG_CODE_RESPONSE_CHANGED, coap_header_ptr->msg_code);
    TEST_ASSERT_EQUAL(0x1234, coap_header_ptr->msg_id);

    TEST_ASSERT_EQUAL(sizeof(option_short), coap_header_ptr->options_list_ptr->proxy_uri_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_short, coap_header_ptr->options_list_ptr->proxy_uri_ptr, sizeof(option_short));

    sn_coap_parser_release_allocated_coap_msg_mem(handle, coap_header_ptr);
}


/**
 * \fn test_libcoap_parser_parse_message_with_multiple_options(void)
 *
 * \brief call coap protocol parser with message with multiple options (ACK - changed - token - max age - URI path - URI host)
 *
 */
void test_libcoap_parser_parse_message_with_multiple_options(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_multiple_options), coap_message_multiple_options, NULL);

    TEST_ASSERT_NOT_NULL(coap_header_ptr);
    TEST_ASSERT_NOT_NULL(coap_header_ptr->options_list_ptr);

    TEST_ASSERT_EQUAL(COAP_MSG_TYPE_ACKNOWLEDGEMENT, coap_header_ptr->msg_type);
    TEST_ASSERT_EQUAL(COAP_MSG_CODE_RESPONSE_CHANGED, coap_header_ptr->msg_code);
    TEST_ASSERT_EQUAL(0x1234, coap_header_ptr->msg_id);


    TEST_ASSERT_EQUAL(sizeof(option_short), coap_header_ptr->options_list_ptr->uri_host_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_short, coap_header_ptr->options_list_ptr->uri_host_ptr, sizeof(option_short));

    TEST_ASSERT_EQUAL(sizeof(option_path), coap_header_ptr->uri_path_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_path, coap_header_ptr->uri_path_ptr, sizeof(option_path));

    TEST_ASSERT_EQUAL(option_short_value, coap_header_ptr->options_list_ptr->max_age);

    TEST_ASSERT_EQUAL(sizeof(option_short), coap_header_ptr->token_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(option_short, coap_header_ptr->token_ptr, sizeof(option_short));


    sn_coap_parser_release_allocated_coap_msg_mem(handle, coap_header_ptr);
}

/*******************************************************************************/
/***                        NEGATIVE TEST CASES                              ***/
/*******************************************************************************/

/**
 * \fn test_libcoap_negative_build_with_null_pointer(void)
 *
 * \brief   - Call sn_coap_builder_calc_needed_packet_data_size with null pointer parameter
 *          - Call sn_coap_builder with null pointer parameter
 *
 */
void test_libcoap_negative_build_with_null_pointer(void)
{
    int16_t ret_val_1 = 0;
    int16_t ret_val_2 = 0;

    message_len = sn_coap_builder_calc_needed_packet_data_size(NULL);
    ret_val_1 = sn_coap_builder(message_ptr, NULL);
    ret_val_2 = sn_coap_builder(NULL, &coap_header);

    TEST_ASSERT_EQUAL(0, message_len);
    TEST_ASSERT_EQUAL(-2, ret_val_1);
    TEST_ASSERT_EQUAL(-2, ret_val_2);
}

/**
 * \fn test_libcoap_negative_parse_with_null_pointer(void)
 *
 * \brief   - Call sn_coap_protocol_parse with null pointer parameter
 *
 */
void test_libcoap_negative_parse_with_null_pointer(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, NULL, sizeof(coap_message_no_options), coap_message_no_options, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

    coap_header_ptr = sn_coap_protocol_parse(NULL, &coap_address, sizeof(coap_message_no_options), coap_message_no_options, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

    coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_no_options), NULL, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);
}

/**
 * \fn test_libcoap_negative_parse_with_coap_packet_len_null(void)
 *
 * \brief   - Call sn_coap_protocol_parse with parameter length = 0
 *
 */
void test_libcoap_negative_parse_with_coap_packet_len_null(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, 0, coap_message_no_options, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);
}

/**
 * \fn test_libcoap_negative_parse_with_wrong_coap_packet_len(void)
 *
 * \brief   - Call sn_coap_protocol_parse with wrong packet length
 *
 */
void test_libcoap_negative_parse_with_wrong_coap_packet_len(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, (sizeof(coap_message_multiple_options) - 1), coap_message_multiple_options, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);
}

/**
 * \fn test_libcoap_negative_parse_with_wrong_coap_version(void)
 *
 * \brief   - Call sn_coap_protocol_parse with wrong CoAP version
 *
 */
void test_libcoap_negative_parse_with_wrong_coap_version(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_wrong_version), coap_message_wrong_version, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);
}

/**
 * \fn test_libcoap_negative_parse_malformed_coap(void)
 *
 * \brief   - Call sn_coap_protocol_parse with malformed CoAP message
 *
 */
void test_libcoap_negative_parse_malformed_coap(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_malformed), coap_message_malformed, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);
}

/**
 * \fn test_libcoap_negative_parse_empty_con(void)
 *
 * \brief   - Call sn_coap_protocol_parse with "CoAP ping" message. Should return RST
 *
 */
void test_libcoap_negative_parse_empty_con(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_empty_con), coap_message_empty_con, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

}

/**
 * \fn test_libcoap_negative_parse_wrong_code_1(void)
 *
 * \brief   - Call sn_coap_protocol_parse with wrong message code. Should return RST
 *
 */
void test_libcoap_negative_parse_wrong_code_1(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_wrong_code_1), coap_message_wrong_code_1, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

}

/**
 * \fn test_libcoap_negative_parse_wrong_code_6(void)
 *
 * \brief   - Call sn_coap_protocol_parse with wrong message code. Should return RST
 *
 */
void test_libcoap_negative_parse_wrong_code_6(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_wrong_code_6), coap_message_wrong_code_6, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

}

/**
 * \fn test_libcoap_negative_parse_wrong_code_7(void)
 *
 * \brief   - Call sn_coap_protocol_parse with wrong message code. Should return RST
 *
 */
void test_libcoap_negative_parse_wrong_code_7(void)
{
    coap_address.addr_ptr = address;

    sn_coap_hdr_s *coap_header_ptr = sn_coap_protocol_parse(handle, &coap_address, sizeof(coap_message_wrong_code_7), coap_message_wrong_code_7, NULL);
    TEST_ASSERT_NULL(coap_header_ptr);

}


/*******************************/
/***   non-test functions    ***/
/*******************************/

void fill_with_random(uint8_t *ptr, uint16_t len)
{
    while (len--) {
        *(ptr + len) = 'a';
    }
}

void *own_alloc(uint16_t size)
{
    return malloc(size);
}

void own_free(void *ptr)
{
    free(ptr);
}

uint8_t tx_function(uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr, void *param)
{
    return 0;
}

int8_t rx_callback(sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *address_ptr, void *param)
{
    return 0;
}









