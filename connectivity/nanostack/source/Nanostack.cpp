/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

/* Nanostack implementation of NetworkSocketAPI */

#include "Nanostack.h"
#include "NanostackLockGuard.h"

#include "ns_address.h"
#include "nsdynmemLIB.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "ip6string.h"

#include "mesh_system.h" // from inside mbed-mesh-api
#include "socket_api.h"
#include "net_interface.h"
#include "nsapi_dns.h"

// Uncomment to enable trace
//#define HAVE_DEBUG
#include "ns_trace.h"
#define TRACE_GROUP "nsif"

//#define NSIF_DEEP_TRACE
#ifdef NSIF_DEEP_TRACE
#define TRACE_DEEP    tr_debug
#else
#define TRACE_DEEP(...)
#endif

#define NANOSTACK_ISDIGIT(c)    ((c) >= '0' && (c) <= '9')

#define NS_INTERFACE_SOCKETS_MAX  16  //same as NanoStack SOCKET_MAX

#define MALLOC  ns_dyn_mem_alloc
#define FREE    ns_dyn_mem_free

// Socket state progressions:
// UDP: UNOPENED -> DATAGRAM
// TCP client: UNOPENED -> OPENED -> CONNECTING -> STREAM -> CLOSED
// TCP server: UNOPENED -> OPENED -> LISTENING
// TCP accept: UNOPENED -> STREAM -> CLOSED
enum socket_mode_t {
    SOCKET_MODE_UNOPENED,   // No socket ID
    SOCKET_MODE_DATAGRAM,   // Socket is datagram type
    SOCKET_MODE_OPENED,     // Socket ID but no assigned use yet
    SOCKET_MODE_CONNECTING, // Socket is connecting but not open yet
    SOCKET_MODE_STREAM,     // Socket has an open stream
    SOCKET_MODE_CLOSED,     // Socket is closed and resources are freed
    SOCKET_MODE_LISTENING,  // Socket is listening for connections
};

#define CALL_EVENT   0x12

class NanostackSocket {
public:
    static void socket_callback(void *cb);
    static void *operator new (std::size_t sz, const std::nothrow_t &nothrow_value) noexcept;
    static void operator delete (void *ptr);

    NanostackSocket(int8_t protocol);
    ~NanostackSocket(void);
    bool open(void);
    int accept(NanostackSocket *accepted_socket, ns_address_t *addr);
    void close(void);
    bool closed(void)
    {
        return SOCKET_MODE_CLOSED == mode;
    }
    bool is_connecting(void);
    void set_connecting(ns_address_t *addr);
    bool is_connected(void);
    void set_connected(void);
    bool is_listening(void);
    void set_listening(void);

    // Socket events from nanostack
    void event_data(socket_callback_t *sock_cb);
    void event_connect_done(socket_callback_t *sock_cb);
    void event_connect_fail(socket_callback_t *sock_cb);
    void event_connect_closed(socket_callback_t *sock_cb);
    void event_connection_reset(socket_callback_t *sock_cb);
    void event_tx_done(socket_callback_t *sock_cb);
    void event_tx_fail(socket_callback_t *sock_cb);
    void event_incoming_connection(socket_callback_t *sock_cb);

    // Run callback to signal the next layer of the NSAPI
    void signal_event(void);

    void (*callback)(void *);
    void *callback_data;
    int8_t socket_id;           /*!< allocated socket ID */
    int8_t proto;               /*!< UDP or TCP */
    bool addr_valid;
    ns_address_t ns_address;
private:
    bool attach(int8_t socket_id);
    socket_mode_t mode;
};

static NanostackSocket *socket_tbl[NS_INTERFACE_SOCKETS_MAX];

