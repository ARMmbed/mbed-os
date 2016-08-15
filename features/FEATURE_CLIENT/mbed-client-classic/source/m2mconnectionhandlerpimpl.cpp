/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed-client-classic/m2mconnectionhandlerpimpl.h"
#include "mbed-client/m2mconnectionobserver.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2msecurity.h"
#include "mbed-client/m2mconnectionhandler.h"

#include "NetworkInterface.h"
#include "UDPSocket.h"
#include "TCPSocket.h"

#include "eventOS_event.h"
#include "eventOS_scheduler.h"

#include "mbed-trace/mbed_trace.h"
#include "mbed.h"

#define TRACE_GROUP "mClt"

int8_t M2MConnectionHandlerPimpl::_tasklet_id = -1;

extern "C" void connection_tasklet_event_handler(arm_event_s *event)
{
    tr_debug("M2MConnectionHandlerPimpl::connection_tasklet_event_handler");
    M2MConnectionHandlerPimpl::TaskIdentifier *task_id = (M2MConnectionHandlerPimpl::TaskIdentifier*)event->data_ptr;
    M2MConnectionHandlerPimpl* pimpl = (M2MConnectionHandlerPimpl*)task_id->pimpl;
    if(pimpl) {
        eventOS_scheduler_set_active_tasklet(pimpl->connection_tasklet_handler());
    }
    switch (event->event_type) {
        case M2MConnectionHandlerPimpl::ESocketIdle:
            tr_debug("Connection Tasklet Generated");
            break;
        case M2MConnectionHandlerPimpl::ESocketReadytoRead:
            tr_debug("connection_tasklet_event_handler - ESocketReadytoRead");
            if(pimpl) {
                if(pimpl->is_handshake_ongoing()) {
                    pimpl->receive_handshake_handler();
                } else {
                    pimpl->receive_handler();
                }
            }
            break;
        case M2MConnectionHandlerPimpl::ESocketDnsHandler:
            tr_debug("connection_tasklet_event_handler - ESocketDnsHandler");
            if(pimpl) {
                pimpl->dns_handler();
            }
            break;
        case M2MConnectionHandlerPimpl::ESocketSend:
            tr_debug("connection_tasklet_event_handler - ESocketSend");
            if(pimpl) {
                pimpl->send_socket_data((uint8_t*)task_id->data_ptr,(uint16_t)event->event_data);
                if (task_id->data_ptr) {
                    free(task_id->data_ptr);
                }
            }
            break;
        default:
            break;
    }
    if (task_id) {
        free(task_id);
    }
}

M2MConnectionHandlerPimpl::M2MConnectionHandlerPimpl(M2MConnectionHandler* base, M2MConnectionObserver &observer,
                                                     M2MConnectionSecurity* sec,
                                                     M2MInterface::BindingMode mode,
                                                     M2MInterface::NetworkStack stack)
:_base(base),
 _observer(observer),
 _security_impl(sec),
 _use_secure_connection(false),
 _binding_mode(mode),
 _network_stack(stack),
 _socket(0),
 _is_handshaking(false),
 _listening(true),
 _server_type(M2MConnectionObserver::LWM2MServer),
 _server_port(0),
 _listen_port(0),
 _running(false),
 _net_iface(0),
 _socket_address(0)
{
    memset(&_address_buffer, 0, sizeof _address_buffer);
    memset(&_address, 0, sizeof _address);
    _address._address = _address_buffer;

    if (_network_stack != M2MInterface::LwIP_IPv4) {
        error("ConnectionHandler: Unsupported network stack, only IPv4 is currently supported");
    }
    _running = true;
    tr_debug("M2MConnectionHandlerPimpl::M2MConnectionHandlerPimpl() - Initializing thread");
    eventOS_scheduler_mutex_wait();
    if (M2MConnectionHandlerPimpl::_tasklet_id == -1) {
        M2MConnectionHandlerPimpl::_tasklet_id = eventOS_event_handler_create(&connection_tasklet_event_handler, ESocketIdle);
    }
    eventOS_scheduler_mutex_release();
}

