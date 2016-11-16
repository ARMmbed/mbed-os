/* Nanostack implementation of NetworkSocketAPI
 * Copyright (c) 2016 ARM Limited
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

#include "mbed.h"
#include "rtos.h"
#include "NanostackInterface.h"

#include "ns_address.h"
#include "nsdynmemLIB.h"
#include "eventOS_scheduler.h"
#include "randLIB.h"

#include "mesh_system.h" // from inside mbed-mesh-api
#include "socket_api.h"
#include "net_interface.h"

// Uncomment to enable trace
//#define HAVE_DEBUG
#include "ns_trace.h"
#define TRACE_GROUP "nsif"

#define NS_INTERFACE_SOCKETS_MAX  16  //same as NanoStack SOCKET_MAX
#define NANOSTACK_SOCKET_UDP 17 // same as nanostack SOCKET_UDP
#define NANOSTACK_SOCKET_TCP 6  // same as nanostack SOCKET_TCP

#define MALLOC  ns_dyn_mem_alloc
#define FREE    ns_dyn_mem_free

enum socket_mode_t {
    SOCKET_MODE_UNOPENED,   // No socket ID
    SOCKET_MODE_OPENED,     // Socket ID but no assigned use yet
    SOCKET_MODE_CONNECTING, // Socket is connecting but not open yet
    SOCKET_MODE_DATAGRAM,   // Socket is bound to a port and listening for datagrams
    SOCKET_MODE_STREAM,     // Socket has an open stream
    SOCKET_MODE_CLOSED,     // Socket is closed and resources are freed
};

class NanostackBuffer {
public:
    NanostackBuffer *next;      /*<! next buffer */
    ns_address_t ns_address;    /*<! address where data is received */
    uint16_t length;            /*<! data length in this buffer */
    uint8_t payload[1];          /*<! Trailing buffer data */
};

class NanostackSocket {
public:
    static void socket_callback(void *cb);
    static void* operator new(std::size_t sz);
    static void operator delete(void* ptr);

    NanostackSocket(int8_t protocol);
    ~NanostackSocket(void);
    bool open(void);
    void close(void);
    bool closed(void) {return SOCKET_MODE_CLOSED == mode;}
    bool is_bound(void);
    void set_bound(void);
    bool is_connecting(void);
    void set_connecting(ns_address_t *addr);
    void set_connected(void);

    // Socket events from nanostack
    void event_data(socket_callback_t *sock_cb);
    void event_bind_done(socket_callback_t *sock_cb);
    void event_connnect_closed(socket_callback_t *sock_cb);
    void event_tx_done(socket_callback_t *sock_cb);

    // Run callback to signal the next layer of the NSAPI
    void signal_event(void);

    // Add or remove a socket to the listening socket
    void accept_list_add(NanostackSocket *socket);
    NanostackSocket * accept_list_remove(void);

    bool data_available(void);
    size_t data_copy_and_free(void *dest, size_t len, SocketAddress *address, bool stream);
    void data_free_all(void);
    void data_attach(NanostackBuffer *data_buf);

    void (*callback)(void *);
    void *callback_data;
    int8_t socket_id;           /*!< allocated socket ID */
    int8_t proto;               /*!< UDP or TCP */
    bool addr_valid;
    ns_address_t ns_address;
private:
    NanostackBuffer *rxBufChain;    /*!< Receive buffers */
    socket_mode_t mode;
};

static NanostackSocket * socket_tbl[NS_INTERFACE_SOCKETS_MAX];

nsapi_error_t map_mesh_error(mesh_error_t err)
{
    switch (err) {
        case MESH_ERROR_NONE: return 0;
        case MESH_ERROR_MEMORY: return NSAPI_ERROR_NO_MEMORY;
        case MESH_ERROR_PARAM: return NSAPI_ERROR_UNSUPPORTED;
        case MESH_ERROR_STATE: return NSAPI_ERROR_DEVICE_ERROR;
        default: return NSAPI_ERROR_DEVICE_ERROR;
    }
}

static void convert_mbed_addr_to_ns(ns_address_t *ns_addr,
                             const SocketAddress *s_addr)
{
    ns_addr->type = ADDRESS_IPV6;
    ns_addr->identifier = s_addr->get_port();
    memcpy(ns_addr->address, s_addr->get_ip_bytes(), 16);
}

