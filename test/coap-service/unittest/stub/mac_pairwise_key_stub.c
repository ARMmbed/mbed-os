/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */


#include "config.h"
#include <string.h>
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "MAC/IEEE802_15_4/mac_pairwise_key.h"

#define TRACE_GROUP "mPKe"

typedef NS_LIST_HEAD(mac_pairwise_key_entry_t, link) mac_pairwise_key_list_t;

typedef struct mac_pairwise_interface_entry {
    int8_t interfaceId;
    mac_pairwise_key_list_t keyList;
    ns_list_link_t link; /*!< List link entry */
} mac_pairwise_interface_entry_t;

mac_pairwise_interface_entry_t *mac_pairwise_key_list_allocate(int8_t interfaceId)
{
    return NULL;
}

mac_pairwise_key_entry_t *mac_pairwise_key_entry_allocate(const uint8_t *eui64)
{
    return NULL;
}

mac_pairwise_key_entry_t *mac_pairwise_key_entry_get(mac_pairwise_key_list_t *list, const uint8_t *eui64, bool Allocate)
{
    return NULL;
}

mac_pairwise_interface_entry_t *mac_pairwise_key_main_class(int8_t interfaceId, bool Allocate)
{
    return NULL;
}



int mac_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, const uint8_t eui64[static 8], const uint8_t key[static 16])
{
    return 0;

}

int mac_pairwise_key_del(int8_t interface_id, const uint8_t eui64[static 8])
{
    return 0;
}


int mac_pairwise_key_flush_list(int8_t interface_id)
{
    return 0;
}


mac_pairwise_key_entry_t *mac_pairwise_key_get_key(int8_t interface_id, const uint8_t *address, uint16_t panId, uint8_t addressType)
{
    return NULL;
}

bool mac_pairwise_key_framecounter_validation(mac_pairwise_key_entry_t *keyEntry, uint32_t frameCounter)
{
    return false;
}

void mac_pairwise_key_framecounter_update(mac_pairwise_key_entry_t *keyEntry, uint32_t frameCounter)
{
}


