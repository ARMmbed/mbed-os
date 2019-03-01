/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif
#ifndef MBED_CONF_APP_ECHO_SERVER_ADDR
#error [NOT_SUPPORTED] Requires echo-server-discard-port parameter from mbed_app.json
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "tls_tests.h"

#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

using namespace utest::v1;

namespace {
Timer tc_bucket; // Timer to limit a test cases run time
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
mbed_stats_socket_t tls_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
#endif

char tls_global::rx_buffer[RX_BUFF_SIZE];
char tls_global::tx_buffer[TX_BUFF_SIZE];

const char *tls_global::cert = \
                               "-----BEGIN CERTIFICATE-----\n"
                               "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n"
                               "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
                               "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n"
                               "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n"
                               "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n"
                               "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n"
                               "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n"
                               "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n"
                               "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n"
                               "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n"
                               "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n"
                               "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n"
                               "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n"
                               "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n"
                               "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n"
                               "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n"
                               "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n"
                               "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n"
                               "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n"
                               "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n"
                               "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n"
                               "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n"
                               "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n"
                               "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n"
                               "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n"
                               "-----END CERTIFICATE-----\n";

void drop_bad_packets(TLSSocket &sock, int orig_timeout)
{
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recv(NULL, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(orig_timeout);
}

static void _ifup()
{
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: TLSClient IP address is '%s'\n", net->get_ip_address());
}

static void _ifdown()
{
    NetworkInterface::get_default_instance()->disconnect();
    printf("MBED: ifdown\n");
}

nsapi_error_t tlssocket_connect_to_srv(TLSSocket &sock, uint16_t port)
{
    SocketAddress tls_addr;

    NetworkInterface::get_default_instance()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &tls_addr);
    tls_addr.set_port(port);

    printf("MBED: Server '%s', port %d\n", tls_addr.get_ip_address(), tls_addr.get_port());

    nsapi_error_t err = sock.set_root_ca_cert(tls_global::cert);
    if (err != NSAPI_ERROR_OK) {
        printf("Error from sock.set_root_ca_cert: %d\n", err);
        return err;
    }

    err = sock.open(NetworkInterface::get_default_instance());
    if (err != NSAPI_ERROR_OK) {
        printf("Error from sock.open: %d\n", err);
        return err;
    }

    err = sock.connect(tls_addr);
    if (err != NSAPI_ERROR_OK) {
        printf("Error from sock.connect: %d\n", err);
        return err;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t tlssocket_connect_to_echo_srv(TLSSocket &sock)
{
    return tlssocket_connect_to_srv(sock, MBED_CONF_APP_ECHO_SERVER_PORT_TLS);
}

nsapi_error_t tlssocket_connect_to_discard_srv(TLSSocket &sock)
{
    return tlssocket_connect_to_srv(sock, MBED_CONF_APP_ECHO_SERVER_DISCARD_PORT_TLS);
}

void fill_tx_buffer_ascii(char *buff, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

int split2half_rmng_tls_test_time()
{
    return (tls_global::TESTS_TIMEOUT - tc_bucket.read()) / 2;
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
int fetch_stats()
{
    return SocketStats::mbed_stats_socket_get_each(&tls_stats[0], MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT);
}
#endif

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(tls_global::TESTS_TIMEOUT, "default_auto");
    _ifup();
    tc_bucket.start();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    tc_bucket.stop();
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}


Case cases[] = {
    Case("TLSSOCKET_ECHOTEST", TLSSOCKET_ECHOTEST),
    Case("TLSSOCKET_ECHOTEST_NONBLOCK", TLSSOCKET_ECHOTEST_NONBLOCK),
    Case("TLSSOCKET_CONNECT_INVALID", TLSSOCKET_CONNECT_INVALID),
    Case("TLSSOCKET_ECHOTEST_BURST", TLSSOCKET_ECHOTEST_BURST),
    Case("TLSSOCKET_ECHOTEST_BURST_NONBLOCK", TLSSOCKET_ECHOTEST_BURST_NONBLOCK),
    Case("TLSSOCKET_RECV_TIMEOUT", TLSSOCKET_RECV_TIMEOUT),
    Case("TLSSOCKET_ENDPOINT_CLOSE", TLSSOCKET_ENDPOINT_CLOSE),
    Case("TLSSOCKET_HANDSHAKE_INVALID", TLSSOCKET_HANDSHAKE_INVALID),
    Case("TLSSOCKET_OPEN_TWICE", TLSSOCKET_OPEN_TWICE),
    Case("TLSSOCKET_OPEN_LIMIT", TLSSOCKET_OPEN_LIMIT),
    Case("TLSSOCKET_OPEN_DESTRUCT", TLSSOCKET_OPEN_DESTRUCT),
    Case("TLSSOCKET_SEND_UNCONNECTED", TLSSOCKET_SEND_UNCONNECTED),
    Case("TLSSOCKET_SEND_CLOSED", TLSSOCKET_SEND_CLOSED),
    Case("TLSSOCKET_SEND_REPEAT", TLSSOCKET_SEND_REPEAT),
    Case("TLSSOCKET_SEND_TIMEOUT", TLSSOCKET_SEND_TIMEOUT),
    Case("TLSSOCKET_NO_CERT", TLSSOCKET_NO_CERT),
//    Temporarily removing this test, as TLS library consumes too much memory
//    and we see frequent memory allocation failures on architectures with less
//    RAM such as DISCO_L475VG_IOT1A and NUCLEO_F207ZG (both have 128 kB RAM)
//    This test also fails for IAR, due to wrong heap configuration in the linker
//    script - see https://github.com/ARMmbed/mbed-os/issues/8306
//    Case("TLSSOCKET_SIMULTANEOUS", TLSSOCKET_SIMULTANEOUS)
};

Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

int retval;
void run_test(void)
{
    retval = !Harness::run(specification);
}

static unsigned char stack_mem[8192];
int main()
{
    Thread *th = new Thread(osPriorityNormal, 8192, stack_mem, "tls_gt_thread");
    th->start(callback(run_test));
    th->join();
    return retval;
}

#else
#error [NOT_SUPPORTED] This device does not support SSL library
#endif // defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)
