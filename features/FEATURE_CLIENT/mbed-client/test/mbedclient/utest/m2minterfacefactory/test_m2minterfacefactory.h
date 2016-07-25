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
#ifndef TEST_M2M_INTERFACE_FACTORY_H
#define TEST_M2M_INTERFACE_FACTORY_H

#include "m2minterfacefactory.h"

class Test_M2MInterfaceFactory
{

public:
    Test_M2MInterfaceFactory();
    virtual ~Test_M2MInterfaceFactory();

    void test_create_interface();

    void test_create_security();

    void test_create_device();

    void test_create_firmware();

    void test_create_server();

    void test_create_object();
};

#endif // TEST_M2M_INTERFACE_FACTORY_H
