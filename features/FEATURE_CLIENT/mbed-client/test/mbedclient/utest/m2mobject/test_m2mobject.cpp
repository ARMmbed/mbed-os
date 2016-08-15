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
#include "test_m2mobject.h"
#include "m2mobjectinstance_stub.h"
#include "m2mresource_stub.h"
#include "m2mbase_stub.h"
#include "common_stub.h"
#include "m2mtlvdeserializer_stub.h"
#include "m2mtlvserializer_stub.h"
#include "m2mreporthandler_stub.h"

class TestReportObserver :  public M2MReportObserver{
public :
    TestReportObserver() {}
    ~TestReportObserver() {}
    void observation_to_be_sent(m2m::Vector<uint16_t>,bool){ }
};

class Handler : public M2MObservationHandler {

public:

    Handler(){}
    ~Handler(){}
    void observation_to_be_sent(M2MBase *, uint16_t, m2m::Vector<uint16_t>,bool){
        visited = true;
    }
    void send_delayed_response(M2MBase *){}
    void resource_to_be_deleted(const String &){visited=true;}
    void remove_object(M2MBase *){visited = true;}
    void value_updated(M2MBase *,const String&){visited = true;}

    void clear() {visited = false;}
    bool visited;
};

Test_M2MObject::Test_M2MObject()
{
    handler = new Handler();
    object = new M2MObject("name");
}

Test_M2MObject::~Test_M2MObject()
{
    m2mobjectinstance_stub::clear();
    m2mresource_stub::clear();
    m2mbase_stub::clear();
    m2mbase_stub::string_value = new String("name");
    m2mtlvdeserializer_stub::clear();
    m2mtlvserializer_stub::clear();
    delete object;
    delete handler;
    delete m2mbase_stub::string_value;
    m2mbase_stub::string_value = NULL;
}

void Test_M2MObject::test_copy_constructor()
{
    String *name = new String("name");
    m2mbase_stub::string_value = name;

    M2MObject* copy = new M2MObject(*name);
    M2MObjectInstance *ins = new M2MObjectInstance("name",*object);
    copy->set_instance_id(0);
    copy->_instance_list.push_back(ins);

    M2MObject* copy1 = new M2MObject(*copy);

    CHECK(1 == copy1->_instance_list.size());

    delete copy;
    delete copy1;
    delete name;
    name = NULL;
}

void Test_M2MObject::test_create_object_instance()
{
    m2mbase_stub::name_id_value = 1;
    CHECK(object->create_object_instance() != NULL);
}

void Test_M2MObject::test_remove_object_instance()
{
    m2mbase_stub::string_value = new String("name");
    
    M2MObjectInstance *ins = new M2MObjectInstance("name",*object);
    object->set_instance_id(0);
    object->_instance_list.push_back(ins);

    CHECK(true == object->remove_object_instance(0));

    CHECK(false == object->remove_object_instance(0));

    delete m2mbase_stub::string_value;
}

void Test_M2MObject::test_object_instance()
{
    String *test = new String("name");
    M2MObjectInstance *ins = new M2MObjectInstance(*test,*object);
    object->set_instance_id(0);
    object->_instance_list.push_back(ins);

    m2mbase_stub::string_value = test;

    M2MObjectInstance *obj = object->object_instance(0);

    CHECK(obj != NULL);
    CHECK(0 == obj->name().compare(0,test->size(),*test));

    delete test;
    test = NULL;
}

void Test_M2MObject::test_instances()
{
    String *test = new String("name");
    M2MObjectInstance *ins = new M2MObjectInstance(*test,*object);
    ins->set_instance_id(0);
    object->_instance_list.push_back(ins);

    M2MObjectInstance *ins1 = new M2MObjectInstance(*test,*object);
    ins1->set_instance_id(1);
    object->_instance_list.push_back(ins1);

    m2mbase_stub::string_value = test;

    M2MObjectInstanceList list = object->instances();

    M2MObjectInstance *obj = list[0];
    CHECK(2 == list.size());
    CHECK(0 == obj->name().compare(0,test->size(),*test));

    delete test;
    test = NULL;
}

