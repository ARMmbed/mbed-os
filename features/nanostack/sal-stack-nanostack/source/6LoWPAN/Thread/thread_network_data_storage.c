/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * \file thread_network_data_storage.c
 * \brief Add short description about this file!!!
 *
 */

#include "nsconfig.h"
#ifdef HAVE_THREAD
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "ip6string.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "thread_management_if.h"
#include "thread_meshcop_lib.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"

#define TRACE_GROUP "thrd"


static void network_server_print(const thread_network_server_data_entry_t *route)
{
#ifndef HAVE_DEBUG
    (void) route;
#endif

    tr_info("%s Info: routerId:%04x",
            (route->stableData ? "Stable" : "Temporary"),
            route->routerID);
}

//@TODO seems that this function is called from application,
//       is there better way than print data inside stack ?
void thread_nd_network_data_print(thread_network_data_cache_entry_t *networkData, uint16_t routerId)
{
    // Route prefix is variable-length, so need to zero pad for ip6tos
    uint8_t addr[16];
    tr_info("Network Data:");

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkData->localPrefixList) {
        memset(addr, 0, 16);
        bitcopy(addr, cur->servicesPrefix, cur->servicesPrefixLen);
        tr_info("Prefix: %40s/%-3u", trace_ipv6(addr), cur->servicesPrefixLen);
        ns_list_foreach(thread_network_server_data_entry_t, curRoute, &cur->routeList) {
            tr_info("Has Route:");
            if (curRoute->routerID != routerId) {
                network_server_print(curRoute);
            } else {
                tr_info("This Interface");
            }
        }

        ns_list_foreach(thread_network_server_data_entry_t, curRoute, &cur->borderRouterList) {
            if (curRoute->P_dhcp) {
                tr_info("DHCPv6:");
            }
            if (curRoute->P_slaac || curRoute->P_preferred) {
                tr_info("SLAAC:");
            }
            if (curRoute->routerID != routerId) {
                network_server_print(curRoute);
            } else {
                tr_info("I'm Hosting");
            }
        }
    }
}

uint8_t thread_nd_server_external_route_list_size(thread_network_server_data_list_t *list, bool stable)
{

    uint8_t listSize = 0;

    ns_list_foreach(thread_network_server_data_entry_t, cur, list) {
        if (cur->stableData == stable) {
            listSize += 3;
        }
    }

    if (listSize) {
        listSize += 2;
    }

    return listSize;
}

uint8_t thread_nd_border_routerserver_list_size(thread_network_server_data_list_t *list, bool stable)
{
    uint8_t listSize = 0;

    ns_list_foreach(thread_network_server_data_entry_t, cur, list) {
        if (cur->stableData == stable) {
            listSize += 4;
        }
    }
    if (listSize) {
        listSize += 2;
    }

    return listSize;
}

uint8_t thread_nd_context_list_size(thread_data_context_list_t *list, bool stable)
{
    uint8_t listSize = 0;

    ns_list_foreach(thread_network_data_context_entry_t, cur, list) {
        if (cur->stableData == stable) {
            listSize += 2;
        }
    }

    if (listSize) {
        listSize += 2;
    }
    return listSize;
}

uint8_t *thread_nd_context_list_write(thread_data_context_list_t *list, uint8_t *dataPtr, bool stableData)
{
    uint8_t tlv_size = 0;
    uint8_t *ptr = (dataPtr + 2); //Leave allways Type & Length free first

    ns_list_foreach(thread_network_data_context_entry_t, cur, list) {
        if (cur->stableData == stableData) {
            tlv_size += 2;
            *ptr++ = cur->cid | (cur->compression ? THREAD_NWK_CONTEXT_COMPRESS_ENABLED : 0);
            *ptr++ = cur->contextPrefixLength;
        }
    }
    if (tlv_size) {
        uint8_t type = THREAD_NWK_DATA_TYPE_6LOWPAN_ID;
        if (stableData) {
            type |= THREAD_NWK_STABLE_DATA;
        }

        *dataPtr++ = type;
        *dataPtr = tlv_size;
        //SET Pointer back to end of data
        dataPtr = ptr;
    }

    return dataPtr;
}

static uint16_t thread_nd_service_border_router_flags_read(thread_network_server_data_entry_t *cur)
{
    uint16_t flags = (cur->Prf << THREAD_PRF_BIT_MOVE);
    flags |= (cur->P_preferred << THREAD_P_PREFERRED_BIT_MOVE);
    flags |= (cur->P_slaac << THREAD_P_SLAAC_BIT_MOVE);
    flags |= (cur->P_dhcp << THREAD_P_DHCP_BIT_MOVE);
    flags |= (cur->P_configure << THREAD_P_CONFIGURE_BIT_MOVE);
    flags |= (cur->P_default_route << THREAD_P_DEF_ROUTE_BIT_MOVE);
    flags |= (cur->P_on_mesh << THREAD_P_ON_MESH_BIT_MOVE);
    flags |= (cur->P_nd_dns << THREAD_P_ND_DNS_BIT_MOVE);
    return flags;
}

uint8_t *thread_nd_server_list_write(thread_network_server_data_list_t *list, uint8_t *dataPtr, uint8_t type, bool stable)
{
    uint8_t tlv_size = 0, tlv_length;
    uint16_t flags;
    uint8_t *ptr = (dataPtr + 2); //Leave allways Type & Length free first
    uint16_t routerId;

    if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
        tlv_length = THREAD_BORDER_ROUTER_TLV_LENGTH;
    } else {
        tlv_length = THREAD_HAS_ROUTE_TLV_LENGTH;
    }

    ns_list_foreach(thread_network_server_data_entry_t, cur, list) {
        if (cur->stableData == stable) {
            tlv_size += tlv_length;
            routerId = cur->routerID;
            if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                flags = thread_nd_service_border_router_flags_read(cur);
                ptr = thread_nd_network_data_border_router_tlv_write(ptr, routerId, flags);
            } else {
                uint8_t pref = (cur->Prf << THREAD_HAS_ROUTE_PRF_BIT_MOVE);
                ptr = thread_nd_network_data_has_route_tlv_write(ptr, routerId, pref);
            }
        }
    }

    if (tlv_size) {
        if (stable) {
            type |= THREAD_NWK_STABLE_DATA;
        }
        *dataPtr++ = type;
        *dataPtr = tlv_size;
        //SET Pointer back to end of data
        dataPtr = ptr;
    }

    return dataPtr;
}

uint8_t *thread_nd_service_server_list_write(thread_network_data_service_server_list_t *list, uint8_t *ptr, bool full_list)
{
    ns_list_foreach(thread_network_data_service_server_entry_t, cur, list) {
        if (!full_list && !cur->stable) {
            continue;
        }

        if (cur->stable) {
            *ptr++ = THREAD_NWK_DATA_TYPE_SERVER_DATA | THREAD_NWK_STABLE_DATA;
        } else {
            *ptr++ = THREAD_NWK_DATA_TYPE_SERVER_DATA;
        }

        *ptr++ = 2 + cur->server_data_length;
        ptr = common_write_16_bit(cur->router_id, ptr);

        memcpy(ptr, cur->server_data, cur->server_data_length);
        ptr += cur->server_data_length;
    }

    return ptr;
}

uint8_t thread_nd_service_based_on_list_entry_size(thread_network_data_service_cache_entry_t *entry, bool fullList)
{
    uint8_t tempTlvSize = 0;

    ns_list_foreach(thread_network_data_service_server_entry_t, cur, &entry->server_list) {
        if (!fullList && !cur->stable) {
            continue;
        }
        tempTlvSize += 4; // Type, Length, and S_server_16
        tempTlvSize += cur->server_data_length;
    }

    if (tempTlvSize) {
        tempTlvSize += 2; // T + S_id + service data length

        if (!entry->T) {
            tempTlvSize += 4;
        }

        tempTlvSize += entry->S_service_data_length;
    }

    return tempTlvSize;
}

uint8_t thread_nd_prefix_based_on_list_entry_size(thread_network_data_prefix_cache_entry_t *entry, bool fullList)
{
    uint8_t tempTlvSize = 0;

    tempTlvSize += thread_nd_border_routerserver_list_size(&entry->borderRouterList, true);
    tempTlvSize += thread_nd_server_external_route_list_size(&entry->routeList, true);
    tempTlvSize += thread_nd_context_list_size(&entry->contextList, true);
    if (fullList) {
        tempTlvSize += thread_nd_border_routerserver_list_size(&entry->borderRouterList, false);
        tempTlvSize += thread_nd_server_external_route_list_size(&entry->routeList, false);
        tempTlvSize += thread_nd_context_list_size(&entry->contextList, false);
    }

    if (tempTlvSize) {
        tempTlvSize += prefixBits_to_bytes(entry->servicesPrefixLen);
        tempTlvSize += 2; //Domain ID & Length
    }
    return tempTlvSize;
}

int thread_nd_local_data_length_updated(thread_network_data_cache_entry_t *networkDataList)
{
    int ret_val = -1;
    uint16_t localDataLength = 0;
    localDataLength = thread_network_data_prefix_set_size(networkDataList, true)
        + thread_network_data_service_set_size(networkDataList, true);

    if (localDataLength != networkDataList->networkDataTlvSize) {
        networkDataList->networkDataTlvSize = localDataLength;
        ret_val = 0;
    }
    return ret_val;
}


thread_network_local_data_entry_t *thread_local_service_list_allocate(thread_prefix_tlv_t *prefixTlv)
{
    thread_network_local_data_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(thread_network_local_data_entry_t));
    if (newEntry) {
        memset(newEntry->servicesPrefix, 0, 16);
        bitcopy(newEntry->servicesPrefix, prefixTlv->Prefix,  prefixTlv->PrefixLen);
        newEntry->servicesPrefixLen = prefixTlv->PrefixLen;
        newEntry->domainId = prefixTlv->domainId;
        newEntry->dhcpv6ServerActive = false;
        newEntry->dhcpv6ServerDataStable = false;
        newEntry->slaacServerActive = false;
        newEntry->slaacServerDataStable = false;
        newEntry->slaacPreferred = false;
        newEntry->routeActive = false;
        newEntry->routeDataStable = false;
        newEntry->preference = 0;
        newEntry->configure = false;
        newEntry->defaultRoute = false;
        newEntry->onMesh = false;
        newEntry->ndDns = false;
    }
    return newEntry;
}

