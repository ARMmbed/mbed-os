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

/*
 * \file coap_service.c
 * \brief Add short description about this file!!!
 *
 */

#include <string.h>
#include <ns_types.h>
#include "ns_list.h"
#include "sn_coap_header.h"
#include "coap_service_api.h"
#include "coap_server.h"

#include <ns_trace.h>

int8_t coap_service_initialize(int8_t interface_id, uint16_t listen_port, uint8_t service_options)
{
	int8_t retVal = -1;

	retVal = coap_server_service_init(interface_id, listen_port, service_options);

	return retVal;
}

int8_t coap_service_register_uri(int8_t service_id, const char *uri, uint8_t allowed_method, coap_service_request_recv_cb *request_recv_cb)
{
	return -1;
}

int8_t coap_service_unregister_uri(int8_t service_id, const char *uri)
{
	return -1;
}

uint16_t coap_service_send(int8_t service_id, uint8_t options, const uint8_t addr[static 16], uint16_t destination_port, sn_coap_hdr_s *request_ptr, coap_service_response_recv *request_response_cb)
{
	return 0;
}

void coap_service_delete( int8_t service_id )
{

}

int requst_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
	return -1;
}


int security_start_ptr(int8_t service_id, uint8_t EUI64[static 8])
{
	return -1;
}

int security_done_cb(int8_t service_id, uint8_t EUI64[static 8], uint8_t keyblock[static 40])
{
	return -1;
}

int coap_service_security_key_set(int8_t service_id, uint8_t EUI64[static 8], uint8_t *PSKd_ptr, uint8_t PSKd_len)
{

	return -1;
}

int requst_response_cb(uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{

	return -1;
}

int virtual_socket_send(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
	return -1;
}

int16_t coap_service_virtual_socket_recv(int8_t service_id, uint8_t source_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
	return -1;
}

int16_t coap_service_virtual_socket_set_cb(int8_t service_id, coap_service_virtual_socket_send_cb *virtual_socket_send)
{

	return -1;
}

int8_t coap_service_register_uri_secure_cb_set(int8_t service_id, coap_service_security_start_cb *security_start_ptr, coap_service_security_done_cb *security_done_cb)
{
	return -1;
}

uint16_t coap_service_request_send(int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16], uint16_t destination_port, uint8_t msg_type, uint8_t msg_code, const char *uri,
	                               uint8_t cont_type, const uint8_t *payload_ptr, uint16_t payload_len, coap_service_response_recv *request_response_cb)
{
	return 0;
}

int8_t coap_service_response_send(int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code, int32_t content_type, const uint8_t *payload_ptr,uint16_t payload_len)
{
	return -1;
}
