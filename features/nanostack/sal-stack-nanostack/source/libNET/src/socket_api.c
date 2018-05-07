/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
/**
 * \file socket_api.c
 * \brief Socket API for library model
 *
 * The socket API functions for library model
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_scheduler.h"
#include "platform/arm_hal_phy.h"
#ifndef BUILD_LIBRARY
#define BUILD_LIBRARY
#endif
#include "ns_address.h"
#include "socket_api.h"
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/ipv6_flow.h"
#include "Common_Protocols/tcp.h"
#include "Common_Protocols/udp.h"
#include "common_functions.h"

#define TRACE_GROUP "sckA"

const uint8_t ns_in6addr_any[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *))
{
    int8_t sid = -1;
    socket_family_t family;
    socket_type_t type;

    switch (protocol) {
        case SOCKET_ICMP:
            family = SOCKET_FAMILY_IPV6;
            type = SOCKET_TYPE_RAW;
            protocol = IPV6_NH_ICMPV6;
            break;
        case SOCKET_UDP:
            family = SOCKET_FAMILY_IPV6;
            type = SOCKET_TYPE_DGRAM;
            protocol = IPV6_NH_UDP;
            break;
#ifndef NO_TCP
        case SOCKET_TCP:
            family = SOCKET_FAMILY_IPV6;
            type = SOCKET_TYPE_STREAM;
            protocol = IPV6_NH_TCP;
            break;
#endif /* NO_TCP */
        case SOCKET_RAW:
            if (identifier > 0xff) {
                return -1;
            }
            family = SOCKET_FAMILY_IPV6;
            type = SOCKET_TYPE_RAW;
            protocol = identifier;
            break;
        default:
            return -1;
    }

    switch (socket_create(family, type, protocol, &sid, identifier, passed_fptr, false)) {
        case eOK:
            return sid;
        case eBUSY:
            return -2;
        case eFALSE:
        default:
            break;
    }

    return sid;
}

int8_t socket_close(int8_t sid)
{
    socket_t *socket = socket_pointer_get(sid);
    if (!socket) {
        return -1;
    }

    socket_id_detach(sid);
    return 0;
}

int8_t socket_listen(int8_t socket, uint8_t backlog)
{
#ifdef NO_TCP
    return -1;
#else
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!socket_ptr || !socket_is_ipv6(socket_ptr) || socket_ptr->inet_pcb->local_port == 0) {
        return -1;
    }

    switch (socket_ptr->type) {
        case SOCKET_TYPE_STREAM:
            break;
        default:
            return -1;
    }

    if (socket_ptr->u.live.fptr == NULL) {
        return -2;
    }

    socket_ptr->flags |= SOCKET_LISTEN_STATE;
    if (backlog < 1) {
        backlog = 1;
    } else if (backlog > SOCKET_LISTEN_BACKLOG_MAX) {
        backlog = SOCKET_LISTEN_BACKLOG_MAX;
    }
    socket_ptr->listen_backlog = backlog;

    return 0;
#endif
}

int8_t socket_accept(int8_t listen_socket_id, ns_address_t *addr, void (*passed_fptr)(void *))
{
#ifdef NO_TCP
    return -1;
#else
    socket_t *socket_ptr = socket_pointer_get(listen_socket_id);
    if (!socket_ptr || !socket_is_ipv6(socket_ptr) || !(socket_ptr->flags & SOCKET_LISTEN_STATE) || passed_fptr == NULL) {
        return -1;
    }

    /* Queue contains both partial and complete connections - find a complete one */
    socket_t *pending_socket = NULL;
    ns_list_foreach(socket_t, so, &socket_ptr->u.live.queue) {
        if (so->flags & SOCKET_FLAG_CONNECTED) {
            pending_socket = so;
            break;
        }
    }
    if (!pending_socket) {
        tr_warn("No pending complete connection for socket %d", listen_socket_id);
        return NS_EWOULDBLOCK;
    }

    int8_t new_sid = socket_id_assign_and_attach(pending_socket);
    if (new_sid == -1) {
        tr_error("Socket id allocation failed, s=%d", listen_socket_id);
        return -1;
    }

    // This takes it off the pending queue, and releases one reference count,
    // but the ID assignment above took one, so it stays live.
    socket_leave_pending_state(pending_socket, passed_fptr);

    // Make a data-received event if anything already pending - some may expect this
    if (pending_socket->rcvq.data_bytes != 0) {
        socket_data_queued_event_push(pending_socket);
    }

    if (addr) {
        addr->type = ADDRESS_IPV6;
        memcpy(addr->address, pending_socket->inet_pcb->remote_address, 16);
        addr->identifier = pending_socket->inet_pcb->remote_port;
    }

    return new_sid;
#endif
}

