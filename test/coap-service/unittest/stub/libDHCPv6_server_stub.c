/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */


#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "libDHCPv6/libDHCPv6_server.h"
#include "libDHCPv6/libDHCPv6.h"
#include "common_functions.h"
#include "platform/ns_debug.h"

void libdhcpv6_gua_servers_time_update(uint32_t timeUpdateInSeconds)
{
}

dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_interfaceid(int8_t interfaceId, const uint8_t *prefixPtr)
{
    return NULL;
}

dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_socketinstance(uint16_t socketInstance, uint8_t *prefixPtr)
{
    return NULL;
}


dhcpv6_gua_server_entry_s *libdhcpv6_gua_server_allocate(uint8_t *prefix, int8_t interfaceId, uint8_t *serverDUID, uint16_t serverDUIDType, dhcp_address_prefer_timeout_cb *prefered_timeout_cb)
{
    return NULL;
}

void libdhcpv6_gua_server_free_by_prefix_and_interfaceid(uint8_t *prefix, int8_t interfaceId)
{
}

void libdhcpv6_gua_server_free_by_interfaceid(int8_t interfaceId)
{
}

dhcpv6_alloacted_address_entry_t *libdhcpv6_address_entry_allocate(uint32_t validLifetime)
{
    return NULL;
}

dhcpv6_alloacted_address_entry_t *libdhcpv6_address_get_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    return NULL;
}

dhcpv6_alloacted_address_entry_t *libdhcpv6_address_allocated_list_scan(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *linkId, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew)
{
    return NULL;
}

