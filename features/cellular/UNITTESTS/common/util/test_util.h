/*
 * Copyright (c) 2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

class Test_util
{
public:
    Test_util();

    virtual ~Test_util();

    void test_util_uint_to_binary_string();

    void test_util_char_str_to_hex();

    void test_util_convert_ipv6();

    void test_util_prefer_ipv6();

    void test_util_separate_ip_addresses();
};

#endif // TEST_UTIL_H