thread_network_data_prefix_cache_entry_t *thread_prefix_service_list_allocate(thread_prefix_tlv_t *prefixTlv)
{
    thread_network_data_prefix_cache_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(thread_network_data_prefix_cache_entry_t));
    if (newEntry) {
        memset(newEntry->servicesPrefix, 0, 16);
        bitcopy(newEntry->servicesPrefix, prefixTlv->Prefix,  prefixTlv->PrefixLen);
        newEntry->servicesPrefixLen = prefixTlv->PrefixLen;
        newEntry->domainId = prefixTlv->domainId;
        ns_list_init(&newEntry->contextList);
        ns_list_init(&newEntry->borderRouterList);
        ns_list_init(&newEntry->routeList);
    }
    return newEntry;
}

thread_network_server_data_entry_t *thread_server_entry_allocate(thread_border_router_tlv_entry_t *service)
{
    thread_network_server_data_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(thread_network_server_data_entry_t));
    if (newEntry) {
        tr_debug("Create new server entry addr: %04x",service->routerID);
        newEntry->routerID = service->routerID;
        newEntry->stableData = service->stableData;
        newEntry->P_configure = service->P_configure;
        newEntry->P_default_route = service->P_default_route;
        newEntry->P_dhcp = service->P_dhcp;
        newEntry->P_preferred = service->P_preferred;
        newEntry->P_slaac = service->P_slaac;
        newEntry->Prf = service->Prf;
        newEntry->P_on_mesh = service->P_on_mesh;
        newEntry->P_nd_dns = service->P_nd_dns;
        newEntry->canDelete = false;

    }
    return newEntry;
}

thread_network_data_context_entry_t *thread_context_service_list_allocate(uint8_t contextLength, uint8_t cid, bool compersioEnabled)
{
    thread_network_data_context_entry_t *newEntry = ns_dyn_mem_alloc(sizeof(thread_network_data_context_entry_t));
    if (newEntry) {
        newEntry->cid = cid;
        newEntry->compression = compersioEnabled;
        newEntry->contextPrefixLength = contextLength;
        newEntry->canDelete = false;
        newEntry->stableData = false;
    }
    return newEntry;
}

int  thread_nd_verify_contex_id_is_free(thread_network_data_cache_entry_t *list, uint8_t *prefixPtr, thread_network_local_data_context_entry_t *context)
{
    int retVal = 0;
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &list->localPrefixList) {

        ns_list_foreach(thread_network_data_context_entry_t, curContetx, &cur->contextList) {
            if (curContetx->cid == context->cid) {
                if (context->contextPrefixLength == cur->servicesPrefixLen) {
                    if (!bitsequal(cur->servicesPrefix, prefixPtr, context->contextPrefixLength)) {
                        retVal = -1; //Do not accept same context anymore again if prefix is different
                    }
                } else {
                    retVal = -1;
                }

                return retVal;
            }
        }

    }
    return retVal;
}

thread_network_data_service_cache_entry_t *thread_network_data_service_entry_find(thread_network_data_service_cache_list_t *list, thread_network_data_service_entry_t *service)
{
    ns_list_foreach(thread_network_data_service_cache_entry_t, cur, list) {
        if (service->S_enterprise_number == cur->S_enterprise_number) {
            if (service->S_service_data_length == cur->S_service_data_length) {
                if (memcmp(service->S_service_data, cur->S_service_data, service->S_service_data_length) == 0) {
                    return cur;
                }
            }
        }
    }

    return NULL;
}

thread_network_data_service_entry_t *thread_local_service_entry_find(thread_network_data_service_list_t *list, thread_network_data_service_entry_t *service)
{
    ns_list_foreach(thread_network_data_service_entry_t, cur, list) {
        if (service->S_enterprise_number == cur->S_enterprise_number) {
            if (service->S_service_data_length == cur->S_service_data_length) {
                if (memcmp(service->S_service_data, cur->S_service_data, service->S_service_data_length) == 0) {
                    return cur;
                }
            }
        }
    }

    return NULL;
}

thread_network_data_service_entry_t *thread_local_service_entry_save(thread_network_data_service_list_t *list, thread_network_data_service_entry_t *service)
{
    thread_network_data_service_entry_t *service_entry;
    service_entry = thread_local_service_entry_find(list, service);

    if (service_entry) {
        service_entry->S_id = service->S_id;
        service_entry->S_stable = service->S_stable;

        if (service_entry->S_server_data) {
            ns_dyn_mem_free(service_entry->S_server_data);
            service_entry->S_server_data = NULL;
        }

        if (service->S_server_data_length) {
            service_entry->S_server_data = ns_dyn_mem_alloc(service->S_server_data_length);
        }

        if (service_entry->S_server_data) {
            service_entry->S_server_data_length = service->S_server_data_length;
            memcpy(service_entry->S_server_data, service->S_server_data, service->S_server_data_length);
        }

        return service_entry;
    }

    service_entry = ns_dyn_mem_alloc(sizeof(thread_network_data_service_entry_t));

    if (service_entry) {
        memset(service_entry, 0, sizeof(thread_network_data_service_entry_t));

        service_entry->T = service->T;
        service_entry->S_id = service->S_id;
        service_entry->S_stable = service->S_stable;
        service_entry->S_enterprise_number = service->S_enterprise_number;
        service_entry->S_service_data = ns_dyn_mem_alloc(service->S_service_data_length);

        if (!service_entry->S_service_data) {
            tr_error("Failed to allocate memory for service data!");
            ns_dyn_mem_free(service_entry);
            return NULL;
        }

        service_entry->S_service_data_length = service->S_service_data_length;
        memcpy(service_entry->S_service_data, service->S_service_data, service->S_service_data_length);

        if (service->S_server_data_length) {
            service_entry->S_server_data = ns_dyn_mem_alloc(service->S_server_data_length);

            if (!service_entry->S_server_data) {
                tr_error("Failed to allocate memory for server data!");
                ns_dyn_mem_free(service_entry->S_service_data);
                ns_dyn_mem_free(service_entry);
                return NULL;
            }

            service_entry->S_server_data_length = service->S_server_data_length;
            memcpy(service_entry->S_server_data, service->S_server_data, service->S_server_data_length);
        }

        ns_list_add_to_end(list, service_entry);

        tr_debug("Added service; enterprise number: %"PRIu32", service: %s", service_entry->S_enterprise_number,
                 trace_array(service_entry->S_service_data, service_entry->S_service_data_length));
    }

    return service_entry;
}

thread_network_local_data_entry_t *thread_local_prefix_entry_find(thread_network_data_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    ns_list_foreach(thread_network_local_data_entry_t, cur, list) {
        if (prefixTlv->domainId == cur->domainId) {
            if (prefixTlv->PrefixLen == cur->servicesPrefixLen) {
                if (bitsequal(cur->servicesPrefix, prefixTlv->Prefix, prefixTlv->PrefixLen)) {
                    return cur;
                }
            }
        }
    }
    return NULL;
}

thread_network_local_data_entry_t *thread_local_prefix_entry_get(thread_network_data_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    thread_network_local_data_entry_t *prefix_entry;

    //Check current Prefix List
    prefix_entry = thread_local_prefix_entry_find(list, prefixTlv);
    if (prefix_entry) {
        return prefix_entry;
    }

    prefix_entry = thread_local_service_list_allocate(prefixTlv);
    if (prefix_entry) {
        bool inserted = false;
        ns_list_foreach(thread_network_local_data_entry_t, cur, list) {
            if (prefixTlv->PrefixLen >= cur->servicesPrefixLen) { //Add always longer or same length to before last one
                //add before new
                tr_debug("Add new Longer");
                inserted = true;
                ns_list_add_before(list, cur, prefix_entry);
                break;
            }
        }
        if (!inserted) {
            tr_debug("Add to end");
            ns_list_add_to_end(list, prefix_entry);
        }
    }
    return prefix_entry;
}

uint8_t thread_service_next_free_service_type_id(thread_network_data_service_cache_list_t *list)
{
    uint8_t sid;

    for (sid = 0; sid < 16; ++sid) {
        bool found = false;

        ns_list_foreach(thread_network_data_service_cache_entry_t, cur, list) {
            if (cur->S_id == sid) {
                found = true;
                break;
            }
        }

        if (!found) {
            return sid;
        }
    }

    return 0xff; // IDs exhausted
}

thread_network_data_prefix_cache_entry_t *thread_prefix_entry_find(thread_network_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, list) {
        if (prefixTlv->domainId == cur->domainId) {
            if (prefixTlv->PrefixLen == cur->servicesPrefixLen) {
                if (bitsequal(cur->servicesPrefix, prefixTlv->Prefix, prefixTlv->PrefixLen)) {
                    return cur;
                }
            }
        }
    }
    return NULL;
}
thread_network_data_prefix_cache_entry_t *thread_prefix_entry_get(thread_network_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv)
{
    thread_network_data_prefix_cache_entry_t *main_list;
    //Check current Prefix List
    main_list = thread_prefix_entry_find(list, prefixTlv);
    if (main_list) {
        return main_list;
    }

    main_list = thread_prefix_service_list_allocate(prefixTlv);
    tr_info("New prefix added");

    if (main_list) {
        bool inserted = false;
        ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, list) {
            if (prefixTlv->PrefixLen >= cur->servicesPrefixLen) { //Add always longer or same length to before last one
                //add before new
                tr_debug("Add new Longer");
                inserted = true;
                ns_list_add_before(list, cur, main_list);
                break;
            }
        }
        if (!inserted) {
            tr_debug("Add to end");
            ns_list_add_to_end(list, main_list);
        }
    }

    return main_list;
}