void Test_M2MObject::test_instance_count()
{
    String test = "name";
    M2MObjectInstance *ins = new M2MObjectInstance(test,*object);
    object->set_instance_id(0);
    object->_instance_list.push_back(ins);

    M2MObjectInstance *ins1 = new M2MObjectInstance(test,*object);
    object->set_instance_id(1);
    object->_instance_list.push_back(ins1);

    CHECK(2 == object->instance_count());

    object->_instance_list.clear();
    delete ins;
    delete ins1;
}

void Test_M2MObject::test_base_type()
{
    CHECK(M2MBase::Object == object->base_type());
}

void Test_M2MObject::test_handle_get_request()
{
    M2MObjectInstance *ins = new M2MObjectInstance("name",*object);
    object->set_instance_id(0);
    object->_instance_list.push_back(ins);

    uint8_t value[] = {"name"};
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_GET;

    String *name = new String("name");
    common_stub::int_value = 0;
    m2mbase_stub::string_value = name;

    m2mbase_stub::operation = M2MBase::GET_ALLOWED;
    m2mbase_stub::uint8_value = 200;

    common_stub::coap_header = (sn_coap_hdr_ *)malloc(sizeof(sn_coap_hdr_));
    memset(common_stub::coap_header,0,sizeof(sn_coap_hdr_));

    m2mtlvserializer_stub::uint8_value = (uint8_t*)malloc(1);

    coap_header->token_ptr = (uint8_t*)malloc(sizeof(value));
    memcpy(coap_header->token_ptr, value, sizeof(value));

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->observe = 0;


    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 110;

    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(coap_header->content_type_ptr) {
        free(coap_header->content_type_ptr);
        coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    // Not OMA TLV or JSON
    m2mbase_stub::uint8_value = 110;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    // OMA TLV
    m2mbase_stub::uint8_value = 99;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    // OMA JSON
    m2mbase_stub::uint8_value = 100;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    coap_header->options_list_ptr->observe = 1;

    uint8_t obs = 0;
    coap_header->options_list_ptr->observe_ptr = (uint8_t*)malloc(sizeof(obs));
    memcpy(coap_header->options_list_ptr->observe_ptr,&obs,sizeof(obs));
    coap_header->options_list_ptr->observe_len = 0;
    m2mbase_stub::uint16_value = 0x1c1c;
    m2mbase_stub::uint8_value = 99;
    m2mbase_stub::bool_value = true;

    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr->observe_ptr) {
        free(common_stub::coap_header->options_list_ptr->observe_ptr);
        common_stub::coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }


    m2mbase_stub::uint16_value = 10;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr->observe_ptr) {
        free(common_stub::coap_header->options_list_ptr->observe_ptr);
        common_stub::coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    // Not observable
    m2mbase_stub::bool_value = false;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    m2mbase_stub::bool_value = true;
    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr->observe_ptr) {
        free(common_stub::coap_header->options_list_ptr->observe_ptr);
        common_stub::coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    coap_header->options_list_ptr->observe_len = 1;

    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr->observe_ptr) {
        free(common_stub::coap_header->options_list_ptr->observe_ptr);
        common_stub::coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }


    obs = 1;
    memcpy(coap_header->options_list_ptr->observe_ptr,&obs,sizeof(obs));
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    if(common_stub::coap_header->content_type_ptr) {
        free(common_stub::coap_header->content_type_ptr);
        common_stub::coap_header->content_type_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr->observe_ptr) {
        free(common_stub::coap_header->options_list_ptr->observe_ptr);
        common_stub::coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(common_stub::coap_header->options_list_ptr->max_age_ptr) {
        free(common_stub::coap_header->options_list_ptr->max_age_ptr);
        common_stub::coap_header->options_list_ptr->max_age_ptr = NULL;
    }

    if(common_stub::coap_header->options_list_ptr) {
        free(common_stub::coap_header->options_list_ptr);
        common_stub::coap_header->options_list_ptr = NULL;
    }

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;
    CHECK(object->handle_get_request(NULL,coap_header,handler) != NULL);

    CHECK(object->handle_get_request(NULL,NULL,handler) != NULL);

    if(coap_header->token_ptr) {
        free(coap_header->token_ptr);
        coap_header->token_ptr = NULL;
    }
    if(coap_header->content_type_ptr) {
        free(coap_header->content_type_ptr);
        coap_header->content_type_ptr = NULL;
    }
    if(coap_header->options_list_ptr->observe_ptr) {
        free(coap_header->options_list_ptr->observe_ptr);
        coap_header->options_list_ptr->observe_ptr = NULL;
    }
    if(coap_header->options_list_ptr) {
        free(coap_header->options_list_ptr);
        coap_header->options_list_ptr = NULL;
    }

    if(common_stub::coap_header){
        if(common_stub::coap_header->content_type_ptr) {
            free(common_stub::coap_header->content_type_ptr);
            common_stub::coap_header->content_type_ptr = NULL;
        }
        if(common_stub::coap_header->options_list_ptr) {
            free(common_stub::coap_header->options_list_ptr);
            common_stub::coap_header->options_list_ptr = NULL;
        }
        free(common_stub::coap_header);
        common_stub::coap_header = NULL;
    }
    free(coap_header);
    coap_header = NULL;

    delete name;
    name = NULL;

    if(m2mtlvserializer_stub::uint8_value) {
        free(m2mtlvserializer_stub::uint8_value);
    }
    m2mtlvserializer_stub::clear();

    m2mbase_stub::clear();
    common_stub::clear();

    object->_instance_list.clear();
    delete ins;
}

