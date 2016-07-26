/*
 * Copyright (c) 2015-2016 ARM Limited. All Rights Reserved.
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
}session_state_t;

typedef struct internal_socket_s {
    coap_conn_handler_t *parent;

    uint32_t timeout_min;
    uint32_t timeout_max;

    uint16_t listen_port;
    int8_t listen_socket;

    ns_address_t dest_addr;
    size_t data_len;
    uint8_t *data;

    bool real_socket;
    uint8_t usage_counter;
    bool is_secure;

    bool bypass_link_sec;

    ns_list_link_t link;
} internal_socket_t;

static NS_LIST_DEFINE(socket_list, internal_socket_t, link);

static void timer_cb(void* param);

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

    secure_timer_t timer;

    session_state_t session_state;
    uint32_t last_contact_time;
    ns_list_link_t link;
} secure_session_t;

static NS_LIST_DEFINE(secure_session_list, secure_session_t, link);
static int send_to_socket(int8_t socket_id, uint8_t *address_ptr, uint16_t port, const unsigned char *buf, size_t len);
static int receive_from_socket(int8_t socket_id, unsigned char *buf, size_t len);
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

static void secure_session_delete(secure_session_t *this)
{
    if (this) {
        ns_list_remove(&secure_session_list, this);
        if( this->sec_handler ){
            coap_security_destroy(this->sec_handler);
            this->sec_handler = NULL;
        }
        if(this->timer.timer){
            eventOS_timeout_cancel(this->timer.timer);
        }
        ns_dyn_mem_free(this);
        this = NULL;
    }

    return;
}

static secure_session_t *secure_session_create(internal_socket_t *parent, uint8_t *address_ptr, uint16_t port)
{
    if(!address_ptr){
        return NULL;
    }

    if(MAX_SECURE_SESSION_COUNT <= ns_list_count(&secure_session_list)){
        // Seek & destroy oldest session where close notify have been sent
        secure_session_t *to_be_removed = NULL;
        ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
            if(cur_ptr->session_state == SECURE_SESSION_CLOSED){
                if(!to_be_removed || cur_ptr->last_contact_time < to_be_removed->last_contact_time){
                    to_be_removed = cur_ptr;
                }
            }
        }
        if(!to_be_removed){
            return NULL;
        }

        secure_session_delete(to_be_removed);
    }

    secure_session_t *this = ns_dyn_mem_alloc(sizeof(secure_session_t));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(secure_session_t));

    uint8_t timer_id = 1;

    while(secure_session_find_by_timer_id(timer_id)){
        if(timer_id == 0xff){
            ns_dyn_mem_free(this);
            return NULL;
        }
        timer_id++;
    }
    this->timer.id = timer_id;

    this->sec_handler = coap_security_create(parent->listen_socket, this->timer.id, address_ptr, port, ECJPAKE,
                                               &send_to_socket, &receive_from_socket, &start_timer, &timer_status);
    if( !this->sec_handler ){
        ns_dyn_mem_free(this);
        return NULL;
    }
    this->parent = parent;

    this->session_state = SECURE_SESSION_HANDSHAKE_ONGOING;
    ns_list_add_to_start(&secure_session_list, this);

    return this;
}


static void clear_secure_sessions(internal_socket_t *this){
    if( this ){
        ns_list_foreach_safe(secure_session_t, cur_ptr, &secure_session_list) {
            if( cur_ptr->parent == this ){
                coap_security_send_close_alert( cur_ptr->sec_handler );
                secure_session_delete(cur_ptr);
            }
        }
    }
}

static secure_session_t *secure_session_find(internal_socket_t *parent, uint8_t *address_ptr, uint16_t port)
{
    secure_session_t *this = NULL;
    ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
        if( cur_ptr->sec_handler ){
            if (cur_ptr->parent == parent && cur_ptr->sec_handler->_remote_port == port &&
                memcmp(cur_ptr->sec_handler->_remote_address, address_ptr, 16) == 0) {
                this = cur_ptr;
    //            hack_save_remote_address(address_ptr, port);
                break;
            }
        }
    }
    return this;
}



static void recv_sckt_msg(void *cb_res);
static void secure_recv_sckt_msg(void *cb_res);

static internal_socket_t *int_socket_create(uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool real_socket, bool bypassSec)
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
    this->listen_socket = -1;
    if( real_socket ){
        if( use_ephemeral_port ){ //socket_api creates ephemeral port if the one provided is 0
            listen_port = 0;
        }
        if( !is_secure ){
            this->listen_socket = socket_open(SOCKET_UDP, listen_port, recv_sckt_msg);
        }else{
            this->listen_socket = socket_open(SOCKET_UDP, listen_port, secure_recv_sckt_msg);
        }
        // Socket create failed
        if(this->listen_socket < 0){
            ns_dyn_mem_free(this);
            return NULL;
        }
        // XXX API for this? May want to get clever to do recommended first query = 1 hop, retries = whole PAN
        socket_setsockopt(this->listen_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &(const int16_t) {
            16
        }, sizeof(int16_t));
    }else{
        this->listen_socket = -1;
    }

    ns_list_add_to_start(&socket_list, this);
    return this;
}

static void int_socket_delete(internal_socket_t *this)
{
    if (this) {
        this->usage_counter--;
        if(this->usage_counter == 0){
            clear_secure_sessions(this);
            socket_free(this->listen_socket);
            ns_list_remove(&socket_list, this);
            if( this->data ){
                ns_dyn_mem_free(this->data);
                this->data = NULL;
            }
            if(this->parent){
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
        if( cur_ptr->listen_socket == id ) {
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
        if( cur_ptr->listen_port == port && cur_ptr->real_socket == is_real_socket &&
            is_secure == cur_ptr->is_secure /*&& bypass_link_sec == bypassSec*/) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static int send_to_socket(int8_t socket_id, uint8_t *address_ptr, uint16_t port, const unsigned char *buf, size_t len)
{
    internal_socket_t *sock = int_socket_find_by_socket_id(socket_id);
    if(!sock){
        return -1;
    }
    if(!sock->real_socket){
        //In this case all clients will have socket_id -1 and socket will not have a real address
        //so sock->dest_addr cannot be used here
        int ret = sock->parent->_send_cb(sock->listen_socket, address_ptr, port, buf, len);
        if( ret < 0 )
            return ret;
        return len;
    }

    int opt_name = SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
    int8_t securityLinkLayer = 1;
    if( sock->bypass_link_sec ){
        securityLinkLayer = 0;
    }
    socket_setsockopt(sock->listen_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_name, sizeof(int));
    socket_setsockopt(sock->listen_socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
    //For some reason socket_sendto returns 0 in success, while other socket impls return number of bytes sent!!!
    //TODO: check if address_ptr is valid and use that instead if it is
    int ret = socket_sendto(sock->listen_socket, &sock->dest_addr, (unsigned char*)buf, len);
    if( ret < 0 )
        return ret;
    return len;
}

static int receive_from_socket(int8_t socket_id, unsigned char *buf, size_t len)
{
    (void)len;
    internal_socket_t *sock = int_socket_find_by_socket_id(socket_id);
    if( sock->data && sock->data_len > 0 ){
        memcpy( buf, sock->data, sock->data_len );
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
    if( sec ){
        if(sec->timer.fin_ms > sec->timer.int_ms){
            /* Intermediate expiry */
            sec->timer.fin_ms -= sec->timer.int_ms;
            sec->timer.state = TIMER_STATE_INT_EXPIRY;
            int error = coap_security_handler_continue_connecting(sec->sec_handler);
            if(MBEDTLS_ERR_SSL_TIMEOUT == error) {
                //TODO: How do we handle timeouts?
                secure_session_delete(sec);
            }
            else{
                sec->timer.timer = eventOS_timeout_ms(timer_cb, sec->timer.int_ms, (void*)sec);
            }
        }
        else{
            /* We have counted the number of cycles - finish */
            eventOS_timeout_cancel(sec->timer.timer);
            sec->timer.fin_ms = 0;
            sec->timer.int_ms = 0;
            sec->timer.timer = NULL;
            sec->timer.state = TIMER_STATE_FIN_EXPIRY;
            int error = coap_security_handler_continue_connecting(sec->sec_handler);
            if(MBEDTLS_ERR_SSL_TIMEOUT == error) {
                //TODO: How do we handle timeouts?
                secure_session_delete(sec);
            }
        }
    }
}

static void start_timer(int8_t timer_id, uint32_t int_ms, uint32_t fin_ms)
{
    secure_session_t *sec = secure_session_find_by_timer_id(timer_id);
    if( sec ){
        if ((int_ms > 0) && (fin_ms > 0)) {
            sec->timer.int_ms = int_ms;
            sec->timer.fin_ms = fin_ms;
            sec->timer.state = TIMER_STATE_NO_EXPIRY;
            if(sec->timer.timer){
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
    if( sec ){
        return (int)sec->timer.state;
    }
    return TIMER_STATE_CANCELLED;
}

static int read_data(socket_callback_t *sckt_data, internal_socket_t *sock, ns_address_t *src_address)
{
    sock->data_len = 0;
    if (sckt_data->event_type == SOCKET_DATA && sckt_data->d_len > 0) {
        if( sock->data ){
            ns_dyn_mem_free(sock->data);
            sock->data = NULL;
        }
        sock->data = ns_dyn_mem_temporary_alloc(sckt_data->d_len);
        if( !sock->data ){
            sock->data = NULL;
            return -1;
        }
        sock->data_len = socket_read(sckt_data->socket_id, src_address, sock->data, sckt_data->d_len);
    }
    if( sock->data_len < 1){
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
        sock->data_len = 0;
        return -1;
    }
    return 0;
}

static void secure_recv_sckt_msg(void *cb_res)
{
    socket_callback_t *sckt_data = cb_res;
    internal_socket_t *sock = int_socket_find_by_socket_id(sckt_data->socket_id);
    ns_address_t src_address;

    if( sock && read_data(sckt_data, sock, &src_address) == 0 ){
        secure_session_t *session = secure_session_find(sock, src_address.address, src_address.identifier);

        // Create session
        if( !session ){
            memcpy( sock->dest_addr.address, src_address.address, 16 );
            sock->dest_addr.identifier = src_address.identifier;
            sock->dest_addr.type = src_address.type;
            session = secure_session_create(sock, src_address.address, src_address.identifier);
        }
        if( !session ){
            tr_err("secure_recv_sckt_msg session creation failed - OOM");
            return;
        }
        session->last_contact_time = coap_service_get_internal_timer_ticks();
        // Start handshake
        if( !session->sec_handler->_is_started ){
            uint8_t *pw = (uint8_t *)ns_dyn_mem_alloc(64);
            uint8_t pw_len;
            if( sock->parent->_get_password_cb && 0 == sock->parent->_get_password_cb(sock->listen_socket, src_address.address, src_address.identifier, pw, &pw_len)){
                //TODO: get_password_cb should support certs and PSK also
                coap_security_keys_t keys;
                keys._priv = pw;
                keys._priv_len = pw_len;
                coap_security_handler_connect_non_blocking(session->sec_handler, true, DTLS, keys, sock->timeout_min, sock->timeout_max);
                //TODO: error handling
            }
            ns_dyn_mem_free(pw);
        }else{
            //Continue handshake
            if(session->session_state == SECURE_SESSION_HANDSHAKE_ONGOING){
                int ret = coap_security_handler_continue_connecting(session->sec_handler);
                // Handshake done
                if(ret == 0){
                    eventOS_timeout_cancel(session->timer.timer);
                    session->timer.timer = NULL;
                    session->session_state = SECURE_SESSION_OK;
                    if( sock->parent->_security_done_cb ){
                        sock->parent->_security_done_cb(sock->listen_socket, src_address.address,
                                                       src_address.identifier,
                                                       session->sec_handler->_keyblk.value);
                    }
                }
                else if (ret < 0){
                    // error handling
                    // TODO: here we also should clear CoAP retransmission buffer and inform that CoAP request sending is failed.
                    secure_session_delete(session);
                }
            //Session valid
            }else{
                unsigned char *data = ns_dyn_mem_temporary_alloc(sock->data_len);
                int len = 0;
                len = coap_security_handler_read(session->sec_handler, data, sock->data_len);
                if( len < 0 ){
                    ns_dyn_mem_free(data);
                    secure_session_delete( session );
                }else{
                    if( sock->parent->_recv_cb ){
                        sock->parent->_recv_cb(sock->listen_socket, src_address.address, src_address.identifier, data, len);
                    }
                    ns_dyn_mem_free(data);
                }
            }
        }
    }
}

static void recv_sckt_msg(void *cb_res)
{
    socket_callback_t *sckt_data = cb_res;
    internal_socket_t *sock = int_socket_find_by_socket_id(sckt_data->socket_id);
    ns_address_t src_address;
    if( sock && read_data(sckt_data, sock, &src_address) == 0 ){
        if(sock->parent && sock->parent->_recv_cb){
            sock->parent->_recv_cb(sock->listen_socket, src_address.address, src_address.identifier, sock->data, sock->data_len);
        }
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
    }
}

int coap_connection_handler_virtual_recv(coap_conn_handler_t *handler, uint8_t address[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
    if( !handler || !handler->socket ){
        return -1;
    }
    internal_socket_t *sock = handler->socket;
    sock->data_len = data_len;
    if( sock->data ){
        ns_dyn_mem_free(sock->data);
        sock->data = NULL;
    }
    sock->data = ns_dyn_mem_temporary_alloc(data_len);
    if( data_len > 0 && !sock->data ){
        return -1;
    }
    if( data_ptr ){
        memcpy(sock->data, data_ptr, data_len);
    }else{
        if( sock->data ){
            ns_dyn_mem_free(sock->data);
            sock->data = NULL;
        }
    }

    if( handler->socket->is_secure ){
        secure_session_t *session = secure_session_find(sock, address, port);
        if( !session ){
            session = secure_session_create(sock, address, port);
        }
        if( !session ){
            tr_err("coap_connection_handler_virtual_recv session creation failed - OOM");
            return -1;
        }

        session->last_contact_time = coap_service_get_internal_timer_ticks();

        if( !session->sec_handler->_is_started ){
            uint8_t *pw = (uint8_t *)ns_dyn_mem_alloc(64);
            uint8_t pw_len;
            if( sock->parent->_get_password_cb && 0 == sock->parent->_get_password_cb(sock->listen_socket, address, port, pw, &pw_len)){
                //TODO: get_password_cb should support certs and PSK also
                coap_security_keys_t keys;
                keys._priv = pw;
                keys._priv_len = pw_len;
                coap_security_handler_connect_non_blocking(session->sec_handler, true, DTLS, keys, handler->socket->timeout_min, handler->socket->timeout_max);
                //TODO: error handling
                ns_dyn_mem_free(pw);
                return 0;
            }else{
                ns_dyn_mem_free(pw);
                return -1;
            }
        }else{
            if(session->session_state == SECURE_SESSION_HANDSHAKE_ONGOING){
                int ret = coap_security_handler_continue_connecting(session->sec_handler);
                if(ret == 0){
                    session->session_state = SECURE_SESSION_OK;
                    if( handler->_security_done_cb ){
                        handler->_security_done_cb(sock->listen_socket,
                                                  address, port,
                                                  session->sec_handler->_keyblk.value);
                    }
                    return 0;
                }
                else if(ret < 0)
                {
                    // error handling
                    // TODO: here we also should clear CoAP retransmission buffer and inform that CoAP request sending is failed.
                    secure_session_delete(session);
                }
                //TODO: error handling
            }else{
                unsigned char *data = ns_dyn_mem_temporary_alloc(sock->data_len);
                int len = 0;
                len = coap_security_handler_read(session->sec_handler, data, sock->data_len);
                if( len < 0 ){
                    ns_dyn_mem_free(data);
                    secure_session_delete( session );
                    return 0;
                }else{
                    if( sock->parent->_recv_cb ){
                        sock->parent->_recv_cb(sock->listen_socket, address, port, data, len);
                    }
                    ns_dyn_mem_free(data);
                    data = NULL;
                }
                return 0;
            }
        }
    }else{
        if( sock->parent->_recv_cb ){
            sock->parent->_recv_cb(sock->listen_socket, address, port, sock->data, sock->data_len);
        }
        if( sock->data ){
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
                                                 security_done_cb *done_cb )
{
    if( recv_from_cb == NULL ){
        return NULL;
    }

    coap_conn_handler_t *handler = ns_dyn_mem_alloc(sizeof(coap_conn_handler_t));
    if(!handler){
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
void connection_handler_destroy(coap_conn_handler_t *handler)
{
    if(handler){
       int_socket_delete(handler->socket);
       ns_dyn_mem_free(handler);
    }
}

void connection_handler_close_secure_connection( coap_conn_handler_t *handler, uint8_t destination_addr_ptr[static 16], uint16_t port )
{
    if(handler){
        if( handler->socket && handler->socket->is_secure){
            secure_session_t *session = secure_session_find( handler->socket, destination_addr_ptr, port);
            if( session ){
                coap_security_send_close_alert( session->sec_handler );
                session->session_state = SECURE_SESSION_CLOSED;
                session->last_contact_time = coap_service_get_internal_timer_ticks();
            }
        }
    }
}

int coap_connection_handler_open_connection(coap_conn_handler_t *handler, uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool is_real_socket, bool bypassSec)
{
    if( !handler ){
        return -1;
    }
    //virtual socket must have send callback
    if( !is_real_socket && !handler->_send_cb ){
        return -1;
    }
    if( handler->socket ){
        int_socket_delete(handler->socket);
    }

    internal_socket_t *current = !use_ephemeral_port?int_socket_find(listen_port, is_secure, is_real_socket, bypassSec):NULL;
    if(!current){
        handler->socket = int_socket_create(listen_port, use_ephemeral_port, is_secure, is_real_socket, bypassSec);
        if(!handler->socket){
            return -1;
        }
        handler->socket->parent = ns_dyn_mem_alloc(sizeof(coap_conn_handler_t));
        if(!handler->socket->parent){
            int_socket_delete(handler->socket);
            return -1;
        }
        *handler->socket->parent = *handler;
    }else{
        current->usage_counter++;
        handler->socket = current;
    }
    return 0;
}

int coap_connection_handler_send_data(coap_conn_handler_t *handler, ns_address_t *dest_addr, uint8_t *data_ptr, uint16_t data_len, bool bypass_link_sec)
{
    if( !handler || !handler->socket || !dest_addr){
        return -1;
    }
    if(handler->socket->is_secure){
        handler->socket->bypass_link_sec = bypass_link_sec;
        memcpy(handler->socket->dest_addr.address, dest_addr->address, 16);
        handler->socket->dest_addr.identifier = dest_addr->identifier;
        handler->socket->dest_addr.type = dest_addr->type;
        secure_session_t *session = secure_session_find(handler->socket, dest_addr->address, dest_addr->identifier);
        if( !session ){
            session = secure_session_create(handler->socket, dest_addr->address, dest_addr->identifier);
            if( !session ){
                return -1;
            }
            session->last_contact_time = coap_service_get_internal_timer_ticks();
            memcpy( handler->socket->dest_addr.address, dest_addr->address, 16 );
            handler->socket->dest_addr.identifier = dest_addr->identifier;
            handler->socket->dest_addr.type = dest_addr->type;
            uint8_t *pw = (uint8_t *)ns_dyn_mem_alloc(64);
            if(!pw){
                //todo: free secure session?
                return -1;
            }
            uint8_t pw_len;
            if( handler->_get_password_cb && 0 == handler->_get_password_cb(handler->socket->listen_socket, dest_addr->address, dest_addr->identifier, pw, &pw_len)){
                //TODO: get_password_cb should support certs and PSK also
                coap_security_keys_t keys;
                keys._priv = pw;
                keys._priv_len = pw_len;
                coap_security_handler_connect_non_blocking(session->sec_handler, false, DTLS, keys, handler->socket->timeout_min, handler->socket->timeout_max);
                ns_dyn_mem_free(pw);
                return -2;
            }else{
                //free secure session?
                ns_dyn_mem_free(pw);
                return -1;
            }
        }else if( session->session_state == SECURE_SESSION_OK ){
            if( coap_security_handler_send_message(session->sec_handler, data_ptr, data_len ) > 0 ){
                session->last_contact_time = coap_service_get_internal_timer_ticks();
                return 0;
            }
        }
        return -1;
    }else{
        if( !handler->socket->real_socket && handler->_send_cb){
            return handler->_send_cb((int8_t)handler->socket->listen_socket, dest_addr->address, dest_addr->identifier, data_ptr, data_len);
        }
        int opt_name = SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;
        int8_t securityLinkLayer = 1;
        if( bypass_link_sec ){
            securityLinkLayer = 0;
        }
        socket_setsockopt(handler->socket->listen_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_name, sizeof(int));
        socket_setsockopt(handler->socket->listen_socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
        return socket_sendto(handler->socket->listen_socket, dest_addr, data_ptr, data_len);
    }
}

bool coap_connection_handler_socket_belongs_to(coap_conn_handler_t *handler, int8_t socket_id)
{
    if( !handler || !handler->socket){
        return false;
    }

    if( handler->socket->listen_socket == socket_id){
        return true;
    }
    return false;
}

int8_t coap_connection_handler_set_timeout(coap_conn_handler_t *handler, uint32_t min, uint32_t max)
{
    if(!handler || !handler->socket){
        return -1;
    }
    handler->socket->timeout_max = max;
    handler->socket->timeout_min = min;

    return 0;
}

/* No need to call every second - call rather like every minute (SECURE_SESSION_CLEAN_INTERVAL sets this) */
void coap_connection_handler_exec(uint32_t time)
{
    if(ns_list_count(&secure_session_list)){
        // Seek & destroy old sessions where close notify have been sent
        ns_list_foreach(secure_session_t, cur_ptr, &secure_session_list) {
            if(cur_ptr->session_state == SECURE_SESSION_CLOSED ||
                    cur_ptr->session_state == SECURE_SESSION_HANDSHAKE_ONGOING){
                if((cur_ptr->last_contact_time +  CLOSED_SECURE_SESSION_TIMEOUT) <= time){
                    secure_session_delete(cur_ptr);
                }
            }
            if(cur_ptr->session_state == SECURE_SESSION_OK){
                if((cur_ptr->last_contact_time +  OPEN_SECURE_SESSION_TIMEOUT) <= time){
                    secure_session_delete(cur_ptr);
                }
            }
        }
    }
}