thread_network_data_service_cache_entry_t *thread_service_entry_get(thread_network_data_service_cache_list_t *list, thread_network_data_service_entry_t *service)
{
    thread_network_data_service_cache_entry_t *service_entry;

    service_entry = thread_network_data_service_entry_find(list, service);

    if (service_entry) {
        return service_entry;
    }

    service_entry = ns_dyn_mem_alloc(sizeof(thread_network_data_service_cache_entry_t));

    if (service_entry) {
        memset(service_entry, 0, sizeof(thread_network_data_service_entry_t));

        service_entry->T = service->T;
        service_entry->S_stable = service->S_stable;
        service_entry->S_enterprise_number = service->S_enterprise_number;

        uint8_t sid = thread_service_next_free_service_type_id(list);

        if (sid == 0xff) {
            tr_error("All Service Type ID (S_id) values used!");
            ns_dyn_mem_free(service_entry);
            return NULL;
        }

        service_entry->S_id = sid;
        service_entry->S_service_data = ns_dyn_mem_alloc(service->S_service_data_length);

        if (!service_entry->S_service_data) {
            tr_error("Failed to allocate memory for service data!");
            ns_dyn_mem_free(service_entry);
            return NULL;
        }

        service_entry->S_service_data_length = service->S_service_data_length;
        memcpy(service_entry->S_service_data, service->S_service_data, service->S_service_data_length);
        ns_list_init(&service_entry->server_list);
        ns_list_add_to_end(list, service_entry);
    }

    return service_entry;
}

thread_network_server_data_entry_t *thread_server_entry_search(thread_network_server_data_list_t *list, uint16_t routerId)
{
    ns_list_foreach(thread_network_server_data_entry_t, cur, list) {
        if (cur->routerID == routerId) {
            cur->canDelete = false;
            return cur;
        }
    }
    return NULL;
}

thread_network_data_service_server_entry_t *thread_service_server_entry_search(thread_network_data_service_server_list_t *list, uint16_t routerId)
{
    ns_list_foreach(thread_network_data_service_server_entry_t, cur, list) {
        if (cur->router_id == routerId) {
            cur->can_delete = false;
            return cur;
        }
    }
    return NULL;
}

thread_network_server_data_entry_t *thread_server_entry_get(thread_network_server_data_list_t *list, thread_border_router_tlv_entry_t *service)
{
    thread_network_server_data_entry_t *server_entry;
    bool inserted = false;

    //Check current Stable List
    server_entry = thread_server_entry_search(list, service->routerID);
    if (server_entry) {
        return server_entry;
    }
    server_entry = thread_server_entry_allocate(service);
    if (!server_entry) {
        return NULL;
    }
    tr_info("New server entry made");
    ns_list_foreach(thread_network_server_data_entry_t, cur, list)
    {
        if (service->routerID < cur->routerID) { //Add always longer or same length to before last one
            //add before new
            tr_debug("Add smaller id");
            inserted = true;
            ns_list_add_before(list, cur, server_entry);
            break;
        }
    }
    if (!inserted) {
        tr_debug("Add new Last");
        ns_list_add_to_end(list, server_entry);
    }

    return server_entry;
}

thread_network_data_service_server_entry_t *thread_service_server_entry_get(thread_network_data_service_server_list_t *list, thread_network_data_service_server_entry_t *server)
{
    thread_network_data_service_server_entry_t *server_entry;

    server_entry = thread_service_server_entry_search(list, server->router_id);

    if (server_entry) {
        return server_entry;
    }

    server_entry = ns_dyn_mem_alloc(sizeof(thread_network_data_service_server_entry_t));

    if (server_entry) {
        memset(server_entry, 0, sizeof(thread_network_data_service_server_entry_t));

        server_entry->can_delete = false;
        server_entry->router_id = server->router_id;
        server_entry->stable = server->stable;
        server_entry->server_data = ns_dyn_mem_alloc(server->server_data_length);

        if (!server_entry->server_data) {
            tr_error("Failed to allocate memory for server data!");
            ns_dyn_mem_free(server_entry);
            return NULL;
        }

        server_entry->server_data_length = server->server_data_length;
        memcpy(server_entry->server_data, server->server_data, server->server_data_length);
        ns_list_add_to_end(list, server_entry);
    }

    return server_entry;
}

uint8_t thread_get_context_id_by_length(thread_data_context_list_t *list, uint8_t contextLength)
{
    //Check current Stable List
    ns_list_foreach(thread_network_data_context_entry_t, cur, list) {
        if (cur->contextPrefixLength == contextLength) {
            return cur->cid;
        }
    }
    return 16;
}

thread_network_data_context_entry_t *thread_get_context_by_id(thread_data_context_list_t *list, uint8_t cid)
{
    //Check current Stable List
    ns_list_foreach(thread_network_data_context_entry_t, cur, list) {
        if (cur->cid == cid) {
            return cur;
        }
    }
    return NULL;
}

thread_network_server_data_entry_t *thread_get_dhcp_server_from_list(thread_network_server_data_list_t *list)
{
    //Check current Stable List
    ns_list_foreach(thread_network_server_data_entry_t, cur, list) {
        if (cur->P_dhcp) {
            return cur;
        }
    }
    return NULL;
}

thread_network_data_context_entry_t *thread_get_main_context_list(thread_data_context_list_t *list, thread_network_local_data_context_entry_t *context)
{
    thread_network_data_context_entry_t *main_list;

    //Check current Stable List
    ns_list_foreach(thread_network_data_context_entry_t, cur, list) {
        if (cur->cid == context->cid) {
            return cur;
        }
    }

    main_list = thread_context_service_list_allocate(context->contextPrefixLength, context->cid, context->compression);
    if (main_list) {
        tr_debug("Add New");
        ns_list_add_to_end(list, main_list);
    }

    return main_list;
}

void thread_server_list_free(thread_network_server_data_list_t *listPtr)
{
    ns_list_foreach_safe(thread_network_server_data_entry_t, cur, listPtr) {
        ns_list_remove(listPtr, cur);
        ns_dyn_mem_free(cur);
    }
}

void thread_service_server_list_free(thread_network_data_service_server_list_t *listPtr)
{
    ns_list_foreach_safe(thread_network_data_service_server_entry_t, cur, listPtr) {
        if (cur->server_data) {
            ns_dyn_mem_free(cur->server_data);
        }
        ns_list_remove(listPtr, cur);
        ns_dyn_mem_free(cur);
    }
}

static int thread_server_context_delete_mark(thread_data_context_list_t *listPtr)
{
    int retVal = -1;
    ns_list_foreach_safe(thread_network_data_context_entry_t, cur, listPtr) {
        cur->canDelete = true;
    }
    return retVal;
}

static int thread_server_data_delete_mark_by_router_id(thread_network_server_data_list_t *listPtr, uint16_t routerID, bool subSet)
{
    int retVal = -1;
    ns_list_foreach_safe(thread_network_server_data_entry_t, cur, listPtr) {
        if (!subSet) {
            cur->canDelete = true;
        }
        if (cur->routerID == routerID) {
            cur->canDelete = true;
        }
    }
    return retVal;
}

static int thread_service_data_delete_mark_by_router_id(thread_network_data_service_server_list_t *listPtr, uint16_t routerID, bool subSet)
{
    int retVal = -1;
    ns_list_foreach_safe(thread_network_data_service_server_entry_t, cur, listPtr) {
        if (!subSet) {
            cur->can_delete = true;
        }
        if (cur->router_id == routerID) {
            cur->can_delete = true;
        }
    }
    return retVal;
}

static int thread_server_context_clean(thread_network_data_cache_entry_t *cachePtr, thread_data_context_list_t *listPtr, thread_network_data_prefix_cache_entry_t *prefixEntry, lowpan_context_list_t *context_list)
{
    int retVal = -1;
    (void) prefixEntry;
    ns_list_foreach_safe(thread_network_data_context_entry_t, cur, listPtr) {
        if (cur->canDelete) {
            cachePtr->temporaryUpdatePushed = true;
            if (cur->stableData) {
                cachePtr->stableUpdatePushed = true;
            }
            // Set context lifetime to 0 to delete
            lowpan_context_update(context_list, cur->cid, 0, NULL, 0, true);
            ns_list_remove(listPtr, cur);
            ns_dyn_mem_free(cur);
            retVal = 0;
        }
    }
    return retVal;
}


static bool thread_server_data_clean_by_router_id(thread_network_data_cache_entry_t *cachePtr, thread_network_server_data_list_t *listPtr, thread_network_data_prefix_cache_entry_t *prefixEntry, bool routeInfo, protocol_interface_info_entry_t *curInterface)
{
    uint8_t addr[16];
    uint8_t *nextHop;
    bool address_removed = false;

    uint16_t mac16 = mac_helper_mac16_address_get(curInterface);
    ns_list_foreach_safe(thread_network_server_data_entry_t, cur, listPtr) {
        if (cur->canDelete) {
            tr_debug("Delete BR %s", trace_array(prefixEntry->servicesPrefix, 8));
            cachePtr->temporaryUpdatePushed = true;
            if (cur->stableData) {
                cachePtr->stableUpdatePushed = true;
            }
            //Call Route and DHCPV6 Updates here
            memcpy(addr, curInterface->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);

            memcpy(&addr[8], ADDR_SHORT_ADR_SUFFIC, 6);
            common_write_16_bit(cur->routerID, &addr[14]);


            if (routeInfo) {
                if (cur->routerID == mac16) {
                    nextHop = NULL;
                } else {
                    nextHop = addr;
                }
                if (prefixEntry) {
                    ipv6_route_delete(prefixEntry->servicesPrefix, prefixEntry->servicesPrefixLen, curInterface->id, nextHop, ROUTE_THREAD);
                } else {
                    ipv6_route_delete(NULL, 0, curInterface->id, nextHop, ROUTE_THREAD);
                }
            } else {
                address_removed = true;

                if (cur->P_dhcp) {
                    tr_debug("Delete DHCPv6 given address");
                    thread_dhcp_client_global_address_delete(curInterface->id, addr, prefixEntry->servicesPrefix);
                } else {
                    tr_debug("Delete SLAAC address");
                    addr_delete_matching(curInterface, prefixEntry->servicesPrefix, 64, ADDR_SOURCE_SLAAC);
                }
            }

            ns_list_remove(listPtr, cur);
            ns_dyn_mem_free(cur);
        }
    }
    return address_removed;
}

