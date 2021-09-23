/*
 * Copyright (c) 2020-2021, Pelion and affiliates.
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
#include "ns_types.h"
#include "ns_trace.h"
#include <string.h>
#include "net_dns_internal.h"
#include "net_interface.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "libNET/src/net_dns_internal.h"
#define TRACE_GROUP "Ndns"


typedef struct dns_server_info {
    uint8_t address[16];
    uint8_t dns_server_address[16];
    uint32_t lifetime;
    uint8_t *dns_search_list_ptr;
    uint16_t dns_search_list_len;
    int8_t interface_id;
    ns_list_link_t link;
} dns_server_info_t;

static NS_LIST_DEFINE(dns_server_list, dns_server_info_t, link);

typedef struct dns_query {
    uint8_t address[16];
    char *domain_str;
    uint32_t lifetime;
    int8_t interface_id;
    ns_list_link_t link;
} dns_query_t;

static NS_LIST_DEFINE(dns_query_list, dns_query_t, link);

static dns_server_info_t *dns_server_info_find(int8_t interface_id, const uint8_t address[16])
{
    dns_server_info_t *this = NULL;
    ns_list_foreach(dns_server_info_t, cur_ptr, &dns_server_list) {
        if (interface_id != -1 && cur_ptr->interface_id != interface_id) {
            continue;
        }
        if (address && memcmp(cur_ptr->address, address, 16) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static dns_server_info_t *dns_server_info_create(int8_t interface_id, const uint8_t address[16])
{
    dns_server_info_t *this = ns_dyn_mem_alloc(sizeof(dns_server_info_t));

    if (!this) {
        return NULL;
    }

    memset(this, 0, sizeof(dns_server_info_t));
    this->interface_id = interface_id;
    memcpy(this->address, address, 16);
    ns_list_add_to_start(&dns_server_list, this);
    tr_debug("Create DNS entry for %s", trace_ipv6(address));

    return this;
}

static void dns_server_info_delete(dns_server_info_t *this)
{
    if (!this) {
        return;
    }

    tr_debug("delete DNS entry for %s", trace_ipv6(this->address));
    ns_list_remove(&dns_server_list, this);
    ns_dyn_mem_free(this->dns_search_list_ptr);
    ns_dyn_mem_free(this);
}


int8_t net_dns_server_address_set(int8_t interface_id, const uint8_t address[16], const uint8_t dns_server_address[16], uint32_t lifetime)
{
    dns_server_info_t *info_ptr;

    if (!address || interface_id < 0) {
        return -1;
    }
    info_ptr = dns_server_info_find(interface_id, address);

    if (!dns_server_address || lifetime == 0) {
        // Delete the entry
        dns_server_info_delete(info_ptr);
        return 0;
    }

    if (!info_ptr) {
        info_ptr = dns_server_info_create(interface_id, address);
        // Trace only when new entry is created
        tr_info("DNS Server: %s from: %s Lifetime: %lu", trace_ipv6(dns_server_address), trace_ipv6(info_ptr->address), (unsigned long) lifetime);
    }
    info_ptr->lifetime = lifetime;
    memcpy(info_ptr->dns_server_address, dns_server_address, 16);
    return 0;
}

int8_t net_dns_server_search_list_set(int8_t interface_id, const uint8_t address[16], uint8_t *dns_search_list_ptr, uint8_t dns_search_list_len, uint32_t lifetime)
{
    dns_server_info_t *info_ptr;

    if (!address || interface_id < 0) {
        return -1;
    }
    info_ptr = dns_server_info_find(interface_id, address);

    if (info_ptr && (!dns_search_list_ptr || lifetime == 0)) {
        // remove search list information
        tr_debug("DNS Search List clear");
        ns_dyn_mem_free(info_ptr->dns_search_list_ptr);
        info_ptr->dns_search_list_ptr = NULL;
        info_ptr->dns_search_list_len = 0;
        return 0;
    }

    if (!info_ptr) {
        info_ptr = dns_server_info_create(interface_id, address);
        if (!info_ptr) {
            return -1;
        }
        info_ptr->lifetime = lifetime;
    }

    if (info_ptr->dns_search_list_ptr && info_ptr->dns_search_list_len != dns_search_list_len) {
        ns_dyn_mem_free(info_ptr->dns_search_list_ptr);
        info_ptr->dns_search_list_ptr = NULL;
        info_ptr->dns_search_list_len = 0;
    }

    if (dns_search_list_len) {
        if (!info_ptr->dns_search_list_ptr) {
            info_ptr->dns_search_list_ptr = ns_dyn_mem_alloc(dns_search_list_len);

            tr_info("DNS Search List: %s Lifetime: %lu", trace_array(dns_search_list_ptr, dns_search_list_len), (unsigned long) info_ptr->lifetime);
        }

        if (!info_ptr->dns_search_list_ptr) {
            return -2;
        }
        memcpy(info_ptr->dns_search_list_ptr, dns_search_list_ptr, dns_search_list_len);
        info_ptr->dns_search_list_len = dns_search_list_len;
    }
    return 0;
}

int8_t net_dns_server_get(int8_t interface_id, uint8_t dns_server_address[16], uint8_t **dns_search_list_ptr, uint8_t *dns_search_list_len, uint8_t index)
{
    dns_server_info_t *info_ptr = NULL;
    uint8_t n = 0;
    ns_list_foreach(dns_server_info_t, cur_ptr, &dns_server_list) {
        if (interface_id != -1 && cur_ptr->interface_id != interface_id) {
            continue;
        }
        if (index == n) {
            info_ptr = cur_ptr;
            break;
        }
        n++;
    }

    if (!info_ptr) {
        return -1;
    }
    if (dns_server_address) {
        memcpy(dns_server_address, info_ptr->dns_server_address, 16);
    }
    if (dns_search_list_ptr) {
        *dns_search_list_ptr = info_ptr->dns_search_list_ptr;
    }
    if (dns_search_list_len) {
        *dns_search_list_len = info_ptr->dns_search_list_len;
    }
    return 0;
}

/**
 * Storage for DNS query results
 */

