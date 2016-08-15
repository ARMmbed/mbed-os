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
#ifndef TEST_M2M_TLV_SERIALIZER_H
#define TEST_M2M_TLV_SERIALIZER_H

#include "m2mtlvserializer.h"

class Test_M2MTLVSerializer
{
public:    
    Test_M2MTLVSerializer();

    virtual ~Test_M2MTLVSerializer();

    void test_serialize_object();

    void test_serialize_object_instance();

    void test_serialize_resource();

    void test_serialize_resource_instance();

    M2MTLVSerializer *serializer;
};

#endif // TEST_M2M_TLV_SERIALIZER_H