M2MConnectionHandlerPimpl::~M2MConnectionHandlerPimpl()
{
    tr_debug("M2MConnectionHandlerPimpl::~M2MConnectionHandlerPimpl()");
    if (_socket) {
        delete _socket;
        _socket = 0;
    }
    _net_iface = 0;
    delete _security_impl;
    tr_debug("M2MConnectionHandlerPimpl::~M2MConnectionHandlerPimpl() - OUT");
}

bool M2MConnectionHandlerPimpl::bind_connection(const uint16_t listen_port)
{
    _listen_port = listen_port;
    return true;
}

bool M2MConnectionHandlerPimpl::resolve_server_address(const String& server_address,
                                                       const uint16_t server_port,
                                                       M2MConnectionObserver::ServerType server_type,
                                                       const M2MSecurity* security)
{
    tr_debug("M2MConnectionHandlerPimpl::resolve_server_address()");
    if (!_net_iface) {
        return false;
    }
    _security = security;
    _server_port = server_port;
    _server_type = server_type;
    _server_address = server_address;
    TaskIdentifier* task = (TaskIdentifier*)malloc(sizeof(TaskIdentifier));
    if (!task) {
        return false;
    }
    task->pimpl = this;

    arm_event_s event;
    event.receiver = M2MConnectionHandlerPimpl::_tasklet_id;
    event.sender = 0;
    event.event_type = ESocketDnsHandler;
    event.data_ptr = task;
    event.priority = ARM_LIB_HIGH_PRIORITY_EVENT;
    return eventOS_event_send(&event) == 0 ? true : false;
}

void M2MConnectionHandlerPimpl::dns_handler()
{
    tr_debug("M2MConnectionHandlerPimpl::dns_handler()");
    _socket_address = new SocketAddress(_net_iface,_server_address.c_str(), _server_port);
    if(*_socket_address) {
        _address._address = (void*)_socket_address->get_ip_address();
        tr_debug("IP Address %s",_socket_address->get_ip_address());
        tr_debug("Port %d",_socket_address->get_port());
        _address._length = strlen((char*)_address._address);
        _address._port = _socket_address->get_port();
        _address._stack = _network_stack;
    } else {
        _observer.socket_error(M2MConnectionHandler::DNS_RESOLVING_ERROR, true);
        close_socket();
        return;
    }

    close_socket();
    init_socket();

    if(is_tcp_connection()) {
       tr_debug("M2MConnectionHandlerPimpl::resolve_server_address - Using TCP");
        if (((TCPSocket*)_socket)->connect(*_socket_address) < 0) {
            _observer.socket_error(M2MConnectionHandler::SOCKET_ABORT);
            return;
        }
    }

    _running = true;

    if (_security) {
        if (_security->resource_value_int(M2MSecurity::SecurityMode) == M2MSecurity::Certificate ||
            _security->resource_value_int(M2MSecurity::SecurityMode) == M2MSecurity::Psk) {

            if( _security_impl != NULL ){
                _security_impl->reset();
                if (_security_impl->init(_security) == 0) {
                    _is_handshaking = true;
                    tr_debug("M2MConnectionHandlerPimpl::resolve_server_address - connect DTLS");
                    if(_security_impl->start_connecting_non_blocking(_base) < 0 ){
                        tr_debug("M2MConnectionHandlerPimpl::dns_handler - handshake failed");
                        _is_handshaking = false;
                        _observer.socket_error(M2MConnectionHandler::SSL_CONNECTION_ERROR);
                        close_socket();
                        return;
                    }
                } else {
                    tr_error("M2MConnectionHandlerPimpl::resolve_server_address - init failed");
                    _observer.socket_error(M2MConnectionHandler::SSL_CONNECTION_ERROR, false);
                    close_socket();
                    return;
                }
            } else {
                tr_error("M2MConnectionHandlerPimpl::dns_handler - sec is null");
                _observer.socket_error(M2MConnectionHandler::SSL_CONNECTION_ERROR, false);
                close_socket();
                return;
            }
        }
    }
    if(!_is_handshaking) {
        enable_keepalive();
        _observer.address_ready(_address,
                                _server_type,
                                _address._port);
    }
}

void M2MConnectionHandlerPimpl::send_handler()
{
    tr_debug("M2MConnectionHandlerPimpl::send_handler()");
    _observer.data_sent();
}

