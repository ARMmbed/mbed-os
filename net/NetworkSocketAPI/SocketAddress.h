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

#include <stdint.h>


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

/** Enum of IP address versions
 *
 *  The IP version specifies the type of an IP address.
 *
 *  @enum nsapi_version_t
 */
enum nsapi_version_t {
    NSAPI_IPv4, /*!< Address is IPv4 */
    NSAPI_IPv6, /*!< Address is IPv6 */
};

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

// Predeclared classes
class NetworkStack;


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
     *  @param iface    Network stack to use for DNS resolution
     *  @param host     Hostname to resolve
     *  @param port     Optional 16-bit port
     */
    SocketAddress(NetworkStack *iface, const char *host, uint16_t port = 0);

    /** Create a SocketAddress from an IP address and port
     *
     *  @param host     Null-terminated representation of the IP address
     *  @param port     Optional 16-bit port
     */
    SocketAddress(const char *addr = 0, uint16_t port = 0);

    /** Create a SocketAddress from a raw IP address and port
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
     */
    void set_ip_address(const char *addr);

    /** Set the raw IP address
     *
     *  @param bytes    Raw IP address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    void set_ip_bytes(const void *bytes, nsapi_version_t version);

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

    /** Get the raw IP address
     *
     *  @return         Raw IP address in big-endian order
     */
    const void *get_ip_bytes() const;

    /** Get the IP address version
     *
     *  @return         IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    nsapi_version_t get_ip_version() const;
    
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

private:
    char _ip_address[NSAPI_IP_SIZE];
    uint8_t _ip_bytes[NSAPI_IP_BYTES];
    nsapi_version_t _ip_version;
    uint16_t _port;
};

#endif
