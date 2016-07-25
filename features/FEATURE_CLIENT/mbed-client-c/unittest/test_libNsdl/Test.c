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
 * \file \test_libNsdl\Test.c
 *
 * \brief   Unit tests for libNsdl
 *          NOTE!! Needs libCoap.a from ../../nsdl-c/libCoap/x86_gcc
 *
 *  */

#include "ns_types.h"

#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol.h"
#include "sn_nsdl_lib.h"

#include "unity.h"
#include "string.h"
#include "stdlib.h"

/* non-test function declarations */
void fill_with_random(uint8_t *ptr, uint16_t len);
void *own_alloc(uint16_t size);
void own_free(void *ptr);
uint8_t nsdl_tx_dummy(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *);
uint8_t nsdl_rx_dummy(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *);
uint8_t dynamic_callback(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *address_ptr, sn_nsdl_capab_e protocol);
static void init_temp_variables(void);

static uint8_t res1_path[] = {"test/res1"};
static uint8_t res1_content[] = {"res1 content"};

static uint8_t res2_path[] = {"test/res2"};
static uint8_t res2_content[] = {"res2 content"};

static uint8_t res3_path[] = {"test/res3"};
static uint8_t res3_content[] = {"res3 content"};

static uint8_t res4_path[] = {"test/res4"};
static uint8_t res4_content[] = {"res4 content"};

static uint8_t res5_path[] = {"test/res5"};
static uint8_t res5_content[] = {"res5 content"};

static uint8_t res_negative_path[] = {"test/negative"};
static uint8_t res_negative_content[] = {"negative_test content"};

