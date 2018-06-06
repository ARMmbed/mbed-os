/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ip_fsc.h"
#include "ns_sha256.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "randLIB.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/ipv6_flow.h"
#include "Common_Protocols/tcp.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "net_interface.h"

#ifndef NO_TCP
#define TRACE_GROUP "tcp"

/* Standard flags for outgoing packets in each state, with FIN applying
 * only if there is no more data in queue (because the state is entered
 * while queue not empty).
 */
static const uint8_t state_flag[TCP_STATES] =
{
    [TCP_STATE_CLOSED] = TCP_FLAG_RST | TCP_FLAG_ACK,
    [TCP_STATE_SYN_SENT] = TCP_FLAG_SYN,
    [TCP_STATE_SYN_RECEIVED] = TCP_FLAG_SYN | TCP_FLAG_ACK,
    [TCP_STATE_ESTABLISHED] = TCP_FLAG_ACK,
    [TCP_STATE_FIN_WAIT_1] = TCP_FLAG_FIN | TCP_FLAG_ACK,
    [TCP_STATE_FIN_WAIT_2] = TCP_FLAG_ACK,
    [TCP_STATE_CLOSE_WAIT] = TCP_FLAG_ACK,
    [TCP_STATE_LAST_ACK] = TCP_FLAG_FIN | TCP_FLAG_ACK,
    [TCP_STATE_CLOSING] = TCP_FLAG_FIN | TCP_FLAG_ACK,
    [TCP_STATE_TIME_WAIT] = TCP_FLAG_ACK
};

#ifdef TCP_TEST
static uint8_t rx_drops[TCP_STATES];
static uint8_t tx_drops[TCP_STATES];

/*Test - functions*/
int8_t tcp_test_drop_tx(int state, uint8_t count) {

    tx_drops[state] = count;
    return 0;
}

int8_t tcp_test_drop_rx(int state, uint8_t count) {

    rx_drops[state] = count;
    return 0;
}

void tcp_test_drop_reset() {

  memset(rx_drops, 0, sizeof rx_drops);
  memset(tx_drops, 0, sizeof tx_drops);

}
#endif

#ifdef FEA_TRACE_SUPPORT
static const char *trace_tcp_flags(uint16_t flags)
{
    static char buf[9];
    buf[0] = flags & TCP_FLAG_CWR ? 'C' : '-';
    buf[1] = flags & TCP_FLAG_ECE ? 'E' : '-';
    buf[2] = flags & TCP_FLAG_URG ? 'U' : '-';
    buf[3] = flags & TCP_FLAG_ACK ? 'A' : '-';
    buf[4] = flags & TCP_FLAG_PSH ? 'P' : '-';
    buf[5] = flags & TCP_FLAG_RST ? 'R' : '-';
    buf[6] = flags & TCP_FLAG_SYN ? 'S' : '-';
    buf[7] = flags & TCP_FLAG_FIN ? 'F' : '-';
    return buf;
}
#endif

static void tcp_timer_handle(uint16_t ticksUpdate);
static void tcp_segment_start(tcp_session_t *tcp_info, bool timeout);
static void tcp_uack_segment(buffer_t *buf, tcp_session_t *tcp_info, uint16_t header_length);
static buffer_t *tcp_ack_buffer(tcp_session_t *tcp_info, uint16_t new_ack);
static void tcp_ack_segment(uint32_t ack, tcp_session_t *tcp_info);
static buffer_t *tcp_build_reset_packet(const sockaddr_t *dst_addr, const sockaddr_t *src_addr, uint32_t seq, uint32_t ack, uint8_t flag);
static void tcp_session_established(protocol_interface_info_entry_t *cur, tcp_session_t *tcp_info);
static void tcp_build(buffer_t *buf, tcp_session_t *tcp_info);

static uint16_t tcp_session_count;

#define TCP_FUNC_ENTRY_TRACE
#ifdef TCP_FUNC_ENTRY_TRACE
#define FUNC_ENTRY_TRACE    tr_debug
#else
#define FUNC_ENTRY_TRACE(...)
#endif

/* Returns true if min <= val <= max */
static bool tcp_seq_in_range(uint32_t min, uint32_t val, uint32_t max)
{
    if (min <= max) {
        return min <= val && val <= max;
    } else {
        return min <= val || val <= max;
    }
}
/* Returns true if val > low */
static bool tcp_seq_gt(uint32_t val, uint32_t low)
{
    return common_serial_number_greater_32(val, low);
}

/* Returns true if val < high */
static bool tcp_seq_lt(uint32_t val, uint32_t high)
{
    return common_serial_number_greater_32(high, val);
}

/* Returns true if val >= min */
static bool tcp_seq_ge(uint32_t val, uint32_t min)
{
    return val == min || common_serial_number_greater_32(val, min);
}

#if 0
/* Returns true if val <= max */
static bool tcp_seq_le(uint32_t val, uint32_t max)
{
    return val == max || common_serial_number_greater_32(max, val);
}
#endif

/* Delete TCP session (optionally with error) */
static tcp_session_t *tcp_session_delete_with_error(tcp_session_t *tcp_info, uint8_t error)
{
    socket_t *socket = tcp_info->inet_pcb->socket;
    FUNC_ENTRY_TRACE("tcp_session_delete_with_error(), s=%d", socket->id);
    socket_connection_abandoned(socket, tcp_info->interface->id, error);
    return tcp_session_ptr_free(tcp_info);
}

static tcp_session_t *tcp_session_delete(tcp_session_t *tcp_info)
{
    return tcp_session_delete_with_error(tcp_info, 0);
}

/* Socket released by application */
void tcp_socket_released(tcp_session_t *cur)
{
    FUNC_ENTRY_TRACE("tcp_socket_released() s=%d", cur->inet_pcb->socket->id);

    /* Most clear-up already done, including tcp_session_close */

    /* Only thing to do is start "orphaned session" timer for FIN-WAIT-2 */

    if (cur->state == TCP_STATE_FIN_WAIT_2) {
        cur->timer = TCP_FIN_WAIT_2_TIMEOUT;
    }
}

/* RFC 6528 Initial Sequence Number generation */
static uint32_t tcp_generate_isn(const uint8_t local_ip[static 16], uint16_t local_port,
                                 const uint8_t remote_ip[static 16], uint16_t remote_port)
{
    static struct {
        uint8_t key[16];
        bool initialised;
    } secret;
    static uint32_t fudge;

    tr_debug("isn [%s]:%u -> [%s]:%u", trace_ipv6(local_ip), local_port, trace_ipv6(remote_ip), remote_port);

    if (!secret.initialised) {
        /* randLIB should do - we're not after total cryptographic security, and
         * it should have been seeded from true randomness. An alternative would
         * be the secret given to addr_opaque_iid_key_set, but that would have
         * the effect of restarting with the same ISN after reboot. Random secret
         * means we start with a random ISN after reboot.
         */
        randLIB_get_n_bytes_random(secret.key, sizeof secret.key);
        secret.initialised = true;
        tr_debug("secret init");
    }

    ns_sha256_context ctx;
    uint32_t F;
    ns_sha256_init(&ctx);
    ns_sha256_starts(&ctx);
    ns_sha256_update(&ctx, local_ip, 16);
    ns_sha256_update(&ctx, &local_port, sizeof local_port);
    ns_sha256_update(&ctx, remote_ip, 16);
    ns_sha256_update(&ctx, &remote_port, sizeof remote_port);
    ns_sha256_update(&ctx, secret.key, sizeof secret.key);
    ns_sha256_finish_nbits(&ctx, &F, 32);
    ns_sha256_free(&ctx);

    /* Want to make sure we're always going up in successive connects, so
     * add a positive fraction of a tick every time.
     */
    fudge += randLIB_get_8bit() + 1;

    /* Monotonic time is 100ms ticks, so *25000 converts to 4us ticks */
    uint32_t M = 25000 * protocol_core_monotonic_time + fudge;
    tr_debug("ISN=%"PRIx32" M=%"PRIx32" F=%"PRIx32" fudge=%"PRIx32, M + F, M, F, fudge);

    return M + F;
}

static void tcp_set_metadata(const inet_pcb_t *inet_pcb, buffer_t *buf)
{
    /* Fill in metadata from inet_pcb. Everything used here should be cloned into
     * new PCBs created by incoming connections from the listening PCB, so that it's
     * possible to set options for the first transactions prior to accept().
     */
    buf->options.traffic_class = inet_pcb->tclass;
    buf->options.flow_label = inet_pcb->flow_label;
    buf->options.ll_security_bypass_tx = (inet_pcb->link_layer_security == 0);
    buf->options.ipv6_dontfrag = true; // Sanity-checking - if doing MTU work right, fragmentation should never be needed
    socket_inet_pcb_set_buffer_hop_limit(inet_pcb, buf, NULL);
}

