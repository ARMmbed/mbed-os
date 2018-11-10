/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
 * \file thread_network_data_storage.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_NETWORK_DATA_PROXY_H_
#define THREAD_NETWORK_DATA_PROXY_H_

#include "ns_list.h"
#include "thread_constants.h"

struct link_configuration;
struct thread_commissioner;

typedef struct thread_network_data_context_entry_s {
    unsigned            cid: 4;                 /*!< 4-bit Context ID */
    bool                compression: 1;         /*!< C (Compression) flag */
    bool                stableData: 1;
    bool                canDelete: 1;
    uint8_t             contextPrefixLength;    /*!< Context prefix length in bits */
    uint32_t            context_reuse_delay;   /*!< Time in seconds */
    ns_list_link_t      link;                   /*!< List link entry */
} thread_network_data_context_entry_t;

typedef struct thread_network_local_data_context_entry_s {
    unsigned            cid: 4;                 /*!< 4-bit Context ID */
    bool                compression: 1;         /*!< C (Compression) flag */
    uint8_t             contextPrefixLength;    /*!< Context prefix length in bits */
    bool                stableData: 1;
} thread_network_local_data_context_entry_t;

typedef NS_LIST_HEAD(thread_network_data_context_entry_t, link) thread_data_context_list_t;

typedef struct thread_network_data_temporary_route_or_dhcpv6_server_entry_s {
    uint16_t            routerID;           /*!< Services UL16 */
    signed              Prf: 2;
    bool                P_preferred: 1;
    bool                P_slaac: 1;
    bool                P_dhcp: 1;
    bool                P_configure: 1;
    bool                P_default_route: 1;
    bool                stableData: 1;
    bool                P_on_mesh: 1;
    bool                P_nd_dns: 1;
    bool                P_res1: 1;
    bool                canDelete: 1;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_server_data_entry_t;

typedef struct thread_border_router_tlv_entry_t {
    uint16_t            routerID;           /*!< Services UL16 */
    signed              Prf: 2;             /* P_preference */
    bool                P_preferred: 1;     /* P_preferred */
    bool                P_slaac: 1;         /* P_slaac */
    bool                P_dhcp: 1;          /* P_dhcp */
    bool                P_configure: 1;     /* P_configure */
    bool                P_default_route: 1; /* P_default */
    bool                stableData: 1;      /* P_stable */
    bool                P_on_mesh: 1;       /* P_on_mesh */
    bool                P_nd_dns: 1;        /* P_nd_dns */
    bool                P_res1: 1;        /* P_res1 */
} thread_border_router_tlv_entry_t;

typedef struct thread_prefix_tlv {
    uint8_t     domainId;
    uint8_t     *Prefix;
    uint8_t     PrefixLen;
} thread_prefix_tlv_t;

typedef NS_LIST_HEAD(thread_network_server_data_entry_t, link) thread_network_server_data_list_t;

typedef struct thread_network_data_service_server_entry_s {
    uint16_t            router_id;           /*!< Services UL16 */
    uint8_t            *server_data;
    uint8_t             server_data_length;
    bool                stable : 1;
    bool                can_delete : 1;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_data_service_server_entry_t;

typedef NS_LIST_HEAD(thread_network_data_service_server_entry_t, link) thread_network_data_service_server_list_t;

typedef struct thread_network_data_service_cache_entry_s {
    bool                T : 1;
    unsigned            S_id : 4;
    bool                S_stable : 1;
    uint32_t            S_enterprise_number;
    uint8_t            *S_service_data;
    uint8_t             S_service_data_length;
    thread_network_data_service_server_list_t server_list;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_data_service_cache_entry_t;

/** This List Not inlude ::/0 Routes those are listed separately */
typedef struct thread_network_data_prefix_cache_entry_s {
    uint8_t           domainId;
    uint8_t           servicesPrefix[16];  /*!< Services Prefix */
    uint8_t           servicesPrefixLen;    /*!< Prefix length in bits This Can Be 1-128 */
    thread_data_context_list_t contextList;
    thread_network_server_data_list_t routeList;
    thread_network_server_data_list_t borderRouterList;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_data_prefix_cache_entry_t;

typedef struct thread_network_local_data_entry_s {
    uint8_t           domainId;
    uint8_t           servicesPrefix[16];  /*!< Services Prefix */
    uint8_t           servicesPrefixLen;    /*!< Prefix length in bits This Can Be 1-128 */
    bool                routeActive: 1;
    bool                routeDataStable: 1;
    bool                brActive: 1;
    bool                dhcpv6ServerActive: 1;
    bool                brDataStable: 1;
    bool                slaacServerActive: 1;
    bool                slaacPreferred: 1;
    unsigned            preference: 2;
    bool                configure: 1;
    bool                defaultRoute: 1;
    bool                onMesh: 1;
    bool                ndDns: 1;
    bool                res1: 1;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_local_data_entry_t;

/* The contents of the Service TLV & Server Set tuple */
typedef struct thread_network_data_service_entry_s {
    bool                T : 1; // '1' if S_enterprise number is THREAD_ENTERPRISE_NUMBER; otherwise, '0'.
    unsigned            S_id : 4;
    uint32_t            S_enterprise_number;
    uint8_t            *S_service_data;
    uint8_t             S_service_data_length;
    uint8_t            *S_server_data;
    uint8_t             S_server_data_length;
    bool                S_stable;
    ns_list_link_t      link;               /*!< List link entry */
} thread_network_data_service_entry_t;

typedef NS_LIST_HEAD(thread_network_data_service_cache_entry_t, link) thread_network_data_service_cache_list_t;
typedef NS_LIST_HEAD(thread_network_data_prefix_cache_entry_t, link) thread_network_prefix_list_t;
typedef NS_LIST_HEAD(thread_network_local_data_entry_t, link) thread_network_data_prefix_list_t;
typedef NS_LIST_HEAD(thread_network_data_service_entry_t, link) thread_network_data_service_list_t;

/** Network Data Main struct for seprate local data and Global */
typedef struct thread_network_data_cache_entry_s {
    thread_network_prefix_list_t localPrefixList; /*!< Local parsed or generated service list */
    thread_network_data_service_cache_list_t service_list;
    uint8_t networkDataTlvSize; /*!< Network data TLV Size */
    uint8_t network_data[THREAD_MAX_NETWORK_DATA_SIZE];
    uint16_t network_data_len;
    uint32_t contex_id_reuse_timeout;
    uint8_t network_data_update_delay;
    bool stableUpdatePushed: 1;
    bool temporaryUpdatePushed: 1;
} thread_network_data_cache_entry_t;

typedef struct thread_network_local_data_cache_entry_s {
    thread_network_data_prefix_list_t prefix_list; /*!< Local parsed or generated service list */
    thread_network_data_service_list_t service_list;
    uint16_t registered_rloc16;/*!< Address used for latest registration */
    uint16_t publish_coap_req_id;/*!< Non-zero when publish is active */
    bool release_old_address: 1; /*!< true if network data  can be released from old address */
    bool publish_pending: 1; /*!< true when publish attempt made during active publish */
} thread_network_local_data_cache_entry_t;

/**
 * Initialise Thread Network Data cache
 *
 * \param cachePtr  Pointer to Network Data Structure which will be initialized
 *
 */
void thread_network_data_base_init(thread_network_data_cache_entry_t *cachePtr);

void thread_network_local_server_data_base_init(thread_network_local_data_cache_entry_t *cachePtr);

void thread_network_data_free_and_clean(thread_network_data_cache_entry_t *cachePtr);

void thread_network_data_router_id_mark_delete(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID, bool subSet);

bool thread_network_data_router_id_free(thread_network_data_cache_entry_t *cachePtr, bool subSet, struct protocol_interface_info_entry *curInterface);

void thread_network_local_data_free_and_clean(thread_network_local_data_cache_entry_t *cachePtr, int8_t interface_id);

void thread_network_data_context_re_use_timer_update(int8_t id, thread_network_data_cache_entry_t *cachePtr, uint32_t ticks, lowpan_context_list_t *context_list);

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
int thread_nd_local_list_add_route(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service);

int thread_nd_local_list_add_service(thread_network_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service, thread_network_data_service_server_entry_t *server);

/**
 * Add new DHCPv6 Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param service On Mesh prefix TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_nd_local_list_add_on_mesh_prefix(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service);


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
int thread_nd_local_list_del_on_mesh_server(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service);

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
int thread_local_server_list_add_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *service);

/**
 * Del Local DHCPv6 Server information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_local_server_list_del_on_mesh_server(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv);

int thread_local_service_list_add(thread_network_local_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service);

int thread_local_service_list_del(thread_network_local_data_cache_entry_t *networkDataList, thread_network_data_service_entry_t *service);

/**
 * Add new local route information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 * \param route HAS Route TLV
 *
 * return 0, ADD OK
 * return <0 Add Not OK
 */
int thread_local_server_add_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_border_router_tlv_entry_t *route);

/**
 * Del local route information to route List
 *
 * \param networkDataList Pointer main network data structure
 * \param prefixTlv Prefix TLV (domainID, Prefix, PrefixLen)
 *
 * return 0, Del OK
 * return <0 Del Not OK
 */
int thread_local_server_del_route(thread_network_local_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv);

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
int thread_nd_local_list_add_contexts(thread_network_data_cache_entry_t *networkDataList, thread_prefix_tlv_t *prefixTlv, thread_network_local_data_context_entry_t *context);

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
uint8_t thread_nd_context_id_allocate(thread_network_data_cache_entry_t *networkDataList, thread_network_local_data_cache_entry_t *localDataList, uint8_t *prefixPtr, uint8_t prefixLength);

int thread_nd_verify_contex_id_is_free(thread_network_data_cache_entry_t *list, uint8_t *prefixPtr, thread_network_local_data_context_entry_t *context);

thread_network_data_service_cache_entry_t *thread_network_data_service_entry_find(thread_network_data_service_cache_list_t *list, thread_network_data_service_entry_t *service);

/**
 * Write Network Data full list or stable subset to given pointer
 *
 * \param networkDataList Network Data structure pointer
 * \param ptr pointer for write Network Data
 * \param fullList Boolean to select Full data or stable subset
 *
 * return Updated Pointer value end of Network Data
 */
uint16_t thread_network_data_service_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList);
uint16_t thread_network_data_prefix_set_size(thread_network_data_cache_entry_t *networkDataList, bool fullList);
uint8_t *thread_network_data_prefix_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr);
uint8_t *thread_network_data_service_set_write(thread_network_data_cache_entry_t *networkDataList, uint8_t *ptr);
bool thread_network_data_service_hosted_by_this_router_id(thread_network_data_service_cache_entry_t *dataList, uint16_t router_id);
uint16_t thread_network_data_service_child_id_from_networkdata_get(thread_network_data_cache_entry_t *networkDataList, uint16_t router_short_addr);
thread_network_data_prefix_cache_entry_t *thread_prefix_entry_find(thread_network_prefix_list_t *list, thread_prefix_tlv_t *prefixTlv);
uint8_t *thread_nd_own_service_list_data_write(thread_network_local_data_cache_entry_t *serverDataList, uint8_t *ptr, uint16_t routerID);

uint16_t thread_nd_own_service_list_data_size(thread_network_local_data_cache_entry_t *serverDataList);

void thread_nd_network_data_print(thread_network_data_cache_entry_t *networkData, uint16_t routerId);
bool thread_nd_dhcp_anycast_address_mapping_from_network_data(thread_network_data_cache_entry_t *networkDataList, uint16_t *rlocAddress, uint8_t contexId);
bool thread_nd_service_anycast_address_mapping_from_network_data(thread_network_data_cache_entry_t *networkDataList, uint16_t *rlocAddress, uint8_t S_id);

bool thread_nd_on_mesh_address_valid(thread_network_server_data_entry_t *curRoute);
thread_network_server_data_entry_t *thread_nd_hosted_by_this_routerid(uint16_t routerId, thread_network_server_data_list_t *list);
bool thread_network_data_services_registered(thread_network_data_cache_entry_t *cachePtr, uint16_t routerID);
int thread_network_data_resulting_tlv_size(thread_network_data_cache_entry_t *networkDataStorage, uint8_t *network_data_ptr, uint16_t network_data_length, uint16_t router_id);

#endif /* THREAD_NETWORK_DATA_PROXY_H_ */
