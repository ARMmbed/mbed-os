/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#include <stdint.h>
#include <inttypes.h>
#include "net_interface.h"
#include "buffer.h"
#include "sec_lib_definitions.h"
#include "tcp.h"

static tcp_session_t stub_tcp_session;

void tcp_segment_init(tcp_session_t *tcp_info, buffer_t *buf)
{
}

tcp_error tcp_close(tcp_session_t *tcp_session, uint8_t close_tls)
{
    return TCP_ERROR_NO_ERROR;
}

tcp_session_t *tcp_session_ptr_allocate(uint8_t socket_id)
{

    return &stub_tcp_session;
}

tcp_session_t *tcp_session_ptr_get(sockaddr_t *addr_ptr, uint8_t socket_id)
{
    return &stub_tcp_session;
}

int8_t tcp_session_removed(tcp_session_t *cur)
{
    return 0;
}

tcp_error tcp_socket_connect(tcp_session_t *tcp_session, sockaddr_t *dst_addr)
{
    return TCP_ERROR_NO_ERROR;
}

int8_t tcp_socket_tx_check(tcp_session_t *tcp_info)
{
    return 0;
}

buffer_t *tcp_tls_encrypt(buffer_t *buf, uint8_t socket_id)
{
    return NULL;
}

tcp_error tcp_socket_close(tcp_session_t *tcp_session) {
    return TCP_ERROR_NO_ERROR;
}

