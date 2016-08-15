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
#include "common_stub.h"
#include "sn_grs.h"
#include <sys/socket.h>

socket_error_t common_stub::error;
socket_event_t * common_stub::event;
socket_addr * common_stub::addr;
void * common_stub::void_value;
bool common_stub::visited;
bool common_stub::bool_value;
int common_stub::int_value;
int common_stub::int2_value;
addrinfo* common_stub::addrinfo;
uint16_t common_stub::uint_value;
omalw_certificate_list_t *common_stub::cert;
sn_coap_hdr_s *common_stub::coap_header;
sn_nsdl_resource_info_s *common_stub::resource;
pthread_t common_stub::thread;
const char* common_stub::char_value;

using namespace mbed;
using namespace mbed::Sockets::v0;

void common_stub::clear()
{
    int_value = -1;
    int2_value = -1;
    uint_value = 0;
    error = SOCKET_ERROR_NONE;
    event = NULL;
    addr = NULL;
    void_value = NULL;
    cert = NULL;
    visited = false;
    bool_value= false;
    coap_header = NULL;
    resource = NULL;
    addrinfo = NULL;
    char_value = NULL;
}

UDPSocket::UDPSocket(socket_stack_t stack) :Socket(stack)
{
}

UDPSocket::~UDPSocket()
{
}

socket_error_t UDPSocket::connect(const SocketAddr *, const uint16_t )
{
    return common_stub::error;
}

//SocketAddr

void SocketAddr::setAddr(const struct socket_addr *)
{
}

void SocketAddr::setAddr(const SocketAddr *)
{
}

bool SocketAddr::SocketAddr::is_v4()
{
    return common_stub::bool_value;
}

void mbed::Ticker::detach()
{
    common_stub::visited = true;
}


// SN_NSDL_LIB

struct nsdl_s *sn_nsdl_init	(uint8_t (*sn_nsdl_tx_cb)(struct nsdl_s *, sn_nsdl_capab_e , uint8_t *, uint16_t, sn_nsdl_addr_s *),
                             uint8_t (*sn_nsdl_rx_cb)(struct nsdl_s *, sn_coap_hdr_s *, sn_nsdl_addr_s *),
                             void *(*sn_nsdl_alloc)(uint16_t),void (*sn_nsdl_free)(void *))
{
    return NULL;
}

uint16_t sn_nsdl_register_endpoint(struct nsdl_s *, sn_nsdl_ep_parameters_s *)
{
    return common_stub::uint_value;
}

uint16_t sn_nsdl_unregister_endpoint(struct nsdl_s *)
{
    return common_stub::uint_value;
}

uint16_t sn_nsdl_update_registration(struct nsdl_s *,uint8_t *, uint8_t)
{
    return common_stub::uint_value;
}

uint16_t sn_nsdl_send_observation_notification(struct nsdl_s *, uint8_t *, uint8_t,
                                                    uint8_t *, uint16_t,
                                                    uint8_t *, uint8_t,
                                                    sn_coap_msg_type_e , uint8_t)
{
    return common_stub::uint_value;
}

int8_t sn_nsdl_destroy(struct nsdl_s *handle)
{
    return common_stub::int_value;
}

sn_coap_hdr_s *sn_nsdl_build_response(struct nsdl_s *, sn_coap_hdr_s *, uint8_t )
{
    return common_stub::coap_header;
}

sn_nsdl_resource_info_s *sn_nsdl_get_resource(struct nsdl_s *, uint16_t, uint8_t *)
{
    return common_stub::resource;
}

int8_t sn_nsdl_process_coap(struct nsdl_s *, uint8_t *, uint16_t , sn_nsdl_addr_s *)
{
    return common_stub::int_value;
}

int8_t sn_nsdl_exec(struct nsdl_s *, uint32_t)
{
    return common_stub::int_value;
}

void sn_nsdl_release_allocated_coap_msg_mem(struct nsdl_s *, sn_coap_hdr_s *header)
{
    if(header && header != common_stub::coap_header){
        if( header->content_type_ptr ){
            free(header->content_type_ptr);
            header->content_type_ptr = NULL;
        }
        if( header->options_list_ptr){
            free(header->options_list_ptr);
            header->options_list_ptr = NULL;
        }
        free(header);
        header = NULL;
    }
}

int8_t sn_nsdl_create_resource(struct nsdl_s *, sn_nsdl_resource_info_s *)
{
    return common_stub::int_value;
}

int8_t sn_nsdl_delete_resource(struct nsdl_s *, uint16_t , uint8_t *)
{
    return common_stub::int_value;
}

int8_t sn_nsdl_send_coap_message(struct nsdl_s *, sn_nsdl_addr_s *, sn_coap_hdr_s *)
{
    return common_stub::int_value;
}

int8_t sn_nsdl_update_resource(struct nsdl_s *, sn_nsdl_resource_info_s *)
{
    return common_stub::int_value;
}

int8_t set_NSP_address(struct nsdl_s *, uint8_t *, uint16_t, sn_nsdl_addr_type_e)
{
    return common_stub::int_value;
}

uint16_t sn_nsdl_oma_bootstrap(struct nsdl_s *, sn_nsdl_addr_s *,
                             sn_nsdl_ep_parameters_s *,
                             sn_nsdl_bs_ep_info_t *)
{
    return common_stub::uint_value;
}

