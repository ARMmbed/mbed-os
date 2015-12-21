/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "Common_Protocols/ipv6.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"

#define TRACE_GROUP_IPHC_DECOMPRESS "iphc"

uint8_t iphc_header_scan(buffer_t *buf, uint16_t *uncompressed_size)
{
    return 0;
}

buffer_t *iphc_decompress(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    return NULL;
}
