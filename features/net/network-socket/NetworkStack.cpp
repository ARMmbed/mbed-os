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
#include "DnsQuery.h"
#include "mbed.h"
#include "stddef.h"
#include <new>


// Default NetworkStack operations
int NetworkStack::gethostbyname(SocketAddress *address, const char *name)
{
    char buffer[NSAPI_IP_SIZE];
    int err = dnsQuery(this, name, buffer);
    if (err) {
        return err;
    }

    address->set_ip_address(buffer);
    return 0;
}

int NetworkStack::setstackopt(int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int NetworkStack::getstackopt(int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int NetworkStack::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int NetworkStack::getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
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

    virtual int gethostbyname(SocketAddress *address, const char *name)
    {
        if (!_stack_api()->gethostbyname) {
            return NetworkStack::gethostbyname(address, name);
        }

        nsapi_addr_t addr = {NSAPI_IPv4, 0};
        int err = _stack_api()->gethostbyname(_stack(), &addr, name);
        address->set_addr(addr);
        return err;
    }

    virtual int setstackopt(int level, int optname, const void *optval, unsigned optlen)
    {
        if (!_stack_api()->setstackopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->setstackopt(_stack(), level, optname, optval, optlen);
    }

    virtual int getstackopt(int level, int optname, void *optval, unsigned *optlen)
    {
        if (!_stack_api()->getstackopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->getstackopt(_stack(), level, optname, optval, optlen);
    }

protected:
    virtual int socket_open(nsapi_socket_t *socket, nsapi_protocol_t proto)
    {
        if (!_stack_api()->socket_open) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_open(_stack(), socket, proto);
    }

    virtual int socket_close(nsapi_socket_t socket)
    {
        if (!_stack_api()->socket_close) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_close(_stack(), socket);
    }

    virtual int socket_bind(nsapi_socket_t socket, const SocketAddress &address)
    {
        if (!_stack_api()->socket_bind) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_bind(_stack(), socket, address.get_addr(), address.get_port());
    }

    virtual int socket_listen(nsapi_socket_t socket, int backlog)
    {
        if (!_stack_api()->socket_listen) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_listen(_stack(), socket, backlog);
    }

    virtual int socket_connect(nsapi_socket_t socket, const SocketAddress &address)
    {
        if (!_stack_api()->socket_connect) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_connect(_stack(), socket, address.get_addr(), address.get_port());
    }

    virtual int socket_accept(nsapi_socket_t *socket, nsapi_socket_t server)
    {
        if (!_stack_api()->socket_accept) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_accept(_stack(), socket, server);
    }

    virtual int socket_send(nsapi_socket_t socket, const void *data, unsigned size)
    {
        if (!_stack_api()->socket_send) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_send(_stack(), socket, data, size);
    }

    virtual int socket_recv(nsapi_socket_t socket, void *data, unsigned size)
    {
        if (!_stack_api()->socket_recv) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_recv(_stack(), socket, data, size);
    }

    virtual int socket_sendto(nsapi_socket_t socket, const SocketAddress &address, const void *data, unsigned size)
    {
        if (!_stack_api()->socket_sendto) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->socket_sendto(_stack(), socket, address.get_addr(), address.get_port(), data, size);
    }

    virtual int socket_recvfrom(nsapi_socket_t socket, SocketAddress *address, void *data, unsigned size)
    {
        if (!_stack_api()->socket_recvfrom) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        nsapi_addr_t addr = {NSAPI_IPv4, 0};
        uint16_t port = 0;

        int err = _stack_api()->socket_recvfrom(_stack(), socket, &addr, &port, data, size);

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

    virtual int setsockopt(nsapi_socket_t socket, int level, int optname, const void *optval, unsigned optlen)
    {
        if (!_stack_api()->setsockopt) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return _stack_api()->setsockopt(_stack(), socket, level, optname, optval, optlen);
    }

    virtual int getsockopt(nsapi_socket_t socket, int level, int optname, void *optval, unsigned *optlen)
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
    MBED_ASSERT(sizeof stack->_stack_buffer >= sizeof(NetworkStackWrapper));
    return new (stack->_stack_buffer) NetworkStackWrapper;
}

NetworkStack *nsapi_create_stack(NetworkStack *stack)
{
    return stack;
}