static void convert_ns_addr_to_mbed(SocketAddress *s_addr, const ns_address_t *ns_addr)
{
    s_addr->set_port(ns_addr->identifier);
    s_addr->set_ip_bytes(ns_addr->address, NSAPI_IPv6);
}

void* NanostackSocket::operator new(std::size_t sz) {
    return MALLOC(sz);
}
void NanostackSocket::operator delete(void* ptr) {
    FREE(ptr);
}

NanostackSocket::NanostackSocket(int8_t protocol)
{
    nanostack_assert_locked();

    callback = NULL;
    callback_data = NULL;
    socket_id = -1;
    rxBufChain = NULL;
    proto = protocol;
    addr_valid = false;
    memset(&ns_address, 0, sizeof(ns_address));
    mode = SOCKET_MODE_UNOPENED;
}

NanostackSocket::~NanostackSocket()
{
    nanostack_assert_locked();

    if (mode != SOCKET_MODE_CLOSED) {
        close();
    }
    data_free_all();
}

bool NanostackSocket::open(void)
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_UNOPENED == mode);

    int temp_socket = socket_open(proto, 0, socket_callback);

    if (temp_socket < 0) {
        tr_error("NanostackSocket::open() failed");
        return false;
    }
    if (temp_socket >= NS_INTERFACE_SOCKETS_MAX) {
        MBED_ASSERT(false);
        return false;
    }
    if (socket_tbl[temp_socket] != NULL) {
        MBED_ASSERT(false);
        return false;
    }
    socket_id = temp_socket;
    socket_tbl[socket_id] = this;
    mode = SOCKET_MODE_OPENED;
    return true;

}

void NanostackSocket::close()
{
    nanostack_assert_locked();
    MBED_ASSERT(mode != SOCKET_MODE_CLOSED);

    if (socket_id >= 0) {
        nsapi_error_t ret = socket_close(socket_id);
        MBED_ASSERT(0 == ret);
        MBED_ASSERT(socket_tbl[socket_id] == this);
        socket_tbl[socket_id] = NULL;
        socket_id = -1;
    } else {
        MBED_ASSERT(SOCKET_MODE_UNOPENED == mode);
    }

    data_free_all();

    mode = SOCKET_MODE_CLOSED;
    signal_event();
}

bool NanostackSocket::is_bound()
{
    return SOCKET_MODE_DATAGRAM == mode;
}

void NanostackSocket::set_bound()
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_OPENED == mode);
    if (SOCKET_UDP == proto) {
        mode = SOCKET_MODE_DATAGRAM;
    }
}

bool NanostackSocket::is_connecting()
{
    return SOCKET_MODE_CONNECTING == mode;
}

void NanostackSocket::set_connecting(ns_address_t *addr)
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_OPENED == mode);

    memcpy(&ns_address, addr, sizeof(ns_address_t));
    mode = SOCKET_MODE_CONNECTING;
}

void NanostackSocket::set_connected()
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_CONNECTING == mode);

    mode = SOCKET_MODE_STREAM;
}

void NanostackSocket::signal_event()
{
    nanostack_assert_locked();

    if (callback != NULL) {
        callback(callback_data);
    }
}

