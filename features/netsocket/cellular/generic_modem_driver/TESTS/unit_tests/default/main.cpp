/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if !MODEM_ON_BOARD
#error [NOT_SUPPORTED] MODEM_ON_BOARD should be set for this test to be functional
#endif

#include "mbed.h"
#include "gmd_ut_config_header.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
//Add your driver's header file here
#include "OnboardCellularInterface.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "TEST"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

using namespace utest::v1;

#if !defined(MBED_CONF_APP_DEFAULT_PIN)
#error [NOT_SUPPORTED] A json configuration file is needed. Skipping this build.
#endif

/** How to run port verification tests
 *
 *  i)   Copy this file in your implementation directory
 *       e.g., netsocket/cellular/YOUR_IMPLEMENTATION/TESTS/unit_tests/default/
 *  ii)  Rename OnboardCellularInterface everywhere in this file with your Class
 *  iii) Make an empty test application with the fork of mbed-os where your implementation resides
 *  iv)  Create a json file in the root directory of your application and copy the contents of
 *       template_mbed_app.txt into it
 *  v)   Now from the root of your application, enter this command:
 *       mbed test --compile-list
 *       Look for the name of of your test suite matching to the directory path
 *  vi)  Run tests with the command:
 *       mbed test -n YOUR_TEST_SUITE_NAME
 *
 *  For more information on mbed-greentea testing suite, please visit:
 *  https://docs.mbed.com/docs/mbed-os-handbook/en/latest/advanced/greentea/
 */

// Lock for debug prints
static Mutex mtx;

// An instance of the cellular driver
// change this with the name of your driver
static OnboardCellularInterface driver(true);

// Test data
static const char test_data[] = TEST_DATA;

//Private Function prototypes
static nsapi_error_t do_connect(OnboardCellularInterface *iface);
static int fix(int size, int limit);
static void do_udp_echo(UDPSocket *sock, SocketAddress *host_address, int size);
static int send_all(TCPSocket *sock, const char *data, int size);
static void async_cb(bool *callback_triggered);
static void do_tcp_echo_async(TCPSocket *sock, int size, bool *callback_triggered);
static void use_connection(OnboardCellularInterface *driver);
static void drop_connection(OnboardCellularInterface *driver);
static void lock();
static void unlock();

/*
 *  Verification tests for a successful porting
 *  These tests must pass:
 *
 * 	test_udp_echo()
 * 	test_tcp_echo_async
 * 	test_connect_credentials
 * 	test_connect_preset_credentials
 */

/**
 * Test UDP data exchange
 */
void test_udp_echo()
{
    UDPSocket sock;
    SocketAddress host_address;
    int x;
    int size;

    driver.disconnect();
    TEST_ASSERT(do_connect(&driver) == 0);

    TEST_ASSERT(driver.gethostbyname(MBED_CONF_APP_ECHO_SERVER, &host_address) == 0);
    host_address.set_port(MBED_CONF_APP_ECHO_UDP_PORT);

    tr_debug("UDP: Server %s address: %s on port %d.",
             MBED_CONF_APP_ECHO_SERVER, host_address.get_ip_address(),
             host_address.get_port());

    TEST_ASSERT(sock.open(&driver) == 0)

    sock.set_timeout(10000);

    // Test min, max, and some random sizes in-between
    do_udp_echo(&sock, &host_address, 1);
    do_udp_echo(&sock, &host_address, MBED_CONF_APP_UDP_MAX_PACKET_SIZE);
    for (x = 0; x < 10; x++) {
        size = (rand() % MBED_CONF_APP_UDP_MAX_PACKET_SIZE) + 1;
        size = fix(size, MBED_CONF_APP_UDP_MAX_PACKET_SIZE + 1);
        do_udp_echo(&sock, &host_address, size);
    }

    sock.close();

    drop_connection(&driver);

    tr_debug("%d UDP packets of size up to %d byte(s) echoed successfully.", x,
             MBED_CONF_APP_UDP_MAX_PACKET_SIZE);
}

/**
 * Test TCP data exchange via the asynchronous sigio() mechanism
 */