int8_t socket_shutdown(int8_t socket, uint8_t how)
{
#ifdef NO_TCP
    return -1;
#else
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    if (socket_ptr->type != SOCKET_TYPE_STREAM) {
        return -1;
    }

    if (!(socket_ptr->flags & SOCKET_FLAG_CONNECTED)) {
        return -2;
    }

    uint8_t flags;

    switch (how) {
        case SOCKET_SHUT_WR:
            flags = SOCKET_FLAG_SHUT_WR;
            break;
        case SOCKET_SHUT_RD:
            flags = SOCKET_FLAG_CANT_RECV_MORE;
            break;
        case SOCKET_SHUT_RDWR:
            flags = SOCKET_FLAG_CANT_RECV_MORE | SOCKET_FLAG_SHUT_WR;
            break;
        default:
            return -1;
    }

    socket_ptr->flags |= flags;

    // Take care never to put tcp_info in local variable - these calls can delete it
    int8_t ret = 0;

    // If we've shutdown writing, signal close to TCP if it exists
    if (tcp_info(socket_ptr->inet_pcb) && (flags & SOCKET_FLAG_SHUT_WR)) {
        switch (tcp_session_close(tcp_info(socket_ptr->inet_pcb))) {
            case TCP_ERROR_NO_ERROR:
                break;
            default:
                ret = -1;
                break;
        }
    }

    // Likewise for the read shutdown
    if (flags & SOCKET_FLAG_CANT_RECV_MORE) {
        if (tcp_info(socket_ptr->inet_pcb)) {
            switch (tcp_session_shutdown_read(tcp_info(socket_ptr->inet_pcb))) {
                case TCP_ERROR_NO_ERROR:
                    break;
                default:
                    ret = -1;
                    break;
            }
        }
        // Note sockbuf flush after tcp shutdown - this allows TCP to see
        // that we are binning unread data, and send a RESET to indicate.
        sockbuf_flush(&socket_ptr->rcvq);
    }

    return ret;
#endif
}

