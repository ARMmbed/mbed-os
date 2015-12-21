/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "string.h"
#include "Core/include/address.h"
#include "Core/include/buffer.h"
#include "Core/include/socket.h"
#include "Core/include/routing_table.h"
#include "nsdynmemLIB.h"
#include "Common_Protocols/ipv6.h"
#ifdef NVM_BORDER_ROUTER
#include "br_list_nvm_api.h"
#endif
#include "common_functions.h"

#define TRACE_GROUP_ROUTING_TABLE "crou"

#if ADDRESS_SIZE == 8
#if MAX_HOP_CNT > 30
#error "DO not set MAX_HOP_CNT over 30 reason by 8-bit size resolution"
#endif
#endif

source_route_t *get_generated_src_route(void)
{
    return NULL;
}

#ifdef NVM_BORDER_ROUTER
uint16_t (*route_nvm_storage_cb)(br_nvm_update_process_t) = 0;

int8_t route_nvm_callback_set(uint16_t (*passed_fptr)(br_nvm_update_process_t), uint8_t *nvm_static_buffer)
{
    return -1;
}

int8_t route_entry_store_from_nvm(const uint8_t *nvm_data)
{
    return -1;
}

#endif
void route_nvm_update(route_table_entry_t *entry)
{
}

void route_nvm_remove(route_table_entry_t *entry)
{
}

void gen_route_del(route_table_entry_t *cur)
{
}

uint8_t gen_source_route(route_info_entry_t *info)
{
    return 0;
}

uint8_t *gen_source_route_set(uint8_t *ptr)
{
    return NULL;
}

route_table_entry_t *gen_route_entry_get(const uint8_t address[16])
{
    return NULL;
}

route_table_entry_t *gen_route_entry_get_by_iid(const uint8_t address[8])
{
    return NULL;
}

void gen_routing_table_init(void)
{
}

route_table_entry_t *gen_table_update(const uint8_t addr[16])
{
    return NULL;
}


int8_t gen_source_route_allocate(uint8_t alloc)
{
    return -1;
}



int8_t gen_route_exist_check(const uint8_t ipv6_address_ptr[16])
{
    return -1;
}



void gen_route_detect(route_info_entry_t *info, uint8_t forward_info)
{
}


void gen_routing_ttl_update(uint8_t id)
{
}

void gen_routing_ttl_set_low_by_repair(uint8_t id)
{
}

void gen_route_remove_by_rpl_instance(uint8_t id)
{
}


void gen_neighbor_ttl_update(uint8_t ttl)
{
}

void routing_table_print(void)
{
}


