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
#include "test_m2mserver.h"
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

Test_M2MServer::Test_M2MServer()
{
    callback = new Callback();
    m2mobject_stub::inst = new M2MObjectInstance("name",*callback);
    m2mresource_stub::bool_value = true;
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);
    server = new M2MServer();

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

Test_M2MServer::~Test_M2MServer()
{
    delete m2mobject_stub::inst;
    m2mobject_stub::inst = NULL;

    m2mobject_stub::clear();
    m2mobjectinstance_stub::clear();
    m2mresource_stub::clear();
    m2mbase_stub::clear();
    delete server;
    delete callback;
}

void Test_M2MServer::test_create_resource_int()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(server->create_resource(M2MServer::DefaultMinPeriod,10) != NULL);
    CHECK(server->create_resource(M2MServer::DefaultMaxPeriod,10) != NULL);
    CHECK(server->create_resource(M2MServer::DisableTimeout,10) != NULL);

    CHECK(server->create_resource(M2MServer::ShortServerID,10) == NULL);
    CHECK(server->create_resource(M2MServer::Lifetime,10) == NULL);
    CHECK(server->create_resource(M2MServer::Disable,10) == NULL);
    CHECK(server->create_resource(M2MServer::Binding,10) == NULL);
    CHECK(server->create_resource(M2MServer::NotificationStorage,10) == NULL);
    CHECK(server->create_resource(M2MServer::Binding,10) == NULL);
    CHECK(server->create_resource(M2MServer::RegistrationUpdate,10) == NULL);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MServer::test_create_resource()
{
    m2mobjectinstance_stub::create_resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(server->create_resource(M2MServer::Disable) != NULL);

    CHECK(server->create_resource(M2MServer::DefaultMinPeriod) == NULL);
    CHECK(server->create_resource(M2MServer::DefaultMaxPeriod) == NULL);
    CHECK(server->create_resource(M2MServer::DefaultMaxPeriod) == NULL);
    CHECK(server->create_resource(M2MServer::ShortServerID) == NULL);
    CHECK(server->create_resource(M2MServer::Lifetime) == NULL);
    CHECK(server->create_resource(M2MServer::DisableTimeout) == NULL);
    CHECK(server->create_resource(M2MServer::Binding) == NULL);
    CHECK(server->create_resource(M2MServer::NotificationStorage) == NULL);
    CHECK(server->create_resource(M2MServer::RegistrationUpdate) == NULL);

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;
}

void Test_M2MServer::test_delete_resource()
{
    m2mobjectinstance_stub::bool_value = true;

    CHECK(server->delete_resource(M2MServer::ShortServerID) == false);
    CHECK(server->delete_resource(M2MServer::Binding) == false);
    CHECK(server->delete_resource(M2MServer::NotificationStorage) == false);
    CHECK(server->delete_resource(M2MServer::Lifetime) == false);
    CHECK(server->delete_resource(M2MServer::RegistrationUpdate) == false);

    CHECK(server->delete_resource(M2MServer::DefaultMinPeriod) == true);
    CHECK(server->delete_resource(M2MServer::DefaultMaxPeriod) == true);
    CHECK(server->delete_resource(M2MServer::DisableTimeout) == true);
    CHECK(server->delete_resource(M2MServer::Disable) == true);
}

