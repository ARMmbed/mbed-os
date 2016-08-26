/* nsapi_dns.h
 * Original work Copyright (c) 2013 Henry Leinen (henry[dot]leinen [at] online [dot] de)
 * Modified work Copyright (c) 2015 ARM Limited
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
#ifndef NSAPI_DNS_H
#define NSAPI_DNS_H

#include "nsapi_types.h"
#ifdef __cplusplus
#include "network-socket/NetworkStack.h"
#endif

#ifndef __cplusplus


/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @param version  IP version to resolve
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
int nsapi_dns_query(nsapi_stack_t *stack, nsapi_addr_t *addr,
        const char *host, nsapi_version_t version);

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param host       Hostname to resolve
 *  @param version    IP version to resolve
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
int nsapi_dns_query_multiple(nsapi_stack_t *stack,
        nsapi_addr_t *addr, unsigned addr_count,
        const char *host, nsapi_version_t version);

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
int nsapi_dns_add_server(nsapi_addr_t addr);


#else


/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
int nsapi_dns_query(NetworkStack *stack, SocketAddress *addr,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
extern "C" int nsapi_dns_query(nsapi_stack_t *stack, nsapi_addr_t *addr,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
template <typename S>
int nsapi_dns_query(S *stack, SocketAddress *addr,
        const char *host, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query(nsapi_create_stack(stack), addr, host, version);
}

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param host       Hostname to resolve
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
int nsapi_dns_query_multiple(NetworkStack *stack,
        SocketAddress *addr, unsigned addr_count,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param host       Hostname to resolve
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
extern "C" int nsapi_dns_query_multiple(nsapi_stack_t *stack,
        nsapi_addr_t *addr, unsigned addr_count,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param host       Hostname to resolve
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
template <typename S>
int nsapi_dns_query_multiple(S *stack,
        SocketAddress *addr, unsigned addr_count,
        const char *host, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query_multiple(nsapi_create_stack(stack),
                addr, addr_count, host, version);
}

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
extern "C" int nsapi_dns_add_server(nsapi_addr_t addr);

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
static inline int nsapi_dns_add_server(const SocketAddress &address)
{
    return nsapi_dns_add_server(address.get_addr());
}

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
static inline int nsapi_dns_add_server(const char *address)
{
    return nsapi_dns_add_server(SocketAddress(address));
}


#endif

#endif