void NanostackSocket::socket_callback(void *cb) {
    nanostack_assert_locked();

    socket_callback_t *sock_cb = (socket_callback_t *) cb;
    NanostackSocket *socket = socket_tbl[sock_cb->socket_id];
    MBED_ASSERT(socket != NULL);

    tr_debug("socket_callback() sock=%d, event=%d, interface=%d, data len=%d",
                     sock_cb->socket_id, sock_cb->event_type, sock_cb->interface_id, sock_cb->d_len);

    switch (sock_cb->event_type) {
        case SOCKET_DATA:
            tr_debug("SOCKET_DATA, sock=%d, bytes=%d", sock_cb->socket_id, sock_cb->d_len);
            socket->event_data(sock_cb);
            break;
        case SOCKET_BIND_DONE:
            tr_debug("SOCKET_BIND_DONE");
            socket->event_bind_done(sock_cb);
            break;
        case SOCKET_BIND_FAIL:
            tr_debug("SOCKET_BIND_FAIL");
            break;
        case SOCKET_BIND_AUTH_FAIL:
            tr_debug("SOCKET_BIND_AUTH_FAIL");
            break;
        case SOCKET_INCOMING_CONNECTION:
            tr_debug("SOCKET_INCOMING_CONNECTION");
            break;
        case SOCKET_TX_FAIL:
            tr_debug("SOCKET_TX_FAIL");
            break;
        case SOCKET_CONNECT_CLOSED:
            tr_debug("SOCKET_CONNECT_CLOSED");
            socket->event_connnect_closed(sock_cb);
            break;
        case SOCKET_CONNECTION_RESET:
            tr_debug("SOCKET_CONNECTION_RESET");
            break;
        case SOCKET_NO_ROUTE:
            tr_debug("SOCKET_NO_ROUTE");
            break;
        case SOCKET_TX_DONE:
            tr_debug("SOCKET_TX_DONE, %d bytes sent", sock_cb->d_len);
            socket->event_tx_done(sock_cb);
            break;
        default:
            // SOCKET_NO_RAM, error case for SOCKET_TX_DONE
            break;
    }
}


bool NanostackSocket::data_available()
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_DATAGRAM == mode) ||
                (SOCKET_MODE_CONNECTING == mode) ||
                (SOCKET_MODE_STREAM == mode));

    return (NULL == rxBufChain) ? false : true;
}

size_t NanostackSocket::data_copy_and_free(void *dest, size_t len,
                                                  SocketAddress *address, bool stream)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_DATAGRAM == mode) ||
                (mode == SOCKET_MODE_STREAM));

    NanostackBuffer *data_buf = rxBufChain;
    if (NULL == data_buf) {
        // No data
        return 0;
    }

    if (address) {
        convert_ns_addr_to_mbed(address, &data_buf->ns_address);
    }

    size_t copy_size = (len > data_buf->length) ? data_buf->length : len;
    memcpy(dest, data_buf->payload, copy_size);

    if (stream && (copy_size < data_buf->length)) {
        // Update the size in the buffer
        size_t new_buf_size = data_buf->length - copy_size;
        memmove(data_buf->payload, data_buf->payload + copy_size, new_buf_size);
        data_buf->length = new_buf_size;
    } else {
        // Entire packet used so free it
        rxBufChain = data_buf->next;
        FREE(data_buf);
    }

    return copy_size;
}

void NanostackSocket::data_free_all(void)
{
    nanostack_assert_locked();
    // No mode requirement

    NanostackBuffer *buffer = rxBufChain;
    rxBufChain = NULL;
    while (buffer != NULL) {
        NanostackBuffer *next_buffer = buffer->next;
        FREE(buffer);
        buffer = next_buffer;
    }
}

void NanostackSocket::data_attach(NanostackBuffer *data_buf)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_DATAGRAM == mode) ||
                (SOCKET_MODE_STREAM == mode));

    // Add to linked list
    tr_debug("data_attach socket=%p", this);
    if (NULL == rxBufChain) {
        rxBufChain = data_buf;
    } else {
        NanostackBuffer *buf_tmp = rxBufChain;
        while (NULL != buf_tmp->next) {
            buf_tmp = buf_tmp->next;
        }
        buf_tmp->next = data_buf;
    }
    signal_event();
}

void NanostackSocket::event_data(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_DATAGRAM == mode) ||
                (SOCKET_MODE_STREAM == mode));

    // Allocate buffer
    NanostackBuffer *recv_buff = (NanostackBuffer *) MALLOC(
                                 sizeof(NanostackBuffer) + sock_cb->d_len);
    if (NULL == recv_buff) {
        tr_error("alloc failed!");
        return;
    }
    recv_buff->next = NULL;

    // Write data to buffer
    int16_t length = socket_read(sock_cb->socket_id,
                                 &recv_buff->ns_address, recv_buff->payload,
                                 sock_cb->d_len);
    if (length < 0) {
        tr_error("socket_read failed!");
        FREE(recv_buff);
        return;
    }
    recv_buff->length = length;

    data_attach(recv_buff);
}

