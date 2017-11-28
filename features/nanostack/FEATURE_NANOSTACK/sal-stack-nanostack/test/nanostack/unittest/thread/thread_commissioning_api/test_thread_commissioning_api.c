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
#include "test_thread_commissioning_api.h"
#include <string.h>
#include "thread_commissioning_api.h"
#include "nsdynmemLIB_stub.h"
#include "coap_service_api_stub.h"
#include "thread_meshcop_lib_stub.h"
#include "nsconfig.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "sn_coap_header.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "protocol_core_stub.h"


int commissioner_joiner_finalisation_handler_cb(int8_t interface_id, uint8_t EUI64[8], uint8_t *message_ptr, uint16_t message_len){
    return 0;
}
void thread_commissioning_native_select(int8_t interface_id, uint8_t count, thread_commissioning_link_configuration_s *link_ptr ){
    return;
}
/*This test case does the covers the commissioner register and device add methods*/
bool test_commissioning_device_add_and_remove()
{
    uint8_t pskd[32] = {0x00};
    int joining_device_cb = 1;
    uint8_t interface_id = 5;
    uint8_t EUI64[] = {0x12,0x13,0x14,0x15,0x12,0x13,0x14,0x15};
    nsdynmemlib_stub.returnCounter = 1;
    uint8_t pw[16];
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;
    nsdynmemlib_stub.returnCounter = 0;
    if( -2 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  &joining_device_cb) )
            return false;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  &joining_device_cb) )
            return false;
    if ( 0 != thread_commissioning_device_delete(interface_id, EUI64))
        return false;
    if ( 0 != thread_commissioning_unregister(interface_id))
            return false;
    /*setting up test case for covering bloom fliter which can add all devices*/
    for (int i = 0; i < 8; i++)
            EUI64[i] = 0xff;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  &joining_device_cb) )
            return false;
    if ( 0 != thread_commissioning_device_delete(interface_id, EUI64))
            return false;
    if ( 0 != thread_commissioning_unregister(interface_id))
            return false;
    return true;
}

/*test case to add commissioning device without a commissioner */

bool test_commissioning_device_add()
{
    uint8_t pskd[32] = {0x00};
    //int joining_device_cb = commissioner_joiner_finalisation_handler_cb;
    uint8_t interface_id = 5;
    uint8_t EUI64[] = {0x12,0x13,0x14,0x15,0x12,0x13,0x14,0x15};
    /*commissioner is not created before device is added */
    nsdynmemlib_stub.returnCounter = 1;
    if( -1 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  commissioner_joiner_finalisation_handler_cb) )
        return false;
    /*now register a commissioner */
    uint8_t pw[16];
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;
    /* PSKd_len is less than 1 */
    nsdynmemlib_stub.returnCounter = 1;
    if( -1 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 0,  commissioner_joiner_finalisation_handler_cb) )
        return false;
    /* PSKd_len is greater than 32 */
    nsdynmemlib_stub.returnCounter = 1;
    if( -1 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 33,  commissioner_joiner_finalisation_handler_cb) )
        return false;
    /*unregister the commissioner */
    if (0 != thread_commissioning_unregister(interface_id))
        return false;
    return true;
}

/*test case to cover a deletion of a device with incorrect interface id */

bool test_commissioning_device_delete()
{
    uint8_t interface_id = 5;
    uint8_t EUI64[] = {0x12,0x13,0x14,0x15,0x12,0x13,0x14,0x15};
    if (-1 != thread_commissioning_device_delete(interface_id,EUI64))
        return false;
    return true;
}

/* test case to cover registering of a commissioner that already exists  &
unregistering of a non existent commissioner*/

bool test_commissioning_register_twice()
{
    uint8_t interface_id = 5;
    nsdynmemlib_stub.returnCounter = 1;
    uint8_t pw[16];
    if ( 0 != thread_commissioning_register(interface_id, pw) )
        return false;
    nsdynmemlib_stub.returnCounter = 1;
    if ( -1 != thread_commissioning_register(interface_id, pw) )
        return false;
    nsdynmemlib_stub.returnCounter = 1;
    if ( 0 != thread_commissioning_unregister(interface_id))
            return false;
    nsdynmemlib_stub.returnCounter = 1;
    if ( -1 != thread_commissioning_unregister(interface_id))
            return false;
    return true;
}

