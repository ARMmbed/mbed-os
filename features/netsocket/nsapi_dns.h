
/** \addtogroup netsocket */
/** @{*/
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
#include "netsocket/NetworkStack.h"
#endif

#ifndef __cplusplus


/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param host     Hostname to resolve
 *  @param addr     Destination for the host address
 *  @param version  IP version to resolve
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_error_t nsapi_dns_query(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_version_t version);

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param host       Hostname to resolve
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param version    IP version to resolve
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_size_or_error_t nsapi_dns_query_multiple(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_size_t addr_count, nsapi_version_t version);

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
nsapi_error_t nsapi_dns_add_server(nsapi_addr_t addr);


#else

typedef mbed::Callback<nsapi_error_t (int delay_ms, mbed::Callback<void()> user_cb)> call_in_callback_cb_t;

/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param host     Hostname to resolve
 *  @param addr     Destination for the host address
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_error_t nsapi_dns_query(NetworkStack *stack, const char *host,
        SocketAddress *addr, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param host     Hostname to resolve
 *  @param callback Callback that is called for result
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure or an unique id that
 *                  represents the hostname translation operation and can be passed to
 *                  cancel, NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_error_t nsapi_dns_query_async(NetworkStack *stack, const char *host,
        NetworkStack::hostbyname_cb_t callback, call_in_callback_cb_t call_in_cb,
        nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname (asynchronous)
 *
 *  @param stack    Network stack as target for DNS query
 *  @param host     Hostname to resolve
 *  @param addr     Destination for the host address
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
extern "C" nsapi_error_t nsapi_dns_query(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname
 *
 *  @param stack    Network stack as target for DNS query
 *  @param host     Hostname to resolve
 *  @param addr     Destination for the host address
 *  @param version  IP version to resolve (defaults to NSAPI_IPv4)
 *  @return         0 on success, negative error code on failure
 *                  NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
template <typename S>
nsapi_error_t nsapi_dns_query(S *stack, const char *host,
        SocketAddress *addr, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query(nsapi_create_stack(stack), host, addr, version);
}

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param host       Hostname to resolve
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_size_or_error_t nsapi_dns_query_multiple(NetworkStack *stack, const char *host,
        SocketAddress *addr, nsapi_size_t addr_count, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for an IP address of a given hostname (asynchronous)
 *
 *  @param stack      Network stack as target for DNS query
 *  @param host       Hostname to resolve
 *  @param callback   Callback that is called for result
 *  @param addr_count Number of addresses allocated in the array
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           0 on success, negative error code on failure or an unique id that
                      represents the hostname translation operation and can be passed to
 *                    cancel, NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
nsapi_size_or_error_t nsapi_dns_query_multiple_async(NetworkStack *stack, const char *host,
        NetworkStack::hostbyname_cb_t callback, nsapi_size_t addr_count,
        call_in_callback_cb_t call_in_cb, nsapi_version_t version = NSAPI_IPv4);

/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param host       Hostname to resolve
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
extern "C" nsapi_size_or_error_t nsapi_dns_query_multiple(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_size_t addr_count, nsapi_version_t version = NSAPI_IPv4);


/** Query a domain name server for multiple IP address of a given hostname
 *
 *  @param stack      Network stack as target for DNS query
 *  @param host       Hostname to resolve
 *  @param addr       Array for the host addresses
 *  @param addr_count Number of addresses allocated in the array
 *  @param version    IP version to resolve (defaults to NSAPI_IPv4)
 *  @return           Number of addresses found on success, negative error code on failure
 *                    NSAPI_ERROR_DNS_FAILURE indicates the host could not be found
 */
template <typename S>
nsapi_size_or_error_t nsapi_dns_query_multiple(S *stack, const char *host,
        SocketAddress *addr, nsapi_size_t addr_count, nsapi_version_t version = NSAPI_IPv4)
{
    return nsapi_dns_query_multiple(nsapi_create_stack(stack),
                host, addr, addr_count, version);
}

/** Cancels asynchronous hostname translation
  *
  *  When translation is cancelled, callback will not be called.
  *
  *  @param id       Unique id of the hostname translation operation
  *  @return         0 on success, negative error code on failure
  */
nsapi_error_t nsapi_dns_query_async_cancel(nsapi_error_t id);

/** Set a call in callback
 *
 *  Can be used to provide an application specific call in callback to
 *  DNS resolver. When callback is set it is used instead of stack
 *  specific call in callbacks.
 *
 *  @param callback  Callback
 */
void nsapi_dns_call_in_set(call_in_callback_cb_t callback);

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
extern "C" nsapi_error_t nsapi_dns_add_server(nsapi_addr_t addr);

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
static inline nsapi_error_t nsapi_dns_add_server(const SocketAddress &address)
{
    return nsapi_dns_add_server(address.get_addr());
}

/** Add a domain name server to list of servers to query
 *
 *  @param addr     Destination for the host address
 *  @return         0 on success, negative error code on failure
 */
static inline nsapi_error_t nsapi_dns_add_server(const char *address)
{
    return nsapi_dns_add_server(SocketAddress(address));
}


#endif

#endif

/** @}*/