static void thread_service_data_clean_by_router_id(thread_network_data_cache_entry_t *cachePtr, thread_network_data_service_server_list_t *listPtr)
{
    ns_list_foreach_safe(thread_network_data_service_server_entry_t, cur, listPtr) {
        if (cur->can_delete) {
            tr_debug("Deleting server %04x", cur->router_id);
            cachePtr->temporaryUpdatePushed = true;
            if (cur->stable) {
                cachePtr->stableUpdatePushed = true;
            }
            if (cur->server_data) {
                ns_dyn_mem_free(cur->server_data);
            }
            ns_list_remove(listPtr, cur);
            ns_dyn_mem_free(cur);
        }
    }
}

void thread_context_service_list_free(thread_data_context_list_t *listPtr)
{
    ns_list_foreach_safe(thread_network_data_context_entry_t, cur, listPtr) {
        ns_list_remove(listPtr, cur);
        ns_dyn_mem_free(cur);
    }
}


/**
 * Initialise Thread Network Data cache
 *
 * \param cachePtr  Pointer to Network Data Structure which will be initialized
 *
 */
void thread_network_data_base_init(thread_network_data_cache_entry_t *cachePtr)
{
    ns_list_init(&cachePtr->localPrefixList);
    ns_list_init(&cachePtr->service_list);
    cachePtr->contex_id_reuse_timeout = THREAD_CONTEXT_ID_REUSE_TIMEOUT;
    cachePtr->networkDataTlvSize = 0;
    cachePtr->stableUpdatePushed = false;
    cachePtr->temporaryUpdatePushed = false;
    cachePtr->network_data_update_delay = 0;
    cachePtr->network_data_len = 0;
}

void thread_network_local_server_data_base_init(thread_network_local_data_cache_entry_t *cachePtr)
{
    ns_list_init(&cachePtr->prefix_list);
    ns_list_init(&cachePtr->service_list);
    cachePtr->registered_rloc16 = 0xffff;
    cachePtr->release_old_address = false;
    cachePtr->publish_active = false;
    cachePtr->publish_pending = false;
}

void thread_network_data_router_id_mark_delete(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID, bool subSet)
{
    ns_list_foreach_safe(thread_network_data_prefix_cache_entry_t, cur, &cachePtr->localPrefixList) {
        thread_server_data_delete_mark_by_router_id(&cur->borderRouterList, routerID, subSet);
        thread_server_data_delete_mark_by_router_id(&cur->routeList, routerID, subSet);
        if (!subSet) {
            thread_server_context_delete_mark(&cur->contextList);
        }
    }

    ns_list_foreach_safe(thread_network_data_service_cache_entry_t, cur, &cachePtr->service_list) {
        thread_service_data_delete_mark_by_router_id(&cur->server_list, routerID, subSet);
    }
}
bool thread_network_data_router_id_free(thread_network_data_cache_entry_t *cachePtr, bool is_leader, protocol_interface_info_entry_t *curInterface)
{
    bool address_removed = false;
    ns_list_foreach_safe(thread_network_data_prefix_cache_entry_t, cur, &cachePtr->localPrefixList) {
        //GET Context
        if (thread_server_data_clean_by_router_id(cachePtr, &cur->borderRouterList, cur, false, curInterface) ) {
            address_removed = true;
        }
        thread_server_data_clean_by_router_id(cachePtr, &cur->routeList, cur, true, curInterface);
        if (!is_leader) {
            thread_server_context_clean(cachePtr, &cur->contextList, cur, &curInterface->lowpan_contexts);
        }

        if (ns_list_is_empty(&cur->borderRouterList)) {
            //Delete On-Mesh Prefix
            ipv6_route_delete(cur->servicesPrefix, cur->servicesPrefixLen, curInterface->id, NULL, ROUTE_THREAD);
        }

        if (!ns_list_is_empty(&cur->borderRouterList)) {

        } else if (!ns_list_is_empty(&cur->routeList)) {

        } else if (!ns_list_is_empty(&cur->contextList)) {
            if (is_leader) {
                ns_list_foreach_safe(thread_network_data_context_entry_t, curContext, &cur->contextList) {
                    cachePtr->temporaryUpdatePushed = true;
                    if (curContext->stableData) {
                        cachePtr->stableUpdatePushed = true;
                    }
                    if (curContext->compression) {
                        curContext->compression = false;
                        tr_debug("disable Cur Context and start timer");
                        curContext->context_reuse_delay = cachePtr->contex_id_reuse_timeout;
                    }
                }
            }
        } else {
            //ALL Empty
            ns_list_remove(&cachePtr->localPrefixList, cur);
            ns_dyn_mem_free(cur);
        }
    }

    ns_list_foreach_safe(thread_network_data_service_cache_entry_t, cur, &cachePtr->service_list) {
        thread_service_data_clean_by_router_id(cachePtr, &cur->server_list);
        if (ns_list_is_empty(&cur->server_list)) {
            if (cur->S_service_data) {
                ns_dyn_mem_free(cur->S_service_data);
            }
            ns_list_remove(&cachePtr->service_list, cur);
            ns_dyn_mem_free(cur);
        }
    }

    if (cachePtr->temporaryUpdatePushed || cachePtr->stableUpdatePushed) {
        //Validate that Length will be always updated
        cachePtr->networkDataTlvSize  = thread_network_data_prefix_set_size(cachePtr, true)
            + thread_network_data_service_set_size(cachePtr, true);
    }
    return address_removed;
}

void thread_network_data_context_re_use_timer_update(thread_network_data_cache_entry_t *cachePtr, uint32_t ticks, lowpan_context_list_t *context_list)
{
    ns_list_foreach_safe(thread_network_data_prefix_cache_entry_t, cur, &cachePtr->localPrefixList) {
        ns_list_foreach_safe(thread_network_data_context_entry_t, curContext, &cur->contextList) {
            if (!curContext->compression) {
                if (curContext->context_reuse_delay > ticks) {
                    curContext->context_reuse_delay -= ticks;
                } else {
                    ns_list_remove(&cur->contextList, curContext);
                    if (curContext->stableData) {
                        cachePtr->stableUpdatePushed = true;
                    } else {
                        cachePtr->temporaryUpdatePushed = true;
                    }
                    // Set context lifetime to 0 to delete
                    lowpan_context_update(context_list, curContext->cid, 0, NULL, 0, true);
                    ns_dyn_mem_free(curContext);
                }
            }
        }
        if (ns_list_is_empty(&cur->borderRouterList) &&
        ns_list_is_empty(&cur->routeList) &&
        ns_list_is_empty(&cur->contextList)) {
            ns_list_remove(&cachePtr->localPrefixList, cur);
            ns_dyn_mem_free(cur);
        }
    }
}


void thread_network_data_free_and_clean(thread_network_data_cache_entry_t *cachePtr)
{
    if (!ns_list_is_empty(&cachePtr->localPrefixList)) {
        ns_list_foreach_safe(thread_network_data_prefix_cache_entry_t, cur, &cachePtr->localPrefixList) {
            thread_context_service_list_free(&cur->contextList);
            thread_server_list_free(&cur->borderRouterList);
            thread_server_list_free(&cur->routeList);
            ns_list_remove(&cachePtr->localPrefixList, cur);
            ns_dyn_mem_free(cur);
        }
        ns_list_init(&cachePtr->localPrefixList);
    }

    if (!ns_list_is_empty(&cachePtr->service_list)) {
        ns_list_foreach_safe(thread_network_data_service_cache_entry_t, cur, &cachePtr->service_list) {
            thread_service_server_list_free(&cur->server_list);
            if (cur->S_service_data) {
                ns_dyn_mem_free(cur->S_service_data);
            }
            ns_list_remove(&cachePtr->service_list, cur);
            ns_dyn_mem_free(cur);
        }
        ns_list_init(&cachePtr->service_list);
    }

    cachePtr->networkDataTlvSize = 0;
    cachePtr->stableUpdatePushed = false;
    cachePtr->temporaryUpdatePushed = false;
    cachePtr->network_data_update_delay = 0;
    cachePtr->network_data_len = 0;
}

void thread_network_local_data_free_and_clean(thread_network_local_data_cache_entry_t *cachePtr, int8_t interface_id)
{
    ns_list_foreach_safe(thread_network_local_data_entry_t, cur, &cachePtr->prefix_list) {
        if (cur->dhcpv6ServerActive) {
            DHCPv6_server_service_delete(interface_id, cur->servicesPrefix, true);
        }

        ns_list_remove(&cachePtr->prefix_list, cur);
        ns_dyn_mem_free(cur);
    }

    ns_list_foreach_safe(thread_network_data_service_entry_t, cur, &cachePtr->service_list) {
        ns_list_remove(&cachePtr->service_list, cur);
        if (cur->S_service_data) {
            ns_dyn_mem_free(cur->S_service_data);
        }
        if (cur->S_server_data) {
            ns_dyn_mem_free(cur->S_server_data);
        }
        ns_dyn_mem_free(cur);
    }

    cachePtr->publish_pending = false;
    cachePtr->publish_active = false;
    cachePtr->release_old_address = false;
}

static bool thread_network_data_has_subtlv(uint8_t *network_data_ptr, uint16_t network_data_length, uint8_t *prefix, uint8_t prefix_length, uint8_t lookup_type)
{
    uint8_t *dptr;
    uint8_t length;
    uint8_t type;
    dptr = network_data_ptr;
    while (network_data_length) {
        if (network_data_length >= 2) {
            type  = *dptr++;
            type &= THREAD_NWK_DATA_TYPE_MASK;
            length = *dptr++;

            if (length == 0) {
                return false;
            }
            network_data_length -= 2;
            if (network_data_length >= length) {
                // Set length ready for next check
                network_data_length -= length;

                if (type == THREAD_NWK_DATA_TYPE_PREFIX) {
                    thread_prefix_tlv_t prefix_tlv;
                    prefix_tlv.domainId = *dptr++;
                    prefix_tlv.PrefixLen = *dptr++;
                    prefix_tlv.Prefix = dptr;
                    length -= 2;

                    uint8_t prefix_bytes_len = prefixBits_to_bytes(prefix_tlv.PrefixLen);
                    uint8_t prefix_bytes_length = prefixBits_to_bytes(prefix_length);

                    if (prefix_bytes_len > length) {
                        return false;
                    }

                    if (prefix_bytes_len != prefix_bytes_length ||
                        memcmp(prefix, prefix_tlv.Prefix, prefix_bytes_len) != 0) {
                        dptr += length;
                        continue;
                    }

                    length -= prefix_bytes_len;
                    dptr += prefix_bytes_len;

                    while (length > 2) {
                        type = *dptr++;
                        type &= THREAD_NWK_DATA_TYPE_MASK;
                        uint8_t subLength = *dptr++;
                        length -= 2;

                        if (subLength <= length) {
                            length -= subLength;
                            dptr += subLength;

                            if (lookup_type == type) {
                                return true;
                            }
                        } else {
                            tr_error("Length fail");
                            return false;
                        }
                    }
                } else {
                    dptr += length;
                }
            } else {
                tr_error("Length fail");
                return false;
            }
        } else {
            return false;
        }
    }

    return false;
}

