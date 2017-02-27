
/** \addtogroup netsocket */
/** @{*/
/* SocketAddress
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

#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include "nsapi_types.h"
#include "mbed_toolchain.h"

// Predeclared classes
class NetworkStack;
class NetworkInterface;


/** SocketAddress class
 *
 *  Representation of an IP address and port pair. 
 */
class SocketAddress {
public:
    /** Create a SocketAddress from a hostname and port
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  On failure, the IP address and port will be set to zero
     *
     *  @param stack    Network stack to use for DNS resolution
     *  @param host     Hostname to resolve
     *  @param port     Optional 16-bit port
     *  @deprecated
     *      Constructors hide possible errors. Replaced by
     *      NetworkInterface::gethostbyname.
     */
    template <typename S>
    MBED_DEPRECATED_SINCE("mbed-os-5.1.3",
        "Constructors hide possible errors. Replaced by "
        "NetworkInterface::gethostbyname.")
    SocketAddress(S *stack, const char *host, uint16_t port = 0)
    {
        _SocketAddress(nsapi_create_stack(stack), host, port);
    }

    /** Create a SocketAddress from a raw IP address and port
     *
     *  @param addr     Raw IP address
     *  @param port     Optional 16-bit port
     */
    SocketAddress(nsapi_addr_t addr = nsapi_addr_t(), uint16_t port = 0);

    /** Create a SocketAddress from an IP address and port
     *
     *  @param host     Null-terminated representation of the IP address
     *  @param port     Optional 16-bit port
     */
    SocketAddress(const char *addr, uint16_t port = 0);

    /** Create a SocketAddress from raw IP bytes, IP version, and port
     *
     *  @param bytes    Raw IP address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     *  @param port     Optional 16-bit port
     */
    SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port = 0);

    /** Create a SocketAddress from another SocketAddress
     *
     *  @param address  SocketAddress to copy
     */
    SocketAddress(const SocketAddress &addr);
   
    /** Set the IP address
     *
     *  @param addr     Null-terminated represention of the IP address
     *  @return         True if address is a valid representation of an IP address,
     *                  otherwise False and SocketAddress is set to null
     */
    bool set_ip_address(const char *addr);

    /** Set the raw IP bytes and IP version
     *
     *  @param bytes    Raw IP address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    void set_ip_bytes(const void *bytes, nsapi_version_t version);

    /** Set the raw IP address
     *
     *  @param addr     Raw IP address
     */
    void set_addr(nsapi_addr_t addr);

    /** Set the port
     *
     *  @param port     16-bit port
     */
    void set_port(uint16_t port);
    
    /** Get the IP address
     *
     *  @return         Null-terminated representation of the IP Address
     */
    const char *get_ip_address() const;

    /*  Get the raw IP bytes
     *
     *  @return         Raw IP address in big-endian order
     */
    const void *get_ip_bytes() const;

    /** Get the IP address version
     *
     *  @return         IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    nsapi_version_t get_ip_version() const;

    /** Get the raw IP address
     *
     *  @return         Raw IP address
     */
    nsapi_addr_t get_addr() const;
    
    /** Get the port
     *
     *  @return         The 16-bit port
     */
    uint16_t get_port() const;

    /** Test if address is zero
     *
     *  @return         True if address is not zero
     */
    operator bool() const;

    /** Compare two addresses for equality
     *
     *  @return         True if both addresses are equal
     */
    friend bool operator==(const SocketAddress &a, const SocketAddress &b);

    /** Compare two addresses for equality
     *
     *  @return         True if both addresses are not equal
     */
    friend bool operator!=(const SocketAddress &a, const SocketAddress &b);

private:
    void _SocketAddress(NetworkStack *iface, const char *host, uint16_t port);

    mutable char _ip_address[NSAPI_IP_SIZE];
    nsapi_addr_t _addr;
    uint16_t _port;
};


#endif

/** @}*/