bool test_thread_commissioning_petition_start()
{
    int8_t interface_id = 6;
    int status_cb_ptr = 1;
    char commissioner_id_ptr[1];
    commissioner_id_ptr[0] = 'a';
    char commissioner_id_ptr_long[65];

    if ( -1 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, &status_cb_ptr))
        return false;
    nsdynmemlib_stub.returnCounter = 1;
    /*now register a commissioner */
    uint8_t pw[16];
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;
    if ( -2 != thread_commissioning_petition_start(interface_id,NULL,&status_cb_ptr))
            return false;

    for (int i = 0; i<65; i++){
        commissioner_id_ptr_long[i] = 'a';
    }
    if ( -3 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr_long, &status_cb_ptr))
            return false;
    if ( 0 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, &status_cb_ptr))
            return false;

    if ( 0 != thread_commissioning_unregister(interface_id))
        return false;

    return true;
}

bool test_thread_commissioning_rx_receive_cb()
{
    uint8_t pskd[32] = {0x00};
    int8_t interface_id = 1;
    thread_commissioning_joiner_finalisation_cb joining_device_cb;
    uint8_t EUI64[] = {0xfe,0x80,0xff,0xff,0xff,0xff,0xff,0xff};
    //set coap service stub values
    coap_service_api_stub.response_resp_cb = NULL;
    coap_service_api_stub.socket_send_cb = NULL;
    coap_service_api_stub.register_uri_send_cb = NULL;
    coap_service_api_stub.expectedInt8 = interface_id;

    //set meshcop stub values
    thread_meshcop_lib_stub.expectedInt8 = 2;
    thread_meshcop_lib_stub.expectedUint16 = 8;
    thread_meshcop_lib_stub.expectedUnit8ResultValue = 1;


    int status_cb_ptr = 1;
    char commissioner_id_ptr[1];
    commissioner_id_ptr[0] = 'a';
    uint8_t data_ptr[3];
    int8_t service_id = 0;
    uint8_t source_address[] = {0xfe,0x80,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf,0xff};
    uint16_t source_port = 1;
    uint8_t payload[] = {0x11,0x12,0x13,0x16,0x01};
    sn_coap_hdr_s response_ptr;
    response_ptr.payload_ptr = payload;
    response_ptr.payload_len = 5;
    sn_coap_hdr_s request_ptr;
    request_ptr.payload_ptr = payload;
    request_ptr.payload_len = 5;
    nsdynmemlib_stub.returnCounter = 4;
    uint8_t pw[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    reset_callback();
    coap_service_api_stub.callBackAccepted = 2;
    /*now register a commissioner */
    if( 0 != thread_commissioning_register(interface_id, pw) ) {
        return false;
    }
    if( 0 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  commissioner_joiner_finalisation_handler_cb) ) {
            return false;
    }
    if (!coap_service_api_stub.socket_send_cb) {
        return false;
    }
    coap_service_api_stub.socket_send_cb(interface_id, source_address, source_port, data_ptr, 3);

    if (!coap_service_api_stub.register_uri_send_cb) {
        return false;
    }
    coap_service_api_stub.register_uri_send_cb(interface_id, source_address, source_port, &request_ptr);

    if ( 0 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, NULL)){
        return false;
    }
    //test here -->
    if (!coap_service_api_stub.response_resp_cb) {
        return false;
    }

    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    response_ptr.payload_ptr = NULL;
    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    //<-- test here

    if ( 0 != thread_commissioning_device_delete(interface_id, EUI64)) {
        return false;
    }
    if ( 0 != thread_commissioning_unregister(interface_id)){
        return false;
    }
    return true;
}

