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
#ifndef TEST_M2M_TLV_DESERIALIZER_H
#define TEST_M2M_TLV_DESERIALIZER_H

#include "m2mtlvdeserializer.h"

class Test_M2MTLVDeserializer
{
public:
    Test_M2MTLVDeserializer();

    virtual ~Test_M2MTLVDeserializer();

    void test_is_object_instance();

    void test_is_resource();

    void test_is_multiple_resource();

    void test_is_resource_instance();

    void test_deserialise_object_instances();

    void test_deserialize_resources();

    void test_deserialize_resource_instance_1();

    void test_deserialize_resource_instance();

    void test_instance_id();

    M2MTLVDeserializer *deserializer;
};

#endif // TEST_M2M_TLV_DESERIALIZER_H
