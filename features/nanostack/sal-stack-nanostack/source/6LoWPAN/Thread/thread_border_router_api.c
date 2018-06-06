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
#include "nsconfig.h"

#include <string.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include <ns_list.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "randLIB.h"
#include "thread_border_router_api.h"
#include "thread_management_if.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_mdns.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "MLE/mle.h"
#include "thread_meshcop_lib.h"
#include "thread_network_data_lib.h"
#include "coap_service_api.h"

#define TRACE_GROUP "tBRa"

#ifdef HAVE_THREAD_ROUTER
/*
 * Structure containing IPv6 Router advertisement options for DNS configuration.
 */
typedef struct {
    uint16_t option_length;
    uint8_t *option_data;
} dns_option_t;

/*
 * Border router instance data.
 */
typedef struct {
    dns_option_t *dns_search_list_option; /* option_data encoded according to RFC6106, DNSSL */
    dns_option_t *recursive_dns_server_option;  /* option_data encoded according to RFC6106, RDNSS */
    uint16_t nwk_data_resubmit_timer; /* network data resubmit timeout */
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} thread_border_router_t;

/* Neighbor discovery options according to RFC6106 (rfc4861) */
#define RFC6106_RECURSIVE_DNS_SERVER_OPTION     25
#define RFC6106_DNS_SEARCH_LIST_OPTION          31

static NS_LIST_DEFINE(border_router_instance_list, thread_border_router_t, link);