static uint8_t address[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

/* Change byte 13 to differend resources */
static uint8_t get_message[14] = {0x40, 0x01, 0x12, 0x34, 0xb4, 0x74, 0x65, 0x73, 0x74, 0x04, 0x72, 0x65, 0x73, 0x31};
static uint8_t put_message[18] = {0x40, 0x03, 0x12, 0x34, 0xb4, 0x74, 0x65, 0x73, 0x74, 0x04, 0x72, 0x65, 0x73, 0x32, 0xff, 0x61, 0x62, 0x63};
static uint8_t post_message[18] = {0x40, 0x02, 0x12, 0x34, 0xb4, 0x74, 0x65, 0x73, 0x74, 0x04, 0x72, 0x65, 0x73, 0x33, 0xff, 0x61, 0x62, 0x63};
static uint8_t delete_message[14] = {0x40, 0x04, 0x12, 0x34, 0xb4, 0x74, 0x65, 0x73, 0x74, 0x04, 0x72, 0x65, 0x73, 0x34};

static uint8_t message_payload[3] = {0x61, 0x62, 0x63};

static uint8_t get_response[17] = {0x60, 0x45, 0x12, 0x34, 0xff, 0x72, 0x65, 0x73, 0x31, 0x20, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74};
static uint8_t response_not_allowed[4] = {0x60, 0x85, 0x12, 0x34};
static uint8_t response_changed[4] = {0x60, 0x44, 0x12, 0x34};
static uint8_t response_created[4] = {0x60, 0x41, 0x12, 0x34};
static uint8_t response_deleted[4] = {0x60, 0x42, 0x12, 0x34};

#define NSP_PORT 5683

#define NSP_REGISTRATION_MESSAGE_LEN 72
static uint8_t registration_message[NSP_REGISTRATION_MESSAGE_LEN] = {
    0x40, 0x02, 0x01, 0x47, 0xb2, 0x72, 0x64, 0xff, 0x3c, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x72,
    0x65, 0x73, 0x35, 0x3e, 0x2c, 0x3c, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x72, 0x65, 0x73, 0x34,
    0x3e, 0x2c, 0x3c, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x72, 0x65, 0x73, 0x33, 0x3e, 0x2c, 0x3c,
    0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x72, 0x65, 0x73, 0x32, 0x3e, 0x2c, 0x3c, 0x2f, 0x74, 0x65,
    0x73, 0x74, 0x2f, 0x72, 0x65, 0x73, 0x31, 0x3e
};

static uint16_t message_temp_len = 0;
static uint8_t *message_temp_ptr = 0;
static sn_nsdl_addr_s *address_temp_ptr = 0;

struct nsdl_s *handle;

/* Unity test code starts */
void setUp(void)
{
    //This is run before EACH TEST

}

void tearDown(void)
{

}

/**
 * \fn void test_libnsdl_init(void)
 *
 * \brief Tests sn_nsdl_init - function. Function must return failure (-1) if initialization fails.
 *
 *  Negative test cases:
 *  - Memory structure contains null pointer(s)
 *  - One of the parameter is null
 *
 */
void test_libnsdl_init(void)
{
    /* Memory struct contains null */
    handle = sn_nsdl_init(&nsdl_tx_dummy, &nsdl_rx_dummy, NULL, NULL);
    TEST_ASSERT_NULL(handle);

    handle = sn_nsdl_init(&nsdl_tx_dummy, &nsdl_rx_dummy, &own_alloc, NULL);
    TEST_ASSERT_NULL(handle);

    handle = sn_nsdl_init(&nsdl_tx_dummy, &nsdl_rx_dummy, NULL, &own_free);
    TEST_ASSERT_NULL(handle);

    /* One of parameters is null */
    handle = sn_nsdl_init(NULL, &nsdl_rx_dummy, &own_alloc, &own_free);
    TEST_ASSERT_NULL(handle);

    handle = sn_nsdl_init(&nsdl_tx_dummy, NULL, &own_alloc, &own_free);
    TEST_ASSERT_NULL(handle);

    /* OK case */
    handle = sn_nsdl_init(&nsdl_tx_dummy, &nsdl_rx_dummy, &own_alloc, &own_free);
    TEST_ASSERT_NOT_NULL(handle);
}

/**
 * \fn void test_libnsdl_create_resource(void)
 *
 * \brief Tests sn_nsdl_create_resource - function. Creates five resources.
 *
 *  STATIC RESOURCES:
 *  - Resource 1
 *      * Get allowed
 *  - Resource 2
 *      * Put allowed
 *  - Resource 3
 *      * Post allowed
 *  - Resource 4
 *      * Delete allowed
 *
 *  DYNAMIC RESOURCES:
 *  - Resource 5
 *      * All allowed
 *
 *  Negative test cases:
 *  - Resource already exists
 *  - Null pointer as a parameter
 *
 */
void test_libnsdl_create_resource(void)
{
    sn_nsdl_resource_info_s resource;
    sn_nsdl_resource_parameters_s resource_parameters;
    int8_t ret_val;

    memset(&resource, 0, sizeof(sn_nsdl_resource_info_s));
    memset(&resource_parameters, 0, sizeof(sn_nsdl_resource_parameters_s));


    /* Resource 1*/
    resource.access = SN_GRS_GET_ALLOWED;
    resource.mode = SN_GRS_STATIC;
    resource.path = res1_path;
    resource.pathlen = sizeof(res1_path) - 1;
    resource.resource = res1_content;
    resource.resourcelen = sizeof(res1_content) - 1;
    resource.resource_parameters_ptr = &resource_parameters;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(0, ret_val);


    /* Resource 2 */
    resource.access = SN_GRS_PUT_ALLOWED;
    resource.path = res2_path;
    resource.pathlen = sizeof(res2_path) - 1;
    resource.resource = res2_content;
    resource.resourcelen = sizeof(res2_content) - 1;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(0, ret_val);


    /* Resource 3 */
    resource.access = SN_GRS_POST_ALLOWED;
    resource.path = res3_path;
    resource.pathlen = sizeof(res3_path) - 1;
    resource.resource = res3_content;
    resource.resourcelen = sizeof(res3_content) - 1;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(0, ret_val);


    /* Resource 4 */
    resource.access = SN_GRS_DELETE_ALLOWED;
    resource.path = res4_path;
    resource.pathlen = sizeof(res4_path) - 1;
    resource.resource = res4_content;
    resource.resourcelen = sizeof(res4_content) - 1;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(0, ret_val);


    /* Resource 5 - Dynamic*/
    resource.mode = SN_GRS_DYNAMIC;

    resource.access = 0xFF;
    resource.path = res5_path;
    resource.pathlen = sizeof(res5_path) - 1;
    resource.resource = res5_content;
    resource.resourcelen = sizeof(res5_content) - 1;
    resource.sn_grs_dyn_res_callback = &dynamic_callback;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(0, ret_val);

    /*** Negative test cases ***/

    /* Already exists */
    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(-2, ret_val);

    /* Null pointer */
    ret_val = sn_nsdl_create_resource(handle, NULL);
    TEST_ASSERT_EQUAL(-1, ret_val);

    ret_val = sn_nsdl_create_resource(NULL, &resource);
    TEST_ASSERT_EQUAL(-1, ret_val);

    resource.path = NULL;
    resource.pathlen = sizeof(res_negative_path) - 1;
    resource.resource = res_negative_content;
    resource.resourcelen = sizeof(res_negative_content) - 1;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(-3, ret_val);

    resource.path = res_negative_path;
    resource.pathlen = 0;

    ret_val = sn_nsdl_create_resource(handle, &resource);
    TEST_ASSERT_EQUAL(-3, ret_val);

}

/**
 * \fn test_libnsdl_list_resources(void)
 *
 * \brief Calls sn_nsdl_list_resource - function
 *  Function must return list of early created resources
 */

void test_libnsdl_list_resources(void)
{
    sn_grs_resource_list_s *resource_list_ptr;

    resource_list_ptr = sn_nsdl_list_resource(handle, 0, 0);

    TEST_ASSERT_EQUAL(5, resource_list_ptr->res_count);

    TEST_ASSERT_EQUAL_INT8_ARRAY(resource_list_ptr->res[4].path, res1_path, resource_list_ptr->res[4].pathlen);
    TEST_ASSERT_EQUAL_INT8_ARRAY(resource_list_ptr->res[3].path, res2_path, resource_list_ptr->res[3].pathlen);
    TEST_ASSERT_EQUAL_INT8_ARRAY(resource_list_ptr->res[2].path, res3_path, resource_list_ptr->res[2].pathlen);
    TEST_ASSERT_EQUAL_INT8_ARRAY(resource_list_ptr->res[1].path, res4_path, resource_list_ptr->res[1].pathlen);
    TEST_ASSERT_EQUAL_INT8_ARRAY(resource_list_ptr->res[0].path, res5_path, resource_list_ptr->res[0].pathlen);

}

/**
 * \fn test_libnsdl_get_resource(void)
 *
 * \brief Calls sn_nsdl_get_resource - function
 *  Function must return pointer to created resource
 *
 * Negative test cases:
 * - call function with null parameter
 *
 */

void test_libnsdl_get_resource(void)
{
    sn_nsdl_resource_info_s *res_ptr;

    /* With null pointer */
    res_ptr = sn_nsdl_get_resource(handle, 0, res1_path);
    TEST_ASSERT_NULL(res_ptr);

    res_ptr = sn_nsdl_get_resource(handle, sizeof(res1_path) - 1, NULL);
    TEST_ASSERT_NULL(res_ptr);

    res_ptr = sn_nsdl_get_resource(NULL, sizeof(res1_path) - 1, res1_path);
    TEST_ASSERT_NULL(res_ptr);

    /* Ok case, gets resource 1 and checks that path and content are ok */
    res_ptr = sn_nsdl_get_resource(handle, sizeof(res1_path) - 1, res1_path);
    TEST_ASSERT_NOT_NULL(res_ptr);

    TEST_ASSERT_EQUAL_INT8_ARRAY(res_ptr->path, res1_path, sizeof(res1_path) - 1);
    TEST_ASSERT_EQUAL_INT8_ARRAY(res_ptr->resource, res1_content, sizeof(res1_content) - 1);
}

/**
 * \fn test_libnsdl_set_nsp_address(void)
 *
 * \brief Sets NSP address to nsdl-library
 *
 *  Negative test cases:
 *  - call function with null parameter
 */
void test_libnsdl_set_nsp_address(void)
{
    int8_t ret_val = 0;

    /* with null pointer */
    ret_val = set_NSP_address(handle, NULL, NSP_PORT, SN_NSDL_ADDRESS_TYPE_IPV4);
    TEST_ASSERT_EQUAL(-1, ret_val);

    ret_val = set_NSP_address(NULL, address, NSP_PORT, SN_NSDL_ADDRESS_TYPE_IPV4);
    TEST_ASSERT_EQUAL(-1, ret_val);

    /* OK case */
    ret_val = set_NSP_address(handle, address, NSP_PORT, SN_NSDL_ADDRESS_TYPE_IPV4);
    TEST_ASSERT_EQUAL(0, ret_val);
}

/**
 * \fn test_libnsdl_register(void)
 *
 * \brief Calls sn_nsdl_get_resource - function
 *
 *  Negative test cases:
 *  - call function with null parameter
 *
 */

void test_libnsdl_register(void)
{
    int8_t ret_val = 0;
    sn_nsdl_ep_parameters_s endpoint_info;

    memset(&endpoint_info, 0, sizeof(sn_nsdl_ep_parameters_s));

    /* With null pointer */
    ret_val = sn_nsdl_register_endpoint(handle, NULL);
    TEST_ASSERT_EQUAL(0, ret_val);

    ret_val = sn_nsdl_register_endpoint(NULL, &endpoint_info);
    TEST_ASSERT_EQUAL(0, ret_val);

    /* OK case */
    ret_val = sn_nsdl_register_endpoint(handle, &endpoint_info);
    TEST_ASSERT_NOT_EQUAL(0, ret_val);

    /* Check address */
    TEST_ASSERT_EQUAL(NSP_PORT, address_temp_ptr->port);
    TEST_ASSERT_EQUAL_INT8_ARRAY(address, address_temp_ptr->addr_ptr, address_temp_ptr->addr_len);

    /* Check registration message !Note, message ID [bytes 2-3] is random in every message! */
    TEST_ASSERT_EQUAL(NSP_REGISTRATION_MESSAGE_LEN, message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_temp_ptr, registration_message, 2); //message header
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_temp_ptr + 4, registration_message + 4, message_temp_len - 4); //Rest of the message

    init_temp_variables();
}

