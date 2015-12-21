/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#ifndef __COAP_CONNECTION_HANDLER_H__
#define __COAP_CONNECTION_HANDLER_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include "ns_address.h"

struct internal_socket_s;

typedef int send_to_socket_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, const unsigned char *, int);
typedef int receive_from_socket_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, unsigned char *, int);
typedef int get_pw_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t *pw_ptr, uint8_t *pw_len);
typedef void security_done_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40]);

typedef struct thread_conn_handler_s{
    struct internal_socket_s *socket;

    receive_from_socket_cb *_recv_cb;
    send_to_socket_cb *_send_cb;
    get_pw_cb *_get_password_cb;
    security_done_cb *_security_done_cb;

} thread_conn_handler_t;

thread_conn_handler_t *connection_handler_create(receive_from_socket_cb *recv_from_cb,
                                                 send_to_socket_cb *send_to_cb,
                                                 get_pw_cb *pw_cb,
                                                 security_done_cb *done_cb);

void connection_handler_destroy( thread_conn_handler_t *handler );

void connection_handler_close_secure_connection( thread_conn_handler_t *handler );

int coap_connection_handler_open_connection(thread_conn_handler_t *handler, uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool real_socket, bool bypassSec);

//If returns -2, it means security was started and data was not send
int coap_connection_handler_send_data(thread_conn_handler_t *handler, ns_address_t *dest_addr, uint8_t *data_ptr, uint16_t data_len, bool bypass_link_sec);

int coap_connection_handler_virtual_recv(thread_conn_handler_t *handler, uint8_t address[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len);

bool coap_connection_handler_socket_belongs_to(thread_conn_handler_t *handler, int8_t socket_id);

#endif