void NanostackSocket::event_tx_done(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_STREAM == mode) ||
                (SOCKET_MODE_DATAGRAM == mode));

    signal_event();
}

void NanostackSocket::event_bind_done(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_CONNECTING == mode);

    set_connected();
    signal_event();
}

void NanostackSocket::event_connnect_closed(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();

    // Only TCP sockets can be closed by the remote end
    MBED_ASSERT((SOCKET_MODE_STREAM == mode) ||
                (SOCKET_MODE_CONNECTING == mode));
    close();
}

NanostackInterface * NanostackInterface::_ns_interface;

NanostackInterface * NanostackInterface::get_stack()
{
    nanostack_lock();

    if (NULL == _ns_interface) {
        _ns_interface = new NanostackInterface();
    }

    nanostack_unlock();

    return _ns_interface;
}


const char * NanostackInterface::get_ip_address()
{
    // Must result a valid IPv6 address
    // For gethostbyname() to detect IP version.
    static const char localhost[] = "::";
    return localhost;
}

nsapi_error_t NanostackInterface::socket_open(void **handle, nsapi_protocol_t protocol)
{
    // Validate parameters
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }
    int8_t ns_proto;
    if (NSAPI_UDP == protocol) {
        ns_proto = SOCKET_UDP;
    } else if (NSAPI_TCP == protocol) {
        ns_proto = SOCKET_TCP;
    } else {
        MBED_ASSERT(false);
        return NSAPI_ERROR_UNSUPPORTED;
    }
    *handle = (void*)NULL;

    nanostack_lock();

    NanostackSocket * socket = new NanostackSocket(ns_proto);
    if (NULL == socket) {
        nanostack_unlock();
        tr_debug("socket_open() ret=%i", NSAPI_ERROR_NO_MEMORY);
        return NSAPI_ERROR_NO_MEMORY;
    }
    if (!socket->open()) {
        delete socket;
        nanostack_unlock();
        tr_debug("socket_open() ret=%i", NSAPI_ERROR_DEVICE_ERROR);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    *handle = (void*)socket;

    nanostack_unlock();

    tr_debug("socket_open() socket=%p, sock_id=%d, ret=0", socket, socket->socket_id);

    return 0;
}

nsapi_error_t NanostackInterface::socket_close(void *handle)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }
    tr_debug("socket_close(socket=%p) sock_id=%d", socket, socket->socket_id);

    nanostack_lock();

    delete socket;

    nanostack_unlock();

    return 0;

}

