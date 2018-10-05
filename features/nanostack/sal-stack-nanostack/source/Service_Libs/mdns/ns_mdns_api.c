/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "fnet.h"
#include "fnet_netif_prv.h"

#include "ns_types.h"
#include "ns_list.h"
#include "nsdynmemLIB.h"
#include "ns_mdns_api.h"
#include "ns_fnet_events.h"
#include "common_functions.h" // common_write
#include "ns_trace.h"

#define TRACE_GROUP "mDNS"

/* Structure for list of services */
typedef struct {
    fnet_mdns_service_t service_opt;        // service options holding service parameters
    fnet_mdns_service_desc_t service_desc;  // service descriptor received from fnet
    ns_list_link_t link;
} ns_mdns_service_entry_t;

typedef NS_LIST_HEAD(ns_mdns_service_entry_t, link) ns_mdns_service_entry_list;

/* Structure for list of servers */
typedef struct {
    fnet_netif_t *ns_netif;         // network interface
    fnet_mdns_desc_t mdns_desc;     // server descriptor from fnet
    ns_mdns_service_entry_list service_list;    // list of services added to this server
    ns_list_link_t link;
} ns_mdns_server_entry_t;

static NS_LIST_DEFINE(server_list, ns_mdns_server_entry_t, link);

/*
 * Find service_entry from a server list based on service_descriptor
 */
static ns_mdns_service_entry_t* ns_mdns_service_entry_find(fnet_mdns_service_desc_t service_desc)
{
    ns_list_foreach(ns_mdns_server_entry_t, cur_server_entry_ptr, &server_list) {
        ns_list_foreach(ns_mdns_service_entry_t, cur_service__entry_ptr, &cur_server_entry_ptr->service_list) {
            if (cur_service__entry_ptr->service_desc == service_desc ) {
                return cur_service__entry_ptr;
            }
        }
    }
    return NULL;
}

/*
 * Find server_entry based on service_entry
 */
static ns_mdns_server_entry_t* ns_mdns_server_entry_find_by_service(ns_mdns_service_entry_t *service_entry)
{
    ns_list_foreach(ns_mdns_server_entry_t, cur_server_entry_ptr, &server_list) {
        ns_list_foreach(ns_mdns_service_entry_t, cur_service_entry_ptr, &cur_server_entry_ptr->service_list) {
            if (cur_service_entry_ptr == service_entry ) {
                return cur_server_entry_ptr;
            }
        }
    }
    return NULL;
}

/*
 * Find server entry based on server_descriptor
 */
static ns_mdns_server_entry_t* ns_mdns_server_entry_find(fnet_mdns_desc_t mdns_desc)
{
    ns_list_foreach(ns_mdns_server_entry_t, cur_server__entry_ptr, &server_list) {
        if (cur_server__entry_ptr->mdns_desc == mdns_desc ) {
            return cur_server__entry_ptr;
        }
    }
    return NULL;
}

static ns_mdns_service_entry_t* ns_mdns_service_entry_create(ns_mdns_server_entry_t *server_entry, ns_mdns_service_param_t *service)
{
    if (!service || !service->service_type || !service->service_get_txt || !server_entry) {
        return NULL;
    }

    ns_mdns_service_entry_t *mdns_service = ns_dyn_mem_alloc(sizeof(ns_mdns_service_entry_t));
    if (!mdns_service) {
        return NULL;
    }
    mdns_service->service_opt.service_type = ns_dyn_mem_alloc(strlen(service->service_type) + 1);
    if (!mdns_service->service_opt.service_type) {
        ns_dyn_mem_free(mdns_service);
        return NULL;
    }
    strcpy((char*)mdns_service->service_opt.service_type, service->service_type);
    // change port to be in network byte order
    mdns_service->service_opt.service_port = FNET_HTONS(service->service_port);
    mdns_service->service_opt.service_get_txt = service->service_get_txt;

    ns_list_add_to_end(&server_entry->service_list, mdns_service);
    return mdns_service;
}

/*
 * Free service entry and remove it from the list
 */
static void ns_mdns_service_entry_free(ns_mdns_server_entry_t *server_entry, ns_mdns_service_entry_t *service_entry)
{
    if (service_entry) {
        ns_list_remove(&server_entry->service_list, service_entry);
        ns_dyn_mem_free((void*)service_entry->service_opt.service_type);
        ns_dyn_mem_free(service_entry);
    }
}