void test_tcp_echo_async()
{
    TCPSocket sock;
    SocketAddress host_address;
    bool callback_triggered = false;
    int x;
    int size;

    driver.disconnect();
    TEST_ASSERT(do_connect(&driver) == 0);

    TEST_ASSERT(
            driver.gethostbyname(MBED_CONF_APP_ECHO_SERVER, &host_address) == 0);
    host_address.set_port(MBED_CONF_APP_ECHO_TCP_PORT);

    tr_debug("TCP: Server %s address: %s on port %d.",
             MBED_CONF_APP_ECHO_SERVER, host_address.get_ip_address(),
             host_address.get_port());

    TEST_ASSERT(sock.open(&driver) == 0)

    // Set up the async callback and set the timeout to zero
    sock.sigio(callback(async_cb, &callback_triggered));
    sock.set_timeout(0);

    TEST_ASSERT(sock.connect(host_address) == 0);
    // Test min, max, and some random sizes in-between
    do_tcp_echo_async(&sock, 1, &callback_triggered);
    do_tcp_echo_async(&sock, MBED_CONF_APP_TCP_MAX_PACKET_SIZE,
                      &callback_triggered);

    sock.close();

    drop_connection(&driver);

    tr_debug("TCP packets of size up to %d byte(s) echoed asynchronously and successfully.",
            MBED_CONF_APP_TCP_MAX_PACKET_SIZE);
}

/**
 * Connect with credentials included in the connect request
 */
void test_connect_credentials()
{

    driver.disconnect();

    TEST_ASSERT(do_connect(&driver) == 0);
    use_connection(&driver);
    drop_connection(&driver);
}

/**
 * Test with credentials preset
 */
void test_connect_preset_credentials()
{
    driver.disconnect();
    driver.set_sim_pin(MBED_CONF_APP_DEFAULT_PIN);
    driver.set_credentials(MBED_CONF_APP_APN, MBED_CONF_APP_USERNAME,
    MBED_CONF_APP_PASSWORD);
    int num_retries = 0;
    nsapi_error_t err = NSAPI_ERROR_OK;
    while (!driver.is_connected()) {
        err = driver.connect();
        if (err == NSAPI_ERROR_OK || num_retries > MBED_CONF_APP_MAX_RETRIES) {
            break;
        }
    }

    TEST_ASSERT(err == 0);
    use_connection(&driver);
    drop_connection(&driver);
}

