/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#include <string.h>
#include "coap_connection_handler.h"
#include "coap_security_handler.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "socket_api.h"
#include "net_interface.h"
#include "eventOS_event_timer.h"
#include "coap_service_api_internal.h"

#define TRACE_GROUP "ThCH"

typedef enum session_state_e {
    SECURE_SESSION_HANDSHAKE_ONGOING = 0,
    SECURE_SESSION_OK,
    SECURE_SESSION_CLOSED
} session_state_t;

typedef struct internal_socket_s {
    coap_conn_handler_t *parent;
    cch_func_cb *cch_function_callback;  // callback function

    uint32_t timeout_min;
    uint32_t timeout_max;

    uint16_t listen_port; // 0 for ephemeral-port sockets

    int16_t data_len;
    uint8_t *data;
    int8_t recv_if_id;    // interface ID where data is coming from

    int8_t socket;  //positive value = socket id, negative value virtual socket id
    bool real_socket;
    uint8_t usage_counter;
    bool is_secure;

    bool bypass_link_sec;

    ns_list_link_t link;
} internal_socket_t;

const uint8_t COAP_MULTICAST_ADDR_LINK_LOCAL[16] = { 0xff, 0x02, [15] = 0xfd }; // ff02::fd, COAP link-local multicast (rfc7390)
const uint8_t COAP_MULTICAST_ADDR_ADMIN_LOCAL[16] = { 0xff, 0x03, [15] = 0xfd }; // ff02::fd, COAP admin-local multicast (rfc7390)
const uint8_t COAP_MULTICAST_ADDR_SITE_LOCAL[16] = { 0xff, 0x05, [15] = 0xfd }; // ff05::fd, COAP site-local multicast (rfc7390)

static NS_LIST_DEFINE(socket_list, internal_socket_t, link);

static uint8_t max_handshakes = MAX_ONGOING_HANDSHAKES;
static uint8_t max_sessions = MAX_SECURE_SESSION_COUNT;

static void timer_cb(void *param);

static void recv_sckt_msg(void *cb_res);
#ifdef COAP_SECURITY_AVAILABLE
static void secure_recv_sckt_msg(void *cb_res);
#endif

#define TIMER_STATE_CANCELLED -1 /* cancelled */
#define TIMER_STATE_NO_EXPIRY 0 /* none of the delays is expired */
#define TIMER_STATE_INT_EXPIRY 1 /* the intermediate delay only is expired */
#define TIMER_STATE_FIN_EXPIRY 2 /* the final delay is expired */

typedef struct secure_timer_s {
    uint8_t id;
    timeout_t *timer;
    int8_t state;
    uint32_t fin_ms;
    uint32_t int_ms;
} secure_timer_t;

typedef struct secure_session {
    coap_security_t *sec_handler; //owned
    internal_socket_t *parent; //not owned

    ns_address_t remote_host;
    uint8_t local_address[16];
    // local port is fixed by socket

    secure_timer_t timer;

    session_state_t session_state;
    uint32_t last_contact_time;
    ns_list_link_t link;
} secure_session_t;

static NS_LIST_DEFINE(secure_session_list, secure_session_t, link);
static int secure_session_sendto(int8_t socket_id, void *handle, const void *buf, size_t len);
static int secure_session_recvfrom(int8_t socket_id, unsigned char *buf, size_t len);
static void start_timer(int8_t timer_id, uint32_t int_ms, uint32_t fin_ms);
static int timer_status(int8_t timer_id);

