/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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

#include "nsconfig.h"
#ifdef HAVE_ND_PROXY
#include "ns_types.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"

#define TRACE_GROUP "prox"

/**
 * Downstream Interface list
 */
typedef struct nd_proxy_downstream_list {
    int8_t id; /**< Proxy Interface Id*/
    bridge_state_update_cb *bridge_status_update; /**< Update bridge state */
    nd_proxy_req_cb *nd_proxy_validation; /**< Validate NS ND proxy response */
    ns_list_link_t link;
} nd_proxy_downstream_list_s;

/**
 * Upstream Interface list
 */
typedef struct nd_proxy_upstream_list {
    int8_t id; /**< Upstream Interface Id*/
    nd_proxy_req_cb *route_on_link_validation; /**< Mesh Can validate OnLink Route */
    ns_list_link_t link;
} nd_proxy_upstream_list_s;

/**
 * Linked Downstream interface list to Upstream
 */
typedef struct nd_proxy_downstream_interfaces_list {
    int8_t id; /**< Downstream Interface Id*/
    ns_list_link_t link;
} nd_proxy_downstream_interfaces_list_s;

typedef NS_LIST_HEAD(nd_proxy_upstream_list_s, link) upstream_interface_list_t;

typedef NS_LIST_HEAD(nd_proxy_downstream_list_s, link) downstream_interface_list_t;

typedef NS_LIST_HEAD(nd_proxy_downstream_interfaces_list_s, link) proxy_connected_downstream_list_t;

/**
 * Linked Upstream and Downstream interface list
 */
typedef struct nd_proxy_connected_list {
    int8_t id; /**< Upstream Interface Id*/
    proxy_connected_downstream_list_t connected_downstream_list; /**< Linked Downstream Interfaces to this interface*/
    ns_list_link_t link;
} nd_proxy_connected_list_s;

typedef NS_LIST_HEAD(nd_proxy_connected_list_s, link) proxy_connected_interface_list_t;

/* List of registered Upstream interfaces */
static upstream_interface_list_t upstream_interface_list;

/* List of registered Downstream interfaces */
static downstream_interface_list_t downstream_interface_list;

/* List of connected Upstream interfaces */
static proxy_connected_interface_list_t paired_interface_list;

/**
 * Get or allocate new Downstream interface entry
 *
 */
static nd_proxy_downstream_list_s *proxy_cache_downstream_interface_allocate(int8_t interface, downstream_interface_list_t *list)
{
    ns_list_foreach(nd_proxy_downstream_list_s, e, list) {
        if (e->id == interface) {
            return e;
        }
    }
    nd_proxy_downstream_list_s *entry = ns_dyn_mem_alloc(sizeof(nd_proxy_downstream_list_s));
    if (entry) {
        entry->id = interface;
        ns_list_add_to_start(list, entry);
    }

    return entry;
}

/**
 * Get or allocate new Upstream interface entry
 *
 */
static nd_proxy_upstream_list_s *proxy_cache_upstream_interface_allocate(int8_t interface, upstream_interface_list_t *list)
{
    ns_list_foreach(nd_proxy_upstream_list_s, e, list) {
        if (e->id == interface) {
            return e;
        }
    }
    nd_proxy_upstream_list_s *entry = ns_dyn_mem_alloc(sizeof(nd_proxy_upstream_list_s));
    if (entry) {
        entry->id = interface;
        ns_list_add_to_start(list, entry);
    }

    return entry;
}

/**
 * Get registered Downstream interface entry
 *
 */
static nd_proxy_downstream_list_s *proxy_cache_downstream_interface_get(int8_t interface, downstream_interface_list_t *list)
{
    ns_list_foreach(nd_proxy_downstream_list_s, e, list) {
        if (e->id == interface) {
            return e;
        }
    }
    return NULL;
}

/**
 * Get registered Upstream interface entry
 *
 */
static nd_proxy_upstream_list_s *proxy_cache_upstream_interface_get(int8_t interface, upstream_interface_list_t *list)
{
    ns_list_foreach(nd_proxy_upstream_list_s, e, list) {
        if (e->id == interface) {
            return e;
        }
    }
    return NULL;
}

/**
 * Get linked proxy entry by Upstream interface id
 *
 */
static nd_proxy_connected_list_s *proxy_upstream_conection_get(int8_t interface_id)
{
    ns_list_foreach(nd_proxy_connected_list_s, e, &paired_interface_list) {
        if (e->id == interface_id) {
            return e;
        }
    }
    return NULL;
}

/**
 * Get or allocate new proxy entry by Upstream interface id
 *
 */
