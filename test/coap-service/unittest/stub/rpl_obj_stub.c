/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include <string.h>
#include "ns_types.h"

#ifdef HAVE_RPL
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ns_trace.h"
#include "ip6string.h"
#include "Core/include/routing_table.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#include "net_rpl.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "randLIB.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#ifdef IPV6_STACK
#include "Common_Protocols/ipv6.h"
#endif
#ifdef NVM_BORDER_ROUTER
#include "br_list_nvm_api.h"
#endif

#include "common_functions.h"

#define TRACE_GROUP_RPL_OBJ "rplo"

const uint8_t ADDR_LINK_LOCAL_ALL_RPL_NODES[16] = { 0xff, 0x02, [15] = 0x1a };

void rpl_dodag_base_init(rpl_dodag_t *new_entry);

int8_t rpl_prefix_update(uint8_t *dptr, rpl_dodag_t *rpl_object);
static void rpl_route_list_free(route_list_t *list);
void rpl_object_release(rpl_dodag_t *router_object);
void rpl_router_object_reset(rpl_dodag_t *router_object);
//uint8_t rpl_metric_parse(buffer_t *buf);
void rpl_dodag_config_update(uint8_t *dptr, dodag_config_t *dodag_conf);
rpl_dodag_t *icmp_rpl_router_object_get(uint8_t *dodag_id, uint8_t instance_id, uint16_t rank);
rpl_dodag_t *icmp_rpl_router_object_get_by_intance(uint8_t instance_id);
uint8_t rpl_dis_build(rpl_dodag_t *cur);
uint8_t rpl_dio_tx(rpl_dodag_t *cur, uint8_t *address , protocol_interface_info_entry_t *cur_interface);
uint8_t rpl_dao_tx(rpl_dodag_t *cur, protocol_interface_info_entry_t *cur_interface);
void rpl_set_trigle_timer(rpl_dodag_t *cur , uint8_t active);
void rpl_unicast_response_allocate(rpl_dodag_t *router, uint8_t *address);
void rpl_object_print(void);
void rpl_obj_update_rank_hop(rpl_dodag_t *temp);
int8_t rpl_route_prefix_update(uint8_t len, const uint8_t *dptr, uint8_t instance_id);
void rpl_route_prefix_remove_by_rpl_instance(uint8_t id);
rpl_dodag_t *rpl_root_get_by_intance_id(uint8_t instace_id);

#ifdef NVM_BORDER_ROUTER

uint16_t (*rpl_nvm_storage_cb)(br_rpl_nvm_update_process_t) = 0;

int8_t rpl_nvm_callback_set(uint16_t (*passed_fptr)(br_rpl_nvm_update_process_t), uint8_t *nvm_static_buffer)
{
    return -1;
}

int8_t rpl_nvm_base_reload(const uint8_t *nvm_data, int8_t interface_id)
{
    return -1;
}

int8_t rpl_prefix_store_from_nvm(const uint8_t *nvm_data, uint8_t instance_id)
{
    return -1;
}

int8_t rpl_route_store_from_nvm(const uint8_t *nvm_data, uint8_t instance_id)
{
    return -1;
}



#endif

