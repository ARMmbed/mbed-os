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
#include "test_m2msecurity.h"
#include "m2mobject_stub.h"
#include "m2mobjectinstance_stub.h"
#include "m2mresource_stub.h"
#include "m2mresourceinstance_stub.h"
#include "m2mbase_stub.h"

class Callback : public M2MObjectCallback {

public:

    Callback(){}
    ~Callback(){}
    void notification_update(uint16_t obj_instance_id) {
        visited = true;
    }

    void clear() {visited = false;}
    bool visited;
};

Test_M2MSecurity::Test_M2MSecurity()
{
    callback = new Callback();
    m2mobject_stub::inst = new M2MObjectInstance("name",*callback);
    m2mresource_stub::bool_value = true;
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    security = new M2MSecurity(M2MSecurity::Bootstrap);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

Test_M2MSecurity::~Test_M2MSecurity()
{
    delete m2mobject_stub::inst;
    m2mobject_stub::inst = NULL;
    m2mobject_stub::clear();
    m2mobjectinstance_stub::clear();
    m2mresource_stub::clear();
    m2mbase_stub::clear();
    delete security;
    delete callback;
}

void Test_M2MSecurity::test_create_resource_int()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->create_resource(M2MSecurity::M2MServerUri,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::BootstrapServer,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::SecurityMode,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::PublicKey,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::ServerPublicKey,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::Secretkey,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::SMSBindingKey,10) == NULL);
    CHECK(security->create_resource(M2MSecurity::SMSBindingSecretKey,10) == NULL);

    CHECK(security->create_resource(M2MSecurity::M2MServerSMSNumber,10) != NULL);
    CHECK(security->create_resource(M2MSecurity::ShortServerID,10) != NULL);
    CHECK(security->create_resource(M2MSecurity::ClientHoldOffTime,10) != NULL);
    CHECK(security->create_resource(M2MSecurity::SMSSecurityMode,10) != NULL);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MSecurity::test_delete_resource()
{
    m2mresourceinstance_stub::bool_value = true;
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->delete_resource(M2MSecurity::SMSBindingSecretKey) == false);
    CHECK(security->delete_resource(M2MSecurity::SMSBindingKey) == false);
    CHECK(security->delete_resource(M2MSecurity::M2MServerUri) == false);
    CHECK(security->delete_resource(M2MSecurity::Secretkey) == false);
    CHECK(security->delete_resource(M2MSecurity::ServerPublicKey) == false);
    CHECK(security->delete_resource(M2MSecurity::BootstrapServer) == false);
    CHECK(security->delete_resource(M2MSecurity::PublicKey) == false);
    CHECK(security->delete_resource(M2MSecurity::SecurityMode) == false);

    m2mobjectinstance_stub::bool_value = true;

    CHECK(security->delete_resource(M2MSecurity::M2MServerSMSNumber) == true);
    CHECK(security->delete_resource(M2MSecurity::ShortServerID) == true);
    CHECK(security->delete_resource(M2MSecurity::ClientHoldOffTime) == true);
    CHECK(security->delete_resource(M2MSecurity::SMSSecurityMode) == true);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MSecurity::test_set_resource_value_string()
{
    m2mresourceinstance_stub::bool_value = true;
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->set_resource_value(M2MSecurity::M2MServerUri,"test") == true);

    CHECK(security->set_resource_value(M2MSecurity::M2MServerSMSNumber,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::ShortServerID,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingKey,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingSecretKey,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::ClientHoldOffTime,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSSecurityMode,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::Secretkey,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::ServerPublicKey,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::PublicKey,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::BootstrapServer,"test") == false);
    CHECK(security->set_resource_value(M2MSecurity::SecurityMode,"test") == false);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MSecurity::test_set_resource_value_int()
{
    m2mresourceinstance_stub::bool_value = true;
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name", "name", M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->set_resource_value(M2MSecurity::M2MServerUri,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingKey,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingSecretKey,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::Secretkey,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::ServerPublicKey,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::PublicKey,10) == false);
    CHECK(security->set_resource_value(M2MSecurity::BootstrapServer,10) == false);

    CHECK(security->set_resource_value(M2MSecurity::SecurityMode,10) == true);
    CHECK(security->set_resource_value(M2MSecurity::SMSSecurityMode,10) == true);
    CHECK(security->set_resource_value(M2MSecurity::M2MServerSMSNumber,10) == true);
    CHECK(security->set_resource_value(M2MSecurity::ShortServerID,10) == true);
    CHECK(security->set_resource_value(M2MSecurity::ClientHoldOffTime,10) == true);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MSecurity::test_set_resource_value_buffer()
{
    m2mresourceinstance_stub::bool_value = true;

    uint8_t value[] = {"value"};
    uint16_t length = (uint16_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->set_resource_value(M2MSecurity::Secretkey,value,length) == true);
    CHECK(security->set_resource_value(M2MSecurity::ServerPublicKey,value,length) == true);
    CHECK(security->set_resource_value(M2MSecurity::PublicKey,value,length) == true);

    CHECK(security->set_resource_value(M2MSecurity::M2MServerUri,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingKey,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSBindingSecretKey,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::BootstrapServer,value,length) == false);

    CHECK(security->set_resource_value(M2MSecurity::SecurityMode,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::SMSSecurityMode,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::M2MServerSMSNumber,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::ShortServerID,value,length) == false);
    CHECK(security->set_resource_value(M2MSecurity::ClientHoldOffTime,value,length) == false);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MSecurity::test_resource_value_int()
{
    m2mresourceinstance_stub::bool_value = true;

    uint8_t value[] = {"10"};
    //m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    uint8_t size = 0;
    m2mresourceinstance_stub::value = String::convert_integer_to_array(10,size);
    /*memset(m2mresourceinstance_stub::value,0,(uint32_t)size);
    memcpy(m2mresourceinstance_stub::value, 10, size);*/
    m2mresourceinstance_stub::int_value = (uint16_t)size;

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,
                                                       "name",
                                                       "type",
                                                       M2MResourceInstance::INTEGER,
                                                       M2MBase::Dynamic);

    CHECK(security->resource_value_int(M2MSecurity::SecurityMode) == 10);
    CHECK(security->resource_value_int(M2MSecurity::SMSSecurityMode) == 10);
    CHECK(security->resource_value_int(M2MSecurity::M2MServerSMSNumber) == 10);
    CHECK(security->resource_value_int(M2MSecurity::ShortServerID) == 10);
    CHECK(security->resource_value_int(M2MSecurity::ClientHoldOffTime) == 10);
    CHECK(security->resource_value_int(M2MSecurity::BootstrapServer) == 0);
    CHECK(security->resource_value_int(M2MSecurity::PublicKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::ServerPublicKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::Secretkey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::SMSBindingSecretKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::SMSBindingKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::M2MServerUri) == 0);

    security->clear_resources();
    CHECK(security->resource_value_int(M2MSecurity::SecurityMode) == 0);
    CHECK(security->resource_value_int(M2MSecurity::SMSSecurityMode) == 0);
    CHECK(security->resource_value_int(M2MSecurity::M2MServerSMSNumber) == 0);
    CHECK(security->resource_value_int(M2MSecurity::ShortServerID) == 0);
    CHECK(security->resource_value_int(M2MSecurity::ClientHoldOffTime) == 0);
    CHECK(security->resource_value_int(M2MSecurity::BootstrapServer) == 0);
    CHECK(security->resource_value_int(M2MSecurity::PublicKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::ServerPublicKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::Secretkey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::SMSBindingSecretKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::SMSBindingKey) == 0);
    CHECK(security->resource_value_int(M2MSecurity::M2MServerUri) == 0);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;
}

void Test_M2MSecurity::test_resource_value_string()
{
    m2mresourceinstance_stub::bool_value = true;
    String test = "string";
    uint8_t value[] = {"string"};
    m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint16_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->resource_value_string(M2MSecurity::M2MServerUri) == test);

    CHECK(security->resource_value_string(M2MSecurity::SMSBindingKey) == "");
    CHECK(security->resource_value_string(M2MSecurity::SMSBindingSecretKey) == "");
    CHECK(security->resource_value_string(M2MSecurity::Secretkey) == "");
    CHECK(security->resource_value_string(M2MSecurity::ServerPublicKey) == "");
    CHECK(security->resource_value_string(M2MSecurity::PublicKey) == "");
    CHECK(security->resource_value_string(M2MSecurity::BootstrapServer) == "");
    CHECK(security->resource_value_string(M2MSecurity::ClientHoldOffTime) == "");
    CHECK(security->resource_value_string(M2MSecurity::ShortServerID) == "");
    CHECK(security->resource_value_string(M2MSecurity::M2MServerSMSNumber) == "");
    CHECK(security->resource_value_string(M2MSecurity::SMSSecurityMode) == "");
    CHECK(security->resource_value_string(M2MSecurity::SecurityMode) == "");

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;
}

