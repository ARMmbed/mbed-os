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

#ifndef TCP_H_
#define TCP_H_


#define TCP_UNACK_MAX 3
#define TCP_WINDOW_SIZE 300
#define TCP_ETHERNET_WINDOW_SIZE 1440
#define TCP_TIMER_PERIOD 150 // milliseconds

#define TCP_LOWPAN_MSS_LIMIT 300

#define TCP_MAX_RX_QLEN 1024

// Times in TCP_TIMER_PERIOD units
#define TCP_TIME_WAIT_TO_CLOSE 1600     // 4 minutes
#define TCP_FIN_WAIT_2_TIMEOUT 500      // 75s
#define TCP_MINIMUM_RTO 8               // 1.05s + 1 tick round-up
#define TCP_INITIAL_RTO 8               // 1.05s (RFC6298) + 1 tick round-up
#define TCP_INITIAL_CONSERVATIVE_RTO 21 // 3s + 1 tick round-up
#define TCP_MAXIMUM_RTO 400             // 1 minute
#define TCP_MAX_RETRIES 6               // if min RTO, 1s+2s+4s+8s+16s+32s(+60s wait) = 123s - RFC1122 says at least 100 seconds
#define TCP_SYN_RETRIES 7               // retry at 1s+2s+4s+8s+16s+32s+60s(+60s wait)  = 3 min 3s - RFC1122 says at least 3 minutes
#define TCP_PROBLEM_RETRIES 3           // report connection difficulties - RFC1122 says at least 3 retries

// maximum RTO should be kept below 4000 to avoid 16-bit calculation overflow

#define TCP_FLAG_FIN 1
#define TCP_FLAG_SYN 2
#define TCP_FLAG_RST 4
#define TCP_FLAG_PSH 8
#define TCP_FLAG_ACK 16
#define TCP_FLAG_URG 32
#define TCP_FLAG_ECE 64
#define TCP_FLAG_CWR 128

#define TCP_OPTION_END 0
#define TCP_OPTION_NOP 1
#define TCP_OPTION_MSS 2

// State numbering must remain fixed, as TCP test API uses it,
// and we don't expose it in public headers. The chosen
// numbering is from the SNMP MIB - RFC 4022.
#define TCP_STATE_CLOSED        1
#define TCP_STATE_LISTEN        2
#define TCP_STATE_SYN_SENT      3
#define TCP_STATE_SYN_RECEIVED  4
#define TCP_STATE_ESTABLISHED   5
#define TCP_STATE_FIN_WAIT_1    6
#define TCP_STATE_FIN_WAIT_2    7
#define TCP_STATE_CLOSE_WAIT    8
#define TCP_STATE_LAST_ACK      9
#define TCP_STATE_CLOSING       10
#define TCP_STATE_TIME_WAIT     11
#define TCP_STATES              12

struct inet_pcb_s;

typedef enum tcp_error_ {

    TCP_ERROR_NO_ERROR,
    TCP_ERROR_BUFFER_ALLOCATION_ERROR,
    TCP_ERROR_SOCKET_ALLOCATION_ERROR,
    TCP_ERROR_SOCKET_NOT_FOUND,
    TCP_ERROR_SECURE_ALLOCATION_ERROR,
    TCP_ERROR_WRONG_STATE,
    TCP_ERROR_ADDR_SELECTION,
    TCP_SESSION_REMOVED

} tcp_error;

typedef struct tcp_session_t {
    struct inet_pcb_s *inet_pcb;
    protocol_interface_info_entry_t *interface;
    uint32_t   send_unacknowledged;
    uint32_t   send_next;
    uint32_t   send_wl1;        // segment seq number at last window update
    uint32_t   send_wl2;        // segment ack number at last window update
    uint16_t   send_window;
    uint16_t   send_mss_peer;   // peer's advertised mss (RFC 5681 RMSS)
    uint16_t   send_mss_eff;    // effective send MSS (RFC 1122 Eff.snd.MSS, RFC 5681 SMSS)
    uint16_t   receive_mss;
    uint32_t   receive_next;    // sequence number
    uint32_t   receive_adv;     // advertised window right edge
    uint16_t   timer;
    uint8_t    retry;
    bool       passive_open : 1;
    uint8_t    state;
    bool       busy : 1;
    bool       persist : 1;
    bool       sent_fin : 1;
    uint16_t   rto;
    int16_t    srtt8;       // these should go into destination cache
    int16_t    srttvar4;
} tcp_session_t;

#ifdef NO_TCP
NS_DUMMY_DEFINITIONS_OK
#undef TCP_TEST
#define tcp_info(pcb) ((struct tcp_session_t *)NULL)
#define tcp_buf_save(buf) (buf)
#define tcp_up(buf) buffer_free(buf)
#define tcp_session_ptr_allocate(socket_id) ((tcp_session_t*) NULL)
#define tcp_socket_released(tcp_info) ((void) 0)
#define tcp_state_name(tcp_session) "NO_TCP"
#define tcp_handle_time_event(tickUpdate) ((void) 0)
#define tcp_session_data_received(tcp_info)  ((void) 0)
#define tcp_session_close(tcp_info) TCP_ERROR_SOCKET_NOT_FOUND
#define tcp_session_shutdown_read(tcp_info) TCP_ERROR_SOCKET_NOT_FOUND
#else
#define tcp_info(pcb) ((struct tcp_session_t *)((pcb)->session))
extern tcp_error tcp_session_open(tcp_session_t *tcp_session);
extern tcp_error tcp_session_close(tcp_session_t *tcp_session);
extern tcp_error tcp_session_shutdown_read(tcp_session_t *tcp_session);
extern tcp_error tcp_session_abort(tcp_session_t *tcp_session);
extern tcp_error tcp_session_send(tcp_session_t *tcp_info, buffer_t *buf);
extern void tcp_session_data_received(tcp_session_t *tcp_info);
extern buffer_t *tcp_up(buffer_t *buf);
extern tcp_session_t *tcp_session_ptr_allocate(struct inet_pcb_s *inet_pcb, tcp_session_t *from_time_wait);
extern tcp_session_t *tcp_session_ptr_free(tcp_session_t *tcp_info);
extern void tcp_socket_released(tcp_session_t *tcp_info);
const char *tcp_state_name(const tcp_session_t *tcp_info);

/**
 * \brief Function used for handling time events.
 */

extern void tcp_handle_time_event(uint16_t tickUpdate);
#endif // NO_TCP

#ifdef TCP_TEST
extern int8_t tcp_test_drop_tx(int state, uint8_t count);
extern int8_t tcp_test_drop_rx(int state, uint8_t count);
extern void tcp_test_drop_reset(void);
#else
#define tcp_test_drop_tx(state, count) ((int8_t) -1)
#define tcp_test_drop_rx(state, codunt) ((int8_t) -1)
#define tcp_test_drop_reset() ((void) 0)
#endif

#endif /* TCP_H_ */
