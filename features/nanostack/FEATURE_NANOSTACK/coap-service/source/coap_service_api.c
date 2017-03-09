/*
 * Copyright (c) 2015-2016 ARM Limited. All Rights Reserved.
 */



#include <string.h>

#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mbed-coap/sn_coap_header.h"
#include "coap_service_api.h"
#include "coap_message_handler.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "common_functions.h"
#include "coap_connection_handler.h"
#include "net_interface.h"
#include "coap_service_api_internal.h"
#include "coap_message_handler.h"

static int16_t coap_msg_process_callback(int8_t socket_id, sn_coap_hdr_s *coap_message, coap_transaction_t *transaction_ptr);

typedef struct uri_registration {
    char *uri_ptr;
    uint16_t uri_len;
    uint8_t allowed_method;
    coap_service_request_recv_cb *request_recv_cb;
    ns_list_link_t link;
} uri_registration_t;

typedef NS_LIST_HEAD(uri_registration_t, link) uri_registration_list_t;

typedef struct coap_service {
    coap_service_security_done_cb *coap_security_done_cb;
    coap_service_security_start_cb *security_start_cb;
    coap_service_virtual_socket_send_cb *virtual_socket_send_cb;
    uri_registration_list_t uri_list;
    coap_conn_handler_t *conn_handler;
    int8_t interface_id;
    int8_t service_id;
    int8_t listen_socket;
    uint8_t service_options;
    ns_list_link_t link;
} coap_service_t;

#define TRACE_GROUP "ThSA"

static NS_LIST_DEFINE(instance_list, coap_service_t, link);
static int8_t tasklet_id = -1;
coap_msg_handler_t *coap_service_handle = NULL;
static uint32_t coap_ticks = 1;

#define COAP_TICK_TIMER 0xf1