int8_t socket_getsockname(int8_t socket, ns_address_t *address) {

    const socket_t *socket_ptr = socket_pointer_get(socket);
    if (!socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    const inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;
    address->identifier = inet_pcb->local_port;
    address->type = ADDRESS_IPV6;
    memcpy(address->address, inet_pcb->local_address, 16);

    return 0;
}

int8_t socket_getpeername(int8_t socket, ns_address_t *address)
{
    const socket_t *socket_ptr = socket_pointer_get(socket);
    if (!socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    const inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;
    address->identifier = inet_pcb->remote_port;
    address->type = ADDRESS_IPV6;
    memcpy(address->address, inet_pcb->remote_address, 16);

    if (addr_ipv6_equal(inet_pcb->remote_address, ns_in6addr_any)) {
        return -2;
    }

    return 0;
}

/* Extracts data from queue, copying to msg->msg_iov.
 * Data is removed from queue. msg->msg_iov pointers and lengths not modified.
 * msg->flags gets NS_MSG_TRUNC set if a datagram was truncated.
 * For non-streams, will return actual datagram length if flags & NS_MSG_TRUNC.
 * Otherwise, returns amount read.
 */
static uint_fast16_t socket_copy_queue_to_user(sockbuf_t *sb, ns_msghdr_t *msg, int flags, bool stream)
{
    uint_fast16_t retVal = 0;

    const ns_iovec_t *iovec_ptr = msg->msg_iov;
    uint_fast16_t iovec_cnt = msg->msg_iovlen;

    uint8_t *out_ptr = iovec_ptr->iov_base;
    uint_fast16_t out_len = iovec_ptr->iov_len;
    buffer_t *buf_src = ns_list_get_first(&sb->bufs);
    const uint8_t *in_ptr = buffer_data_pointer(buf_src);
    uint_fast16_t in_len = buffer_data_length(buf_src);

    for (;;) {
        /* Compute amount to copy in this chunk - smaller of current input and output block */
        uint_fast16_t len = in_len;
        if (len > out_len) {
            len = out_len;
        }

        memcpy(out_ptr, in_ptr, len);
        retVal += len;

        /* Advance input pointer, consume buffer */
        in_ptr += len;
        in_len -= len;
        if (!(flags & NS_MSG_PEEK)) {
            buffer_data_strip_header(buf_src, len);
            sb->data_bytes -= len;
        }
        if (in_len == 0) {
            /* Drop this buffer from queue, and advance to next buffer if stream */
            if (flags & NS_MSG_PEEK) {
                buf_src = stream ? ns_list_get_next(&sb->bufs, buf_src) : NULL;
            } else {
                sockbuf_drop_first(sb);
                buf_src = stream ? ns_list_get_first(&sb->bufs) : NULL;
            }
            if (!buf_src) {
                break;
            }
            in_ptr = buffer_data_pointer(buf_src);
            in_len = buffer_data_length(buf_src);
        }

        /* There's still more input - advance output pointer */
        out_ptr += len;
        out_len -= len;
        if (out_len == 0) {
            /* Advance to next vector */
            if (--iovec_cnt == 0) {
                break;
            }
            iovec_ptr++;
            out_ptr = iovec_ptr->iov_base;
            out_len = iovec_ptr->iov_len;
        }
    }

    /* If there was anything left in buf_src, for not-stream need to drop it now */
    if (buf_src && !stream) {
        msg->msg_flags |= NS_MSG_TRUNC;
        if (flags & NS_MSG_TRUNC) {
            // Return actual length, not read length
            retVal += in_len;
        }
        if (!(flags & NS_MSG_PEEK)) {
            sockbuf_drop_first(sb);
        }
    }
    return retVal;
}

static uint8_t * socket_cmsg_ipv6_header_set(ns_cmsghdr_t *cmsg, uint8_t type, uint16_t length)
{
    cmsg->cmsg_level = SOCKET_IPPROTO_IPV6;
    cmsg->cmsg_type = type;
    cmsg->cmsg_len = NS_CMSG_LEN(length);
    return NS_CMSG_DATA(cmsg);
}

int16_t socket_read(int8_t socket, ns_address_t *src_addr, uint8_t *buffer, uint16_t length)
{
    return socket_recvfrom(socket, buffer, length, NS_MSG_LEGACY0, src_addr);
}

int16_t socket_recv(int8_t socket, void *buffer, uint16_t length, int flags)
{
    return socket_recvfrom(socket, buffer, length, flags, NULL);
}

int16_t socket_recvfrom(int8_t socket, void *buffer, uint16_t length, int flags, ns_address_t *src_addr)
{
    ns_iovec_t msg_iov;
    ns_msghdr_t msghdr;

    //Init message payload vector
    msg_iov.iov_base = buffer;
    msg_iov.iov_len = length;

    //Set messages name buffer
    msghdr.msg_name = src_addr;
    msghdr.msg_namelen = src_addr ? sizeof(ns_address_t) : 0;
    msghdr.msg_iov = &msg_iov;
    msghdr.msg_iovlen = 1;
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;

    return socket_recvmsg(socket, &msghdr, flags);
}

static uint_fast16_t copy_ancillary_to_user(ns_msghdr_t *msg, uint_fast16_t written_data, uint8_t type, void * data_ptr, uint16_t length)
{
    if (!msg->msg_control || msg->msg_controllen < (written_data + NS_CMSG_SPACE(length))) {
        msg->msg_flags |= NS_MSG_CTRUNC;
        return written_data;
    }
    uint8_t *msg_start = msg->msg_control;
    uint8_t *data_start = socket_cmsg_ipv6_header_set((ns_cmsghdr_t*) (msg_start + written_data) , type, length);
    memcpy(data_start, data_ptr, length);

    return (written_data + NS_CMSG_SPACE(length));
}

int16_t socket_recvmsg(int8_t socket, ns_msghdr_t *msg, int flags)
{
    if (!msg) {
        return -1;
    }
    msg->msg_flags = 0;
    /**
     * Validate socket id
     */
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!socket_ptr) {
        return -1;
    }

    //Read Buffer
    buffer_t *socket_buf = ns_list_get_first(&socket_ptr->rcvq.bufs);
    if (!socket_buf) {
        if (flags & NS_MSG_LEGACY0) {
            return 0;
        } else {
            return (socket_ptr->flags & SOCKET_FLAG_CANT_RECV_MORE) ? 0 : NS_EWOULDBLOCK;
        }
    }

    /**
     * Validate message payload buffer size
     */
    if (!socket_message_validate_iov(msg, NULL)) {
        return -1;
    }

    /**
     * Write Ancillary
     */
    uint_fast16_t cmsg_written = 0;
    if (socket_ptr->inet_pcb->recvpktinfo) {
        ns_in6_pktinfo_t pktinfo;
        pktinfo.ipi6_ifindex = socket_buf->interface->id;
        memcpy(pktinfo.ipi6_addr, socket_buf->dst_sa.address, 16);
        cmsg_written = copy_ancillary_to_user(msg, cmsg_written, SOCKET_IPV6_PKTINFO, &pktinfo, sizeof pktinfo);
    }

    if (socket_ptr->inet_pcb->recvhoplimit) {
        int16_t hoplimit = socket_buf->options.hop_limit;
        cmsg_written = copy_ancillary_to_user(msg, cmsg_written, SOCKET_IPV6_HOPLIMIT, &hoplimit, sizeof hoplimit);
    }

    if (socket_ptr->inet_pcb->recvtclass) {
        int16_t tclass = socket_buf->options.traffic_class;
        cmsg_written = copy_ancillary_to_user(msg, cmsg_written, SOCKET_IPV6_TCLASS, &tclass, sizeof tclass);
    }
    msg->msg_controllen = cmsg_written;

    ns_address_t *src_addr = msg->msg_name;
    //Read src address
    if (src_addr) {

        if (msg->msg_namelen != sizeof(ns_address_t)) {
            return -1;
        }

        src_addr->identifier = socket_buf->src_sa.port;
        src_addr->type = ADDRESS_IPV6;
        memcpy(src_addr->address, socket_buf->src_sa.address, 16);
    }

    //Read Data to Buffer
    int16_t len = socket_copy_queue_to_user(&socket_ptr->rcvq, msg, flags, socket_ptr->type == SOCKET_TYPE_STREAM);

    if (len > 0 && !(flags & NS_MSG_PEEK) && socket_is_ipv6(socket_ptr) && tcp_info(socket_ptr->inet_pcb)) {
        tcp_session_data_received(tcp_info(socket_ptr->inet_pcb));
    }

    return len;
}

int16_t socket_sendmsg(int8_t socket, const ns_msghdr_t *msg, int flags)
{
    if (!msg) {
        return -1;
    }

    return socket_buffer_sendmsg(socket, NULL, msg, flags);
}

int16_t socket_sendto(int8_t socket, const ns_address_t *address, const void *buffer, uint16_t length)
{
    ns_iovec_t data_vector;
    ns_msghdr_t msghdr;

    //SET IOV vector
    data_vector.iov_base = (void *) buffer;
    data_vector.iov_len = length;

    //Set message name
    msghdr.msg_name = (void *) address;
    msghdr.msg_namelen = address ? sizeof(ns_address_t) : 0;
    msghdr.msg_iov = &data_vector;
    msghdr.msg_iovlen = 1;
    //No ancillary data
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;

    return socket_sendmsg(socket, &msghdr, NS_MSG_LEGACY0);
}

int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number)
{
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!address || !socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    switch (socket_ptr->type) {
        case SOCKET_TYPE_STREAM:
        case SOCKET_TYPE_DGRAM:
            break;
        default:
            return -5;
    }

    if (socket_ptr->flags & SOCKET_LISTEN_STATE) {
        return -3;
    }

    if (socket_ptr->flags & (SOCKET_FLAG_CONNECTED|SOCKET_FLAG_CONNECTING)) {
        return -4;
    }

    inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    /* Save hash computation time in IP layer by using a random flow for connected sockets */
    if (inet_pcb->flow_label == IPV6_FLOW_AUTOGENERATE ||
            (inet_pcb->flow_label == IPV6_FLOW_UNSPECIFIED && ipv6_flow_auto_label)) {
        inet_pcb->flow_label = ipv6_flow_random();
    }

    // Choose local port if not already bound
    (void) randomly_take_src_number;
    if (inet_pcb->local_port == 0) {
        inet_pcb->local_port = socket_generate_random_port(inet_pcb->protocol);
        if (inet_pcb->local_port == 0) {
            return -7;
        }
    }

    // Choose local address if not already bound
    int8_t status = socket_bind2addrsel(socket, address);
    switch(status) {
        case 0: //OK
        case -3: // already bound
            break;
        case -1: // invalid param
        case -2: //mem alloc fail
            return status;
        case -4: // no interface
        case -5: // addr selection fails
        default:
            return -7;
    }

    memcpy(inet_pcb->remote_address, address->address, 16);
    inet_pcb->remote_port = address->identifier;
    status = 0;

#ifndef NO_TCP
    if (socket_ptr->type == SOCKET_TYPE_STREAM) {
        if (tcp_info(socket_ptr->inet_pcb)) {
            status = -4; // shouldn't happen - flags should cover it
            goto exit;
        }

        //Allocate session here
        tcp_session_t *tcp_session = tcp_session_ptr_allocate(socket_ptr->inet_pcb, NULL);
        if (!tcp_session) {
            status = -2;
            goto exit;
        }

        if (tcp_session_open(tcp_session) != TCP_ERROR_NO_ERROR) {
            tcp_session_ptr_free(tcp_session);
            status = -2;
            goto exit;
        }

        socket_ptr->flags |= SOCKET_FLAG_CONNECTING;
    }
#endif
exit:
    if (status != 0) {
        memcpy(inet_pcb->remote_address, ns_in6addr_any, 16);
        inet_pcb->remote_port = 0;
    }
    return status;
}