static uint16_t thread_network_data_prefix_shrink_size(thread_network_data_cache_entry_t *networkDataStorage, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t router_id)
{
    uint16_t network_data_len = 0;

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataStorage->localPrefixList) {
        ns_list_foreach(thread_network_server_data_entry_t, br_cur, &cur->borderRouterList) {
            if (br_cur->routerID == router_id) {
                tr_debug("Checking prefix: %s", trace_ipv6_prefix(cur->servicesPrefix, cur->servicesPrefixLen));
                if (!thread_network_data_has_subtlv(network_data_ptr, network_data_length, cur->servicesPrefix, cur->servicesPrefixLen, THREAD_NWK_DATA_TYPE_BORDER_ROUTER)) {
                    if (ns_list_count(&cur->borderRouterList) == 1) {
                        network_data_len += 2 + THREAD_BORDER_ROUTER_TLV_LENGTH;
                    } else {
                        network_data_len += THREAD_BORDER_ROUTER_TLV_LENGTH;
                    }
                }
            }
        }

        ns_list_foreach(thread_network_server_data_entry_t, route_cur, &cur->routeList) {
            if (route_cur->routerID == router_id) {
                if (!thread_network_data_has_subtlv(network_data_ptr, network_data_length, cur->servicesPrefix, cur->servicesPrefixLen, THREAD_NWK_DATA_TYPE_ROUTE)) {
                    if (ns_list_count(&cur->routeList) == 1) {
                        network_data_len += 2 + THREAD_HAS_ROUTE_TLV_LENGTH;
                        if (ns_list_count(&cur->borderRouterList) == 0) {
                            uint8_t prefix_bytes_len = prefixBits_to_bytes(cur->servicesPrefixLen);
                            network_data_len += 2 + 2 + prefix_bytes_len;
                        }
                    } else {
                        network_data_len += THREAD_HAS_ROUTE_TLV_LENGTH;
                    }
                }
            }
        }
    }

    tr_debug("Shrink size: %d", network_data_len);
    return network_data_len;
}

/**
 * Calculates what the resulting size of prefixes for Network Data TLV would be.
 */
static int thread_network_data_resulting_prefix_set_size(thread_network_data_cache_entry_t *networkDataStorage, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t rid)
{
    uint8_t *dptr;
    uint8_t length;
    uint8_t type;

    if (!networkDataStorage || !network_data_ptr) {
        return -1;
    }

    uint16_t network_data_len = thread_network_data_prefix_set_size(networkDataStorage, true);
    tr_debug("Old network data length: %d", network_data_len);

    /* Calculate how much removed network data would reduce the total size */
    network_data_len -= thread_network_data_prefix_shrink_size(networkDataStorage, network_data_ptr, network_data_length, rid);

    dptr = network_data_ptr;
    while (network_data_length) {
        if (network_data_length >= 2) {
            type = *dptr++;
            type &= THREAD_NWK_DATA_TYPE_MASK;
            length = *dptr++;

            if (length == 0) {
                // 0 is not valid length for TLV
                return -1;
            }

            network_data_length -= 2;

            if (network_data_length >= length) {
                // Set length ready for next check
                network_data_length -= length;

                if (type == THREAD_NWK_DATA_TYPE_PREFIX) {
                    thread_prefix_tlv_t prefix_tlv;
                    prefix_tlv.domainId = *dptr++;
                    prefix_tlv.PrefixLen = *dptr++;
                    prefix_tlv.Prefix = dptr;
                    length -= 2;

                    uint8_t prefix_bytes_len = prefixBits_to_bytes(prefix_tlv.PrefixLen);

                    if (prefix_bytes_len > length) {
                        return -1;
                    }

                    length -= prefix_bytes_len;
                    dptr += prefix_bytes_len;

                    tr_debug("Prefix: %s", trace_ipv6_prefix(prefix_tlv.Prefix, prefix_tlv.PrefixLen));
                    thread_network_data_prefix_cache_entry_t *prefix = thread_prefix_entry_find(&networkDataStorage->localPrefixList, &prefix_tlv);

                    if (!prefix) {
                        tr_debug("Adding new prefix!");
                        // This is a new prefix; add header length
                        network_data_len += 2 + 2 + prefix_bytes_len;
                    }

                    while (length > 2) {
                        type = *dptr++;
                        type &= THREAD_NWK_DATA_TYPE_MASK;
                        uint8_t subLength = *dptr++;
                        length -= 2;

                        tr_debug("SubType: %02x, %s", type, trace_array(dptr, subLength));

                        if (subLength <= length) {
                            length -= subLength;

                            if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                                if (!prefix) {
                                    // Add 6LoWPAN ID length (plus type & length)
                                    network_data_len += 2 + THREAD_6LOWPAN_ID_TLV_LENGTH;
                                }

                                while (subLength) {
                                    thread_network_server_data_entry_t *server_entry = NULL;
                                    uint16_t router_id = common_read_16_bit(dptr);
                                    dptr += 4;

                                    if (prefix) {
                                        server_entry = thread_server_entry_search(&prefix->borderRouterList, router_id);
                                    }

                                    if (!prefix || !server_entry) {
                                        if (!prefix || ns_list_count(&prefix->borderRouterList) == 0) {
                                            // Add Border Router type and length
                                            network_data_len += 2;
                                        }
                                        // Add Border Router TLV length
                                        network_data_len += THREAD_BORDER_ROUTER_TLV_LENGTH;
                                    }

                                    subLength -= THREAD_BORDER_ROUTER_TLV_LENGTH;
                                }

                                tr_debug("Type is BORDER ROUTER");
                            } else if (type == THREAD_NWK_DATA_TYPE_ROUTE) {
                                while (subLength) {
                                    thread_network_server_data_entry_t *server_entry = NULL;
                                    uint16_t router_id  = common_read_16_bit(dptr);
                                    dptr += 3;

                                    if (prefix) {
                                        server_entry = thread_server_entry_search(&prefix->routeList, router_id);
                                    }

                                    if (!prefix || !server_entry) {
                                        if (!prefix || ns_list_count(&prefix->routeList) == 0) {
                                            // Add Has Route type and length
                                            network_data_len += 2;
                                        }
                                        // Add Has Route TLV length
                                        network_data_len += THREAD_HAS_ROUTE_TLV_LENGTH;
                                    }

                                    subLength -= THREAD_HAS_ROUTE_TLV_LENGTH;
                                }

                                tr_debug("Type is HAS ROUTE");
                            } else {
                                dptr += subLength;
                            }
                        } else {
                            tr_error("Length fail");
                            return -1;
                        }
                    }
                } else {
                    dptr += length;
                }
            } else {
                tr_error("Length fail");
                return -1;
            }
        } else {
            return -1;
        }
    }

    return network_data_len;
}

/**
 * Calculates what the future size of Network Data TLV would be if new data was
 * incorporated to the old data. NOTE: Use of this function should be replaced
 * by some sophisticated logic to analyze if new data or part of it should be
 * given priority. Now we simply limit by the total size.
 */
int thread_network_data_resulting_tlv_size(thread_network_data_cache_entry_t *networkDataStorage, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t router_id)
{
    int network_data_len = thread_network_data_resulting_prefix_set_size(networkDataStorage, network_data_ptr, network_data_length, router_id);

    // TODO: Add support for other TLVs than Prefix here

    tr_debug("Size of the prefix set: %d", network_data_len);

    if (network_data_len >= 0) {
        // Add maximum Commission TLV size
        network_data_len += THREAD_MAX_COMM_DATA_TLV_LEN;
    }

    return network_data_len;
}