static dns_query_t *dns_query_result_find(int8_t interface_id, const char *domain_str)
{
    dns_query_t *this = NULL;
    ns_list_foreach(dns_query_t, cur_ptr, &dns_query_list) {
        if (interface_id != -1 && cur_ptr->interface_id != interface_id) {
            continue;
        }
        if (strcasecmp(cur_ptr->domain_str, domain_str) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static dns_query_t *dns_query_result_create(int8_t interface_id, const char *domain_str)
{
    dns_query_t *this = NULL;

    if (!domain_str) {
        return NULL;
    }
    this = ns_dyn_mem_alloc(sizeof(dns_query_t));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(dns_query_t));

    this->domain_str = ns_dyn_mem_alloc(strlen(domain_str) + 1);
    if (!this->domain_str) {
        ns_dyn_mem_free(this);
        return NULL;
    }
    this->interface_id = interface_id;
    strcpy(this->domain_str, domain_str);
    //tr_debug("Create DNS query entry for %s", this->domain_str);
    ns_list_add_to_start(&dns_query_list, this);
    return this;
}

static void dns_query_result_delete(dns_query_t *this)
{
    if (!this) {
        return;
    }

    tr_debug("Delete DNS query entry for %s", this->domain_str);
    ns_list_remove(&dns_query_list, this);
    ns_dyn_mem_free(this->domain_str);
    ns_dyn_mem_free(this);
}

int8_t net_dns_query_result_set(int8_t interface_id, const uint8_t address[16], const char *domain_name_ptr, uint32_t lifetime)
{
    dns_query_t *this;

    if (!domain_name_ptr || interface_id < 0) {
        return -1;
    }
    this = dns_query_result_find(interface_id, domain_name_ptr);

    if (!address || lifetime == 0) {
        // Delete the entry
        dns_query_result_delete(this);
        return 0;
    }

    if (!this) {
        this = dns_query_result_create(interface_id, domain_name_ptr);
    }

    if (!this) {
        return -2;
    }
    // update address and lifetime also to old query results
    memcpy(this->address, address, 16);
    this->lifetime = lifetime;
    tr_info("DNS query set: %s address  %s Lifetime: %lu", this->domain_str, trace_ipv6(this->address), (unsigned long) this->lifetime);

    return 0;
}

int8_t net_dns_query_result_get(int8_t interface_id, uint8_t address[16], const char *domain_name_ptr)
{
    dns_query_t *this;

    if (!domain_name_ptr) {
        return -1;
    }
    this = dns_query_result_find(interface_id, domain_name_ptr);

    if (!this) {
        return -1;
    }
    if (address) {
        memcpy(address, this->address, 16);
    }

    return 0;
}

/**
 * Generic timeout handler for all interfaces and entries.
 */
void net_dns_timer_seconds(uint32_t seconds)
{
    ns_list_foreach_safe(dns_query_t, cur_ptr, &dns_query_list) {
        if (cur_ptr->lifetime == 0xffffffff) {
            continue;
        }
        if (cur_ptr->lifetime <= seconds) {
            dns_query_result_delete(cur_ptr);
            continue;
        }
        cur_ptr->lifetime -= seconds;
    }

    ns_list_foreach_safe(dns_server_info_t, cur_ptr, &dns_server_list) {
        if (cur_ptr->lifetime == 0xffffffff) {
            continue;
        }
        if (cur_ptr->lifetime <= seconds) {
            dns_server_info_delete(cur_ptr);
            continue;
        }
        cur_ptr->lifetime -= seconds;
    }
    return;
}