nsapi_error_t map_mesh_error(mesh_error_t err)
{
    switch (err) {
        case MESH_ERROR_NONE:
            return 0;
        case MESH_ERROR_MEMORY:
            return NSAPI_ERROR_NO_MEMORY;
        case MESH_ERROR_PARAM:
            return NSAPI_ERROR_PARAMETER;
        case MESH_ERROR_STATE:
            return NSAPI_ERROR_IS_CONNECTED;
        default:
            return NSAPI_ERROR_DEVICE_ERROR;
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

static int8_t find_interface_by_address(const uint8_t target_addr[16])
{
    for (int if_id = 1; if_id <= 127; if_id++) {
        int i = 0;
        uint8_t if_addr[16];
        while (arm_net_address_list_get_next(if_id, &i, if_addr) == 0) {
            if (memcmp(target_addr, if_addr, 16) == 0) {
                return if_id;
            }
        }
    }
    return -1;
}

static int8_t nanostack_interface_id_parse(const char *interface_name)
{
    int namelen;
    int8_t interface_id = -1;

    TRACE_DEEP("nanostack_interface_id_parse() %s", interface_name ? interface_name : "null");

    if (!interface_name) {
        return -1;
    }

    // parse interface ID from the interface_name
    namelen = strlen(interface_name);
    if (namelen < 4 || namelen > 5) {
        return -1;
    }

    if ((strncmp("MES", interface_name, 3) == 0) && NANOSTACK_ISDIGIT(interface_name[3])) {
        interface_id = atoi(&interface_name[3]);
    }

    TRACE_DEEP("parsed interfaceID = %d", interface_id);
    return interface_id;
}

static int nanostack_dns_query_result_check(const char *domain_name, SocketAddress *address, const char *interface_name)
{
    uint8_t dns_query_addr[16] = {0};
    int8_t interface_id, ns_query_result;

    interface_id = nanostack_interface_id_parse(interface_name);

    ns_query_result = arm_net_dns_query_result_get(interface_id, dns_query_addr, (char *)domain_name);

    TRACE_DEEP("nanostack_dns_query_result_check(): interface_id=%d, ret=%d, resolved %s to %s",
               interface_id, ns_query_result, domain_name, trace_ipv6(dns_query_addr));

    if (ns_query_result == 0) {
        address->set_ip_bytes(dns_query_addr, NSAPI_IPv6);
        return 0;
    }

    return -1;
}

void *NanostackSocket::operator new (std::size_t sz, const std::nothrow_t &nothrow_value) noexcept
{
    return MALLOC(sz);
}

void NanostackSocket::operator delete (void *ptr)
{
    FREE(ptr);
}

NanostackSocket::NanostackSocket(int8_t protocol)
{
    nanostack_assert_locked();

    callback = NULL;
    callback_data = NULL;
    socket_id = -1;
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

    if (proto == SOCKET_TCP) {
        /* Receive and send buffers enabled by default */
        mode = SOCKET_MODE_OPENED;
    } else {
        static const int32_t rcvbuf_size = 2048;
        socket_setsockopt(temp_socket, SOCKET_SOL_SOCKET, SOCKET_SO_RCVBUF, &rcvbuf_size, sizeof rcvbuf_size);
        mode = SOCKET_MODE_DATAGRAM;
    }

    return attach(temp_socket);
}

int NanostackSocket::accept(NanostackSocket *accepted_socket, ns_address_t *addr)
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_LISTENING == mode && SOCKET_MODE_UNOPENED == accepted_socket->mode);

    int temp_socket = socket_accept(socket_id, addr, socket_callback);
    if (temp_socket < 0) {
        tr_error("NanostackSocket::accept() failed");
        return temp_socket;
    }
    if (!accepted_socket->attach(temp_socket)) {
        return -1;
    }
    accepted_socket->mode = SOCKET_MODE_STREAM;
    return temp_socket;
}

bool NanostackSocket::attach(int8_t temp_socket)
{
    nanostack_assert_locked();
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

    mode = SOCKET_MODE_CLOSED;
    signal_event();
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

bool NanostackSocket::is_connected()
{
    return SOCKET_MODE_STREAM == mode;
}

void NanostackSocket::set_connected()
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_CONNECTING == mode);

    mode = SOCKET_MODE_STREAM;
}

