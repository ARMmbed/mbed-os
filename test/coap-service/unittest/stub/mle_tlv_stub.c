/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "platform/ns_debug.h"
#include "string.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "common_functions.h"

int mle_message_malformed_check(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

int mle_tlv_option_discover(uint8_t *ptr, uint16_t data_len, mle_tlv_type_t discovered_type, mle_tlv_info_t *option_info)
{
    return -1;
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
    return NULL;
}

uint8_t *mle_tlv_write_source_address(uint8_t *ptr, uint16_t shortAddress)
{
    return NULL;
}

uint8_t *mle_tlv_write_short_address(uint8_t *ptr, uint16_t shortAddress)
{
    return NULL;
}

uint8_t *mle_tlv_write_mode(uint8_t *ptr, uint8_t mode)
{
    return NULL;
}

uint8_t *mle_tlv_write_timeout(uint8_t *ptr, uint32_t timeOut)
{
    return NULL;
}

uint8_t *mle_tlv_write_holdtime(uint8_t *ptr, uint16_t holdTime)
{
    return NULL;
}

uint8_t *mle_tlv_write_challenge(uint8_t *ptr, uint8_t *challengePtr, uint8_t challenLen)
{
    return NULL;
}

uint8_t *mle_tlv_write_link_layer_framecount(uint8_t *ptr, uint32_t frameCount)
{
    return NULL;
}

uint8_t *mle_tlv_write_scan_mask(uint8_t *ptr, uint8_t scanMask)
{
    return NULL;
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
    return NULL;
}


