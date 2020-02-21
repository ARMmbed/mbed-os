/* InternetSocket
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

#include "InternetSocket.h"
#include "platform/mbed_critical.h"
#include "platform/Callback.h"

using namespace mbed;

InternetSocket::InternetSocket()
    : _stack(0), _socket(0), _timeout(osWaitForever),
      _remote_peer(),
      _readers(0), _writers(0),
      _factory_allocated(false)
{
    core_util_atomic_flag_clear(&_pending);
    _socket_stats.stats_new_socket_entry(this);
}

InternetSocket::~InternetSocket()
{
    close();
}

nsapi_error_t InternetSocket::open(NetworkStack *stack)
{
    _lock.lock();

    if (_stack != NULL || stack == NULL) {
        _lock.unlock();
        return NSAPI_ERROR_PARAMETER;
    }
    _stack = stack;

    nsapi_socket_t socket;
    nsapi_error_t err = _stack->socket_open(&socket, get_proto());
    if (err) {
        _lock.unlock();
        return err;
    }

    _socket_stats.stats_update_socket_state(this, SOCK_OPEN);
    _socket = socket;
    _event = callback(this, &InternetSocket::event);
    _stack->socket_attach(_socket, Callback<void()>::thunk, &_event);
    _interface_name[0] = '\0';
    _lock.unlock();
    return NSAPI_ERROR_OK;
}

nsapi_error_t InternetSocket::close()
{
    _lock.lock();

    nsapi_error_t ret = NSAPI_ERROR_OK;
    if (!_socket)  {
        _lock.unlock();
        return NSAPI_ERROR_NO_SOCKET;
    }

    // Just in case - tell the stack not to callback any more, then remove this socket.
    _stack->socket_attach(_socket, 0, 0);
    nsapi_socket_t socket = _socket;
    _socket = 0;
    ret = _stack->socket_close(socket);
    _stack = 0; // Invalidate the stack pointer - otherwise open() fails.
    _socket_stats.stats_update_socket_state(this, SOCK_CLOSED);
    // Wakeup anything in a blocking operation
    // on this socket
    event();

    // Wait until all readers and writers are gone
    while (_readers || _writers) {
        _lock.unlock();
        _event_flag.wait_any(FINISHED_FLAG, osWaitForever);
        _lock.lock();
    }
    _interface_name[0] = '\0';
    _lock.unlock();

    // When allocated by accept() call, will destroy itself on close();
    if (_factory_allocated) {
        delete this;
    }
    return ret;
}

int InternetSocket::modify_multicast_group(const SocketAddress &address, nsapi_socket_option_t socketopt)
{
    nsapi_ip_mreq_t mreq;

    // Set up group address
    mreq.imr_multiaddr = address.get_addr();
    mreq.imr_interface = nsapi_addr_t();   // Default address, NSAPI_UNSPEC

    return this->setsockopt(NSAPI_SOCKET, socketopt, &mreq, sizeof(mreq));
}

int InternetSocket::join_multicast_group(const SocketAddress &address)
{
    return modify_multicast_group(address, NSAPI_ADD_MEMBERSHIP);
}

int InternetSocket::leave_multicast_group(const SocketAddress &address)
{
    return modify_multicast_group(address, NSAPI_DROP_MEMBERSHIP);
}


nsapi_error_t InternetSocket::bind(uint16_t port)
{
    // Underlying bind is thread safe
    SocketAddress addr(0, port);
    return bind(addr);
}

nsapi_error_t InternetSocket::bind(const SocketAddress &address)
{
    _lock.lock();
    nsapi_error_t ret;

    if (!_socket) {
        ret = NSAPI_ERROR_NO_SOCKET;
    } else {
        ret = _stack->socket_bind(_socket, address);
    }

    _lock.unlock();
    return ret;
}

void InternetSocket::set_blocking(bool blocking)
{
    // InternetSocket::set_timeout is thread safe
    set_timeout(blocking ? -1 : 0);
}

void InternetSocket::set_timeout(int timeout)
{
    _lock.lock();

    if (timeout >= 0) {
        _timeout = (uint32_t)timeout;
    } else {
        _timeout = osWaitForever;
    }

    _lock.unlock();
}

nsapi_error_t InternetSocket::setsockopt(int level, int optname, const void *optval, unsigned optlen)
{
    _lock.lock();
    nsapi_error_t ret;

    if (!_socket) {
        ret = NSAPI_ERROR_NO_SOCKET;
    } else {
        ret = _stack->setsockopt(_socket, level, optname, optval, optlen);
        if (optname == NSAPI_BIND_TO_DEVICE && level == NSAPI_SOCKET) {
            strncpy(_interface_name, static_cast<const char *>(optval), optlen);
        }
    }

    _lock.unlock();
    return ret;
}

nsapi_error_t InternetSocket::getsockopt(int level, int optname, void *optval, unsigned *optlen)
{
    _lock.lock();
    nsapi_error_t ret;

    if (!_socket) {
        ret = NSAPI_ERROR_NO_SOCKET;
    } else {
        ret = _stack->getsockopt(_socket, level, optname, optval, optlen);
    }

    _lock.unlock();
    return ret;

}
void InternetSocket::event()
{
    _event_flag.set(READ_FLAG | WRITE_FLAG);

    if (_callback && !core_util_atomic_flag_test_and_set(&_pending)) {
        _callback();
    }
}

void InternetSocket::sigio(Callback<void()> callback)
{
    core_util_critical_section_enter();
    _callback = callback;
    core_util_critical_section_exit();
}

void InternetSocket::attach(Callback<void()> callback)
{
    sigio(callback);
}

nsapi_error_t InternetSocket::getpeername(SocketAddress *address)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (!_remote_peer) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    *address = _remote_peer;
    return NSAPI_ERROR_OK;
}
