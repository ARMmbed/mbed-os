/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */

/**
 * \file thread_routerid_server.c
 *
 */
#include "config.h"
#include <string.h>
#include <ns_types.h>
#include <ns_trace.h>
#include "common_functions.h"
#include "libDHCPv6/libDHCPv6.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routerid_server.h"

#define TRACE_GROUP "thid"

int thread_routerid_assign_cb(int8_t interface_id, const uint8_t mac[8], uint8_t router_id_mask_out[9], uint16_t *router_id)
{
    return -1;
}

int thread_routerid_release_cb(int8_t interface_id, uint8_t mac[8], uint16_t router_id)
{
    return -1;
}