bool NanostackSocket::is_listening()
{
    return SOCKET_MODE_LISTENING == mode;
}

void NanostackSocket::set_listening()
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_OPENED == mode);

    mode = SOCKET_MODE_LISTENING;
}

void NanostackSocket::signal_event()
{
    nanostack_assert_locked();

    if (callback != NULL) {
        callback(callback_data);
    }
}

void NanostackSocket::socket_callback(void *cb)
{
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
        case SOCKET_CONNECT_DONE:
            tr_debug("SOCKET_CONNECT_DONE");
            socket->event_connect_done(sock_cb);
            break;
        case SOCKET_CONNECT_FAIL:
            tr_debug("SOCKET_CONNECT_FAIL");
            socket->event_connect_fail(sock_cb);
            break;
        case SOCKET_CONNECT_AUTH_FAIL:
            tr_debug("SOCKET_CONNECT_AUTH_FAIL");
            break;
        case SOCKET_INCOMING_CONNECTION:
            tr_debug("SOCKET_INCOMING_CONNECTION");
            socket->event_incoming_connection(sock_cb);
            break;
        case SOCKET_TX_FAIL:
            tr_debug("SOCKET_TX_FAIL");
            socket->event_tx_fail(sock_cb);
            break;
        case SOCKET_CONNECT_CLOSED:
            tr_debug("SOCKET_CONNECT_CLOSED");
            socket->event_connect_closed(sock_cb);
            break;
        case SOCKET_CONNECTION_RESET:
            tr_debug("SOCKET_CONNECTION_RESET");
            socket->event_connection_reset(sock_cb);
            break;
        case SOCKET_NO_ROUTE:
            tr_debug("SOCKET_NO_ROUTE");
            socket->event_tx_fail(sock_cb);
            break;
        case SOCKET_TX_DONE:
            socket->event_tx_done(sock_cb);
            break;
        case SOCKET_NO_RAM:
            tr_debug("SOCKET_NO_RAM");
            socket->event_tx_fail(sock_cb);
            break;
        case SOCKET_CONNECTION_PROBLEM:
            tr_debug("SOCKET_CONNECTION_PROBLEM");
            break;
        default:
            break;
    }
}


void NanostackSocket::event_data(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_STREAM == mode) ||
                (SOCKET_MODE_DATAGRAM == mode));

    signal_event();
}

void NanostackSocket::event_tx_done(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT((SOCKET_MODE_STREAM == mode) ||
                (SOCKET_MODE_DATAGRAM == mode));

    if (mode == SOCKET_MODE_DATAGRAM) {
        tr_debug("SOCKET_TX_DONE, %d bytes sent", sock_cb->d_len);
    } else if (mode == SOCKET_MODE_STREAM) {
        tr_debug("SOCKET_TX_DONE, %d bytes remaining", sock_cb->d_len);
        signal_event();
    }
}

void NanostackSocket::event_connect_done(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT(SOCKET_MODE_CONNECTING == mode);

    set_connected();
    signal_event();
}

void NanostackSocket::event_connect_fail(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT(mode == SOCKET_MODE_CONNECTING);
    close();
}

void NanostackSocket::event_incoming_connection(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();
    MBED_ASSERT(mode == SOCKET_MODE_LISTENING);
    signal_event();
}

void NanostackSocket::event_connect_closed(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();

    // Can happen if we have an orderly close()
    // Might never happen as we have not implemented shutdown() in abstraction layer.
    MBED_ASSERT(mode == SOCKET_MODE_STREAM);
    close();
}

void NanostackSocket::event_tx_fail(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();

    switch (mode) {
        case SOCKET_MODE_CONNECTING:
        case SOCKET_MODE_STREAM:
            // TX_FAIL is fatal for stream sockets
            close();
            break;
        case SOCKET_MODE_DATAGRAM:
            // TX_FAIL is non-fatal for datagram sockets
            break;
        default:
            MBED_ASSERT(false);
            break;
    }
}

