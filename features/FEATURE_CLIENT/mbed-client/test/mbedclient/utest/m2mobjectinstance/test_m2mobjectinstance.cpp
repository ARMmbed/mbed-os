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
#include "test_m2mobjectinstance.h"
#include "m2mresource_stub.h"
#include "common_stub.h"
#include "m2mresourceinstance_stub.h"
#include "m2mbase_stub.h"
#include "m2mtlvdeserializer_stub.h"
#include "m2mtlvserializer_stub.h"
#include "m2mreporthandler_stub.h"

// Length is 65
String max_length = "65656565656565656565656565656565656565656565656565656565656565656";

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

class TestReportObserver :  public M2MReportObserver{
public :
    TestReportObserver() {}
    ~TestReportObserver() {}
    void observation_to_be_sent(m2m::Vector<uint16_t>,bool){ }
};

class Callback : public M2MObjectCallback {

public:

    Callback() : visited(false){}
    ~Callback(){}
    void notification_update(uint16_t obj_instance_id) {
        visited = true;
    }

    void clear() {visited = false;}
    bool visited;
};

Test_M2MObjectInstance::Test_M2MObjectInstance()
{
    handler = new Handler();
    callback = new Callback();
    object = new M2MObjectInstance("name",*callback);
}

void Test_M2MObjectInstance::test_copy_constructor()
{
    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static);
    m2mbase_stub::string_value = new String("name");
    object->_resource_list.push_back(res);

    M2MObjectInstance* copy = new M2MObjectInstance(*object);

    CHECK(1 == copy->_resource_list.size());
    delete copy;
}


Test_M2MObjectInstance::~Test_M2MObjectInstance()
{
    delete object;
    delete callback;
    delete handler;
    delete m2mbase_stub::string_value;
    m2mresource_stub::clear();
    m2mbase_stub::clear();
    m2mtlvdeserializer_stub::clear();
    m2mtlvserializer_stub::clear();

}

void Test_M2MObjectInstance::test_create_static_resource()
{
    m2mbase_stub::string_value = new String("name");
    u_int8_t value[] = {"value"};

    m2mbase_stub::bool_value = true;

    m2mbase_stub::uint8_value = 99;
    M2MResource * res = object->create_static_resource("name","type",M2MResourceInstance::STRING,value,(u_int32_t)sizeof(value),false);

    CHECK(res != NULL);
    res = object->create_static_resource("name2","type",M2MResourceInstance::STRING,value,(u_int32_t)sizeof(value),true);

    CHECK(res != NULL);
    CHECK(2 == object->_resource_list.size());

    res = object->create_static_resource("","type",M2MResourceInstance::STRING,value,(u_int32_t)sizeof(value));
    CHECK(res == NULL);

    res = object->create_static_resource(max_length,"type",M2MResourceInstance::STRING,value,(u_int32_t)sizeof(value));
    CHECK(res == NULL);
}

void Test_M2MObjectInstance::test_create_static_resource_instance()
{
    m2mbase_stub::string_value = new String("name");
    u_int8_t value[] = {"value"};

    m2mbase_stub::bool_value = true;
    m2mresource_stub::bool_value = true;
    m2mbase_stub::uint8_value = 99;
    M2MResourceInstance *ins = object->create_static_resource_instance("name","type",
                                                                M2MResourceInstance::STRING,
                                                                value,(u_int32_t)sizeof(value),
                                                                0);

    CHECK(ins != NULL);
    CHECK(1 == object->_resource_list.size());

    delete ins;

    ins = object->create_static_resource_instance("name","type",
                                                  M2MResourceInstance::STRING,
                                                  value,(u_int32_t)sizeof(value),
                                                  1);

    CHECK(ins != NULL);
    CHECK(1 == object->_resource_list.size());

    delete ins;

    ins = object->create_static_resource_instance("","type",
                                                  M2MResourceInstance::STRING,value,
                                                  (u_int32_t)sizeof(value),0);
    CHECK(ins == NULL);

    delete ins;

    ins = object->create_static_resource_instance(max_length,"type",
                                                  M2MResourceInstance::STRING,value,
                                                  (u_int32_t)sizeof(value),0);
    CHECK(ins == NULL);
}

void Test_M2MObjectInstance::test_create_dynamic_resource_instance()
{
    m2mbase_stub::string_value = new String("name");

    m2mresource_stub::bool_value = true;
    m2mbase_stub::uint8_value = 99;

    M2MResourceInstance * ins = object->create_dynamic_resource_instance("name","type",
                                                                         M2MResourceInstance::STRING,
                                                                         false,0);
    CHECK(ins != NULL);
    CHECK(1 == object->_resource_list.size());

    delete ins;

    ins = object->create_dynamic_resource_instance("name","type",
                                                   M2MResourceInstance::STRING,
                                                   false,1);
    CHECK(ins != NULL);
    CHECK(1 == object->_resource_list.size());

    delete ins;

    ins = object->create_dynamic_resource_instance("","type",
                                                   M2MResourceInstance::STRING,
                                                   false,1);

    CHECK(ins == NULL);

    ins = object->create_dynamic_resource_instance(max_length,"type",
                                                   M2MResourceInstance::STRING,
                                                   false,1);
    CHECK(ins == NULL);
}