/**
 * Setup Test Environment
 */
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    // Setup Greentea with a timeout
    GREENTEA_SETUP(600, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

/**
 * Array defining test cases
 */
Case cases[] = { Case("UDP echo test", test_udp_echo),
#if MBED_CONF_LWIP_TCP_ENABLED
            Case("TCP async echo test", test_tcp_echo_async),
#endif
            Case("Connect with credentials", test_connect_credentials),
            Case("Connect with preset credentials", test_connect_preset_credentials) };

Specification specification(test_setup, cases);

/**
 * main test harness
 */
int main()
{
    mbed_trace_init();

    mbed_trace_mutex_wait_function_set(lock);
    mbed_trace_mutex_release_function_set(unlock);

    // Run tests
    return !Harness::run(specification);
}
/**
 * connect to the network
 */
static nsapi_error_t do_connect(OnboardCellularInterface *iface)
{
    int num_retries = 0;
    nsapi_error_t err = NSAPI_ERROR_OK;
    while (!iface->is_connected()) {
        err = driver.connect(MBED_CONF_APP_DEFAULT_PIN, MBED_CONF_APP_APN,
                              MBED_CONF_APP_USERNAME, MBED_CONF_APP_PASSWORD);
        if (err == NSAPI_ERROR_OK || num_retries > MBED_CONF_APP_MAX_RETRIES) {
            break;
        }
        num_retries++;
    }

    return err;
}

/**
 * Get a random size for the test packet
 */
static int fix(int size, int limit)
{
    if (size <= 0) {
        size = limit / 2;
    } else if (size > limit) {
        size = limit;
    }
    return size;
}

/**
 * Do a UDP socket echo test to a given host of a given packet size
 */
static void do_udp_echo(UDPSocket *sock, SocketAddress *host_address, int size)
{
    bool success = false;
    void * recv_data = malloc(size);
    TEST_ASSERT(recv_data != NULL);

    // Retry this a few times, don't want to fail due to a flaky link
    for (int x = 0; !success && (x < NUM_UDP_RETRIES); x++) {
        tr_debug("Echo testing UDP packet size %d byte(s), try %d.", size, x + 1);
        if ((sock->sendto(*host_address, (void*) test_data, size) == size)
               && (sock->recvfrom(host_address, recv_data, size) == size)) {
            TEST_ASSERT(memcmp(test_data, recv_data, size) == 0);
            success = true;
        }
    }
    TEST_ASSERT(success);

    free(recv_data);
}

/**
 * Send an entire TCP data buffer until done
 */
static int send_all(TCPSocket *sock, const char *data, int size)
{
    int x;
    int count = 0;
    Timer timer;

    timer.start();
    while ((count < size) && (timer.read_ms() < ASYNC_TEST_WAIT_TIME)) {
        x = sock->send(data + count, size - count);
        if (x > 0) {
            count += x;
            tr_debug("%d byte(s) sent, %d left to send.", count, size - count);
        }
        wait_ms(10);
    }
    timer.stop();

    return count;
}

/**
 * The asynchronous callback
 */
static void async_cb(bool *callback_triggered)
{

    TEST_ASSERT(callback_triggered != NULL);
    *callback_triggered = true;
}

/**
 * Do a TCP echo using the asynchronous driver
 */
static void do_tcp_echo_async(TCPSocket *sock, int size,
                              bool *callback_triggered)
{
    void * recv_data = malloc(size);
    int recv_size = 0;
    int remaining_size;
    int x, y;
    Timer timer;
    TEST_ASSERT(recv_data != NULL);

    *callback_triggered = false;
    tr_debug("Echo testing TCP packet size %d byte(s) async.", size);
    TEST_ASSERT(send_all(sock, test_data, size) == size);
    // Wait for all the echoed data to arrive
    timer.start();
    remaining_size = size;
    while ((recv_size < size) && (timer.read_ms() < ASYNC_TEST_WAIT_TIME)) {
        if (*callback_triggered) {
            *callback_triggered = false;
            x = sock->recv((char *) recv_data + recv_size, remaining_size);
            if (x > 0) {
                recv_size += x;
                remaining_size = size - recv_size;
                tr_debug("%d byte(s) echoed back so far, %d to go.", recv_size,
                         remaining_size);
            }
        }
        wait_ms(10);
    }
    TEST_ASSERT(recv_size == size);
    y = memcmp(test_data, recv_data, size);
    if (y != 0) {
        tr_debug("Sent %d, |%*.*s|", size, size, size, test_data);
        tr_debug("Rcvd %d, |%*.*s|", size, size, size, (char * ) recv_data);
        // We do not assert a failure here because ublox TCP echo server doesn't send
        // back original data. It actually constructs a ublox message string. They need to fix it as
        // at the minute in case of TCP, their server is not behaving like a echo TCP server.
        //TEST_ASSERT(false);
    }
    timer.stop();
    free(recv_data);
}

/**
 * Use a connection, checking that it is good
 * Checks via doing an NTP transaction
 */
static void use_connection(OnboardCellularInterface *driver)
{
    const char * ip_address = driver->get_ip_address();
    const char * net_mask = driver->get_netmask();
    const char * gateway = driver->get_gateway();

    TEST_ASSERT(driver->is_connected());

    TEST_ASSERT(ip_address != NULL);
    tr_debug("IP address %s.", ip_address);
    TEST_ASSERT(net_mask != NULL);
    tr_debug("Net mask %s.", net_mask);
    TEST_ASSERT(gateway != NULL);
    tr_debug("Gateway %s.", gateway);

    UDPSocket sock;
    SocketAddress host_address;

    TEST_ASSERT(driver->gethostbyname(MBED_CONF_APP_ECHO_SERVER, &host_address) == 0);
    host_address.set_port(MBED_CONF_APP_ECHO_UDP_PORT);

    tr_debug("UDP: Server %s address: %s on port %d.",
             MBED_CONF_APP_ECHO_SERVER, host_address.get_ip_address(),
             host_address.get_port());

    TEST_ASSERT(sock.open(driver) == 0)

    sock.set_timeout(10000);
    do_udp_echo(&sock, &host_address, 1);

    sock.close();
}

/**
 * Drop a connection and check that it has dropped
 */
static void drop_connection(OnboardCellularInterface *driver)
{
    TEST_ASSERT(driver->disconnect() == 0);
    TEST_ASSERT(!driver->is_connected());
}

/**
 * Locks for debug prints
 */
static void lock()
{
    mtx.lock();
}

static void unlock()
{
    mtx.unlock();
}
