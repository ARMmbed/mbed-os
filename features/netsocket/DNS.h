/*
 * Copyright (c) 2018 ARM Limited
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

/** @file DNS.h Domain Name Service */
/** @addtogroup netsocket
 * @{ */

#ifndef DNS_H
#define DNS_H

/** Base class for DNS provider */
class DNS {
public:

    /** Translate a hostname to an IP address with specific version using network interface name.
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  @param host     Hostname to resolve.
     *  @param address  Pointer to a SocketAddress to store the result.
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC).
     *  @param interface_name  Network interface name
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t gethostbyname(const char *host,
                                        SocketAddress *address, nsapi_version_t version = NSAPI_UNSPEC, const char *interface_name = NULL) = 0;
    /** Hostname translation callback for gethostbyname_async.
     *
     *  The callback is called after DNS resolution completes, or a failure occurs.
     *
     *  Callback should not take more than 10ms to execute, otherwise it might
     *  prevent underlying thread processing. A portable user of the callback
     *  should not make calls to network operations due to stack size limitations.
     *  The callback should not perform expensive operations such as socket recv/send
     *  calls or blocking operations.
     *
     *  @param result  NSAPI_ERROR_OK on success, negative error code on failure.
     *  @param address On success, destination for the host SocketAddress.
     */
    typedef mbed::Callback<void (nsapi_error_t result, SocketAddress *address)> hostbyname_cb_t;

    /** Translate a hostname to an IP address (asynchronous)
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolved using a UDP socket on the stack.
     *
     *  The call is non-blocking. The result of the DNS operation is returned by the callback.
     *  If this function returns failure, the callback will not be called. If it is successful,
     *  (the IP address was found from the DNS cache), the callback will be called
     *  before the function returns.
     *
     *  @param host     Hostname to resolve.
     *  @param callback Callback that is called to return the result.
     *  @param version  IP version of address to resolve. NSAPI_UNSPEC indicates that the
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC).
     *  @param interface_name  Network interface name
     *  @return         NSAPI_ERROR_OK on immediate success,
     *                  negative error code on immediate failure or
     *                  a positive unique ID that represents the hostname translation operation
     *                  and can be passed to cancel.
     */
    virtual nsapi_value_or_error_t gethostbyname_async(const char *host, hostbyname_cb_t callback, nsapi_version_t version = NSAPI_UNSPEC,
                                                       const char *interface_name = NULL) = 0;

    /** Cancel asynchronous hostname translation.
     *
     *  When translation is canceled, callback is not called.
     *
     *  @param id       Unique ID of the hostname translation operation returned by gethostbyname_async.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t gethostbyname_async_cancel(int id) = 0;

    /** Add a domain name server to list of servers to query.
     *
     *  @param address  DNS server host address.
     *  @param interface_name  Network interface name
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t add_dns_server(const SocketAddress &address, const char *interface_name = NULL) = 0;
};

#endif

/** @} */
