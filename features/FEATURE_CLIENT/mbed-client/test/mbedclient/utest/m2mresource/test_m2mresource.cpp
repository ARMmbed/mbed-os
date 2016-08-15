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
#include "test_m2mresource.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "m2mbase_stub.h"
#include "m2mresourceinstance_stub.h"
#include "m2mobjectinstance_stub.h"
#include "m2mtlvdeserializer_stub.h"
#include "m2mreporthandler_stub.h"
#include "common_stub.h"
#include "m2mreporthandler.h"

class TestReportObserver :  public M2MReportObserver{
public :
    TestReportObserver() {}
    ~TestReportObserver() {}
    void observation_to_be_sent(m2m::Vector<uint16_t>,bool){ }
};




class Callback : public M2MObjectInstanceCallback {

public:

    Callback(){}
    ~Callback(){}
    void notification_update(M2MBase::Observation) {
        visited = true;
    }

    void clear() {visited = false;}
    bool visited;
};

class Handler : public M2MObservationHandler {

public:

    Handler(){}
    ~Handler(){}
    void observation_to_be_sent(M2MBase *, uint16_t,m2m::Vector<uint16_t>,bool){
        visited = true;
    }
    void send_delayed_response(M2MBase *){}
    void resource_to_be_deleted(const String &){visited=true;}
    void remove_object(M2MBase *){visited = true;}
    void value_updated(M2MBase *,const String&){visited = true;}

    void clear() {visited = false;}
    bool visited;
};

Test_M2MResource::Test_M2MResource()
{
    callback = new Callback();
    handler = new Handler();
    resource = new M2MResource(*callback,
                               "name",
                               "resource_type",
                               M2MResourceInstance::INTEGER,
                               false,false);
}

void Test_M2MResource::test_copy_constructor()
{
    u_int8_t value[] = {"value"};
    resource->set_value(value,(u_int32_t)sizeof(value));
    resource->_delayed_token = (u_int8_t*)malloc(sizeof(value));
    resource->_delayed_token_len = sizeof(value);

    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->add_resource_instance(res);

    M2MResource* copy = new M2MResource(*resource);
    u_int8_t* out_value = (u_int8_t*)malloc(sizeof(u_int8_t));
    u_int32_t out_size;

    uint8_t* ptr = (uint8_t*)malloc((uint32_t)sizeof(value));
    m2mresourceinstance_stub::value = ptr;
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint32_t)sizeof(value);

    copy->get_value(out_value,out_size);

    CHECK(out_size == sizeof(value));

    free(out_value);
    free(ptr);

    delete copy;
}

Test_M2MResource::~Test_M2MResource()
{
    delete resource;
    delete handler;
    delete callback;
    m2mtlvdeserializer_stub::clear();
}

void Test_M2MResource::test_static_resource()
{
    u_int8_t value[] = {"value"};
    M2MResource *res = new M2MResource(*callback,
                                       "name",
                                      "resource_type",
                                      M2MResourceInstance::INTEGER,
                                      value,(uint32_t)sizeof(value),
                                      true);

    CHECK(res != NULL);
    delete res;

}

void Test_M2MResource::test_base_type()
{
    m2mresourceinstance_stub::base_type = M2MBase::Resource;
    CHECK(M2MBase::Resource == resource->base_type());
}

void Test_M2MResource::test_muliptle_instances()
{
    CHECK(false == resource->supports_multiple_instances());
}

void Test_M2MResource::test_handle_observation_attribute()
{
    char *d = "s";

    m2mresourceinstance_stub::resource_type = M2MResourceInstance::INTEGER;
    CHECK(false == resource->handle_observation_attribute(d));

    m2mresourceinstance_stub::resource_type = M2MResourceInstance::FLOAT;
    CHECK(false == resource->handle_observation_attribute(d));

    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::INTEGER,*callback);
    resource->add_resource_instance(res);

    TestReportObserver obs;
    m2mbase_stub::report = new M2MReportHandler(obs);

    m2mbase_stub::bool_value = true;    
    CHECK(false == resource->handle_observation_attribute(d));

    resource->_resource_type = M2MResourceInstance::INTEGER;
    m2mreporthandler_stub::bool_return = true;
    CHECK(true == resource->handle_observation_attribute(d));

    m2mbase_stub::bool_value = false;
    CHECK(true == resource->handle_observation_attribute(d));

    delete m2mbase_stub::report;
    m2mbase_stub::report = NULL;
}