static thread_border_router_t *thread_border_router_find_by_interface(int8_t interface_id)
{
    thread_border_router_t *this = NULL;
    ns_list_foreach(thread_border_router_t, cur_br, &border_router_instance_list) {
        if (cur_br->interface_id == interface_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static thread_border_router_t *thread_border_router_find_by_service(int8_t service_id)
{
    thread_border_router_t *this = NULL;
    ns_list_foreach(thread_border_router_t, cur_br, &border_router_instance_list) {
        if (cur_br->coap_service_id == service_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

/*
 * Read Border Router TLV: DNS search list option.
 * DNSSL option data will be fetched from:
 * 1. User has provided static configuration by using method thread_border_router_dns_search_list_option_set.
 * 2. ICMP RA messages (if service is enabled and running in background).
 */
static uint8_t *thread_management_server_border_router_nd_dnssl_option_read(thread_border_router_t *this, uint16_t *resp_len)
{
    if (this->dns_search_list_option) {
        *resp_len = this->dns_search_list_option->option_length;
        return (uint8_t*)&this->dns_search_list_option->option_data;
    } else {
        // TODO: Read DNSSL from stored ICMP RA messages.
        *resp_len = 0;
        return NULL;
    }
}

/*
 * Read Border Router TLV: Recursive DNS Server option.
 * RDNSS option data will be fetched from:
 * 1. User has provided static configuration by using method thread_border_router_recursive_dns_server_option_set.
 * 2. ICMP RA messages (if service is enabled and running in background).
 */
static uint8_t *thread_management_server_border_router_nd_rdnss_option_read(thread_border_router_t *this, uint16_t *resp_len)
{
    if (this->recursive_dns_server_option) {
        *resp_len = this->recursive_dns_server_option->option_length;
        return (uint8_t*)&this->recursive_dns_server_option->option_data;
    } else {
        // TODO: Read RDNSS from stored ICMP RA messages.
        *resp_len = 0;
        return NULL;
    }
}

/**
 * Thread Neighbor discovery data request handler
 */
static int thread_border_router_neighbor_discovery_data_req_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_border_router_t *this = thread_border_router_find_by_service(service_id);
    uint8_t *request_tlv_ptr;
    uint8_t *resp_payload_ptr;
    uint16_t options_len, rdnss_option_len, dnssl_option_len, payload_len;
    uint8_t options_data[4];
    uint8_t *dns_server_tlv_ptr = NULL;
    uint8_t *dns_search_list_tlv_ptr = NULL;
    uint8_t *ptr;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    (void) source_port;
    (void) source_address;

    tr_debug("neighbor data request");

    if (!this) {
        return -1;
    }

    request_tlv_ptr = options_data;
    rdnss_option_len = dnssl_option_len = 0;

    options_len = thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_ND_OPTION, &request_tlv_ptr);

    while (options_len > 0) {
        switch (*request_tlv_ptr) {
            case RFC6106_RECURSIVE_DNS_SERVER_OPTION:
                dns_server_tlv_ptr = thread_management_server_border_router_nd_rdnss_option_read(this, &rdnss_option_len);
                break;
            case RFC6106_DNS_SEARCH_LIST_OPTION:
                dns_search_list_tlv_ptr = thread_management_server_border_router_nd_dnssl_option_read(this, &dnssl_option_len);
                break;
        }
        request_tlv_ptr++;
        options_len--;
    }

    payload_len = rdnss_option_len + dnssl_option_len;

    resp_payload_ptr = ptr = ns_dyn_mem_alloc(payload_len + 4); //reserve space also for type/length
    if (!resp_payload_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }

    *ptr++ = TMFCOP_TLV_ND_DATA;
    if (payload_len > 255) {
        *ptr++ = 0xff;
        ptr = common_write_16_bit(payload_len, ptr);
    } else {
        *ptr++ = payload_len;
    }

    if (dns_server_tlv_ptr) {
        memcpy(ptr, dns_server_tlv_ptr, rdnss_option_len);
        ptr += rdnss_option_len;
    }

    if (dns_search_list_tlv_ptr) {
        memcpy(ptr, dns_search_list_tlv_ptr, dnssl_option_len);
        ptr += dnssl_option_len;
    }

send_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_OCTET_STREAM, resp_payload_ptr, ptr-resp_payload_ptr);
    ns_dyn_mem_free(resp_payload_ptr);
    return 0;
}
/*
 * Check that local prefix is available in received network data
 */
static bool thread_border_router_network_data_prefix_match(thread_network_data_cache_entry_t *network_data_ptr, thread_network_local_data_entry_t *local_prefix, uint16_t router_id)
{
    ns_list_foreach(thread_network_data_prefix_cache_entry_t, nwk_prefix, &network_data_ptr->localPrefixList) {
        if (nwk_prefix->servicesPrefixLen != local_prefix->servicesPrefixLen) {
            continue;
        }

        if (!bitsequal(nwk_prefix->servicesPrefix, local_prefix->servicesPrefix, local_prefix->servicesPrefixLen)) {
            continue;
        }

        // check that prefix is hosted by this router
        if (!thread_nd_hosted_by_this_routerid(router_id, &nwk_prefix->routeList) &&
            !thread_nd_hosted_by_this_routerid(router_id, &nwk_prefix->borderRouterList)) {
            return false;
        }

        return true;
    }

    return false;
}

/*
 * Check that local service is available in network data
 */
static bool thread_border_router_network_data_service_match(thread_network_data_cache_entry_t *network_data_ptr, thread_network_data_service_entry_t *local_service, uint16_t router_id)
{
    thread_network_data_service_cache_entry_t *service_cache_entry = thread_network_data_service_entry_find(&network_data_ptr->service_list, local_service);

    if (service_cache_entry) {
        return thread_network_data_service_hosted_by_this_router_id(service_cache_entry, router_id);
    }

    return false;
}

static bool thread_border_router_local_network_data_prefix_match(thread_network_local_data_cache_entry_t *local_data, thread_network_data_prefix_cache_entry_t *prefix, uint16_t router_id)
{
    bool instance_found = false;

    if (thread_nd_hosted_by_this_routerid(router_id, &prefix->routeList)) {
        instance_found = true;
    }

    if (thread_nd_hosted_by_this_routerid(router_id, &prefix->borderRouterList)) {
        instance_found = true;
    }

    if (!instance_found) {
        /* Router ID not in propagated network data; skip */
        return true;
    }

    instance_found = false;

    ns_list_foreach(thread_network_local_data_entry_t, localPrefix, &local_data->prefix_list) {
        if (prefix->servicesPrefixLen != localPrefix->servicesPrefixLen) {
            continue;
        }

        if (bitsequal(prefix->servicesPrefix, localPrefix->servicesPrefix, localPrefix->servicesPrefixLen)) {
            /* Prefix exists in local data; return true */
            instance_found = true;
            break;
        }
    }

    if (!instance_found) {
        tr_debug("Found missing prefix: %s", trace_array(prefix->servicesPrefix, 8));
        return false;
    }

    return true;
}

static void thread_border_router_child_network_data_clean(uint8_t interface_id, uint16_t child_id)
{
    uint8_t addr16_buf[2];

    common_write_16_bit(child_id, addr16_buf);
    if (mle_class_get_by_link_address(interface_id, addr16_buf, ADDR_802_15_4_SHORT)) {
        /* Child is available in mle, do nothing */
        return;
    }

    // Child is not our child => network data contains data from lost children, remove it
    tr_debug("Remove nwk data from lost child: %04x", child_id);
    thread_management_client_network_data_unregister(interface_id, child_id);
}

static void thread_border_router_lost_children_nwk_data_validate(protocol_interface_info_entry_t *cur, uint16_t router_short_addr)
{
    if (!thread_is_router_addr(router_short_addr)) {
        // not validating children nwk data
        return;
    }

    thread_network_data_cache_entry_t *network_data = &cur->thread_info->networkDataStorage;

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, curLP, &network_data->localPrefixList) {
        /* Go throgh all routes */
        ns_list_foreach(thread_network_server_data_entry_t, curRoute, &curLP->routeList) {
            if (thread_addr_is_child(router_short_addr, curRoute->routerID)) {
                // Router children found
                thread_border_router_child_network_data_clean(cur->id, curRoute->routerID);
            }
        }

        /* Go through all BR's */
        ns_list_foreach(thread_network_server_data_entry_t, curBR, &curLP->borderRouterList) {
            if (thread_addr_is_child(router_short_addr, curBR->routerID)) {
                // Router children found
                thread_border_router_child_network_data_clean(cur->id, curBR->routerID);
            }
        }
    }

    /* Go throgh all services */
    ns_list_foreach(thread_network_data_service_cache_entry_t, service, &network_data->service_list) {
        ns_list_foreach(thread_network_data_service_server_entry_t, server, &service->server_list) {
            if (thread_addr_is_child(router_short_addr, server->router_id)) {
                // Router children found
                thread_border_router_child_network_data_clean(cur->id, server->router_id);
            }
        }
    }
}

static bool thread_border_router_local_network_data_service_match(thread_network_local_data_cache_entry_t *local_data, thread_network_data_service_cache_entry_t *service, uint16_t router_id)
{
    bool instance_found = false;

    ns_list_foreach(thread_network_data_service_server_entry_t, server, &service->server_list) {
        if (server->router_id == router_id) {
            instance_found = true;
            break;
        }
    }

    if (!instance_found) {
        /* Router ID not in propagated network data; skip */
        return true;
    }

    ns_list_foreach(thread_network_data_service_entry_t, local_service, &local_data->service_list) {
        if (local_service->S_service_data_length != service->S_service_data_length) {
            continue;
        }

        if (memcmp(local_service->S_service_data, service->S_service_data, local_service->S_service_data_length) != 0) {
            continue;
        }

        if (local_service->S_enterprise_number != service->S_enterprise_number) {
            continue;
        }

        /* Service exists in local data; return true */
        return true;
    }

    tr_debug("Found missing service: %s", trace_array(service->S_service_data, service->S_service_data_length));
    return false;
}

/*
 * Check that local server data is available in network data
 */
static bool thread_border_router_local_srv_data_in_network_data_check(protocol_interface_info_entry_t *cur)
{
    thread_network_data_cache_entry_t *network_data;
    thread_network_local_data_cache_entry_t *local_data;
    uint16_t router_id = cur->mac_parameters->mac_short_address;

    network_data = &cur->thread_info->networkDataStorage;
    local_data = &cur->thread_info->localServerDataBase;

    ns_list_foreach(thread_network_local_data_entry_t, localPrefix, &local_data->prefix_list) {
        // find matching prefix from network data
        if (thread_border_router_network_data_prefix_match(network_data, localPrefix, router_id) == false) {
            return false;
        }
    }

    ns_list_foreach(thread_network_data_service_entry_t, localService, &local_data->service_list) {
        // find matching service from network data
        if (thread_border_router_network_data_service_match(network_data, localService, router_id) == false) {
            return false;
        }
    }

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, prefix, &network_data->localPrefixList) {
        if (thread_border_router_local_network_data_prefix_match(local_data, prefix, router_id) == false) {
            return false;
        }
    }

    ns_list_foreach(thread_network_data_service_cache_entry_t, service, &network_data->service_list) {
        if (thread_border_router_local_network_data_service_match(local_data, service, router_id) == false) {
            return false;
        }
    }

    thread_border_router_lost_children_nwk_data_validate(cur, router_id);

    return true;
}

#ifdef HAVE_THREAD_BORDER_ROUTER
static int thread_border_router_recursive_dns_server_option_store(int8_t interface_id, uint8_t *recursive_dns_server_option, uint16_t recursive_dns_server_option_len)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);
    if (!this) {
        return -1;
    }

    ns_dyn_mem_free(this->recursive_dns_server_option);

    if (recursive_dns_server_option) {
        this->recursive_dns_server_option = ns_dyn_mem_alloc(sizeof(dns_option_t) + recursive_dns_server_option_len);
        if (!this->recursive_dns_server_option) {
            return -2;
        }
        memcpy(&this->recursive_dns_server_option->option_data, recursive_dns_server_option, recursive_dns_server_option_len);
        this->recursive_dns_server_option->option_length = recursive_dns_server_option_len;
    }
    return 0;
}
#endif

