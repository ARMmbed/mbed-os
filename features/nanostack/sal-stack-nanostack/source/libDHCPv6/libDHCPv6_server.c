/*
 * Copyright (c) 2014-2021, Pelion and affiliates.
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

#define TRACE_GROUP "dhcp"

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
    entry->firstUnusedId = DHCP_ADDRESS_ID_START;
    entry->anonymousAddress = false;
    entry->disableAddressList = false;
    entry->maxSupportedClients = 200;
    entry->validLifetime = 7200;
    entry->removeCb = NULL;
    entry->addCb = NULL;
    ns_list_init(&entry->allocatedAddressList);
    ns_list_init(&entry->dnsServerList);
    ns_list_init(&entry->vendorDataList);
    return entry;
}

static uint16_t libdhcpv6_get_unique_id(dhcpv6_gua_server_entry_s *serverInfo)
{
    uint8_t suggested_id = serverInfo->firstUnusedId;

    do {
        ns_list_foreach(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
            if (serverInfo->firstUnusedId == cur->allocatedID) {
                serverInfo->firstUnusedId++;
                if (serverInfo->firstUnusedId == 0) {
                    serverInfo->firstUnusedId++;
                }
                //Start from beginning
                break;
            }
        }

    } while (suggested_id != serverInfo->firstUnusedId);
    //return the first free and increase the value for the next time.
    serverInfo->firstUnusedId = suggested_id + 1;
    return suggested_id;
}

static uint16_t libdhcpv6_address_id_allocate(dhcpv6_gua_server_entry_s *serverInfo)
{
    // Unique IDs start to generate from 1 and continue to go up every new address
    // if we go around 65535 we start from 1 and search for free id

    return libdhcpv6_get_unique_id(serverInfo);
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

static void libdhcpv6_address_list_entry_free(dhcpv6_gua_server_entry_s *server_info, dhcpv6_allocated_address_entry_t *entry)
{
    ns_list_remove(&server_info->allocatedAddressList, entry);
    ns_dyn_mem_free(entry);
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
                        uint8_t ipAddress[16];
                        libdhcpv6_allocated_address_write(ipAddress, address, cur);
                        cur->removeCb(cur->interfaceId, ipAddress, cur->guaPrefix);
                    }
                } else {
                    address->preferredLifetime -= timeUpdateInSeconds;
                }
            }

            if (address->lifetime <= timeUpdateInSeconds) {
                libdhcpv6_address_list_entry_free(cur, address);
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
                libdhcpv6_address_list_entry_free(serverInfo, cur);
            }

            ns_list_foreach_safe(dhcpv6_dns_server_data_t, cur, &serverInfo->dnsServerList) {
                //DNS Server Info Remove
                ns_list_remove(&serverInfo->dnsServerList, cur);
                ns_dyn_mem_free(cur->search_list);
                ns_dyn_mem_free(cur);
            }

            ns_list_foreach_safe(dhcpv6_vendor_data_t, cur, &serverInfo->vendorDataList) {
                ns_list_remove(&serverInfo->vendorDataList, cur);
                ns_dyn_mem_free(cur->vendor_data);
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

void libdhcpv6_allocated_address_write(uint8_t *ptr, dhcpv6_allocated_address_entry_t *address, dhcpv6_gua_server_entry_s *serverInfo)
{
    memcpy(ptr, serverInfo->guaPrefix, 8);
    ptr += 8;
    if (serverInfo->anonymousAddress) {
        //Generate from 16-bit allocate and default suffic's
        libdhcpv6_gen_suffics_from_allocated_id(ptr, serverInfo->clientIdDefaultSuffics, address->allocatedID);
        return;
    }
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

static void libdhcpv6_generate_address_entry(dhcpv6_allocated_address_t *address, dhcpv6_allocated_address_entry_t *cur, dhcpv6_gua_server_entry_s *serverInfo)
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

static void libdhcpv6_address_list_entry_add_to_list(dhcpv6_gua_server_entry_s *serverInfo, dhcpv6_allocated_address_entry_t *allocated)
{
    if (allocated->allocatedID != 0) {
        // If we have allocated ID we sort by it if it is 0 we dont do any sorting
        ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
            if (cur->allocatedID > allocated->allocatedID) {
                //Add before new allocated
                ns_list_add_before(&serverInfo->allocatedAddressList, cur, allocated);
                return;
            }
        }
    }
    ns_list_add_to_end(&serverInfo->allocatedAddressList, allocated);
}

void libdhcpv6_address_delete(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    uint8_t device_address[16];
    if (memcmp(serverInfo->guaPrefix, address, 8)) {
        return;
    }

    ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        libdhcpv6_allocated_address_write(device_address, cur, serverInfo);
        if (memcmp(address, device_address, 16) == 0) {
            libdhcpv6_address_list_entry_free(serverInfo, cur);
            return;
        }
    }
}

static dhcpv6_allocated_address_entry_t *libdhcpv6_address_list_entry_create(dhcpv6_gua_server_entry_s *serverInfo, dhcpv6_allocated_address_entry_t *source)
{
    dhcpv6_allocated_address_entry_t *entry;

    entry = ns_dyn_mem_alloc(sizeof(dhcpv6_allocated_address_entry_t));

    if (!entry) {
        return NULL;
    }

    *entry = *source;
    libdhcpv6_address_list_entry_add_to_list(serverInfo, entry);
    return entry;
}


dhcpv6_allocated_address_t *libdhcpv6_address_allocate(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *linkId, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew)
{
    dhcpv6_allocated_address_entry_t newEntry;
    uint16_t duiLength = 6;
    if (linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE ||
            linkType == DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE) {
        duiLength = 8;
    }

    // Search if we have old address in list
    ns_list_foreach(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
        if (cur->linkType == linkType && memcmp(cur->linkId, linkId, duiLength) == 0) {
            cur->iaID = iaID;
            cur->T0 = T0;
            cur->T1 = T1;
            libdhcpv6_address_entry_lifetime_set(cur, serverInfo->validLifetime);
            libdhcpv6_generate_address_entry(&serverInfo->tempAddressEntry, cur, serverInfo);
            return &serverInfo->tempAddressEntry;
        }
    }
    if (!allocateNew) {
        return NULL;
    }

    if (ns_list_count(&serverInfo->allocatedAddressList) >= serverInfo->maxSupportedClients) {
        // Maximum supported clients reached
        return NULL;
    }

    // Generate new address
    memcpy(newEntry.linkId, linkId, duiLength);
    newEntry.linkType = linkType;
    newEntry.iaID = iaID;
    newEntry.T0 = T0;
    newEntry.T1 = T1;
    newEntry.allocatedID = 0;
    libdhcpv6_address_entry_lifetime_set(&newEntry, serverInfo->validLifetime);
    if (serverInfo->anonymousAddress) {
        // Generate anonymous address id
        newEntry.allocatedID = libdhcpv6_address_id_allocate(serverInfo);
    }

    if (!serverInfo->disableAddressList) {
        // Create new List item and add to list
        libdhcpv6_address_list_entry_create(serverInfo, &newEntry);
    }

    libdhcpv6_generate_address_entry(&serverInfo->tempAddressEntry, &newEntry, serverInfo);
    return &serverInfo->tempAddressEntry;
}

dhcpv6_dns_server_data_t *libdhcpv6_dns_server_discover(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    ns_list_foreach(dhcpv6_dns_server_data_t, cur, &serverInfo->dnsServerList) {
        if (memcmp(cur->server_address, address, 16) == 0) {
            return cur;
        }
    }
    return NULL;
}

dhcpv6_dns_server_data_t *libdhcpv6_dns_server_allocate(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address)
{
    dhcpv6_dns_server_data_t *entry = libdhcpv6_dns_server_discover(serverInfo, address);
    if (entry) {
        return entry;
    }

    entry = ns_dyn_mem_alloc(sizeof(dhcpv6_dns_server_data_t));
    if (!entry) {
        return NULL;
    }
    ns_list_add_to_end(&serverInfo->dnsServerList, entry);
    memcpy(entry->server_address, address, 16);
    entry->search_list = NULL;
    entry->search_list_length = 0;
    return entry;
}

dhcpv6_vendor_data_t *libdhcpv6_vendor_data_discover(dhcpv6_gua_server_entry_s *serverInfo, uint32_t enterprise_number)
{
    ns_list_foreach(dhcpv6_vendor_data_t, cur, &serverInfo->vendorDataList) {
        if (cur->enterprise_number == enterprise_number) {
            return cur;
        }
    }
    return NULL;
}

dhcpv6_vendor_data_t *libdhcpv6_vendor_data_allocate(dhcpv6_gua_server_entry_s *serverInfo, uint32_t enterprise_number)
{
    dhcpv6_vendor_data_t *entry = libdhcpv6_vendor_data_discover(serverInfo, enterprise_number);

    if (entry) {
        return entry;
    }

    entry = ns_dyn_mem_alloc(sizeof(dhcpv6_vendor_data_t));
    if (!entry) {
        return NULL;
    }
    ns_list_add_to_end(&serverInfo->vendorDataList, entry);
    entry->enterprise_number = enterprise_number;
    entry->vendor_data_cb = NULL;
    entry->vendor_data = NULL;
    entry->vendor_data_length = 0;
    return entry;
}

uint16_t libdhcpv6_dns_server_message_sizes(dhcpv6_gua_server_entry_s *serverInfo)
{
    uint16_t message_size = 0;
    ns_list_foreach(dhcpv6_dns_server_data_t, cur, &serverInfo->dnsServerList) {
        message_size += 4 + 16; //Type Length + address //
        //Search List part
        message_size += 4 + cur->search_list_length; //Type Length + search_list_length
    }
    return message_size;
}

uint16_t libdhcpv6_vendor_data_message_sizes(dhcpv6_gua_server_entry_s *serverInfo)
{
    uint16_t message_size = 0;
    ns_list_foreach(dhcpv6_vendor_data_t, cur, &serverInfo->vendorDataList) {
        uint16_t size = cur->vendor_data_length; //Type + Length + enterprise + vendor_data_length
        if (cur->vendor_data_cb) {
            cur->vendor_data_cb(serverInfo->interfaceId, NULL, &size);
        }
        if (size != 0) {
            message_size += 2 + 2 + 4 + size;
        }
    }
    return message_size;
}

uint8_t *libdhcpv6_dns_server_message_writes(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *ptr)
{
    ns_list_foreach(dhcpv6_dns_server_data_t, cur, &serverInfo->dnsServerList) {
        //Write first DNS Server info
        ptr = common_write_16_bit(DHCPV6_OPTION_DNS_SERVERS, ptr);
        ptr = common_write_16_bit(16, ptr); //Length
        memcpy(ptr, cur->server_address, 16);
        ptr += 16;

        ptr = common_write_16_bit(DHCPV6_OPTION_DOMAIN_LIST, ptr);
        ptr = common_write_16_bit(cur->search_list_length, ptr); //Length
        if (cur->search_list_length) {
            memcpy(ptr, cur->search_list, cur->search_list_length);
            ptr += cur->search_list_length;
        }
    }
    return ptr;
}

uint8_t *libdhcpv6_vendor_data_message_writes(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *ptr)
{
    ns_list_foreach(dhcpv6_vendor_data_t, cur, &serverInfo->vendorDataList) {

        uint16_t length = cur->vendor_data_length;

        if (cur->vendor_data_cb) {
            cur->vendor_data_cb(serverInfo->interfaceId, NULL, &length);
        }
        if (length == 0) {
            // No vendor data
            continue;
        }
        length += 4;

        ptr = common_write_16_bit(DHCPV6_OPTION_VENDOR_SPECIFIC_INFO, ptr);
        ptr = common_write_16_bit(length, ptr); //Length
        ptr = common_write_32_bit(cur->enterprise_number, ptr);
        if (cur->vendor_data_length) {
            memcpy(ptr, cur->vendor_data, cur->vendor_data_length);
            ptr += cur->vendor_data_length;
        }
        if (cur->vendor_data_cb) {
            ptr = cur->vendor_data_cb(serverInfo->interfaceId, ptr, NULL);
        }
    }
    return ptr;
}


#endif

