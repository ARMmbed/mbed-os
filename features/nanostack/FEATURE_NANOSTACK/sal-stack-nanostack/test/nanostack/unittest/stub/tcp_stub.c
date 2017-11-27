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
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "randLIB.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/tcp.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "tcp_stub.h"
#include "socket_api.h"
#include "socket.h"
#include "sockbuf.h"

void tcp_timer_handle(uint16_t ticksUpdate);
void tcp_segment_start(tcp_session_t *tcp_info);

buffer_t *tcp_up(buffer_t *buf);
void tcp_uack_segment(buffer_t *buf, tcp_session_t *tcp_info, uint16_t header_length);


tcp_stub_data_t tcp_stub;

tcp_error tcp_session_close(tcp_session_t *tcp_info)
{
    return tcp_stub.tcp_error_value;
}

tcp_session_t *tcp_session_delete_with_error(tcp_session_t *tcp_info, uint8_t error)
{
    return tcp_stub.tcp_session_ptr;
}

tcp_error tcp_session_open(tcp_session_t *tcp_session)
{
    return tcp_stub.tcp_error_value;
}

void tcp_segment_start(tcp_session_t *tcp_info)
{
}

buffer_t *tcp_build_reset_packet(const sockaddr_t *dst_addr, const sockaddr_t *src_addr, uint32_t seq, uint32_t ack, uint8_t flag)
{
    return tcp_stub.buffer_ptr;
}

tcp_session_t *tcp_resend_segment(tcp_session_t *tcp_info)
{
    return tcp_stub.tcp_session_ptr;
}

buffer_t *tcp_ack_buffer(tcp_session_t *tcp_info, uint16_t new_ack)
{
    return tcp_stub.buffer_ptr;
}

void tcp_uack_segment(buffer_t *buf, tcp_session_t *tcp_info, uint16_t header_length)
{
}

buffer_t *tcp_buf_save(buffer_t *buf)
{
    return tcp_stub.buffer_ptr;
}

tcp_session_t *tcp_ack_segment(uint32_t ack, tcp_session_t *tcp_info)
{
    return tcp_stub.tcp_session_ptr;
}

tcp_error tcp_session_accept(tcp_session_t *tcp_info)
{
    return tcp_stub.tcp_error_value;
}

tcp_session_t *tcp_session_ptr_free(tcp_session_t *tcp_info)
{
    return tcp_stub.tcp_session_ptr;
}

void tcp_timer_handle(uint16_t ticksUpdate)
{
}

void tcp_handle_time_event(uint16_t tickUpdate)
{
}

tcp_error tcp_session_shutdown_read(tcp_session_t *tcp_session)
{
    return tcp_stub.tcp_error_value;
}
tcp_error tcp_session_send(tcp_session_t *tcp_info, buffer_t *buf)
{
    return tcp_stub.tcp_error_value;
}

void tcp_socket_released(tcp_session_t *cur)
{
}

const char *tcp_state_name(const tcp_session_t *tcp_info)
{
    return tcp_stub.state;
}

buffer_t *tcp_up(buffer_t *buf)
{
    return tcp_stub.buffer_ptr;
}

tcp_session_t *tcp_session_ptr_allocate(struct inet_pcb_s *inet_pcb, tcp_session_t *from_time_wait)
{
    return tcp_stub.tcp_session_ptr;
}

void tcp_session_data_received(tcp_session_t *tcp_info)
{

}

/* end of file tcp.c */