void Test_M2MObject::test_handle_put_request()
{
    uint8_t value[] = {"name"};
    bool execute_value_updated = false;
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));
    sn_coap_hdr_s * coap_response = NULL;

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_PUT;

    String *name = new String("name");
    common_stub::int_value = 0;
    m2mbase_stub::string_value = name;

    m2mbase_stub::operation = M2MBase::PUT_ALLOWED;
    m2mbase_stub::uint8_value = 200;

    common_stub::coap_header = (sn_coap_hdr_ *)malloc(sizeof(sn_coap_hdr_));
    memset(common_stub::coap_header,0,sizeof(sn_coap_hdr_));

    coap_header->payload_ptr = (uint8_t*)malloc(1);

    CHECK(object->handle_put_request(NULL,coap_header,handler,execute_value_updated) != NULL);
    free(coap_header->payload_ptr);
    coap_header->payload_ptr = NULL;

    CHECK(object->handle_put_request(NULL,coap_header,handler,execute_value_updated) != NULL);

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->uri_query_ptr = value;
    coap_header->options_list_ptr->uri_query_len = sizeof(value);

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 99;
    m2mtlvdeserializer_stub::bool_value = true;

    m2mbase_stub::bool_value = false;

    CHECK(object->handle_put_request(NULL,coap_header,handler,execute_value_updated) != NULL);

    m2mtlvdeserializer_stub::bool_value = false;

    CHECK(object->handle_put_request(NULL,coap_header,handler, execute_value_updated) != NULL);

    *coap_header->content_type_ptr = 100;

    CHECK(object->handle_put_request(NULL,coap_header,handler, execute_value_updated) != NULL);

    m2mbase_stub::bool_value = true;

    CHECK(object->handle_put_request(NULL,coap_header,handler, execute_value_updated) != NULL);

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    CHECK(object->handle_put_request(NULL,coap_header,handler, execute_value_updated) != NULL);

    CHECK(object->handle_put_request(NULL,NULL,handler, execute_value_updated) != NULL);

    m2mbase_stub::operation = M2MBase::PUT_ALLOWED;




    free(coap_header->payload_ptr);
    coap_header->payload_ptr = NULL;
    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK(coap_response != NULL);
    CHECK(coap_response->msg_code == COAP_MSG_CODE_RESPONSE_CHANGED);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }
    m2mbase_stub::bool_value = false;
    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK(coap_response != NULL);
    CHECK(coap_response->msg_code == COAP_MSG_CODE_RESPONSE_BAD_REQUEST);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    free(coap_header->options_list_ptr);
    coap_header->options_list_ptr = NULL;
    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK(coap_response != NULL);
    CHECK(coap_response->msg_code == COAP_MSG_CODE_RESPONSE_BAD_REQUEST);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    free(coap_header->content_type_ptr);
    free(coap_header->options_list_ptr);    
    free(common_stub::coap_header);
    delete name;
    free(coap_header);

    m2mtlvdeserializer_stub::clear();
    common_stub::clear();
    m2mbase_stub::clear();
}

