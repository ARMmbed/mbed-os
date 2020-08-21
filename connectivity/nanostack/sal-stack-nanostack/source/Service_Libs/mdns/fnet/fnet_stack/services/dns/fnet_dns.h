/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
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
* @brief DNS Resolver API.
*
***************************************************************************/

#ifndef _FNET_DNS_H_

#define _FNET_DNS_H_

#if FNET_CFG_DNS_RESOLVER || defined(__DOXYGEN__)

/*! @addtogroup fnet_dns
*
* The DNS client/resolver service allows user application to resolve IP addresses
* of internet hosts that are identified by a host name. @n
* It does this by sending DNS request to a DNS Server.
* The IP address of a DNS Server is specified manually or can be obtained from
* the DHCP Server for the Local Area Network. @n
* @n
* After the DNS client is initialized by calling the @ref fnet_dns_init() function,
* the user application should call the main service-polling function
* @ref fnet_poll_service() periodically in background. @n
* The resolved IP-address will be passed to the @ref fnet_dns_callback_resolved_t callback function,
* which is set during the DNS-client service initialization.
* @n
* The DNS client service is released automatically as soon as the requested host name is
* fully resolved or an error occurs. Your application code may still continue
* to call @ref fnet_poll_service() to handle other services, but this will not have any
* impact on the DNS client communication until you initialize the next IP address resolving by calling
* @ref fnet_dns_init() again. @n
* @n
* For the DNS-client service example, refer to the FNET Shell demo source code.@n
* @note
* Current version of the DNS client:
*  - does not cache the resolved IP addresses.
*  - can process only one request at a time.
*  - uses UDP protocol, without message truncation.
*  - does not support DNS servers without recursion (all real-life DNS servers support it).
*  - takes the first resolved IP address, even if the DNS server provides several ones.
*
* Configuration parameters:
* - @ref FNET_CFG_DNS
* - @ref FNET_CFG_DNS_RESOLVER
* - @ref FNET_CFG_DNS_PORT
* - @ref FNET_CFG_DNS_RETRANSMISSION_MAX
* - @ref FNET_CFG_DNS_RETRANSMISSION_TIMEOUT
* - @ref FNET_CFG_ND6_RDNSS
* - @ref FNET_CFG_ND6_RDNSS_LIST_SIZE
*
*/

/*! @{ */

/**************************************************************************/ /*!
 * @brief DNS-client states.@n
 * Used mainly for debugging purposes.
 ******************************************************************************/
typedef enum
{
    FNET_DNS_STATE_DISABLED = 0,    /**< @brief The DNS-client service is not
                                    * initialized or is released.*/
    FNET_DNS_STATE_TX,              /**< @brief The DNS-client service sends the
                                    * request to the DNS server.*/
    FNET_DNS_STATE_RX,             /**< @brief The DNS-client service waits a response from the DNS server.*/
    FNET_DNS_STATE_RELEASE          /**< @brief The DNS resolving is completed
                                    * or received error.*/
} fnet_dns_state_t;

/**************************************************************************/ /*!
 * @brief Initialization parameters for the @ref fnet_dns_init() function.
 ******************************************************************************/
struct fnet_dns_resolved_addr
{
    struct sockaddr             resolved_addr;      /**< @brief Socket address of the resolved host name.*/
    fnet_uint32_t               resolved_addr_ttl;  /**< @brief Specifies the time interval (in seconds) that the
                                                    * resolved address may be cached before it should be discarded.*/
};

/**************************************************************************/ /*!
 * @brief Prototype of the DNS-client callback function that is
 * called when the DNS client has completed the resolving.
 *
 * @param addr_family       IP address family.@n
 *                          It determines the address pointed to by @c addr.
 * @param addr_list         Pointer to the list of addresses or @ref FNET_NULL if the resolving was failed.
 * @param addr_list_size    Number of resolved addresses in addr_list.
 * @param cookie            User-application specific parameter. It's set during
 *                          the DNS-client service initialization as part of
 *                          @ref fnet_dns_params.
 *
 * @see fnet_dns_resolve(), fnet_dns_params
 ******************************************************************************/
typedef void(*fnet_dns_callback_resolved_t)(const struct fnet_dns_resolved_addr *addr_list, fnet_size_t addr_list_size, fnet_uint32_t cookie);

/**************************************************************************/ /*!
 * @brief Initialization parameters for the @ref fnet_dns_init() function.
 ******************************************************************************/
struct fnet_dns_params
{
    struct sockaddr                 dns_server_addr;    /**< @brief Socket address of the remote DNS server to
                                                        * connect to. */
    fnet_char_t                     *host_name;         /**< @brief Host name to resolve (null-terminated string). */
    fnet_address_family_t           addr_family;        /**< @brief Family of the IP Address which is queried.*/
    fnet_dns_callback_resolved_t    callback;            /**< @brief Pointer to the callback function defined by
                                                        * @ref fnet_dns_callback_resolved_t. It is called when the
                                                        * DNS-client resolving is finished or an error is occurred. */
    fnet_uint32_t                   cookie;             /**< @brief Optional application-specific parameter. @n
                                                        * It's passed to the @c callback
                                                        * function as input parameter. */
};

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/ /*!
 *
 * @brief    Initializes DNS client service and starts the host name resolving.
 *
 * @param params     Initialization parameters.
 *
 * @return This function returns:
 *   - @ref FNET_OK if no error occurs.
 *   - @ref FNET_ERR if an error occurs.
 *
 * @see fnet_dns_params, fnet_dns_callback_resolved_t, fnet_dns_release()
 *
 ******************************************************************************
 *
 * This function initializes the DNS client service and starts the
 * host name resolving. It allocates all needed
 * resources and registers the DNS service in the polling list.@n
 * After the initialization, the user application should call the main polling
 * function @ref fnet_poll_service() periodically to run the DNS service routine
 * in the background.@n
 * The resolved IP-address will be passed to the @ref fnet_dns_callback_resolved_t callback function,
 * which is set in @c params. @n
 * The DNS service is released automatically as soon as the
 * resolving is finished or an error is occurred.
 *
 ******************************************************************************/
fnet_return_t fnet_dns_init( struct fnet_dns_params *params );

/***************************************************************************/ /*!
 *
 * @brief    Aborts the resolving and releases the DNS-client service.
 *
 * @see fnet_dns_init()
 *
 ******************************************************************************
 *
 * This function stops the DNS-client service. It releases all resources
 * used by the service, and unregisters it from the polling list.@n
 * Use this function only in the case of the early termination of the service,
 * because the DNS service is released automatically as soon as the
 * resolving is finished.
 *
 ******************************************************************************/
void fnet_dns_release(void);

/***************************************************************************/ /*!
 *
 * @brief    Retrieves the current state of the DNS-client service (for debugging purposes).
 *
 * @return This function returns the current state of the DNS-client service.
 *   The state is defined by the @ref fnet_dns_state_t.
 *
 ******************************************************************************
 *
 * This function returns the current state of the DNS-client service.
 * If the state is @ref FNET_DNS_STATE_DISABLED, the DNS client is not initialized
 * or released.@n
 * It is used mainly for debugging purposes.
 *
 ******************************************************************************/
fnet_dns_state_t fnet_dns_state(void);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* FNET_CFG_DNS_RESOLVER */

#endif /* _FNET_DNS_H_ */
