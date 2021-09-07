/*
 * Copyright (c) 2014-2018, Pelion and affiliates.
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

/**
 * \file thread_border_router_api.h
 * \brief Thread border router application interface.
 *
 * This file contains functions for managing Thread border router features.
 * Border routers can set up services in the Thread network including routes,
 * DHCPv6 servers, on-mesh prefixes and services.
 *
 */

#ifndef THREAD_BORDER_ROUTER_API_H_
#define THREAD_BORDER_ROUTER_API_H_

#include "ns_types.h"

/**
 * \brief Border router network data structure.
 */
typedef struct thread_border_router_info_t {
    unsigned            Prf: 2;               /**< Prefix preference, 01 = High, 00 = Default, 11 = Low, 10 = Reserved. */
    bool                P_preferred: 1;       /**< Address is considered preferred address. */
    bool                P_slaac: 1;           /**< Allowed to configure a new address */
    bool                P_dhcp: 1;            /**< DHCPv6 server is available in the network. */
    bool                P_configure: 1;       /**< DHCPv6 agent provides other configuration. */
    bool                P_default_route: 1;   /**< This device provides the default route. */
    bool                P_on_mesh: 1;         /**< This prefix is considered to be on-mesh */
    bool                P_nd_dns: 1;          /**< this border router is able to provide DNS information */
    bool                P_res1: 1;            /**< First reserved bit */
    bool                stableData: 1;        /**< This data is stable and expected to be available at least 48h. */
} thread_border_router_info_t;

/**
 * \brief Create local service that is provided to the Thread network.
 * If a prefix exists it is updated. For example, when removing SLAAC (Stateless Address Autoconfiguration) you should modify the prefix
 * first to remove the creation of new addresses and after a while, remove the prefix.
 *
 * When you have configured the services locally, you need to call
 * thread_border_router_publish to make the services available in the network.
 *
 * \param interface_id Network interface ID.
 * \param prefix_ptr Pointer prefix.
 * \param prefix_len Length of prefix.
 * \param prefix_info_ptr Prefix service structure configuring the published service.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
int thread_border_router_prefix_add(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, thread_border_router_info_t *prefix_info_ptr);

/**
 * \brief Delete local service.
 *
 * \param interface_id Network interface ID.
 * \param prefix_ptr Pointer prefix.
 * \param prefix_len Length of prefix.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
int thread_border_router_prefix_delete(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len);

/**
 * \brief Add a new route to the Thread network. Other devices can use the route.
 * For example, 0::0/0 means that this device provides the default route.
 * For example, 2001::0/64 means that this device provides a more specific route.
 *
 * If a prefix exists it is updated.
 *
 * \param interface_id Network interface ID.
 * \param prefix_ptr Pointer prefix. Can be NULL for the default route.
 * \param prefix_len Length of prefix.
 * \param stable This data is stable and expected to be available at least 48h.
 * \param prf Route preference, 01 = High, 00 = Default, 11 = Low, 10 = Reserved.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
int thread_border_router_route_add(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, bool stable, int8_t prf);

/**
 * \brief Delete locally served route.
 *
 * \param interface_id Network interface ID.
 * \param prefix_ptr Pointer prefix.
 * \param prefix_len Length of prefix.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
int thread_border_router_route_delete(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len);

/**
 * \brief Add or modify a local service.
 *
 * \param interface_id Network interface ID.
 * \param service_data Pointer to a byte string specifying the type of service.
 * \param service_len Length of service data.
 * \param sid Service Type ID, a value between 0 and 15, inclusive, assigned to this service data.
 * \param enterprise_number Enterprise number of the vendor that defined the type of the server.
 * \param server_data Pointer to a byte string containing server-specific information.
 * \param server_data_len Length of server data.
 * \param stable This data is stable and expected to be available at least 48h.
 *
 * \return 0, Addition OK.
 * \return <0 Addition not OK.
 */
int thread_border_router_service_add(int8_t interface_id, uint8_t *service_data, uint8_t service_len, uint8_t sid, uint32_t enterprise_number, uint8_t *server_data, uint8_t server_data_len, bool stable);