int8_t socket_bind(int8_t socket, const ns_address_t *address)
{
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!address || !socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    switch (socket_ptr->type) {
        case SOCKET_TYPE_STREAM:
        case SOCKET_TYPE_DGRAM:
            break;
        default:
            return -5;
    }

    inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    // are they specifying a new port?
    if (address->identifier && address->identifier != inet_pcb->local_port) {
        // don't let them do so if a port was already chosen
        if (inet_pcb->local_port != 0) {
            return -4;
        }
        if (socket_port_validate(address->identifier, inet_pcb->protocol) == eOK) {
            inet_pcb->local_port = address->identifier;
        }
        else {
            return -2;
        }
    }
    // we don't actually allocate ephemeral ports until later when we know
    // the destination (potentially useful to follow RFC 6056 some day)

    // are they specifying a new address?
    if (!addr_ipv6_equal(address->address, ns_in6addr_any) && !addr_ipv6_equal(address->address, inet_pcb->local_address)) {
        // don't let them do so if an address was already chosen
        if (!addr_ipv6_equal(inet_pcb->local_address, ns_in6addr_any)) {
            return -4;
        }

        if (protcol_interface_address_compare(NULL, address->address) != 0) {
            return -3;
        }

        memcpy(inet_pcb->local_address, address->address, 16);
    }

    return 0;
}