bool M2MConnectionHandlerPimpl::send_data(uint8_t *data,
                                          uint16_t data_len,
                                          sn_nsdl_addr_s *address)
{
    tr_debug("M2MConnectionHandlerPimpl::send_data()");
    if (address == NULL || data == NULL) {
        return false;
    }

    uint8_t *buffer = (uint8_t*)malloc(data_len);
    if(!buffer) {
        return false;
    }

    TaskIdentifier* task = (TaskIdentifier*)malloc(sizeof(TaskIdentifier));
    if (!task) {
        free(buffer);
        return false;
    }
    task->pimpl = this;
    memcpy(buffer, data, data_len);
    task->data_ptr = buffer;
    arm_event_s event;
    event.receiver = M2MConnectionHandlerPimpl::_tasklet_id;
    event.sender = 0;
    event.event_type = ESocketSend;
    event.data_ptr = task;
    event.event_data = data_len;
    event.priority = ARM_LIB_HIGH_PRIORITY_EVENT;

    return eventOS_event_send(&event) == 0 ? true : false;
}

void M2MConnectionHandlerPimpl::send_socket_data(uint8_t *data,
                                                 uint16_t data_len)
{
    bool success = false;
    if( _use_secure_connection ){
        if( _security_impl->send_message(data, data_len) > 0){
            success = true;
        }
    } else {
        int32_t ret = -1;
        if(is_tcp_connection()){
            //We need to "shim" the length in front
            uint16_t d_len = data_len+4;
            uint8_t* d = (uint8_t*)malloc(data_len+4);

            d[0] = (data_len >> 24 )& 0xff;
            d[1] = (data_len >> 16 )& 0xff;
            d[2] = (data_len >> 8 )& 0xff;
            d[3] = data_len & 0xff;
            memmove(d+4, data, data_len);
            ret = ((TCPSocket*)_socket)->send(d,d_len);
            free(d);
        }else {
            ret = ((UDPSocket*)_socket)->sendto(*_socket_address,data, data_len);
        }
        if (ret > 0) {
            success = true;
        }
    }

    if (!success) {
        _observer.socket_error(M2MConnectionHandler::SOCKET_SEND_ERROR, true);
        close_socket();
    }
}

int8_t M2MConnectionHandlerPimpl::connection_tasklet_handler()
{
    return M2MConnectionHandlerPimpl::_tasklet_id;
}

void M2MConnectionHandlerPimpl::socket_event()
{
    tr_debug("M2MConnectionHandlerPimpl::socket_event()");

    TaskIdentifier* task = (TaskIdentifier*)malloc(sizeof(TaskIdentifier));
    if (!task) {
    	_observer.socket_error(M2MConnectionHandler::SOCKET_READ_ERROR, true);
        return;
    }
    task->pimpl = this;

    arm_event_s event;
    event.receiver = M2MConnectionHandlerPimpl::_tasklet_id;
    event.sender = 0;
    event.event_type = ESocketReadytoRead;
    event.data_ptr = task;
    event.priority = ARM_LIB_HIGH_PRIORITY_EVENT;
    int8_t error = eventOS_event_send(&event);
    if(error != 0) {
    	_observer.socket_error(M2MConnectionHandler::SOCKET_READ_ERROR, true);
    }
}

bool M2MConnectionHandlerPimpl::start_listening_for_data()
{
    tr_debug("M2MConnectionHandlerPimpl::start_listening_for_data()");
    // Boolean return required for other platforms,
    // not needed in mbed OS Socket.
    _listening = true;
    _running = true;
    return _listening;
}

void M2MConnectionHandlerPimpl::stop_listening()
{
    tr_debug("M2MConnectionHandlerPimpl::stop_listening()");
    _listening = false;
    if(_security_impl) {
        _security_impl->reset();
    }
}

