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
#include "test_m2mconnectionsecuritypimpl_mbedtls.h"
#include "m2mtimerobserver.h"
#include "m2msecurity.h"
#include "m2msecurity_stub.h"
#include "mbedtls_stub.h"
#include "mbed-client/m2mconnectionhandler.h"
#include "m2mtimer_stub.h"

uint32_t get_random_number(void)
{
    return time(NULL);
}

entropy_cb ent_cb;

int ent_poll( void *, unsigned char *output, size_t len,
                           size_t *olen )
{
    for(uint16_t i=0; i < len; i++){
        srand(time(NULL));
        output[i] = rand() % 256;
    }
    *olen = len;

    return( 0 );
}

class TestObserver : public M2MConnectionObserver {

public:
    TestObserver(){}
    void data_available(uint8_t*,
                        uint16_t,
                        const M2MConnectionObserver::SocketAddress &){}

    void socket_error(uint8_t error_code, bool retry = true){}

    void address_ready(const M2MConnectionObserver::SocketAddress &,
                       M2MConnectionObserver::ServerType,
                       const uint16_t){}

    void data_sent(){}
};

Test_M2MConnectionSecurityPimpl::Test_M2MConnectionSecurityPimpl()
{
    mbedtls_stub::clear();
    m2msecurity_stub::clear();
}

Test_M2MConnectionSecurityPimpl::~Test_M2MConnectionSecurityPimpl()
{
}

void Test_M2MConnectionSecurityPimpl::test_constructor()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    CHECK(impl._timer != NULL);
}

void Test_M2MConnectionSecurityPimpl::test_destructor()
{
    M2MConnectionSecurityPimpl* impl = new M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    delete impl;
    //Memory leak detector will report an error if leaks
}

void Test_M2MConnectionSecurityPimpl::test_reset()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    impl._init_done = true;
    impl.reset();
    CHECK(impl._init_done == false);
}

void Test_M2MConnectionSecurityPimpl::test_init()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    CHECK( -1 == impl.init(NULL) );

    m2msecurity_stub::int_value = 5;
    M2MSecurity* sec = new M2MSecurity(M2MSecurity::Bootstrap);
    CHECK( -1 == impl.init(sec) );

    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = true;
    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = -1;
    m2msecurity_stub::int_value = M2MSecurity::Psk;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = true;
    mbedtls_stub::expected_int = -1;
    mbedtls_stub::crt_expected_int = -1;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::crt_expected_int = 0;
    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    m2msecurity_stub::int_value = M2MSecurity::Psk;
    CHECK( 0 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = -1;
    mbedtls_stub::retArray[2] = -1;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    mbedtls_stub::retArray[3] = 0;
    mbedtls_stub::retArray[4] = -1;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    mbedtls_stub::retArray[3] = 0;
    mbedtls_stub::retArray[4] = 0;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( 0 == impl.init(sec) );

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    m2msecurity_stub::int_value = 99;
    CHECK( -1 == impl.init(sec) );

    impl.set_random_number_callback(NULL);

    ent_cb.entropy_source_ptr = ent_poll;
    ent_cb.p_source = NULL;
    ent_cb.threshold = 128;
    ent_cb.strong = 0;

    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = false;
    mbedtls_stub::useCounter = false;
    mbedtls_stub::expected_int = 0;
    mbedtls_stub::crt_expected_int = 0;
    mbedtls_stub::counter = 1;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    impl.set_entropy_callback(ent_cb);
    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = false;
    mbedtls_stub::useCounter = false;
    mbedtls_stub::expected_int = -1;
    mbedtls_stub::crt_expected_int = 0;
    mbedtls_stub::counter = 1;
    m2msecurity_stub::int_value = M2MSecurity::Certificate;
    CHECK( -1 == impl.init(sec) );

    delete sec;
    mbedtls_stub::clear();
}

void Test_M2MConnectionSecurityPimpl::test_connect()
{
    mbedtls_stub::invalidate_timer = true;
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    CHECK( -1 == impl.connect(NULL));

    impl._init_done = true;
    CHECK( -1 == impl.connect(NULL));

    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = true;

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    CHECK( -1 == impl.connect(NULL));

    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    CHECK( -1 == impl.connect(NULL));

    m2mtimer_stub::bool_value = true;

    TestObserver obs;
    M2MConnectionHandler* obj = new M2MConnectionHandler(obs, NULL, M2MInterface::NOT_SET,M2MInterface::LwIP_IPv4);

    mbedtls_stub::expected_uint32_value = 12;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = MBEDTLS_ERR_SSL_WANT_READ;
    mbedtls_stub::retArray[3] = MBEDTLS_ERR_SSL_WANT_WRITE;
    mbedtls_stub::retArray[4] = 0;
    CHECK( -1 == impl.connect(obj));

    delete obj;

    mbedtls_stub::counter = 0;
    mbedtls_stub::expected_uint32_value = 0;
    CHECK( 0 == impl.connect(NULL));

    //Tests mbedtls_timing_get_delay() for intermediate case
    mbedtls_stub::counter = 0;
    m2mtimer_stub::total_bool_value = false;
    m2mtimer_stub::bool_value = true;
    mbedtls_stub::expected_uint32_value = 1;

    CHECK( -1 == impl.connect(obj));

    //Tests mbedtls_timing_get_delay() for cancelled case
    mbedtls_stub::invalidate_timer = true;

    mbedtls_stub::counter = 0;
    m2mtimer_stub::total_bool_value = true;
    CHECK( -1 == impl.connect(obj));

    //Tests mbedtls_timing_get_delay() for NULL case
    mbedtls_stub::counter = 0;
    delete(impl._timer);
    impl._timer = NULL;
    CHECK( -1 == impl.connect(obj));
}