void Test_M2MServer::test_set_resource_value_int()
{
    m2mbase_stub::bool_value = true;

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name", "name",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(server->set_resource_value(M2MServer::DefaultMinPeriod,10) == true);
    CHECK(server->set_resource_value(M2MServer::DefaultMaxPeriod,10) == true);
    CHECK(server->set_resource_value(M2MServer::DisableTimeout,10) == true);
    CHECK(server->set_resource_value(M2MServer::ShortServerID,10) == true);
    CHECK(server->set_resource_value(M2MServer::NotificationStorage,10) == true);
    CHECK(server->set_resource_value(M2MServer::Lifetime,10) == true);

    CHECK(server->set_resource_value(M2MServer::Binding,10) == false);
    CHECK(server->set_resource_value(M2MServer::RegistrationUpdate,10) == false);
    CHECK(server->set_resource_value(M2MServer::Disable,10) == false);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MServer::test_set_resource_value_string()
{
    m2mresourceinstance_stub::bool_value = true;

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name", "name", M2MResourceInstance::STRING, M2MBase::Dynamic);

    CHECK(server->set_resource_value(M2MServer::Binding,"test") == true);

    CHECK(server->set_resource_value(M2MServer::DefaultMinPeriod,"test") == false);
    CHECK(server->set_resource_value(M2MServer::DefaultMaxPeriod,"test") == false);
    CHECK(server->set_resource_value(M2MServer::DisableTimeout,"test") == false);
    CHECK(server->set_resource_value(M2MServer::ShortServerID,"test") == false);
    CHECK(server->set_resource_value(M2MServer::NotificationStorage,"test") == false);
    CHECK(server->set_resource_value(M2MServer::Lifetime,"test") == false);
    CHECK(server->set_resource_value(M2MServer::RegistrationUpdate,"test") == false);
    CHECK(server->set_resource_value(M2MServer::Disable,"test") == false);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;
}

void Test_M2MServer::test_resource_value_int()
{
    uint8_t value[] = {"10"};
    m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint32_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name", "name", M2MResourceInstance::STRING, M2MBase::Dynamic);

   CHECK(server->resource_value_int(M2MServer::ShortServerID) == 10);
   CHECK(server->resource_value_int(M2MServer::DefaultMinPeriod) == 10);
   CHECK(server->resource_value_int(M2MServer::DefaultMaxPeriod) == 10);
   CHECK(server->resource_value_int(M2MServer::DisableTimeout) == 10);
   CHECK(server->resource_value_int(M2MServer::NotificationStorage) == 10);
   CHECK(server->resource_value_int(M2MServer::Lifetime) == 10);
   CHECK(server->resource_value_int(M2MServer::Binding) == 0);
   CHECK(server->resource_value_int(M2MServer::RegistrationUpdate) == 0);
   CHECK(server->resource_value_int(M2MServer::Disable) == 0);

   delete m2mobjectinstance_stub::resource;
   m2mobjectinstance_stub::resource = NULL;

   free(m2mresourceinstance_stub::value);
   m2mresourceinstance_stub::value = NULL;
}

void Test_M2MServer::test_resource_value_string()
{
    String test = "string";
    uint8_t value[] = {"string"};

    m2mresourceinstance_stub::value = (uint8_t*)malloc((uint32_t)sizeof(value));
    memset(m2mresourceinstance_stub::value,0,(uint32_t)sizeof(value));
    memcpy(m2mresourceinstance_stub::value,value,sizeof(value));
    m2mresourceinstance_stub::int_value = (uint32_t)sizeof(value);

    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name", "name", M2MResourceInstance::STRING, M2MBase::Dynamic);

    CHECK(server->resource_value_string(M2MServer::Binding) == test);

    CHECK(server->resource_value_string(M2MServer::Disable) == "");
    CHECK(server->resource_value_string(M2MServer::RegistrationUpdate) == "");
    CHECK(server->resource_value_string(M2MServer::Lifetime) == "");
    CHECK(server->resource_value_string(M2MServer::NotificationStorage) == "");
    CHECK(server->resource_value_string(M2MServer::DisableTimeout) == "");
    CHECK(server->resource_value_string(M2MServer::DefaultMaxPeriod) == "");
    CHECK(server->resource_value_string(M2MServer::DefaultMinPeriod) == "");
    CHECK(server->resource_value_string(M2MServer::ShortServerID) == "");

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;
}

void Test_M2MServer::test_is_resource_present()
{
    m2mobjectinstance_stub::resource = new M2MResource(*m2mobject_stub::inst,"name","type",M2MResourceInstance::STRING,M2MBase::Dynamic);

    CHECK(server->is_resource_present(M2MServer::ShortServerID) == true);

    delete m2mobjectinstance_stub::resource;
    m2mobjectinstance_stub::resource = NULL;

    CHECK(server->is_resource_present(M2MServer::DefaultMinPeriod) == false);
}

void Test_M2MServer::test_total_resource_count()
{
    M2MResource *res = new M2MResource(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(res);
    M2MResource *res2 = new M2MResource(*m2mobject_stub::inst,"test","test",M2MResourceInstance::STRING,M2MBase::Dynamic);

    m2mobjectinstance_stub::resource_list.push_back(res2);

    CHECK(server->total_resource_count() == 2);

    m2mobjectinstance_stub::resource_list.clear();

    delete res2;
    delete res;
}