bool test_thread_commissioning_application_provision_cb()
{
    uint8_t pskd[32] = {0x00};
    int8_t interface_id = 1;
    thread_commissioning_joiner_finalisation_cb joining_device_cb;
    uint8_t EUI64[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    //set coap service stub values
    coap_service_api_stub.response_resp_cb = NULL;
    coap_service_api_stub.socket_send_cb = NULL;
    coap_service_api_stub.register_uri_send_cb = NULL;
    coap_service_api_stub.expectedInt8 = interface_id;
    coap_service_api_stub.callBackAccepted = 3;
    //set meshcop stub values
    thread_meshcop_lib_stub.expectedInt8 = 2;
    thread_meshcop_lib_stub.expectedUint16 = 1;
    thread_meshcop_lib_stub.expectedUnit8ResultValue = 1;


    int status_cb_ptr = 1;
    char commissioner_id_ptr[1];
    commissioner_id_ptr[0] = 'a';
    uint8_t data_ptr[3];
    int8_t service_id = 0;
    uint8_t source_address[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    uint16_t source_port = 1;
    uint8_t payload[] = {0x11,0x12,0x13,0x16,0x01};
    sn_coap_hdr_s response_ptr;
    response_ptr.payload_ptr = payload;
    response_ptr.payload_len = 5;
    sn_coap_hdr_s request_ptr;
    request_ptr.payload_ptr = payload;
    request_ptr.payload_len = 5;
    nsdynmemlib_stub.returnCounter = 4;
    uint8_t pw[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    reset_callback();

    /*now register a commissioner */
    if( 0 != thread_commissioning_register(interface_id, pw) ) {
        return false;
    }
    if( 0 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  commissioner_joiner_finalisation_handler_cb) ) {
            return false;
    }
    if (!coap_service_api_stub.socket_send_cb) {
        return false;
    }
    coap_service_api_stub.socket_send_cb(interface_id, source_address, source_port, data_ptr, 3);

    if (!coap_service_api_stub.register_uri_send_cb) {
        return false;
    }
    coap_service_api_stub.register_uri_send_cb(interface_id, source_address, source_port, &request_ptr);

    if ( 0 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, NULL)){
        return false;
    }
    //test here -->
    if (!coap_service_api_stub.response_resp_cb) {
        return false;
    }

    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    response_ptr.payload_ptr = NULL;
    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    //<-- test here

    if ( 0 != thread_commissioning_device_delete(interface_id, EUI64)) {
        return false;
    }
    if ( 0 != thread_commissioning_unregister(interface_id)){
        return false;
    }
    return true;
}

bool test_thread_commissioning_finalisation_req_recv_cb()
{
    uint8_t pskd[32] = {0x00};
    int8_t interface_id = 1;
    thread_commissioning_joiner_finalisation_cb joining_device_cb;
    uint8_t EUI64[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    //set coap service stub values
    reset_callback();
    coap_service_api_stub.response_resp_cb = NULL;
    coap_service_api_stub.socket_send_cb = NULL;
    coap_service_api_stub.register_uri_send_cb = NULL;
    coap_service_api_stub.expectedInt8 = interface_id;
    coap_service_api_stub.callBackAccepted = 5;
    //set meshcop stub values
    thread_meshcop_lib_stub.expectedInt8 = 2;
    thread_meshcop_lib_stub.expectedUint16 = 1;
    thread_meshcop_lib_stub.expectedUnit8ResultValue = 1;


    int status_cb_ptr = 1;
    char commissioner_id_ptr[1];
    commissioner_id_ptr[0] = 'a';
    uint8_t data_ptr[3];
    int8_t service_id = 0;
    uint8_t source_address[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    uint16_t source_port = 1;
    uint8_t payload[] = {0x11,0x12,0x13,0x16,0x01};
    sn_coap_hdr_s response_ptr;
    response_ptr.payload_ptr = payload;
    response_ptr.payload_len = 5;
    sn_coap_hdr_s request_ptr;
    request_ptr.payload_ptr = payload;
    request_ptr.payload_len = 5;
    nsdynmemlib_stub.returnCounter = 4;
    uint8_t pw[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    /*now register a commissioner */
    if( 0 != thread_commissioning_register(interface_id, pw) ) {
        return false;
    }
    if( 0 != thread_commissioning_device_add(interface_id, true, EUI64,  pskd, 32,  commissioner_joiner_finalisation_handler_cb) ) {
            return false;
    }
    if (!coap_service_api_stub.socket_send_cb) {
        return false;
    }
    coap_service_api_stub.socket_send_cb(interface_id, source_address, source_port, data_ptr, 3);

    if (!coap_service_api_stub.register_uri_send_cb) {
        return false;
    }
    coap_service_api_stub.register_uri_send_cb(interface_id, source_address, source_port, &request_ptr);

    if ( 0 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, NULL)){
        return false;
    }
    //test here -->
    if (!coap_service_api_stub.response_resp_cb) {
        return false;
    }

    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    response_ptr.payload_ptr = NULL;
    coap_service_api_stub.response_resp_cb(interface_id, source_address, source_port, &response_ptr);
    //<-- test here

    if ( 0 != thread_commissioning_device_delete(interface_id, EUI64)) {
        return false;
    }
    if ( 0 != thread_commissioning_unregister(interface_id)){
        return false;
    }

    return true;
}

bool test_thread_commissioning_petition_keep_alive()
{

    int8_t interface_id = 7;
    commissioning_state_e state = COMMISSIONING_STATE_ACCEPT;
    char commissioner_id_ptr[1];
    int status_cb_ptr = 1;
    commissioner_id_ptr[0] = 'a';
    if (-1 != thread_commissioning_petition_keep_alive(interface_id, state))
        return false;
    nsdynmemlib_stub.returnCounter = 1;
    uint8_t pw[16];
    /*now register a commissioner */
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;

    if ( 0 != thread_commissioning_petition_start(interface_id,commissioner_id_ptr, &status_cb_ptr))
            return false;
    if (0 != thread_commissioning_petition_keep_alive(interface_id, state))
        return false;

    if ( 0 != thread_commissioning_unregister(interface_id))
        return false;

    return true;

}

bool test_thread_commissioning_get_management_id()
{
    int8_t interface_id = 8;
    if (-1 != thread_commissioning_get_management_id(interface_id))
        return false;

    nsdynmemlib_stub.returnCounter = 1;
    uint8_t pw[16];
    /*now register a commissioner */
    if( 0 != thread_commissioning_register(interface_id, pw) )
        return false;

    if (0 != thread_commissioning_get_management_id(interface_id))
        return false;

    if ( 0 != thread_commissioning_unregister(interface_id))
        return false;

    return true;
}

bool test_thread_commissioning_native_commissioner_start()
{
    int8_t interface_id = 1;
    protocol_interface_info_entry_t cur;
    thread_info_t thread_info;
    thread_info.version = 1;
    cur.id = interface_id;
    cur.thread_info = NULL;
    protocol_core_stub.entry_ptr = &cur;

    if (-1 != thread_commissioning_native_commissioner_start(interface_id, NULL)) {
        return false;
    }
    cur.thread_info = &thread_info;
    protocol_core_stub.entry_ptr = &cur;
    if (0 != thread_commissioning_native_commissioner_start(interface_id, thread_commissioning_native_select)) {
        return false;
    }

    return true;
}
bool test_thread_commissioning_native_commissioner_connect()
{
    thread_commissioning_link_configuration_s link_configuration;
    int8_t interface_id = 1;
    protocol_interface_info_entry_t cur;
    thread_info_t thread_info;
    thread_info.version = 1;
    thread_info.thread_attached_state = THREAD_STATE_CONNECTED;
    thread_info.native_commissioner_port = 10;
    uint8_t address_ptr;
    uint16_t port;
    cur.id = interface_id;
    cur.thread_info = NULL;
    protocol_core_stub.entry_ptr = NULL;
    if (-1 != thread_commissioning_native_commissioner_connect(interface_id, &link_configuration)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    cur.thread_info = &thread_info;
    protocol_core_stub.entry_ptr = &cur;
    if (0 != thread_commissioning_native_commissioner_connect(interface_id, &link_configuration)) {
        return false;
    }
    if (0 != thread_commissioning_native_commissioner_stop(interface_id)) {
        return false;
    }
    if (0 != thread_commissioning_native_commissioner_get_connection_info(interface_id,&address_ptr,&port)){
        return false;
    }

    return true;
}