void NanostackSocket::event_connection_reset(socket_callback_t *sock_cb)
{
    nanostack_assert_locked();

    // Only TCP sockets can be closed by the remote end
    MBED_ASSERT((SOCKET_MODE_STREAM == mode) ||
                (SOCKET_MODE_CONNECTING == mode));
    close();
}

Nanostack::Nanostack()
    : call_event_tasklet(-1)
{
    mesh_system_init();
}

void Nanostack::call_event_tasklet_main(arm_event_s *event)
{
    if (event->event_id == CALL_EVENT) {
        nanostack_callback *cb = static_cast<nanostack_callback *>(event->data_ptr);
        cb->callback();
        delete cb;
    }
}

nsapi_error_t Nanostack::call_in(int delay, mbed::Callback<void()> func)
{
    if (call_event_tasklet < 0) {
        call_event_tasklet = eventOS_event_handler_create(&call_event_tasklet_main, 0);
        if (call_event_tasklet < 0) {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    nanostack_callback *cb = new (std::nothrow) nanostack_callback;
    if (!cb) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    cb->callback = func;

    arm_event_s event;

    event.sender =  call_event_tasklet,
    event.event_id = CALL_EVENT,
    event.receiver = call_event_tasklet,
    event.data_ptr = cb;
    event.event_type = APPLICATION_EVENT;
    event.priority = ARM_LIB_LOW_PRIORITY_EVENT;

    if (delay) {
        uint32_t ticks = eventOS_event_timer_ms_to_ticks(delay);
        if (!eventOS_event_send_in(&event, ticks)) {
            delete cb;
            return NSAPI_ERROR_NO_MEMORY;
        }
    } else {
        if (eventOS_event_send(&event) < 0) {
            delete cb;
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    return NSAPI_ERROR_OK;
}

Nanostack::call_in_callback_cb_t Nanostack::get_call_in_callback()
{
    call_in_callback_cb_t cb(this, &Nanostack::call_in);
    return cb;
}

nsapi_error_t Nanostack::get_ip_address(SocketAddress *sockAddr)
{
    NanostackLockGuard lock;

    for (int if_id = 1; if_id <= 127; if_id++) {
        uint8_t address[NSAPI_IP_BYTES];
        int ret = arm_net_address_get(if_id, ADDR_IPV6_GP, address);
        if (ret == 0) {
            sockAddr->set_ip_bytes(address, NSAPI_IPv6);
            ip6tos(address, text_ip_address);
            return NSAPI_ERROR_OK;
        }
    }
    return NSAPI_ERROR_NO_ADDRESS;
}

nsapi_error_t Nanostack::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version, const char *interface_name)
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

    // Nanostack is IPv6 stack
    if (version == NSAPI_UNSPEC) {
        version = NSAPI_IPv6;
    }

    // try nanostack DNS cache, if not found then fallback to dns query
    if (nanostack_dns_query_result_check(name, address, interface_name) == 0) {
        return NSAPI_ERROR_OK;
    }

    return nsapi_dns_query(this, name, address, interface_name, version);
}

nsapi_value_or_error_t Nanostack::gethostbyname_async(const char *name, hostbyname_cb_t callback, nsapi_version_t version, const char *interface_name)
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

    // Nanostack is IPv6 stack
    if (version == NSAPI_UNSPEC) {
        version = NSAPI_IPv6;
    }

    // try nanostack DNS cache, if not found then fallback to dns query
    if (nanostack_dns_query_result_check(name, &address, interface_name) == 0) {
        // hit found, return result immediately
        callback(NSAPI_ERROR_OK, &address);
        return NSAPI_ERROR_OK;
    }

    call_in_callback_cb_t call_in_cb = get_call_in_callback();
    return nsapi_dns_query_async(this, name, callback, call_in_cb, interface_name, version);
}

nsapi_error_t Nanostack::get_dns_server(int index, SocketAddress *address, const char *interface_name)
{
    uint8_t dns_srv_address[16];
    int8_t interface_id;
    int8_t ret;

    interface_id = nanostack_interface_id_parse(interface_name);

    ret = arm_net_dns_server_get(interface_id, dns_srv_address, NULL, 0, index);

    if (ret == 0) {
        address->set_ip_bytes(dns_srv_address, NSAPI_IPv6);
        TRACE_DEEP("get_dns_server(), index=%d, ret=%d, address=%s", index, ret, trace_ipv6((uint8_t *)address->get_ip_bytes()));
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_ADDRESS;
}

nsapi_error_t Nanostack::socket_open(void **handle, nsapi_protocol_t protocol)
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
    *handle = (void *)NULL;

    NanostackLockGuard lock;

    NanostackSocket *socket = new (std::nothrow) NanostackSocket(ns_proto);
    if (socket == NULL) {
        tr_debug("socket_open() ret=%i", NSAPI_ERROR_NO_MEMORY);
        return NSAPI_ERROR_NO_MEMORY;
    }
    if (!socket->open()) {
        delete socket;
        tr_debug("socket_open() ret=%i", NSAPI_ERROR_NO_MEMORY);
        return NSAPI_ERROR_NO_MEMORY;
    }
    *handle = (void *)socket;

    tr_debug("socket_open() socket=%p, sock_id=%d, ret=0", socket, socket->socket_id);

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::socket_close(void *handle)
{
    NanostackLockGuard lock;
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (NULL == handle) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }
    tr_debug("socket_close(socket=%p) sock_id=%d", socket, socket->socket_id);

    delete socket;

    return 0;

}

nsapi_size_or_error_t Nanostack::do_sendto(void *handle, const ns_address_t *address, const void *data, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    nsapi_size_or_error_t ret;

    NanostackLockGuard lock;

    if (socket->closed() || (!address && !socket->is_connected())) {
        ret = NSAPI_ERROR_NO_CONNECTION;
        goto out;
    }

    if (address && socket->proto == SOCKET_TCP) {
        tr_error("socket_sendto() not supported with TCP!");
        ret = NSAPI_ERROR_IS_CONNECTED;
        goto out;
    }

    int retcode;
#if 0
    retcode = ::socket_sendto(socket->socket_id, address,
                              data, size);
#else
    // Use sendmsg purely to get the new return style
    // of returning data written rather than 0 on success,
    // which means TCP can do partial writes. (Sadly,
    // it's the only call which takes flags so we can
    // leave the NS_MSG_LEGACY0 flag clear).
    ns_msghdr_t msg;
    ns_iovec_t iov;
    iov.iov_base = const_cast<void *>(data);
    iov.iov_len = size;
    msg.msg_name = const_cast<ns_address_t *>(address);
    msg.msg_namelen = address ? sizeof * address : 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    retcode = ::socket_sendmsg(socket->socket_id, &msg, 0);
#endif

    /*
     * \return length if entire amount written (which could be 0)
     * \return value >0 and <length if partial amount written (stream only)
     * \return NS_EWOULDBLOCK if nothing written due to lack of queue space.
     * \return -1 Invalid socket ID or message structure.
     * \return -2 Socket memory allocation fail.
     * \return -3 TCP state not established or address scope not defined .
     * \return -4 Socket TX process busy or unknown interface.
     * \return -5 Socket not connected
     * \return -6 Packet too short (ICMP raw socket error).
     * */
    if (retcode >= 0) {
        ret = retcode;
    } else switch (retcode) {
            case -1:
                ret = NSAPI_ERROR_PARAMETER;
                break;
            case -2:
                ret = NSAPI_ERROR_NO_MEMORY;
                break;
            case -3:
                ret = NSAPI_ERROR_NO_ADDRESS;
                break;
            case -4:
                ret = NSAPI_ERROR_BUSY;
                break;
            case -5:
                ret = NSAPI_ERROR_NO_CONNECTION;
                break;
            case -6:
                ret = NSAPI_ERROR_PARAMETER;
                break;
            case NS_EWOULDBLOCK:
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            default:
                tr_error("socket_sendmsg: error=%d", retcode);
                ret = NSAPI_ERROR_DEVICE_ERROR;
                break;
        }

out:
    tr_debug("socket_sendto(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_size_or_error_t Nanostack::socket_sendto(void *handle, const SocketAddress &address, const void *data, nsapi_size_t size)
{
    if (address.get_ip_version() != NSAPI_IPv6) {
        return NSAPI_ERROR_PARAMETER;
    }

    ns_address_t ns_address;
    convert_mbed_addr_to_ns(&ns_address, &address);
    /*No lock gaurd needed here as do_sendto() will handle locks.*/
    return do_sendto(handle, &ns_address, data, size);
}

nsapi_size_or_error_t Nanostack::socket_recvfrom(void *handle, SocketAddress *address, void *buffer, nsapi_size_t size)
{
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    nsapi_size_or_error_t ret;

    NanostackLockGuard lock;

    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
        goto out;
    }

    ns_address_t ns_address;

    int retcode;
    retcode = ::socket_recvfrom(socket->socket_id, buffer, size, 0, &ns_address);

    if (retcode == NS_EWOULDBLOCK) {
        ret = NSAPI_ERROR_WOULD_BLOCK;
    } else if (retcode < 0) {
        ret = NSAPI_ERROR_PARAMETER;
    } else {
        ret = retcode;
        if (address != NULL) {
            convert_ns_addr_to_mbed(address, &ns_address);
        }
    }

out:
    if (address) {
        tr_debug("socket_recvfrom(socket=%p) sock_id=%d, ret=%i, addr=[%s]:%i", socket, socket->socket_id, ret,
                 trace_ipv6(address->get_ip_bytes()), address->get_port());
    } else {
        tr_debug("socket_recv(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);
    }

    return ret;
}

nsapi_error_t Nanostack::socket_bind(void *handle, const SocketAddress &address)
{
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
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
            return NSAPI_ERROR_PARAMETER;
    }

    NanostackLockGuard lock;

    ns_address_t ns_address;
    ns_address.type = ADDRESS_IPV6;
    memcpy(ns_address.address, addr_field, sizeof ns_address.address);
    ns_address.identifier = address.get_port();
    nsapi_error_t ret;
    int retcode = ::socket_bind(socket->socket_id, &ns_address);

    if (retcode == 0) {
        ret = NSAPI_ERROR_OK;
    } else {
        ret = NSAPI_ERROR_PARAMETER;
    }

    tr_debug("socket_bind(socket=%p) sock_id=%d, retcode=%i, ret=%i", socket, socket->socket_id, retcode, ret);

    return ret;
}

nsapi_error_t Nanostack::setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen)
{
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    NanostackLockGuard lock;

    ns_ipv6_mreq_t ns_mreq;

    if (level == NSAPI_SOCKET) {
        switch (optname) {
            case NSAPI_ADD_MEMBERSHIP:
            case NSAPI_DROP_MEMBERSHIP: {
                if (optlen != sizeof(nsapi_ip_mreq_t)) {
                    return NSAPI_ERROR_PARAMETER;
                }
                const nsapi_ip_mreq_t *imr = static_cast<const nsapi_ip_mreq_t *>(optval);

                /* Check address types are IPv6, or unspecified for interface */
                if (imr->imr_multiaddr.version != NSAPI_IPv6 ||
                        (imr->imr_interface.version != NSAPI_UNSPEC && imr->imr_interface.version != NSAPI_IPv6)) {
                    return NSAPI_ERROR_PARAMETER;
                }

                /* Convert all parameters to Nanostack native, and proceed with setsockopt */
                memcpy(ns_mreq.ipv6mr_multiaddr, imr->imr_multiaddr.bytes, 16);
                if (imr->imr_interface.version == NSAPI_UNSPEC || memcmp(imr->imr_interface.bytes, ns_in6addr_any, 16) == 0) {
                    ns_mreq.ipv6mr_interface = 0;
                } else {
                    // If this fails, Nanostack will itself fault the invalid -1 interface ID
                    ns_mreq.ipv6mr_interface = find_interface_by_address(imr->imr_interface.bytes);
                }

                level = SOCKET_IPPROTO_IPV6;
                optname = optname == NSAPI_ADD_MEMBERSHIP ? SOCKET_IPV6_JOIN_GROUP : SOCKET_IPV6_LEAVE_GROUP;
                optval = &ns_mreq;
                optlen = sizeof ns_mreq;
                break;
            }
            default:
                return NSAPI_ERROR_UNSUPPORTED;
        }
    }

    int retcode = ::socket_setsockopt(socket->socket_id, level, optname, optval, optlen);
    if (retcode == 0) {
        return NSAPI_ERROR_OK;
    } else if (retcode == -2) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        return NSAPI_ERROR_PARAMETER;
    }
}