void Test_M2MResource::test_add_resource_instance()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->add_resource_instance(res);
    CHECK(resource->_resource_instance_list.size() == 1);
}

void Test_M2MResource::test_remove_resource_instance()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);
    CHECK(resource->remove_resource_instance(0) == true);
}

void Test_M2MResource::test_resource_instance()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);
    CHECK(resource->resource_instance(0) != NULL);
}

void Test_M2MResource::test_resource_instances()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);
    M2MResourceInstanceList list = resource->resource_instances();
    CHECK(list.size() ==1);
}

void Test_M2MResource::test_resource_instance_count()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);
    CHECK(resource->resource_instance_count() ==1);
}

void Test_M2MResource::test_add_observation_level()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);

    M2MBase::Observation obs_level = M2MBase::R_Attribute;

    resource->add_observation_level(obs_level);
}

void Test_M2MResource::test_remove_observation_level()
{
    M2MResourceInstance *res = new M2MResourceInstance("name","type",M2MResourceInstance::STRING,*callback);
    resource->_resource_instance_list.push_back(res);

    M2MBase::Observation obs_level = M2MBase::R_Attribute;

    resource->remove_observation_level(obs_level);
}

void Test_M2MResource::test_handle_get_request()
{
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

    coap_header->token_ptr = (uint8_t*)malloc(sizeof(value));
    memcpy(coap_header->token_ptr, value, sizeof(value));

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->observe = 0;

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 110;

    m2mresourceinstance_stub::header = NULL;

    M2MResourceInstance* res_instance = new M2MResourceInstance("name","res2",
                                                                M2MResourceInstance::INTEGER,
                                                                *callback);
    M2MResourceInstance* res_instance_1 = new M2MResourceInstance("name","res2",
                                                                M2MResourceInstance::INTEGER,
                                                                *callback);

    CHECK(resource->handle_get_request(NULL,coap_header,handler) == NULL);

    resource->_has_multiple_instances = true;
    resource->add_resource_instance(res_instance);
    resource->add_resource_instance(res_instance_1);

    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);
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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    m2mbase_stub::bool_value = true;

    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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

    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

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
    CHECK(resource->handle_get_request(NULL,coap_header,handler) != NULL);

    CHECK(resource->handle_get_request(NULL,NULL,handler) != NULL);

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

    m2mbase_stub::clear();
    common_stub::clear();
}

void Test_M2MResource::test_handle_put_request()
{
    uint8_t value[] = {"name"};
    bool execute_value_updated = false;
    sn_coap_hdr_s *coap_header = (sn_coap_hdr_s *)malloc(sizeof(sn_coap_hdr_s));
    TestReportObserver obs;
    m2mbase_stub::report = new M2MReportHandler(obs);

    memset(coap_header, 0, sizeof(sn_coap_hdr_s));

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

    sn_coap_hdr_s *coap_response = NULL;

    m2mresourceinstance_stub::header = NULL;

    CHECK(resource->handle_put_request(NULL,coap_header,handler,execute_value_updated) == NULL);

    resource->_has_multiple_instances = true;
    m2mbase_stub::uint8_value = 99;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);

    m2mbase_stub::uint8_value = 0;

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->uri_query_ptr = value;
    coap_header->options_list_ptr->uri_query_len = sizeof(value);

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 99;
    m2mtlvdeserializer_stub::bool_value = true;

    m2mbase_stub::bool_value = false;


    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }
    free(coap_header->options_list_ptr);
    coap_header->options_list_ptr = NULL;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);
    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }


    m2mtlvdeserializer_stub::bool_value = false;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotFound;
    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotValid;
    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mtlvdeserializer_stub::error = M2MTLVDeserializer::NotAllowed;
    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    *coap_header->content_type_ptr = 100;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mbase_stub::bool_value = true;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    coap_response = resource->handle_put_request(NULL,coap_header,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    coap_response = resource->handle_put_request(NULL,NULL,handler,execute_value_updated);

    CHECK( coap_response != NULL);
    if(coap_response) {
        if(coap_response->content_type_ptr) {
            free(coap_response->content_type_ptr);
            coap_response->content_type_ptr = NULL;
        }
    }

    free(coap_header->content_type_ptr);
    //free(coap_header->options_list_ptr);
    free(coap_header->payload_ptr);
    free(common_stub::coap_header);
    delete name;
    free(coap_header);
    delete m2mbase_stub::report;
    m2mbase_stub::report = NULL;
    m2mtlvdeserializer_stub::clear();
    common_stub::clear();
    m2mbase_stub::clear();
}

