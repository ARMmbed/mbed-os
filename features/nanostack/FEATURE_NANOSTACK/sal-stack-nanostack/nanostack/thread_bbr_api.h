/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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
 * \file thread_bbr_api.h
 * \brief Thread backbone border router (BBR) application interface.
 *
 * This is Thread backbone Border router service.
 * When started the module takes care of starting the
 * components that enables default border router functionality in Thread mesh network.
 *
 */

#ifndef THREAD_BBR_API_H_
#define THREAD_BBR_API_H_

#include "ns_types.h"

/**
 * Start backbone border router service.
 *
 * Enables the mDNS responder with default service name. \<rand\>-ARM-\<Network name\>
 * if backbone interface is enabled and allows routing.
 *    Publishes Default route with DHCP addressing
 *    Enables ND proxy
 *
 * if Commercial mode primary BBR activated publishes the commercial primary backbone information to Leader.
 *    Enables the Backbone address resolution.
 *    Enables the DUA, MLR registrations.
 *
 * \param interface_id Thread network interface id.
 * \param backbone_interface_id backbone interface id.
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_start(int8_t interface_id, int8_t backbone_interface_id);

/**
 * Set the bbr timeout values. Default timeout values are used if not called.
 *
 * If this function is called it will trigger the modification to thread network causing'
 * refreshing of registrations.
 *
 * \param interface_id interface ID of the Thread network.
 * \param mlr_timeout Multicast registration timeout. 0 do not change.
 * \param dua_timeout Timeout when BBR updates the sequence number for network data. 0 do not change
 * \param dua_delay Jitter delay value for Domain Unicast Address (DUA) registrations. 0 do not change
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_timeout_set(int8_t interface_id, uint32_t mlr_timeout, uint32_t dua_timeout, uint32_t dua_delay);

/**
 * Set the Thread registration interface (TRI) destination address.
 * TRI address must be set to enable TRI relay.
 *
 * \param interface_id interface ID of the Thread network.
 * \param tri_addr_ptr IPv6 address of the TRI server.
 * \param tri_port UDP port of the TRI server.
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_tri_address_set(int8_t interface_id, const uint8_t *tri_addr_ptr, uint16_t tri_port);

/**
 * Stop backbone Border router.
 *
 * \param interface_id interface ID of the Thread network
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
void thread_bbr_stop(int8_t interface_id);


#endif /* THREAD_BBR_API_H_ */
