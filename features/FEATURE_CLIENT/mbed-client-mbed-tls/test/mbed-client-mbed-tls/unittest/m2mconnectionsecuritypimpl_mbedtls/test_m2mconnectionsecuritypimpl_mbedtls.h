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
#ifndef TEST_M2M_CONNECTION_SECURITY_PIMPL_H
#define TEST_M2M_CONNECTION_SECURITY_PIMPL_H

#include "mbed-client-mbedtls/m2mconnectionsecuritypimpl.h"

uint32_t test_random_callback(void);

class Test_M2MConnectionSecurityPimpl
{
public:
    Test_M2MConnectionSecurityPimpl();

    virtual ~Test_M2MConnectionSecurityPimpl();

    void test_constructor();

    void test_destructor();

    void test_reset();

    void test_init();

    void test_connect();

    void test_start_connecting_non_blocking();

    void test_continue_connecting();

    void test_send_message();

    void test_read();

    void test_timer_expired();

    void test_set_random_number_callback();

    void test_set_entropy_callback();

};


#endif // TEST_M2M_CONNECTION_SECURITY_PIMPL_H
