/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include <string.h>
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include <net_thread_test.h>

#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"

#define TRACE_GROUP "tapi"

int_fast8_t arm_nwk_6lowpan_thread_test_router_id_push(
    int8_t interface_id,
    const uint8_t *euid64,
    uint8_t router_id)
{
    return 0;
}

int_fast8_t arm_nwk_6lowpan_thread_test_add_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr,
    uint_fast8_t link_margin_db,
    uint8_t id_sequence,
    const uint8_t *id_mask,
    const uint8_t *route_data)
{
    return 0;
}

int_fast8_t arm_nwk_6lowpan_thread_test_remove_neighbour(
    int8_t interface_id,
    uint16_t neighbour_short_addr)
{
    return 0;

}

int_fast8_t arm_nwk_6lowpan_thread_test_set_addr_filter(
    int8_t interface_id,
    uint8_t num_short_addrs,
    const uint16_t valid_short_addrs[num_short_addrs],
    uint8_t num_long_addrs,
    const uint8_t valid_long_addrs[num_long_addrs][8])
{
    return 0;
}

int_fast8_t arm_nwk_6lowpan_thread_adjust_link_margins(
    int8_t interface_id,
    int8_t adjustment)
{
    return 0;
}

void arm_nwk_6lowpan_thread_test_print_routing_database(int8_t interface_id)
{
}

int8_t arm_nwk_key_update_trig(int8_t interface_id)
{
    return 0;
}
