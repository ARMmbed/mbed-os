/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include "string.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "common_functions.h"
#include "Service_Libs/mle_service/mle_service_api.h"

int mle_message_malformed_check(uint8_t *ptr, uint16_t data_len)
{
    uint8_t *dptr;
    uint16_t length;
    dptr = ptr;
    while (data_len) {
        if (data_len >= 2) {
            dptr += 1; //Skip TLV Type
            length = *dptr++;
            if (length == 0xff) {
                // Long length format
                data_len -= 2;
                if (data_len < 2) {
                    return -1;
                }
                length = common_read_16_bit(dptr);
                dptr += 2;
            }
            data_len -= 2;
            if (data_len >= length) {
                if (length) {
                    data_len -= length;
                    dptr += length;
                }
            } else {
                // buffer is overrun this is malformed.
                return -1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}

int mle_tlv_option_discover(uint8_t *ptr, uint16_t data_len, mle_tlv_type_t discovered_type, mle_tlv_info_t *option_info)
{
    uint8_t *dptr;
    uint16_t length;
    mle_tlv_type_t type;

    option_info->tlvLen = 0;
    option_info->tlvType = MLE_TYPE_UNASSIGNED;
    option_info->dataPtr = NULL;
    dptr = ptr;
    while (data_len) {
        type = (mle_tlv_type_t) * dptr++;
        length = *dptr++;
        if (length == 0xff) {
            // Long length format
            data_len -= 2;
            if (data_len < 2) {
                return -1;
            }
            length = common_read_16_bit(dptr);
            dptr += 2;
        }
        data_len -= 2;
        if (data_len >= length) {
            if (type == discovered_type) {
                option_info->tlvLen = length;
                option_info->tlvType = type;
                option_info->dataPtr = dptr;
                return length;
            } else {
                data_len -= length;
                dptr += length;
            }
        }
    }
    return -1;
}

bool mle_tlv_type_requested(uint8_t reqType, uint8_t *ptr, uint16_t data_len)
{
    mle_tlv_info_t tlv_info;
    bool retVal = false;
    if (mle_tlv_option_discover(ptr, data_len, MLE_TYPE_TLV_REQUEST, &tlv_info) > 0) {
        uint16_t i;
        uint8_t *t_ptr;
        t_ptr = tlv_info.dataPtr;
        for (i = 0; i < tlv_info.tlvLen; i++) {
            if (*t_ptr++ == reqType) {
                retVal = true;
                break;
            }
        }
    }
    return retVal;
}

bool mle_tlv_read_8_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint8_t *buffer)
{
    mle_tlv_info_t tlv_info;
    if (mle_tlv_option_discover(ptr, data_len, reqType, &tlv_info) >= 1) {
        uint8_t *t_ptr;
        t_ptr = tlv_info.dataPtr;
        *buffer = *t_ptr;
        return true;
    }

    return false;
}
bool mle_tlv_read_16_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint16_t *buffer)
{
    mle_tlv_info_t tlv_info;
    if (mle_tlv_option_discover(ptr, data_len, reqType, &tlv_info) >= 2) {
        *buffer = common_read_16_bit(tlv_info.dataPtr);
        return true;
    }

    return false;
}
bool mle_tlv_read_32_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint32_t *buffer)
{
    mle_tlv_info_t tlv_info;
    if (mle_tlv_option_discover(ptr, data_len, reqType, &tlv_info) >= 4) {
        *buffer = common_read_32_bit(tlv_info.dataPtr);
        return true;
    }

    return false;
}

bool mle_tlv_read_64_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint64_t *buffer)
{
    mle_tlv_info_t tlv_info;
    if (mle_tlv_option_discover(ptr, data_len, reqType, &tlv_info) >= 8) {
        *buffer = common_read_64_bit(tlv_info.dataPtr);
        return true;
    }

    return false;
}

bool mle_registerd_address_data_check(uint8_t *ptr, uint8_t length)
{
    uint8_t context;
    if (length < 9) {
        return false;
    }

    while (length) {
        context = *ptr++;
        if (context & 0x80) {
            if (length < 9) {
                return false;
            }
            ptr += 8;
            length -= 9;
        } else {
            if (length < 17) {
                return false;
            }
            ptr += 16;
            length -= 17;
        }
    }
    return true;
}

bool mle_tlv_read_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, mle_tlv_info_t *tlv_info)
{
    int length;
    length = mle_tlv_option_discover(ptr, data_len, reqType, tlv_info);
    if (length != -1) {
        if (reqType == MLE_TYPE_CHALLENGE && length > 3) { //32-bit challenge MIN which is accepted
            return true;
        } else if (reqType == MLE_TYPE_RESPONSE && length > 3) { //32-bit challenge MIN which is accepted
            return true;
        } else if (reqType == MLE_TYPE_ADDRESS_REGISTRATION && length) {
            return mle_registerd_address_data_check(tlv_info->dataPtr, length);
        } else if (reqType == MLE_TYPE_ROUTE && length >= 9) {
            return true;
        } else if (reqType == MLE_TYPE_LINK_METRICS_REPORT) {
            return true;
        } else if (reqType == MLE_TYPE_TLV_REQUEST || reqType ==  MLE_TYPE_NETWORK_DATA ||
                   reqType ==  MLE_TYPE_PENDING_OPERATIONAL_DATASET || reqType ==  MLE_TYPE_OPERATIONAL_DATASET) {
            return true;
        }
    }
    return false;
}