/**
 * Add new route information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param service Route TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_nd_local_list_add_route(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *route)
{
    thread_network_server_data_entry_t *entry;
    tr_debug("Add Route: %s", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen));

    route->P_configure = false;
    route->P_default_route = false;
    route->P_dhcp = false;
    route->P_preferred = false;
    route->P_slaac = false;

    if (!networkDataList) {
        return -1;
    }

    thread_network_data_prefix_cache_entry_t *prefix_entry = thread_prefix_entry_get(&networkDataList->localPrefixList, prefixTlv);
    if (!prefix_entry) {
        return -1;
    }

    entry = thread_server_entry_get(&prefix_entry->routeList, route);
    if (!entry) {
        return -1;
    }

    entry->canDelete = false;
    if ((thread_nd_local_data_length_updated(networkDataList) == 0)) {
        if (route->stableData) {
            networkDataList->stableUpdatePushed = true;
        } else {
            networkDataList->temporaryUpdatePushed = true;
        }
    } else if (entry->Prf != route->Prf) {
        entry->Prf = route->Prf;
        if (route->stableData) {
            networkDataList->stableUpdatePushed = true;
        } else {
            networkDataList->temporaryUpdatePushed = true;
        }
    }
    return 0;
}

int thread_nd_local_list_add_service(thread_network_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service, thread_network_data_service_server_entry_t *server)
{
    bool changed = false;
    tr_debug("Add Service: %s", trace_array(service->S_service_data, service->S_service_data_length));

    if (!networkDataList) {
        return -1;
    }

    thread_network_data_service_cache_entry_t *service_entry = thread_service_entry_get(&networkDataList->service_list, service);
    if (!service_entry) {
        return -1;
    }

    thread_network_data_service_server_entry_t *entry = thread_service_server_entry_get(&service_entry->server_list, server);
    if (!entry) {
        return -1;
    }

    if (entry->server_data_length != server->server_data_length || memcmp(entry->server_data,server->server_data,server->server_data_length) != 0) {
        // Server data changed
        if (entry->server_data_length != server->server_data_length ) {
            ns_dyn_mem_free(entry->server_data);
            entry->server_data = ns_dyn_mem_alloc(server->server_data_length);
            if (!entry->server_data) {
                return -1;
            }
        }
        memcpy(entry->server_data,server->server_data,server->server_data_length);
        entry->server_data_length = server->server_data_length;
        changed = true;
    }
    entry->can_delete = false;

    if (changed || (thread_nd_local_data_length_updated(networkDataList) == 0)) {
        if (server->stable) {
            networkDataList->stableUpdatePushed = true;
        } else {
            networkDataList->temporaryUpdatePushed = true;
        }
    }

    return 0;
}

/**
 * Add new BorderRouter Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLv
 * \param service Border Router TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_nd_local_list_add_on_mesh_prefix(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    bool trigDataPropagate = false;
    thread_network_data_prefix_cache_entry_t *prefix_entry;
    thread_network_server_data_entry_t *server_entry;
    if (service->P_dhcp) {
        tr_debug("Add DHCPv6 prefix:%s server: %04x", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen),service->routerID);
    } else {
        tr_debug("Add SLAAC prefix:%s server: %04x", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen),service->routerID);
    }

    if (!networkDataList) {
        return -1;
    }

    if (!prefixTlv->PrefixLen) {
        return -1;
    }

    prefix_entry = thread_prefix_entry_get(&networkDataList->localPrefixList, prefixTlv);
    if (!prefix_entry) {
        return -1;
    }

    server_entry = thread_server_entry_get(&prefix_entry->borderRouterList, service);

    if (!server_entry) {
        return -1;
    }
    tr_debug("update current BR");

    server_entry->canDelete = false;
    if (thread_nd_local_data_length_updated(networkDataList) == 0) {
        trigDataPropagate = true;
    } else {
        if (server_entry->P_configure != service->P_configure) {
            server_entry->P_configure = service->P_configure;
            trigDataPropagate = true;
        }

        if (server_entry->P_default_route != service->P_default_route) {
            server_entry->P_default_route = service->P_default_route;
            trigDataPropagate = true;
        }

        if (server_entry->Prf != service->Prf) {
            server_entry->Prf = service->Prf;
            trigDataPropagate = true;
        }

        if (server_entry->P_preferred != service->P_preferred || server_entry->P_slaac != service->P_slaac) {
            server_entry->P_preferred = service->P_preferred;
            server_entry->P_slaac = service->P_slaac;
            trigDataPropagate = true;
        }

        if (server_entry->P_dhcp != service->P_dhcp) {
            server_entry->P_dhcp = service->P_dhcp;
            trigDataPropagate = true;
        }

        if (server_entry->P_on_mesh != service->P_on_mesh) {
            server_entry->P_on_mesh = service->P_on_mesh;
            trigDataPropagate = true;
        }

        if (server_entry->P_nd_dns != service->P_nd_dns) {
            server_entry->P_nd_dns = service->P_nd_dns;
            trigDataPropagate = true;
        }
    }

    if (trigDataPropagate) {
        if (service->stableData) {
            networkDataList->stableUpdatePushed = true;
        } else {
            networkDataList->temporaryUpdatePushed = true;
        }
    }

    return 0;
}


/**
 * Del DHCPv6 Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param service On Mesh prefix TLV
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_nd_local_list_del_on_mesh_server(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    thread_network_data_prefix_cache_entry_t *main_list;
    thread_network_server_data_entry_t *entry;

    if (!networkDataList) {
        return -1;
    }

    if (prefixTlv->PrefixLen) {
        return -1;
    }

    if (service->P_dhcp) {
        tr_debug("Del DHCPv6 server: %s",
                 trace_array(prefixTlv->Prefix, prefixBits_to_bytes(prefixTlv->PrefixLen)));
    } else {
        tr_debug("Del SLAAC server: %s",
                 trace_array(prefixTlv->Prefix, prefixBits_to_bytes(prefixTlv->PrefixLen)));
    }

    main_list = thread_prefix_entry_get(&networkDataList->localPrefixList, prefixTlv);
    if (!main_list) {
        return -1;
    }

    entry = thread_server_entry_search(&main_list->borderRouterList, service->routerID);

    if (!entry) {
        return -1;
    }

    entry->canDelete = true;
    if (entry->stableData) {
        networkDataList->stableUpdatePushed = true;
    } else {
        networkDataList->temporaryUpdatePushed = true;
    }

    return 0;
}

/**
 * Add new Local DHCPv6 Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param service On Mesh prefix TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_local_server_list_add_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service)
{
    int retVal = -1;
    tr_debug("Add prefix: %s prf:%d %s%s%s%s%s", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen), service->Prf,
             service->P_default_route?"Default Route ":"",service->P_dhcp?"DHCPv6 Server ":"", service->P_configure?"DHCPv6 Configuration ":"",
             service->P_slaac?"SLAAC ":"",service->P_preferred?"Preferred ":"");

    if (networkDataList) {
        thread_network_local_data_entry_t *prefix_entry = thread_local_prefix_entry_get(&networkDataList->prefix_list, prefixTlv);
        if (prefix_entry) {
            prefix_entry->preference = service->Prf;
            prefix_entry->configure = service->P_configure;
            prefix_entry->defaultRoute = service->P_default_route;
            prefix_entry->onMesh = service->P_on_mesh;
            prefix_entry->ndDns = service->P_nd_dns;

            if (service->P_dhcp) {
                prefix_entry->dhcpv6ServerActive = true;
                prefix_entry->dhcpv6ServerDataStable = service->stableData;
            }

            if (service->P_slaac) {
                prefix_entry->slaacServerActive = true;
                prefix_entry->slaacServerDataStable = service->stableData;
                prefix_entry->slaacPreferred = service->P_preferred;
            }

            if (prefixTlv->PrefixLen == 0) {
                // Adding as default route ::/0
                prefix_entry->routeActive = true;
                prefix_entry->routeDataStable = service->stableData;
            }
            retVal = 0;
        }
    }
    return retVal;
}

/**
 * Del Local DHCPv6 Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_local_server_list_del_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv)
{
    thread_network_local_data_entry_t *prefix_entry;

    if (!networkDataList) {
        return -1;
    }

    tr_debug("Delete prefix %s", trace_array(prefixTlv->Prefix, prefixBits_to_bytes(prefixTlv->PrefixLen)));

    prefix_entry = thread_local_prefix_entry_find(&networkDataList->prefix_list, prefixTlv);
    if (!prefix_entry) {
        return -1;
    }

    tr_debug("Prefix deleted");
    //Delete Entry
    ns_list_remove(&networkDataList->prefix_list, prefix_entry);
    ns_dyn_mem_free(prefix_entry);
    return 0;
}

/**
 * Modify or add new service information to the local service list.
 *
 * \param networkDataList Pointer main network data structure.
 * \param service Service information to store to the list.
 *
 * return 0, Operation OK
 * return <0 Operation not OK
 */
int thread_local_service_list_add(thread_network_local_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service)
{
    if (!networkDataList) {
        return -1;
    }

    tr_debug("Add service %"PRIu32": %s", service->S_enterprise_number, trace_array(service->S_service_data, service->S_service_data_length));

    thread_network_data_service_entry_t *service_entry = thread_local_service_entry_save(&networkDataList->service_list, service);

    if (!service_entry) {
        return -1;
    }

    tr_debug("Service added");

    return 0;
}

/**
 * Delete service information from the local service list.
 *
 * \param networkDataList Pointer main network data structure
 * \param service Service information to use in deletion.
 *
 * return 0, Delete OK
 * return <0 Delete not OK
 */
int thread_local_service_list_del(thread_network_local_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service)
{
    thread_network_data_service_entry_t *service_entry;

    if (!networkDataList || !service) {
        return -1;
    }

    tr_debug("Delete service %"PRIu32": %s", service->S_enterprise_number, trace_array(service->S_service_data, service->S_service_data_length));

    service_entry = thread_local_service_entry_find(&networkDataList->service_list, service);

    if (!service_entry) {
        return -1;
    }

    tr_debug("Service deleted");

    ns_list_remove(&networkDataList->service_list, service_entry);

    ns_dyn_mem_free(service_entry->S_service_data);

    if (service_entry->S_server_data) {
        ns_dyn_mem_free(service_entry->S_server_data);
    }

    ns_dyn_mem_free(service_entry);
    return 0;
}