nsapi_error_t Nanostack::getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen)
{
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);

    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    NanostackLockGuard lock;
    // pointers to Mbed OS structures
    nsapi_latency_req_t *ns_latency_r = static_cast<nsapi_latency_req_t *>(optval);
    nsapi_stagger_req_t *ns_stagger_r = static_cast<nsapi_stagger_req_t *>(optval);
    // Nanostack internal structures
    ns_ipv6_latency_t nanostack_latency;
    ns_ipv6_stagger_t nanostack_stagger;
    int nanostack_optname = optname;

    void *ns_option_value = optval;

    if (level == NSAPI_SOCKET) {
        if (optname == NSAPI_LATENCY) {
            if (*optlen < sizeof(nsapi_latency_req_t)) {
                return NSAPI_ERROR_PARAMETER;
            }
            // Adjust to Nanostack namespace
            level = SOCKET_IPPROTO_IPV6;
            nanostack_optname = SOCKET_LATENCY;
            memcpy(nanostack_latency.dest_addr, ns_latency_r->addr, 16);
            ns_option_value = &nanostack_latency;
        } else if (optname == NSAPI_STAGGER) {
            if (*optlen < sizeof(nsapi_stagger_req_t)) {
                return NSAPI_ERROR_PARAMETER;
            }
            // Adjust to Nanostack namespace
            level = SOCKET_IPPROTO_IPV6;
            nanostack_optname = SOCKET_STAGGER;
            memcpy(nanostack_stagger.dest_addr, ns_stagger_r->addr, 16);
            nanostack_stagger.data_amount = ns_stagger_r->data_amount;
            ns_option_value = &nanostack_stagger;
        }
    }

    uint16_t optlen16 = *optlen;

    int retcode = ::socket_getsockopt(socket->socket_id, level, nanostack_optname, ns_option_value, &optlen16);
    if (retcode == 0) {
        *optlen = optlen16;
        if (optname == NSAPI_LATENCY) {
            ns_latency_r->latency = nanostack_latency.latency;
        } else if (optname == NSAPI_STAGGER) {
            ns_stagger_r->stagger_min = nanostack_stagger.stagger_min;
            ns_stagger_r->stagger_max = nanostack_stagger.stagger_max;
            ns_stagger_r->stagger_rand = nanostack_stagger.stagger_rand;
        }
        return NSAPI_ERROR_OK;
    } else if (retcode == -2) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        return NSAPI_ERROR_PARAMETER;
    }
}