#ifdef HAVE_THREAD_BORDER_ROUTER
static int thread_border_router_dns_search_list_option_store(int8_t interface_id, uint8_t *dns_search_list_option, uint16_t search_list_option_len)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);
    if (!this) {
        return -1;
    }

    ns_dyn_mem_free(this->dns_search_list_option);
    if (dns_search_list_option) {
        this->dns_search_list_option = ns_dyn_mem_alloc(sizeof(dns_option_t) + search_list_option_len);
        if (!this->dns_search_list_option) {
            return -2;
        }
        memcpy(&this->dns_search_list_option->option_data, dns_search_list_option, search_list_option_len);
        this->dns_search_list_option->option_length = search_list_option_len;
    }
    return 0;
}
#endif

int8_t thread_border_router_init(int8_t interface_id)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);
    if (this) {
        return 0;
    }

    tr_debug("thread_border_router_init if=%d", interface_id);

    this = ns_dyn_mem_alloc(sizeof(thread_border_router_t));
    if (!this) {
        return -2;
    }
    this->dns_search_list_option = NULL;
    this->recursive_dns_server_option = NULL;
    this->interface_id = interface_id;
    this->nwk_data_resubmit_timer = 0;
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    if (this->coap_service_id < 0) {
        tr_warn("Thread border router coap init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
    // Register to Mesh CoAP URIs
    coap_service_register_uri(this->coap_service_id, THREAD_URI_NEIGHBOR_DISCOVERY_DATA_REQ, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_router_neighbor_discovery_data_req_cb);

    ns_list_add_to_start(&border_router_instance_list, this);
    return 0;
}

void thread_border_router_delete(int8_t interface_id)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);
    if (!this) {
        return;
    }

    coap_service_delete(this->coap_service_id);

    ns_list_remove(&border_router_instance_list, this);
    ns_dyn_mem_free(this->dns_search_list_option);
    ns_dyn_mem_free(this->recursive_dns_server_option);
    ns_dyn_mem_free(this);
}

