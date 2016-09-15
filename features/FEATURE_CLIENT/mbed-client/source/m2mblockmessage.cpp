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
#include "mbed-client/m2mblockmessage.h"
#include <stdlib.h>
#include <string.h>

M2MBlockMessage::M2MBlockMessage() :
    _block_number(0),
    _total_message_size(0),
    _is_last_block(false),
    _block_data_ptr(NULL),
    _block_data_len(0),
    _error_code(M2MBlockMessage::ErrorNone),
    _is_block_message(false)
{
}

M2MBlockMessage::~M2MBlockMessage()
{
    free(_block_data_ptr);
    _block_data_ptr = NULL;
}

void M2MBlockMessage::set_message_info(sn_coap_hdr_s *coap_header)
{
    _is_block_message = (coap_header &&
                coap_header->options_list_ptr &&
                coap_header->options_list_ptr->block1_ptr &&
                coap_header->options_list_ptr->block1_len > 0) ? true : false;

    if (coap_header && coap_header->options_list_ptr) {
        // Check total size
        if (coap_header->options_list_ptr->size1_ptr) {
            for(int i=0;i < coap_header->options_list_ptr->size1_len; i++) {
                _total_message_size += (*(coap_header->options_list_ptr->size1_ptr + i) & 0xff) <<
                         8*(coap_header->options_list_ptr->size1_len- 1 - i);
                }
        }

        // Default value in coap library is 65kb
        uint32_t max_size = UINT16_MAX;
        #if YOTTA_CFG_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE
            max_size = YOTTA_CFG_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE;
        #endif
        #if MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE
            max_size = MBED_CONF_MBED_CLIENT_SN_COAP_MAX_INCOMING_MESSAGE_SIZE;
        #endif
        if (_total_message_size > max_size) {
            _error_code = M2MBlockMessage::EntityTooLarge;
        } else {
            _error_code = M2MBlockMessage::ErrorNone;
        }
        if (M2MBlockMessage::ErrorNone == _error_code) {
            // Is last block
            if (coap_header->options_list_ptr->block1_ptr) {
                if (!(*(coap_header->options_list_ptr->block1_ptr + (coap_header->options_list_ptr->block1_len - 1)) & 0x08)) {
                    _is_last_block = true;
                } else {
                    _is_last_block = false;
                }
            }

            // Block number
            if (coap_header->options_list_ptr->block1_len == 3) {
                _block_number = *(coap_header->options_list_ptr->block1_ptr) << 12;
                _block_number |= *(coap_header->options_list_ptr->block1_ptr + 1) << 4;
                _block_number |= (*(coap_header->options_list_ptr->block1_ptr + 2)) >> 4;
            }

            else if (coap_header->options_list_ptr->block1_len == 2) {
                _block_number = *(coap_header->options_list_ptr->block1_ptr) << 4;
                _block_number |= (*(coap_header->options_list_ptr->block1_ptr + 1)) >> 4;
            }
            else if (coap_header->options_list_ptr->block1_len == 1) {
                _block_number = (*coap_header->options_list_ptr->block1_ptr) >> 4;
            }
            else {
                _block_number = 0;
            }

            // Payload
            free(_block_data_ptr);
            _block_data_ptr = NULL;
            _block_data_len = coap_header->payload_len;
            if(_block_data_len > 0) {
                _block_data_ptr = (uint8_t *)malloc(_block_data_len);
                if (_block_data_ptr) {
                    memcpy(_block_data_ptr, coap_header->payload_ptr, _block_data_len);
                }
            }
        }
    }
}

void M2MBlockMessage::clear_values()
{
    free(_block_data_ptr);
    _block_data_ptr = NULL;
    _block_data_len = 0;
    _block_number = 0;
    _total_message_size = 0;
    _is_last_block = false;
    _error_code = M2MBlockMessage::ErrorNone;
}

bool M2MBlockMessage::is_block_message() const
{
    return _is_block_message;
}

uint16_t M2MBlockMessage::block_number() const
{
    return _block_number;
}

uint32_t M2MBlockMessage::total_message_size() const
{
    return _total_message_size;
}

bool M2MBlockMessage::is_last_block() const
{
    return _is_last_block;
}

uint8_t* M2MBlockMessage::block_data() const
{
    return _block_data_ptr;
}

uint32_t M2MBlockMessage::block_data_len() const
{
    return _block_data_len;
}

M2MBlockMessage::Error M2MBlockMessage::error_code() const
{
    return _error_code;
}
