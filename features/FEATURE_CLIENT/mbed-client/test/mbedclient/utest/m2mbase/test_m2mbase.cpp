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
#include "test_m2mbase.h"
#include "m2mobservationhandler.h"
#include "m2mreportobserver.h"
#include "m2mreporthandler.h"
#include "m2mreporthandler_stub.h"
static bool value_update_called = false;
static void value_updated_function(const char* name) {
    value_update_called = true;
}

class MyTest{
public:
    void value_updated_function(const char* name) {
        visited = true;
    }

    bool visited;
};

class Handler : public M2MObservationHandler {

public:

    Handler() : visited(false) {}
    ~Handler(){}
    void observation_to_be_sent(M2MBase *, uint16_t, m2m::Vector<uint16_t>, bool){
        visited = true;
    }
    void send_delayed_response(M2MBase *){}
    void resource_to_be_deleted(const String &){visited=true;}
    void remove_object(M2MBase *){visited = true;}
    void value_updated(M2MBase *,const String&){visited = true;}

    void clear() {visited = false;}
    bool visited;
};

class Observer : public M2MReportObserver {
public:
    Observer(){}
    ~Observer(){}

    void observation_to_be_sent(m2m::Vector<uint16_t>,bool){}
};

Test_M2MBase::Test_M2MBase()
    :M2MBase("name",M2MBase::Static)
{

}

Test_M2MBase::~Test_M2MBase()
{
}

void Test_M2MBase::test_copy_constructor()
{
    String test_string = "test";
    //Test stack constructor
    Test_M2MBase b;
    Test_M2MBase c(b);
    Test_M2MBase a;
    a = b;

    //Test heap constructor
    Test_M2MBase* test = new Test_M2MBase();
    test->_interface_description = test_string;

    test->_token_length = 3;
    test->_token = (u_int8_t *)malloc(test->_token_length);

    Observer obs;
    test->_report_handler = new M2MReportHandler(obs);

    Test_M2MBase* copy = new Test_M2MBase(*test);

    CHECK(copy->_interface_description.compare(0,test_string.size(),test_string) == 0);

    CHECK(copy->_token != NULL);

    CHECK(copy->_report_handler != NULL);

    delete test;
    delete copy;
}

void Test_M2MBase::test_assignment_operator()
{
    //Test stack
    M2MBase b("name", M2MBase::Static);
    Test_M2MBase a;
    M2MBase c = a;
    c = b;

    //Test heap
    Test_M2MBase* test = new Test_M2MBase();
    Test_M2MBase* test2 = new Test_M2MBase();
    Test_M2MBase* test3 = new Test_M2MBase();

    test->operator=(*test3);
    delete test3;

    test->_token_length = 3;
    test->_token = (u_int8_t *)malloc(test->_token_length);

    Observer obs;
    test->_report_handler = new M2MReportHandler(obs);

    test2->_token_length = 8;
    test2->_token = (u_int8_t *)malloc(test2->_token_length);

    Observer obs2;
    test2->_report_handler = new M2MReportHandler(obs2);

    *test = *test2;

    CHECK(test->_token != NULL);

    CHECK(test->_report_handler != NULL);

    delete test2;
    delete test;
}

void Test_M2MBase::test_set_operation()
{
    M2MBase::Operation test = M2MBase::GET_ALLOWED;
    set_operation(test);

    CHECK(test == this->_operation);

    this->_mode = M2MBase::Dynamic;
    test = M2MBase::PUT_ALLOWED;
    set_operation(test);

    CHECK(test == this->_operation);
}

void Test_M2MBase::test_set_base_type()
{
    set_base_type(M2MBase::ObjectInstance);

    CHECK(M2MBase::ObjectInstance == this->_base_type);
}

void Test_M2MBase::test_set_interface_description()
{
    String test = "interface_description";
    set_interface_description(test);

    CHECK(test == this->_interface_description);
}

void Test_M2MBase::test_set_uri_path()
{
    String test = "10/0/1";
    set_uri_path(test);
    CHECK(test == this->_uri_path);
}

void Test_M2MBase::test_uri_path()
{
    String test = "interface_description";
    this->_uri_path = test;

    CHECK(test == uri_path());
}

void Test_M2MBase::test_set_resource_type()
{
    String test = "resource_type";
    set_resource_type(test);

    CHECK(test == this->_resource_type);
}

void Test_M2MBase::test_set_coap_content_type()
{
    u_int8_t test = 1;
    set_coap_content_type(test);

    CHECK(test == this->_coap_content_type);
}

void Test_M2MBase::test_set_instance_id()
{
    u_int16_t test = 1;
    set_instance_id(test);

    CHECK(test == this->_instance_id);
}

void Test_M2MBase::test_set_observable()
{
    bool test = true;
    set_observable(test);

    CHECK(test == this->_observable);
}

void Test_M2MBase::test_add_observation_level()
{
    add_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::R_Attribute == this->_observation_level);

    add_observation_level(M2MBase::O_Attribute);
    CHECK(M2MBase::OR_Attribute == this->_observation_level);
}

