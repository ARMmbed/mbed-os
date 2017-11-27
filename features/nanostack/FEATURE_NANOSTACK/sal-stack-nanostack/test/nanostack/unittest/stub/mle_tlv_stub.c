/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "string.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "common_functions.h"
#include "mle_tlv_stub.h"

mle_tlv_stub_def mle_tlv_stub;

int mle_message_malformed_check(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

int mle_tlv_option_discover(uint8_t *ptr, uint16_t data_len, mle_tlv_type_t discovered_type, mle_tlv_info_t *option_info)
{
    return mle_tlv_stub.expectedInt;
}

int mle_validate_advertisment_message(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

int mle_validate_router_link_accept_request_message(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

int mle_validate_router_link_accept_message(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

bool mle_tlv_type_requested(uint8_t reqType, uint8_t *ptr, uint16_t data_len)
{
    return false;
}

bool mle_tlv_read_8_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint8_t *buffer)
{
    return false;
}

bool mle_tlv_read_16_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint16_t *buffer)
{
    return false;
}

bool mle_tlv_read_32_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint32_t *buffer)
{
    return false;
}

bool mle_tlv_read_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, mle_tlv_info_t *tlv_info)
{
    return false;
}

int mle_validate_child_keep_alive_message(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

int mle_validate_child_update_message(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

uint8_t *mle_tlv_write_response(uint8_t *ptr, uint8_t *response_ptr, uint8_t responseLen)
{
    return ptr + responseLen + 2;
}

uint8_t *mle_tlv_write_source_address(uint8_t *ptr, uint16_t shortAddress)
{
    return ptr + 4;
}

uint8_t *mle_tlv_write_short_address(uint8_t *ptr, uint16_t shortAddress)
{
    return ptr + 4;
}

uint8_t *mle_tlv_write_mode(uint8_t *ptr, uint8_t mode)
{
    return ptr + 3;
}

uint8_t *mle_tlv_write_timeout(uint8_t *ptr, uint32_t timeOut)
{
    return ptr + 6;
}

uint8_t *mle_tlv_write_holdtime(uint8_t *ptr, uint16_t holdTime)
{
    return ptr + 4;
}

uint8_t *mle_tlv_write_challenge(uint8_t *ptr, uint8_t *challengePtr, uint8_t challenLen)
{
    return ptr + challenLen + 2;
}

uint8_t *mle_tlv_write_link_layer_framecount(uint8_t *ptr, uint32_t frameCount)
{
    return ptr + 6;
}

uint8_t *mle_tlv_write_scan_mask(uint8_t *ptr, uint8_t scanMask)
{
    return ptr + 3;
}

uint8_t *mle_tlv_req_tlv(uint8_t *ptr, uint8_t *mle_req_tlv_list, uint8_t req_list_len)
{
    return NULL;
}
uint8_t *mle_tlv_rssi_tlv(uint8_t *ptr, uint8_t linkMargin)
{
    return NULL;
}

uint8_t *mle_tlv_write_version(uint8_t *ptr, uint16_t version)
{
    return ptr + 4;
}

uint8_t *mle_tlv_write_link_quality(uint8_t *ptr, uint8_t incoming_idr, uint8_t *mac64, uint16_t short_address, uint8_t priority_flag)
{
    if( mac64 ) {
        return ptr + 13;
    } else {
        return ptr + 7;
    }
}

uint16_t mle_tlv_validate_response(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