int8_t socket_bind2addrsel(int8_t socket, const ns_address_t *dst_address)
{
    protocol_interface_info_entry_t *if_info;

    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!dst_address || !socket_ptr || !socket_is_ipv6(socket_ptr)) {
        return -1;
    }

    switch (socket_ptr->type) {
        case SOCKET_TYPE_STREAM:
        case SOCKET_TYPE_DGRAM:
            break;
        default:
            return -6;
    }

    inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    if (!addr_ipv6_equal(inet_pcb->local_address, ns_in6addr_any)) {
        return -3;
    }

    // select source address based on destination
    buffer_t *buf = buffer_get(20);
    if (!buf) {
        return -2;
    }

    memcpy(&buf->dst_sa.address, dst_address->address, 16);
    buf->dst_sa.port = dst_address->identifier;
    if_info = socket_interface_determine(socket_ptr, buf);

    if (!if_info) {
        tr_error("socket_bind2addrsel: No interface");
        buffer_free(buf);
        return -4;
    }

    if (addr_interface_select_source(if_info, inet_pcb->local_address, dst_address->address, inet_pcb->addr_preferences) < 0) {
        tr_error("src_addr selection failed");
        buffer_free(buf);
        return -5;
    }

    buffer_free(buf);

    /* Could choose an ephemeral port here, but no point - see bind() */

    return 0;
}

int16_t socket_send(int8_t socket, const void *buffer, uint16_t length)
{
    return socket_sendto(socket, NULL, buffer, length);
}