void Test_M2MBase::test_remove_observation_level()
{
    this->_observation_level = M2MBase::OR_Attribute;
    remove_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::O_Attribute == this->_observation_level);

    remove_observation_level(M2MBase::O_Attribute);
    CHECK(M2MBase::None == this->_observation_level);

    this->_observation_level = M2MBase::OI_Attribute;
    remove_observation_level(M2MBase::R_Attribute);
    CHECK(M2MBase::OI_Attribute == this->_observation_level);

    remove_observation_level(M2MBase::OI_Attribute);
    CHECK(M2MBase::None == this->_observation_level);
    remove_observation_level(M2MBase::OI_Attribute);
    CHECK(M2MBase::None == this->_observation_level);
}

void Test_M2MBase::test_set_under_observation()
{
    Handler handler;

    this->_base_type = M2MBase::ObjectInstance;

    bool test = true;
    set_under_observation(test,NULL);
    set_under_observation(test,&handler);

    CHECK(&handler == this->_observation_handler);

    set_under_observation(test,NULL);

    test = false;
    set_under_observation(test,NULL);

    test = false;
    set_under_observation(test,&handler);

    set_under_observation(test,&handler);
}

void Test_M2MBase::test_set_observation_token()
{
    this->_token_length = 4;
    this->_token = (u_int8_t *)malloc(this->_token_length);
    String test = "token";
    set_observation_token((const u_int8_t*)test.c_str(), (u_int8_t)test.size());

    CHECK(this->_token_length == 5);
}

void Test_M2MBase::test_is_observable()
{
    bool test = false;
    this->_observable = test;

    CHECK(test == is_observable());
}

void Test_M2MBase::test_observation_level()
{
    this->_observation_level = M2MBase::OR_Attribute;
    CHECK(M2MBase::OR_Attribute == this->observation_level());
}

void Test_M2MBase::test_get_observation_token()
{
    u_int8_t test_value[] = {"val"};
    u_int32_t value_length((u_int32_t)sizeof(test_value));

    u_int8_t* out_value = (u_int8_t *)malloc(value_length);
    u_int32_t out_size = value_length;
    memcpy((u_int8_t *)out_value, (u_int8_t *)test_value, value_length);

    u_int8_t test[] = {"token"};
    this->_token_length = (u_int8_t)sizeof(test);
    this->_token = (u_int8_t *)malloc(this->_token_length);
    memcpy((u_int8_t *)this->_token, (u_int8_t *)test, this->_token_length);

    get_observation_token(out_value,out_size);

    CHECK(out_size == 6);

    free(out_value);
}

void Test_M2MBase::test_mode()
{
    CHECK(M2MBase::Static == mode());
}

void Test_M2MBase::test_observation_number()
{
    u_int8_t test = 1;
    this->_observation_number = test;

    CHECK(test == observation_number());
}

void Test_M2MBase::test_operation()
{
    M2MBase::Operation test = M2MBase::DELETE_ALLOWED;
    this->_operation = test;

    CHECK(test == operation());
}

void Test_M2MBase::test_name()
{
    String test = "name";
    this->_name = test;

    CHECK(test == name());
}

void Test_M2MBase::test_name_id()
{
    int id = 10;
    this->_name_id = id;

    CHECK(id == name_id());
}

void Test_M2MBase::test_instance_id()
{
    u_int16_t test = 1;
    this->_instance_id = test;

    CHECK(test == instance_id());
}

void Test_M2MBase::test_interface_description()
{
    String test = "interface_description";
    this->_interface_description = test;

    CHECK(test == interface_description());
}

void Test_M2MBase::test_resource_type()
{
    String test = "resource_type";
    this->_resource_type = test;

    CHECK(test == resource_type());
}

void Test_M2MBase::test_coap_content_type()
{
    u_int8_t test = 1;
    this->_coap_content_type = test;

    CHECK(test == coap_content_type());
}

void Test_M2MBase::test_base_type()
{
    this->_base_type = M2MBase::ObjectInstance;

    CHECK(M2MBase::ObjectInstance == base_type());
}

//void Test_M2MBase::test_set_value()
//{
//    u_int8_t value[] = {"value2"};
//    this->_value = (u_int8_t*)malloc(sizeof(u_int8_t));

//    CHECK(set_value(value,(u_int32_t)sizeof(value)) == true);
//    CHECK( this->_value_length == sizeof(value));
//    CHECK( *this->_value == *value);

//    Observer obs;
//    this->_report_handler = new M2MReportHandler(obs);

//    u_int8_t value2[] = {"12"};
//    CHECK(set_value(value2,(u_int32_t)sizeof(value2), true) == true);
//}

//void Test_M2MBase::test_get_value()
//{
//    u_int8_t test_value[] = {"value3"};
//    u_int32_t value_length((u_int32_t)sizeof(test_value));

//    u_int8_t* out_value = (u_int8_t *)malloc(1);
//    u_int32_t out_size = 1;