void Test_M2MSecurity::test_resource_value_buffer()
{
    m2mresourceinstance_stub::bool_value = true;

    uint8_t value[] = {"buffer"};
    uint8_t *out_value = NULL;

    m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint16_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->resource_value_buffer(M2MSecurity::Secretkey,out_value) != 0);
    CHECK(security->resource_value_buffer(M2MSecurity::ServerPublicKey,out_value) != 0);
    CHECK(security->resource_value_buffer(M2MSecurity::PublicKey,out_value) != 0);

    CHECK(security->resource_value_buffer(M2MSecurity::M2MServerUri,out_value) == 0);

    CHECK(security->resource_value_buffer(M2MSecurity::SMSBindingKey,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::SMSBindingSecretKey,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::BootstrapServer,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::ClientHoldOffTime,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::ShortServerID,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::M2MServerSMSNumber,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::SMSSecurityMode,out_value) == 0);
    CHECK(security->resource_value_buffer(M2MSecurity::SecurityMode,out_value) == 0);

    const uint8_t *outvalue = NULL;
    CHECK(security->resource_value_buffer(M2MSecurity::Secretkey,outvalue) != 0);
    CHECK(security->resource_value_buffer(M2MSecurity::ServerPublicKey,outvalue) != 0);
    CHECK(security->resource_value_buffer(M2MSecurity::PublicKey,outvalue) != 0);

    free(out_value);
    out_value = NULL;

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;
}

void Test_M2MSecurity::test_is_resource_present()
{
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(security->is_resource_present(M2MSecurity::M2MServerUri) == true);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    CHECK(security->is_resource_present(M2MSecurity::ClientHoldOffTime) == false);
}

void Test_M2MSecurity::test_total_resource_count()
{
    M2MResource *res = new M2MResource(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(res);
    M2MResource *res2 = new M2MResource(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(res2);

    CHECK(security->total_resource_count() == 2);

    m2mobjectinstance_stub::resource_list.clear();

    delete res2;
    delete res;
}

void Test_M2MSecurity::test_m2m_server_constructor()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    M2MSecurity* sec = new M2MSecurity(M2MSecurity::M2MServer);
    CHECK(M2MSecurity::M2MServer == sec->server_type());

    delete sec;
    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MSecurity::test_server_type()
{
    CHECK(M2MSecurity::Bootstrap ==security->server_type());
}