void thread_border_router_seconds_timer(int8_t interface_id, uint32_t seconds)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);
    if (!this) {
        return;
    }

    if (this->nwk_data_resubmit_timer) {
        if (this->nwk_data_resubmit_timer > seconds) {
            this->nwk_data_resubmit_timer -= seconds;
        } else {
             protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
             this->nwk_data_resubmit_timer = 0;
             if (cur) {
                 if (!thread_border_router_local_srv_data_in_network_data_check(cur)) {
                     tr_info("nwk data mismatch - resubmit");
                     thread_border_router_publish(cur->id);
                 }
             }
        }
    }
}

void thread_border_router_resubmit_timer_set(int8_t interface_id, int16_t seconds)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(interface_id);

    if (!this) {
        return;
    }

    if (seconds >= 0) {
        this->nwk_data_resubmit_timer = seconds;
    } else {
        // re-init network data resubmit timer to default value
        this->nwk_data_resubmit_timer = THREAD_DATA_RESUBMIT_DELAY + randLIB_get_random_in_range(0, THREAD_DATA_RESUBMIT_DELAY/10);
    }
}

void thread_border_router_network_data_appl_callback(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->network_data_tlv_cb) {
        uint16_t payload_len = thread_network_data_tlv_size(cur, true);
        uint8_t *payload_ptr = ns_dyn_mem_alloc(payload_len + 3); /* 3 => room is also needed for TLV ID and length */
        if (payload_ptr) {
            thread_network_data_tlv_write(cur, payload_ptr, true);
            // Send Network data TLV to application without TLV ID and length
            cur->thread_info->network_data_tlv_cb(cur->thread_info->interface_id, payload_ptr + 2, payload_len);
            ns_dyn_mem_free(payload_ptr);
        }
    }
}

void thread_border_router_network_data_update_notify(protocol_interface_info_entry_t *cur)
{
    thread_border_router_t *this = thread_border_router_find_by_interface(cur->thread_info->interface_id);

    if (!this) {
        return;
    }
    if (!thread_border_router_local_srv_data_in_network_data_check(cur)) {
        if (this->nwk_data_resubmit_timer == 0) {
            this->nwk_data_resubmit_timer = 5;
            tr_debug("Resubmitted timer trig");
        }

    } else {
        tr_debug("All data registered");
        this->nwk_data_resubmit_timer = 0;
    }

    thread_border_router_network_data_appl_callback(cur);
}
#endif // HAVE_THREAD_ROUTER