static secure_session_t *secure_session_find_by_timer_id(int8_t timer_id)
{
    secure_session_t *this = NULL;
    ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
        if (cur_ptr->timer.id == timer_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static bool is_secure_session_valid(secure_session_t *session)
{
    ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
        if (cur_ptr == session) {
            return true;
        }
    }
    return false;
}

static void secure_session_delete(secure_session_t *this)
{
    if (this) {
        ns_list_remove(&secure_session_list, this);
        transactions_delete_all(this->remote_host.address, this->remote_host.identifier);
        if (this->sec_handler) {
            coap_security_destroy(this->sec_handler);
            this->sec_handler = NULL;
        }
        if (this->timer.timer) {
            eventOS_timeout_cancel(this->timer.timer);
        }
        ns_dyn_mem_free(this);
        this = NULL;
    }

    return;
}

static int8_t virtual_socket_id_allocate()
{
    int8_t new_virtual_socket_id = -1; // must not overlap with real socket id's
    ns_list_foreach(internal_socket_t, cur_ptr, &socket_list) {
        if (cur_ptr->socket <= new_virtual_socket_id) {
            new_virtual_socket_id = cur_ptr->socket - 1;
        }
    }
    return new_virtual_socket_id;
}

static secure_session_t *secure_session_create(internal_socket_t *parent, const uint8_t *address_ptr, uint16_t port, SecureConnectionMode secure_mode)
{
    uint8_t handshakes = 0;
    if (!address_ptr) {
        return NULL;
    }

    if (max_sessions <= ns_list_count(&secure_session_list)) {
        // Seek & destroy oldest session where close notify have been sent
        secure_session_t *to_be_removed = NULL;
        ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
            if (cur_ptr->session_state == SECURE_SESSION_CLOSED) {
                if (!to_be_removed || cur_ptr->last_contact_time < to_be_removed->last_contact_time) {
                    to_be_removed = cur_ptr;
                }
            }
        }
        if (!to_be_removed) {
            tr_err("max session count exceeded");
            return NULL;
        }

        secure_session_delete(to_be_removed);
    }

    // Count for ongoing handshakes
    ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
        if (cur_ptr->session_state == SECURE_SESSION_HANDSHAKE_ONGOING) {
            handshakes++;
        }
    }
    if (handshakes >= max_handshakes) {
        tr_err("ongoing handshakes exceeded");
        return NULL;
    }

    secure_session_t *this = ns_dyn_mem_alloc(sizeof(secure_session_t));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(secure_session_t));

    uint8_t timer_id = 1;

    while (secure_session_find_by_timer_id(timer_id)) {
        if (timer_id == 0xff) {
            ns_dyn_mem_free(this);
            return NULL;
        }
        timer_id++;
    }
    this->last_contact_time = coap_service_get_internal_timer_ticks();
    this->timer.id = timer_id;
    this->remote_host.type = ADDRESS_IPV6;
    memcpy(this->remote_host.address, address_ptr, 16);
    this->remote_host.identifier = port;

    this->sec_handler = coap_security_create(parent->socket, this->timer.id, this, secure_mode,
                                             &secure_session_sendto, &secure_session_recvfrom, &start_timer, &timer_status);
    if (!this->sec_handler) {
        tr_err("security create failed");
        ns_dyn_mem_free(this);
        return NULL;
    }
    this->parent = parent;

    this->session_state = SECURE_SESSION_HANDSHAKE_ONGOING;
    ns_list_add_to_start(&secure_session_list, this);

    return this;
}


static void clear_secure_sessions(internal_socket_t *this)
{
    if (this) {
        ns_list_foreach_safe(secure_session_t, cur_ptr, &secure_session_list) {
            if (cur_ptr->parent == this) {
                coap_security_send_close_alert(cur_ptr->sec_handler);
                secure_session_delete(cur_ptr);
            }
        }
    }
}

static secure_session_t *secure_session_find(internal_socket_t *parent, const uint8_t *address_ptr, uint16_t port)
{
    secure_session_t *this = NULL;
    ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
        if (cur_ptr->sec_handler) {
            if (cur_ptr->parent == parent && cur_ptr->remote_host.identifier == port &&
                    memcmp(cur_ptr->remote_host.address, address_ptr, 16) == 0) {
                this = cur_ptr;
                break;
            }
        }
    }
    return this;
}

static void coap_multicast_group_join_or_leave(int8_t socket_id, uint8_t opt_name, int8_t interface_id)
{
    ns_ipv6_mreq_t ns_ipv6_mreq;
    int8_t ret_val;

    // Join or leave COAP multicast groups
    ns_ipv6_mreq.ipv6mr_interface = interface_id;

    memcpy(ns_ipv6_mreq.ipv6mr_multiaddr, COAP_MULTICAST_ADDR_LINK_LOCAL, 16);
    ret_val = socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, opt_name, &ns_ipv6_mreq, sizeof(ns_ipv6_mreq));

    memcpy(ns_ipv6_mreq.ipv6mr_multiaddr, COAP_MULTICAST_ADDR_ADMIN_LOCAL, 16);
    ret_val |= socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, opt_name, &ns_ipv6_mreq, sizeof(ns_ipv6_mreq));

    memcpy(ns_ipv6_mreq.ipv6mr_multiaddr, COAP_MULTICAST_ADDR_SITE_LOCAL, 16);
    ret_val |= socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, opt_name, &ns_ipv6_mreq, sizeof(ns_ipv6_mreq));

    if (ret_val) {
        tr_error("Multicast group access failed, err=%d, name=%d", ret_val, opt_name);
    }
}