void Test_M2MResource::test_handle_post_request()
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

    coap_header->payload_ptr = (uint8_t*)malloc(1);

    coap_header->options_list_ptr = (sn_coap_options_list_s*)malloc(sizeof(sn_coap_options_list_s));
    coap_header->options_list_ptr->uri_query_ptr = value;
    coap_header->options_list_ptr->uri_query_len = sizeof(value);

    coap_header->content_type_ptr = (uint8_t*)malloc(1);
    coap_header->content_type_len = 1;
    *coap_header->content_type_ptr = 99;


    resource->_delayed_response = true;
    resource->_delayed_token = (uint8_t*)malloc(1);
    *resource->_delayed_token  = 2;
    coap_header->token_ptr = (uint8_t*)malloc(1);
    *coap_header->token_ptr = 1;
    coap_header->token_len = 1;

    m2mbase_stub::bool_value = false;    
    m2mresourceinstance_stub::string_value = name;

    CHECK(resource->handle_post_request(NULL,coap_header,handler,execute_value_updated) != NULL);

    *coap_header->content_type_ptr = 0;

    CHECK(resource->handle_post_request(NULL,coap_header,handler,execute_value_updated) != NULL);

    m2mresourceinstance_stub::int_value = sizeof(value);
    m2mresourceinstance_stub::value = value;

    resource->_delayed_response = false;
    sn_coap_hdr_s *coap_response = resource->handle_post_request(NULL,coap_header,handler,execute_value_updated);

    CHECK(coap_response != NULL);
    free(coap_response->payload_ptr);

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;
    CHECK(resource->handle_post_request(NULL,coap_header,handler,execute_value_updated) != NULL);

    CHECK(resource->handle_post_request(NULL,NULL,handler,execute_value_updated) != NULL);

    free(coap_header->token_ptr);
    free(coap_header->content_type_ptr);
    free(coap_header->options_list_ptr);
    free(coap_header->payload_ptr);

    delete name;
    free(coap_header);
    free(common_stub::coap_header);

    m2mbase_stub::clear();
    common_stub::clear();
}

void Test_M2MResource::test_notification_update()
{
    TestReportObserver obs;
    m2mbase_stub::report = new M2MReportHandler(obs);
    m2mbase_stub::bool_value = true;

    resource->notification_update();

    delete m2mbase_stub::report;
    m2mbase_stub::report = NULL;
}

void Test_M2MResource::test_set_delayed_response()
{
    resource->set_delayed_response(true);
    CHECK(resource->_delayed_response == true);
}

void Test_M2MResource::test_send_delayed_post_response()
{
    resource->_delayed_response = true;
    m2mbase_stub::observe = handler;
    CHECK(resource->send_delayed_post_response() == true);
}

void Test_M2MResource::test_get_delayed_token()
{
    uint8_t value[] = {"1"};
    uint8_t *token = NULL;
    uint8_t token_len = 0;
    resource->_delayed_token_len = 1;
    resource->_delayed_token = (uint8_t*)malloc(sizeof(1));
    memcpy(resource->_delayed_token,value,1);
    resource->get_delayed_token(token,token_len);
    CHECK(token != NULL);
}

void Test_M2MResource::test_delayed_response()
{
    resource->_delayed_response = false;
    CHECK(resource->delayed_response() == false);
}

void Test_M2MResource::test_execute_params()
{
    M2MResource::M2MExecuteParameter *params = new M2MResource::M2MExecuteParameter();
    CHECK(params->get_argument_value() == NULL);
    CHECK(params->get_argument_value_length() == 0);
    CHECK(params->get_argument_object_name() == "");
    CHECK(params->get_argument_resource_name() == "");
    CHECK(params->get_argument_object_instance_id() == 0);

    uint8_t value[] = {"test"};
    int length = sizeof(value);
    params->_value = (uint8_t*)malloc(length);
    memcpy(params->_value,value,length);
    params->_value_length = length;
    params->_object_name = "object";
    params->_resource_name = "resource";
    params->_object_instance_id = 0;
    CHECK(params->_value == params->get_argument_value());
    CHECK(params->_value_length == params->get_argument_value_length());
    CHECK(params->_resource_name == params->get_argument_resource_name());
    CHECK(params->_object_name == params->get_argument_object_name());
    CHECK(params->_object_instance_id == params->get_argument_object_instance_id());

    delete params;
}