/*External APIs*/

int thread_border_router_prefix_add(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, thread_border_router_info_t *prefix_info_ptr)
{
#ifdef HAVE_THREAD_ROUTER
    protocol_interface_info_entry_t *cur;
    thread_prefix_tlv_t prefixTlv;
    thread_border_router_tlv_entry_t service;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!prefix_info_ptr || !prefix_ptr) {
        return -2;
    }
    if(prefix_info_ptr->P_dhcp == true && prefix_info_ptr->P_slaac == true) {
        return -3;// Can not configure both services on
    }

    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = prefix_len;

    service.P_configure = prefix_info_ptr->P_configure;
    service.P_default_route = prefix_info_ptr->P_default_route;
    service.P_dhcp = prefix_info_ptr->P_dhcp;
    service.P_preferred = prefix_info_ptr->P_preferred;
    service.P_slaac = prefix_info_ptr->P_slaac;
    service.Prf = prefix_info_ptr->Prf;
    service.stableData = prefix_info_ptr->stableData;
    service.P_on_mesh = prefix_info_ptr->P_on_mesh;
    service.P_nd_dns = prefix_info_ptr->P_nd_dns;

    return thread_local_server_list_add_on_mesh_server(&cur->thread_info->localServerDataBase, &prefixTlv, &service);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) prefix_len;
    (void) prefix_info_ptr;
    return -1;
#endif
}

int thread_border_router_prefix_delete(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len)
{
#ifdef HAVE_THREAD_ROUTER
    protocol_interface_info_entry_t *cur;
    thread_prefix_tlv_t prefixTlv;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info || thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!prefix_ptr) {
        return -2;
    }
    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = prefix_len;

    return thread_local_server_list_del_on_mesh_server(&cur->thread_info->localServerDataBase, &prefixTlv);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) prefix_len;
    return -1;
#endif
}

int thread_border_router_route_add(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, bool stable, int8_t prf)
{
#ifdef HAVE_THREAD_ROUTER
    thread_prefix_tlv_t prefixTlv;
    thread_border_router_tlv_entry_t route;
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info || thread_attach_ready(cur) != 0) {
        return -2;
    }
    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = prefix_len;

    memset(&route, 0, sizeof(thread_border_router_tlv_entry_t));
    route.Prf = prf;
    route.stableData = stable;

    return thread_local_server_add_route(&cur->thread_info->localServerDataBase, &prefixTlv, &route);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) prefix_len;
    (void) stable;
    (void) prf;
    return -1;
#endif
}

int thread_border_router_route_delete(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len)
{
#ifdef HAVE_THREAD_ROUTER
    thread_prefix_tlv_t prefixTlv;
    protocol_interface_info_entry_t *cur;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info || thread_attach_ready(cur) != 0) {
        return -1;
    }

    prefixTlv.domainId = 0;
    prefixTlv.Prefix = prefix_ptr;
    prefixTlv.PrefixLen = prefix_len;

    return thread_local_server_del_route(&cur->thread_info->localServerDataBase, &prefixTlv);
#else
    (void) interface_id;
    (void) prefix_ptr;
    (void) prefix_len;
    return -1;
#endif

}

int thread_border_router_service_add(int8_t interface_id, uint8_t *service_data, uint8_t service_len, uint8_t sid, uint32_t enterprise_number, uint8_t *server_data, uint8_t server_data_len, bool stable)
{
#ifdef HAVE_THREAD_ROUTER
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info || thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!service_data || !service_len) {
        return -2;
    }

    thread_network_data_service_entry_t service = {
        .T = false,
        .S_id = sid,
        .S_enterprise_number = enterprise_number,
        .S_service_data = service_data,
        .S_service_data_length = service_len,
        .S_server_data = server_data,
        .S_server_data_length = server_data_len,
        .S_stable = stable,
    };

    if (enterprise_number == THREAD_ENTERPRISE_NUMBER) {
        service.T = true;
    }

    return thread_local_service_list_add(&cur->thread_info->localServerDataBase, &service);
#else
    (void)interface_id;
    (void)service_data;
    (void)service_len;
    (void)sid;
    (void)enterprise_number;
    (void)server_data;
    (void)server_data_len;
    (void)stable;
    return -1;
