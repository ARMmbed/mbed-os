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
#include "network-socket/NetworkStack.h"


/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
NSAPI_C_LINKAGE
int nsapi_dns_query(nsapi_stack_t *stack, nsapi_addr_t *addr,
        const char *host, nsapi_version_t version);

#ifdef __cplusplus
int nsapi_dns_query(nsapi_stack_t *stack, nsapi_addr_t *addr,
        const char *host);
int nsapi_dns_query(NetworkStack *stack, SocketAddress *addr,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

template <typename S>
int nsapi_dns_query(S *stack, SocketAddress *addr,
        const char *host, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query(nsapi_create_stack(stack), addr, host, version);
}
#endif


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
NSAPI_C_LINKAGE
int nsapi_dns_query_multiple(nsapi_stack_t *stack,
        nsapi_addr_t *addr, unsigned addr_count,
        const char *host, nsapi_version_t version);

#ifdef __cplusplus
int nsapi_dns_query_multiple(nsapi_stack_t *stack,
        nsapi_addr_t *addr, unsigned addr_count,
        const char *host);
int nsapi_dns_query_multiple(NetworkStack *stack,
        SocketAddress *addr, unsigned addr_count,
        const char *host, nsapi_version_t version = NSAPI_IPv4);

template <typename S>
int nsapi_dns_query_multiple(S *stack,
        SocketAddress *addr, unsigned addr_count,
        const char *host, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query_multiple(nsapi_create_stack(stack),
                addr, addr_count, host, version);
}
#endif


/** Add a domain name server to list of servers to query
 *
 *  @param stack    Network stack as target for DNS query
 *  @param addr     Destination for the host address
 *  @param host     Hostname to resolve
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
NSAPI_C_LINKAGE
int nsapi_dns_add_server(nsapi_addr_t addr);

#ifdef __cplusplus
int nsapi_dns_add_server(const SocketAddress &address);
int nsapi_dns_add_server(const char *address);
#endif


#endif
