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
* @brief DNS Resolver configuration file.
*
***************************************************************************/

/**************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 **************************************************************************/

#ifndef _FNET_DNS_CONFIG_H_

#define _FNET_DNS_CONFIG_H_

/*! @addtogroup fnet_dns_config */
/*! @{ */

/**************************************************************************/ /*!
 * @def      FNET_CFG_DNS_RESOLVER
 * @brief    DNS client/resolver support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_DNS_RESOLVER
    #define FNET_CFG_DNS_RESOLVER                   (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DNS_PORT
 * @brief   Default DNS port number (in network byte order).@n
 *          The DNS client uses this port for sending and receiving of DNS
 *          messages. @n
 *          It can be changed during the DNS Resolver initialization by the
 *          @ref fnet_dns_init() function.@n
 *          Default value is FNET_HTONS(53).
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DNS_PORT
    #define FNET_CFG_DNS_PORT                       (FNET_HTONS(53U))
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DNS_RETRANSMISSION_MAX
 * @brief   The maximum number of times the DNS client/resolver will retransmit
 *          the request message to a DNS server.@n
 *          Default value is @b @c 5.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DNS_RETRANSMISSION_MAX
    #define FNET_CFG_DNS_RETRANSMISSION_MAX         (5U)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DNS_RETRANSMISSION_TIMEOUT
 * @brief   Timeout for the response from the remote DNS server (in seconds).@n
 *          @n
 *          The recommended value is @c 1 second.@n
 *          If the DNS client does not receive any response from a DNS server,
 *          it sends new request message.@n
 *          Default value is @b @c 1.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DNS_RETRANSMISSION_TIMEOUT
    #define FNET_CFG_DNS_RETRANSMISSION_TIMEOUT     (1U)  /* seconds */
#endif

/*! @} */

#endif /* _FNET_DNS_CONFIG_H_ */