static int8_t ipv6_setsockopt(socket_t *socket_ptr, uint8_t opt_name, const void *opt_value, uint16_t opt_len)
{
    inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    switch (opt_name) {

        /* These first few aren't really IPv6, but hey... */
        case SOCKET_INTERFACE_SELECT: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }
            int8_t interface_id = *(const int8_t *) opt_value;
            socket_ptr->default_interface_id = interface_id;

            return 0;
        }
        case SOCKET_LINK_LAYER_SECURITY: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }
            int8_t securityEnabled = *(const int8_t *) opt_value;
            inet_pcb->link_layer_security = securityEnabled;

            return 0;
        }
        case SOCKET_BROADCAST_PAN: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }
            socket_ptr->broadcast_pan = *(const int8_t *) opt_value;

            return 0;
        }
        case SOCKET_IPV6_TCLASS: {
            /* Supporting TCP properly is fiddly, and not yet needed */
            if (socket_ptr->type == SOCKET_TYPE_STREAM) {
                return -2;
            }

            if (opt_len != sizeof(int16_t)) {
                return -3;
            }

            int16_t tclass = *(const int16_t *) opt_value;
            if (tclass == -1) {
                inet_pcb->tclass = SOCKET_IPV6_TCLASS_DEFAULT;
            } else if (tclass >= 0 && tclass <= 255) {
                inet_pcb->tclass = tclass;
            } else {
                return -3;
            }

            return 0;
        }
        case SOCKET_IPV6_FLOW_LABEL: {
            if (opt_len != sizeof(int32_t)) {
                return -3;
            }

            int32_t flow = *(const int32_t *) opt_value;
            if (flow >= -2 && flow <= 0xfffff) {
                inet_pcb->flow_label = flow;
            } else {
                return -3;
            }

            return 0;
        }
        case SOCKET_IPV6_UNICAST_HOPS: {
            if (opt_len != sizeof(int16_t)) {
                return -3;
            }

            int16_t hops = *(const int16_t *) opt_value;
            if (hops >= -1 && hops <= 255) {
                inet_pcb->unicast_hop_limit = hops;
            } else {
                return -3;
            }

            return 0;
        }
        case SOCKET_IPV6_MULTICAST_HOPS: {
            if (opt_len != sizeof(int16_t)) {
                return -3;
            }

            int16_t hops = *(const int16_t *) opt_value;
            if (hops == -1) {
                inet_pcb->multicast_hop_limit = 1;
            } else if (hops >= 0 && hops <= 255) {
                inet_pcb->multicast_hop_limit = hops;
            } else {
                return -3;
            }

            return 0;
        }
        case SOCKET_IPV6_MULTICAST_IF: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }
            int8_t interface_id = *(const int8_t *) opt_value;
            if (interface_id != 0 && !protocol_stack_interface_info_get_by_id(interface_id)) {
                return -3;
            }
            inet_pcb->multicast_if = interface_id;

            return 0;
        }
        case SOCKET_IPV6_MULTICAST_LOOP: {
            if (opt_len != sizeof(bool)) {
                return -3;
            }
            inet_pcb->multicast_loop = *(const bool *) opt_value;

            return 0;
        }
        case SOCKET_IPV6_JOIN_GROUP:
        case SOCKET_IPV6_LEAVE_GROUP: {
            if (opt_len != sizeof(ns_ipv6_mreq_t)) {
                return -3;
            }
            const ns_ipv6_mreq_t *mreq = opt_value;
            if (!addr_is_ipv6_multicast(mreq->ipv6mr_multiaddr)) {
                return -3;
            }
            if (opt_name == SOCKET_IPV6_JOIN_GROUP) {
                return socket_inet_pcb_join_group(inet_pcb, mreq->ipv6mr_interface, mreq->ipv6mr_multiaddr);
            } else {
                return socket_inet_pcb_leave_group(inet_pcb, mreq->ipv6mr_interface, mreq->ipv6mr_multiaddr);
            }
        }
        case SOCKET_IPV6_ADDR_PREFERENCES: {
            if (opt_len != sizeof(uint32_t)) {
                return -3;
            }

            uint32_t flags = *(const uint32_t *) opt_value;

            /* Fault contradictory flags */
            if (flags & (flags >> 16)) {
                return -3;
            }

            /* Set specified flags, clear opposite flags */
            inet_pcb->addr_preferences |= flags;
            inet_pcb->addr_preferences &= ~((flags >> 16) | (flags << 16));

            return 0;
        }
#ifndef NO_IPV6_PMTUD
        case SOCKET_IPV6_USE_MIN_MTU: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }

            int8_t mode = *(const int8_t *) opt_value;
            if (mode < -1 || mode > 1) {
                return -3;
            }

            inet_pcb->use_min_mtu = mode;

            return 0;
        }
#endif
#ifndef NO_IP_FRAGMENT_TX
        case SOCKET_IPV6_DONTFRAG: {
            if (opt_len != sizeof(int8_t)) {
                return -3;
            }

            int8_t mode = *(const int8_t *) opt_value;
            if (mode < 0 || mode > 1) {
                return -3;
            }

            inet_pcb->dontfrag = mode;

            return 0;
        }
