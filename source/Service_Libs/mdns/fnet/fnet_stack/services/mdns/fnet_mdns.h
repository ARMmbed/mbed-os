/**************************************************************************
*
* Copyright (c) 2017, Pelion and affiliates.
* SPDX-License-Identifier: Apache-2.0
* Copyright 2016 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
/*!
* @brief mDNS (Bonjour) Server/Responder API.
*
***************************************************************************/

#ifndef _FNET_MDNS_H_

#define _FNET_MDNS_H_

#if FNET_CFG_MDNS || defined(__DOXYGEN__)

/*! @addtogroup fnet_mdns
*
* Multicast DNS (mDNS) is used to enable DNS-like
* name resolution in scenarios in which conventional Unicast DNS name resolution is not possible.
* It allows both IPv4 and IPv6 hosts to perform name resolution for hosts on the same local link.@n
* The protocol specification is defined by RFC6762.@n
* It is natively supported by Apple OSs, and named Bonjour.@n
* @n
* After the MDNS server is initialized by calling the @ref fnet_mdns_init() function,
* the user application should call the main service-polling function
* @ref fnet_poll_service() periodically in background. @n
* @n
* For the MDNS-server service example, refer to the FNET Shell demo source code.@n
*
* Configuration parameters:
* - @ref FNET_CFG_MDNS
* - @ref FNET_CFG_MDNS_MAX
* - @ref FNET_CFG_MDNS_PORT
* - @ref FNET_CFG_MDNS_RR_TTL
* - @ref FNET_CFG_MDNS_SERVICE_MAX
*
*/

/*! @{ */

/**************************************************************************/ /*!
 * @brief Initialization parameters for the @ref fnet_mdns_init() function.
 ******************************************************************************/
struct fnet_mdns_params
{
    fnet_netif_desc_t           netif_desc;     /**< @brief Network interface descriptor to be used by the MDNS server.*/
    fnet_address_family_t       addr_family;    /**< @brief Address family (IPv6 or IPv4 or both) the server will listen for MDNS query (it is optional).@n
                                                   Default value is defined by @ref AF_SUPPORTED.*/
    fnet_uint32_t               rr_ttl;         /**< @brief TTL value that indicates for how many seconds mDNS resource records is valid for mDNS querier, in seconds (it is optional).@n
                                                 * Default value is defined by @ref FNET_CFG_MDNS_RR_TTL. */
    fnet_uint32_t               rr_ttl_ip;      /**< @brief TTL value for IP header that is hop-count limit for the packet (it is optional).@n
                                                 * Default value is defined by @ref FNET_CFG_MDNS_RR_TTL_IP. */
    const fnet_char_t           *name;          /**< @brief Name used as a host-name and service-names, advertised by the MDNS server. */
};

/**************************************************************************/ /*!
 * @brief The mDNS Service structure defining application-specific
 * service, advertised by the mDNS server.
 * @see fnet_mdns_service_register()
 ******************************************************************************/
typedef struct fnet_mdns_service
{
    const char*         service_type;               /**< @brief Service Type. Null-terminated string. Example "_http._tcp". */
    fnet_uint16_t       service_port;               /**< @brief Service Port number (in network byte order). */
    const fnet_uint8_t* (*service_get_txt)(void);   /**< @brief Call-back function, which returns a pointer to the service TXT record (null-terminated).
                                                    If the service does not provide any TXT record, this parameter must be set to NULL. */
} fnet_mdns_service_t;

/**************************************************************************/ /*!
 * @brief mDNS server descriptor.
 * @see fnet_mdns_init(), fnet_mdns_release()
 ******************************************************************************/
typedef void* fnet_mdns_desc_t;

/**************************************************************************/ /*!
 * @brief  mDNS advertised-service descriptor.
 * @see fnet_mdns_service_register(), fnet_mdns_service_unregister()
 ******************************************************************************/
