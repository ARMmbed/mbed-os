/* Socket
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

/** Enum of address families
 *  @enum nsapi_family_t
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
class NetworkInterface;


/** A general address class composed of the IP address and optional port
 */
class SocketAddress {
public:
    /** SocketAddress construction using DNS resolution
     *  @param iface    NetworkInterface to use for DNS resolution
     *  @param addr     Null-terminated hostname that will be resolved
     *  @param port     Optional 16-bit port
     *  @note on failure, IP address and port will be set to zero
     */
    SocketAddress(NetworkInterface *iface, const char *addr, uint16_t port = 0);

    /** SocketAddress construction
     *  @param addr     Null-terminated IP address
     *  @param port     Optional 16-bit port
     */
    SocketAddress(const char *addr = 0, uint16_t port = 0);

    /** SocketAddress construction
     *  @param bytes    Bytes to assign to address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     *  @param port     Optional 16-bit port
     */
    SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port = 0);

    /** SocketAddress construction
     *  @param addr     SocketAddress to copy
     */
    SocketAddress(const SocketAddress &addr);
   
    /** Set the IP address
     *  @param addr     Null-terminated string representing the IP address
     */
    void set_ip_address(const char *addr);

    /** Set the IP address bytes directly
     *  @param bytes    Bytes to assign to address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    void set_ip_bytes(const void *bytes, nsapi_version_t version);

    /** Set the port
     *  @param port     16-bit port
     */
    void set_port(uint16_t port);
    
    /** Get the IP address
     *  @return         The string representation of the IP Address
     */
    const char *get_ip_address() const;

    /** Get the IP address bytes directly
     *  @return         IP address bytes
     */
    const void *get_ip_bytes() const;

    /** Get the type of the IP address
     *  @return         IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    nsapi_version_t get_ip_version() const;
    
    /** Get the port
     *  @return         The 16-bit port
     */
    uint16_t get_port() const;

    /** Determine if address is all zeros
     *  @return         True if address is not zero address
     */
    operator bool() const;

private:
    char _ip_address[NSAPI_IP_SIZE];
    uint8_t _ip_bytes[NSAPI_IP_BYTES];
    nsapi_version_t _ip_version;
    uint16_t _port;
};

#endif
