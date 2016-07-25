/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
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
 * Structure specifying the prefix service.
 *
 */
typedef struct thread_border_router_info_t {
    unsigned            Prf: 2;               /**!< Prefix preference, 01 = High, 00 = Default, 11 = Low, 10 = Reserved. */
    bool                P_preferred: 1;       /**!< Address is considered preferred address. */
    bool                P_slaac: 1;           /**!< Allowed to configure a new address */
    bool                P_dhcp: 1;            /**!< DHCPv6 server is available in the network. */
    bool                P_configure: 1;       /**!< DHCPv6 agent provides other configuration. */
    bool                P_default_route: 1;   /**!< This device provides the default route. */
    bool                P_on_mesh: 1;         /**!< This prefix is considered to be on-mesh */
    bool                P_nd_dns: 1;          /**!< this border router is able to provide DNS information */
    bool                stableData: 1;        /**!< This data is stable and expected to be available at least 48h. */
} thread_border_router_info_t;

/* Define flags for backwards compatibility. Remove once applications have been updated */
#define P_slaac_preferred   P_preferred
#define P_slaac_valid       P_slaac

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
 * \brief Add local service.
 *
 * \param interface_id Network interface ID.
 * \param service_ptr Pointer to service data.
 * \param service_len Length of service.
 * \param thread_enteprise True if Thread enterprise number is used.
 * \param sid Service identifier.
 * \param enterprise_number If thread_enteprise is false this must be given.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
//TODO PUUTTUUU SERVER data
int thread_border_router_service_add(int8_t interface_id, uint8_t *service_ptr, uint8_t service_len, bool thread_enteprise, uint8_t sid, uint32_t enterprise_number);

/**
 * \brief Delete local service.
 *
 * \param interface_id Network interface ID.
 * \param service_ptr Pointer to service data.
 * \param service_len Length of service.
 *
 * \return 0, Set OK.
 * \return <0 Set not OK.
 */
int thread_border_router_service_delete(int8_t interface_id, uint8_t *service_ptr, uint8_t service_len);

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

#endif /* THREAD_DHCPV6_SERVER_H_ */