static nd_proxy_connected_list_s *proxy_upstream_connection_allocate(int8_t interface_id)
{
    ns_list_foreach(nd_proxy_connected_list_s, e, &paired_interface_list) {
        if (e->id == interface_id) {
            return e;
        }
    }

    nd_proxy_connected_list_s *entry = ns_dyn_mem_alloc(sizeof(nd_proxy_connected_list_s));
    if (entry) {
        entry->id = interface_id;
        ns_list_init(&entry->connected_downstream_list);
        ns_list_add_to_start(&paired_interface_list, entry);
    }
    return entry;
}

/**
 * Link Downstream interface to allocated proxy
 *
 */
static bool proxy_downstream_conection_allocate(int8_t interface_id, proxy_connected_downstream_list_t *list)
{
    ns_list_foreach(nd_proxy_downstream_interfaces_list_s, e, list) {
        if (e->id == interface_id) {
            return true;
        }
    }

    nd_proxy_downstream_interfaces_list_s *entry = ns_dyn_mem_alloc(sizeof(nd_proxy_downstream_interfaces_list_s));
    if (entry) {
        entry->id = interface_id;
        ns_list_add_to_start(list, entry);
        return true;
    }
    return false;
}

/**
 * Remove Downstream interface from linked Upstream list
 *
 */
static bool proxy_cache_untie_connection_by_downstream(int8_t downstream_id)
{
    bool ret_val = false;
    ns_list_foreach_safe(nd_proxy_connected_list_s, e, &paired_interface_list) {

        ns_list_foreach_safe(nd_proxy_downstream_interfaces_list_s, downstream, &e->connected_downstream_list) {
            if (downstream->id == downstream_id) {
                //Remove from the list and free
                ns_list_remove(&e->connected_downstream_list, downstream);
                ns_dyn_mem_free(downstream);
                ret_val = true;
            }
        }

        /* Free proxy connection if all mesh interfaces are disconnected */
        if (ns_list_is_empty(&e->connected_downstream_list)) {
            //Remove connection from list and free memory
            ns_list_remove(&paired_interface_list, e);
            ns_dyn_mem_free(e);
        }

    }
    return ret_val;
}

/**
 * Remove Upstream interface from connected list
 *
 * Function will tell to mesh interface by registered function pointer disabled proxy functionality
 *
 */
static bool proxy_cache_untie_connection_by_upstream(int8_t upstream_id)
{
    nd_proxy_downstream_list_s *downstream_interface;

    ns_list_foreach_safe(nd_proxy_connected_list_s, e, &paired_interface_list) {

        if (e->id != upstream_id) {
            continue;
        }

        ns_list_foreach_safe(nd_proxy_downstream_interfaces_list_s, downstream, &e->connected_downstream_list) {
            //Remove from the list and free
            downstream_interface = proxy_cache_downstream_interface_get(downstream->id, &downstream_interface_list);
            if (downstream_interface) {
                //Indicate Downstream for missing Upstream
                if (downstream_interface->bridge_status_update) {
                    downstream_interface->bridge_status_update(upstream_id, downstream->id, false);
                }
            }

            ns_list_remove(&e->connected_downstream_list, downstream);
            ns_dyn_mem_free(downstream);
        }

        //Remove connection from list and free memory
        ns_list_remove(&paired_interface_list, e);
        ns_dyn_mem_free(e);
        return true;

    }
    return false;
}

/**
 * Generate Proxy between Upstream and Downstream interface
 *
 * Function will tell to Downstream interface by registered function pointer enabled proxy functionality
 *
 */
static int proxy_cache_interface_enable_proxy(int8_t upstream_id, int8_t downstream_id)
{
    //validate first current connection
    nd_proxy_connected_list_s *proxy = NULL;
    nd_proxy_downstream_list_s *downstream_interface = proxy_cache_downstream_interface_get(downstream_id, &downstream_interface_list);
    if (!downstream_interface) {
        tr_error("Unknown Downstream id");
        return -1;
    }

    if (!proxy_cache_upstream_interface_get(upstream_id, &upstream_interface_list)) {
        tr_error("Unknown Upstream id");
        return -1;
    }

    ns_list_foreach(nd_proxy_connected_list_s, e, &paired_interface_list) {

        if (e->id != upstream_id) {
            continue;
        }

        //Check Downstream
        ns_list_foreach_safe(nd_proxy_downstream_interfaces_list_s, mesh, &e->connected_downstream_list) {
            if (mesh->id == downstream_id) {
                return 0;
            }
        }
    }

    //Allocate new Upstream and connection
    proxy = proxy_upstream_connection_allocate(upstream_id);

    if (!proxy) {
        tr_error("Proxy alloc fail");
        return -1;
    }

    if (!proxy_downstream_conection_allocate(downstream_id, &proxy->connected_downstream_list)) {
        tr_error("Up<->Down stream connect alloc fail");
        if (ns_list_is_empty(&proxy->connected_downstream_list)) {
            //Remove connection from list and free memory
            ns_list_remove(&paired_interface_list, proxy);
            ns_dyn_mem_free(proxy);
        }
        return -1;
    }

    if (downstream_interface->bridge_status_update) {
        downstream_interface->bridge_status_update(upstream_id, downstream_id, true);
    }
    return 0;
}