static void tcp_rethink_mss(tcp_session_t *tcp_info)
{
    tcp_info->send_mss_eff = tcp_info->send_mss_peer;
    if (tcp_info->interface->nwk_id == IF_6LoWPAN) {
        // This conservatism also covers up worries about extension headers,
        // RPL tunnelling...
        if (tcp_info->send_mss_eff > TCP_LOWPAN_MSS_LIMIT) {
            tcp_info->send_mss_eff = TCP_LOWPAN_MSS_LIMIT;
        }
    }
    /* No proper PMTUD at the minute... */
    if (tcp_info->send_mss_eff > IPV6_MIN_LINK_MTU - 20 - 40) {
        tcp_info->send_mss_eff = IPV6_MIN_LINK_MTU - 20 - 40;
    }

    /* Don't get totally silly */
    if (tcp_info->send_mss_eff < 64) {
        tcp_info->send_mss_eff = 64;
    }
}

/* Allocate new TCP socket data structure */
tcp_session_t *tcp_session_ptr_allocate(inet_pcb_t *inet_pcb, tcp_session_t *from_time_wait)
{
    tcp_session_t *cur = NULL;
    FUNC_ENTRY_TRACE("tcp_session_ptr_allocate() s=%d", inet_pcb->socket->id);

    cur = ns_dyn_mem_alloc(sizeof(tcp_session_t));
    if (cur) {
        if (++tcp_session_count == 1) {
            protocol_timer_start(PROTOCOL_TIMER_TCP_TIM, tcp_timer_handle, TCP_TIMER_PERIOD);
        }
        cur->passive_open = false;
        cur->interface = 0;
        cur->busy = false;
        cur->persist = false;
        cur->sent_fin = false;
        cur->receive_next = 0;
        cur->receive_adv = 0;
        cur->send_window = 0;
        if (from_time_wait) {
            cur->send_next = from_time_wait->send_next + UINT32_C(250000) + randLIB_get_16bit();
            tcp_session_delete(from_time_wait);
        } else {
            cur->send_next = tcp_generate_isn(inet_pcb->local_address, inet_pcb->local_port, inet_pcb->remote_address, inet_pcb->remote_port);
        }
        cur->send_unacknowledged = cur->send_next;
        cur->state = TCP_STATE_CLOSED;
        cur->timer = 0;
        cur->retry = 0;
        cur->inet_pcb = inet_pcb;
        cur->srtt8 = INT16_MAX; // indicates no sample yet
        cur->srttvar4 = INT16_MAX;
        cur->rto = TCP_INITIAL_RTO;
        cur->receive_mss = IPV6_MIN_LINK_MTU - 20 - 40;
        cur->send_mss_eff = 536;
        cur->send_mss_peer = 536;
        inet_pcb->session = cur;
        socket_reference(inet_pcb->socket);
    }
    return cur;
}

/* Free TCP session pointer */
tcp_session_t *tcp_session_ptr_free(tcp_session_t *tcp_info)
{
    socket_t *so = tcp_info->inet_pcb->socket;
    FUNC_ENTRY_TRACE("tcp_session_ptr_free() s=%d", so->id);
    tcp_info->inet_pcb->session = NULL;
    so->flags |= SOCKET_FLAG_CANT_RECV_MORE | SOCKET_FLAG_SHUT_WR;
    // This could free both the inet PCB and the socket - do not reference further
    socket_dereference(so);
    tcp_info->inet_pcb = NULL;

    ns_dyn_mem_free(tcp_info);

    if (--tcp_session_count == 0) {
        protocol_timer_stop(PROTOCOL_TIMER_TCP_TIM);
    }

    return NULL;
}

tcp_error tcp_session_send(tcp_session_t *tcp_info, buffer_t *buf)
{
    FUNC_ENTRY_TRACE("tcp_session_send()");

    if (tcp_info->state != TCP_STATE_ESTABLISHED && tcp_info->state != TCP_STATE_CLOSE_WAIT) {
        tr_error("tcp error, bad state %d", tcp_info->state);
        return TCP_ERROR_WRONG_STATE;
    }

    /* Wipe out the socket ID - never having socket ID set on down buffers ensures no callbacks are generated by core layers */
    buffer_socket_set(buf, NULL);
    buf->session_ptr = NULL;
    sockbuf_append_and_compress(&tcp_info->inet_pcb->socket->sndq, buf);

    if (!tcp_info->busy) {
        tcp_info->timer = 0;
        tcp_info->retry = 0;
        tcp_info->busy = true;
        tcp_segment_start(tcp_info, false);
    }

    return TCP_ERROR_NO_ERROR;
}


/* Builds and returns a RST packet; frees incoming buffer;
 * if session is provided, deletes session and signals error to user
 */
static buffer_t *tcp_reset_response(tcp_session_t *tcp_info, uint8_t error, buffer_t *incoming_buf, uint32_t seq, uint32_t ack, uint8_t flags)
{
    FUNC_ENTRY_TRACE("tcp_reset_response()");
    buffer_t *rst_buffer;

    rst_buffer = tcp_build_reset_packet(&incoming_buf->src_sa, &incoming_buf->dst_sa, seq, ack, flags);
    if (rst_buffer) {
        rst_buffer->interface = incoming_buf->interface;
        if (tcp_info) {
            tcp_set_metadata(tcp_info->inet_pcb, rst_buffer);
        }
    }
    buffer_free(incoming_buf);
    if (tcp_info) {
        tcp_session_delete_with_error(tcp_info, error);
    }

    return (rst_buffer);
}

static buffer_t *tcp_build_reset_packet(const sockaddr_t *dst_addr, const sockaddr_t *src_addr, uint32_t seq, uint32_t ack, uint8_t flags)
{
    buffer_t *buf;
    uint8_t *ptr;

    FUNC_ENTRY_TRACE("tcp_build_reset_packet()");
    buf = buffer_get(20);

    if (!buf) {
        return NULL;
    }

    buf->dst_sa = *dst_addr;
    buf->src_sa = *src_addr;
    buf->info = (buffer_info_t)(B_DIR_DOWN | B_FROM_TCP | B_TO_IPV6);
    buf->options.type = IPV6_NH_TCP;

    ptr = buffer_data_pointer(buf);
    ptr = common_write_16_bit(src_addr->port, ptr);
    ptr = common_write_16_bit(dst_addr->port, ptr);
    ptr = common_write_32_bit(seq, ptr);
    ptr = common_write_32_bit(ack, ptr);
    *ptr++ = 5 << 4; /* data offset */
    *ptr++ = flags;
    ptr = common_write_16_bit(0, ptr);
    ptr = common_write_32_bit(0, ptr); /* checksum and URG pointer */
    buffer_data_end_set(buf, ptr);

    /* calculate checksum */
    common_write_16_bit(buffer_ipv6_fcf(buf, IPV6_NH_TCP), buffer_data_pointer(buf) + 16);

    return (buf);
}

static void tcp_session_send_reset_to_abort_connection(tcp_session_t *tcp_info) {

    sockaddr_t dst, src;
    memcpy(&dst.address, tcp_info->inet_pcb->remote_address, 16);
    memcpy(&src.address, tcp_info->inet_pcb->local_address, 16);
    dst.addr_type = ADDR_IPV6;
    src.addr_type = ADDR_IPV6;
    dst.port = tcp_info->inet_pcb->remote_port;
    src.port = tcp_info->inet_pcb->local_port;
    buffer_t *buf = tcp_build_reset_packet(&dst, &src, tcp_info->send_next, 0, TCP_FLAG_RST);
    if (!buf) {
        return;
    }

    buf->interface = tcp_info->interface;
    tcp_set_metadata(tcp_info->inet_pcb, buf);
    /* Don't fill in socket ID, or app would get a TX DONE callback for this */
    protocol_push(buf);
}

/**
 * \brief Function used for starting a TCP connection
 * to a remote server.
 *
 * \param tcp_session pointer to allocated tcp session
 * \param dst_address_ptr pointer to connection dstination address
 * \param dst_port connection destination port
 *
 * \return TCP_ERROR_NO_ERROR
 * \return TCP_ERROR_BUFFER_ALLOCATION_ERROR
 */

