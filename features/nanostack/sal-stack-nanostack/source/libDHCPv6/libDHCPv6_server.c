/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * \file dhcpv6_server_api.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "libDHCPv6/libDHCPv6_server.h"
#include "libDHCPv6/libDHCPv6.h"
#include "common_functions.h"
#include "ns_trace.h"

#ifdef HAVE_DHCPV6_SERVER

static NS_LARGE NS_LIST_DEFINE(dhcpv6_gua_server_list, dhcpv6_gua_server_entry_s, link);

bool libdhcpv6_gua_server_list_empty(void)
{
    return ns_list_is_empty(&dhcpv6_gua_server_list);
}

static dhcpv6_gua_server_entry_s *libdhcpv6_server_entry_allocate(void)
{
    dhcpv6_gua_server_entry_s *entry = ns_dyn_mem_alloc(sizeof(dhcpv6_gua_server_entry_s));
    if (entry) {
        entry->clientIdSequence = 0;
        entry->enableAddressMapping = false;
        entry->enableAddressAutonous = true;
        entry->clientIdDefaultSuffics = 0x0000000;
        entry->maxSuppertedClients = 200;
        entry->validLifetime = 7200;
        ns_list_init(&entry->allocatedAddressList);
    }
    return entry;
}
static void libdhcpv6_address_generate(dhcpv6_gua_server_entry_s *serverInfo, dhcpv6_alloacted_address_entry_t *entry)
{
    //GENERATE ADDRESS
    uint8_t *ptr = entry->nonTemporalAddress;
    memcpy(ptr, serverInfo->guaPrefix, 8);
    ptr += 8;
    if (serverInfo->enableAddressAutonous) {
        if (entry->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE) {
            memcpy(ptr, entry->linkId, 8);
            *ptr ^= 2;
        } else if (entry->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE) {
            *ptr++  = entry->linkId[0] ^ 2;
            *ptr++  = entry->linkId[1];
            *ptr++  = entry->linkId[2];
            *ptr++  = 0xff;
            *ptr++  = 0xfe;
            *ptr++  = entry->linkId[3];
            *ptr++  = entry->linkId[4];
            *ptr    = entry->linkId[5];
        } else {
            ptr = common_write_32_bit((serverInfo->clientIdDefaultSuffics | 02000000), ptr);
            ptr = common_write_32_bit((serverInfo->clientIdSequence + 2), ptr);
        }

    } else {
        ptr = common_write_32_bit((serverInfo->clientIdDefaultSuffics | 02000000), ptr);
        ptr = common_write_32_bit((serverInfo->clientIdSequence + 2), ptr);
    }
    serverInfo->clientIdSequence++;
}


void libdhcpv6_gua_servers_time_update(uint32_t timeUpdateInSeconds)
{
    //Check All allocated server inside this loop
    ns_list_foreach(dhcpv6_gua_server_entry_s, cur, &dhcpv6_gua_server_list) {
        //Check All allocated address in this module
        ns_list_foreach_safe(dhcpv6_alloacted_address_entry_t, address, &cur->allocatedAddressList) {
            //Update
            if (address->preferredLifetime) {
                if (address->preferredLifetime <= timeUpdateInSeconds) {
                    //Stop use this address for leasequery and delete Route or address map

                    address->preferredLifetime = 0;
                    cur->timeoutCb(cur->interfaceId, address->nonTemporalAddress);
                } else {
                    address->preferredLifetime -= timeUpdateInSeconds;
                }
            }

            if (address->lifetime <= timeUpdateInSeconds) {
                ns_list_remove(&cur->allocatedAddressList, address);
                ns_dyn_mem_free(address);
            } else {
                address->lifetime -= timeUpdateInSeconds;
            }
        }
    }
}

dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_interfaceid(int8_t interfaceId, const uint8_t *prefixPtr)
{
    ns_list_foreach(dhcpv6_gua_server_entry_s, cur, &dhcpv6_gua_server_list) {
        if (cur->interfaceId == interfaceId) {
            if (memcmp(cur->guaPrefix, prefixPtr, 8) == 0) {
                return cur;
            }
        }
    }
    return NULL;
}

dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_socketinstance(uint16_t socketInstance, uint8_t *prefixPtr)
{
    ns_list_foreach(dhcpv6_gua_server_entry_s, cur, &dhcpv6_gua_server_list) {
        if (cur->socketInstance_id == socketInstance) {
            if (memcmp(cur->guaPrefix, prefixPtr, 8) == 0) {
                return cur;
            }
        }
    }
    return NULL;
}


