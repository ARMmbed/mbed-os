/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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

#ifndef MLE_TLV_H_
#define MLE_TLV_H_

#include "MLE/mle.h"

/**
 * TLV message Info structure will be used for Catching proper TLV and save info to current structure
 *
 */
typedef struct mle_tlv_info_s {
    mle_tlv_type_t tlvType;
    uint16_t tlvLen;
    uint8_t *dataPtr;
} mle_tlv_info_t;

/**
 * This function is for Verify That MLE TLV's are valid
 */
int mle_message_malformed_check(uint8_t *ptr, uint16_t data_len);

/**
 * This Function help to discover Requested MLE TLV type from Payload
 *
 * return value
 *  -1 if TLV is not found.
 *  length of TLV if it is found always larger than 0.
 */
int mle_tlv_option_discover(uint8_t *ptr, uint16_t data_len, mle_tlv_type_t discovered_type, mle_tlv_info_t *option_info);

bool mle_tlv_type_requested(uint8_t reqType, uint8_t *ptr, uint16_t data_len);

bool mle_tlv_read_8_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint8_t *buffer);
bool mle_tlv_read_16_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint16_t *buffer);
bool mle_tlv_read_32_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint32_t *buffer);
bool mle_tlv_read_64_bit_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, uint64_t *buffer);
bool mle_tlv_read_tlv(mle_tlv_type_t reqType, uint8_t *ptr, uint16_t data_len, mle_tlv_info_t *tlv_info);

/**
 * Next Function for MLE TLV option write
 *
 */
/**
 * Write MLE Response TLV
 */
uint8_t *mle_tlv_write_response(uint8_t *ptr, uint8_t *response_ptr, uint8_t responseLen);
/**
 * Write MLE Source Address TLV
 */
uint8_t *mle_tlv_write_source_address(uint8_t *ptr, uint16_t shortAddress);
/**
 * Write MLE Short Address TLV
 */
uint8_t *mle_tlv_write_short_address(uint8_t *ptr, uint16_t shortAddress);
/**
 * Write MLE Mode TLV
 */
uint8_t *mle_tlv_write_mode(uint8_t *ptr, uint8_t mode);
/**
 * Write MLE Timeout TLV
 */
uint8_t *mle_tlv_write_timeout(uint8_t *ptr, uint32_t timeOut);
/**
 * Write MLE Challenge TLV
 */
uint8_t *mle_tlv_write_challenge(uint8_t *ptr, uint8_t *challengePtr, uint8_t challenLen);
/**
 * Write MLE Link Layer Frame count TLV
 */
uint8_t *mle_tlv_write_link_layer_framecount(uint8_t *ptr, uint32_t frameCount);
/**
 * Write MLE Frame counter TLV
 */
uint8_t *mle_tlv_write_framecounter(uint8_t *ptr, uint32_t frameCount);
/**
 * Write MLE Scan Mask TLV
 */
uint8_t *mle_tlv_write_scan_mask(uint8_t *ptr, uint8_t scanMask);
/**
 * Write MLE TLV request TLV
 */
uint8_t *mle_tlv_req_tlv(uint8_t *ptr, uint8_t *mle_req_tlv_list, uint8_t req_list_len);
/**
 * Write MLE RSSI TLV
 */
uint8_t *mle_tlv_rssi_tlv(uint8_t *ptr, uint8_t linkMargin);

/**
 * Write MLE version TLV
 */
uint8_t *mle_tlv_write_version(uint8_t *ptr, uint16_t version);

/**
 * Write MLE link quality TLV
 */
uint8_t *mle_tlv_write_link_quality(uint8_t *ptr, uint8_t incoming_idr, uint8_t *mac64, uint16_t short_address, uint8_t priority_flag);
/**
 * Validate response TLV to mle services challenge TLV
 *
 * return 0 when Unknow response other it return buffer id
 */
uint16_t mle_tlv_validate_response(uint8_t *ptr, uint16_t data_len);

/**
 * check if specific TLV is in request TLV list
 *
 * return true if TLV is requested else false
 */
bool mle_tlv_requested(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type);
/**
 * Modifies the TLV list so that specific TLV is removed from the list
 *
 * return 0 when Unknow response other it return buffer id
 */
void mle_tlv_ignore(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type);

#endif /* MLE_TLV_H_ */
