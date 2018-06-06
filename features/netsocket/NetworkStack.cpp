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

#include "NetworkStack.h"
#include "nsapi_dns.h"
#include "mbed.h"
#include "stddef.h"
#include <new>

// Default NetworkStack operations
const char *NetworkStack::get_ip_address()
{
    return 0;

}
nsapi_error_t NetworkStack::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
{
    if (name[0] == '\0') {
        return NSAPI_ERROR_PARAMETER;
    }

    // check for simple ip addresses
    if (address->set_ip_address(name)) {
        if (version != NSAPI_UNSPEC && address->get_ip_version() != version) {
            return NSAPI_ERROR_DNS_FAILURE;
        }

        return NSAPI_ERROR_OK;
    }

    // if the version is unspecified, try to guess the version from the
    // ip address of the underlying stack
    if (version == NSAPI_UNSPEC) {
        SocketAddress testaddress;
        if (testaddress.set_ip_address(this->get_ip_address())) {
            version = testaddress.get_ip_version();
        }
    }

    return nsapi_dns_query(this, name, address, version);
}

nsapi_value_or_error_t NetworkStack::gethostbyname_async(const char *name, hostbyname_cb_t callback, nsapi_version_t version)
{
    SocketAddress address;

    if (name[0] == '\0') {
        return NSAPI_ERROR_PARAMETER;
    }

    // check for simple ip addresses
    if (address.set_ip_address(name)) {
        if (version != NSAPI_UNSPEC && address.get_ip_version() != version) {
            return NSAPI_ERROR_DNS_FAILURE;
        }

        callback(NSAPI_ERROR_OK, &address);
        return NSAPI_ERROR_OK;
    }

    // if the version is unspecified, try to guess the version from the
    // ip address of the underlying stack
    if (version == NSAPI_UNSPEC) {
        SocketAddress testaddress;
        if (testaddress.set_ip_address(this->get_ip_address())) {
            version = testaddress.get_ip_version();
        }
    }

    call_in_callback_cb_t call_in_cb = get_call_in_callback();

    return nsapi_dns_query_async(this, name, callback, call_in_cb, version);
}

nsapi_error_t NetworkStack::gethostbyname_async_cancel(int id)
{
    return nsapi_dns_query_async_cancel(id);
}

nsapi_error_t NetworkStack::add_dns_server(const SocketAddress &address)
{
    return nsapi_dns_add_server(address);
}