//    this->_value = (u_int8_t *)malloc(value_length);
//    this->_value_length = value_length;
//    memcpy((u_int8_t *)this->_value, (u_int8_t *)test_value, value_length);

//    get_value(out_value,out_size);

//    CHECK(out_size == value_length);

//    free(out_value);
//}

void Test_M2MBase::test_handle_observation_attribute()
{
    char query[] = "wrong";
    char* s = query;
    bool ret = handle_observation_attribute(s);
    CHECK(ret == false);

    Observer obs;
    this->_report_handler = new M2MReportHandler(obs);

    m2mreporthandler_stub::bool_return = true;
    ret = handle_observation_attribute(s);
    CHECK(ret == true);

    this->_is_under_observation = true;
    ret = handle_observation_attribute(s);
    CHECK(ret == true);

    this->_is_under_observation = true;
    m2mreporthandler_stub::bool_return = false;
    ret = handle_observation_attribute(s);
    CHECK(ret == false);

}

void Test_M2MBase::test_observation_to_be_sent()
{
    Handler handler;
    Vector<uint16_t> list;
    observation_to_be_sent(list);
    CHECK(handler.visited == false);

    this->_base_type = M2MBase::ObjectInstance;

    bool test = true;
    set_under_observation(test,&handler);

    observation_to_be_sent(list);

    CHECK(handler.visited == true);
}

void Test_M2MBase::test_remove_resource_from_coap()
{
    Handler handler;

    this->_base_type = M2MBase::ObjectInstance;

    const String s = "test";
    remove_resource_from_coap(s);
    CHECK(handler.visited == false);

    bool test = true;
    set_under_observation(test,&handler);
    remove_resource_from_coap(s);

    CHECK(handler.visited == true);
}

void Test_M2MBase::test_remove_object_from_coap()
{
    Handler handler;

    this->_base_type = M2MBase::ObjectInstance;

    remove_object_from_coap();
    CHECK(handler.visited == false);

    bool test = true;
    set_under_observation(test,&handler);
    remove_object_from_coap();

    CHECK(handler.visited == true);
}

void Test_M2MBase::test_handle_get_request()
{
    CHECK(this->handle_get_request(NULL,NULL,NULL) == NULL);
}

void Test_M2MBase::test_handle_put_request()
{
    bool execute = false;
    CHECK(this->handle_put_request(NULL,NULL,NULL, execute) == NULL);
}

void Test_M2MBase::test_handle_post_request()
{
    bool execute = false;
    CHECK(this->handle_post_request(NULL,NULL,NULL, execute) == NULL);
}

void Test_M2MBase::test_memory_alloc()
{
    CHECK(memory_alloc(0) == 0);
    uint8_t *ptr = 0;
    ptr = (uint8_t*)memory_alloc(sizeof(uint8_t));
    CHECK(ptr != NULL);
    memory_free(ptr);
}

void Test_M2MBase::test_memory_free()
{
    uint8_t *ptr = (uint8_t*)memory_alloc(sizeof(uint8_t));
    memory_free((void*)ptr);
    //memory leak test will fail, if there is a leak, so no need for CHECK
}

void Test_M2MBase::test_report_handler()
{
    CHECK(report_handler() == NULL);
}

void Test_M2MBase::test_observation_handler()
{
    CHECK(observation_handler() == NULL);
}

void Test_M2MBase::test_id_number()
{
    M2MBase b("10", M2MBase::Static);
    CHECK(b.name_id() == 10);
    M2MBase * test1 = new M2MBase("66567",M2MBase::Static);
    CHECK(test1->name_id() == -1);
    delete test1;

}

void Test_M2MBase::test_set_register_uri()
{
    this->set_register_uri(false);
    CHECK(this->_register_uri == false);
}

void Test_M2MBase::test_register_uri()
{
    this->_register_uri = false;
    CHECK(this->register_uri() == false);
}

void Test_M2MBase::test_set_observation_number()
{
    set_observation_number(0);
    CHECK(0 == this->_observation_number);
}

void Test_M2MBase::test_set_max_age()
{
    this->set_max_age(10000);
    CHECK(this->_max_age == 10000);
}

void Test_M2MBase::test_max_age()
{
    this->_max_age = 10000;
    CHECK(this->max_age() == 10000);
}

void Test_M2MBase::test_is_under_observation()
{
    CHECK(false == is_under_observation());
    this->_is_under_observation = true;
    CHECK(true == is_under_observation());
}

void Test_M2MBase::test_value_updated_function()
{
    MyTest test;
    test.visited = false;

    CHECK(this->is_value_updated_function_set() == false);

    this->set_value_updated_function(value_updated_callback(&test,&MyTest::value_updated_function));
    this->execute_value_updated("test");
    CHECK(this->is_value_updated_function_set() == true);
    CHECK(test.visited == true);

    value_update_called = false;
    this->set_value_updated_function(value_updated_callback2(value_updated_function));
    this->execute_value_updated("test");
    CHECK(value_update_called == true);
}