nsapi_size_or_error_t NanostackInterface::socket_sendto(void *handle, const SocketAddress &address, const void *data, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (address.get_ip_version() != NSAPI_IPv6) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    nanostack_lock();

    nsapi_size_or_error_t ret;
    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
    } else if (NANOSTACK_SOCKET_TCP == socket->proto) {
        tr_error("socket_sendto() not supported with SOCKET_STREAM!");
        ret = NSAPI_ERROR_UNSUPPORTED;
    } else {
        ns_address_t ns_address;
        convert_mbed_addr_to_ns(&ns_address, &address);
        if (!socket->is_bound()) {
            socket->set_bound();
        }
        int8_t send_to_status = ::socket_sendto(socket->socket_id, &ns_address,
                                       (uint8_t *)data, size);
        /*
         * \return 0 on success.
         * \return -1 invalid socket id.
         * \return -2 Socket memory allocation fail.
         * \return -3 TCP state not established.
         * \return -4 Socket tx process busy.
         * \return -5 TLS authentication not ready.
         * \return -6 Packet too short.
         * */
        if (-4 == send_to_status) {
            ret = NSAPI_ERROR_WOULD_BLOCK;
        } else if (0 != send_to_status) {
            tr_error("socket_sendto: error=%d", send_to_status);
            ret = NSAPI_ERROR_DEVICE_ERROR;
        } else {
            ret = size;
        }
    }

    nanostack_unlock();

    tr_debug("socket_sendto(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_size_or_error_t NanostackInterface::socket_recvfrom(void *handle, SocketAddress *address, void *buffer, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (NULL == buffer) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_PARAMETER;
    }
    if (0 == size) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_PARAMETER;
    }

    nanostack_lock();

    nsapi_size_or_error_t ret;
    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
    } else if (NANOSTACK_SOCKET_TCP == socket->proto) {
        tr_error("recv_from() not supported with SOCKET_STREAM!");
        ret = NSAPI_ERROR_UNSUPPORTED;
    } else if (!socket->data_available()) {
        ret = NSAPI_ERROR_WOULD_BLOCK;
    } else {
        ret = socket->data_copy_and_free(buffer, size, address, false);
    }

    nanostack_unlock();

    tr_debug("socket_recvfrom(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_error_t NanostackInterface::socket_bind(void *handle, const SocketAddress &address)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    const void *addr_field;
    switch (address.get_ip_version()) {
        case NSAPI_IPv6:
            addr_field = address.get_ip_bytes();
            break;
        case NSAPI_UNSPEC:
            addr_field = &ns_in6addr_any;
            break;
        default:
            return NSAPI_ERROR_UNSUPPORTED;
    }

    nanostack_lock();

    ns_address_t ns_address;
    ns_address.type = ADDRESS_IPV6;
    memcpy(ns_address.address, addr_field, sizeof ns_address.address);
    ns_address.identifier = address.get_port();
    nsapi_error_t ret = NSAPI_ERROR_DEVICE_ERROR;
    if (0 == ::socket_bind(socket->socket_id, &ns_address)) {
        socket->set_bound();
        ret = 0;
    }

    nanostack_unlock();

    tr_debug("socket_bind(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_error_t NanostackInterface::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NanostackInterface::getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NanostackInterface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NanostackInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (addr.get_ip_version() != NSAPI_IPv6) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    nanostack_lock();

    nsapi_error_t ret;
    ns_address_t ns_addr;
    int random_port = socket->is_bound() ? 0 : 1;
    convert_mbed_addr_to_ns(&ns_addr, &addr);
    if (0 == ::socket_connect(socket->socket_id, &ns_addr, random_port)) {
        socket->set_connecting(&ns_addr);
        ret = 0;
    } else {
        ret = NSAPI_ERROR_DEVICE_ERROR;
    }

    nanostack_unlock();

    tr_debug("socket_connect(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_error_t NanostackInterface::socket_accept(void *server, void **handle, SocketAddress *address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_size_or_error_t NanostackInterface::socket_send(void *handle, const void *p, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    nanostack_lock();

    nsapi_size_or_error_t ret;
    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
    } else if (socket->is_connecting()) {
        ret = NSAPI_ERROR_WOULD_BLOCK;
    } else {
        ret = ::socket_sendto(socket->socket_id,  &socket->ns_address, (uint8_t*)p, size);
        /*
         * \return 0 on success.
         * \return -1 invalid socket id.
         * \return -2 Socket memory allocation fail.
         * \return -3 TCP state not established.
         * \return -4 Socket tx process busy.
         * \return -5 TLS authentication not ready.
         * \return -6 Packet too short.
         * */
        if (-4 == ret) {
            ret = NSAPI_ERROR_WOULD_BLOCK;
        } else if (ret != 0) {
            tr_warning("socket_sendto ret %i, socket_id %i", ret, socket->socket_id);
            ret = NSAPI_ERROR_DEVICE_ERROR;
        } else {
            ret = size;
        }
    }

    nanostack_unlock();

    tr_debug("socket_send(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_size_or_error_t NanostackInterface::socket_recv(void *handle, void *data, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    nanostack_lock();

    nsapi_size_or_error_t ret;
    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
    } else if (socket->data_available()) {
        ret = socket->data_copy_and_free(data, size, NULL, true);
    } else {
        ret = NSAPI_ERROR_WOULD_BLOCK;
    }

    nanostack_unlock();

    tr_debug("socket_recv(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

void NanostackInterface::socket_attach(void *handle, void (*callback)(void *), void *id)
{
    // Validate parameters
    NanostackSocket * socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return;
    }

    nanostack_lock();

    socket->callback = callback;
    socket->callback_data = id;

    nanostack_unlock();

    tr_debug("socket_attach(socket=%p) sock_id=%d", socket, socket->socket_id);
}