nsapi_error_t NetworkStack::get_dns_server(int index, SocketAddress *address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkStack::setstackopt(int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkStack::getstackopt(int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkStack::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkStack::getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkStack::call_in(int delay, mbed::Callback<void()> func)
{
    static events::EventQueue *event_queue = mbed::mbed_event_queue();

    if (!event_queue) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    if (delay > 0) {
        if (event_queue->call_in(delay, func) == 0) {
            return NSAPI_ERROR_NO_MEMORY;
        }
    } else {
        if (event_queue->call(func) == 0) {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    return NSAPI_ERROR_OK;
}

call_in_callback_cb_t NetworkStack::get_call_in_callback()
{
    call_in_callback_cb_t cb(this, &NetworkStack::call_in);
    return cb;
}

// NetworkStackWrapper class for encapsulating the raw nsapi_stack structure
class NetworkStackWrapper : public NetworkStack
{
private:
    inline nsapi_stack_t *_stack()
    {
        return reinterpret_cast<nsapi_stack_t *>(
                reinterpret_cast<uint8_t *>(this)
                - offsetof(nsapi_stack_t, _stack_buffer));
    }

    inline const nsapi_stack_api_t *_stack_api()
    {
        return _stack()->stack_api;
    }

public:
    virtual const char *get_ip_address()
    {
        if (!_stack_api()->get_ip_address) {
            return 0;
        }

        static uint8_t buffer[sizeof(SocketAddress)];
        SocketAddress *address = new (buffer) SocketAddress(_stack_api()->get_ip_address(_stack()));
        return address->get_ip_address();
    }

    virtual nsapi_error_t gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
    {
        if (!_stack_api()->gethostbyname) {
            return NetworkStack::gethostbyname(name, address, version);
        }

        nsapi_addr_t addr = {NSAPI_UNSPEC, 0};
        nsapi_error_t err = _stack_api()->gethostbyname(_stack(), name, &addr, version);
        address->set_addr(addr);
        return err;
    }

    virtual nsapi_error_t add_dns_server(const SocketAddress &address)
    {
        if (!_stack_api()->add_dns_server) {
            return NetworkStack::add_dns_server(address);
        }

        return _stack_api()->add_dns_server(_stack(), address.get_addr());
    }

    virtual nsapi_error_t setstackopt(int level, int optname, const void *optval, unsigned optlen)
    {
        if (!_stack_api()->setstackopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->setstackopt(_stack(), level, optname, optval, optlen);
    }

    virtual nsapi_error_t getstackopt(int level, int optname, void *optval, unsigned *optlen)
    {
        if (!_stack_api()->getstackopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->getstackopt(_stack(), level, optname, optval, optlen);
    }

protected:
    virtual nsapi_error_t socket_open(nsapi_socket_t *socket, nsapi_protocol_t proto)
    {
        if (!_stack_api()->socket_open) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_open(_stack(), socket, proto);
    }

    virtual nsapi_error_t socket_close(nsapi_socket_t socket)
    {
        if (!_stack_api()->socket_close) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_close(_stack(), socket);
    }

    virtual nsapi_error_t socket_bind(nsapi_socket_t socket, const SocketAddress &address)
    {
        if (!_stack_api()->socket_bind) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_bind(_stack(), socket, address.get_addr(), address.get_port());
    }

    virtual nsapi_error_t socket_listen(nsapi_socket_t socket, int backlog)
    {
        if (!_stack_api()->socket_listen) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_listen(_stack(), socket, backlog);
    }

    virtual nsapi_error_t socket_connect(nsapi_socket_t socket, const SocketAddress &address)
    {
        if (!_stack_api()->socket_connect) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_connect(_stack(), socket, address.get_addr(), address.get_port());
    }

    virtual nsapi_error_t socket_accept(nsapi_socket_t server, nsapi_socket_t *socket, SocketAddress *address)
    {
        if (!_stack_api()->socket_accept) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        nsapi_addr_t addr = {NSAPI_IPv4, 0};
        uint16_t port = 0;

        nsapi_error_t err = _stack_api()->socket_accept(_stack(), server, socket, &addr, &port);

        if (address) {
            address->set_addr(addr);
            address->set_port(port);
        }

        return err;
    }

    virtual nsapi_size_or_error_t socket_send(nsapi_socket_t socket, const void *data, nsapi_size_t size)
    {
        if (!_stack_api()->socket_send) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_send(_stack(), socket, data, size);
    }

    virtual nsapi_size_or_error_t socket_recv(nsapi_socket_t socket, void *data, nsapi_size_t size)
    {
        if (!_stack_api()->socket_recv) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_recv(_stack(), socket, data, size);
    }

    virtual nsapi_size_or_error_t socket_sendto(nsapi_socket_t socket, const SocketAddress &address, const void *data, nsapi_size_t size)
    {
        if (!_stack_api()->socket_sendto) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_sendto(_stack(), socket, address.get_addr(), address.get_port(), data, size);
    }

    virtual nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t socket, SocketAddress *address, void *data, nsapi_size_t size)
    {
        if (!_stack_api()->socket_recvfrom) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        nsapi_addr_t addr = {NSAPI_IPv4, 0};
        uint16_t port = 0;

        nsapi_size_or_error_t err = _stack_api()->socket_recvfrom(_stack(), socket, &addr, &port, data, size);

        if (address) {
            address->set_addr(addr);
            address->set_port(port);
        }

        return err;
    }

    virtual void socket_attach(nsapi_socket_t socket, void (*callback)(void *), void *data)
    {
        if (!_stack_api()->socket_attach) {
            return;
        }

        return _stack_api()->socket_attach(_stack(), socket, callback, data);
    }

    virtual nsapi_error_t setsockopt(nsapi_socket_t socket, int level, int optname, const void *optval, unsigned optlen)
    {
        if (!_stack_api()->setsockopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->setsockopt(_stack(), socket, level, optname, optval, optlen);
    }

    virtual nsapi_error_t getsockopt(nsapi_socket_t socket, int level, int optname, void *optval, unsigned *optlen)
    {
        if (!_stack_api()->getsockopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->getsockopt(_stack(), socket, level, optname, optval, optlen);
    }
};


// Conversion function for network stacks
NetworkStack *nsapi_create_stack(nsapi_stack_t *stack)
{
    MBED_STATIC_ASSERT(sizeof stack->_stack_buffer >= sizeof(NetworkStackWrapper),
            "The nsapi_stack_t stack buffer must fit a NetworkStackWrapper");
    return new (stack->_stack_buffer) NetworkStackWrapper;
}

NetworkStack *nsapi_create_stack(NetworkStack *stack)
{
    return stack;
}