void Test_M2MObjectInstance::test_create_dynamic_resource()
{
    m2mbase_stub::string_value = new String("name");
    m2mbase_stub::uint8_value = 99;

    M2MResource * res = object->create_dynamic_resource("name","type",M2MResourceInstance::STRING,false,false);
    CHECK(res != NULL);
    CHECK(1 == object->_resource_list.size());

    res = object->create_dynamic_resource("multiple","type",M2MResourceInstance::STRING,false,true);
    CHECK(res != NULL);
    CHECK(2 == object->_resource_list.size());

    M2MResource * res1 = object->create_dynamic_resource("name1","type",M2MResourceInstance::STRING,false,false);
    CHECK(res1 != NULL);
    CHECK(3 == object->_resource_list.size());

    M2MResource * res2 = object->create_dynamic_resource("","type",M2MResourceInstance::STRING,false,false);
    CHECK(res2 == NULL);

    M2MResource * res3 = object->create_dynamic_resource(max_length,"type",M2MResourceInstance::STRING,false,false);
    CHECK(res3 == NULL);
}

void Test_M2MObjectInstance::test_remove_resource()
{
    CHECK(false == object->remove_resource("name"));

    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    m2mbase_stub::string_value = new String("name");
    m2mbase_stub::int_value = 0;
    m2mbase_stub::void_value = malloc(20);

    m2mresource_stub::bool_value = true;
    CHECK(true == object->remove_resource("name"));
    CHECK(0 == object->_resource_list.size());

    free(m2mbase_stub::void_value);
}

void Test_M2MObjectInstance::test_remove_resource_instance()
{
    CHECK(false == object->remove_resource_instance("name",0));

    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    m2mbase_stub::string_value = new String("name");
    m2mbase_stub::int_value = 0;
    m2mbase_stub::void_value = malloc(20);

    m2mresource_stub::bool_value = true;

    M2MResourceInstance *ins = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*object);

    m2mresource_stub::list.push_back(ins);

    CHECK(true == object->remove_resource_instance("name",0));
    CHECK(0 == object->_resource_list.size());

    free(m2mbase_stub::void_value);

    delete ins;
    ins = NULL;
}


void Test_M2MObjectInstance::test_resource()
{
    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    m2mbase_stub::string_value = new String("name");
    m2mbase_stub::int_value = 0;

    M2MResource *result = object->resource("name");
    CHECK(result != NULL);

    res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    m2mbase_stub::int_value = 1;

    result = object->resource("name");
    CHECK(result != NULL);
}

void Test_M2MObjectInstance::test_resources()
{
    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    m2mbase_stub::string_value = new String("name");
    object->_resource_list.push_back(res);

    res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    M2MResourceList resources =object->resources();

    CHECK(2 == resources.size());
}

void Test_M2MObjectInstance::test_resource_count()
{
    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    m2mbase_stub::string_value = new String("name");
    object->_resource_list.push_back(res);

    res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);


    m2mbase_stub::int_value = 0;
    m2mresource_stub::bool_value = true;
    m2mresource_stub::int_value = 1;

    CHECK(2 == object->resource_count("name"));

    m2mresource_stub::bool_value = false;

    CHECK(2 == object->resource_count("name"));
}

void Test_M2MObjectInstance::test_total_resource_count()
{
    M2MResource *res = new M2MResource(*object,"name","type",M2MResourceInstance::STRING,M2MBase::Static,true);
    m2mbase_stub::string_value = new String("name");
    object->_resource_list.push_back(res);

    res = new M2MResource(*object,"name1","type1",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);

    m2mresource_stub::bool_value = true;
    m2mresource_stub::int_value = 1;

    CHECK(2 == object->resource_count());

    m2mresource_stub::bool_value = false;

    CHECK(2 == object->resource_count());
}

void Test_M2MObjectInstance::test_base_type()
{
    m2mbase_stub::base_type = M2MBase::ObjectInstance;
    CHECK(M2MBase::ObjectInstance == object->base_type());
}

void Test_M2MObjectInstance::test_handle_get_request()
{
    M2MResource *res = new M2MResource(*object,"name1","type1",M2MResourceInstance::STRING,M2MBase::Static,true);
    object->_resource_list.push_back(res);
    m2mbase_stub::string_value = new String("name1");
    uint8_t value[] = {"name"};
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_GET;

    common_stub::int_value = 0;

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
    m2mbase_stub::uint8_value = 99;
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

    if(m2mtlvserializer_stub::uint8_value) {
        free(m2mtlvserializer_stub::uint8_value);
    }
    m2mtlvserializer_stub::clear();
    common_stub::clear();
}