#endif
}

int thread_border_router_service_delete(int8_t interface_id, uint8_t *service_data, uint8_t service_len, uint32_t enterprise_number)
{
#ifdef HAVE_THREAD_ROUTER
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info || thread_attach_ready(cur) != 0) {
        return -1;
    }

    if (!service_data || !service_len) {
        return -2;
    }

    thread_network_data_service_entry_t service = {
        .S_enterprise_number = enterprise_number,
        .S_service_data = service_data,
        .S_service_data_length = service_len,
    };

    return thread_local_service_list_del(&cur->thread_info->localServerDataBase, &service);
#else
    (void) interface_id;
    (void) service_data;
    (void) service_len;
    (void) enterprise_number;
    return -1;
#endif
}

int thread_border_router_recursive_dns_server_option_set(int8_t interface_id, uint8_t *recursive_dns_server_option, uint16_t recursive_dns_server_option_len)
{
#ifdef HAVE_THREAD_BORDER_ROUTER
    return thread_border_router_recursive_dns_server_option_store(interface_id, recursive_dns_server_option, recursive_dns_server_option_len);
#else
    (void)interface_id;
    (void)recursive_dns_server_option;
    (void)recursive_dns_server_option_len;
    return -1;
#endif
}

int thread_border_router_dns_search_list_option_set(int8_t interface_id, uint8_t *dns_search_list_option, uint16_t search_list_option_len)
{
#ifdef HAVE_THREAD_BORDER_ROUTER
    return thread_border_router_dns_search_list_option_store(interface_id, dns_search_list_option, search_list_option_len);
#else
    (void)interface_id;
    (void)dns_search_list_option;
    (void)search_list_option_len;
    return -1;
#endif
}

/** Network data set response callback.
 *
 * callback to inform if network data was set to leader.
 *
 * /param status status of operation 0 success, -1 failure from leader received
 * /param data_ptr pointer to network data TLV that leader accepted.
 * /param data_len length of network data.
 *
 */
#ifdef HAVE_THREAD_ROUTER
static void thread_tmf_client_network_data_set_cb(int8_t interface_id, int8_t status, uint8_t *data_ptr, uint16_t data_len)
{
    protocol_interface_info_entry_t *cur;
    (void) status;
    (void) data_len;
    (void) data_ptr;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    cur->thread_info->localServerDataBase.publish_active = false;

    tr_debug("BR a/sd response status: %s, addr: %x",status?"Fail":"OK", cur->thread_info->localServerDataBase.registered_rloc16);

    if (cur->thread_info->localServerDataBase.publish_pending) {
        cur->thread_info->localServerDataBase.publish_pending = false;
        thread_border_router_publish(cur->id);
    }

    // always update RLOC to new one. If COAP response fails then resubmit timer will trigger new a/sd
    cur->thread_info->localServerDataBase.registered_rloc16 = mac_helper_mac16_address_get(cur);
    cur->thread_info->localServerDataBase.release_old_address = false;
}
#endif

int thread_border_router_publish(int8_t interface_id)
{
#ifdef HAVE_THREAD_ROUTER
    uint16_t network_data_len;
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t rloc16;
    int ret_val;
    protocol_interface_info_entry_t *cur;

    tr_debug("Border router Publish Local Services");
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info || thread_attach_ready(cur) != 0) {
        return -2;
    }

    rloc16 = mac_helper_mac16_address_get(cur);
    tr_debug("Border router old: %x, new: %x", cur->thread_info->localServerDataBase.registered_rloc16, rloc16);

    if (cur->thread_info->localServerDataBase.publish_active) {
        if (rloc16 != cur->thread_info->localServerDataBase.registered_rloc16) {
            /*
             * Device short address has changed, cancel previous a/sd and a/as requests
             * and start resubmit timer
             * */
            tr_debug("address changed, kill pending reuqests");
            thread_management_client_pending_coap_request_kill(cur->id);
            thread_border_router_resubmit_timer_set(interface_id, 5);
            return 0;
        } else {
            cur->thread_info->localServerDataBase.publish_pending = true;
            tr_debug("Activate pending status for publish");
            return 0;
        }
    }

    //Allocate Memory for Data
    network_data_len = thread_nd_own_service_list_data_size(&cur->thread_info->localServerDataBase);

    // Room for RLOC16  Room for Network data TLV
    ptr = payload_ptr = ns_dyn_mem_temporary_alloc(network_data_len + 4 + 5);
    if (!ptr) {
        return -3;
    }

    ptr = thread_tmfcop_tlv_data_write_header(ptr, TMFCOP_TLV_NETWORK_DATA, network_data_len);
    ptr = thread_nd_own_service_list_data_write(&cur->thread_info->localServerDataBase, ptr, rloc16);

    if (cur->thread_info->localServerDataBase.registered_rloc16 != 0xffff &&
            cur->thread_info->localServerDataBase.release_old_address &&
            cur->thread_info->localServerDataBase.registered_rloc16 != rloc16) {
        // Our address has changed so we must register our network with new address and remove the old address
        tr_debug("BR address changed - remove old %x", cur->thread_info->localServerDataBase.registered_rloc16);
        ptr = thread_tmfcop_tlv_data_write_uint16(ptr,TMFCOP_TLV_RLOC16,cur->thread_info->localServerDataBase.registered_rloc16);
    }

    cur->thread_info->localServerDataBase.registered_rloc16 = rloc16;
    ret_val = thread_management_client_network_data_register(cur->id, payload_ptr, ptr - payload_ptr, thread_tmf_client_network_data_set_cb);
    if (payload_ptr) {
        ns_dyn_mem_free(payload_ptr);
    }
    if (ret_val == 0) {
        cur->thread_info->localServerDataBase.publish_active = true;
    }

    thread_border_router_resubmit_timer_set(interface_id, -1);

    return ret_val;
