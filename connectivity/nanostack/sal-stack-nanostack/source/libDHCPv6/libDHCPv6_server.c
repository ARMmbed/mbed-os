/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
    uint8_t *server_duid_ptr = ns_dyn_mem_alloc(16);// Allocate 128-bit DUID-UUID by default it cover DUID-LL and DUID-LLTP also
    if (!entry || !server_duid_ptr) {
        ns_dyn_mem_free(entry);
        ns_dyn_mem_free(server_duid_ptr);
        return NULL;
    }
    entry->serverDynamic_DUID = server_duid_ptr;
    entry->serverDynamic_DUID_length = 16;
    entry->firstFreedId = 0;
    entry->firstUnusedId = DHCP_ADDRESS_ID_START;
    entry->enableAddressAutonous = true;
    entry->disableAddressListAllocation = false;
    entry->maxSupportedClients = 200;
    entry->validLifetime = 7200;
    entry->removeCb = NULL;
    entry->addCb = NULL;
    ns_list_init(&entry->allocatedAddressList);
    return entry;
}

static uint16_t libdhcpv6_get_next_freed_id(dhcpv6_gua_server_entry_s *serverInfo)
{
    uint16_t last_allocated_id = DHCP_ADDRESS_ID_START - 1;
    ns_list_foreach(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {

        if (last_allocated_id + 1 == cur->allocatedID) {
            //Last and current plus 1 so normal order
            last_allocated_id = cur->allocatedID;
            continue;
        }

        if ((cur->allocatedID - last_allocated_id) == 2) {
            //one missing sequence between last and current
            if (last_allocated_id + 1 == serverInfo->firstFreedId) {
                //Skip Current freedID this will update after this call to new one
                last_allocated_id = cur->allocatedID;
                continue;
            }
        } else if (last_allocated_id + 1 == serverInfo->firstFreedId) {
            //Skip first if it is last freedId
            return last_allocated_id + 2;
        }

        return last_allocated_id + 1;
    }
    //No more freed ID so return 0
    return 0;
}

static uint16_t libdhcpv6_address_id_allocate(dhcpv6_gua_server_entry_s *serverInfo)
{
    uint16_t address_id;
    if (serverInfo->firstFreedId) {
        address_id = serverInfo->firstFreedId;
        //Discover next free freed possible value
        serverInfo->firstFreedId = libdhcpv6_get_next_freed_id(serverInfo);
    } else {
        //Allocated new ID
        address_id = serverInfo->firstUnusedId++;
    }
    return address_id;
}

static void libdhcpv6_gen_suffics_from_eui48(uint8_t *ptr, uint8_t *eui48)
{
    *ptr++  = *eui48++ ^ 2;
    *ptr++  = *eui48++;
    *ptr++  = *eui48++;
    *ptr++  = 0xff;
    *ptr++  = 0xfe;
    *ptr++  = *eui48++;
    *ptr++  = *eui48++;
    *ptr    = *eui48++;
}

static void libdhcpv6_gen_suffics_from_allocated_id(uint8_t *ptr, uint8_t *server_unique_48_bit_id, uint16_t allocated_id)
{
    memcpy(ptr, server_unique_48_bit_id, 6);
    common_write_16_bit(allocated_id, ptr + 6);
}


static uint16_t libdhcpv6_address_generate(dhcpv6_gua_server_entry_s *serverInfo, dhcpv6_allocated_address_t *entry)
{
    //GENERATE ADDRESS
    uint8_t *ptr = entry->nonTemporalAddress;
    memcpy(ptr, serverInfo->guaPrefix, 8);
    ptr += 8;
    if (serverInfo->enableAddressAutonous) {
        if (entry->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE ||
                entry->linkType == DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) {
            memcpy(ptr, entry->linkId, 8);
            *ptr ^= 2;
            return 0;
        }

        if (entry->linkType == DHCPV6_DUID_HARDWARE_EUI48_TYPE) {
            libdhcpv6_gen_suffics_from_eui48(ptr, entry->linkId);
            return 0;
        }
    }

    uint16_t allocated_id = libdhcpv6_address_id_allocate(serverInfo);
    libdhcpv6_gen_suffics_from_allocated_id(ptr, serverInfo->clientIdDefaultSuffics, allocated_id);
    return allocated_id;
}

static void libdhcpv6_address_free(dhcpv6_gua_server_entry_s *server_info, dhcpv6_allocated_address_entry_t *entry)
{
    ns_list_remove(&server_info->allocatedAddressList, entry);
    if (!server_info->enableAddressAutonous) {
        if (entry->allocatedID + 1 == server_info->firstUnusedId) {
            server_info->firstUnusedId--;
        } else if (server_info->firstFreedId == 0 || server_info->firstFreedId > entry->allocatedID) {
            server_info->firstFreedId = entry->allocatedID;
        }
    }
    ns_dyn_mem_free(entry);
}

void libdhcpv6_allocated_address_write(uint8_t *ptr, dhcpv6_allocated_address_entry_t *address, dhcpv6_gua_server_entry_s *serverInfo)
{
    memcpy(ptr, serverInfo->guaPrefix, 8);
    ptr += 8;
    if (serverInfo->enableAddressAutonous) {
        //Generate address from link layer address
        if (address->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE ||
                address->linkType == DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) {
            memcpy(ptr, address->linkId, 8);
            *ptr ^= 2;
            return;
        } else if (address->linkType == DHCPV6_DUID_HARDWARE_EUI48_TYPE) {
            libdhcpv6_gen_suffics_from_eui48(ptr, address->linkId);
            return;
        }
    }
    //Generate from 16-bit allocate and default suffic's
    libdhcpv6_gen_suffics_from_allocated_id(ptr, serverInfo->clientIdDefaultSuffics, address->allocatedID);
}

static bool libdhcpv6_address_suffics_compare(const uint8_t *suffics, dhcpv6_allocated_address_entry_t *address, dhcpv6_gua_server_entry_s *serverInfo)
{
    uint8_t allocated_suffics[8];
    if (serverInfo->enableAddressAutonous) {
        //Generate address from link layer address
        if (address->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE ||
                address->linkType == DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) {
            memcpy(allocated_suffics, address->linkId, 8);
            allocated_suffics[0] ^= 2;
            goto compare_suffics;
        } else if (address->linkType == DHCPV6_DUID_HARDWARE_EUI48_TYPE) {
            libdhcpv6_gen_suffics_from_eui48(allocated_suffics, address->linkId);
            goto compare_suffics;
        }
    }
    //Generate from 16-bit allocate and default suffic's
    libdhcpv6_gen_suffics_from_allocated_id(allocated_suffics, serverInfo->clientIdDefaultSuffics, address->allocatedID);

compare_suffics:
    if (memcmp(allocated_suffics, suffics, 8)) {
        return false;
    }

    return true;

}


void libdhcpv6_gua_servers_time_update(uint32_t timeUpdateInSeconds)
{
    //Check All allocated server inside this loop
    ns_list_foreach(dhcpv6_gua_server_entry_s, cur, &dhcpv6_gua_server_list) {
        //Check All allocated address in this module
        ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, address, &cur->allocatedAddressList) {
            //Update
            if (address->preferredLifetime) {
                if (address->preferredLifetime <= timeUpdateInSeconds) {
                    //Stop use this address for leasequery and delete Route or address map
                    address->preferredLifetime = 0;
                    if (cur->removeCb) {
                        uint8_t allocated_address[16];
                        libdhcpv6_allocated_address_write(allocated_address, address, cur);
                        cur->removeCb(cur->interfaceId, allocated_address, cur->guaPrefix);
                    }
                } else {
                    address->preferredLifetime -= timeUpdateInSeconds;
                }
            }

            if (address->lifetime <= timeUpdateInSeconds) {
                libdhcpv6_address_free(cur, address);
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

            if (!prefixPtr || memcmp(cur->guaPrefix, prefixPtr, 8) == 0) {
                return cur;
            }
        }
    }
    return NULL;
}

int libdhcpv6_server_duid_set(dhcpv6_gua_server_entry_s *server_info, uint8_t *duid_ptr, uint16_t duid_type, uint8_t duid_length)
{
    //Allocate dynamically new Server DUID if needed
    if (duid_length > server_info->serverDynamic_DUID_length) {
        //Allocate dynamic new bigger
        uint8_t *new_ptr = ns_dyn_mem_alloc(duid_length);
        if (!new_ptr) {
            return -1;
        }
        server_info->serverDynamic_DUID_length = duid_length;
        ns_dyn_mem_free(server_info->serverDynamic_DUID);
        server_info->serverDynamic_DUID = new_ptr;
    }
    //SET DUID
    server_info->serverDUID.duid = server_info->serverDynamic_DUID;
    memcpy(server_info->serverDUID.duid, duid_ptr, duid_length);
    server_info->serverDUID.duid_length = duid_length;
    server_info->serverDUID.type = duid_type;
    return 0;
}


dhcpv6_gua_server_entry_s *libdhcpv6_gua_server_allocate(uint8_t *prefix, int8_t interfaceId, uint8_t *serverDUID, uint16_t serverDUIDType)
{


    dhcpv6_gua_server_entry_s *entry = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefix);
    if (entry == NULL) {
        entry = libdhcpv6_server_entry_allocate();
        if (!entry) {
            return NULL;
        }
        //Generate Server DUID-LL by default
        uint8_t *ptr;
        uint8_t duid_ll[16];
        uint8_t duid_length;
        ptr = duid_ll;
        duid_length = libdhcpv6_duid_linktype_size(serverDUIDType) + 2;
        ptr = common_write_16_bit(serverDUIDType, ptr);
        memcpy(ptr, serverDUID, libdhcpv6_duid_linktype_size(serverDUIDType));
        //SET Defaultsuffics
        if (libdhcpv6_duid_linktype_size(serverDUIDType) == 8) {
            memcpy(entry->clientIdDefaultSuffics, serverDUID, 3);
            memcpy(entry->clientIdDefaultSuffics + 3, serverDUID + 5, 3);
        } else {
            memcpy(entry->clientIdDefaultSuffics, serverDUID, 6);
        }

        entry->clientIdDefaultSuffics[0] ^= 0x02;

        //SET DUID
        if (libdhcpv6_server_duid_set(entry, duid_ll, DHCPV6_DUID_LINK_LAYER_TYPE, duid_length) != 0) {
            ns_dyn_mem_free(entry->serverDynamic_DUID);
            ns_dyn_mem_free(entry);
            return NULL;
        }


        memcpy(entry->guaPrefix, prefix, 8);
        entry->interfaceId = interfaceId;
        ns_list_add_to_end(&dhcpv6_gua_server_list, entry);
    }
    return entry;
}