void Test_M2MConnectionSecurityPimpl::test_start_connecting_non_blocking()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    CHECK( -1 == impl.start_connecting_non_blocking(NULL));

    m2mtimer_stub::total_bool_value = false;
    impl._init_done = true;
    impl._sec_mode = M2MConnectionSecurity::TLS;
    CHECK( -1 == impl.start_connecting_non_blocking(NULL));

    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = true;

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    CHECK( -1 == impl.start_connecting_non_blocking(NULL));

    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    CHECK( -1 == impl.start_connecting_non_blocking(NULL));

    TestObserver obs;
    M2MConnectionHandler* obj = new M2MConnectionHandler(obs, NULL, M2MInterface::NOT_SET, M2MInterface::LwIP_IPv4);

    m2mtimer_stub::total_bool_value = true;
    mbedtls_stub::expected_uint32_value = 12;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[2] = 0;
    mbedtls_stub::retArray[3] = 0;
    mbedtls_stub::retArray[4] = 0;
    CHECK( 1 == impl.start_connecting_non_blocking(obj));

    delete obj;

    mbedtls_stub::counter = 0;
    mbedtls_stub::expected_uint32_value = 0;
    mbedtls_stub::retArray[3] = -1;
    CHECK( -1 == impl.start_connecting_non_blocking(NULL));
}

void Test_M2MConnectionSecurityPimpl::test_continue_connecting()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    impl._ssl.state = MBEDTLS_SSL_HELLO_REQUEST;
    mbedtls_stub::expected_int = MBEDTLS_ERR_SSL_WANT_READ;
    CHECK( M2MConnectionHandler::CONNECTION_ERROR_WANTS_READ == impl.continue_connecting());

    mbedtls_stub::expected_int = MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE_REQUEST;
    impl._ssl.state = MBEDTLS_SSL_CLIENT_HELLO;
    CHECK( MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE_REQUEST == impl.continue_connecting());

    mbedtls_stub::expected_int = -6;
    impl._ssl.state = MBEDTLS_SSL_HANDSHAKE_OVER;
    CHECK( -6 == impl.continue_connecting());

    mbedtls_stub::expected_int = 0;
    impl._ssl.state = MBEDTLS_SSL_HANDSHAKE_OVER;
    CHECK( 0 == impl.continue_connecting());
}

void Test_M2MConnectionSecurityPimpl::test_send_message()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    unsigned char msg[6] = "hello";
    CHECK( -1 == impl.send_message(msg, 5) );
    impl._init_done = true;

    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = MBEDTLS_ERR_SSL_WANT_READ;
    mbedtls_stub::retArray[1] = MBEDTLS_ERR_SSL_WANT_WRITE;
    mbedtls_stub::retArray[2] = 65;
    CHECK( 65 == impl.send_message(msg, 5) );
}

void Test_M2MConnectionSecurityPimpl::test_read()

{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    unsigned char msg[50];
    CHECK( -1 == impl.read(msg, 49));

    impl._init_done = true;
    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = MBEDTLS_ERR_SSL_WANT_READ;
    mbedtls_stub::retArray[1] = MBEDTLS_ERR_SSL_WANT_WRITE;
    mbedtls_stub::retArray[2] = 25;
    CHECK( 25 == impl.read(msg, 49));
}

void Test_M2MConnectionSecurityPimpl::test_timer_expired()
{
    //This uses static function so we need to prepare test using connect() function.
    mbedtls_stub::invalidate_timer = false;
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    impl._init_done = true;
    m2msecurity_stub::int_value = 5;
    m2msecurity_stub::has_value = true;
    mbedtls_stub::useCounter = true;
    mbedtls_stub::counter = 0;
    mbedtls_stub::retArray[0] = 0;
    mbedtls_stub::retArray[1] = 0;
    mbedtls_stub::retArray[3] = M2MConnectionHandler::CONNECTION_ERROR_WANTS_READ;
    impl.connect(NULL);

    void *bio = malloc(1);
    impl._ssl.p_bio = bio;
    impl.timer_expired(M2MTimerObserver::Dtls);

    // For testing blocking mode use case
    mbedtls_stub::useCounter = false;
    mbedtls_stub::expected_int = MBEDTLS_ERR_SSL_TIMEOUT;

    impl.timer_expired(M2MTimerObserver::Dtls);
    free(bio);
}

void Test_M2MConnectionSecurityPimpl::test_set_random_number_callback()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    random_number_cb cb(&test_random_callback);
    impl.set_random_number_callback(cb);
}

void Test_M2MConnectionSecurityPimpl::test_set_entropy_callback()
{
    M2MConnectionSecurityPimpl impl = M2MConnectionSecurityPimpl(M2MConnectionSecurity::TLS);
    impl.set_entropy_callback(ent_cb);
}

uint32_t test_random_callback(void)
{
    return 1;
}
