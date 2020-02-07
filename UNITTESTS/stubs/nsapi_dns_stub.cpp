/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#include "netsocket/nsapi_dns.h"


nsapi_error_t nsapi_stub_return_value = NSAPI_ERROR_DNS_FAILURE;
NetworkStack::hostbyname_cb_t query_callback;
call_in_callback_cb_t callin_callback;

nsapi_error_t nsapi_dns_query(NetworkStack *stack, const char *host,
                              SocketAddress *addr, const char *interface_name, nsapi_version_t version)
{
    return nsapi_stub_return_value;
}

nsapi_size_or_error_t nsapi_dns_query_multiple(NetworkStack *stack, const char *host,
                                               SocketAddress *addresses, nsapi_size_t addr_count, const char *interface_name, nsapi_version_t version)
{
    return nsapi_stub_return_value;
}

nsapi_error_t nsapi_dns_query_async(NetworkStack *stack, const char *host,
                                    NetworkStack::hostbyname_cb_t callback, call_in_callback_cb_t call_in_cb, const char *interface_name,
                                    nsapi_version_t version)
{
    query_callback = callback;
    callin_callback = call_in_cb;
    return nsapi_stub_return_value;
}

nsapi_value_or_error_t nsapi_dns_query_multiple_async(NetworkStack *stack, const char *host,
                                                      NetworkStack::hostbyname_cb_t callback, nsapi_size_t addr_count,
                                                      call_in_callback_cb_t call_in_cb, const char *interface_name, nsapi_version_t version)
{
    query_callback = callback;
    callin_callback = call_in_cb;
    return nsapi_stub_return_value;
}

nsapi_error_t nsapi_dns_query_async_cancel(nsapi_error_t id)
{
    return nsapi_stub_return_value;
}

extern "C" nsapi_error_t nsapi_dns_add_server(nsapi_addr_t addr, const char *interface_name)
{
    return NSAPI_ERROR_OK;
}

void nsapi_dns_cache_reset()
{

}