/**
 * Add new local route information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param routePrefixPtr pointer to route prefix
 * \param prefixLength indicate prefix pointer valid information in bits
 * \param stableService Boolean true generate stable service, false temporary
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_local_server_add_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *route)
{
    tr_debug("Add Route: %s", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen));

    if (!networkDataList) {
        return -1;
    }

    thread_network_local_data_entry_t *prefix_entry = thread_local_prefix_entry_get(&networkDataList->prefix_list, prefixTlv);
    if (!prefix_entry) {
        return -1;
    }

    prefix_entry->routeActive = true;
    prefix_entry->routeDataStable = route->stableData;
    prefix_entry->preference = route->Prf;

    return 0;
}


/**
 * Del local route information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_local_server_del_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv)
{
    thread_network_local_data_entry_t *prefix_entry;
    tr_debug("Del Route: %s",
             trace_array(prefixTlv->Prefix, prefixBits_to_bytes(prefixTlv->PrefixLen)));

    if (!networkDataList) {
        return -1;
    }

    prefix_entry = thread_local_prefix_entry_find(&networkDataList->prefix_list, prefixTlv);
    if (!prefix_entry) {
        return -1;
    }

    prefix_entry->routeActive = false;
    if (prefix_entry->dhcpv6ServerActive) {
        return 0;
    } else if (prefix_entry->slaacServerActive) {
        return 0;
    }

    tr_debug("Free Entry");
    //Delete Entry
    ns_list_remove(&networkDataList->prefix_list, prefix_entry);
    ns_dyn_mem_free(prefix_entry);

    return 0;
}

static bool thread_nd_public_contex_id_allocated(thread_network_data_cache_entry_t *networkDataList, uint8_t tempId)
{
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        if (thread_get_context_by_id(&cur->contextList, tempId)) {
            return true;
        }
    }
    return false;
}

/**
 * Add new 6LoWPAN contexts information to Network Data list
 *
 * \param networkDataList Network Interface
 * \param prefixPtr pointer 6LoWPAN Contexts
 * \param prefixLength indicate prefix pointer valid information in bits
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
uint8_t thread_nd_context_id_allocate(thread_network_data_cache_entry_t *networkDataList, thread_network_local_data_cache_entry_t *localDataList, uint8_t *prefixPtr, uint8_t prefixLength)
{
    uint8_t cid = 16;
    uint8_t tempId;

    tr_debug("Disvover Context id for: %s",
             trace_ipv6_prefix(prefixPtr, prefixLength));

    if (!networkDataList || !localDataList) {
        return cid;
    } else if (!prefixLength) {
        return cid;
    }

    //Check first is context already allocated

    //Check Fisrt Public list
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        if (cur->servicesPrefixLen >= prefixLength) {
            if (bitsequal(cur->servicesPrefix, prefixPtr, prefixLength)) {

                //Check Prefix context List
                tempId = thread_get_context_id_by_length(&cur->contextList, prefixLength);
                if (tempId != 16) {
                    return tempId;
                }
            }
        }
    }

    //Allocate Free context id
    if (thread_extension_version_check(thread_version)) {
        tempId = 2;
    } else {
        tempId = 1;
    }
    for (; tempId < 16; tempId++) {
        if (thread_nd_public_contex_id_allocated(networkDataList, tempId)) {
            //Allocated
        } else {
            cid  = tempId;
            break;
        }
    }

    return cid;
}


/**
 * Add new 6LoWPAN contexts information to Network Data list
 *
 * \param networkDataList Network Data structure pointer
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param context Context TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_nd_local_list_add_contexts(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_network_local_data_context_entry_t *context)
{
    thread_network_data_prefix_cache_entry_t *main_list;
    thread_network_data_context_entry_t *cur;
    tr_debug("Add Public Context: %s", trace_ipv6_prefix(prefixTlv->Prefix, prefixTlv->PrefixLen));

    if (!networkDataList) {
        return -1;
    }

    if (!prefixTlv->PrefixLen) {
        return -1;
    }

    main_list = thread_prefix_entry_get(&networkDataList->localPrefixList, prefixTlv);
    if (!main_list) {
        return -1;
    }

    cur = thread_get_main_context_list(&main_list->contextList, context);
    if (!cur) {
        return -1;
    }
    cur->canDelete = false;
    if (context->stableData == true) {
        cur->stableData = true;
    }
    cur->compression = context->compression;
    cur->contextPrefixLength = context->contextPrefixLength;

    return 0;
}

uint8_t thread_server_prefix_length(thread_network_local_data_entry_t *cur)
{
    uint8_t tempLength = 0;


    if (cur->routeActive) {
        if (!((cur->slaacServerActive || cur->dhcpv6ServerActive) && cur->defaultRoute)) {
            // HasRoute is added if BorderRouter TLV does not have default route bit
            tempLength += 5;
        }
    }

    if (cur->dhcpv6ServerActive) {
        tempLength += 6;
    }

    if (cur->slaacServerActive) {
        tempLength += 6;
    }

    if (tempLength) {
        tempLength += prefixBits_to_bytes(cur->servicesPrefixLen);
        tempLength += 2; //Length &domainId
    }

    return tempLength;
}

static uint8_t thread_service_length(thread_network_data_service_entry_t *cur)
{
    uint8_t length = 0;

    length += 1; // T + S_id

    if (!cur->T) {
        length += 4; // S_enterprise_number
    }

    // Service data length + service data
    length += 1 + cur->S_service_data_length;

    if (cur->S_server_data) {
        length += 2; // Sub-TLV's type & length
        length += 2; // S_server_16
        length += cur->S_server_data_length;
    }

    return length;
}

uint16_t thread_nd_own_service_list_data_size(thread_network_local_data_cache_entry_t *serverDataList)
{
    uint16_t localDataLength = 0;
    uint16_t tempLength;

    ns_list_foreach(thread_network_data_service_entry_t, cur, &serverDataList->service_list) {
        tempLength = thread_service_length(cur);
        if (tempLength) {
            localDataLength += tempLength + 2; //Type & Length for service
        }
    }

    ns_list_foreach(thread_network_local_data_entry_t, cur, &serverDataList->prefix_list) {
        tempLength = thread_server_prefix_length(cur);
        if (tempLength) {
            localDataLength += tempLength + 2; //Type & Length for prefix
        }
    }

    return localDataLength;
}

static bool thread_check_local_data_prefix_stable_boolean(thread_network_local_data_entry_t *dataList)
{
    if (dataList->dhcpv6ServerActive && dataList->dhcpv6ServerDataStable) {
        return true;
    }

    if (dataList->slaacServerActive && dataList->slaacServerDataStable) {
        return true;
    }

    return false;
}

static uint8_t *thread_nd_hosted_prefix_header_write(uint8_t *ptr, uint8_t length,thread_network_local_data_entry_t *cur)
{
    uint8_t prefixBytesLen = prefixBits_to_bytes(cur->servicesPrefixLen);
    if (thread_check_local_data_prefix_stable_boolean(cur)) {
        *ptr++ = THREAD_NWK_DATA_TYPE_PREFIX | THREAD_NWK_STABLE_DATA;
    } else {
        *ptr++ = THREAD_NWK_DATA_TYPE_PREFIX;
    }
    *ptr++ = length;
    *ptr++ = cur->domainId;
    *ptr++ = cur->servicesPrefixLen;
    if (prefixBytesLen) {
        memset(ptr, 0, prefixBytesLen);
        bitcopy(ptr, cur->servicesPrefix, cur->servicesPrefixLen);
        ptr += prefixBytesLen;
    }
    return ptr;
}

static uint8_t *thread_nd_hosted_service_header_write(uint8_t *ptr, uint8_t length, thread_network_data_service_entry_t *cur)
{
    if (cur->S_stable) {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVICE_DATA | THREAD_NWK_STABLE_DATA;
    } else {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVICE_DATA;
    }

    *ptr++ = length;
    *ptr++ = (cur->T << 7) | cur->S_id;

    if (!cur->T) {
        ptr = common_write_32_bit(cur->S_enterprise_number, ptr);
    }

    *ptr++ = cur->S_service_data_length;

    memcpy(ptr, cur->S_service_data, cur->S_service_data_length);
    ptr += cur->S_service_data_length;

    return ptr;
}

static bool thread_nd_network_data_prefix_stable(thread_network_data_prefix_cache_entry_t *dataList)
{
    ns_list_foreach(thread_network_server_data_entry_t, cur, &dataList->borderRouterList) {
        if (cur->stableData) {
            return true;
        }
    }
    ns_list_foreach(thread_network_server_data_entry_t, cur, &dataList->routeList) {
        if (cur->stableData) {
            return true;
        }
    }
    ns_list_foreach(thread_network_data_context_entry_t, cur, &dataList->contextList) {
        if (cur->stableData) {
            return true;
        }
    }
    return false;
}

static uint8_t *thread_nd_prefix_header_write(uint8_t *ptr, thread_network_data_prefix_cache_entry_t *cur, uint8_t tlvLength)
{
    uint8_t prefixBytesLen = prefixBits_to_bytes(cur->servicesPrefixLen);
    if (thread_nd_network_data_prefix_stable(cur)) {
        *ptr++ = THREAD_NWK_DATA_TYPE_PREFIX | THREAD_NWK_STABLE_DATA;
    } else {
        *ptr++ = THREAD_NWK_DATA_TYPE_PREFIX;
    }
    *ptr++ = tlvLength;
    *ptr++ = cur->domainId;
    *ptr++ = cur->servicesPrefixLen;
    if (prefixBytesLen) {
        memcpy(ptr, cur->servicesPrefix,  prefixBytesLen);
        ptr += prefixBytesLen;
    }

    return ptr;
}

static bool thread_nd_network_data_service_stable(thread_network_data_service_cache_entry_t *dataList)
{
    ns_list_foreach(thread_network_data_service_server_entry_t, cur, &dataList->server_list) {
        if (cur->stable) {
            return true;
        }
    }
    return false;
}

static uint8_t *thread_nd_service_header_write(uint8_t *ptr, thread_network_data_service_cache_entry_t *cur, uint8_t tlvLength)
{
    if (thread_nd_network_data_service_stable(cur)) {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVICE_DATA | THREAD_NWK_STABLE_DATA;
    } else {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVICE_DATA;
    }

    *ptr++ = tlvLength;
    *ptr++ = (cur->T << 7) | cur->S_id;

    if (!cur->T) {
        ptr = common_write_32_bit(cur->S_enterprise_number, ptr);
    }

    *ptr++ = cur->S_service_data_length;

    memcpy(ptr, cur->S_service_data, cur->S_service_data_length);
    ptr += cur->S_service_data_length;

    return ptr;
}

static uint8_t *thread_nd_hosted_service_server_write(uint8_t *ptr, uint16_t router_id, thread_network_data_service_entry_t *cur)
{
    if (cur->S_stable) {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVER_DATA | THREAD_NWK_STABLE_DATA;
    } else {
        *ptr++ = THREAD_NWK_DATA_TYPE_SERVER_DATA;
    }

    *ptr++ = 2 + cur->S_server_data_length;
    ptr = common_write_16_bit(router_id, ptr);

    memcpy(ptr, cur->S_server_data, cur->S_server_data_length);
    ptr += cur->S_server_data_length;

    return ptr;
}

static uint8_t *thread_service_border_router_tlv_write(uint8_t *ptr, uint8_t tlvType, uint16_t routerId, uint16_t flags)
{
    *ptr++ = tlvType;
    *ptr++ = THREAD_BORDER_ROUTER_TLV_LENGTH;
    ptr = thread_nd_network_data_border_router_tlv_write(ptr, routerId, flags);
    return ptr;
}

static uint8_t *thread_service_has_route_tlv_write(uint8_t *ptr, uint8_t tlvType, uint16_t routerId, uint8_t preference)
{
    *ptr++ = tlvType;
    *ptr++ = THREAD_HAS_ROUTE_TLV_LENGTH;
     ptr = thread_nd_network_data_has_route_tlv_write(ptr, routerId, preference);
    return ptr;
}

uint8_t * thread_nd_own_service_list_data_write(thread_network_local_data_cache_entry_t *serverDataList, uint8_t *ptr, uint16_t routerID)
{
    uint8_t tlvType;
    uint8_t servicesLen;

    ns_list_foreach(thread_network_data_service_entry_t, cur, &serverDataList->service_list) {
        servicesLen = thread_service_length(cur);
        if (servicesLen) {
            ptr = thread_nd_hosted_service_header_write(ptr, servicesLen, cur);
            if (cur->S_server_data) {
                ptr = thread_nd_hosted_service_server_write(ptr, routerID, cur);
            }
        }
    }

    ns_list_foreach(thread_network_local_data_entry_t, cur, &serverDataList->prefix_list) {
        servicesLen = thread_server_prefix_length(cur);
        if (servicesLen) {
            ptr = thread_nd_hosted_prefix_header_write(ptr,servicesLen, cur);
            if (cur->routeActive) {
                if (!((cur->slaacServerActive || cur->dhcpv6ServerActive) && cur->defaultRoute)) {
                    // HasRoute is added if BorderRouter TLV does not have default route bit
                    uint8_t preference = 0;
                    tlvType = THREAD_NWK_DATA_TYPE_ROUTE;
                    if (cur->routeDataStable) {
                        tlvType |= THREAD_NWK_STABLE_DATA;
                    }
                    if (cur->preference) {
                        preference |= cur->preference << THREAD_HAS_ROUTE_PRF_BIT_MOVE;
                    }
                    ptr = thread_service_has_route_tlv_write(ptr, tlvType, routerID, preference);
                }
            }

            if (cur->slaacServerActive || cur->dhcpv6ServerActive) {
                uint16_t flags = 0;
                tlvType = THREAD_NWK_DATA_TYPE_BORDER_ROUTER;
                if (cur->slaacServerDataStable || cur->dhcpv6ServerDataStable) {
                    tlvType |= THREAD_NWK_STABLE_DATA;
                }
                if (cur->slaacServerActive) {
                    flags |= 1 << THREAD_P_SLAAC_BIT_MOVE;
                }
                if (cur->dhcpv6ServerActive) {
                    flags |= 1 << THREAD_P_DHCP_BIT_MOVE;
                }
                if (cur->slaacPreferred) {
                    flags |= 1 << THREAD_P_PREFERRED_BIT_MOVE;
                }
                if (cur->preference) {
                    flags |= cur->preference << THREAD_PRF_BIT_MOVE;
                }
                if (cur->configure) {
                    flags |= 1 << THREAD_P_CONFIGURE_BIT_MOVE;
                }
                if (cur->defaultRoute) {
                    flags |= 1 << THREAD_P_DEF_ROUTE_BIT_MOVE;
                }
                if (cur->onMesh) {
                    flags |= 1 << THREAD_P_ON_MESH_BIT_MOVE;
                }
                if (cur->ndDns) {
                    flags |= 1 << THREAD_P_ND_DNS_BIT_MOVE;
                }
                ptr = thread_service_border_router_tlv_write(ptr, tlvType, routerID, flags);
            } // slaac or dhcp
        }
    }
    return ptr;
}

bool thread_nd_dhcp_anycast_address_mapping_from_network_data(thread_network_data_cache_entry_t *networkDataList, uint16_t *rlocAddress, uint8_t contexId)
{
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        if (thread_get_context_by_id(&cur->contextList, contexId)) {
            //Check current Stable List
            thread_network_server_data_entry_t *server = thread_get_dhcp_server_from_list(&cur->borderRouterList);
            if (server) {
                *rlocAddress = server->routerID;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool thread_nd_service_anycast_address_mapping_from_network_data(thread_network_data_cache_entry_t *networkDataList, uint16_t *rlocAddress, uint8_t S_id)
{
    ns_list_foreach(thread_network_data_service_cache_entry_t, curService, &networkDataList->service_list) {
        // Go through all services
        if (curService->S_id != S_id) {
            continue;
        }
        ns_list_foreach(thread_network_data_service_server_entry_t, curServiceServer, &curService->server_list) {
            *rlocAddress = curServiceServer->router_id;
            return true;
        }
    }
    return false;
}

bool thread_nd_on_mesh_address_valid(thread_network_server_data_entry_t *curRoute)
{
    bool onMeshActive = false;
    if (curRoute->P_dhcp || curRoute->P_slaac || curRoute->P_preferred) {
        onMeshActive = true;
    }

    return onMeshActive;
}

thread_network_server_data_entry_t *thread_nd_hosted_by_this_routerid(uint16_t routerId, thread_network_server_data_list_t *list)
{
    ns_list_foreach(thread_network_server_data_entry_t, curInfo, list) {
        if (curInfo->routerID == routerId) {
            return curInfo;
        }
    }
    return NULL;
}
bool thread_network_data_services_registered(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID)
{
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &cachePtr->localPrefixList) {
        if (thread_nd_hosted_by_this_routerid(routerID, &cur->borderRouterList)) {
            return true;
        }
        if (thread_nd_hosted_by_this_routerid(routerID, &cur->routeList)) {
            return true;
        }
    }
    return false;
}

uint16_t thread_network_data_service_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList)
{
    uint16_t localDataLength = 0;
    uint16_t tempLength;

    ns_list_foreach(thread_network_data_service_cache_entry_t, cur, &networkDataList->service_list) {
        tempLength = thread_nd_service_based_on_list_entry_size(cur, fullList);
        if (tempLength) {
            localDataLength += tempLength + 2; //Type & Length for service
        }
    }

    return localDataLength;
}

uint16_t thread_network_data_prefix_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList)
{
    uint16_t localDataLength = 0;
    uint16_t tempLength;

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        tempLength = thread_nd_prefix_based_on_list_entry_size(cur, fullList);
        if (tempLength) {
            localDataLength += tempLength + 2; //Type & Length for prefix
        }
    }

    return localDataLength;
}

uint8_t *thread_network_data_prefix_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr)
{
    uint8_t tlvLength;

    //Add always Prefix based data first and Border Router List after That
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        tlvLength = thread_nd_prefix_based_on_list_entry_size(cur, true);
        if (tlvLength) {
            ptr = thread_nd_prefix_header_write(ptr, cur, tlvLength);
            tr_debug("Prefix: %s", trace_ipv6_prefix(cur->servicesPrefix, cur->servicesPrefixLen));

            //Follow canonical order first temporary data and then stable
            //Stable & Temporary subTLVS need to be smallest first also
            //SET Route always to 1. THREAD_NWK_DATA_TYPE_ROUTE
            //DHCPv6 Services 2. THREAD_NWK_DATA_TYPE_BORDER_ROUTER
            //SET Context always to 3. THREAD_NWK_DATA_TYPE_6LOWPAN_ID

            //write Temporary List this includes the stable and unstable servers with correct address.
            ptr = thread_nd_server_list_write(&cur->routeList, ptr, THREAD_NWK_DATA_TYPE_ROUTE, false);
            ptr = thread_nd_server_list_write(&cur->borderRouterList, ptr, THREAD_NWK_DATA_TYPE_BORDER_ROUTER, false);
            ptr = thread_nd_context_list_write(&cur->contextList, ptr, false);

            ptr = thread_nd_server_list_write(&cur->routeList, ptr, THREAD_NWK_DATA_TYPE_ROUTE, true);
            ptr = thread_nd_server_list_write(&cur->borderRouterList, ptr, THREAD_NWK_DATA_TYPE_BORDER_ROUTER, true);
            ptr = thread_nd_context_list_write(&cur->contextList, ptr, true);
        }
    }
    return ptr;
}

uint8_t *thread_network_data_service_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr)
{
    uint8_t tlvLength;

    ns_list_foreach(thread_network_data_service_cache_entry_t, cur, &networkDataList->service_list) {
        tlvLength = thread_nd_service_based_on_list_entry_size(cur, true);
        if (tlvLength) {
            ptr = thread_nd_service_header_write(ptr, cur, tlvLength);
            tr_debug("Service: sid:%d e:%"PRIu32" data:%s",cur->S_id, cur->S_enterprise_number, trace_array(cur->S_service_data, cur->S_service_data_length));
            ptr = thread_nd_service_server_list_write(&cur->server_list, ptr, true);
        }
    }

    return ptr;
}

bool thread_network_data_service_hosted_by_this_router_id(thread_network_data_service_cache_entry_t *dataList, uint16_t router_id)
{
    ns_list_foreach(thread_network_data_service_server_entry_t, cur, &dataList->server_list) {
        if (cur->router_id == router_id) {
            return true;
        }
    }
    return false;
}

uint16_t thread_network_data_service_child_id_from_networkdata_get(thread_network_data_cache_entry_t *networkDataList, uint16_t router_short_addr)
{
    ns_list_foreach(thread_network_data_service_cache_entry_t, service, &networkDataList->service_list) {
        ns_list_foreach(thread_network_data_service_server_entry_t, server, &service->server_list) {
            if (thread_addr_is_child(router_short_addr, server->router_id)) {
                return server->router_id;
            }
        }
    }

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, cur, &networkDataList->localPrefixList) {
        ns_list_foreach(thread_network_server_data_entry_t, curRoute, &cur->routeList) {
            if (thread_addr_is_child(router_short_addr, curRoute->routerID)) {
                return curRoute->routerID;
            }
        }

        ns_list_foreach(thread_network_server_data_entry_t, curBR, &cur->borderRouterList) {
            if (thread_addr_is_child(router_short_addr, curBR->routerID)) {
                return curBR->routerID;
            }
        }
    }

    return 0;
}
#endif