typedef void* fnet_mdns_service_desc_t;

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/ /*!
 *
 * @brief    Initializes Multicast DNS (mDNS) server/responder.
 *
 * @param params     Initialization parameters defined by @ref fnet_mdns_params.
 *
 * @return This function returns:
 *   - mDNS server descriptor if no error occurs.
 *   - @c 0 if an error occurs.
 *
 * @see fnet_mdns_release(), fnet_mdns_params
 *
 ******************************************************************************
 *
 * This function initializes the Multicast DNS (mDMS)server/responder.@n
 * It allocates all needed resources and registers the mDNS service in the polling list.@n
 * After the initialization, the user application should call the main polling
 * function @ref fnet_poll_service() periodically to run the mDNS service routine
 * in the background.
 *
 ******************************************************************************/
fnet_mdns_desc_t fnet_mdns_init( struct fnet_mdns_params *params );

/***************************************************************************/ /*!
 *
 * @brief    Releases the Multicast DNS (mDNS) server/responder.
 *
 * @param mdns_desc     mDNS server descriptor to be unregistered.
 *
 * @see fnet_mdns_init()
 *
 ******************************************************************************
 *
 * This function releases the mDNS Server assigned to the @c desc
 * descriptor.@n
 * It releases all occupied resources, and unregisters the mDNS service from
 * the polling list.
 *
 ******************************************************************************/
void fnet_mdns_release(fnet_mdns_desc_t mdns_desc);

/***************************************************************************/ /*!
 *
 * @brief    Registers application-specific service in the mDNS server.
 *
 * @param mdns_desc     mDNS server descriptor.
 *
 * @param service      Service parameters to be advertised by the mDNS server.
 *
 * @return This function returns:
 *   - service descriptor if no error occurs.
 *   - @c 0 if an error occurs.
 *
 * @see fnet_mdns_service_unregister(), fnet_mdns_service_t
 *
 ******************************************************************************
 *
 * This function registers an application service and starts its
 * advertisement by the mDNS server.
 *
 ******************************************************************************/
fnet_mdns_service_desc_t fnet_mdns_service_register(fnet_mdns_desc_t mdns_desc, const fnet_mdns_service_t *service);

/***************************************************************************/ /*!
 *
 * @brief    Unregisters application service from the mDNS server.
 *
 * @param service_desc     Service descriptor to be unregistered.
 *
 * @see fnet_mdns_service_register()
 *
 ******************************************************************************
 *
 * This function unregisters application service, assigned to the @c service_desc
 * descriptor, and stops its advertisement by the mDNS server.
 *
 ******************************************************************************/
void fnet_mdns_service_unregister(fnet_mdns_service_desc_t service_desc);

/***************************************************************************/ /*!
 *
 * @brief    Sends unsolicited mDNS announcement.
 *
 * @param mdns_desc     mDNS server descriptor.
 *
 * @see fnet_mdns_init()
 *
 ******************************************************************************
 *
 * This function sends unsolicited mDNS announcement.@n
 * Application may call it when any advertised application-specific parameter
 * has changed (e.g. network interface IP address or service TXT-record content).@n
 * RFC 6762: "At any time, if the rdata of any of a host's Multicast DNS records
 *  changes, the host MUST repeat the Announcing step to
 *  update neighboring caches.  For example, if any of a host's IP
 *  addresses change, it MUST re-announce those address records".
 *
 ******************************************************************************/
void fnet_mdns_announce(fnet_mdns_desc_t mdns_desc);

/***************************************************************************/ /*!
 *
 * @brief    Detects if the mDNS Server is enabled or disabled.
 *
 * @param desc     mDNS server descriptor
 *
 * @return This function returns:
 *          - @ref FNET_TRUE if the mDNS Server is successfully initialized.
 *          - @ref FNET_FALSE if the mDNS Server is not initialized or is released.
 *
 ******************************************************************************
 *
 * This function detects if the mDNS Server is initialized or is released.
 *
 ******************************************************************************/
fnet_bool_t fnet_mdns_is_enabled(fnet_mdns_desc_t desc);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* FNET_CFG_MDNS */

#endif /* _FNET_MDNS_H_ */