dhcpv6_gua_server_entry_s *libdhcpv6_gua_server_allocate(uint8_t *prefix, int8_t interfaceId, uint8_t *serverDUID, uint16_t serverDUIDType, dhcp_address_prefer_timeout_cb *prefered_timeout_cb)
{
    dhcpv6_gua_server_entry_s *entry = NULL;
    if (libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefix) == NULL) {
        entry = libdhcpv6_server_entry_allocate();
        if (entry) {
            memcpy(entry->guaPrefix, prefix, 8);
            memcpy(entry->serverDUID, serverDUID, 8);
            entry->serverLinkType = serverDUIDType;
            entry->interfaceId = interfaceId;
            entry->timeoutCb = prefered_timeout_cb;
            ns_list_add_to_end(&dhcpv6_gua_server_list, entry);
        }
    }
    return entry;
}

void libdhcpv6_gua_server_free_by_prefix_and_interfaceid(uint8_t *prefix, int8_t interfaceId)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefix);
    if (serverInfo) {
        if ((serverInfo->interfaceId == interfaceId) && (memcmp(serverInfo->guaPrefix, prefix, 8) == 0)) {
            ns_list_foreach_safe(dhcpv6_alloacted_address_entry_t, cur, &serverInfo->allocatedAddressList) {
                ns_list_remove(&serverInfo->allocatedAddressList, cur);
                ns_dyn_mem_free(cur);
            }
            ns_list_remove(&dhcpv6_gua_server_list, serverInfo);
            ns_dyn_mem_free(serverInfo);
        }
    }
}

static dhcpv6_alloacted_address_entry_t *libdhcpv6_address_entry_allocate(uint32_t validLifetime)
{
    dhcpv6_alloacted_address_entry_t *entry = ns_dyn_mem_alloc(sizeof(dhcpv6_alloacted_address_entry_t));
    if (entry) {
        if (validLifetime != 0xffffffff) {
            entry->lifetime = validLifetime;
            entry->preferredLifetime = (validLifetime >> 1);
        } else {
            entry->lifetime = 0xffffffff;
            entry->preferredLifetime = 0xffffffff;
        }
    }
    return entry;
}

dhcpv6_alloacted_address_entry_t *libdhcpv6_address_get_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    ns_list_foreach(dhcpv6_alloacted_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (memcmp(cur->nonTemporalAddress, address, 16) == 0) {
            return cur;
        }
    }
    return NULL;
}

void libdhcpv6_address_rm_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    ns_list_foreach_safe(dhcpv6_alloacted_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (memcmp(cur->nonTemporalAddress, address, 16) == 0) {
            ns_list_remove(&serverInfo->allocatedAddressList, cur);
            ns_dyn_mem_free(cur);
            return;
        }
    }
}

dhcpv6_alloacted_address_entry_t *libdhcpv6_address_allocated_list_scan(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *linkId, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew)
{
    dhcpv6_alloacted_address_entry_t *newEntry = NULL;
    uint16_t duiLength = 6;
    if (linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE) {
        duiLength = 8;
    }
    ns_list_foreach(dhcpv6_alloacted_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (cur->linkType == linkType) {
            if (memcmp(cur->linkId, linkId, duiLength) == 0) {
                cur->iaID = iaID;
                if (serverInfo->validLifetime != 0xffffffff) {
                    cur->lifetime = serverInfo->validLifetime ;
                    cur->preferredLifetime = (serverInfo->validLifetime  >> 1);
                } else {
                    cur->lifetime = 0xffffffff;
                    cur->preferredLifetime = 0xffffffff;
                }
                return cur;
            }
        }
    }
    if (allocateNew) {
        if (ns_list_count(&serverInfo->allocatedAddressList) < serverInfo->maxSuppertedClients) {
            newEntry = libdhcpv6_address_entry_allocate(serverInfo->validLifetime);
            if (newEntry) {
                memcpy(newEntry->linkId, linkId, duiLength);
                newEntry->linkType = linkType;
                newEntry->iaID = iaID;
                newEntry->T0 = T0;
                newEntry->T1 = T1;
                libdhcpv6_address_generate(serverInfo, newEntry);
                ns_list_add_to_end(&serverInfo->allocatedAddressList, newEntry);
            }
        }
    }

    return newEntry;
}

#endif