void libdhcpv6_gua_server_free_by_prefix_and_interfaceid(uint8_t *prefix, int8_t interfaceId)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefix);
    if (serverInfo) {
        if ((serverInfo->interfaceId == interfaceId) && (memcmp(serverInfo->guaPrefix, prefix, 8) == 0)) {
            ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
                ns_list_remove(&serverInfo->allocatedAddressList, cur);
                ns_dyn_mem_free(cur);
            }
            ns_list_remove(&dhcpv6_gua_server_list, serverInfo);
            ns_dyn_mem_free(serverInfo->serverDynamic_DUID);
            ns_dyn_mem_free(serverInfo);
        }
    }
}

static void libdhcpv6_address_entry_lifetime_set(dhcpv6_allocated_address_entry_t *entry, uint32_t validLifetime)
{
    if (validLifetime != 0xffffffff) {
        entry->lifetime = validLifetime;
        entry->preferredLifetime = (validLifetime >> 1);
    } else {
        entry->lifetime = 0xffffffff;
        entry->preferredLifetime = 0xffffffff;
    }
}

static void libdhcpv6_copy_allocated_entry_to_temp(dhcpv6_allocated_address_entry_t *cur, dhcpv6_allocated_address_t *address, dhcpv6_gua_server_entry_s *serverInfo)
{
    libdhcpv6_allocated_address_write(address->nonTemporalAddress, cur, serverInfo);
    memcpy(address->linkId, cur->linkId, 8);
    address->T0 = cur->T0;
    address->T1 = cur->T1;
    address->iaID = cur->iaID;
    address->lifetime = cur->lifetime;
    address->preferredLifetime = cur->preferredLifetime;
    address->linkType = cur->linkType;
}