#else
    (void) interface_id;
    return -1;
#endif
}

int thread_border_router_delete_all(int8_t interface_id)
{
#ifdef HAVE_THREAD_ROUTER
    protocol_interface_info_entry_t *cur;
    tr_debug("Border router Delete Local Service");
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info || thread_attach_ready(cur) != 0) {
        return -2;
    }

    //Delete first Local List
    thread_network_local_data_free_and_clean(&cur->thread_info->localServerDataBase, interface_id);

    return 0;
#else
    (void) interface_id;
    return -1;
#endif
}

int thread_border_router_network_data_callback_register(int8_t interface_id, thread_network_data_tlv_cb* nwk_data_cb)
{
#ifdef HAVE_THREAD
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->thread_info || thread_attach_ready(cur) != 0) {
        return -2;
    }

    cur->thread_info->network_data_tlv_cb = nwk_data_cb;

    thread_border_router_network_data_appl_callback(cur);

    return 0;
#else
    (void)interface_id;
    (void)nwk_data_cb;
    return -1;
#endif
}

int thread_border_router_prefix_tlv_find(uint8_t* network_data_tlv, uint16_t network_data_tlv_length, uint8_t** prefix_tlv, bool *stable)
{
#ifdef HAVE_THREAD
    uint16_t tlv_length;
    if (!network_data_tlv || !network_data_tlv_length || !prefix_tlv) {
        return -1;
    }
    //tr_debug("thread_tlv_lib_prefix_find() len=%d, tlv=%s", network_data_tlv_length, trace_array(network_data_tlv, network_data_tlv_length));
    *stable = true;
    tlv_length = thread_meshcop_tlv_find_next(network_data_tlv, network_data_tlv_length, THREAD_NWK_DATA_TYPE_PREFIX|THREAD_NWK_STABLE_DATA, prefix_tlv);
    if (tlv_length == 0) {
        tlv_length = thread_meshcop_tlv_find_next(network_data_tlv, network_data_tlv_length, THREAD_NWK_DATA_TYPE_PREFIX, prefix_tlv);
        *stable = false;
    }
    return tlv_length;
#else
    (void)network_data_tlv;
    (void)network_data_tlv_length;
    (void)prefix_tlv;
    (void)stable;
    return -1;
#endif
}

int thread_border_router_tlv_find(uint8_t* prefix_tlv, uint16_t prefix_tlv_length, uint8_t** border_router_tlv, bool *stable)
{
#ifdef HAVE_THREAD
    uint16_t tlv_length;
    if (!prefix_tlv || !prefix_tlv_length || !border_router_tlv) {
        return -1;
    }

    //tr_debug("thread_tlv_lib_border_router_find() len=%d, tlv=%s", prefix_tlv_length, trace_array(prefix_tlv, prefix_tlv_length));
    uint8_t prefix_length = prefix_tlv[1];
    uint8_t prefix_byte_len = prefixBits_to_bytes(prefix_length);
    prefix_tlv = prefix_tlv + 2 + prefix_byte_len; //2 = domain ID + prefix length
    prefix_tlv_length = prefix_tlv_length - prefix_byte_len - 2;

    // find stable prefix first and if not found return unstable data
    *stable = true;
    tlv_length = thread_meshcop_tlv_find_next(prefix_tlv, prefix_tlv_length, THREAD_NWK_DATA_TYPE_BORDER_ROUTER|THREAD_NWK_STABLE_DATA, border_router_tlv);
    if (tlv_length == 0) {
        tlv_length = thread_meshcop_tlv_find_next(prefix_tlv, prefix_tlv_length, THREAD_NWK_DATA_TYPE_BORDER_ROUTER, border_router_tlv);
        *stable = false;
    }
    return tlv_length;
#else
    (void)prefix_tlv;
    (void)prefix_tlv_length;
    (void)border_router_tlv;
    (void)stable;
    return -1;
#endif
}