/**
 * \fn void test_libnsdl_get_requests_to_resources(void)
 *
 * \brief Process CoAP message "GET"
 *
 *  - GET to resource that is allowed
 *  - GET to resource that is not allowed
 *
 */

void test_libnsdl_get_requests_to_resources(void)
{
    sn_nsdl_addr_s address_struct;
    int8_t ret_val = 0;

    address_struct.addr_ptr = address;
    address_struct.addr_len = 16;
    address_struct.port = NSP_PORT;

    /* Get to resource that is allowed */
    ret_val = sn_nsdl_process_coap(handle, get_message, sizeof(get_message), &address_struct);

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(get_response), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_temp_ptr, get_response, message_temp_len);

    init_temp_variables();

    /* Get to resource that is not allowed */
    get_message[13] = 0x32;
    ret_val = sn_nsdl_process_coap(handle, get_message, sizeof(get_message), &address_struct);
    get_message[13] = 0x31;

    TEST_ASSERT_EQUAL(0, ret_val);

    TEST_ASSERT_EQUAL(sizeof(response_not_allowed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_temp_ptr, response_not_allowed, message_temp_len);

    init_temp_variables();
}

/**
 * \fn void test_libnsdl_put_requests_to_resources(void)
 *
 * \brief Process CoAP message "PUT"
 *
 *  - PUT to resource that is allowed
 *  - PUT to resource that is not allowed
 */