static void libdhcpv6_copy_temp_to_allocated_entry(dhcpv6_allocated_address_entry_t *cur, dhcpv6_allocated_address_t *address, uint16_t allocated_id)
{
    memcpy(cur->linkId, address->linkId, 8);
    cur->allocatedID = allocated_id;
    cur->T0 = address->T0;
    cur->T1 = address->T1;
    cur->iaID = address->iaID;
    cur->lifetime = address->lifetime;
    cur->preferredLifetime = address->preferredLifetime;
    cur->linkType = address->linkType;
}

dhcpv6_allocated_address_t *libdhcpv6_address_get_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    if (memcmp(serverInfo->guaPrefix, address, 8)) {
        return NULL;
    }

    ns_list_foreach(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (libdhcpv6_address_suffics_compare(address + 8, cur, serverInfo) == 0) {
            libdhcpv6_copy_allocated_entry_to_temp(cur, &serverInfo->tempAddressEntry, serverInfo);
            return &serverInfo->tempAddressEntry;
        }
    }
    return NULL;
}

void libdhcpv6_address_rm_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    if (memcmp(serverInfo->guaPrefix, address, 8)) {
        return;
    }

    ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (libdhcpv6_address_suffics_compare(address + 8, cur, serverInfo) == 0) {
            libdhcpv6_address_free(serverInfo, cur);
            return;
        }
    }
}