int M2MConnectionHandlerPimpl::send_to_socket(const unsigned char *buf, size_t len)
{
    tr_debug("M2MConnectionHandlerPimpl::send_to_socket len - %d", len);
    int size = -1;
    if(is_tcp_connection()) {
        size = ((TCPSocket*)_socket)->send(buf,len);
    } else {
        size = ((UDPSocket*)_socket)->sendto(*_socket_address,buf,len);
    }
    tr_debug("M2MConnectionHandlerPimpl::send_to_socket size - %d", size);
    if(NSAPI_ERROR_WOULD_BLOCK == size){
        if(_is_handshaking) {
            return M2MConnectionHandler::CONNECTION_ERROR_WANTS_WRITE;
        } else {
            return len;
        }
    }else if(size < 0){
        return -1;
    }else{
        if(!_is_handshaking) {
            _observer.data_sent();
        }
        return size;
    }
}

int M2MConnectionHandlerPimpl::receive_from_socket(unsigned char *buf, size_t len)
{
    tr_debug("M2MConnectionHandlerPimpl::receive_from_socket");
    int recv = -1;
    if(is_tcp_connection()) {
        recv = ((TCPSocket*)_socket)->recv(buf, len);
    } else {
        recv = ((UDPSocket*)_socket)->recvfrom(NULL,buf, len);
    }
    tr_debug("M2MConnectionHandlerPimpl::receive_from_socket recv size %d", recv);
    if(NSAPI_ERROR_WOULD_BLOCK == recv){
        return M2MConnectionHandler::CONNECTION_ERROR_WANTS_READ;
    }else if(recv < 0){
        return -1;
    }else{
        return recv;
    }
}

void M2MConnectionHandlerPimpl::handle_connection_error(int error)
{
    tr_debug("M2MConnectionHandlerPimpl::handle_connection_error");
    _observer.socket_error(error);
}

void M2MConnectionHandlerPimpl::set_platform_network_handler(void *handler)
{
    tr_debug("M2MConnectionHandlerPimpl::set_platform_network_handler");
    _net_iface = (NetworkInterface*)handler;
}

void M2MConnectionHandlerPimpl::receive_handshake_handler()
{
    tr_debug("M2MConnectionHandlerPimpl::receive_handshake_handler()");
    if( _is_handshaking ){
        int ret = _security_impl->continue_connecting();
        tr_debug("M2MConnectionHandlerPimpl::receive_handshake_handler() - ret %d", ret);
        if( ret == M2MConnectionHandler::CONNECTION_ERROR_WANTS_READ ){ //We wait for next readable event
            tr_debug("M2MConnectionHandlerPimpl::receive_handshake_handler() - We wait for next readable event");
            return;
        } else if( ret == 0 ){
            _is_handshaking = false;
            _use_secure_connection = true;
            enable_keepalive();
            _observer.address_ready(_address,
                                    _server_type,
                                    _server_port);
        }else if( ret < 0 ){
            _is_handshaking = false;
            _observer.socket_error(M2MConnectionHandler::SSL_CONNECTION_ERROR, true);
            close_socket();
        }
    }
}

bool M2MConnectionHandlerPimpl::is_handshake_ongoing()
{
    return _is_handshaking;
}

void M2MConnectionHandlerPimpl::receive_handler()
{
    tr_debug("M2MConnectionHandlerPimpl::receive_handler()");
    memset(_recv_buffer, 0, 1024);
    size_t receive_length = sizeof(_recv_buffer);

    if(_listening) {
        if( _use_secure_connection ){
            int rcv_size = _security_impl->read(_recv_buffer, receive_length);

            if(rcv_size >= 0){
                _observer.data_available((uint8_t*)_recv_buffer,
                                         rcv_size, _address);
            } else if (M2MConnectionHandler::CONNECTION_ERROR_WANTS_READ != rcv_size) {
                _observer.socket_error(M2MConnectionHandler::SOCKET_READ_ERROR, true);
                close_socket();
                return;
            }
        }else{
            int recv = -1;
            if(is_tcp_connection()){
                recv = ((TCPSocket*)_socket)->recv(_recv_buffer, receive_length);

            }else{
                recv = ((UDPSocket*)_socket)->recvfrom(NULL,_recv_buffer, receive_length);
            }
            if (recv > 0) {
                // Send data for processing.
                if(is_tcp_connection()){
                    //We need to "shim" out the length from the front
                    if( receive_length > 4 ){
                        uint64_t len = (_recv_buffer[0] << 24 & 0xFF000000) + (_recv_buffer[1] << 16 & 0xFF0000);
                        len += (_recv_buffer[2] << 8 & 0xFF00) + (_recv_buffer[3] & 0xFF);
                        if(len > 0) {
                            uint8_t* buf = (uint8_t*)malloc(len);
                            if(buf) {
                                memmove(buf, _recv_buffer+4, len);
                                // Observer for TCP plain mode
                                _observer.data_available(buf,len,_address);
                                free(buf);
                            }
                        }
                    }else{
                        _observer.socket_error(M2MConnectionHandler::SOCKET_READ_ERROR, true);
                        close_socket();
                    }
                } else { // Observer for UDP plain mode
                    tr_debug("M2MConnectionHandlerPimpl::receive_handler - data received %d", recv);
                    _observer.data_available((uint8_t*)_recv_buffer,
                                             recv, _address);
                }
            } else if(NSAPI_ERROR_WOULD_BLOCK != recv) {
                // Socket error in receiving
                _observer.socket_error(M2MConnectionHandler::SOCKET_READ_ERROR, true);
                close_socket();
            }
        }
    }
}