tcp_error tcp_session_open(tcp_session_t *tcp_session)
{
    buffer_t *new_buffer;
    protocol_interface_info_entry_t *cur_interface;
    FUNC_ENTRY_TRACE("tcp_session_open()");

    if (tcp_session->state != TCP_STATE_CLOSED && tcp_session->state != TCP_STATE_LISTEN) {
        return TCP_ERROR_WRONG_STATE;
    }

    new_buffer = buffer_get(127);
    if (!new_buffer) {
        return TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    }

    new_buffer->dst_sa.port = tcp_session->inet_pcb->remote_port;
    new_buffer->dst_sa.addr_type = ADDR_IPV6;
    memcpy(new_buffer->dst_sa.address, tcp_session->inet_pcb->remote_address, 16);

    cur_interface = socket_interface_determine(tcp_session->inet_pcb->socket, new_buffer);

    if (!cur_interface) {
        buffer_free(new_buffer);
        return TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    }

    /**
     * Save interface id and type
     */
    new_buffer->interface = cur_interface;
    tcp_session->interface = cur_interface;

    new_buffer->interface->id = cur_interface->id;
    tcp_session->interface->id = cur_interface->id;

    if (tcp_session->interface->nwk_id == IF_6LoWPAN) {
        if (tcp_session->receive_mss > TCP_LOWPAN_MSS_LIMIT) {
            tcp_session->receive_mss = TCP_LOWPAN_MSS_LIMIT;
        }
    }

    tcp_session->state = TCP_STATE_SYN_SENT;
    new_buffer->options.code = TCP_FLAG_SYN;

    tcp_build(new_buffer, tcp_session);
    return TCP_ERROR_NO_ERROR;

}

tcp_error tcp_session_shutdown_read(tcp_session_t *tcp_session)
{
    FUNC_ENTRY_TRACE("tcp_session_shutdown_read");

    if (tcp_session->inet_pcb->socket->rcvq.data_bytes != 0) {
        // Force a reset shutdown (RFC 1122 4.2.2.13, RFC 2525 2.17)
        tr_warn("Shutdown read with pending data");
        return tcp_session_abort(tcp_session);
    }

    return TCP_ERROR_NO_ERROR;
}

/**
 * \brief Function used for closing a TCP connection.
 *
 * If it returns TCP_ERROR_NO_ERROR, TCP session is persisting.
 * Any error return means TCP session has been deleted.
 *
 * \param tcp_session pointer to indicate tcp session
 *
 * \return TCP_ERROR_NO_ERROR
 * \return TCP_ERROR_BUFFER_ALLOCATION_ERROR
 */

tcp_error tcp_session_close(tcp_session_t *tcp_session)
{
    FUNC_ENTRY_TRACE("tcp_session_close");
    switch (tcp_session->state) {
        case TCP_STATE_CLOSED:
        case TCP_STATE_LAST_ACK:
        case TCP_STATE_FIN_WAIT_1:
        case TCP_STATE_FIN_WAIT_2:
        case TCP_STATE_CLOSING:
        case TCP_STATE_TIME_WAIT:
            return TCP_ERROR_NO_ERROR;
        case TCP_STATE_LISTEN:
        case TCP_STATE_SYN_SENT:
        default:
            tcp_session_delete(tcp_session);
            return TCP_SESSION_REMOVED;
        case TCP_STATE_SYN_RECEIVED:
        case TCP_STATE_ESTABLISHED:
        case TCP_STATE_CLOSE_WAIT:
            // Do the state change immediately, even if data
            // pending - this deals with the API changes. Data
            // output routines need to set flags appropriately.
            if (tcp_session->state == TCP_STATE_CLOSE_WAIT) {
                tcp_session->state = TCP_STATE_LAST_ACK; // RFC 1122
                tr_debug("sLA");
            } else {
                tcp_session->state = TCP_STATE_FIN_WAIT_1;
                tr_debug("sFW");
            }
            if (!tcp_session->busy) {
                tcp_session->busy = true;
                tcp_segment_start(tcp_session, false);
            }
            return TCP_ERROR_NO_ERROR;
    }
}

tcp_error tcp_session_abort(tcp_session_t *tcp_session)
{
    FUNC_ENTRY_TRACE("tcp_session_abort");

    switch (tcp_session->state) {
        case TCP_STATE_CLOSED:
            return TCP_ERROR_SOCKET_NOT_FOUND;
        case TCP_STATE_SYN_RECEIVED:
        case TCP_STATE_ESTABLISHED:
        case TCP_STATE_FIN_WAIT_1:
        case TCP_STATE_FIN_WAIT_2:
        case TCP_STATE_CLOSE_WAIT:
            tcp_session_send_reset_to_abort_connection(tcp_session);
            /* fall through */
        case TCP_STATE_LISTEN:
        case TCP_STATE_SYN_SENT:
            tcp_session_delete_with_error(tcp_session, SOCKET_CONNECTION_RESET);
            return TCP_ERROR_NO_ERROR;
        default:
            tcp_session_delete(tcp_session);
            return TCP_ERROR_NO_ERROR;
    }
}

/**
 * \brief Function used to send TCP message
 *
 * Can be either used to send a data segment or FIN in case there
 * is no data. When retransmission is made, starts to sent from the
 * start of the unacknowledged data.
 *
 * \param tcp_info pointer to indicate tcp session
 * \param timeout triggered from timeout
 *
 * \return TCP_ERROR_NO_ERROR
 * \return TCP_ERROR_BUFFER_ALLOCATION_ERROR
 */
static void tcp_segment_start(tcp_session_t *tcp_info, bool timeout)
{
    socket_t *so = tcp_info->inet_pcb->socket;
    buffer_t *tx_data = ns_list_get_first(&so->sndq.bufs);
    buffer_t *buf;

    FUNC_ENTRY_TRACE("tcp_segment_start() s=%d", so->id);

    if (timeout) {
        // If not same means that retransmission is made. Could be either
        // partial or full depending on what has been acknowledged. If data
        // is to be sent, sends always window size
        tcp_info->send_next = tcp_info->send_unacknowledged;
        tcp_info->sent_fin = false;
    }

    uint16_t data_length;

    if (!tx_data) {
        // Should only be called with no data if needing to send a SYN or FIN
        if ((state_flag[tcp_info->state] & TCP_FLAG_SYN) ||
            ((state_flag[tcp_info->state] & TCP_FLAG_FIN) && !tcp_info->sent_fin)) {
            data_length = 0;
            tcp_info->persist = false;
            goto build;
        } else {
            tr_warn("unexpected tcp_segment_start");
            return;
        }
    }

    data_length = buffer_data_length(tx_data);
    if (data_length > tcp_info->send_mss_eff) {
        data_length = tcp_info->send_mss_eff;
    }
    /* Can transmit up to SND.UNA+SND.WND-1 */
    /* Simplified by SND.UNA==SND.NXT */
    if (data_length > tcp_info->send_window) {
        data_length = tcp_info->send_window;
    }

    if (data_length == 0) {
        if (!timeout) {
            /* Start timer for probe */
            tcp_info->persist = true;
            tcp_info->timer = tcp_info->rto;
            tcp_info->retry = 0;
            return;
        } else {
            // Think - why actually 1? Given we don't resegmentise, and end
            // up committed to sending a 1-byte segment in this situation, may
            // as well just go for send_mss_eff or 64 for our probe. No rule
            // against it.
            data_length = 1;
        }
    } else {
        tcp_info->persist = false;
    }

build:
    /* Create buffer for message */
    buf = buffer_get(data_length);
    if (buf == NULL) {
        goto out;
    }

    buf->dst_sa.port = tcp_info->inet_pcb->remote_port;
    buf->dst_sa.addr_type = ADDR_IPV6;
    memcpy(buf->dst_sa.address, tcp_info->inet_pcb->remote_address, 16);
    buf->src_sa.port = tcp_info->inet_pcb->local_port;
    buf->src_sa.addr_type = ADDR_IPV6;
    memcpy(buf->src_sa.address, tcp_info->inet_pcb->local_address, 16);

    // Add data to message buffer, but do not remove it from socket buffer until
    // it is acked
    if (tx_data) {
        buffer_data_add(buf, buffer_data_pointer(tx_data), data_length);
    }

    buf->interface = tcp_info->interface;

    tcp_build(buf, tcp_info);
out:
    /* Start retransmit timer (also covers buffer failure) */
    tcp_info->timer = tcp_info->rto;
}


/**
 * \brief Function used for resenting segment after timeout.
 *
 * \param tcp_info identifies the connection
 */

