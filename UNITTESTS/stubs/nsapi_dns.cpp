#include "netsocket/nsapi_dns.h"


nsapi_error_t nsapi_stub_return_value = NSAPI_ERROR_DNS_FAILURE;

nsapi_error_t nsapi_dns_query(NetworkStack *stack, const char *host,
                              SocketAddress *addr, nsapi_version_t version)
{
    return nsapi_stub_return_value;
}

nsapi_error_t nsapi_dns_query_async(NetworkStack *stack, const char *host,
                                    NetworkStack::hostbyname_cb_t callback, call_in_callback_cb_t call_in_cb,
                                    nsapi_version_t version)
{
    return nsapi_stub_return_value;
}

nsapi_error_t nsapi_dns_query_async_cancel(nsapi_error_t id)
{
    return nsapi_stub_return_value;
}

extern "C" nsapi_error_t nsapi_dns_add_server(nsapi_addr_t addr)
{
    return NSAPI_ERROR_OK;
}