#endif
        /* Are there any users of this yet? Can we remove it? */
        case SOCKET_IPV6_ADDRESS_SELECT: {
            if (opt_len != sizeof(int16_t)) {
                return -3;
            }

            int16_t address_mode = *(const int16_t *) opt_value;

            if (address_mode == SOCKET_SRC_ADDRESS_MODE_PRIMARY) {
                inet_pcb->addr_preferences |= SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
                inet_pcb->addr_preferences &= ~(uint32_t) SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG;
            } else if (address_mode == SOCKET_SRC_ADDRESS_MODE_SECONDARY) {
                inet_pcb->addr_preferences |= SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG;
                inet_pcb->addr_preferences &= ~(uint32_t) SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
            } else {
                return -3;
            }

            return 0;
        }

        case SOCKET_IPV6_RECVPKTINFO: {
            if (opt_len != sizeof(bool)) {
                return -3;
            }
            inet_pcb->recvpktinfo = *(const bool *) opt_value;
            return 0;
        }
        case SOCKET_IPV6_RECVHOPLIMIT: {
            if (opt_len != sizeof(bool)) {
                return -3;
            }
            inet_pcb->recvhoplimit = *(const bool *) opt_value;
            return 0;
        }

        case SOCKET_IPV6_RECVTCLASS: {
            if (opt_len != sizeof(bool)) {
                return -3;
            }
            inet_pcb->recvtclass = *(const bool *) opt_value;
            return 0;
        }
        default:
            return -2;
    }
}

int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len)
{
    socket_t *socket_ptr = socket_pointer_get(socket);
    if (!opt_value || !socket_ptr) {
        return -1;
    }

    if (level == SOCKET_IPPROTO_IPV6 && socket_is_ipv6(socket_ptr)) {
        return ipv6_setsockopt(socket_ptr, opt_name, opt_value, opt_len);
    }

    if (level != SOCKET_SOL_SOCKET) {
        return -2;
    }

    switch (opt_name) {
        case SOCKET_SO_SNDBUF:
        case SOCKET_SO_RCVBUF: {
            if (opt_len != sizeof(int32_t)) {
                return -3;
            }
            int32_t value = *(const int32_t *) opt_value;
            if (value < 0) {
                return -3;
            }
            sockbuf_t *sb = opt_name == SOCKET_SO_SNDBUF ? &socket_ptr->sndq
                                                         : &socket_ptr->rcvq;
            return sockbuf_reserve(sb, value) ? 0 : -3;
        }
        case SOCKET_SO_SNDLOWAT: {
            if (opt_len != sizeof(int32_t)) {
                return -3;
            }
            int32_t value = *(const int32_t *) opt_value;
            if (value < 1) {
                return -3;
            }
            sockbuf_t *sb = &socket_ptr->sndq;
            if ((uint32_t) value > sb->data_byte_limit) {
                value = sb->data_byte_limit;
            }
            sb->low_water_mark = value;
            return 0;
        }
        default:
            return -2;
    }
}

static union {
    int8_t s8;
    uint16_t u16;
    int16_t s16;
    uint32_t u32;
    int32_t s32;
    bool boolean;
} opt_temp;