nsapi_error_t Nanostack::socket_listen(void *handle, int backlog)
{
    //Check if socket exists
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    nsapi_error_t ret = NSAPI_ERROR_OK;

    NanostackLockGuard lock;

    if (::socket_listen(socket->socket_id, backlog) < 0) {
        ret = NSAPI_ERROR_PARAMETER;
    } else {
        socket->set_listening();
    }

    return ret;
}

nsapi_error_t Nanostack::socket_connect(void *handle, const SocketAddress &addr)
{
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    nsapi_error_t ret;
    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    NanostackLockGuard lock;

    if (addr.get_ip_version() != NSAPI_IPv6) {
        ret = NSAPI_ERROR_PARAMETER;
        goto out;
    }

    if (socket->closed()) {
        ret = NSAPI_ERROR_NO_CONNECTION;
        goto out;
    }

    if (socket->is_connecting()) {
        ret = NSAPI_ERROR_ALREADY;
        goto out;
    }

    if (socket->is_connected()) {
        ret = NSAPI_ERROR_IS_CONNECTED;
        goto out;
    }

    ns_address_t ns_addr;

    convert_mbed_addr_to_ns(&ns_addr, &addr);
    if (::socket_connect(socket->socket_id, &ns_addr, 0) == 0) {
        if (socket->proto == SOCKET_TCP) {
            socket->set_connecting(&ns_addr);
            ret = NSAPI_ERROR_IN_PROGRESS;
        } else {
            ret = NSAPI_ERROR_OK;
        }
    } else {
        ret = NSAPI_ERROR_DEVICE_ERROR;
    }

out:
    tr_debug("socket_connect(socket=%p) sock_id=%d, ret=%i", socket, socket->socket_id, ret);

    return ret;
}

