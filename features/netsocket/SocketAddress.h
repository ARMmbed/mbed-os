/*
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

/** @file SocketAddress.h SocketAddress class */
/** \addtogroup netsocket
 * @{*/

#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <memory>
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
    /** Create an unspecified SocketAddress
     */
    constexpr SocketAddress() = default;

    /** Create a SocketAddress from a raw IP address and port
     *
     *  @note To construct from a host name, use @ref NetworkInterface::gethostbyname
     *
     *  @param addr     Raw IP address
     *  @param port     Optional 16-bit port, defaults to 0
     */
    SocketAddress(const nsapi_addr_t &addr, uint16_t port = 0);

    /** Create a SocketAddress from an IP address and port
     *
     *  @param addr     Null-terminated representation of the IP address
     *  @param port     Optional 16-bit port, defaults to 0
     */
    SocketAddress(const char *addr, uint16_t port = 0);

    /** Create a SocketAddress from raw IP bytes, IP version, and port
     *
     *  @param bytes    Raw IP address in big-endian order
     *  @param version  IP address version, NSAPI_IPv4 or NSAPI_IPv6
     *  @param port     Optional 16-bit port, defaults to 0
     */
    SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port = 0);

    /** Create a SocketAddress from another SocketAddress
     *
     *  @param addr  SocketAddress to copy
     */
    SocketAddress(const SocketAddress &addr);

    /** Destructor */
    ~SocketAddress() = default;

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
    void set_addr(const nsapi_addr_t &addr);

    /** Set the port
     *
     *  @param port     16-bit port
     */
    void set_port(uint16_t port)
    {
        _port = port;
    }

    /** Get the human-readable IP address
     *
     *  Allocates memory for a string and converts binary address to
     *  human-readable format. String is freed in the destructor.
     *
     *  @return         Null-terminated representation of the IP Address
     */
    const char *get_ip_address() const;

    /** Get the raw IP bytes
     *
     *  @return         Raw IP address in big-endian order
     */
    const void *get_ip_bytes() const
    {
        return _addr.bytes;
    }

    /** Get the IP address version
     *
     *  @return         IP address version, NSAPI_IPv4 or NSAPI_IPv6
     */
    nsapi_version_t get_ip_version() const
    {
        return _addr.version;
    }

    /** Get the raw IP address
     *
     *  @return         Raw IP address
     */
    nsapi_addr_t get_addr() const
    {
        return _addr;
    }

    /** Get the port
     *
     *  @return         The 16-bit port
     */
    uint16_t get_port() const
    {
        return _port;
    }

    /** Test if address is zero
     *
     *  @return         True if address is not zero
     */
    explicit operator bool() const;

    /** Copy address from another SocketAddress
     *
     * @param addr  SocketAddress to copy
     *
     * @retval SocketAddress reference to this address
     */
    SocketAddress &operator=(const SocketAddress &addr);

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
    mutable std::unique_ptr<char[]> _ip_address;
    nsapi_addr_t _addr{};
    uint16_t _port = 0;
};


#endif

/** @}*/