int8_t rpl_root_dodag_poiso_ready(uint8_t instace_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig(int8_t nwk_interface_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_version_increment(int8_t nwk_interface_id)
{
    return -1;
}

uint8_t rpl_routers_active(void)
{
    return 0;
}

int8_t rpl_root_active(rpl_dodag_t *compare_ptr)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_start(int8_t interface_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_init(int8_t interface_id, const uint8_t *dodag_id, const dodag_config_t *config, uint8_t instace_id, uint8_t flags)
{
    return -1;
}

int8_t rpl_root_setup_refresh(uint8_t instance_id)
{
    return -1;
}

rpl_dodag_t *rpl_root_get_by_intance_id(uint8_t instance_id)
{
    return NULL;
}

int8_t arm_nwk_6lowpan_rpl_dodag_poison(int8_t nwk_interface_id)
{
    return -1;
}

int8_t arm_nwk_6lowpan_rpl_dodag_remove(int8_t interface_id)
{
    return -1;
}


void rpl_routers_init(void)
{
}

void rpl_obj_change_parent(rpl_dodag_t *temp)
{
}

void rpl_obj_update_rank_hop(rpl_dodag_t *temp)
{
}


void rpl_obj_dao_trig(rpl_dodag_t *cur)
{
}

void rpl_object_release(rpl_dodag_t *router_object)
{
}

void rpl_router_object_reset(rpl_dodag_t *router_object)
{
}

rpl_dodag_t *icmp_rpl_router_object_get_by_intance(uint8_t instance_id)
{
    return NULL;
}

void rpl_dodag_base_init(rpl_dodag_t *new_entry)
{
}

rpl_dodag_t *icmp_rpl_router_object_get(uint8_t *dodag_id, uint8_t instance_id, uint16_t rank)
{
    return NULL;
}

void rpl_unicast_response_allocate(rpl_dodag_t *router, uint8_t *address)
{
}

void rpl_dis_handler(buffer_t *buf, bool multicast_dst)
{
}

buffer_t *rpl_dao_parse(buffer_t *buf)
{
    return NULL;
}

void rpl_dao_ack_parse(uint8_t *dptr, uint16_t length, nwk_interface_id nwk_id)
{
}

void rpl_dio_parse(buffer_t *buf)
{
}

int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update(int8_t nwk_interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime)
{
    return -1;
}

int8_t rpl_prefix_update(uint8_t *dptr, rpl_dodag_t *rpl_object)
{
    return -1;
}


int8_t arm_nwk_6lowpan_rpl_dodag_route_update(int8_t nwk_interface_id, uint8_t *route_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime)
{
    return -1;
}

rpl_dodag_t *rpl_route_longest_matches_prefix(const uint8_t *address)
{
    return NULL;
}

void rpl_route_prefix_remove_by_rpl_instance(uint8_t id)
{
}

int8_t rpl_route_prefix_update(uint8_t len, const uint8_t *dptr, uint8_t instance_id)
{
    return -1;
}

route_entry_t *rpl_root_route_update(const uint8_t *dptr, rpl_dodag_proxy_t *rpl_object)
{
    return NULL;
}

void rpl_dodag_config_update(uint8_t *dptr, dodag_config_t *dodag_conf)
{
}

uint8_t rpl_dodag_object_heared(void)
{
    return 0;
}

void rpl_dodag_root_dao_repair(void)
{
}


rpl_dodag_t *rpl_dodag_root_get(nwk_interface_id nwk_id)
{
    return NULL;
}

uint8_t rpl_dodag_cnt(void)
{
    return 0;
}

void rpl_dodag_dao_external_timer_trig(void)
{
}

void rpl_router_prefix_ttl_update(rpl_dodag_t *router_object)
{
}

void rpl_route_prefix_route_ttl_update(uint8_t intance_id)
{

}

uint16_t rpl_route_prefix_route_length(uint8_t intance_id)
{
    return 0;
}

uint8_t *rpl_route_prefix_route_set(uint8_t *dptr , uint8_t intance_id)
{
    return NULL;
}

void rpl_object_timer_second_update(nwk_interface_id nwk_id)
{

}

void rpl_object_timer(uint8_t ticks, protocol_interface_info_entry_t *cur_interface)
{

}

uint8_t rpl_object_poisons(void)
{
    return 0;
}

void rpl_object_timer_balance(uint16_t sleep_time)
{

}

uint8_t rpl_dis_build(rpl_dodag_t *cur)
{
    return 0;
}


uint8_t rpl_dao_tx(rpl_dodag_t *cur, protocol_interface_info_entry_t *cur_interface)
{
    return 0;
}

uint8_t rpl_poison_req(rpl_dodag_t *cur)
{
    return 0;
}

uint8_t rpl_host_dao_tx(rpl_dodag_t *cur, uint8_t *adr_ptr, uint8_t short_address_type)
{
    return 0;
}

void rpl_set_trigle_timer(rpl_dodag_t *cur , uint8_t active)
{
}

//extern void get_suffics(uint8_t *ptr);
uint8_t rpl_dio_tx(rpl_dodag_t *cur, uint8_t *address , protocol_interface_info_entry_t *cur_interface)
{
    return 0;
}


uint8_t rpl_routeloop_check(rpl_loopdetect_t *loop)
{
    return 0;
}

rpl_dodag_t *rpl_route(const uint8_t *addr, uint8_t id/*, route_info_entry_t * response*/)
{
    return NULL;
}

rpl_dodag_t *rpl_route_by_prefix(const uint8_t *addr, bool root_is_valid)
{
    return NULL;
}

uint8_t rpl_prefix_adr_valid_check(uint8_t *addr)
{
    return 0;
}

uint8_t rpl_get_router_address(uint8_t *addr)
{
    return 0;
}

uint8_t rpl_compare_parent(uint8_t *addr, rpl_dodag_t **response)
{
    return 0;
}

void rpl_parent_remove(uint8_t *addr, protocol_interface_info_entry_t *cur_interface)
{
}

void rpl_object_print(void)
{
}

uint8_t rpl_instance_list_read(uint8_t *cache_ptr, uint8_t cache_size)
{
    return 0;
}

uint8_t rpl_read_dodag_info(rpl_dodag_info_t *dodag_ptr, uint8_t instance_id)
{
    return 0;
}

bool rpl_root_rank(rpl_dodag_t *cur)
{
    return false;
}

#endif
