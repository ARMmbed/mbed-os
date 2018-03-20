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

#ifndef _NS_MDNS_API_H_
#define _NS_MDNS_API_H_

/**
 * \file ns_mdns_api.h
 * \brief Nanostack mDNS-SD API.
 */

/*!
 * \struct ns_mdns_t
 * \brief Structure for Nanostack mDNS instance
 */
typedef struct ns_mdns *ns_mdns_t;  /**< Instance */

/*!
 * \struct ns_mdns_service_t
 * \brief Structure for Nanostack mDNS service instance
 */
typedef struct ns_mdns_service *ns_mdns_service_t;  /**< Service instance */

/*!
 * \struct ns_mdns_service_param_t
 * \brief Structure for mDNS service parameters
 */
typedef struct ns_mdns_service_param
{
    const char *service_type;                   /**< Null-terminated string owned by the caller */
    uint16_t    service_port;                   /**< Service Port number */
    const uint8_t *(*service_get_txt)(void);    /**< Call-back function, which returns a pointer to the service TXT record (null-terminated).
                                                   If the service does not provide any TXT record, this parameter must be set to NULL. */
} ns_mdns_service_param_t;

/**
 * \brief Start mDNS server
 *
 * \param server_name NULL terminated string, max length 63 characters
 *
 * \param ttl time-to-live value in seconds, if set to 0 default value is used
 *
 * \param ttl_ip time-to-live in hop count, if set to 0 default value is used
 *
 * \param interface_id ID of the network interface where mDNS will operate
 *
 * \return mDNS server instace or NULL in case of failure..
 */
ns_mdns_t ns_mdns_server_start(const char *server_name, uint32_t ttl, uint32_t ttl_ip, int8_t interface_id);

/**
 * \brief Stop mDNS server
 *
 * \param ns_mdns_instance Server instance received from ns_mdns_server_start
 *
 */
void ns_mdns_server_stop(ns_mdns_t ns_mdns_instance);

/**
 * \brief Register service to mDNS server
 *
 * \param ns_mdns_instance Server instance received from ns_mdns_server_start
 *
 * \param service Parameters for service
 *
 * \return mDNS Service descriptor or NULL in case of failure.
 *
 */
ns_mdns_service_t ns_mdns_service_register(ns_mdns_t ns_mdns_instance, ns_mdns_service_param_t *service);

/**
 * \brief Unregister service from mDNS server
 *
 * \param service_desc mDNS Service descriptor received from call to ns_mdns_service_register.
 */

void ns_mdns_service_unregister(ns_mdns_service_t service_desc);

/**
 * \brief Send mDNS announcement. Application should call this method once application
 * advertised parameters has changed.
 *
 * \param ns_mdns_instance Server instance received from ns_mdns_server_start
 */

void ns_mdns_announcement_send(ns_mdns_t ns_mdns_instance);

#endif /* _NS_MDNS_API_H_ */