nsapi_error_t Nanostack::socket_accept(void *server, void **handle, SocketAddress *address)
{
    NanostackSocket *socket = static_cast<NanostackSocket *>(server);
    NanostackSocket *accepted_sock = NULL;
    nsapi_error_t ret;

    if (handle == NULL) {
        MBED_ASSERT(false);
        return NSAPI_ERROR_NO_SOCKET;
    }

    NanostackLockGuard lock;

    if (!socket->is_listening()) {
        ret = NSAPI_ERROR_PARAMETER;
        goto out;
    }

    accepted_sock = new (std::nothrow) NanostackSocket(socket->proto);
    if (accepted_sock == NULL) {
        ret = NSAPI_ERROR_NO_MEMORY;
        goto out;
    }

    ns_address_t ns_addr;
    int retcode;
    retcode = socket->accept(accepted_sock, &ns_addr);
    if (retcode < 0) {
        delete accepted_sock;
        if (retcode == NS_EWOULDBLOCK) {
            ret = NSAPI_ERROR_WOULD_BLOCK;
        } else {
            ret = NSAPI_ERROR_DEVICE_ERROR;
        }
        goto out;
    }
    ret = NSAPI_ERROR_OK;

    if (address) {
        convert_ns_addr_to_mbed(address, &ns_addr);
    }

    *handle = accepted_sock;

out:
    tr_debug("socket_accept() socket=%p, sock_id=%d, ret=%i", accepted_sock, accepted_sock ? accepted_sock->socket_id : -1, ret);

    return ret;
}

