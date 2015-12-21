/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "Core/include/socket.h"
#include "string.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "MulticastTrigle/multicast.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"

void multicast_push(buffer_t *buf)
{

}

void multicast_init(void)
{

}

void multicast_handle_time_event(uint16_t ticksUpdate)
{

}

int multicast_sleepy_unicast_address_register(uint8_t * multicast_address, multicast_forward_to_sleepy *handler)
{
    return 0;
}

void multicast_set_parameters(uint8_t i_min, uint8_t i_max, uint8_t k, uint8_t timer_expirations, uint8_t window_expiration)
{

}

uint8_t multicast_add_address(const uint8_t *address_ptr, uint8_t use_trickle)
{
    return 0;
}

uint8_t multicast_free_address(uint8_t *address_ptr)
{
    return 0;
}