/**
 * \brief Delete local service by service data and enterprise number.
 *
 * \param interface_id Network interface ID.
 * \param service_data Pointer to a byte string specifying the type of service.
 * \param service_len Length of service.
 * \param enterprise_number Enterprise number of the vendor that defined the type of the server.
 *
 * \return 0, Delete OK.
 * \return <0 Delete not OK.
 */
int thread_border_router_service_delete(int8_t interface_id, uint8_t *service_data, uint8_t service_len, uint32_t enterprise_number);

/**
 * \brief Publish local services to Thread network.
 *
 * If local services are deleted before calling this, all services are deregistered from the network.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0, Push OK.
 * \return <0 Push not OK.
 */
int thread_border_router_publish(int8_t interface_id);

/**
 * \brief Clear the local service list.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0, Push OK.
 * \return <0 Push not OK.
 */
int thread_border_router_delete_all(int8_t interface_id);

/**
 * \brief Set Recursive DNS server (RDNSS) option that is encoded according to RFC6106.
 * Setting a new RDNSS will overwrite previous RDNSS option. Set RNDDS will be used
 * until it is cleared.
 *
 * \param interface_id Network interface ID.
 * \param recursive_dns_server_option Recursive DNS server option encoded according to rfc6106, can be NULL to clear existing RDNSS.
 * \param recursive_dns_server_option_len Length of the recursive_dns_server_option in bytes.
 *
 * \return 0, Option saved OK.
 * \return <0 when error occurs during option processing.
 */
int thread_border_router_recursive_dns_server_option_set(int8_t interface_id, uint8_t *recursive_dns_server_option, uint16_t recursive_dns_server_option_len);

/**
 * \brief Set DNS server search list (DNSSL) option that is encoded according to RFC6106.
 * Setting a new DNSSL will overwrite previous DNSSL option. Set DNSSL will be used
 * until it is cleared.
 *
 * \param interface_id Network interface ID.
 * \param dns_search_list_option DNS search list option encoded according to rfc6106, can be NULL to clear existing DNSSL.
 * \param search_list_option_len Length of the dns_search_list_option in bytes.
 *
 * \return 0, Option saved OK.
 * \return <0 when error occurs during option processing.
 */
int thread_border_router_dns_search_list_option_set(int8_t interface_id, uint8_t *dns_search_list_option, uint16_t search_list_option_len);

/**
 * \brief Callback type for Thread network data TLV registration
 *
 * \param interface_id Network interface ID.
 * \param network_data_tlv Thread Network data TLV as specified in Thread specification.
 * \param network_data_tlv_length length of the network data TLV.
 */
typedef void (thread_network_data_tlv_cb)(int8_t interface_id, uint8_t *network_data_tlv, uint16_t network_data_tlv_length);

/**
 * \brief Register callback function to receive thread network data TLV in byte array. For the first time the callback
 * will be called before returning from the function. Afterwards the callback will be called when there is a change
 * to the network data or when the network data is advertised. Application is not allowed to block the callback execution
 * and must make a copy of the network data if it is needed afterwards.
 *
 * Setting nwk_data_cb to NULL will prevent further calls to the callback function.
 *
 * \param interface_id Network interface ID.
 * \param nwk_data_cb callback function to receive network data TLV in byte array as specified in the Thread specification.
 *
 * \return 0, Callback function registered successfully.
 * \return <0 when error occurs during registering the callback function.
 */
int thread_border_router_network_data_callback_register(int8_t interface_id, thread_network_data_tlv_cb *nwk_data_cb);

/**
 * Find Prefix TLV from the Network Data TLV byte array.
 *
 * \param network_data_tlv [IN] Network data TLV in byte array.
 * \param network_data_tlv_length [IN] Length of the network data TLV byte array in bytes.
 * \param prefix_tlv [IN] pointer to the previous prefix_tlv, NULL if previous TLV not known.
 *                   [OUT] Pointer to the prefix TLV found.
 * \param stable [OUT] value set to true if found TLV is stable, false otherwise.
 *
 * \return Length of the found Prefix TLV
 * \return 0 if TLV is empty or no Prefix TLV found.
 * \return negative value indicates error in input parameters.
 */
