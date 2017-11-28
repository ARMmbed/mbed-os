/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#ifdef HAVE_THREAD
#include <string.h>

#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "coap_service_api.h"
#include "coap_message_handler.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "common_functions.h"
#include "net_interface.h"
#include "coap_service_api_stub.h"
uint8_t callBackSent;
coap_service_api_stub_def coap_service_api_stub;


int8_t coap_service_initialize(int8_t interface_id, uint16_t listen_port, uint8_t service_options,
                                 coap_service_security_start_cb *start_ptr, coap_service_security_done_cb *security_done_cb)
{
    return coap_service_api_stub.expectedInt8;
}

void coap_service_delete(int8_t service_id)
{
}

int16_t coap_service_virtual_socket_recv(int8_t service_id, uint8_t source_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
    return coap_service_api_stub.expectedInt16;
}

int16_t coap_service_virtual_socket_set_cb(int8_t service_id, coap_service_virtual_socket_send_cb *send_method_ptr)
{
    coap_service_api_stub.socket_send_cb = send_method_ptr;
    return coap_service_api_stub.expectedInt16;
}

int8_t coap_service_register_uri(int8_t service_id, const char *uri, uint8_t allowed_method, coap_service_request_recv_cb *request_recv_cb)
{
    if (coap_service_api_stub.callBackAccepted == callBackSent) {
    coap_service_api_stub.register_uri_send_cb = request_recv_cb;
    }
    callBackSent++;
    return coap_service_api_stub.expectedInt8;
}

int8_t coap_service_unregister_uri(int8_t service_id, const char *uri)
{
    return coap_service_api_stub.expectedInt8;
}

uint16_t coap_service_request_send(int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16], uint16_t destination_port, sn_coap_msg_type_e msg_type, sn_coap_msg_code_e msg_code, const char *uri,
        sn_coap_content_format_e cont_type, const uint8_t *payload_ptr, uint16_t payload_len, coap_service_response_recv *request_response_cb)
{
    coap_service_api_stub.response_resp_cb = request_response_cb;
    return coap_service_api_stub.expectedUint16;
}

int8_t coap_service_response_send(int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code, int32_t content_type, const uint8_t *payload_ptr,uint16_t payload_len){
    return coap_service_api_stub.expectedInt8;
}
void coap_service_close_secure_connection(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port)
{
    return;
}
void reset_callback()
{
    callBackSent = 1;
}

int8_t coap_service_request_delete(int8_t service_id, uint16_t msg_id)
{
    return -1;
}

#endif
