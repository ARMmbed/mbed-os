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
#include "test_m2mtlvdeserializer.h"
#include "m2mtlvserializer.h"
#include "m2mobject_stub.h"
#include "m2mobjectinstance_stub.h"
#include "m2mresource_stub.h"
#include "m2mresourceinstance_stub.h"
#include "m2mbase_stub.h"


Test_M2MTLVDeserializer::Test_M2MTLVDeserializer()
{
    deserializer = new M2MTLVDeserializer();
}

Test_M2MTLVDeserializer::~Test_M2MTLVDeserializer()
{
    delete deserializer;
    m2mbase_stub::clear();
    m2mresourceinstance_stub::clear();
    m2mobjectinstance_stub::clear();
    m2mobject_stub::clear();
    m2mresource_stub::clear();
}

void Test_M2MTLVDeserializer::test_is_object_instance()
{
    uint8_t data_resource[] = {0xC3,0x0};
    uint8_t data_object[] = {0x08,0x0};

    CHECK(deserializer->is_object_instance(data_resource) == false);
    CHECK(deserializer->is_object_instance(data_object) == true);
}

void Test_M2MTLVDeserializer::test_is_resource()
{
    uint8_t data_resource[] = {0xC3,0x0};
    uint8_t data_object[] = {0x08,0x0};

    CHECK(deserializer->is_resource(data_resource) == true);
    CHECK(deserializer->is_resource(data_object) == false);

}

void Test_M2MTLVDeserializer::test_is_multiple_resource()
{
    uint8_t data_resource[] = {0xC3,0x0};
    uint8_t data_object[] = {0x88,0x0};

    CHECK(deserializer->is_multiple_resource(data_resource) == false);
    CHECK(deserializer->is_multiple_resource(data_object) == true);
}

void Test_M2MTLVDeserializer::test_is_resource_instance()
{
    uint8_t data_resource[] = {0xC3,0x0};
    uint8_t data_object[] = {0x44,0x0};

    CHECK(deserializer->is_resource_instance(data_object) == true);
    CHECK(deserializer->is_resource_instance(data_resource) == false);
}

void Test_M2MTLVDeserializer::test_deserialise_object_instances()
{
    uint32_t size = 0;
    uint8_t *data = 0;

    String *name = new String("1");
    M2MObject *object = new M2MObject(*name);
    M2MObjectInstance* instance = new M2MObjectInstance(*name,*object);

    M2MObjectInstance* instance_1 = new M2MObjectInstance("2",*object);


    m2mobject_stub::instance_list.push_back(instance);   

    M2MResource* resource = new M2MResource(*instance,
                                                   *name,
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);

    M2MResource* resource_1 = new M2MResource(*instance,
                                                   "2",
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);


    m2mobjectinstance_stub::resource_list.push_back(resource);
    m2mobjectinstance_stub::resource_list.push_back(resource_1);

    m2mbase_stub::name_id_value = 0;
    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    M2MTLVSerializer *serializer = new M2MTLVSerializer();

    data = serializer->serialize( m2mobject_stub::instance_list,size);

    CHECK(M2MTLVDeserializer::None == deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post));

    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    m2mbase_stub::operation = M2MBase::GET_ALLOWED;

    data = serializer->serialize( m2mobject_stub::instance_list,size);

    CHECK(M2MTLVDeserializer::NotAllowed == deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post));

    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    u_int8_t value[] = {"value"};
    m2mresourceinstance_stub::value = value;
    m2mresourceinstance_stub::int_value = sizeof(value);

    data = serializer->serialize( m2mobject_stub::instance_list,size);

    delete serializer;

    CHECK(M2MTLVDeserializer::None == deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post));
    CHECK(M2MTLVDeserializer::None == deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Put));

    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    m2mobject_stub::instance_list.push_back(instance_1);

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;
    serializer = new M2MTLVSerializer();

    data = serializer->serialize( m2mobject_stub::instance_list,size);

    delete serializer;

    CHECK(M2MTLVDeserializer::None != deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post));
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    CHECK(M2MTLVDeserializer::NotValid == deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post));

    M2MResourceInstance* res_instance = new M2MResourceInstance(*name,
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);

    M2MResourceInstance* res_instance_1 = new M2MResourceInstance("2",
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);


    m2mresource_stub::bool_value = true;
    m2mresource_stub::list.push_back(res_instance);
    m2mresource_stub::list.push_back(res_instance_1);

    m2mobject_stub::instance_list.pop_back();

    serializer = new M2MTLVSerializer();
    data = serializer->serialize( m2mobject_stub::instance_list,size);
    delete serializer;

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    m2mbase_stub::string_value = new String("name");
    m2mobjectinstance_stub::create_resource_instance = new M2MResource(*instance,
                                                                       *name,
                                                                       *name,
                                                                       M2MResourceInstance::INTEGER,
                                                                       M2MResource::Dynamic,
                                                                       false);

    deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post);

    deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Put);

    size = 0;
    if(data) {
        free(data);
        data = 0;
    }

    m2mbase_stub::name_id_value = 300;

    m2mresourceinstance_stub::int_value = 6;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( m2mobject_stub::instance_list,size);
    delete serializer;

    deserializer->deserialise_object_instances(data,size,*object, M2MTLVDeserializer::Post);
    deserializer->deserialise_object_instances(data,size,*object, M2MTLVDeserializer::Put);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }
    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    m2mresourceinstance_stub::int_value = 256;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( m2mobject_stub::instance_list,size);
    delete serializer;

    deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post);    
    deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Put);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }
    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    m2mresourceinstance_stub::int_value = 65536;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( m2mobject_stub::instance_list,size);
    delete serializer;

   deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Post);
   deserializer->deserialise_object_instances(data,size,*object,M2MTLVDeserializer::Put);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    delete m2mbase_stub::string_value;
    m2mbase_stub::string_value = NULL;

    delete m2mobjectinstance_stub::create_resource_instance;
    m2mobjectinstance_stub::create_resource_instance = NULL;

    delete res_instance;
    delete res_instance_1;
    delete name;
    delete resource;
    delete resource_1;
    delete instance;
    delete instance_1;
    delete object;
}

