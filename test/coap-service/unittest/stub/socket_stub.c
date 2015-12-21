/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */
#include <stdint.h>
#include <inttypes.h>
#include "socket.h"
#include "ns_error_types.h"
#include "buffer.h"
#include "socket.h"
#include "sec_lib_definitions.h"
#include "tcp.h"
#include "socket_stub.h"

NS_LARGE socket_t socket_instance[SOCKETS_MAX] = {0};
socket_stub_data_t socket_stub_data = {
    0,  // socket_id
    eOK // err code
};

inet_pcb_t inet_pcb[SOCKETS_MAX];

static buffer_t stub_socket_buffer = {0};

socket_stub_data_t *socket_stub_get_test_data(void)
{
    socket_stub_clear_sockets();
    socket_stub_data.socket_id = 0;
    socket_stub_data.err = eOK;
    return &socket_stub_data;
}

void socket_stub_clear_sockets()
{
    int i;
    for (i = 0; i < SOCKETS_MAX; i++) {
        memset(&socket_instance[i], 0, sizeof(socket_t));
        memset(&inet_pcb[i], 0, sizeof(inet_pcb_t));
    }
}

void socket_init(void)
{
}

error_t socket_release(int8_t sid)
{
    return socket_stub_data.err;
}

error_t socket_port_validate(uint16_t port, uint8_t socket_type)
{
    return socket_stub_data.err;
}

error_t socket_create(int8_t *sid, uint16_t port, uint8_t type, void (*passed_fptr)(void *), uint8_t buffer_type)
{
    if (eOK == socket_stub_data.err) {
        *sid = socket_stub_data.socket_id;
        socket_instance[socket_stub_data.socket_id].type = type;
        socket_instance[socket_stub_data.socket_id].fptr = passed_fptr;
        socket_instance[socket_stub_data.socket_id].inet_pcb = &inet_pcb[socket_stub_data.socket_id];
    }
    return socket_stub_data.err;
}

int8_t socket_buffer_sendto(int8_t sid, buffer_t *buf)
{
    return 0;
}

buffer_t *socket_buffer_read(int8_t sid)
{
    return &stub_socket_buffer;
}

void *socket_event_session_read(int8_t sid)
{
    return NULL;
}

void socket_set_buffer_hop_limit(int8_t sid, buffer_t *buf)
{
}

struct protocol_interface_info_entry *socket_interface_determine(int8_t socket_id, buffer_t *buf) {
    return NULL;
}

buffer_t *socket_tx_buffer_check(buffer_t *buf, uint8_t status)
{
    return NULL;
}