int nd_proxy_downstream_interface_register(int8_t interface_id, nd_proxy_req_cb *nd_proxy_req, bridge_state_update_cb *bridge_state_update)
{
    if (interface_id < 0) {
        return -1;
    } else if (!nd_proxy_req) {
        return -1;
    }
    nd_proxy_downstream_list_s *entry = proxy_cache_downstream_interface_allocate(interface_id, &downstream_interface_list);
    if (!entry) {
        return -2;
    }
    //Set Function pointers
    entry->bridge_status_update = bridge_state_update;
    entry->nd_proxy_validation = nd_proxy_req;

    ns_list_foreach(nd_proxy_upstream_list_s, e, &upstream_interface_list) {

        if (proxy_cache_interface_enable_proxy(e->id, interface_id) == 0) {
            tr_debug("Proxy bridge enabled for interface %i to %i\n", e->id, interface_id);
        }
    }

    return 0;
}

int nd_proxy_downstream_interface_unregister(int8_t interface_id)
{
    //Release from paired
    proxy_cache_untie_connection_by_downstream(interface_id);

    ns_list_foreach(nd_proxy_downstream_list_s, e, &downstream_interface_list) {
        if (e->id == interface_id) {
            ns_list_remove(&downstream_interface_list, e);
            ns_dyn_mem_free(e);
            return 0;
        }
    }

    return -1;
}

int nd_proxy_upstream_interface_register(int8_t interface_id, nd_proxy_req_cb *route_validation_req)
{
    if (interface_id < 0) {
        return -1;
    } else if (!route_validation_req) {
        return -1;
    }

    nd_proxy_upstream_list_s *entry = proxy_cache_upstream_interface_allocate(interface_id, &upstream_interface_list);
    if (!entry) {
        return -1;
    }
    //Set Function pointers
    entry->route_on_link_validation = route_validation_req;

    //Link now all available interfaces which give proxy service
    ns_list_foreach(nd_proxy_downstream_list_s, e, &downstream_interface_list) {

        if (proxy_cache_interface_enable_proxy(interface_id, e->id) == 0) {
            tr_debug("Proxy bridge enabled for interface %i to %i \n", interface_id, e->id);
        }
    }
    return 0;
}

int nd_proxy_upstream_interface_unregister(int8_t interface_id)
{
    proxy_cache_untie_connection_by_upstream(interface_id);

    ns_list_foreach(nd_proxy_upstream_list_s, e, &upstream_interface_list) {
        if (e->id == interface_id) {
            ns_list_remove(&upstream_interface_list, e);
            ns_dyn_mem_free(e);
            return 0;
        }
    }

    return -1;
}

bool nd_proxy_enabled_for_downstream(int8_t interface_id)
{
    if (proxy_upstream_conection_get(interface_id)) {
        return true;
    }
    return false;
}

bool nd_proxy_enabled_for_upstream(int8_t interface_id)
{
    ns_list_foreach(nd_proxy_connected_list_s, e, &paired_interface_list) {

        ns_list_foreach(nd_proxy_downstream_interfaces_list_s, mesh, &e->connected_downstream_list) {
            if (mesh->id == interface_id) {
                return true;
            }
        }
    }
    return false;
}

bool nd_proxy_target_address_validation(int8_t upstream_id, uint8_t *address)
{
    nd_proxy_downstream_list_s *downstream;
    nd_proxy_connected_list_s *upstream = proxy_upstream_conection_get(upstream_id);

    if (!upstream) {
        return false;
    }

    ns_list_foreach(nd_proxy_downstream_interfaces_list_s, downstream_entry, &upstream->connected_downstream_list) {
        downstream = proxy_cache_downstream_interface_get(downstream_entry->id, &downstream_interface_list);
        if (downstream) {
            if (downstream->nd_proxy_validation(downstream_entry->id, address) == 0) {
                return true;
            }
        }
    }

    return false;
}

bool nd_proxy_upstream_route_onlink(int8_t downstream_id, uint8_t *address)
{
    ns_list_foreach(nd_proxy_connected_list_s, e, &paired_interface_list) {

        ns_list_foreach(nd_proxy_downstream_interfaces_list_s, downstream, &e->connected_downstream_list) {
            if (downstream->id == downstream_id) {
                nd_proxy_upstream_list_s *upstream = proxy_cache_upstream_interface_get(e->id, &upstream_interface_list);
                if (upstream) {
                    if (upstream->route_on_link_validation(e->id, address) == 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
#endif /* HAVE_ND_PROXY */