static uri_registration_t *uri_registration_find(coap_service_t *this, const void *uri_ptr, uint16_t uri_len)
{
    ns_list_foreach(uri_registration_t, cur_ptr, &this->uri_list) {
        if (cur_ptr->uri_len == uri_len && memcmp(cur_ptr->uri_ptr, uri_ptr, uri_len) == 0) {
            return cur_ptr;
        }
    }
    return NULL;
}
static coap_service_t *service_find(int8_t service_id)
{
    coap_service_t *this = NULL;
    ns_list_foreach(coap_service_t, cur_ptr, &instance_list) {
        if (cur_ptr->service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_service_t *service_find_by_socket(int8_t socket_id)
{
    coap_service_t *this = NULL;
    ns_list_foreach(coap_service_t, cur_ptr, &instance_list) {
        if( coap_connection_handler_socket_belongs_to(cur_ptr->conn_handler, socket_id) ){
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static coap_service_t *service_find_by_uri(uint8_t socket_id, uint8_t *uri_ptr, uint16_t uri_len)
{
    ns_list_foreach(coap_service_t, cur_ptr, &instance_list) {
        if (coap_connection_handler_socket_belongs_to(cur_ptr->conn_handler, socket_id) && uri_registration_find(cur_ptr, uri_ptr, uri_len)) {
            return cur_ptr;
        }
    }
    return NULL;
}


/**
 *  Coap handling functions
 */
static void *own_alloc(uint16_t size)
{
    if (size) {
        return ns_dyn_mem_temporary_alloc(size);
    } else {
        return 0;
    }
}

static void own_free(void *ptr)
{
    if (ptr) {
        ns_dyn_mem_free(ptr);
    }
}

static uint8_t coap_tx_function(uint8_t *data_ptr, uint16_t data_len, sn_nsdl_addr_s *address_ptr, void *param)
{
    coap_service_t *this;
    coap_transaction_t *transaction_ptr = coap_message_handler_transaction_valid(param);
    ns_address_t dest_addr;

    if (!transaction_ptr || !data_ptr) {
        return 0;
    }

    tr_debug("Service %d, CoAP TX Function - mid: %d", transaction_ptr->service_id, common_read_16_bit(data_ptr + 2));

    this = service_find(transaction_ptr->service_id);
    if (!this) {
        return 0;
    }

    memcpy(&(dest_addr.address), address_ptr->addr_ptr, 16);
    dest_addr.identifier = address_ptr->port;
    dest_addr.type = ADDRESS_IPV6;

    if (-2 == coap_connection_handler_send_data(this->conn_handler, &dest_addr, transaction_ptr->local_address,
            data_ptr, data_len, (this->service_options & COAP_SERVICE_OPTIONS_SECURE_BYPASS) == COAP_SERVICE_OPTIONS_SECURE_BYPASS)) {
        transaction_ptr->data_ptr = ns_dyn_mem_alloc(data_len);
        if (!transaction_ptr->data_ptr) {
            tr_debug("coap tx out of memory");
            return 0;

        }
        memcpy(transaction_ptr->data_ptr, data_ptr, data_len);
        transaction_ptr->data_len = data_len;
    } else if (transaction_ptr->resp_cb == NULL ) {
        transaction_delete(transaction_ptr);
    }

    return 0;
}

static void service_event_handler(arm_event_s *event)
{
    if (event->event_type == ARM_LIB_TASKLET_INIT_EVENT) {
        tr_debug("service tasklet initialised");
        /*initialize coap service and listen socket*/
    }
    if (event->event_type == ARM_LIB_SYSTEM_TIMER_EVENT && event->event_id == COAP_TICK_TIMER) {
        coap_message_handler_exec(coap_service_handle, coap_ticks++);
        if(coap_ticks && !coap_ticks % SECURE_SESSION_CLEAN_INTERVAL){
            coap_connection_handler_exec(coap_ticks);
        }
    }
    eventOS_event_timer_request((uint8_t)COAP_TICK_TIMER, ARM_LIB_SYSTEM_TIMER_EVENT, tasklet_id, 1000);
}

static int16_t coap_msg_process_callback(int8_t socket_id, sn_coap_hdr_s *coap_message, coap_transaction_t *transaction_ptr)
{
    coap_service_t *this;
    if (!coap_message || !transaction_ptr) {
        return -1;
    }

    // Message is request, find correct handle
    this = service_find_by_uri(socket_id, coap_message->uri_path_ptr, coap_message->uri_path_len);
    if (!this) {
        tr_debug("not registered uri %.*s", coap_message->uri_path_len, coap_message->uri_path_ptr);
        if (coap_message->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
            coap_message_handler_response_send(coap_service_handle, transaction_ptr->service_id, COAP_SERVICE_OPTIONS_NONE, coap_message,
                COAP_MSG_CODE_RESPONSE_NOT_FOUND, COAP_CT_NONE, NULL, 0);
            return 0;
        }
        return -1;
    }

    uri_registration_t *uri_reg_ptr = uri_registration_find(this, coap_message->uri_path_ptr, coap_message->uri_path_len);
    if (uri_reg_ptr && uri_reg_ptr->request_recv_cb) {
        tr_debug("Service %d, call request recv cb uri %.*s", this->service_id, coap_message->uri_path_len, coap_message->uri_path_ptr);

        if ((this->service_options & COAP_SERVICE_OPTIONS_SECURE_BYPASS) == COAP_SERVICE_OPTIONS_SECURE_BYPASS ) {//TODO Add secure bypass option
            // Service has secure bypass active TODO this is not defined in interface
            // this check can be removed I think
            transaction_ptr->options = COAP_REQUEST_OPTIONS_SECURE_BYPASS;
        }
        transaction_ptr->service_id = this->service_id;
        return uri_reg_ptr->request_recv_cb(this->service_id, transaction_ptr->remote_address, transaction_ptr->remote_port, coap_message);
    }
    return -1;
}

static int recv_cb(int8_t socket_id, uint8_t src_address[static 16], uint16_t port, const uint8_t dst_address[static 16], unsigned char *data, int len)
{
    uint8_t *data_ptr = NULL;
    uint16_t data_len = 0;

    if (!data || !len) {
        return -1;
    }

    data_ptr = own_alloc(len);

    if (!data_ptr) {
        return -1;
    }
    memcpy(data_ptr, data, len);
    data_len = len;
    tr_debug("service recv socket data len %d ", data_len);

    //parse coap message what CoAP to use
    int ret = coap_message_handler_coap_msg_process(coap_service_handle, socket_id, src_address, port, dst_address, data_ptr, data_len, &coap_msg_process_callback);
    own_free(data_ptr);
    return ret;
}

static int send_cb(int8_t socket_id, const uint8_t address[static 16], uint16_t port, const void *data_ptr, int data_len)
{
    coap_service_t *this = service_find_by_socket(socket_id);
    if (this && this->virtual_socket_send_cb) {
        tr_debug("send to virtual socket, service: %d", this->service_id);
        return this->virtual_socket_send_cb(this->service_id, (uint8_t*)address, port, data_ptr, data_len);
    }
    return -1;
}

static void sec_done_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40])
{
    //TODO: this is not enough if shared socket. Inform all!
    coap_service_t *this = service_find_by_socket(socket_id);
    if (this && this->coap_security_done_cb) { // secure done callback
        this->coap_security_done_cb(this->service_id, address, keyblock);
    }

    //TODO: send all unsend transactions if more than 1
    coap_transaction_t *transaction_ptr = coap_message_handler_find_transaction(address, port);
    if (transaction_ptr && transaction_ptr->data_ptr) {
        tr_debug("send delayed packet");
        ns_address_t dest_addr;
        memcpy(dest_addr.address, address, 16);
        dest_addr.identifier = port;
        dest_addr.type = ADDRESS_IPV6;

        coap_connection_handler_send_data(this->conn_handler, &dest_addr, transaction_ptr->local_address,
                transaction_ptr->data_ptr, transaction_ptr->data_len, (this->service_options & COAP_SERVICE_OPTIONS_SECURE_BYPASS) == COAP_SERVICE_OPTIONS_SECURE_BYPASS);
        ns_dyn_mem_free(transaction_ptr->data_ptr);
        transaction_ptr->data_ptr = NULL;
        transaction_ptr->data_len = 0;
        if (transaction_ptr->resp_cb == NULL) {
            transaction_delete(transaction_ptr);
        }
    }
}

static int get_passwd_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t *pw_ptr, uint8_t *pw_len)
{
    coap_service_t *this = service_find_by_socket(socket_id);
    if (this && this->security_start_cb) {
        return this->security_start_cb(this->service_id, address, port, pw_ptr, pw_len);
    }
    return -1;
}

int8_t coap_service_initialize(int8_t interface_id, uint16_t listen_port, uint8_t service_options,
                                 coap_service_security_start_cb *start_ptr, coap_service_security_done_cb *coap_security_done_cb)
{
    (void) interface_id;

    coap_service_t *this = ns_dyn_mem_alloc(sizeof(coap_service_t));
    if (!this) {
        return -1;
    }
    memset(this, 0, sizeof(coap_service_t));
    tr_debug("service init interface %d, port %d, options %d", interface_id, listen_port, service_options);

    int8_t id = 1;// get unique id
    while (service_find(id) && id < 127) {
        id++;
    }
    this->service_id = id;
    this->service_options = service_options;

    this->security_start_cb = start_ptr;
    this->coap_security_done_cb = coap_security_done_cb;

    if (tasklet_id == -1) {
        tr_debug("service tasklet init");
        tasklet_id = eventOS_event_handler_create(&service_event_handler, ARM_LIB_TASKLET_INIT_EVENT);
    }

    this->conn_handler = connection_handler_create(recv_cb, send_cb, get_passwd_cb, sec_done_cb);
    if(!this->conn_handler){
        ns_dyn_mem_free(this);
        return -1;
    }

    if (0 > coap_connection_handler_open_connection(this->conn_handler, listen_port, ((this->service_options & COAP_SERVICE_OPTIONS_EPHEMERAL_PORT) == COAP_SERVICE_OPTIONS_EPHEMERAL_PORT),
                                              ((this->service_options & COAP_SERVICE_OPTIONS_SECURE) == COAP_SERVICE_OPTIONS_SECURE),
                                              ((this->service_options & COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET) != COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET),
                                              ((this->service_options & COAP_SERVICE_OPTIONS_SECURE_BYPASS) == COAP_SERVICE_OPTIONS_SECURE_BYPASS))){
        ns_dyn_mem_free(this->conn_handler);
        ns_dyn_mem_free(this);
        return -1;
    }

    if (!coap_service_handle) {
        coap_service_handle = coap_message_handler_init(&own_alloc, &own_free, &coap_tx_function);
    }
    if (!coap_service_handle) {
        tr_error("coap service alloc failed");
        //TODO proper handling
    }

    ns_list_add_to_start(&instance_list, this);

    return id;
}

void coap_service_delete(int8_t service_id)
{
    coap_service_t *this = service_find(service_id);
    if (!this) {
        return;
    }

    if (this->conn_handler){
        connection_handler_destroy(this->conn_handler);
    }

    //TODO clear all transactions
    ns_list_foreach_safe(uri_registration_t, cur_ptr, &this->uri_list) {
        ns_dyn_mem_free(cur_ptr->uri_ptr);
        ns_list_remove(&this->uri_list, cur_ptr);
        ns_dyn_mem_free(cur_ptr);
    }

    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
    return;
}

extern void coap_service_close_secure_connection(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port)
{
    coap_service_t *this = service_find(service_id);
    if (!this || !destination_addr_ptr) {
        return;
    }
    if (this->conn_handler){
        connection_handler_close_secure_connection(this->conn_handler, destination_addr_ptr, port);
    }
}

int16_t coap_service_virtual_socket_recv(int8_t service_id, uint8_t source_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
    coap_service_t *this = service_find(service_id);
    tr_debug("Service %d, virtual socket received", service_id);
    if (!this) {
        return -1;
    }
    return coap_connection_handler_virtual_recv(this->conn_handler, source_addr_ptr, port, data_ptr, data_len);
}

int16_t coap_service_virtual_socket_set_cb(int8_t service_id, coap_service_virtual_socket_send_cb *send_method_ptr)
{
    coap_service_t *this = service_find(service_id);
    tr_debug("register virtual socket cb to service %d", service_id);
    if (!this) {
        return -1;
    }
    this->virtual_socket_send_cb = send_method_ptr;
    return 0;
}

int8_t coap_service_register_uri(int8_t service_id, const char *uri, uint8_t allowed_method, coap_service_request_recv_cb *request_recv_cb)
{
    coap_service_t *this = service_find(service_id);
    uri_registration_t *uri_reg_ptr;
    char *uri_ptr = NULL;
    uint16_t uri_len;
    tr_debug("Service %d, Uri registration uri: %s", service_id, uri);
    if (!this || !uri) {
        return -1;
    }
    uri_len = strlen(uri);

    uri_reg_ptr = uri_registration_find(this, uri, uri_len);
    if (!uri_reg_ptr) {
        uri_reg_ptr = ns_dyn_mem_alloc(sizeof(uri_registration_t));
        if( !uri_reg_ptr ){
            tr_error("Uri registration failed, OOM");
            return -2;
        }
        uri_reg_ptr->uri_ptr = NULL;
    } else {
        ns_dyn_mem_free(uri_reg_ptr->uri_ptr);
        ns_list_remove(&this->uri_list, uri_reg_ptr);
    }

    uri_ptr = ns_dyn_mem_alloc(uri_len);
    if (!uri_ptr) {
        ns_dyn_mem_free(uri_reg_ptr);
        tr_error("Uri registration failed, OOM");
        return -2;
    }

    uri_reg_ptr->uri_ptr = memcpy(uri_ptr, uri, uri_len);
    uri_reg_ptr->uri_len = uri_len;
    uri_reg_ptr->request_recv_cb = request_recv_cb;
    uri_reg_ptr->allowed_method = allowed_method;
    ns_list_add_to_start(&this->uri_list, uri_reg_ptr);
    return 0;
}

int8_t coap_service_unregister_uri(int8_t service_id, const char *uri)
{
    coap_service_t *this = service_find(service_id);
    uri_registration_t *uri_reg_ptr;
    tr_debug("Service %d, Uri unregistration uri: %s", service_id, uri);
    if (!this || !uri) {
        return -1;
    }

    uri_reg_ptr = uri_registration_find(this, uri, strlen(uri));
    if (!uri_reg_ptr) {
        return -2;
    }

    ns_dyn_mem_free(uri_reg_ptr->uri_ptr);
    ns_list_remove(&this->uri_list, uri_reg_ptr);
    ns_dyn_mem_free(uri_reg_ptr);

    return 0;
}

uint16_t coap_service_request_send(int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16], uint16_t destination_port, sn_coap_msg_type_e msg_type, sn_coap_msg_code_e msg_code, const char *uri,
        sn_coap_content_format_e cont_type, const uint8_t *payload_ptr, uint16_t payload_len, coap_service_response_recv *request_response_cb){
    //TODO: coap_service_response_recv is an ugly cast, this should be refactored away + sn_coap_hdr_s MUST NOT be exposed to users of coap-service!
    //Callback would be still needed, but where to store callback?
    return coap_message_handler_request_send(coap_service_handle, service_id, options, destination_addr, destination_port, msg_type, msg_code, uri, cont_type, payload_ptr, payload_len, request_response_cb);
}

int8_t coap_service_response_send(int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code, sn_coap_content_format_e content_type, const uint8_t *payload_ptr,uint16_t payload_len){
    return coap_message_handler_response_send(coap_service_handle, service_id, options, request_ptr, message_code, content_type, payload_ptr, payload_len);
}

int8_t coap_service_set_handshake_timeout(int8_t service_id, uint32_t min, uint32_t max)
{
    coap_service_t *this = service_find(service_id);
    if(!this){
        return -1;
    }

    return coap_connection_handler_set_timeout(this->conn_handler, min, max);
}

uint32_t coap_service_get_internal_timer_ticks(void)
{
    return coap_ticks;
}

uint16_t coap_service_id_find_by_socket(int8_t socket_id)
{
    coap_service_t *this = service_find_by_socket(socket_id);

    return this ? this->service_id:0;
}
