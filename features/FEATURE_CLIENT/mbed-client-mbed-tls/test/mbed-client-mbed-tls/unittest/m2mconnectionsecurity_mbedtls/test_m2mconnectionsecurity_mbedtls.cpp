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
#include "test_m2mconnectionsecurity_mbedtls.h"
#include "m2mtimerobserver.h"
#include "m2msecurity.h"
#include "m2msecurity_stub.h"
#include "mbedtls_stub.h"
#include "m2mconnectionhandler.h"
#include "mbed-client-mbed-os/m2mconnectionhandlerpimpl.h"
#include "m2mconnectionsecuritypimpl_stub.h"

entropy_cb ent_cb;

class TestObserver : public M2MConnectionObserver {

public:
    TestObserver(){}
    void data_available(uint8_t*,
                        uint16_t,
                        const M2MConnectionObserver::SocketAddress &){}

    void socket_error(uint8_t error_code){}

    void address_ready(const M2MConnectionObserver::SocketAddress &,
                       M2MConnectionObserver::ServerType,
                       const uint16_t){}

    void data_sent(){}
};

Test_M2MConnectionSecurity::Test_M2MConnectionSecurity()
{
    mbedtls_stub::clear();
    m2msecurity_stub::clear();
}

Test_M2MConnectionSecurity::~Test_M2MConnectionSecurity()
{
}

void Test_M2MConnectionSecurity::test_constructor()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
}

void Test_M2MConnectionSecurity::test_destructor()
{
    M2MConnectionSecurity* impl = new M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    delete impl;
    //Memory leak detector will report an error if leaks
}

void Test_M2MConnectionSecurity::test_reset()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    impl.reset();
}

void Test_M2MConnectionSecurity::test_init()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.init(NULL) );
}

void Test_M2MConnectionSecurity::test_connect()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.connect(NULL));
}

void Test_M2MConnectionSecurity::test_start_connecting_non_blocking()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.start_connecting_non_blocking(NULL));
}

void Test_M2MConnectionSecurity::test_continue_connecting()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.continue_connecting());
}

void Test_M2MConnectionSecurity::test_send_message()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    unsigned char msg[6] = "hello";
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.send_message(msg, 5) );
}

void Test_M2MConnectionSecurity::test_read()

{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    unsigned char msg[50];
    m2mconnectionsecuritypimpl_stub::int_value = 7;
    CHECK( 7 == impl.read(msg, 49));
}

void Test_M2MConnectionSecurity::test_set_random_number_callback()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    random_number_cb cb(&test_random_callback);
    impl.set_random_number_callback(cb);

}

void Test_M2MConnectionSecurity::test_set_entropy_callback()
{
    M2MConnectionSecurity impl = M2MConnectionSecurity(M2MConnectionSecurity::TLS);
    impl.set_entropy_callback(ent_cb);
}

uint32_t test_random_callback(void)
{
    return 1;
}