ns_mdns_t ns_mdns_server_start(const char *server_name, uint32_t ttl, uint32_t ttl_ip, int8_t interface_id)
{
    FNET_DEBUG("ns_mdns_server_start(), interface=%d", interface_id);
    struct fnet_mdns_params params;
    fnet_netif_t *ns_netif;
    ns_mdns_server_entry_t *server_entry;
    fnet_mdns_desc_t mdns_desc;

    if (interface_id < 0) {
        return NULL;
    }

    server_entry = ns_dyn_mem_alloc(sizeof(ns_mdns_server_entry_t));
    if (!server_entry) {
       return NULL;
    }

    ns_netif = ns_dyn_mem_alloc(sizeof(fnet_netif_t));
    if (!ns_netif) {
        ns_dyn_mem_free(server_entry);
       return NULL;
    }
    ns_netif->scope_id = interface_id;
    server_entry->ns_netif = ns_netif;

    params.netif_desc = (fnet_mdns_desc_t)ns_netif;
    params.name = server_name;
    params.rr_ttl = ttl;
    params.rr_ttl_ip = ttl_ip;
    params.addr_family = AF_INET6;

    ns_fnet_events_start();

    /* Start mDNS server */
    mdns_desc = fnet_mdns_init(&params);
    if (!mdns_desc) {
        ns_dyn_mem_free(ns_netif);
        ns_dyn_mem_free(server_entry);
        tr_error("mdns init failure");
        return NULL;
    }

    server_entry->mdns_desc = mdns_desc;

    ns_list_init(&server_entry->service_list);
    ns_list_add_to_end(&server_list, server_entry);
    return mdns_desc;
}

void ns_mdns_server_stop(ns_mdns_t ns_mdns)
{
    FNET_DEBUG("ns_mdns_server_stop()");
    ns_mdns_server_entry_t *server_entry;

    server_entry = ns_mdns_server_entry_find(ns_mdns);
    if (server_entry) {
        fnet_mdns_release((fnet_mdns_desc_t)ns_mdns);
        ns_list_remove(&server_list, server_entry);

        ns_list_foreach_safe(ns_mdns_service_entry_t, cur_service_entry_ptr, &server_entry->service_list) {
            // free all services from the current server
            ns_mdns_service_entry_free(server_entry, cur_service_entry_ptr);
        }

        ns_dyn_mem_free(server_entry->ns_netif);
        ns_dyn_mem_free(server_entry);
    }

   if (ns_list_is_empty(&server_list)) {
       // No more servers in the system, stop events
       ns_fnet_events_stop();
   }
}

ns_mdns_service_t ns_mdns_service_register(ns_mdns_t ns_mdns, ns_mdns_service_param_t *service)
{
    ns_mdns_server_entry_t *server_entry = ns_mdns_server_entry_find(ns_mdns);
    ns_mdns_service_entry_t *mdns_service_entry = ns_mdns_service_entry_create(server_entry, service);

    if (!mdns_service_entry) {
        tr_error("Failed to create service entry");
        return NULL;
    }

    mdns_service_entry->service_desc = fnet_mdns_service_register((fnet_mdns_desc_t)ns_mdns, &mdns_service_entry->service_opt);
    return mdns_service_entry->service_desc;
}

void ns_mdns_service_unregister(ns_mdns_service_t service_desc)
{
    ns_mdns_service_entry_t *service_entry;
    ns_mdns_server_entry_t *server_entry;

    // find service from a server
    service_entry = ns_mdns_service_entry_find(service_desc);
    // find server that holds current server
    server_entry = ns_mdns_server_entry_find_by_service(service_entry);
    if (service_entry && server_entry) {
        fnet_mdns_service_unregister(service_desc);
        ns_mdns_service_entry_free(server_entry, service_entry);
    }
}

void ns_mdns_announcement_send(ns_mdns_t ns_mdns)
{
    FNET_DEBUG("ns_mdns_announcement_send()");
    if (ns_mdns_server_entry_find(ns_mdns)) {
        fnet_mdns_announce((fnet_mdns_desc_t)ns_mdns);
    }
}