static void libdhcpv6_address_id_add_to_list(dhcpv6_gua_server_entry_s *serverInfo, dhcpv6_allocated_address_entry_t *allocated)
{
    if (serverInfo->firstUnusedId != allocated->allocatedID + 1) {
        ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
            if (cur->allocatedID > allocated->allocatedID) {
                //Add before new allocated
                if (cur->link.prev) {
                    ns_list_add_before(&serverInfo->allocatedAddressList, cur, allocated);
                } else {
                    //New first
                    ns_list_add_to_start(&serverInfo->allocatedAddressList, allocated);
                }
                return;
            }
        }
    }
    ns_list_add_to_end(&serverInfo->allocatedAddressList, allocated);
}


dhcpv6_allocated_address_t *libdhcpv6_address_allocated_list_scan(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *linkId, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew)
{
    dhcpv6_allocated_address_t *newEntry = NULL;
    dhcpv6_allocated_address_entry_t *allocatedEntry = NULL;
    uint16_t duiLength = 6;
    if (linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE ||
            linkType == DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) {
        duiLength = 8;
    }

    if (serverInfo->enableAddressAutonous && serverInfo->disableAddressListAllocation) {
        //Accept allways when autonous
        newEntry = &serverInfo->tempAddressEntry;
        allocateNew = false;
    }

    ns_list_foreach(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (cur->linkType == linkType) {
            if (memcmp(cur->linkId, linkId, duiLength) == 0) {
                cur->iaID = iaID;
                libdhcpv6_address_entry_lifetime_set(cur, serverInfo->validLifetime);
                libdhcpv6_copy_allocated_entry_to_temp(cur, &serverInfo->tempAddressEntry, serverInfo);
                return &serverInfo->tempAddressEntry;
            }
        }
    }
    if (allocateNew) {
        if (ns_list_count(&serverInfo->allocatedAddressList) < serverInfo->maxSupportedClients) {
            allocatedEntry = ns_dyn_mem_alloc(sizeof(dhcpv6_allocated_address_entry_t));
            if (allocatedEntry) {
                newEntry = &serverInfo->tempAddressEntry;
            }
        }
    }
    if (newEntry) {

        if (serverInfo->validLifetime != 0xffffffff) {
            newEntry->lifetime = serverInfo->validLifetime;
            newEntry->preferredLifetime = (serverInfo->validLifetime >> 1);
        } else {
            newEntry->lifetime = 0xffffffff;
            newEntry->preferredLifetime = 0xffffffff;
        }
        memcpy(newEntry->linkId, linkId, duiLength);
        newEntry->linkType = linkType;
        newEntry->iaID = iaID;
        newEntry->T0 = T0;
        newEntry->T1 = T1;
        uint16_t allocated_id = libdhcpv6_address_generate(serverInfo, newEntry);
        if (!serverInfo->disableAddressListAllocation) {
            libdhcpv6_copy_temp_to_allocated_entry(allocatedEntry, newEntry, allocated_id);
            if (serverInfo->enableAddressAutonous) {
                ns_list_add_to_end(&serverInfo->allocatedAddressList, allocatedEntry);
            } else {
                //Add to list to proper order
                libdhcpv6_address_id_add_to_list(serverInfo, allocatedEntry);
            }
        }
    }

    return newEntry;
}

#endif

