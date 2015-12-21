/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "NWK_INTERFACE/Include/protocol_interface_driver.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "platform/arm_hal_interrupt.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "common_functions.h"
#include "randLIB.h"
#include "platform/arm_hal_phy.h"
#include "platform/arm_hal_interrupt.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#ifndef NO_TLS
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "net_pana_parameters_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#else
#define sec_timer_handle()
#endif
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/RPL/rpl_definition.h"
#ifdef HAVE_RPL
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#endif
#include "NAP/nap_config.h"
#include "libDHCPv6/libDHCPv6.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#ifdef IPV6_STACK
#include "ipv6_stack/protocol_ipv6.h"
#endif
#ifdef WHITEBOARD
#ifdef NVM_BORDER_ROUTER
#include "br_list_nvm_api.h"
#endif
#include "Service_Libs/whiteboard/whiteboard.h"
#endif

#include "MulticastTrigle/multicast.h"

static protocol_interface_info_entry_t stub_protocol_interface_info_entry;

#define TRACE_GROUP_CORE "core"

#ifndef SEC_LIB_X_100MS_COUNTER
#define SEC_LIB_X_100MS_COUNTER 1
#endif
uint32_t protocol_core_monotonic_time;

typedef struct {
    uint8_t core_timer_ticks;
    uint8_t core_timer_event;
    uint16_t core_security_ticks_counter;
    uint8_t nd_ttl_timer;
} lowpan_core_timer_structures_s;

void core_timer_event_handle(uint16_t ticksUpdate);

#ifndef IPV6_STACK
#define ipv6_core_timer_event_handle(x,y)
#define ipv6_core_slow_timer_event_handle(x)
#endif

uint32_t event_idle_dummy(uint8_t result, uint32_t s_time)
{
    return 0;
}


int8_t protocol_read_tasklet_id(void)
{
	return -1;
}

uint8_t check_power_state(uint8_t mode)
{
    return 0;
}


void set_power_state(uint8_t mode)
{
}

void clear_power_state(uint8_t mode)
{
}

protocol_interface_info_entry_t *nwk_interface_get_pointer_by_timer_id(int8_t id, arm_nwk_timer_id_e type)
{
    return NULL;
}

void timer_ifs_interrupt(int8_t timer_id, uint16_t slots)
{
}

void protocol_root_tasklet(arm_event_s *event)
{
}
void protocol_core_security_tick_update(uint16_t tick_update)
{
}

void nwk_bootsrap_timer(protocol_interface_info_entry_t *cur)
{
}

uint8_t nwk_border_router_active(nwk_interface_id nwk_id)
{
    return 0;
}

void core_timer_event_handle(uint16_t ticksUpdate)
{
}

void protocol_core_cb(uint16_t ticksUpdate)
{
}


void protocol_core_init(void)
{
}

void protocol_core_interface_info_reset(protocol_interface_info_entry_t *entry)
{
}

void bootsrap_next_state_kick(icmp_state_t new_state, protocol_interface_info_entry_t *cur)
{
}

uint32_t protocol_stack_interface_set_reachable_time(protocol_interface_info_entry_t *cur, uint32_t base_reachable_time)
{
    return 0;
}
protocol_interface_info_entry_t *nwk_interface_get_ipv6_ptr(void)
{
    return NULL;
}

void nwk_interface_print_neigh_cache(uint8_t dlevel)
{
}

void nwk_interface_flush_neigh_cache(void)
{
}

void ipv6_prefix_on_link_update(uint8_t *address)
{
}

void ipv6_prefix_on_link_remove(uint8_t *address)
{
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id)
{
    return &stub_protocol_interface_info_entry;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_id(int8_t nwk_id)
{
    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_bootstrap_id(int8_t id)
{
    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_device_id(int8_t nwk_id)
{
    return NULL;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_tun_device_id(int8_t nwk_id)
{
    return NULL;
}



protocol_interface_info_entry_t *protocol_stack_interface_sleep_possibility(void)
{
    return NULL;
}

uint8_t nwk_bootsrap_ready(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int8_t nwk_interface_MAC_MTU_allocate(protocol_interface_info_entry_t *cur)
{
    return 0;
}

protocol_interface_info_entry_t *protocol_stack_interface_generate(nwk_interface_id nwk_id, int8_t driver_id)
{
    return NULL;
}

void nwk_interface_link_layer_key_synch_lost_check(protocol_interface_info_entry_t *cur, uint8_t key_id)
{
}

bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t *addr)
{
    return 0;
}

int nwk_lowpan_routing_check(buffer_t *buf)
{
    return 0;
}

int8_t nwk_interface_solication_address_check(protocol_interface_info_entry_t *cur_interface, const uint8_t address_ptr[__static 16])
{
    return 0;
}

int protocol_stack_interface_check_prefix_extension_support(int8_t nwk_id, uint8_t *prefix)
{
    return -1;
}

void protocol_stack_pointer_clear(nwk_interface_id if_id)
{
}

int8_t protocol_stack_pointer_check(nwk_interface_id if_id)
{
    return 0;
}

uint8_t protocol_too_busy(void)
{
    return 0;
}

void protocol_push(buffer_t *buf)
{
}

void nwk_bootsrap_state_update(arm_nwk_interface_status_type_e posted_event, protocol_interface_info_entry_t *cur)
{
}

void net_bootsrap_cb_run(uint8_t event)
{
}

void protocol_core_dhcpv6_allocated_address_remove(protocol_interface_info_entry_t *cur, uint8_t *guaPrefix)
{
}

int8_t protcol_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    return -1;
}

