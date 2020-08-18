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

/**********************************************************************/ /*!
*
* @file fnet_user_config.h
*
* @brief FNET User configuration file.
* It should be used to change any default configuration parameter.
*
***************************************************************************/

#ifndef _FNET_USER_CONFIG_H_

#define _FNET_USER_CONFIG_H_

/*****************************************************************************
* IPv4 and/or IPv6 protocol support.
******************************************************************************/
#define FNET_CFG_IP4                (0)
#define FNET_CFG_IP6                (1)

/*****************************************************************************
* Multicast DNS (mDNS) "Bonjour" server/responder support.
******************************************************************************/
#define FNET_CFG_MDNS                       (1)
#define FNET_CFG_MULTICAST                  (1)

/* Environment */
#define FNET_CFG_CPU_LITTLE_ENDIAN      (1) // Simulator is big endian, Add HW endianess also, follow target
#define FNET_CFG_MDNS_MAX               (1) // allow only 1 mDNS server, as socket needs to be bound to port 5353
#define FNET_CFG_MDNS_SERVICE_MAX       (2) // Allow 2 services per server

#define FNET_CFG_OVERLOAD_MEMCPY        (1) // Use standard memcpy
#define fnet_memcpy                     memcpy

/* Testing */
#define FNET_CFG_DEBUG                  (0) // enable debug traces
#define FNET_CFG_DEBUG_MDNS             (0) // enable mDNS debug traces

#endif /* _FNET_USER_CONFIG_H_ */