static tcp_session_t *tcp_resend_segment(tcp_session_t *tcp_info)
{
    FUNC_ENTRY_TRACE("tcp_resend_segment()");

    if (tcp_info->retry >= (tcp_info->state < TCP_STATE_ESTABLISHED ? TCP_SYN_RETRIES : TCP_MAX_RETRIES)) {
        tr_debug("Too many retries");
        return tcp_session_delete_with_error(tcp_info, SOCKET_TX_FAIL);
    }

    if (++tcp_info->retry == TCP_PROBLEM_RETRIES) {
        socket_event_push(SOCKET_CONNECTION_PROBLEM, tcp_info->inet_pcb->socket, tcp_info->interface->id, NULL, 0);
        ipv6_neighbour_reachability_problem(tcp_info->inet_pcb->remote_address, tcp_info->interface->id);
    }

    tcp_segment_start(tcp_info, true);

    tr_debug("RE-TX, timer %d", tcp_info->timer);
    return tcp_info;
}

static uint16_t tcp_compute_window_incr(tcp_session_t *tcp_info)
{
    // Careful window adjustment (RFC 1122 et al) - don't move right edge
    // leftwards, and don't jump up in small steps
    int32_t window = sockbuf_space(&tcp_info->inet_pcb->socket->rcvq);
    if (window < 0) {
        window = 0;
    }
    if (window > 0xffff) {
        window = 0xffff;
    }
    if (window < (int32_t) (tcp_info->receive_adv - tcp_info->receive_next)) {
        // Don't move left
        return 0;
    }

    // Don't make small increments (avoid Silly Window Syndrome)
    uint16_t incr = tcp_info->receive_next + window - tcp_info->receive_adv;
    if (incr > 0) {
        if (incr < tcp_info->inet_pcb->socket->rcvq.data_byte_limit / 2 &&
            incr < tcp_info->send_mss_eff) {
            incr = 0;
        } else {
            tcp_info->receive_adv += incr;
        }
    }

    return incr;
}

void tcp_session_data_received(tcp_session_t *tcp_info)
{
    // If this read nudges up the window, send an ack
    // Layering all a little wonky here, but this should roughly work.
    // Revisit when delayed ack infrastructure in place.
    if (tcp_compute_window_incr(tcp_info)) {
        buffer_t *buf = tcp_ack_buffer(tcp_info, 0);
        tcp_build(buf, tcp_info);
    }
}

/**
 * \brief Function used for allocating buffer for ack segment.
 *
 * \param tcp_info identifies the connection
 * \param new_ack new bytes to be acknowledged
 *
 * \return allocated buffer on success
 * \return 0 pointer indicates an error
 */

static buffer_t *tcp_ack_buffer(tcp_session_t *tcp_info, uint16_t new_ack)
{
    /* make ack segment */

    buffer_t *new_buffer;
    FUNC_ENTRY_TRACE("tcp_ack_buffer() s=%d, new_ack=%d", tcp_info->inet_pcb->socket->id, new_ack);

    new_buffer = buffer_get(0);
    if (!new_buffer) {
        return NULL;
    }

    new_buffer->dst_sa.port = tcp_info->inet_pcb->remote_port;
    new_buffer->dst_sa.addr_type = ADDR_IPV6;
    memcpy(new_buffer->dst_sa.address, tcp_info->inet_pcb->remote_address, 16);
    new_buffer->src_sa.port = tcp_info->inet_pcb->local_port;
    new_buffer->src_sa.addr_type = ADDR_IPV6;
    memcpy(new_buffer->src_sa.address, tcp_info->inet_pcb->local_address, 16);

    new_buffer->interface = tcp_info->interface;
    new_buffer->info = (buffer_info_t)(B_DIR_DOWN | B_FROM_TCP);
    new_buffer->options.code = 0xff;
    tcp_info->receive_next += new_ack;

    return new_buffer;
}

/**
 * \brief Function used for saving segments needing to be acknowledged.
 *
 * \param buf buffer to be acknowledged
 * \param tcp_info identifies the connection
 */

static void tcp_uack_segment(buffer_t *buf, tcp_session_t *tcp_info, uint16_t header_length)
{
    FUNC_ENTRY_TRACE("tcp_uack_segment() s=%d", tcp_info->inet_pcb->socket->id);

    uint16_t seg_size = buffer_data_length(buf) - header_length;
    if (buf->options.code & TCP_FLAG_SYN) {
        seg_size++;
    }
    if (buf->options.code & TCP_FLAG_FIN) {
        seg_size++;
        if (tcp_info->sent_fin) {
            tr_err("sent 2 FINs");
        }
        tcp_info->sent_fin = true;
    }

    if (seg_size == 0) {
        return;
    }

    tcp_info->send_next += seg_size;
    if (tcp_info->timer == 0) {
        tcp_info->timer = tcp_info->rto;
    }

    tcp_info->busy = true;
}


/**
 * \brief Peer has acknowledged data.
 *
 * \param ack sequence being acknowledged
 * \param tcp_info identifies the connection
 *
 */

static void tcp_ack_segment(uint32_t ack, tcp_session_t *tcp_info)
{
    FUNC_ENTRY_TRACE("tcp_ack_segment() s=%d", (int)tcp_info->inet_pcb->socket->id);

    socket_t *so = tcp_info->inet_pcb->socket;

    uint32_t acked_bytes;
    acked_bytes = ack - tcp_info->send_unacknowledged;

    tr_debug("tcp_ack_segment() acked %"PRIu32, acked_bytes);

    // Do not allow to remove more than sent data from buffer
    if (acked_bytes > so->sndq.data_bytes) {
        acked_bytes = so->sndq.data_bytes;
    }
    sockbuf_drop(&so->sndq, acked_bytes);
    tr_debug("tcp_ack_segment() socket remove from buffer %"PRIu32" data to be sent %"PRIu32, acked_bytes, so->sndq.data_bytes);

    tcp_info->send_unacknowledged = ack;

    ipv6_neighbour_reachability_confirmation(tcp_info->inet_pcb->remote_address, tcp_info->interface->id);

    uint32_t remaining_bytes = so->sndq.data_bytes;

    if (tcp_info->state >= TCP_STATE_ESTABLISHED) {
        // Could consider not sending event if space below low water?
        socket_event_push(SOCKET_TX_DONE, so, tcp_info->interface->id, tcp_info, remaining_bytes);
    }

    // All data that has been send is acked so sent next segment
    if (ack == tcp_info->send_next) {

        // Made when all data has been acked
        // Could do this only periodically, based on a timer, for speed
        // Or, as RFC 4861 suggests, once per RTT. Which RTO calculation is,
        // so keep this with the RTO recalculation code.
        if (tcp_info->retry == 0 && !tcp_info->persist) {
            /* Update RTT - RFC 6298, using tricks in Van Jacoben's 1988 paper */
            /* Most variables held as scaled 16-bit positive signed integers */
            int16_t R = (int16_t) tcp_info->rto - (int16_t)tcp_info->timer;
            if (R < 0) {
                tr_err("R=%"PRId16, R);
                R = 0;
            }
            if (tcp_info->srtt8 == INT16_MAX) {
                tcp_info->srtt8 = R << 3; // srtt := R
                tcp_info->srttvar4 = R << (2-1); // rttvar := R / 2
            } else {
                /* 1/8 gain and scaling on smoothed RTT measurement */
                int16_t R_diff = R - (tcp_info->srtt8 >> 3);
                tcp_info->srtt8 += R_diff;
                if (R_diff < 0) {
                    R_diff = -R_diff;
                }
                /* 1/4 gain and scaling on smoothed RTTVAR measurement */
                int16_t V_diff = R_diff - (tcp_info->srttvar4 >> 2);
                tcp_info->srttvar4 += V_diff;
            }
            /* RTO = RTT + 4 * RTTVAR - rounds nicely as described by Van Jacobsen */
            tcp_info->rto = (tcp_info->srtt8 >> 3) + tcp_info->srttvar4;
            if (tcp_info->rto < TCP_MINIMUM_RTO) {
                tcp_info->rto = TCP_MINIMUM_RTO;
            }
            //tr_debug("R=%"PRId16" rto=%"PRIu16" srtt8=%"PRId16" rttvar4=%"PRId16, R, tcp_info->rto, tcp_info->srtt8, tcp_info->srttvar4);
            //tr_debug("R=%.2f rto=%.2f srtt=%.2f rttvar=%.2f", R * .150F, tcp_info->rto * .150F, tcp_info->srtt8 * (.15F/8), tcp_info->srttvar4 * (.15F/4));
        }
        tcp_info->retry = 0;
        tcp_info->timer = 0;

        if (remaining_bytes ||
            ((state_flag[tcp_info->state] & TCP_FLAG_FIN) && !tcp_info->sent_fin)) {
            tcp_segment_start(tcp_info, false);
        } else {
            tcp_info->busy = false;
        }
    }
}


