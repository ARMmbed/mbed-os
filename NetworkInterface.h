/* NetworkInterface Base Class
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

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "SocketInterface.h"
#include "stdint.h"

/** Maximum storage needed for IP address and MAC addresses
 */
#define NS_IP_SIZE 16
#define NS_MAC_SIZE 18

/**
 *  @enum ns_error_t
 *  @brief enum of standardized error codes
 */
enum ns_error_t {
    NS_ERROR_WOULD_BLOCK   = -3000,     /*!< no data is not available but call is non-blocking */
    NS_ERROR_TIMEOUT       = -3001,     /*!< operation took longer than allowed */
    NS_ERROR_NO_CONNECTION = -3002,     /*!< not connected to a network */
    NS_ERROR_NO_SOCKET     = -3003,     /*!< socket not available for use */
    NS_ERROR_NO_ADDRESS    = -3004,     /*!< IP address is not known */
    NS_ERROR_NO_MEMORY     = -3005,     /*!< memory resource not available */
    NS_ERROR_DNS_FAILURE   = -3006,     /*!< DNS failed to complete successfully */
    NS_ERROR_DHCP_FAILURE  = -3007,     /*!< DHCP failed to complete successfully */
    NS_ERROR_AUTH_FAILURE  = -3008,     /*!< connection to access point faield */
    NS_ERROR_DEVICE_ERROR  = -3009      /*!< failure interfacing with the network procesor */
};

/** NetworkInterface class
 *  Common interface that is shared between all hardware that
 *  can connect to a network over IP.
 */
class NetworkInterface
{
public:
    virtual ~NetworkInterface() {};

    /** Get the IP address
     *  @return IP address of the interface or 0 if not yet connected
     */
    virtual const char *getIPAddress() = 0;

    /** Get the current MAC address
     *  @return String MAC address of the interface
     */
    virtual const char *getMACAddress() = 0;

    /** Get the current status of the interface
     *  @return true if connected
     */
    virtual bool isConnected();

    /** Looks up the specified host's IP address
     *  @param name URL of host
     *  @param ip Buffer to hold IP address, must be at least SOCK_IP_SIZE
     *  @return 0 on success
     */
    virtual int32_t getHostByName(const char *name, char *ip);

protected:
    friend class Socket;

    /** Internally create a socket
     *  @param proto The type of socket to open, NS_TCP or NS_UDP
     *  @return The allocated socket
     */
    virtual SocketInterface *createSocket(ns_protocol_t proto) = 0;

    /** Internally destroy a socket
     *  @param socket An allocated SocketInterface
     *  @returns 0 on success
     */
    virtual void destroySocket(SocketInterface *socket) = 0;
};

#endif
