/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "ns_list.h"
#include "common_functions.h"
#include "platform/ns_debug.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"

typedef struct iphc_compress_state {
    protocol_interface_info_entry_t *const interface;
    const uint8_t *in;
    uint8_t *out;
    uint16_t len;
    uint16_t out_space;
    uint16_t consumed;
    uint16_t produced;
    const uint8_t *outer_src_iid;
    const uint8_t *outer_dst_iid;
} iphc_compress_state_t;

buffer_t *iphc_compress(protocol_interface_info_entry_t *cur, buffer_t *buf, uint16_t hc_space)
{
    return NULL;
}