static int8_t ipv6_getsockopt(const socket_t *socket_ptr, uint8_t opt_name, const void **value, uint16_t *len)
{
    const inet_pcb_t *inet_pcb = socket_ptr->inet_pcb;

    switch (opt_name) {
        case SOCKET_INTERFACE_SELECT: {
            const int8_t *p = &socket_ptr->default_interface_id;
            *value = p;
            *len = sizeof *p;
            break;
        }
        case SOCKET_LINK_LAYER_SECURITY: {
            const int8_t *p = &inet_pcb->link_layer_security;
            *value = p;
            *len = sizeof *p;
            break;
        }
        case SOCKET_BROADCAST_PAN: {
            const int8_t *p = &socket_ptr->broadcast_pan;
            *value = p;
            *len = sizeof *p;
            break;
        }
        case SOCKET_IPV6_TCLASS: {
            opt_temp.s16 = inet_pcb->tclass;
            *value = &opt_temp.s16;
            *len = sizeof(int16_t);
            break;
        }
        case SOCKET_IPV6_FLOW_LABEL: {
            opt_temp.s32 = inet_pcb->flow_label;
            *value = &opt_temp.s32;
            *len = sizeof(int32_t);
            break;
        }
        case SOCKET_IPV6_UNICAST_HOPS: {
            opt_temp.s16 = inet_pcb->unicast_hop_limit;
            if (opt_temp.s16 == -1) {
                /* Hop limit is determined dynamically on TX - try to work out
                 * a sensible number for the caller; we can't just say -1.
                 */
                opt_temp.s16 = UNICAST_HOP_LIMIT_DEFAULT;

                /* Try to look at interface for connected sockets */
                if (socket_ptr->type == SOCKET_TYPE_STREAM) {
                    tcp_session_t *info = tcp_info(socket_ptr->inet_pcb);
                    if (info) {
                        opt_temp.s16 = info->interface->cur_hop_limit;
                    }
                }
            }
            *value = &opt_temp.s16;
            *len = sizeof(int16_t);
            break;
        }
        case SOCKET_IPV6_MULTICAST_HOPS: {
            opt_temp.s16 = inet_pcb->multicast_hop_limit;
            /* Unlike unicast hop limit, this can't be -1 */
            *value = &opt_temp.s16;
            *len = sizeof(int16_t);
            break;
        }
        case SOCKET_IPV6_MULTICAST_IF: {
            opt_temp.s8 = inet_pcb->multicast_if;
            *value = &opt_temp.s8;
            *len = sizeof(int8_t);
            break;
        }
        case SOCKET_IPV6_MULTICAST_LOOP: {
            opt_temp.boolean = inet_pcb->multicast_loop;
            *value = &opt_temp.boolean;
            *len = sizeof(bool);
            break;
        }
        case SOCKET_IPV6_ADDR_PREFERENCES: {
            const uint32_t *p = &inet_pcb->addr_preferences;
            *value = p;
            *len = sizeof *p;
            break;
        }
#ifndef NO_IPV6_PMTUD
        case SOCKET_IPV6_USE_MIN_MTU: {
            const int8_t *p = &inet_pcb->use_min_mtu;
            *value = p;
            *len = sizeof *p;
            break;
        }
#endif
#ifndef NO_IP_FRAGMENT_TX
        case SOCKET_IPV6_DONTFRAG: {
            const int8_t *p = &inet_pcb->dontfrag;
            *value = p;
            *len = sizeof *p;
            break;
        }
#endif
        case SOCKET_IPV6_RECVPKTINFO:
            opt_temp.boolean = inet_pcb->recvpktinfo;
            *value = &opt_temp.boolean;
            *len = sizeof(bool);
            break;
        case SOCKET_IPV6_RECVHOPLIMIT:
            opt_temp.boolean = inet_pcb->recvhoplimit;
            *value = &opt_temp.boolean;
            *len = sizeof(bool);
            break;
        case SOCKET_IPV6_RECVTCLASS:
            opt_temp.boolean = inet_pcb->recvtclass;
            *value = &opt_temp.boolean;
            *len = sizeof(bool);
            break;

        default:
            return -2;
    }

    return 0;
}

int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len)
{
    const socket_t *socket_ptr = socket_pointer_get(socket);
    if (!opt_value || !opt_len || !socket_ptr) {
        return -1;
    }

    const void *value;
    uint16_t len;

    if (level == SOCKET_IPPROTO_IPV6 && socket_is_ipv6(socket_ptr)) {
        int8_t ret = ipv6_getsockopt(socket_ptr, opt_name, &value, &len);
        if (ret != 0) {
            return ret;
        }
    } else if (level == SOCKET_SOL_SOCKET) {
        switch (opt_name) {
            case SOCKET_SO_SNDBUF:
                opt_temp.s32 = socket_ptr->sndq.data_byte_limit;
                value = &opt_temp.s32;
                len = sizeof(int32_t);
                break;
            case SOCKET_SO_RCVBUF:
                opt_temp.s32 = socket_ptr->rcvq.data_byte_limit;
                value = &opt_temp.s32;
                len = sizeof(int32_t);
                break;
            case SOCKET_SO_SNDLOWAT:
                opt_temp.s32 = socket_ptr->rcvq.low_water_mark;
                value = &opt_temp.s32;
                len = sizeof(int32_t);
                break;
            default:
                return -2;
        }
    } else {
        return -2;
    }

    if (*opt_len > len) {
        *opt_len = len;
    }

    memcpy(opt_value, value, *opt_len);

    return 0;
}

ns_cmsghdr_t *NS_CMSG_NXTHDR(const ns_msghdr_t *msgh, const ns_cmsghdr_t *cmsg)
{
    if (!cmsg) {
        return NS_CMSG_FIRSTHDR(msgh);
    }
    uint8_t *start_of_next_header = (uint8_t *) (cmsg) + NS_ALIGN_SIZE(cmsg->cmsg_len, CMSG_HEADER_ALIGN);
    uint8_t *end_of_next_header = start_of_next_header + NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN);
    if (end_of_next_header > (uint8_t *) (msgh)->msg_control + (msgh)->msg_controllen) {
        return NULL;
    }
    return (ns_cmsghdr_t *) start_of_next_header;
}

