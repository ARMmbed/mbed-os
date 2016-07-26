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
#include "CppUTest/TestHarness.h"
#include "test_nsdlaccesshelper.h"
#include "common_stub.h"
#include "m2mnsdlinterface_stub.h"
#include "m2mnsdlobserver.h"
#include "m2mconnectionobserver.h"

class TestObserver : public M2MNsdlObserver,
                     public M2MConnectionObserver
{

public:
    TestObserver(){}
    virtual ~TestObserver(){}
    void coap_message_ready(uint8_t *,
                            uint16_t,
                            sn_nsdl_addr_s *){
        message_ready = true;
    }

    void client_registered(M2MServer *){
        registered = true;
    }

    void registration_updated(const M2MServer &){

    }

    void registration_error(uint8_t, bool retry = false){
        register_error = true;
    }

    void client_unregistered(){
        unregistered = true;
    }

    void bootstrap_done(M2MSecurity *sec){
        if(sec) {
            boot_done = true;
        }
    }

    void bootstrap_error(){
        boot_error = true;
    }

    void coap_data_processed(){
        data_processed = true;
    }

    void value_updated(M2MBase *){
    }

    void data_available(uint8_t*,
                        uint16_t,
                        const M2MConnectionObserver::SocketAddress &) {

    }

    void socket_error(uint8_t, bool) {

    }

    void address_ready(const M2MConnectionObserver::SocketAddress &,
                               M2MConnectionObserver::ServerType,
                               const uint16_t) {

    }

    void data_sent() {

    }

    bool register_error;
    bool boot_error;
    bool boot_done;
    bool registered;
    bool data_processed;
    bool unregistered;
    bool message_ready;

};

Test_NsdlAccessHelper::Test_NsdlAccessHelper()
{
    observer = new TestObserver();
}

Test_NsdlAccessHelper::~Test_NsdlAccessHelper()
{
    delete observer;
    observer = NULL;

    clear_list();
}

void Test_NsdlAccessHelper::test_nsdl_c_callback()
{
    CHECK(__nsdl_c_callback(NULL,NULL,NULL,SN_NSDL_PROTOCOL_HTTP) == 0 );

    m2mnsdlinterface_stub::int_value = 1;
    m2mnsdlinterface_stub::void_value = malloc(1);
    __nsdl_interface_list.clear();
    __nsdl_interface_list.push_back(new M2MNsdlInterface(*observer));
    common_stub::coap_header = (sn_coap_hdr_s *) malloc(sizeof(sn_coap_hdr_s));
    memset(common_stub::coap_header, 0, sizeof(sn_coap_hdr_s));
    common_stub::coap_header->options_list_ptr = (sn_coap_options_list_s *) malloc(sizeof(sn_coap_options_list_s));
    common_stub::coap_header->options_list_ptr->block1_len = 2;

    CHECK(__nsdl_c_callback((nsdl_s*)m2mnsdlinterface_stub::void_value,
                            common_stub::coap_header,NULL,SN_NSDL_PROTOCOL_HTTP) == 1 );
    free(common_stub::coap_header->options_list_ptr);
    free(common_stub::coap_header);
    free(m2mnsdlinterface_stub::void_value);
    clear_list();
}

void Test_NsdlAccessHelper::test_nsdl_c_memory_alloc()
{
    void *ptr = __nsdl_c_memory_alloc(6);
    CHECK(ptr != NULL);
    free(ptr);
    ptr = NULL;
    ptr = __nsdl_c_memory_alloc(UINT16_MAX+1);
    CHECK(ptr == NULL);
    free(ptr);
}

void Test_NsdlAccessHelper::test_nsdl_c_memory_free()
{
    void* ptr = malloc(7);
    __nsdl_c_memory_free(ptr);

    CHECK(ptr != NULL);

    ptr = NULL;
    //No need to check anything, since memory leak is the test
}

void Test_NsdlAccessHelper::test_nsdl_c_send_to_server()
{
    CHECK(__nsdl_c_send_to_server(NULL, SN_NSDL_PROTOCOL_HTTP, NULL, 0, NULL) == 0);

    m2mnsdlinterface_stub::int_value = 1;
    m2mnsdlinterface_stub::void_value = malloc(1);
    __nsdl_interface_list.clear();
    __nsdl_interface_list.push_back(new M2MNsdlInterface(*observer));

    CHECK(__nsdl_c_send_to_server((nsdl_s*)m2mnsdlinterface_stub::void_value, SN_NSDL_PROTOCOL_HTTP, NULL, 0, NULL) == 1);
    free(m2mnsdlinterface_stub::void_value);
    clear_list();
}

void Test_NsdlAccessHelper::test_nsdl_c_received_from_server()
{
    CHECK( 0 == __nsdl_c_received_from_server(NULL, NULL, NULL));

    m2mnsdlinterface_stub::int_value = 1;
    m2mnsdlinterface_stub::void_value = malloc(1);
    __nsdl_interface_list.clear();
    __nsdl_interface_list.push_back(new M2MNsdlInterface(*observer));
    common_stub::coap_header = (sn_coap_hdr_s *) malloc(sizeof(sn_coap_hdr_s));
    memset(common_stub::coap_header, 0, sizeof(sn_coap_hdr_s));
    common_stub::coap_header->options_list_ptr = (sn_coap_options_list_s *) malloc(sizeof(sn_coap_options_list_s));
    common_stub::coap_header->options_list_ptr->block1_len = 2;
    CHECK( 1 == __nsdl_c_received_from_server((nsdl_s*)m2mnsdlinterface_stub::void_value, common_stub::coap_header, NULL));
    free(common_stub::coap_header->options_list_ptr);
    free(common_stub::coap_header);
    free(m2mnsdlinterface_stub::void_value);
    clear_list();
}

void Test_NsdlAccessHelper::test_socket_malloc()
{
    void *ptr = __socket_malloc(NULL, 6);
    CHECK(ptr != NULL);

    free(ptr);
}

void Test_NsdlAccessHelper::test_socket_free()
{
    void* ptr = malloc(7);
    __socket_free(NULL, ptr);

    ptr = NULL;
    //No need to check anything, since memory leak is the test
}

void Test_NsdlAccessHelper::test_mutex_claim()
{
    __connection_handler = new M2MConnectionHandler(*observer,
                                                    NULL,
                                                    M2MInterface::UDP,
                                                    M2MInterface::LwIP_IPv4);
    __mutex_claim();

    delete __connection_handler;
}

void Test_NsdlAccessHelper::test_mutex_release()
{
    __connection_handler = new M2MConnectionHandler(*observer,
                                                    NULL,
                                                    M2MInterface::UDP,
                                                    M2MInterface::LwIP_IPv4);
    __mutex_release();

    delete __connection_handler;
}

void Test_NsdlAccessHelper::clear_list()
{
    M2MNsdlInterfaceList::const_iterator it;
    it = __nsdl_interface_list.begin();
    int size = __nsdl_interface_list.size();
    if (!__nsdl_interface_list.empty()) {
        for (int i = 0; i < size; i++) {
                delete __nsdl_interface_list[i];
                __nsdl_interface_list.erase(i);
            }
        __nsdl_interface_list.clear();
    }
}