nsapi_size_or_error_t Nanostack::socket_send(void *handle, const void *data, nsapi_size_t size)
{
    return do_sendto(handle, NULL, data, size);
}

nsapi_size_or_error_t Nanostack::socket_recv(void *handle, void *data, nsapi_size_t size)
{
    return socket_recvfrom(handle, NULL, data, size);
}

void Nanostack::socket_attach(void *handle, void (*callback)(void *), void *id)
{
    // Validate parameters
    NanostackSocket *socket = static_cast<NanostackSocket *>(handle);
    if (handle == NULL) {
        MBED_ASSERT(false);
        return;
    }

    NanostackLockGuard lock;

    socket->callback = callback;
    socket->callback_data = id;

    tr_debug("socket_attach(socket=%p) sock_id=%d", socket, socket->socket_id);
}

Nanostack &Nanostack::get_instance()
{
    static Nanostack nanostack;
    return nanostack;
}

// This works as long as it's not ever set to something which corresponds to
// a macro defined as a non-integer. Eg `#define Nanostack "Foo"`
#define NANOSTACK 0x99119911
#if MBED_CONF_NSAPI_DEFAULT_STACK == NANOSTACK
#undef NANOSTACK
OnboardNetworkStack &OnboardNetworkStack::get_default_instance()
{
    return Nanostack::get_instance();
}
#endif