void M2MConnectionHandlerPimpl::claim_mutex()
{
    eventOS_scheduler_mutex_wait();
}

void M2MConnectionHandlerPimpl::release_mutex()
{
    eventOS_scheduler_mutex_release();
}

void M2MConnectionHandlerPimpl::init_socket()
{
    tr_debug("M2MConnectionHandlerPimpl::init_socket - IN");
    _is_handshaking = false;
    _running = true;

    if(is_tcp_connection()) {
       tr_debug("M2MConnectionHandlerPimpl::init_socket - Using TCP");
        _socket = new TCPSocket(_net_iface);
        if(_socket) {
            _socket->attach(this, &M2MConnectionHandlerPimpl::socket_event);
        } else {
            _observer.socket_error(M2MConnectionHandler::SOCKET_ABORT);
            return;
        }
    } else {
       tr_debug("M2MConnectionHandlerPimpl::init_socket - Using UDP - port %d", _listen_port);
        _socket = new UDPSocket(_net_iface);
        if(_socket) {
            _socket->bind(_listen_port);
            _socket->attach(this, &M2MConnectionHandlerPimpl::socket_event);
        } else {
            _observer.socket_error(M2MConnectionHandler::SOCKET_ABORT);
            return;
        }
    }
    _socket->set_blocking(false);
    tr_debug("M2MConnectionHandlerPimpl::init_socket - OUT");
}

bool M2MConnectionHandlerPimpl::is_tcp_connection()
{
    return _binding_mode == M2MInterface::TCP ||
            _binding_mode == M2MInterface::TCP_QUEUE ? true : false;
}

void M2MConnectionHandlerPimpl::close_socket()
{
    tr_debug("M2MConnectionHandlerPimpl::close_socket() - IN");
    if(_socket) {
       _running = false;
        _socket->close();
        delete _socket;
        _socket = NULL;
    }
    tr_debug("M2MConnectionHandlerPimpl::close_socket() - OUT");
}

void M2MConnectionHandlerPimpl::enable_keepalive()
{
#if MBED_CLIENT_TCP_KEEPALIVE_TIME
    if(is_tcp_connection()) {
        int keepalive = MBED_CLIENT_TCP_KEEPALIVE_TIME;
        int enable = 1;
        tr_debug("M2MConnectionHandlerPimpl::resolve_hostname - keepalive %d s\n", keepalive);
        if(_socket->setsockopt(1,NSAPI_KEEPALIVE,&enable,sizeof(enable)) != 0) {
            tr_error("M2MConnectionHandlerPimpl::enable_keepalive - setsockopt fail to Set Keepalive\n");
        }
        if(_socket->setsockopt(1,NSAPI_KEEPINTVL,&keepalive,sizeof(keepalive)) != 0) {
            tr_error("M2MConnectionHandlerPimpl::enable_keepalive - setsockopt fail to Set Keepalive TimeInterval\n");
        }
        if(_socket->setsockopt(1,NSAPI_KEEPIDLE,&keepalive,sizeof(keepalive)) != 0) {
            tr_error("M2MConnectionHandlerPimpl::enable_keepalive - setsockopt fail to Set Keepalive Time\n");
        }
    }
#endif
}