static internal_socket_t *int_socket_create(uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool real_socket, bool bypassSec, int8_t socket_interface_selection, bool multicast_registration)
{
    internal_socket_t *this = ns_dyn_mem_alloc(sizeof(internal_socket_t));

    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(internal_socket_t));

    this->data_len = 0;
    this->data = NULL;

    this->is_secure = is_secure;
    this->usage_counter = 1;

    this->listen_port = listen_port;
    this->real_socket = real_socket;
    this->bypass_link_sec = bypassSec;
    this->socket = -1;
    if (real_socket) {
        if (use_ephemeral_port) { //socket_api creates ephemeral port if the one provided is 0
            listen_port = 0;
        }
        if (!is_secure) {
            this->socket = socket_open(SOCKET_UDP, listen_port, recv_sckt_msg);
        } else {
#ifdef COAP_SECURITY_AVAILABLE
            this->socket = socket_open(SOCKET_UDP, listen_port, secure_recv_sckt_msg);
#else
            tr_err("Secure CoAP unavailable - SSL library not configured, possibly due to lack of entropy source");
#endif
        }
        // Socket create failed
        if (this->socket < 0) {
            ns_dyn_mem_free(this);
            return NULL;
        }

        socket_setsockopt(this->socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &(const int8_t) {
            bypassSec ? 0 : 1
        }, sizeof(int8_t));

        // XXX API for this? May want to get clever to do recommended first query = 1 hop, retries = whole PAN
        socket_setsockopt(this->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &(const int16_t) {
            16
        }, sizeof(int16_t));

        // Set socket option to receive packet info
        socket_setsockopt(this->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &(const bool) {
            1
        }, sizeof(bool));
        if (socket_interface_selection > 0) {
            // Interface selection requested as socket_interface_selection set
            socket_setsockopt(this->socket, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &socket_interface_selection, sizeof(socket_interface_selection));
        }

        if (multicast_registration) {
            coap_multicast_group_join_or_leave(this->socket, SOCKET_IPV6_JOIN_GROUP, socket_interface_selection);
        }
    } else {
        this->socket = virtual_socket_id_allocate();
    }

    ns_list_add_to_start(&socket_list, this);
    return this;
}

static void int_socket_delete(internal_socket_t *this)
{
    if (this) {
        this->usage_counter--;
        if (this->usage_counter == 0) {
            clear_secure_sessions(this);
            socket_close(this->socket);
            ns_list_remove(&socket_list, this);
            if (this->data) {
                ns_dyn_mem_free(this->data);
                this->data = NULL;
            }
            if (this->parent) {
                ns_dyn_mem_free(this->parent);
            }
            ns_dyn_mem_free(this);
        }
    }
}