omalw_certificate_list_t *sn_nsdl_get_certificates(struct nsdl_s *)
{
    return common_stub::cert;
}

int8_t sn_nsdl_update_certificates(struct nsdl_s *, omalw_certificate_list_t*, uint8_t)
{
    return common_stub::int_value;
}

int8_t sn_nsdl_create_oma_device_object(struct nsdl_s *, sn_nsdl_oma_device_t *)
{
    return common_stub::int_value;
}

//Coap Headers

sn_coap_hdr_s *sn_coap_build_response(struct coap_s *, sn_coap_hdr_s *, uint8_t )
{
    return common_stub::coap_header;
}

void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *, sn_coap_hdr_s *)
{
}

int8_t sn_coap_protocol_set_retransmission_parameters(uint8_t, uint8_t)
{
    return common_stub::int_value;
}

// IP6String.h
uint_fast8_t ip6tos(const void *, char *)
{
}

//Socket
Socket::Socket(const socket_stack_t ) : _irq(this)
{
    _socket.api = NULL;
}

Socket::~Socket()
{
}

socket_error_t Socket::open(const socket_address_family_t , const socket_proto_family_t )
{
    return common_stub::error;
}

bool Socket::error_check(socket_error_t )
{
    return common_stub::bool_value;
}

void Socket::_eventHandler(struct socket_event *)
{
}

void Socket::setOnError(const ErrorHandler_t &)
{
}

void Socket::setOnReadable(const ReadableHandler_t &)
{
}

void Socket::setOnSent(const SentHandler_t & )
{
}

void Socket::_nvEventHandler(void * )
{
}

socket_error_t Socket::getLocalAddr(SocketAddr *) const
{
    return common_stub::error;
}

socket_error_t Socket::getLocalPort(uint16_t *) const
{
    return common_stub::error;
}

socket_error_t Socket::getRemoteAddr(SocketAddr *) const
{
    return common_stub::error;
}

socket_error_t Socket::getRemotePort(uint16_t *) const
{
    return common_stub::error;
}

socket_error_t Socket::resolve(const char* , const DNSHandler_t & )
{
    return common_stub::error;
}

socket_error_t Socket::bind(const char * , const uint16_t )
{
    return common_stub::error;
}
socket_error_t Socket::bind(const SocketAddr * , const uint16_t )
{
    return common_stub::error;
}

socket_error_t Socket::close()
{
    return common_stub::error;
}

socket_error_t Socket::recv(void * , size_t *)
{
    return common_stub::error;
}
socket_error_t Socket::recv_from(void * , size_t *, SocketAddr *, uint16_t *)
{
    return common_stub::error;
}

socket_error_t Socket::send(const void * , const size_t )
{
    return common_stub::error;
}
socket_error_t Socket::send_to(const void * , const size_t , const SocketAddr *, uint16_t )
{
    return common_stub::error;
}

bool Socket::isConnected() const {
    return common_stub::error;
}

// pthread
int pthread_equal (pthread_t , pthread_t ) __THROW
{
    return common_stub::int2_value;
}

pthread_t pthread_self (void) __THROW
{
    return common_stub::thread;
}

int pthread_detach (pthread_t) __THROW
{
    return common_stub::int_value;
}

int pthread_create (pthread_t * ,
                    const pthread_attr_t *,
                    void *(*__start_routine) (void *),
                    void *) __THROWNL
{
    __start_routine(NULL);
    return common_stub::int_value;
}

int pthread_cancel (pthread_t)
{
    return common_stub::int_value;
}

int pthread_join (pthread_t , void **)
{
    return common_stub::int_value;
}

void pthread_exit (void *__retval)/* __attribute__ ((__noreturn__))*/
{

}

// sys/socket
#ifndef __SOCKADDR_ARG
# define __SOCKADDR_ARG		struct sockaddr *__restrict
#endif

#ifndef __CONST_SOCKADDR_ARG
# define __CONST_SOCKADDR_ARG	const struct sockaddr *
#endif

#ifndef __socklen_t_defined
typedef __socklen_t socklen_t;
# define __socklen_t_defined
#endif

int bind (int , __CONST_SOCKADDR_ARG , socklen_t )
     __THROW
{
    return common_stub::int_value;
}

ssize_t sendto (int , const void *, size_t ,
                               int , __CONST_SOCKADDR_ARG ,
                               socklen_t )
{
    return common_stub::int_value;
}

ssize_t recvfrom (int , void * , size_t ,
             int , __SOCKADDR_ARG ,
             socklen_t * )
{
    return common_stub::int_value;
}

int getaddrinfo (const char *__restrict,
            const char *__restrict,
            const struct addrinfo *__restrict,
            struct addrinfo **__restrict addr)
{
    //*addr = (addrinfo*)malloc(sizeof(addrinfo));
    //(*addr)->ai_addr = (sockaddr*)malloc(sizeof(sockaddr));
    *addr = common_stub::addrinfo;
    return common_stub::int_value;
}

void freeaddrinfo (struct addrinfo * addr)
__THROW
{
//    free(addr->ai_addr);
//    free(addr);
}

const char *inet_ntop (int, const void *__restrict,
                  char *__restrict, socklen_t)
     __THROW
{
    return common_stub::char_value;
}

int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
{
    return common_stub::int_value;
}