void Test_M2MObject::test_handle_post_request()
{
    uint8_t value[] = {"name"};
    bool execute_value_updated = false;
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_POST;

    String *name = new String("name");
    common_stub::int_value = 0;
    m2mbase_stub::string_value = name;

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mbase_stub::uint8_value = 200;

    common_stub::coap_header = (sn_coap_hdr_ *)malloc(sizeof(sn_coap_hdr_));
    memset(common_stub::coap_header,0,sizeof(sn_coap_hdr_));

    m2mbase_stub::bool_value = false;

    sn_coap_hdr_s * coap_response = NULL;
    m2mbase_stub::uint8_value = 99;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);


    m2mbase_stub::uint8_value = 100;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);

    coap_header->payload_ptr = (uint8_t*)malloc(1);

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);

    m2mbase_stub::uint8_value = 99;

    object->_max_instance_count = 0;

    String *test = new String("name");
    M2MObjectInstance *ins = new M2MObjectInstance(*test,*object);
    ins->set_instance_id(0);
    object->_instance_list.push_back(ins);

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);

    object->remove_object_instance(0);
    delete test;

    object->_max_instance_count = 65535;

    m2mbase_stub::uint8_value = 0;

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->uri_query_ptr = value;
    coap_header->options_list_ptr->uri_query_len = sizeof(value);

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 99;
    m2mtlvdeserializer_stub::is_object_bool_value = true;
    m2mtlvdeserializer_stub::bool_value = false;
    m2mbase_stub::bool_value = false;

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::None;
    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr->location_path_ptr) {
            free(coap_response->options_list_ptr->location_path_ptr);
            coap_response->options_list_ptr->location_path_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mtlvdeserializer_stub::is_object_bool_value = true;
    m2mtlvdeserializer_stub::bool_value = false;
    m2mtlvdeserializer_stub::int_value = 10;
    m2mbase_stub::bool_value = false;

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotAllowed;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mtlvdeserializer_stub::is_object_bool_value = false;
    m2mtlvdeserializer_stub::bool_value = true;
    m2mbase_stub::bool_value = false;
    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::None;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mtlvdeserializer_stub::is_object_bool_value = false;
    m2mtlvdeserializer_stub::bool_value = true;
    m2mbase_stub::bool_value = false;
    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotFound;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mtlvdeserializer_stub::bool_value = false;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }


    *coap_header->content_type_ptr = 100;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }


    m2mbase_stub::bool_value = true;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }


    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    coap_response = object->handle_post_request(NULL,NULL,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mtlvdeserializer_stub::int_value = 0;
    m2mtlvdeserializer_stub::is_object_bool_value = true;
    *coap_header->content_type_ptr = 99;
    coap_response = object->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
        }
    }

    free(coap_header->content_type_ptr);
    free(coap_header->options_list_ptr);
    free(coap_header->payload_ptr);
    free(common_stub::coap_header);
    delete name;
    free(coap_header);

    m2mtlvdeserializer_stub::clear();
    common_stub::clear();
    m2mbase_stub::clear();
}

void Test_M2MObject::test_notification_update()
{
    TestReportObserver obs;
    m2mbase_stub::report = new M2MReportHandler(obs);
    m2mbase_stub::bool_value = true;

    object->notification_update(0);

    delete m2mbase_stub::report;
    m2mbase_stub::report = NULL;
}

