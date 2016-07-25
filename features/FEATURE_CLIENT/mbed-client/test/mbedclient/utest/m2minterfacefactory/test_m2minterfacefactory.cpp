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
#include "test_m2minterfacefactory.h"
#include "m2minterfaceobserver.h"
#include "m2mserver.h"
#include "m2mdevice.h"
#include "m2mfirmware.h"

class TestObserver : public M2MInterfaceObserver {

public:
    TestObserver(){}
    void bootstrap_done(M2MSecurity */*server_object*/){}
    void object_registered(M2MSecurity */*security_object*/,
                           const M2MServer &/*server_object*/) {}

    void object_unregistered(M2MSecurity */*server_object*/){}

    void registration_updated(M2MSecurity */*security_object*/,
                              const M2MServer &/*server_object*/){}

    void error(M2MInterface::Error /*error*/){}

    void value_updated(M2MBase */*base*/, M2MBase::BaseType /*type*/){}
};

Test_M2MInterfaceFactory::Test_M2MInterfaceFactory()
{
}

Test_M2MInterfaceFactory::~Test_M2MInterfaceFactory()
{

}

void Test_M2MInterfaceFactory::test_create_interface()
{
    TestObserver test_obs;
    M2MInterface *test = M2MInterfaceFactory::create_interface(test_obs,
                                                "endpoint_name",
                                                "endpoint_type",
                                                120,
                                                8000,
                                                "domain");
    CHECK(test != NULL);
    delete test;
    test = NULL;

    // Endpoint name and type of length more than 64 characters
    test = M2MInterfaceFactory::create_interface(test_obs,
                                                "endpoint_name-very-long-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1",
                                                "endpoint_type-very-long-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1",
                                                120,
                                                8000,
                                                "domain");
    CHECK(test == NULL);
    delete test;

    // Domain length more than 64 characters
    test = M2MInterfaceFactory::create_interface(test_obs,
                                                "endpoint_name",
                                                "endpoint_type",
                                                120,
                                                8000,
                                                "domaidomaindomaindomaindomaindomaindomaindomaindomaindomaindomaindomaindomaindomain");
    CHECK(test == NULL);
    delete test;

}

void Test_M2MInterfaceFactory::test_create_security()
{
    M2MSecurity *test = M2MInterfaceFactory::create_security(M2MSecurity::Bootstrap);
    CHECK(test != NULL);
    delete test;
    test = NULL;
}

void Test_M2MInterfaceFactory::test_create_device()
{
    M2MDevice *test = M2MInterfaceFactory::create_device();
    CHECK(test != NULL);
    delete test;
    test = NULL;
}

void Test_M2MInterfaceFactory::test_create_firmware()
{
    M2MFirmware *test = M2MInterfaceFactory::create_firmware();
    CHECK(test != NULL);
    delete test;
    test = NULL;
}

void Test_M2MInterfaceFactory::test_create_server()
{
    M2MServer *test = M2MInterfaceFactory::create_server();
    CHECK(test != NULL);
    delete test;
    test = NULL;
}

void Test_M2MInterfaceFactory::test_create_object()
{
    M2MObject *test = M2MInterfaceFactory::create_object("name");
    CHECK(test != NULL);
    delete test;
    test = NULL;

    // Length 65, should fail
    String max_lenght = "65656565656565656565656565656565656565656565656565656565656565656";
    CHECK(M2MInterfaceFactory::create_object("") == NULL);
    CHECK(M2MInterfaceFactory::create_object(max_lenght) == NULL);
}