uint8_t *mle_tlv_write_response(uint8_t *ptr, uint8_t *response_ptr, uint8_t responseLen)
{
    *ptr++ = MLE_TYPE_RESPONSE;
    *ptr++ = responseLen;
    memcpy(ptr, response_ptr,  responseLen);
    ptr += responseLen;
    return ptr;
}

uint8_t *mle_tlv_write_source_address(uint8_t *ptr, uint16_t shortAddress)
{
    *ptr++ = MLE_TYPE_SRC_ADDRESS;
    *ptr++ = 2;
    ptr = common_write_16_bit(shortAddress, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_short_address(uint8_t *ptr, uint16_t shortAddress)
{
    *ptr++ = MLE_TYPE_ADDRESS16;
    *ptr++ = 2;
    ptr = common_write_16_bit(shortAddress, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_mode(uint8_t *ptr, uint8_t mode)
{
    *ptr++ = MLE_TYPE_MODE;
    *ptr++ = 1;
    *ptr++ = mode;
    return ptr;
}

uint8_t *mle_tlv_write_timeout(uint8_t *ptr, uint32_t timeOut)
{
    *ptr++ = MLE_TYPE_TIMEOUT;
    *ptr++ = 4;
    ptr = common_write_32_bit(timeOut, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_challenge(uint8_t *ptr, uint8_t *challengePtr, uint8_t challenLen)
{
    *ptr++ = MLE_TYPE_CHALLENGE;
    *ptr++ = challenLen;
    memcpy(ptr, challengePtr, challenLen);
    ptr += challenLen;
    return ptr;
}

uint8_t *mle_tlv_write_link_layer_framecount(uint8_t *ptr, uint32_t frameCount)
{
    *ptr++ = MLE_TYPE_LL_FRAME_COUNTER;
    *ptr++ = 4;
    ptr = common_write_32_bit(frameCount, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_framecounter(uint8_t *ptr, uint32_t frameCount)
{
    *ptr++ = MLE_TYPE_MLE_FRAME_COUNTER;
    *ptr++ = 4;
    ptr = common_write_32_bit(frameCount, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_scan_mask(uint8_t *ptr, uint8_t scanMask)
{
    *ptr++ = MLE_TYPE_SCAN_MASK;
    *ptr++ = 1;
    *ptr++ = scanMask;
    return ptr;
}
uint8_t *mle_tlv_req_tlv(uint8_t *ptr, uint8_t *mle_req_tlv_list, uint8_t req_list_len)
{
    *ptr++ = MLE_TYPE_TLV_REQUEST;
    *ptr++ = req_list_len;
    memcpy(ptr, mle_req_tlv_list, req_list_len);
    ptr += req_list_len;
    return ptr;
}
uint8_t *mle_tlv_rssi_tlv(uint8_t *ptr, uint8_t linkMargin)
{
    *ptr++ = MLE_TYPE_RSSI;
    *ptr++ = 1;
    *ptr++ = linkMargin;
    return ptr;
}

uint8_t *mle_tlv_write_version(uint8_t *ptr, uint16_t version)
{
    *ptr++ = MLE_TYPE_VERSION;
    *ptr++ = 2;
    ptr = common_write_16_bit(version, ptr);
    return ptr;
}

uint8_t *mle_tlv_write_link_quality(uint8_t *ptr, uint8_t incoming_idr, uint8_t *mac64, uint16_t short_address, uint8_t priority_flag)
{
    *ptr++ = MLE_TYPE_LINK_QUALITY;
    if (mac64) {
        *ptr++ = 11;
        *ptr++ = 0x07; /* 8 bytes long address */
    } else {
        *ptr++ = 5;
        *ptr++ = 0x01; /* 2 bytes long address */
    }

    if (priority_flag) {
        *ptr++ = MLE_NEIGHBOR_PRIORITY_LINK | MLE_NEIGHBOR_INCOMING_LINK | MLE_NEIGHBOR_OUTGOING_LINK;
    } else {
        *ptr++ = MLE_NEIGHBOR_INCOMING_LINK | MLE_NEIGHBOR_OUTGOING_LINK;
    }

    *ptr++ = incoming_idr;

    if (mac64) {
        memcpy(ptr, mac64, 8);
        ptr += 8;
    } else {
        ptr = common_write_16_bit(short_address, ptr);
    }

    return ptr;
}

/**
 * This function will be used for validate response from ACCEPT or ACCEPT Request message
 */
uint16_t mle_tlv_validate_response(uint8_t *ptr, uint16_t data_len)
{
    mle_tlv_info_t tlv_info;
    uint16_t msgId = 0;
    if (mle_tlv_option_discover(ptr, data_len, MLE_TYPE_RESPONSE, &tlv_info) > 3) {
        mle_service_transaction_buffer_get_for_response(tlv_info.dataPtr, tlv_info.tlvLen, &msgId);
        return msgId;
    }
    return msgId;
}

bool mle_tlv_requested(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type)
{
    for (uint16_t i = 0; i < tlv_len; i++) {
        if (tlv_ptr[i] == tlv_type) {
            return true;
        }
    }
    return false;
}

void mle_tlv_ignore(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type)
{
    for (uint16_t i = 0; i < tlv_len; i++) {
        if (tlv_ptr[i] == tlv_type) {
            tlv_ptr[i] = MLE_TYPE_UNASSIGNED;
        }
    }
}