void Test_M2MTLVDeserializer::test_deserialize_resources()
{
    uint8_t data_resource[] = {0xC3,0x0,'A','R','M'};

    String *name = new String("1");
    M2MObject *object = new M2MObject(*name);
    M2MObjectInstance* instance = new M2MObjectInstance(*name,*object);

    m2mobject_stub::instance_list.push_back(instance);



    M2MResource* resource = new M2MResource(*instance,
                                                   *name,
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);


    m2mobjectinstance_stub::resource_list.push_back(resource);

    CHECK(deserializer->is_resource(data_resource) == true);

    uint8_t invalid_data_resource[] = {0x65,0x0,'A','R','M'};

    CHECK( M2MTLVDeserializer::NotValid == deserializer->deserialize_resources(invalid_data_resource,
                                                                               sizeof(invalid_data_resource),
                                                                               *instance,M2MTLVDeserializer::Post));

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    m2mobjectinstance_stub::create_resource = new M2MResource(*instance,
                                                              *name,
                                                              *name,
                                                              M2MResourceInstance::INTEGER,
                                                              M2MResource::Dynamic,
                                                              false);

    CHECK( M2MTLVDeserializer::None == deserializer->deserialize_resources(data_resource,sizeof(data_resource),*instance,M2MTLVDeserializer::Post));

    delete m2mobjectinstance_stub::create_resource;
    m2mobjectinstance_stub::create_resource = NULL;

    CHECK( M2MTLVDeserializer::NotFound == deserializer->deserialize_resources(data_resource,sizeof(data_resource),*instance,M2MTLVDeserializer::Put));

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    deserializer->deserialize_resources(data_resource,sizeof(data_resource),*instance,M2MTLVDeserializer::Post);


    M2MResourceInstance* res_instance = new M2MResourceInstance(*name,
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);


    m2mresource_stub::bool_value = true;
    m2mresource_stub::list.push_back(res_instance);

//    M2MResourceInstance* res_instance_1 = new M2MResourceInstance("2",
//                                                                *name,
//                                                                M2MResourceInstance::INTEGER,
//                                                                *instance);

//    m2mresource_stub::list.push_back(res_instance_1);

    uint8_t data_multiple_resource[] = {0x86,0x6, 0x81,0x0, 0x01, 0x41, 0x01, 0x05};
    CHECK(deserializer->is_multiple_resource(data_multiple_resource) == true);
    deserializer->deserialize_resources(data_multiple_resource,sizeof(data_multiple_resource),*instance,M2MTLVDeserializer::Post);

    M2MTLVSerializer *serializer = new M2MTLVSerializer();

    m2mbase_stub::name_id_value = 1;
    uint32_t size = 0;
    uint8_t* data = serializer->serialize(resource,size);

    delete serializer;

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    deserializer->deserialize_resources(data,size,*instance,M2MTLVDeserializer::Post);

    free(data);

    delete res_instance;
//    delete res_instance_1;
    delete resource;
    delete instance;
    delete object;
    delete name;

    m2mresourceinstance_stub::clear();
    m2mresource_stub::clear();
    m2mobjectinstance_stub::clear();
    m2mobject_stub::clear();
}

