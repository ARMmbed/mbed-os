/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
/*
 * \file thread_network_data_lib.c
 * \brief Add short description about this file!!!
 *
 */
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "common_functions.h"
#include "ns_trace.h"

#define TRACE_GROUP "tndl"

uint8_t prefixBits_to_bytes(uint8_t prefixLenInBits)
{
    return 0;
}

int thread_network_data_malformed_check(uint8_t *network_data_ptr, uint8_t network_data_length)
{
    return 0;
}

int thread_network_data_sub_tlv_malformed_check(uint8_t *network_data_ptr, uint8_t network_data_length)
{
    return 0;
}


uint8_t *thread_nd_commission_data_write_steering_data(uint8_t *ptr, const uint8_t *steeringData, uint8_t length)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_commissioner_id(uint8_t *ptr, char *commissioner_id_ptr)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_security_policy(uint8_t *ptr, uint8_t policy)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_timestamp(uint8_t *ptr, uint8_t *timestamp, uint16_t tick)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_session_id(uint8_t *ptr, uint16_t id)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_network_name(uint8_t *ptr, uint8_t *name)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_border_router_locator(uint8_t *ptr, uint8_t *locatorML16)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_commissioning_credentials(uint8_t *ptr, char *crendentials, uint8_t length)
{
    return NULL;
}

uint8_t *thread_nd_network_data_write_border_router_or_router_info(uint8_t *ptr, uint16_t routerId, uint8_t flags, bool reservedBytes)
{
    return NULL;
}

