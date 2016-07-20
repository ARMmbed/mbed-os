/* nsapi.h - The network socket API
 * Copyright (c) 2015 ARM Limited
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

#ifndef NSAPI_TYPES_H
#define NSAPI_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** Enum of standardized error codes 
 *
 *  Valid error codes have negative values and may
 *  be returned by any network operation.
 *
 *  @enum nsapi_error_t
 */
typedef enum nsapi_error {
    NSAPI_ERROR_WOULD_BLOCK   = -3001,     /*!< no data is not available but call is non-blocking */
    NSAPI_ERROR_UNSUPPORTED   = -3002,     /*!< unsupported functionality */
    NSAPI_ERROR_PARAMETER     = -3003,     /*!< invalid configuration */
    NSAPI_ERROR_NO_CONNECTION = -3004,     /*!< not connected to a network */
    NSAPI_ERROR_NO_SOCKET     = -3005,     /*!< socket not available for use */
    NSAPI_ERROR_NO_ADDRESS    = -3006,     /*!< IP address is not known */
    NSAPI_ERROR_NO_MEMORY     = -3007,     /*!< memory resource not available */
    NSAPI_ERROR_DNS_FAILURE   = -3008,     /*!< DNS failed to complete successfully */
    NSAPI_ERROR_DHCP_FAILURE  = -3009,     /*!< DHCP failed to complete successfully */
    NSAPI_ERROR_AUTH_FAILURE  = -3010,     /*!< connection to access point faield */
    NSAPI_ERROR_DEVICE_ERROR  = -3011,     /*!< failure interfacing with the network procesor */
} nsapi_error_t;


/** Maximum size of IP address representation
 */
#define NSAPI_IP_SIZE NSAPI_IPv6_SIZE

/** Maximum number of bytes for IP address
 */
#define NSAPI_IP_BYTES NSAPI_IPv6_BYTES

/** Maximum size of MAC address representation
 */
#define NSAPI_MAC_SIZE 18

/** Maximum number of bytes for MAC address
 */
#define NSAPI_MAC_BYTES 6

/** Size of IPv4 representation
 */
#define NSAPI_IPv4_SIZE 16

/** Number of bytes in IPv4 address
 */
#define NSAPI_IPv4_BYTES 4

/** Size of IPv6 representation
 */
#define NSAPI_IPv6_SIZE 40

/** Number of bytes in IPv6 address
 */
#define NSAPI_IPv6_BYTES 16

/** Enum of IP address versions
 *
 *  The IP version specifies the type of an IP address.
 *
 *  @enum nsapi_version_t
 */
typedef enum nsapi_version {
    NSAPI_IPv4, /*!< Address is IPv4 */
    NSAPI_IPv6, /*!< Address is IPv6 */
} nsapi_version_t;

/** IP address structure for passing IP addresses by value
 */
typedef struct nsapi_addr {
    /** IP version
     *  NSAPI_IPv4 or NSAPI_IPv6
     */
    nsapi_version_t version;

    /** IP address
     *  The raw bytes of the IP address stored in big-endian format
     */
    uint8_t bytes[NSAPI_IP_BYTES];
} nsapi_addr_t;


/** Opaque handle for network sockets
 */
typedef void *nsapi_socket_t;


/** Enum of socket protocols
 *
 *  The socket protocol specifies a particular protocol to
 *  be used with a newly created socket. 
 *
 *  @enum nsapi_protocol_t
 */
typedef enum nsapi_protocol {
   NSAPI_TCP, /*!< Socket is of TCP type */
   NSAPI_UDP, /*!< Socket is of UDP type */
} nsapi_protocol_t;

/*  Enum of standardized stack option levels
 *
 *  @enum nsapi_level_t
 */
typedef enum nsapi_level {
    NSAPI_STACK,  /*!< Stack option level */
    NSAPI_SOCKET, /*!< Socket option level */
} nsapi_level_t;

/*  Enum of standardized stack options
 *
 *  These options may not be supported on all stacks, in which
 *  case NSAPI_ERROR_UNSUPPORTED may be returned from setsockopt.
 *
 *  @enum nsapi_option_t
 */
typedef enum nsapi_option {
    NSAPI_REUSEADDR, /*!< Allow bind to reuse local addresses */
    NSAPI_KEEPALIVE, /*!< Enables sending of keepalive messages */
    NSAPI_KEEPIDLE,  /*!< Sets timeout value to initiate keepalive */
    NSAPI_KEEPINTVL, /*!< Sets timeout value for keepalive */
    NSAPI_LINGER,    /*!< Keeps close from returning until queues empty */
    NSAPI_SNDBUF,    /*!< Sets send buffer size */
    NSAPI_RCVBUF,    /*!< Sets recv buffer size */
} nsapi_option_t;


#ifdef __cplusplus
}
#endif

#endif