static internal_socket_t *int_socket_find_by_socket_id(int8_t id)
{
    internal_socket_t *this = NULL;
    ns_list_foreach(internal_socket_t, cur_ptr, &socket_list) {
        if (cur_ptr->socket == id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static internal_socket_t *int_socket_find(uint16_t port, bool is_secure, bool is_real_socket, bool bypassSec)
{
    (void) bypassSec;

    internal_socket_t *this = NULL;
    ns_list_foreach(internal_socket_t, cur_ptr, &socket_list) {
        if (cur_ptr->listen_port == port && cur_ptr->real_socket == is_real_socket &&
                is_secure == cur_ptr->is_secure /*&& bypass_link_sec == bypassSec*/) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static int send_to_real_socket(int8_t socket_id, const ns_address_t *address, const uint8_t source_address[static 16], const void *buffer, uint16_t length)
{
    ns_iovec_t msg_iov = {
        .iov_base = (void *) buffer,
        .iov_len = length
    };
    ns_msghdr_t msghdr = {
        .msg_name = (void *) address,
        .msg_namelen = sizeof(ns_address_t),
        .msg_iov = &msg_iov,
        .msg_iovlen = 1
    };

    uint8_t ancillary_databuffer[NS_CMSG_SPACE(sizeof(ns_in6_pktinfo_t))];

    if (memcmp(source_address, ns_in6addr_any, 16)) {
        ns_cmsghdr_t *cmsg;
        ns_in6_pktinfo_t *pktinfo;

        msghdr.msg_control = ancillary_databuffer;
        msghdr.msg_controllen = sizeof(ancillary_databuffer);

        cmsg = NS_CMSG_FIRSTHDR(&msghdr);
        cmsg->cmsg_type = SOCKET_IPV6_PKTINFO;
        cmsg->cmsg_level = SOCKET_IPPROTO_IPV6;
        cmsg->cmsg_len = NS_CMSG_LEN(sizeof(ns_in6_pktinfo_t));

        pktinfo = (ns_in6_pktinfo_t *)NS_CMSG_DATA(cmsg);
        pktinfo->ipi6_ifindex = 0;
        memcpy(pktinfo->ipi6_addr, source_address, 16);
    }

    return socket_sendmsg(socket_id, &msghdr, 0);
}

static int secure_session_sendto(int8_t socket_id, void *handle, const void *buf, size_t len)
{
    secure_session_t *session = handle;
    internal_socket_t *sock = int_socket_find_by_socket_id(socket_id);
    if (!sock) {
        return -1;
    }
    if (!sock->real_socket) {
        // Send to virtual socket cb
        int ret = sock->parent->_send_cb(sock->socket, session->remote_host.address, session->remote_host.identifier, buf, len);
        if (ret < 0) {
            return ret;
        }
        return len;
    }

    int opt_name = SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
    int8_t securityLinkLayer = 1;
    if (sock->bypass_link_sec) {
        securityLinkLayer = 0;
    }
    socket_setsockopt(sock->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_name, sizeof(int));
    socket_setsockopt(sock->socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
    //For some reason socket_sendto returns 0 in success, while other socket impls return number of bytes sent!!!
    //TODO: check if address_ptr is valid and use that instead if it is

    int ret = send_to_real_socket(sock->socket, &session->remote_host, session->local_address, buf, len);
    if (ret < 0) {
        return ret;
    }
    return len;
}

static int secure_session_recvfrom(int8_t socket_id, unsigned char *buf, size_t len)
{
    (void)len;
    internal_socket_t *sock = int_socket_find_by_socket_id(socket_id);
    if (sock->data && sock->data_len > 0) {
        memcpy(buf, sock->data, sock->data_len);
        int l = sock->data_len;
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
        sock->data_len = 0;
        return l;
    }
    return MBEDTLS_ERR_SSL_WANT_READ;
}

/**
 * Callback timer. Maybe called in interrupt context
 * so keep it simple.
 * TODO - might be better to use an event timer in conjunction with
 * CoAP tasklet
 */
static void timer_cb(void *param)
{
    secure_session_t *sec = param;

    if (sec && is_secure_session_valid(sec)) {
        if (sec->timer.fin_ms > sec->timer.int_ms) {
            /* Intermediate expiry */
            sec->timer.fin_ms -= sec->timer.int_ms;
            sec->timer.state = TIMER_STATE_INT_EXPIRY;
            int error = coap_security_handler_continue_connecting(sec->sec_handler);
            if (MBEDTLS_ERR_SSL_TIMEOUT == error) {
                //TODO: How do we handle timeouts?
                secure_session_delete(sec);
            } else {
                sec->timer.timer = eventOS_timeout_ms(timer_cb, sec->timer.int_ms, (void *)sec);
            }
        } else {
            /* We have counted the number of cycles - finish */
            eventOS_timeout_cancel(sec->timer.timer);
            sec->timer.fin_ms = 0;
            sec->timer.int_ms = 0;
            sec->timer.timer = NULL;
            sec->timer.state = TIMER_STATE_FIN_EXPIRY;
            int error = coap_security_handler_continue_connecting(sec->sec_handler);
            if (MBEDTLS_ERR_SSL_TIMEOUT == error) {
                //TODO: How do we handle timeouts?
                secure_session_delete(sec);
            }
        }
    }
}

static void start_timer(int8_t timer_id, uint32_t int_ms, uint32_t fin_ms)
{
    secure_session_t *sec = secure_session_find_by_timer_id(timer_id);
    if (sec) {
        if ((int_ms > 0) && (fin_ms > 0)) {
            sec->timer.int_ms = int_ms;
            sec->timer.fin_ms = fin_ms;
            sec->timer.state = TIMER_STATE_NO_EXPIRY;
            if (sec->timer.timer) {
                eventOS_timeout_cancel(sec->timer.timer);
            }
            sec->timer.timer = eventOS_timeout_ms(timer_cb, int_ms, sec);
        } else if (fin_ms == 0) {
            /* fin_ms == 0 means cancel the timer */
            sec->timer.state = TIMER_STATE_CANCELLED;
            eventOS_timeout_cancel(sec->timer.timer);
            sec->timer.fin_ms = 0;
            sec->timer.int_ms = 0;
            sec->timer.timer = NULL;
        }
    }
}

static int timer_status(int8_t timer_id)
{
    secure_session_t *sec = secure_session_find_by_timer_id(timer_id);
    if (sec) {
        return (int)sec->timer.state;
    }
    return TIMER_STATE_CANCELLED;
}

static int read_data(socket_callback_t *sckt_data, internal_socket_t *sock, ns_address_t *src_address, uint8_t dst_address[static 16])
{
    sock->data_len = 0;
    sock->recv_if_id = -1;
    if (sckt_data->event_type == SOCKET_DATA && sckt_data->d_len > 0) {
        uint8_t ancillary_databuffer[NS_CMSG_SPACE(sizeof(ns_in6_pktinfo_t))];
        ns_iovec_t msg_iov;
        ns_msghdr_t msghdr;
        ns_in6_pktinfo_t *pkt = NULL;

        if (sock->data) {
            ns_dyn_mem_free(sock->data);
            sock->data = NULL;
        }

        sock->data = ns_dyn_mem_temporary_alloc(sckt_data->d_len);
        if (!sock->data) {
            sock->data = NULL;
            return -1;
        }

        msghdr.msg_name = src_address;
        msghdr.msg_namelen = sizeof(ns_address_t);
        msghdr.msg_iov = &msg_iov;
        msghdr.msg_iovlen = 1;
        msghdr.msg_control = ancillary_databuffer;
        msghdr.msg_controllen = sizeof(ancillary_databuffer);

        msg_iov.iov_base = sock->data;
        msg_iov.iov_len = sckt_data->d_len;

        sock->data_len = socket_recvmsg(sckt_data->socket_id, &msghdr, 0);

        if (sock->data_len > 0) {
            ns_cmsghdr_t *cmsg = NS_CMSG_FIRSTHDR(&msghdr);

            while (cmsg) {
                switch (cmsg->cmsg_type) {
                    case SOCKET_IPV6_PKTINFO:
                        pkt = (ns_in6_pktinfo_t *)NS_CMSG_DATA(cmsg);
                        break;
                    default:
                        break;
                }
                cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
            }
            if (pkt) {
                memcpy(dst_address, pkt->ipi6_addr, 16);
                sock->recv_if_id = pkt->ipi6_ifindex;
            } else {
                goto return_failure;
            }
        } else {
            goto return_failure;
        }
    } else {
        goto return_failure;
    }

    return 0;

return_failure:
    ns_dyn_mem_free(sock->data);
    sock->data = NULL;
    sock->data_len = 0;
    return -1;


}

#ifdef COAP_SECURITY_AVAILABLE
static void secure_recv_sckt_msg(void *cb_res)
{
    socket_callback_t *sckt_data = cb_res;
    internal_socket_t *sock = int_socket_find_by_socket_id(sckt_data->socket_id);
    ns_address_t src_address;
    uint8_t dst_address[16] = {0};
    memset(&src_address, 0, sizeof(ns_address_t));

    if (sock && read_data(sckt_data, sock, &src_address, dst_address) == 0) {
        /* If received from multicast address, reject */
        if (*(dst_address) == 0xFF) {
            return;
        }
        secure_session_t *session = secure_session_find(sock, src_address.address, src_address.identifier);

        // Create session
        if (!session) {
            coap_security_keys_t keys;
            memset(&keys, 0, sizeof(coap_security_keys_t));

            if (sock->parent->_get_password_cb && 0 == sock->parent->_get_password_cb(sock->socket, src_address.address, src_address.identifier, &keys)) {
                session = secure_session_create(sock, src_address.address, src_address.identifier, keys.mode);
                if (!session) {
                    tr_err("secure_recv_sckt_msg session creation failed - OOM");
                    ns_dyn_mem_free(keys._key);
                    return;
                }
                //TODO: error handling
            } else {
                return;
            }

            // Record the destination. We are not strict on local address - all
            // session_find calls match only on remote address and port. But we
            // record the last-used destination address to use it as the source of
            // outgoing packets.
            memcpy(session->local_address, dst_address, 16);

            session->last_contact_time = coap_service_get_internal_timer_ticks();
            // Start handshake
            if (!coap_security_handler_is_started(session->sec_handler)) {
                if (-1 == coap_security_handler_connect_non_blocking(session->sec_handler, true, DTLS, keys, sock->timeout_min, sock->timeout_max)) {
                    tr_err("Connection start failed");
                    secure_session_delete(session);
                }
                ns_dyn_mem_free(keys._key);
            }
        } else {
            //Continue handshake
            if (session->session_state == SECURE_SESSION_HANDSHAKE_ONGOING) {
                int ret = coap_security_handler_continue_connecting(session->sec_handler);
                // Handshake done
                if (ret == 0) {
                    eventOS_timeout_cancel(session->timer.timer);
                    session->timer.timer = NULL;
                    session->session_state = SECURE_SESSION_OK;
                    if (sock->parent->_security_done_cb) {
                        sock->parent->_security_done_cb(sock->socket, src_address.address,
                                                        src_address.identifier,
                                                        (void *)coap_security_handler_keyblock(session->sec_handler));
                    }
                } else if (ret < 0) {
                    // error handling
                    // TODO: here we also should clear CoAP retransmission buffer and inform that CoAP request sending is failed.
                    secure_session_delete(session);
                }
                //Session valid
            } else {
                unsigned char *data = ns_dyn_mem_temporary_alloc(sock->data_len);
                int len = 0;
                len = coap_security_handler_read(session->sec_handler, data, sock->data_len);
                if (len < 0) {
                    if (len != MBEDTLS_ERR_SSL_WANT_READ && len != MBEDTLS_ERR_SSL_WANT_WRITE &&
                            len != MBEDTLS_ERR_SSL_UNEXPECTED_MESSAGE) {
                        secure_session_delete(session);
                    }
                    ns_dyn_mem_free(data);
                } else {
                    if (sock->parent->_recv_cb) {
                        sock->parent->_recv_cb(sock->socket, sock->recv_if_id, src_address.address, src_address.identifier, dst_address, data, len);
                    }
                    ns_dyn_mem_free(data);
                }
            }
        }
    }
}
#endif

static void recv_sckt_msg(void *cb_res)
{
    socket_callback_t *sckt_data = cb_res;
    internal_socket_t *sock = int_socket_find_by_socket_id(sckt_data->socket_id);
    ns_address_t src_address;
    uint8_t dst_address[16];

    if (sock && read_data(sckt_data, sock, &src_address, dst_address) == 0) {
        if (sock->parent && sock->parent->_recv_cb) {
            sock->parent->_recv_cb(sock->socket, sock->recv_if_id, src_address.address, src_address.identifier, dst_address, sock->data, sock->data_len);
        }
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
    }
}

int coap_connection_handler_virtual_recv(coap_conn_handler_t *handler, uint8_t address[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
    if (!handler || !handler->socket) {
        return -1;
    }
    internal_socket_t *sock = handler->socket;
    sock->data_len = data_len;
    if (sock->data) {
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
    }
    sock->data = ns_dyn_mem_temporary_alloc(data_len);
    if (data_len > 0 && !sock->data) {
        return -1;
    }
    if (data_ptr) {
        memcpy(sock->data, data_ptr, data_len);
    } else {
        if (sock->data) {
            ns_dyn_mem_free(sock->data);
            sock->data = NULL;
        }
    }

    if (handler->socket->is_secure) {
        coap_security_keys_t keys;
        memset(&keys, 0, sizeof(coap_security_keys_t));

        secure_session_t *session = secure_session_find(sock, address, port);
        if (!session) {
            if (sock->parent->_get_password_cb && 0 == sock->parent->_get_password_cb(sock->socket, address, port, &keys)) {
                session = secure_session_create(sock, address, port, keys.mode);
                if (!session) {
                    tr_err("coap_connection_handler_virtual_recv session creation failed");
                    ns_dyn_mem_free(keys._key);
                    return -1;
                }
                if (-1 == coap_security_handler_connect_non_blocking(session->sec_handler, true, DTLS, keys, handler->socket->timeout_min, handler->socket->timeout_max)) {
                    tr_err("Connection start failed");
                    ns_dyn_mem_free(keys._key);
                    secure_session_delete(session);
                    return -1;
                }
                ns_dyn_mem_free(keys._key);
                return 0;
            } else {
                return -1;
            }
        }

        session->last_contact_time = coap_service_get_internal_timer_ticks();

        if (coap_security_handler_is_started(session->sec_handler)) {
            if (session->session_state == SECURE_SESSION_HANDSHAKE_ONGOING) {
                int ret = coap_security_handler_continue_connecting(session->sec_handler);
                if (ret == 0) {
                    session->session_state = SECURE_SESSION_OK;
                    if (handler->_security_done_cb) {
                        handler->_security_done_cb(sock->socket,
                                                   address, port,
                                                   (void *)coap_security_handler_keyblock(session->sec_handler));
                    }
                    return 0;
                } else if (ret < 0) {
                    // error handling
                    // TODO: here we also should clear CoAP retransmission buffer and inform that CoAP request sending is failed.
                    secure_session_delete(session);
                }
                //TODO: error handling
            } else {
                unsigned char *data = ns_dyn_mem_temporary_alloc(sock->data_len);
                int len = 0;
                len = coap_security_handler_read(session->sec_handler, data, sock->data_len);
                if (len < 0) {
                    if (len != MBEDTLS_ERR_SSL_WANT_READ && len != MBEDTLS_ERR_SSL_WANT_WRITE &&
                            len != MBEDTLS_ERR_SSL_UNEXPECTED_MESSAGE) {
                        secure_session_delete(session);
                    }
                    ns_dyn_mem_free(data);
                    return 0;
                } else {
                    if (sock->parent->_recv_cb) {
                        sock->parent->_recv_cb(sock->socket, sock->recv_if_id, address, port, ns_in6addr_any, data, len);
                    }
                    ns_dyn_mem_free(data);
                    data = NULL;
                }
                return 0;
            }
        }
    } else {
        /* unsecure*/
        if (sock->parent->_recv_cb) {
            sock->parent->_recv_cb(sock->socket, sock->recv_if_id, address, port, ns_in6addr_any, sock->data, sock->data_len);
        }
        if (sock->data) {
            ns_dyn_mem_free(sock->data);
            sock->data = NULL;
        }
        return 0;
    }
    return -1;
}

coap_conn_handler_t *connection_handler_create(receive_from_socket_cb *recv_from_cb,
                                               send_to_socket_cb *send_to_cb,
                                               get_pw_cb *pw_cb,
                                               security_done_cb *done_cb)
{
    if (recv_from_cb == NULL) {
        return NULL;
    }

    coap_conn_handler_t *handler = ns_dyn_mem_alloc(sizeof(coap_conn_handler_t));
    if (!handler) {
        return NULL;
    }
    memset(handler, 0, sizeof(coap_conn_handler_t));
    handler->socket = NULL;
    handler->_recv_cb = recv_from_cb;
    handler->_send_cb = send_to_cb;

    handler->_get_password_cb = pw_cb;
    handler->_security_done_cb = done_cb;

    return handler;
}

void connection_handler_destroy(coap_conn_handler_t *handler, bool multicast_group_leave)
{
    if (handler) {
        if (multicast_group_leave) {
            coap_multicast_group_join_or_leave(handler->socket->socket, SOCKET_IPV6_LEAVE_GROUP, handler->socket_interface_selection);
        }
        if (handler->security_keys) {
            ns_dyn_mem_free(handler->security_keys);
        }
        int_socket_delete(handler->socket);
        ns_dyn_mem_free(handler);
    }
}

void connection_handler_close_secure_connection(coap_conn_handler_t *handler, uint8_t destination_addr_ptr[static 16], uint16_t port)
{
    if (handler) {
        if (handler->socket && handler->socket->is_secure) {
            secure_session_t *session = secure_session_find(handler->socket, destination_addr_ptr, port);
            if (session) {
                coap_security_send_close_alert(session->sec_handler);
                session->session_state = SECURE_SESSION_CLOSED;
                session->last_contact_time = coap_service_get_internal_timer_ticks();
            }
        }
    }
}

int coap_connection_handler_open_connection(coap_conn_handler_t *handler, uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool is_real_socket, bool bypassSec)
{
    if (!handler) {
        return -1;
    }
    //virtual socket must have send callback
    if (!is_real_socket && !handler->_send_cb) {
        return -1;
    }
    if (handler->socket) {
        int_socket_delete(handler->socket);
    }

    internal_socket_t *current = !use_ephemeral_port ? int_socket_find(listen_port, is_secure, is_real_socket, bypassSec) : NULL;
    if (!current) {
        handler->socket = int_socket_create(listen_port, use_ephemeral_port, is_secure, is_real_socket, bypassSec, handler->socket_interface_selection, handler->registered_to_multicast);
        if (!handler->socket) {
            return -1;
        }
        handler->socket->parent = ns_dyn_mem_alloc(sizeof(coap_conn_handler_t));
        if (!handler->socket->parent) {
            int_socket_delete(handler->socket);
            return -1;
        }
        *handler->socket->parent = *handler;
    } else {
        current->usage_counter++;
        handler->socket = current;
    }
    return 0;
}

int coap_connection_handler_send_data(coap_conn_handler_t *handler, const ns_address_t *dest_addr, const uint8_t src_address[static 16], uint8_t *data_ptr, uint16_t data_len, bool bypass_link_sec)
{
    if (!handler || !handler->socket || !dest_addr) {
        return -1;
    }

    /* Secure send */
    if (handler->socket->is_secure) {
        handler->socket->bypass_link_sec = bypass_link_sec;
        secure_session_t *session = secure_session_find(handler->socket, dest_addr->address, dest_addr->identifier);
        if (!session) {
            coap_security_keys_t security_material;
            int ret_val = 0;

            memset(&security_material, 0, sizeof(coap_security_keys_t));

            if (!handler->_get_password_cb || 0 != handler->_get_password_cb(handler->socket->socket, (uint8_t *)dest_addr->address, dest_addr->identifier, &security_material)) {
                return -1;
            }

            session = secure_session_create(handler->socket, dest_addr->address, dest_addr->identifier, security_material.mode);
            if (!session || (0 > coap_security_handler_connect_non_blocking(session->sec_handler, false, DTLS, security_material, handler->socket->timeout_min, handler->socket->timeout_max))) {
                ret_val = -1;
            }

            ns_dyn_mem_free(security_material._key);
            return ret_val;

        } else if (session->session_state == SECURE_SESSION_OK) {
            session->last_contact_time = coap_service_get_internal_timer_ticks();
            if (0 > coap_security_handler_send_message(session->sec_handler, data_ptr, data_len)) {
                return -1;
            }
        }
        /* Unsecure */
    } else {
        /* Virtual socket */
        if (!handler->socket->real_socket && handler->_send_cb) {
            if (handler->_send_cb((int8_t)handler->socket->socket, dest_addr->address, dest_addr->identifier, data_ptr, data_len) < 0) {
                return -1;
            }
        } else {
            int opt_name = SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
            int8_t securityLinkLayer = 1;
            if (bypass_link_sec) {
                securityLinkLayer = 0;
            }

            socket_setsockopt(handler->socket->socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_name, sizeof(int));
            socket_setsockopt(handler->socket->socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));

            if (0 > send_to_real_socket(handler->socket->socket, dest_addr, src_address, data_ptr, data_len)) {
                return -1;
            }
        }
    }

    return 1;
}

bool coap_connection_handler_socket_belongs_to(coap_conn_handler_t *handler, int8_t socket_id)
{
    if (!handler || !handler->socket) {
        return false;
    }

    if (handler->socket->socket == socket_id) {
        return true;
    }
    return false;
}

int8_t coap_connection_handler_set_timeout(coap_conn_handler_t *handler, uint32_t min, uint32_t max)
{
    if (!handler || !handler->socket) {
        return -1;
    }
    handler->socket->timeout_max = max;
    handler->socket->timeout_min = min;

    return 0;
}

int8_t coap_connection_handler_handshake_limits_set(uint8_t handshakes_limit, uint8_t connections_limit)
{
    if (!handshakes_limit || !connections_limit) {
        return -1;
    }
    max_handshakes = handshakes_limit;
    max_sessions = connections_limit;

    return 0;
}

/* No need to call every second - call rather like every minute (SECURE_SESSION_CLEAN_INTERVAL sets this) */
void coap_connection_handler_exec(uint32_t time)
{
    if (ns_list_count(&secure_session_list)) {
        // Seek & destroy old sessions where close notify have been sent
        ns_list_foreach_safe(secure_session_t, cur_ptr, &secure_session_list) {
            if (cur_ptr->session_state == SECURE_SESSION_CLOSED) {
                if ((cur_ptr->last_contact_time +  CLOSED_SECURE_SESSION_TIMEOUT) <= time) {
                    secure_session_delete(cur_ptr);
                }
            } else if (cur_ptr->session_state == SECURE_SESSION_OK) {
                if ((cur_ptr->last_contact_time +  OPEN_SECURE_SESSION_TIMEOUT) <= time) {
                    secure_session_delete(cur_ptr);
                }
            } else if (cur_ptr->session_state == SECURE_SESSION_HANDSHAKE_ONGOING) {
                if ((cur_ptr->last_contact_time +  ONGOING_HANDSHAKE_TIMEOUT) <= time) {
                    secure_session_delete(cur_ptr);
                }
            }
        }
    }
}

int coap_connection_handler_msg_prevalidate_callback_set(coap_conn_handler_t *handler, cch_func_cb *function_callback)
{
    if (!handler) {
        return -1;
    }
    handler->socket->cch_function_callback = function_callback;
    return 0;
}

coap_conn_handler_t *coap_connection_handler_find_by_socket_port(uint16_t listen_port)
{
    ns_list_foreach(internal_socket_t, cur_ptr, &socket_list) {
        if (cur_ptr->listen_port == listen_port) {
            return cur_ptr->parent;
        }
    }
    return NULL;
}

cch_func_cb *coap_connection_handler_msg_prevalidate_callback_get(coap_conn_handler_t *handler, uint16_t *listen_socket_port)
{
    if (!handler || !listen_socket_port) {
        return NULL;
    }

    *listen_socket_port = handler->socket->listen_port;
    return handler->socket->cch_function_callback;
}
