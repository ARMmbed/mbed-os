/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "6LoWPAN/IPHC_Decode/lowpan_context.h"
#include "common_functions.h"

#define TRACE_GROUP_LOWPAN_CONTEXT "lCon"

lowpan_context_t *lowpan_contex_get_by_id(lowpan_context_list_t *list, uint8_t id)
{
    return NULL;
}

lowpan_context_t *lowpan_context_get_by_address(lowpan_context_list_t *list, const uint8_t *ipv6Address)
{
    return NULL;
}


int_fast8_t lowpan_context_update(lowpan_context_list_t *list, uint8_t cid_flags, uint16_t lifetime, const uint8_t *prefix, uint_fast8_t len)
{
    return 0;
}

void lowpan_context_list_free(lowpan_context_list_t *list)
{
}

void lowpan_context_timer(lowpan_context_list_t *list, uint_fast16_t ticks)
{
}