void Test_M2MObjectInstance::test_handle_put_request()
{
    uint8_t value[] = {"name"};
    bool execute_value_updated = false;
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_PUT;

    common_stub::int_value = 0;
    m2mbase_stub::string_value = new String("name");

    m2mbase_stub::operation = M2MBase::PUT_ALLOWED;
    m2mbase_stub::uint8_value = 200;

    common_stub::coap_header = (sn_coap_hdr_ *)malloc(sizeof(sn_coap_hdr_));
    memset(common_stub::coap_header,0,sizeof(sn_coap_hdr_));

    coap_header->payload_ptr = (uint8_t*)malloc(1);

    sn_coap_hdr_s *coap_response = NULL;
    m2mbase_stub::uint8_value = 99;
    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);

    m2mbase_stub::uint8_value = 0;

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->uri_query_ptr = value;
    coap_header->options_list_ptr->uri_query_len = sizeof(value);

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 99;
    m2mtlvdeserializer_stub::bool_value = true;
    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::None;

    m2mbase_stub::bool_value = false;


    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mbase_stub::bool_value = true;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    free(coap_header->options_list_ptr);
    coap_header->options_list_ptr = NULL;

    m2mbase_stub::bool_value = false;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotFound;

    m2mbase_stub::bool_value = false;


    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotValid;

    m2mbase_stub::bool_value = false;


    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotAllowed;

    m2mbase_stub::bool_value = false;


    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::bool_value = false;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    *coap_header->content_type_ptr = 100;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    free(coap_header->payload_ptr);
    coap_header->payload_ptr = NULL;

    m2mbase_stub::bool_value = true;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mbase_stub::bool_value = false;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    coap_response = object->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    coap_response = object->handle_put_request(NULL,NULL,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }


    free(coap_header->content_type_ptr);
    //free(coap_header->options_list_ptr);
    free(common_stub::coap_header);
    delete m2mbase_stub::string_value;
    free(coap_header);

    m2mtlvdeserializer_stub::clear();
    common_stub::clear();
    m2mbase_stub::clear();
}

void Test_M2MObjectInstance::test_handle_post_request()
{
    uint8_t value[] = {"name"};
    bool execute_value_updated = false;
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

    coap_header->uri_path_ptr = value;
    coap_header->uri_path_len = sizeof(value);

    coap_header->msg_code = COAP_MSG_CODE_REQUEST_POST;

    common_stub::int_value = 0;
    m2mbase_stub::string_value = new String("name");

    m2mbase_stub::operation = M2MBase::POST_ALLOWED;
    m2mbase_stub::uint8_value = 200;

    common_stub::coap_header = (sn_coap_hdr_ *)malloc(sizeof(sn_coap_hdr_));
    memset(common_stub::coap_header,0,sizeof(sn_coap_hdr_));

    m2mbase_stub::bool_value = false;

    sn_coap_hdr_s * coap_response = NULL;
    m2mbase_stub::uint8_value = 99;

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

    m2mbase_stub::uint8_value = 100;

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
    coap_header->payload_ptr = (uint8_t*)malloc(1);

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
    m2mbase_stub::uint8_value = 99;

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
    m2mtlvdeserializer_stub::is_object_bool_value = true;
    m2mtlvdeserializer_stub::bool_value = false;
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

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotValid;

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
        if (coap_response->options_list_ptr) {
            if (coap_response->options_list_ptr->location_path_ptr) {
                free(coap_response->options_list_ptr->location_path_ptr);
                coap_response->options_list_ptr->location_path_ptr = NULL;
            }
            free(coap_response->options_list_ptr);
            coap_response->options_list_ptr = NULL;
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

    free(coap_header->content_type_ptr);
    free(coap_header->options_list_ptr);
    free(coap_header->payload_ptr);
    free(common_stub::coap_header);
    free(coap_header);
    delete m2mbase_stub::string_value;
    m2mtlvdeserializer_stub::clear();
    common_stub::clear();
    m2mbase_stub::clear();

}

void Test_M2MObjectInstance::test_notification_update()
{
    M2MBase::Observation obs_level = M2MBase::O_Attribute;

    object->notification_update(obs_level);
    CHECK(callback->visited == true);

    obs_level = M2MBase::OI_Attribute;

    TestReportObserver obs;
    m2mbase_stub::report = new M2MReportHandler(obs);
    m2mbase_stub::bool_value = true;

    object->notification_update(obs_level);

    delete m2mbase_stub::report;
    m2mbase_stub::report = NULL;
}