/**
 * \brief Function called every time timer ticks.
 */

static void tcp_timer_handle(uint16_t ticksUpdate)
{
    arm_event_s event = {
        .receiver = socket_event_handler_id_get(),
        .sender = 0,
        .event_type = ARM_SOCKET_TCP_TIMER_CB,
        .event_data = ticksUpdate,
        .data_ptr = NULL,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
    };
    if (eventOS_event_send(&event) != 0) {
        tr_error("tcp_timer_handle(): event send failed");
    }
}

/**
 * \brief Function used for handling time events.
 */

void tcp_handle_time_event(uint16_t tickUpdate)
{
    if (tcp_session_count == 0) {
        return;
    }

    ns_list_foreach_safe(socket_t, socket, &socket_list) {
        if (!socket_is_ipv6(socket) || socket->type != SOCKET_TYPE_STREAM) {
            continue;
        }
        inet_pcb_t *inet_pcb = socket->inet_pcb;
        tcp_session_t *cur = inet_pcb->session;
        if (cur && cur->timer) {
            if (cur->timer > tickUpdate) {
                cur->timer -= tickUpdate;
            } else {
                cur->timer = 0;
                if (inet_pcb->socket->flags & SOCKET_LISTEN_STATE) {
                    //shouldn't happen
                } else if (cur->state == TCP_STATE_CLOSED) {
                    //tcp_timers_active &= ~the_bit;
                } else if (cur->state == TCP_STATE_TIME_WAIT) {
                    //tr_debug("Timewait --> Close");
                    tcp_session_delete(cur);
                } else {
                    if (cur->busy) {
                        cur->rto *= 2;
                        if (cur->rto > TCP_MAXIMUM_RTO) {
                            cur->rto = TCP_MAXIMUM_RTO;
                        }
                        cur = tcp_resend_segment(cur);
                    } else if (cur->state == TCP_STATE_FIN_WAIT_2) {
                        if (inet_pcb->socket->flags & SOCKET_FLAG_CLOSED) {
                            tr_debug("sTW No Fin from host yet"); // Timeout shouldn't happen if socket is still open, so logically no event needed
                            cur->state = TCP_STATE_TIME_WAIT;
                            cur->timer = TCP_TIME_WAIT_TO_CLOSE;
                        } else {
                            tr_err("FW2 timeout with socket open");
                        }
                    }
                }
            }
        }
    }

    // XXX is this right? Surely should check current session list.
    // Guess we just get one more tick though, as we won't re-set next time.
    protocol_timer_start(PROTOCOL_TIMER_TCP_TIM, tcp_timer_handle, TCP_TIMER_PERIOD);
}

/* find listen socket from socket instances */
static socket_t *tcp_find_listen_socket(const uint8_t addr[static 16], uint16_t port)
{
    FUNC_ENTRY_TRACE("tcp_find_listen_socket() %d", port);
    ns_list_foreach(socket_t, so, &socket_list) {
        if ((so->flags & SOCKET_LISTEN_STATE) &&
            socket_is_ipv6(so) &&
            so->inet_pcb->local_port == port &&
            (addr_ipv6_equal(so->inet_pcb->local_address, ns_in6addr_any) ||
             addr_ipv6_equal(so->inet_pcb->local_address, addr))
            ) {
            return so;
        }
    }

    return NULL;
}

/**
 * \brief Function used for sending data through TCP.
 *
 * \param buf buffer to be sent
 * \param tcp_info points to TCP session
 *
 * Input: data = TCP payload
 *        options.code = TCP flags (or 0 meaning "normal data from socket" or 0xFF meaning "ACK-only segment")
 *        src_sa and dst_sa need not be set
 * Output to IP down:
 *         data = IP payload
 *         src_sa and dst_sa addresses and ports filled in from session
 *         options.type = IPV6_NH_TCP
 *
 */

static void tcp_build(buffer_t *buf, tcp_session_t *tcp_info)
{
    uint16_t header_length = 20;
    uint8_t *ptr;

    if (!buf) {
        return;
    }

    if (!tcp_info) {
        buffer_free(buf);
        return;
    }

    FUNC_ENTRY_TRACE("tcp_build() s=%d", tcp_info->inet_pcb->socket->id);

    if (!tcp_info) {
        tr_error("DW No Session: dst: %s, src port: %d, dst port: %d", tr_ipv6(buf->dst_sa.address), buf->src_sa.port,
                buf->dst_sa.port);
        buffer_free(buf);
        return;
    }

    if (buffer_data_length(buf) != 0 && tcp_info->sent_fin) {
        tr_error("TCP:DW send fail by state %02x", tcp_info->state);
        buffer_free(buf);
        return;
    }

    //Set Flags
    if (buf->options.code) {
        /* options.code contains TCP flags when used with TCP */
        if (buf->options.code == 0xff) {
            /* code == 0xff if ACK only */
            buf->options.code = TCP_FLAG_ACK;
        }
        //tr_debug("options from icmp");
    } else {
        /* data send request from socket */
        buf->options.code = state_flag[tcp_info->state];
        uint16_t buf_len = buffer_data_length(buf);
        if (tcp_info->inet_pcb->socket->sndq.data_bytes <= buf_len) {
            /* push data if we have no more (RFC 1122 4.2.2.2) */
            if (buffer_data_length(buf) != 0) {
                buf->options.code |= TCP_FLAG_PSH;
            }
        } else {
            /* Close advances state even while outstanding data (more like BSD
             * than RFC 793), so make sure FIN flag only actually sent when no
             * more data */
            buf->options.code &= ~TCP_FLAG_FIN;
        }
    }

    if (buf->options.code != 0xff) {
        if (buf->options.code & TCP_FLAG_SYN) {
            header_length += 4;
        }
    }

    buf = buffer_headroom(buf, header_length);

    if (!buf) {
        tr_error("TCP_DW:HeadROOM Fail");
        return;
    }

    ptr = buffer_data_reserve_header(buf, header_length);

    ptr = common_write_16_bit(tcp_info->inet_pcb->local_port, ptr);
    ptr = common_write_16_bit(tcp_info->inet_pcb->remote_port, ptr);

    ptr = common_write_32_bit(tcp_info->send_next, ptr);
    ptr = common_write_32_bit(tcp_info->receive_next, ptr);

    *ptr++ = header_length << 2; /* data offset */

    *ptr++ = buf->options.code;

    tcp_compute_window_incr(tcp_info);

    ptr = common_write_16_bit(tcp_info->receive_adv - tcp_info->receive_next, ptr);

    ptr = common_write_32_bit(0, ptr); /* checksum and URG pointer */

    /* Advertise a minimal MSS based on minimum IPv6 MTU; keeping it simple for
     * now, rather than actually looking at the real link MTUs. See RFC 6991,
     * RFC 1191, etc.
     */
    if (buf->options.code & TCP_FLAG_SYN) {
        *ptr++ = TCP_OPTION_MSS;
        *ptr++ = 4; // option length
        ptr = common_write_16_bit(tcp_info->receive_mss, ptr);
    }

    memcpy(buf->dst_sa.address, tcp_info->inet_pcb->remote_address, 16);
    buf->dst_sa.port = tcp_info->inet_pcb->remote_port;
    buf->dst_sa.addr_type = ADDR_IPV6;
    memcpy(buf->src_sa.address, tcp_info->inet_pcb->local_address, 16);
    buf->src_sa.port = tcp_info->inet_pcb->local_port;
    buf->src_sa.addr_type = ADDR_IPV6;

    /* calculate checksum */
    common_write_16_bit(buffer_ipv6_fcf(buf, IPV6_NH_TCP), buffer_data_pointer(buf) + 16);

    tcp_uack_segment(buf, tcp_info, header_length);

    buf->info = (buffer_info_t)(B_FROM_TCP | B_TO_IPV6 | B_DIR_DOWN);
    buf->options.type = IPV6_NH_TCP;
    buf->options.code = 0;

    tcp_set_metadata(tcp_info->inet_pcb, buf);

#ifdef TCP_TEST
    if (tx_drops[tcp_info->state]) {
        tx_drops[tcp_info->state]--;
        tr_info("TX drop %s", tcp_state_name(tcp_info));
        socket_tx_buffer_event_and_free(buf, SOCKET_TX_FAIL);
        return;
    }
#endif

    //tr_debug("DW buf_len=%d", buffer_data_length(buf));
    protocol_push(buf);
}

