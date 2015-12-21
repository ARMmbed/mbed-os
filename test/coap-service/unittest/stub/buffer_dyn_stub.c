/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "Core/include/buffer.h"
#include "platform/ns_debug.h"
#include "platform/arm_hal_interrupt.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "ip_fsc.h"


uint8_t *(buffer_corrupt_check)(buffer_t *buf)
{
    return 0;
}


buffer_t *buffer_get(uint16_t size)
{
    return NULL;
}

buffer_t *buffer_headroom(buffer_t *buf, uint16_t size)
{
    return NULL;
}

buffer_t *buffer_free_route(buffer_t *buf)
{
    return NULL;
}

buffer_t *buffer_free(buffer_t *buf)
{
    return NULL;
}

buffer_t *buffer_free_list(buffer_t *b)
{
    return NULL;
}


void buffer_data_add(buffer_t *buf, const uint8_t *data_ptr, uint16_t data_len)
{
    return;
}


buffer_t *buffer_clone(buffer_t *buf)
{
    return NULL;
}

uint16_t buffer_ipv6_fcf(const buffer_t *buf, uint8_t next_header)
{
    return 0;
}