int thread_border_router_prefix_tlv_find(uint8_t *network_data_tlv, uint16_t network_data_tlv_length, uint8_t **prefix_tlv, bool *stable);

/**
 * Find Border router TLV from the Network Data TLV (under Prefix TLV) byte array.
 *
 * \param prefix_tlv [IN] Network data TLV in byte array.
 * \param prefix_tlv_length [IN] Length of the Network data TLV byte array in bytes.
 * \param border_router_tlv [IN] pointer to the previous Border Router TLV, NULL if not known.
 *                          [OUT] Pointer to the Border Router TLV found.
 * \param stable [OUT] value set to true if found TLV is stable, false otherwise
 *
 * \return Length of the Prefix found
 * \return 0 if TLV is empty or no TLV found.
 * \return negative value indicates error in input parameters.
 */
int thread_border_router_tlv_find(uint8_t *prefix_tlv, uint16_t prefix_tlv_length, uint8_t **border_router_tlv, bool *stable);

/**
 * Find Service TLV from the Network Data TLV byte array.
 *
 * \param network_data_tlv [IN] Network data TLV in byte array.
 * \param network_data_tlv_length [IN] Length of the network data TLV byte array in bytes.
 * \param service_tlv [IN] pointer to the previous Service TLV, NULL if previous TLV not known.
 *                   [OUT] Pointer to the Service TLV found.
 * \param stable [OUT] value set to true if found TLV is stable, false otherwise.
 *
 * \return Length of the found Service TLV
 * \return 0 if TLV is empty or no Service TLV found.
 * \return negative value indicates error in input parameters.
 */
int thread_border_router_service_tlv_find(uint8_t *network_data_tlv, uint16_t network_data_tlv_length, uint8_t **service_tlv, bool *stable);

/**
 * Find Server TLV from the Network Data TLV (under Service TLV) byte array.
 *
 * \param service_tlv [IN] Network data TLV in byte array.
 * \param service_tlv_length [IN] Length of the Network data TLV byte array in bytes.
 * \param server_tlv [IN] pointer to the previous Server TLV, NULL if not known.
 *                          [OUT] Pointer to the Server TLV found.
 * \param stable [OUT] value set to true if found TLV is stable, false otherwise
 *
 * \return Length of the Prefix found
 * \return 0 if TLV is empty or no TLV found.
 * \return negative value indicates error in input parameters.
 */
int thread_border_router_server_tlv_find(uint8_t *service_tlv, uint16_t service_tlv_length, uint8_t **server_tlv, bool *stable);

/**
 * Determine context ID from the Network Data TLV (under Prefix TLV) byte array.
 *
 * \param prefix_tlv [IN] Prefix TLV in byte array.
 * \param prefix_tlv_length [IN] Length of the Prefix TLV byte array in bytes.
 *
 * \return The context ID value found
 * \return -1 if error in input parameters.
 * \return -2 if no context ID value found.
 */
int thread_border_router_prefix_context_id(uint8_t *prefix_tlv, uint16_t prefix_tlv_length);

/**
 * Start mDNS responder service. The responder will respond to DNS-SD queries and send announcement when
 * Thread network data is updated.
 *
 * The mDNS responder can be closed by calling thread_border_router_mdns_responder_stop(). Closing the Thread
 * network interface will stop the mDNS responder automatically.
 *
 * \param interface_id interface ID of the Thread network
 * \param interface_id_mdns interface where mDNS messaging occurs
 * \param service_name mDNS instance name
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_border_router_mdns_responder_start(int8_t interface_id, int8_t interface_id_mdns, const char *service_name);

/**
 * Stop mDNS responder service
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_border_router_mdns_responder_stop(void);
#endif /* THREAD_BORDER_ROUTER_API_H_ */