static void tcp_session_established(protocol_interface_info_entry_t *cur, tcp_session_t *tcp_info)
{
    tr_debug("UP:sES");
    socket_t *so = tcp_info->inet_pcb->socket;
    socket_connection_complete(so, cur->id);
    tcp_info->state = TCP_STATE_ESTABLISHED;
    /* RFC 6298 - if timed out during connection, revert to conservative initial RTO for data */
    /* (What's the point - Karn's algorithm would cover this?) */
    if (tcp_info->retry > 0 && tcp_info->rto < TCP_INITIAL_CONSERVATIVE_RTO) {
        tcp_info->rto = TCP_INITIAL_CONSERVATIVE_RTO;
    }
}
/**
 * \brief Function that handles data coming from lower level to TCP.
 *
 * \param buf buffer received
 *
 * \return allocated pointer if acknowledgment needs to be sent
 * \return zero pointer, no further action required
 */

buffer_t *tcp_up(buffer_t *buf)
{
    uint8_t *ptr;
    protocol_interface_info_entry_t *cur;
    uint16_t data_offset;
    uint16_t mss_option = 536;
    uint16_t window_size;
    inet_pcb_t *inet_pcb;
    tcp_session_t *tcp_info;
    buffer_t *segment_ack = NULL;
    uint32_t ack_no;
    uint32_t seq_no;
    uint16_t seg_len;
    uint8_t flags;

    cur = buf->interface;

    /* Multicast source or link-layer destination already handled by IP */
    if (!cur || addr_is_ipv6_multicast(buf->dst_sa.address) || buf->options.ll_security_bypass_rx) {
        tr_error("TCP UP:Invalid");
        return buffer_free(buf);
    }

    if (buffer_data_length(buf) < 20) {
        tr_error("TCP UP:Too short");
        return buffer_free(buf);
    }

    if (buffer_ipv6_fcf(buf, IPV6_NH_TCP)) {
        tr_warn("TCP CKSUM ERROR!!!: src: %s, dst: %s", tr_ipv6(buf->src_sa.address), tr_ipv6(buf->dst_sa.address));
        protocol_stats_update(STATS_IP_CKSUM_ERROR, 1);
        return buffer_free(buf);
    }

    // save received port(s), seq_no and ack_no, data_offset, flags, window_size,
    ptr = buffer_data_pointer(buf);
    buf->src_sa.port = common_read_16_bit(ptr);
    ptr += 2;
    buf->dst_sa.port = common_read_16_bit(ptr);
    ptr += 2;
    seq_no = common_read_32_bit(ptr);
    ptr += 4;
    ack_no = common_read_32_bit(ptr);
    ptr += 4;

    data_offset = (*ptr++ >> 4) << 2;
    if (data_offset > buffer_data_length(buf)) {
        tr_error("TCP UP:Offset length fail");
        return buffer_free(buf);
    }

    buffer_data_strip_header(buf, data_offset);

    flags = *ptr++;
    window_size = common_read_16_bit(ptr);
    ptr += 2;

    // Skip over Checksum (already checked) and Urgent Pointer (ignored)
    ptr += 4;

    seg_len = buffer_data_length(buf);
    if (flags & TCP_FLAG_SYN) {
        seg_len++;
    }
    if (flags & TCP_FLAG_FIN) {
        seg_len++;
    }

    tr_debug("TCP_UP: dst_p=%d, src_p=%d, flags=%s", buf->dst_sa.port, buf->src_sa.port, trace_tcp_flags(flags));
    /* clear flags that will be ignored */
    flags &= ~(TCP_FLAG_CWR | TCP_FLAG_ECE | TCP_FLAG_URG);

    // Parse options
    bool malformed_options = false;
    if (data_offset > 20) {
        uint16_t options_len = data_offset - 20;
        while (options_len > 0) {
            uint8_t type = ptr[0];
            uint8_t len;
            if (type == TCP_OPTION_END) {
                break;
            } else if (type == TCP_OPTION_NOP) {
                len = 1;
            } else {
                if (options_len < 2) {
                    goto bad_opts;
                }
                len = ptr[1];
                if (len < 2) {
                    goto bad_opts;
                }
            }

            if (len > options_len) {
            bad_opts:
                malformed_options = true;
                tr_err("Malformed options");
                break;
            }

            if (type == TCP_OPTION_MSS && len == 4) {
                mss_option = common_read_16_bit(ptr + 2);
                tr_debug("MSS %"PRIu16, mss_option);
            } else if (type != TCP_OPTION_NOP){
                tr_info("Unsupported option %d", type);
            }
            ptr += len;
            options_len -= len;
        }
    }

    // find socket from existing connections based on local and remote addresses
    socket_t *so = socket_lookup_ipv6(IPV6_NH_TCP, &buf->dst_sa, &buf->src_sa, false);
    inet_pcb = so ? so->inet_pcb : NULL;
    tcp_info = inet_pcb ? inet_pcb->session : NULL;
    // if not found, and it's a SYN, look for a listening socket
    if (tcp_info == NULL && !malformed_options) find_listen: { // Can jump here from TIME-WAIT, with tcp_info set
        socket_t *listen_socket = tcp_find_listen_socket(buf->dst_sa.address, buf->dst_sa.port);
        if (listen_socket) {
            tr_debug("UP: Packet for LISTEN socket %d", listen_socket->id);
#ifdef TCP_TEST
            if (rx_drops[TCP_STATE_LISTEN]) {
                tr_info("RX drop LISTEN");
                rx_drops[TCP_STATE_LISTEN]--;
                return buffer_free(buf);
            }
#endif
            if (flags & TCP_FLAG_RST) {
                tr_warn("UP, RST to LISTEN");
                return buffer_free(buf);
            }
            if (flags & TCP_FLAG_ACK) {
                tr_warn("UP, ACK to LISTEN");
                return tcp_reset_response(NULL, 0, buf, ack_no, 0, TCP_FLAG_RST);
            }
            if (flags & TCP_FLAG_SYN) {
                socket_t *new_socket = socket_new_incoming_connection(listen_socket);
                if (!new_socket) {
                    tr_error("Couldn't allocate socket");
                    return buffer_free(buf);
                }
                inet_pcb = new_socket->inet_pcb;
                /* new socket has an inet PCB cloned from the listening one - lock down addresses+ports */
                memcpy(inet_pcb->remote_address, buf->src_sa.address, 16);
                memcpy(inet_pcb->local_address, buf->dst_sa.address, 16);
                inet_pcb->local_port = buf->dst_sa.port;
                inet_pcb->remote_port = buf->src_sa.port;
                tr_debug("remote=[%s]:%u", trace_ipv6(inet_pcb->remote_address), inet_pcb->remote_port);
                tr_debug("local=[%s]:%u", trace_ipv6(inet_pcb->local_address), inet_pcb->local_port);
                /* If someone sets a fixed (non-0) flow label on a listening socket, all incoming
                 * connections will end up using the same flow label, which is dumb, but user's fault. */
                /* Same logic as connect(). */
                if (inet_pcb->flow_label == IPV6_FLOW_AUTOGENERATE ||
                        (inet_pcb->flow_label == IPV6_FLOW_UNSPECIFIED && ipv6_flow_auto_label)) {
                    inet_pcb->flow_label = ipv6_flow_random();
                }
                tcp_info = tcp_session_ptr_allocate(inet_pcb, tcp_info);
                if (!tcp_info) {
                    tr_error("Couldn't allocate TCP session");
                    socket_release(new_socket);
                    return buffer_free(buf);
                }
                // save source port and address to tcp session data
                tcp_info->interface = cur;

                if (tcp_info->interface->nwk_id == IF_6LoWPAN) {
                    if (tcp_info->receive_mss > TCP_LOWPAN_MSS_LIMIT) {
                        tcp_info->receive_mss = TCP_LOWPAN_MSS_LIMIT;
                    }
                }

                tcp_info->receive_next = seq_no;
                tcp_info->receive_adv = seq_no;
                tcp_info->send_window = window_size;
                tcp_info->send_mss_peer = mss_option;
                tcp_rethink_mss(tcp_info);
                buffer_free(buf);
                // Acknowledge 1 byte (the SYN) - ignore anything further
                buf = tcp_ack_buffer(tcp_info, 1);
                if (!buf) {
                    tr_error("Couldn't create SYN-ACK");
                    tcp_session_delete(tcp_info);
                    socket_release(new_socket);
                    return NULL;
                }
                buf->options.code = TCP_FLAG_SYN | TCP_FLAG_ACK;

                tr_debug("UP:sSR");
                tcp_info->state = TCP_STATE_SYN_RECEIVED;
                tcp_info->passive_open = true;
                tcp_build(buf, tcp_info);
                return NULL;
            } // SYN
            // Not RST, ACK or SYN
            tr_warn("UP, no flags to LISTEN");
            return buffer_free(buf);
        } // Listening socket found

        // Wipe out tcp_info, for the case where we jumped into this block from TIME-WAIT, so we send
        // a reset (for this new connection attempt, with a higher sequence number, for which
        // we have no listening socket).
        tcp_info = NULL;
    } // No existing session

    if (tcp_info == NULL || tcp_info->state == TCP_STATE_CLOSED || malformed_options) {
        tr_info("No tcp_info for port=%d from %s", buf->dst_sa.port, trace_ipv6(buf->src_sa.address));
        if (flags & TCP_FLAG_RST) {
            tr_debug("RST to CLOSED");
            return buffer_free(buf);
        }
        if (!(flags & TCP_FLAG_ACK)) {
            return tcp_reset_response(tcp_info, SOCKET_CONNECTION_RESET, buf, 0, seq_no + seg_len, TCP_FLAG_RST|TCP_FLAG_ACK);
        }
        return tcp_reset_response(tcp_info, SOCKET_CONNECTION_RESET, buf, ack_no, 0, TCP_FLAG_RST);
    }

#ifdef TCP_TEST
    if (rx_drops[tcp_info->state]) {
        tr_info("RX drop %s", tcp_state_name(tcp_info));
        rx_drops[tcp_info->state]--;
        return buffer_free(buf);
    }
#endif

    buffer_socket_set(buf, so);

    tr_debug("UP: sock=%d: state=%s", so->id, tcp_state_name(tcp_info));

    if (tcp_info->state == TCP_STATE_SYN_SENT) {
        if (flags & TCP_FLAG_ACK) {
            if (ack_no != tcp_info->send_next) {
                if (flags & TCP_FLAG_RST) {
                    return buffer_free(buf);
                }
                return tcp_reset_response(NULL, 0, buf, ack_no, 0, TCP_FLAG_RST);
            }
        }
        if (flags & TCP_FLAG_RST) {
            if (flags & TCP_FLAG_ACK) {
                tcp_session_delete_with_error(tcp_info, SOCKET_CONNECT_FAIL);
            }
            return buffer_free(buf);
        }
        if (flags & TCP_FLAG_SYN) {
            tcp_info->receive_next = seq_no;
            tcp_info->receive_adv = seq_no;
            tcp_info->send_mss_peer = mss_option;
            tcp_rethink_mss(tcp_info);
            if (flags & TCP_FLAG_ACK) {
                tcp_ack_segment(ack_no, tcp_info);
                tcp_info->send_wl1 = seq_no; // RFC 1122 4.2.2.20(c)
                tcp_info->send_wl2 = ack_no;
                tcp_info->send_window = window_size;
                tcp_session_established(cur, tcp_info);
                goto syn_sent_to_established;
            } else {
                tr_debug("UP:sSR");
                tcp_info->state = TCP_STATE_SYN_RECEIVED;
                segment_ack = tcp_ack_buffer(tcp_info, 1);
                if (segment_ack) {
                    buffer_socket_set(segment_ack, buf->socket);
                    segment_ack->options.code = TCP_FLAG_SYN | TCP_FLAG_ACK;
                    tcp_build(segment_ack, tcp_info);
                }
                buffer_free(buf);
                return NULL;
            }
        }
    } // state == TCP_STATE_SYN_SENT

    // Early escape from TIME-WAIT: processing of SYN segments as per RFC 6191
    // (simple non-timestamp case)
    if ((flags & (TCP_FLAG_SYN|TCP_FLAG_FIN|TCP_FLAG_ACK|TCP_FLAG_RST)) == TCP_FLAG_SYN && tcp_info->state == TCP_STATE_TIME_WAIT) {
        if (tcp_seq_ge(seq_no, tcp_info->receive_next)) {
            goto find_listen;
        } else {
            return buffer_free(buf);
        }
    }

    // Window checks
    int32_t receive_window = sockbuf_space(&so->rcvq);
    if (receive_window < 0) {
        receive_window = 0;
    }
    if (receive_window < (int32_t) (tcp_info->receive_adv - tcp_info->receive_next)) {
        receive_window = tcp_info->receive_adv - tcp_info->receive_next;
    }

    bool seq_ok = false;
    if (receive_window == 0) {
        if (seg_len == 0 && seq_no == tcp_info->receive_next) {
            seq_ok = true;
        }
    } else {
        if (tcp_seq_in_range(tcp_info->receive_next, seq_no, tcp_info->receive_next + receive_window - 1)) {
            seq_ok = true;
        } else if (seg_len > 0 && tcp_seq_in_range(tcp_info->receive_next, seq_no + seg_len - 1, tcp_info->receive_next + receive_window - 1)) {
            seq_ok = true;
        }
    }

    if (!seq_ok) {
        tr_debug("Out-of-window seq_no=%"PRIu32", seg_len=%"PRIu16", window [%"PRIu32" - %"PRIu32")", seq_no, seg_len, tcp_info->receive_next, tcp_info->receive_next + receive_window);
        buffer_free(buf);
        buf = tcp_ack_buffer(tcp_info, 0);
        tcp_build(buf, tcp_info);
        return NULL;
    }

    /* Strip data preceding the window */
    if (tcp_seq_lt(seq_no, tcp_info->receive_next)) {
        uint16_t excess = tcp_info->receive_next - seq_no;
        tr_debug("Strip preceding excess %d", excess);
        /* Strip SYN "byte" */
        if (flags & TCP_FLAG_SYN) {
            flags &= ~TCP_FLAG_SYN;
            seg_len--;
            excess--;
            seq_no++;
        }
        if (excess) {
            buffer_data_strip_header(buf, excess);
            seg_len -= excess;
            seq_no += excess;
        }
    }

    /* Strip data following the window */
    if (tcp_seq_gt(seq_no + buffer_data_length(buf), tcp_info->receive_next + receive_window)) {
        uint16_t excess = (seq_no + buffer_data_length(buf)) - (tcp_info->receive_next + receive_window);
        tr_debug("Strip trailing excess %d", excess);
        /* Strip FIN "byte" */
        if (flags & TCP_FLAG_FIN) {
            flags &= ~TCP_FLAG_FIN;
            seg_len--;
        }
        buf->buf_end -= excess;
        seg_len -= excess;
    }

    if (flags & TCP_FLAG_RST) {
        switch (tcp_info->state) {
            case TCP_STATE_SYN_RECEIVED:
                if (tcp_info->passive_open) {
                    tcp_session_delete_with_error(tcp_info, 0);
                } else {
                    tcp_session_delete_with_error(tcp_info, SOCKET_CONNECT_FAIL);
                }
                break;
            case TCP_STATE_ESTABLISHED:
            case TCP_STATE_FIN_WAIT_1:
            case TCP_STATE_FIN_WAIT_2:
            case TCP_STATE_CLOSE_WAIT:
                tcp_session_delete_with_error(tcp_info, SOCKET_CONNECTION_RESET);
                break;
            case TCP_STATE_CLOSING:
            case TCP_STATE_LAST_ACK:
                tcp_session_delete(tcp_info);
                break;
            case TCP_STATE_TIME_WAIT: // RFC 1337
            default:
                break;
        }
        return buffer_free(buf);
    }

    if (flags & TCP_FLAG_SYN) {
        tr_debug("unexpected SYN");
        if (tcp_info->state == TCP_STATE_SYN_RECEIVED && tcp_info->passive_open) {
            // RFC 1122 4.2.2.20(e)
            tcp_session_delete_with_error(tcp_info, 0); // really no reset sent?
            return buffer_free(buf);
        }
        if (!(flags & TCP_FLAG_ACK)) {
            return tcp_reset_response(tcp_info, SOCKET_CONNECTION_RESET, buf, 0, seq_no + seg_len, TCP_FLAG_RST|TCP_FLAG_ACK);
        }
        return tcp_reset_response(tcp_info, SOCKET_CONNECTION_RESET, buf, ack_no, 0, TCP_FLAG_RST);
    }

    if (!(flags & TCP_FLAG_ACK)) {
        tr_debug("ACK not set");
        return buffer_free(buf);
    }

    if (tcp_info->state == TCP_STATE_SYN_RECEIVED) {
        if (ack_no == tcp_info->send_next) {
            tcp_info->send_wl1 = seq_no;  // RFC 1122 4.2.2.20(f)
            tcp_info->send_wl2 = ack_no;
            tcp_info->send_window = window_size;
            tcp_session_established(cur, tcp_info);
        } else {
            return tcp_reset_response(NULL, 0, buf, ack_no, 0, TCP_FLAG_RST);
        }
    }

    if (tcp_seq_lt(ack_no, tcp_info->send_unacknowledged)) {
        tr_debug("Already acked ack_no=%"PRIu32", in-flight [%"PRIu32" - %"PRIu32")", ack_no, tcp_info->send_unacknowledged, tcp_info->send_next);
    } else if (tcp_seq_gt(ack_no, tcp_info->send_next)) {
        tr_debug("Future ack ack_no=%"PRIu32", in-flight [%"PRIu32" - %"PRIu32")", ack_no, tcp_info->send_unacknowledged, tcp_info->send_next);
        buffer_free(buf);
        // Generating this ack can lead to an ack storm if we're somehow out of sync...
        // Seems to be a TCP flaw...
        buf = tcp_ack_buffer(tcp_info, 0);
        tcp_build(buf, tcp_info);
        return NULL;
    } else /* SND.UNA <= SEG.ACK <= SND.NXT */ {
        /* Update window, if packet not older than last window information */
        if (tcp_seq_gt(seq_no, tcp_info->send_wl1) ||
                (seq_no == tcp_info->send_wl1 && tcp_seq_ge(ack_no, tcp_info->send_wl2))) {
            if (tcp_info->send_window != window_size) {
                tr_debug("Send window update %"PRIu16"->%"PRIu16, tcp_info->send_window, window_size);
            }
            tcp_info->send_wl1 = seq_no;
            tcp_info->send_wl2 = ack_no;
            tcp_info->send_window = window_size;
            // Watch out for shrinking right edge
            if (window_size == 0 && ack_no != tcp_info->send_next) {
                tcp_info->persist = true;
            }
        }
        if (ack_no != tcp_info->send_unacknowledged) {
            tr_debug("New ack_no=%"PRIu32", in-flight [%"PRIu32" - %"PRIu32")", ack_no, tcp_info->send_unacknowledged, tcp_info->send_next);
            tcp_ack_segment(ack_no, tcp_info);
        } else if (tcp_info->persist) {
            tcp_info->retry = 0;
            if (window_size != 0) {
                tcp_info->persist = false;
                // Got a pure window update. (Re)transmit now
                tcp_resend_segment(tcp_info);
            } else {
                // Let the retransmit timer keep on running, which will trigger
                // another persist probe at RTO after the last one (effectively
                // ignoring this ack for timing purposes), but reset the retry
                // count so we don't give up in resend_segment.
            }
        }

        bool fin_acknowledged = tcp_info->sent_fin && tcp_info->send_unacknowledged == tcp_info->send_next;
        switch (tcp_info->state) {
            case TCP_STATE_FIN_WAIT_1:
                if (fin_acknowledged) {
                    tcp_info->state = TCP_STATE_FIN_WAIT_2;
                    // Only time out FIN_WAIT_2 if no longer have a socket
                    if (so->flags & SOCKET_FLAG_CLOSED) {
                        tcp_info->timer = TCP_FIN_WAIT_2_TIMEOUT;
                    } else {
                        tcp_info->timer = 0;
                    }
                    tr_debug("UP:sF2");
                }
                break;
            case TCP_STATE_CLOSING:
                if (fin_acknowledged) {
                    tcp_info->state = TCP_STATE_TIME_WAIT;
                    tr_debug("UP:sTW");
                    tcp_info->timer = TCP_TIME_WAIT_TO_CLOSE;
                }
                break;
            case TCP_STATE_LAST_ACK:
                if (fin_acknowledged) {
                    socket_event_push(SOCKET_CONNECT_CLOSED, so, cur->id, NULL, 0);
                    tr_debug("Last ACK remove Session");
                    tcp_session_delete(tcp_info);
                    return buffer_free(buf);
                }
                break;
            case TCP_STATE_TIME_WAIT:
                tcp_info->timer = TCP_TIME_WAIT_TO_CLOSE;
                break;
        }
    }

syn_sent_to_established:
    if (flags & TCP_FLAG_URG) {
        /* No handling of urgent */
    }

    /* This is the point we're actually processing incoming text or FINs. All of the
     * above is done as long as in-window, but from here on we need the correct sequence.
     * seg_len is length including FIN marker.
     */
    if (seg_len > 0 && seq_no != tcp_info->receive_next) {
        tr_debug("Out-of-order data");
        buffer_t *ack_buf = tcp_ack_buffer(tcp_info, 0);
        tcp_build(ack_buf, tcp_info);
        return buffer_free(buf);
    }

    /* Data only processed in some states - other states silently ignore */
    if (buffer_data_length(buf) &&
            (tcp_info->state == TCP_STATE_ESTABLISHED || tcp_info->state == TCP_STATE_FIN_WAIT_1 || tcp_info->state == TCP_STATE_FIN_WAIT_2)) {
        // This handles closed sockets (see RFC 1122 4.2.2.13 and RFC2525 2.16)
        // and also extends it to read shutdown (like Windows, apparently, but not Linux or BSD)
        if (so->flags & SOCKET_FLAG_CANT_RECV_MORE) {
            tr_warn("Excess data we can't receive - resetting");
            return tcp_reset_response(tcp_info, SOCKET_CONNECTION_RESET, buf, ack_no, 0, TCP_FLAG_RST);
        }
        buf->info = (buffer_info_t)(B_FROM_TCP | B_TO_NONE | B_DIR_UP);

        //tr_debug("data up");
        sockbuf_append_and_compress(&so->rcvq, buf);
        buf = NULL;
        if ((flags & (TCP_FLAG_FIN|TCP_FLAG_PSH)) || sockbuf_space(&so->rcvq) <= (int32_t) (so->rcvq.data_byte_limit / 2)) {
            socket_data_queued_event_push(so);
        }
    }

    // Original buffer no longer required
    if (buf) {
        buf = buffer_free(buf);
    }

    if (flags & TCP_FLAG_FIN) {
        tr_debug("fin");
        switch (tcp_info->state) {
            case TCP_STATE_SYN_RECEIVED: // can't happen?
            case TCP_STATE_ESTABLISHED:
                tcp_info->state = TCP_STATE_CLOSE_WAIT;
                tcp_info->timer = 0;
                tr_debug("UP:sCW");
                socket_cant_recv_more(so, cur->id);
                break;
            case TCP_STATE_FIN_WAIT_1:
                if (tcp_info->send_unacknowledged == tcp_info->send_next) {
                    tcp_info->state = TCP_STATE_TIME_WAIT;
                    tcp_info->timer = TCP_TIME_WAIT_TO_CLOSE;
                    tr_debug("UP:sTW");
                } else {
                    tcp_info->state = TCP_STATE_CLOSING;
                    tr_debug("UP:sCl");
                }
                socket_cant_recv_more(so, cur->id);
                break;
            case TCP_STATE_FIN_WAIT_2:
                tcp_info->state = TCP_STATE_TIME_WAIT;
                tr_debug("UP:sTW");
                tcp_info->timer = TCP_TIME_WAIT_TO_CLOSE;
                socket_cant_recv_more(so, cur->id);
                socket_event_push(SOCKET_CONNECT_CLOSED, so, cur->id, 0, 0);
                break;
            case TCP_STATE_TIME_WAIT:
                tcp_info->timer = TCP_TIME_WAIT_TO_CLOSE;
                break;
            default:
                tr_debug("UP:fin, other state");
                // No action
                break;
        }
    }

    // if there is new data to be acknowledged then send ack
    if (seg_len > 0) {
        segment_ack = tcp_ack_buffer(tcp_info, seg_len);
        tcp_build(segment_ack, tcp_info);
    }
    return buf;
}

const char *tcp_state_name(const tcp_session_t *tcp_info)
{
    switch (tcp_info->state) {
        case TCP_STATE_LISTEN:
             return "LISTEN";
        case TCP_STATE_SYN_SENT:
            return "SYN-SENT";
        case TCP_STATE_SYN_RECEIVED:
            return "SYN-RECEIVED";
        case TCP_STATE_ESTABLISHED:
            return "ESTABLISHED";
        case TCP_STATE_CLOSE_WAIT:
            return "CLOSE-WAIT";
        case TCP_STATE_LAST_ACK:
            return "LAST-ACK";
        case TCP_STATE_FIN_WAIT_1:
            return "FIN-WAIT-1";
        case TCP_STATE_FIN_WAIT_2:
            return "FIN-WAIT-2";
        case TCP_STATE_CLOSING:
            return "CLOSING";
        case TCP_STATE_TIME_WAIT:
            return "TIME-WAIT";
        default:
            return "?";
    }
}
#endif /*NO_TCP*/
/* end of file tcp.c */