void Test_M2MTLVDeserializer::test_deserialize_resource_instance_1()
{
    uint8_t data_resource[] = {0xC3,0x0,'A','R','M'};

    m2mbase_stub::name_id_value = 1;

    String *name = new String("1");
    M2MObject *object = new M2MObject(*name);
    M2MObjectInstance* instance = new M2MObjectInstance(*name,*object);

    m2mobject_stub::instance_list.push_back(instance);



    M2MResource* resource = new M2MResource(*instance,
                                                   *name,
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);


    m2mobjectinstance_stub::resource_list.push_back(resource);

    CHECK(deserializer->is_resource(data_resource) == true);

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    deserializer->deserialize_resource_instances(data_resource,sizeof(data_resource),
                                                 *resource,M2MTLVDeserializer::Put);

    M2MResourceInstance* res_instance = new M2MResourceInstance(*name,
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);


    m2mresource_stub::bool_value = true;
    m2mresource_stub::list.push_back(res_instance);

    M2MResourceInstance* res_instance_1 = new M2MResourceInstance("2",
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);

    m2mresource_stub::list.push_back(res_instance_1);

    M2MTLVSerializer *serializer = new M2MTLVSerializer();

    uint32_t size = 0;
    uint8_t* data = serializer->serialize( resource,size);

    delete serializer;

    deserializer->deserialize_resource_instances(data,size,
                                                *resource,M2MTLVDeserializer::Put);


    free(data);

    delete res_instance;
    delete res_instance_1;
    delete resource;
    delete instance;
    delete object;
    delete name;

    m2mresourceinstance_stub::clear();
    m2mresource_stub::clear();
    m2mobjectinstance_stub::clear();
    m2mobject_stub::clear();
}


void Test_M2MTLVDeserializer::test_deserialize_resource_instance()
{
    uint32_t size = 0;
    uint8_t *data = 0;

    String *name = new String("1");
    M2MObject *object = new M2MObject(*name);
    M2MObjectInstance* instance = new M2MObjectInstance(*name,*object);

    M2MObjectInstance* instance_1 = new M2MObjectInstance("2",*object);


    m2mobject_stub::instance_list.push_back(instance);
    m2mobject_stub::instance_list.push_back(instance_1);


    M2MResource* resource = new M2MResource(*instance,
                                                   *name,
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);

    M2MResource* resource_1 = new M2MResource(*instance,
                                                   "2",
                                                   *name,
                                                   M2MResourceInstance::INTEGER,
                                                   M2MResource::Dynamic,
                                                   false);

    m2mobjectinstance_stub::resource_list.push_back(resource);
    m2mobjectinstance_stub::resource_list.push_back(resource_1);

    m2mbase_stub::name_id_value = 0;

    M2MTLVSerializer *serializer = new M2MTLVSerializer();

    data = serializer->serialize( resource,size);

    delete serializer;

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    m2mbase_stub::operation = M2MBase::NOT_ALLOWED;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    M2MResourceInstance* res_instance = new M2MResourceInstance(*name,
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);

    M2MResourceInstance* res_instance_1 = new M2MResourceInstance("2",
                                                                *name,
                                                                M2MResourceInstance::INTEGER,
                                                                *instance);


    m2mbase_stub::operation = M2MBase::GET_ALLOWED;

    m2mresource_stub::bool_value = true;
    m2mresource_stub::list.push_back(res_instance);
    m2mresource_stub::list.push_back(res_instance_1);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( resource,size);
    delete serializer;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);

    m2mbase_stub::operation = M2MBase::GET_PUT_POST_ALLOWED;
    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);

    m2mresource_stub::list.pop_back();
    m2mresource_stub::list.pop_back();

    CHECK(M2MTLVDeserializer::NotAllowed ==deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post));

    CHECK(M2MTLVDeserializer::NotFound == deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Put));

    size = 0;
    if(data) {
        free(data);
        data = 0;
    }

    serializer = new M2MTLVSerializer();
    data = serializer->serialize( resource,size);
    delete serializer;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Put);
    size = 0;
    if(data) {
        free(data);
        data = 0;
    }

    m2mresource_stub::list.push_back(res_instance);
    m2mresource_stub::list.push_back(res_instance_1);

    m2mbase_stub::name_id_value = 300;

    m2mresourceinstance_stub::int_value = 6;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( resource,size);
    delete serializer;

    deserializer->deserialize_resource_instances(data,size,*resource, M2MTLVDeserializer::Post);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }
    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    m2mresourceinstance_stub::int_value = 256;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( resource,size);
    delete serializer;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }
    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    m2mresourceinstance_stub::int_value = 65536;
    m2mresourceinstance_stub::value = (uint8_t*) malloc(m2mresourceinstance_stub::int_value);
    serializer = new M2MTLVSerializer();
    data = serializer->serialize( resource,size);
    delete serializer;

    deserializer->deserialize_resource_instances(data,size,*resource,M2MTLVDeserializer::Post);
    size = 0;

    if(data) {
        free(data);
        data = 0;
    }

    free(m2mresourceinstance_stub::value);
    m2mresourceinstance_stub::value = NULL;

    delete res_instance;
    delete res_instance_1;
    delete name;
    delete resource;
    delete resource_1;
    delete instance;
    delete instance_1;
    delete object;
}

void Test_M2MTLVDeserializer::test_instance_id()
{
    uint8_t data_resource[] = {0xC3,0x1};
    uint8_t data_object[] = {0x08,0x2};

    CHECK(deserializer->instance_id(data_resource) == 1);
    CHECK(deserializer->instance_id(data_object) == 2);
}