int thread_border_router_prefix_context_id(uint8_t *prefix_tlv, uint16_t prefix_tlv_length)
{
#ifdef HAVE_THREAD
    if (!prefix_tlv || !prefix_tlv_length) {
        return -1;
    }

    uint16_t data_length = prefix_tlv_length;

    while (data_length) {
        uint8_t type = *prefix_tlv++;
        uint16_t len = *prefix_tlv++;
        data_length -= 2;

        type &= THREAD_NWK_DATA_TYPE_MASK;

        if (type == THREAD_NWK_DATA_TYPE_6LOWPAN_ID) {
            return (*prefix_tlv & 0x0f);
        }

        data_length -= len;
        prefix_tlv += len;
    }

    return -2;
#else
    (void)prefix_tlv;
    (void)prefix_tlv_length;
    return -1;
#endif
}

int thread_border_router_service_tlv_find(uint8_t* network_data_tlv, uint16_t network_data_tlv_length, uint8_t** service_tlv, bool* stable)
{
#ifdef HAVE_THREAD
    uint16_t tlv_length;
    if (!network_data_tlv || !network_data_tlv_length || !service_tlv) {
        return -1;
    }

    *stable = true;
    tlv_length = thread_meshcop_tlv_find_next(network_data_tlv, network_data_tlv_length, THREAD_NWK_DATA_TYPE_SERVICE_DATA|THREAD_NWK_STABLE_DATA, service_tlv);
    if (tlv_length == 0) {
        tlv_length = thread_meshcop_tlv_find_next(network_data_tlv, network_data_tlv_length, THREAD_NWK_DATA_TYPE_SERVICE_DATA, service_tlv);
        *stable = false;
    }
    return tlv_length;
#else
    (void)network_data_tlv;
    (void)network_data_tlv_length;
    (void)service_tlv;
    (void)stable;
    return -1;
#endif
}

int thread_border_router_server_tlv_find(uint8_t* service_tlv, uint16_t service_tlv_length, uint8_t** server_tlv, bool* stable)
{
#ifdef HAVE_THREAD
    uint16_t tlv_length;
    if (!service_tlv || !service_tlv_length || !server_tlv) {
        return -1;
    }

    uint8_t t_flag = service_tlv[0] >> 7;
    service_tlv += 1;

    if (!t_flag) {
        service_tlv_length -= 4;
        service_tlv += 4;
    }

    uint8_t service_data_len = *service_tlv;
    service_tlv += 1 + service_data_len;
    service_tlv_length = service_tlv_length - service_data_len - 2;

    *stable = true;
    tlv_length = thread_meshcop_tlv_find_next(service_tlv, service_tlv_length, THREAD_NWK_DATA_TYPE_SERVER_DATA|THREAD_NWK_STABLE_DATA, server_tlv);
    if (tlv_length == 0) {
        tlv_length = thread_meshcop_tlv_find_next(service_tlv, service_tlv_length, THREAD_NWK_DATA_TYPE_SERVER_DATA, server_tlv);
        *stable = false;
    }
    return tlv_length;
#else
    (void)service_tlv;
    (void)service_tlv_length;
    (void)server_tlv;
    (void)stable;
    return -1;
#endif
}

int thread_border_router_mdns_responder_start(int8_t interface_id, int8_t interface_id_mdns, const char *service_name)
{
#ifdef HAVE_THREAD_BORDER_ROUTER
    return thread_mdns_start(interface_id, interface_id_mdns, service_name);
#else
    (void)interface_id;
    (void)interface_id_mdns;
    (void)service_name;
    return -1;
#endif
}

int thread_border_router_mdns_responder_stop(void)
{
#ifdef HAVE_THREAD_BORDER_ROUTER
    return thread_mdns_stop();
#else
    return -1;
#endif
}

