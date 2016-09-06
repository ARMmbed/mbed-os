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
#include "test_m2mblockmessage.h"
#include "sn_coap_header.h"
#include <string.h>
Test_M2MBlockMessage::Test_M2MBlockMessage()
{
    block_message = new M2MBlockMessage();
}

Test_M2MBlockMessage::~Test_M2MBlockMessage()
{
    delete block_message;
}

void Test_M2MBlockMessage::test_set_message_info()
{
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));
    block_message->set_message_info(NULL);
    CHECK(block_message->is_block_message() == false);
    CHECK(block_message->total_message_size() == 0);

    coap_header->options_list_ptr = (sn_coap_options_list_s *)malloc(sizeof(sn_coap_options_list_s));
    memset(coap_header->options_list_ptr, 0, sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->size1_ptr = (uint8_t*)malloc(2);
    coap_header->options_list_ptr->size1_ptr[0] = 0xff;
    coap_header->options_list_ptr->size1_ptr[1] = 0xff;
    coap_header->options_list_ptr->size1_len = 2;

    block_message->set_message_info(coap_header);
    CHECK(block_message->total_message_size() == 0xffff);
    CHECK(!block_message->is_block_message());
    block_message->clear_values();

    free(coap_header->options_list_ptr->size1_ptr);
    coap_header->options_list_ptr->size1_ptr = NULL;
    coap_header->options_list_ptr->size1_ptr = (uint8_t*)malloc(3);
    coap_header->options_list_ptr->size1_ptr[0] = 0xff;
    coap_header->options_list_ptr->size1_ptr[1] = 0xff;
    coap_header->options_list_ptr->size1_ptr[2] = 0xff;
    coap_header->options_list_ptr->size1_len = 3;

    block_message->set_message_info(coap_header);
    CHECK(block_message->error_code() == M2MBlockMessage::EntityTooLarge);
    block_message->clear_values();

    coap_header->options_list_ptr->size1_len = 2;
    coap_header->options_list_ptr->block1_ptr = (uint8_t *)malloc(3);
    coap_header->options_list_ptr->block1_ptr[0] = 0x1;
    coap_header->options_list_ptr->block1_ptr[1] = 0x1;
    coap_header->options_list_ptr->block1_ptr[2] = 0x1;
    coap_header->options_list_ptr->block1_len = 3;
    block_message->set_message_info(coap_header);
    CHECK(block_message->is_block_message());
    CHECK(block_message->block_number() == 4112);
    block_message->clear_values();

    coap_header->options_list_ptr->block1_len = 2;
    block_message->set_message_info(coap_header);
    CHECK(block_message->is_block_message());
    CHECK(block_message->block_number() == 16);
    block_message->clear_values();

    coap_header->options_list_ptr->block1_len = 1;
    block_message->set_message_info(coap_header);
    CHECK(block_message->is_block_message());
    CHECK(block_message->block_number() == 0);
    block_message->clear_values();

    coap_header->options_list_ptr->block1_len = 3;
    coap_header->options_list_ptr->block1_ptr[0] = 0x1;
    coap_header->options_list_ptr->block1_ptr[1] = 0x1;
    coap_header->options_list_ptr->block1_ptr[2] = 0x8;
    block_message->set_message_info(coap_header);
    CHECK(!block_message->is_last_block());
    block_message->clear_values();

    coap_header->payload_len = 10;
    coap_header->payload_ptr = (uint8_t *)malloc(10);
    memset(coap_header->payload_ptr, 'x', 10);
    block_message->set_message_info(coap_header);
    CHECK(!block_message->is_last_block());
    CHECK(block_message->block_data_len() == 10);
    CHECK(memcmp(coap_header->payload_ptr, block_message->block_data(),10) == 0);
    block_message->clear_values();

    free(coap_header->payload_ptr);
    free(coap_header->options_list_ptr->block1_ptr);
    free(coap_header->options_list_ptr->size1_ptr);
    free(coap_header->options_list_ptr);
    free(coap_header);
}