void test_libnsdl_put_requests_to_resources(void)
{
    sn_nsdl_addr_s address_struct;
    int8_t ret_val = 0;
    sn_nsdl_resource_info_s *res_ptr;

    address_struct.addr_ptr = address;
    address_struct.addr_len = 16;
    address_struct.port = NSP_PORT;

    /* Put to resource that is allowed */
    ret_val = sn_nsdl_process_coap(handle, put_message, sizeof(put_message), &address_struct);

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(response_changed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(response_changed, message_temp_ptr, message_temp_len);

    //get resource and read res value
    res_ptr = sn_nsdl_get_resource(handle, sizeof(res2_path) - 1, res2_path);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_payload, res_ptr->resource, sizeof(message_payload));

    init_temp_variables();

    /* Put to resource that is not allowed */
    put_message[13] = 0x31;
    ret_val = sn_nsdl_process_coap(handle, put_message, sizeof(put_message), &address_struct);
    put_message[13] = 0x32;

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(response_not_allowed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(response_not_allowed, message_temp_ptr, message_temp_len);

    init_temp_variables();
}

/**
 * \fn void test_libnsdl_post_requests_to_resources(void)
 *
 * \brief Process CoAP message "POST"
 *
 *  - POST to resource that is allowed
 *  - POST to resource that is not allowed
 *
 */
void test_libnsdl_post_requests_to_resources(void)
{
    sn_nsdl_addr_s address_struct;
    int8_t ret_val = 0;
    sn_nsdl_resource_info_s *res_ptr;

    address_struct.addr_ptr = address;
    address_struct.addr_len = 16;
    address_struct.port = NSP_PORT;

    /* Post to resource that is allowed */
    ret_val = sn_nsdl_process_coap(handle, post_message, sizeof(post_message), &address_struct);

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(response_changed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(response_changed, message_temp_ptr, message_temp_len);

    //get resource and read res value
    res_ptr = sn_nsdl_get_resource(handle, sizeof(res3_path) - 1, res3_path);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_payload, res_ptr->resource, sizeof(message_payload));

    init_temp_variables();

    /* Post to resource that is not allowed */
    post_message[13] = 0x32;
    ret_val = sn_nsdl_process_coap(handle, post_message, sizeof(post_message), &address_struct);
    post_message[13] = 0x33;

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(response_not_allowed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(response_not_allowed, message_temp_ptr, message_temp_len);

    init_temp_variables();
}

/**
 * \fn void test_libnsdl_delete_requests_to_resources(void)
 *
 * \brief Process CoAP message "DELETE"
 *
 *  - DELETE to resource that is allowed
 *  - DELETE to resource that is not allowed
 *
 */
void test_libnsdl_delete_requests_to_resources(void)
{
    sn_nsdl_addr_s address_struct;
    int8_t ret_val = 0;
    sn_nsdl_resource_info_s *res_ptr;

    address_struct.addr_ptr = address;
    address_struct.addr_len = 16;
    address_struct.port = NSP_PORT;

    /* Delete to resource that is not allowed */
    delete_message[13] = 0x33;
    ret_val = sn_nsdl_process_coap(handle, delete_message, sizeof(delete_message), &address_struct);
    delete_message[13] = 0x34;

    TEST_ASSERT_EQUAL(0, ret_val);
    TEST_ASSERT_EQUAL(sizeof(response_not_allowed), message_temp_len);
    TEST_ASSERT_EQUAL_INT8_ARRAY(response_not_allowed, message_temp_ptr, message_temp_len);

    init_temp_variables();
}

/**
 * \fn void test_libnsdl_update_resource_value(void)
 *
 * \brief Updates resource value from application
 *
 */
void test_libnsdl_update_resource_value(void)
{
    sn_nsdl_resource_info_s resource;
    sn_nsdl_resource_info_s *res_ptr;
    sn_nsdl_resource_parameters_s resource_parameters;
    int8_t ret_val;

    memset(&resource, 0, sizeof(sn_nsdl_resource_info_s));
    memset(&resource_parameters, 0, sizeof(sn_nsdl_resource_parameters_s));

    /* Resource 1*/
    resource.access = SN_GRS_GET_ALLOWED;
    resource.mode = SN_GRS_STATIC;
    resource.path = res1_path;
    resource.pathlen = sizeof(res1_path) - 1;
    resource.resource = message_payload;
    resource.resourcelen = sizeof(message_payload);
    resource.resource_parameters_ptr = &resource_parameters;

    ret_val = sn_nsdl_update_resource(handle, &resource);

    TEST_ASSERT_EQUAL(0, ret_val);

    //get resource and read res value
    res_ptr = sn_nsdl_get_resource(handle, sizeof(res1_path) - 1, res1_path);
    TEST_ASSERT_NOT_NULL(res_ptr);
    TEST_ASSERT_EQUAL_INT8_ARRAY(message_payload, res_ptr->resource, sizeof(message_payload));

}

/**
 * \fn void test_libnsdl_delete_resource(void)
 *
 * \brief Delete resource from application
 *
 */
void test_libnsdl_delete_resource(void)
{
    sn_nsdl_resource_info_s *res_ptr;
    int8_t ret_val;

    ret_val = sn_nsdl_delete_resource(handle, sizeof(res4_path) - 1, res4_path);

    TEST_ASSERT_EQUAL(0, ret_val);

    //get resource and read res value
    res_ptr = sn_nsdl_get_resource(handle, sizeof(res4_path) - 1, res4_path);
    TEST_ASSERT_NULL(res_ptr);
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

uint8_t nsdl_tx_dummy(struct nsdl_s *handle, sn_nsdl_capab_e protocol, uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr)
{
    message_temp_ptr = malloc(data_len);
    memcpy(message_temp_ptr, data_ptr, data_len);

    message_temp_len = data_len;

    address_temp_ptr = malloc(sizeof(sn_nsdl_addr_s));
    memcpy(address_temp_ptr, address_ptr, sizeof(sn_nsdl_addr_s));

#if 0
    uint8_t i;
    printf("\nlength = %d\n", data_len);
    for (i = 0; i < data_len; i++) {
        printf("0x");
        if (*(data_ptr + i) <= 0x0F) {
            printf("0");
        }
        printf("%x, ", *(data_ptr + i));
        if (!((i + 1) % 16)) {
            printf("\n");
        }
    }
    printf("\n");
#endif
}

uint8_t nsdl_rx_dummy(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *address_ptr)
{

}

uint8_t dynamic_callback(struct nsdl_s *handle, sn_coap_hdr_s *coap_header_ptr, sn_nsdl_addr_s *address_ptr, sn_nsdl_capab_e protocol)
{

}

static void init_temp_variables(void)
{
    if (message_temp_ptr) {
        free(message_temp_ptr);
        message_temp_ptr = 0;
    }
    if (address_temp_ptr) {
        free(address_temp_ptr);
        address_temp_ptr = 0;
    }
    message_temp_len = 0;
}








