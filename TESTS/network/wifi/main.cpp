/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
#else
#error [NOT_SUPPORTED] Only built in WiFi modules are supported at this time.
#endif

using namespace utest::v1;

/**
 * WiFi tests require following macros to be defined:
 * - MBED_CONF_APP_WIFI_SSID - SSID of a network the test will try connecting to
 * - MBED_CONF_APP_WIFI_PASSWORD - Passphrase that will be used to connecting to the network
 * - WIFI_TEST_NETWORKS - List of network that presence will be asserted e.g. "net1", "net2", "net3"
 */
#if !defined(MBED_CONF_APP_WIFI_SSID) || !defined(MBED_CONF_APP_WIFI_PASSWORD) || !defined(MBED_CONF_APP_WIFI_NETWORKS)
#error [NOT_SUPPORTED] MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD and MBED_CONF_APP_WIFI_NETWORKS have to be defined for this test.
#endif

const char *networks[] = {MBED_CONF_APP_WIFI_NETWORKS, NULL};

WiFiInterface *wifi;

/* In normal circumstances the WiFi object could be global, but the delay introduced by WiFi initialization is an issue
   for the tests. It causes Greentea to timeout on syncing with the board. To solve it we defer the actual object
   creation till we actually need it.
 */
WiFiInterface *get_wifi()
{
    if (wifi == NULL) {
        /* We don't really care about freeing this, as its lifetime is through the full test suit run. */
#if TARGET_UBLOX_EVK_ODIN_W2
        wifi = new OdinWiFiInterface;
#endif
    }

    return wifi;
}

void check_wifi(const char *ssid, bool *net_stat)
{
    int i = 0;
    while(networks[i]) {
        if (strcmp(networks[i], ssid) == 0) {
            net_stat[i] = true;
            break;
        }
        i++;
    }
}

void wifi_scan()
{
    int count;
    WiFiAccessPoint *aps;
    const int net_len = sizeof(networks)/sizeof(networks[0]);
    bool net_stat[net_len - 1];

    memset(net_stat, 0, sizeof(net_stat));

    count = get_wifi()->scan(NULL, 0);
    TEST_ASSERT_MESSAGE(count >= 0, "WiFi interface returned error");
    TEST_ASSERT_MESSAGE(count > 0, "Scan result empty");

    aps = new WiFiAccessPoint[count];
    count = get_wifi()->scan(aps, count);
    for(int i = 0; i < count; i++) {
        check_wifi(aps[i].get_ssid(), net_stat);
    }

    delete[] aps;

    for (unsigned i = 0; i < sizeof(net_stat); i++) {
        TEST_ASSERT_MESSAGE(net_stat[i] == true, "Not all required WiFi network detected");
    }
}

void wifi_connect()
{
    int ret;

    ret = get_wifi()->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT_MESSAGE(ret == 0, "Connect failed");

    ret = get_wifi()->disconnect();
    TEST_ASSERT_MESSAGE(ret == 0, "Disconnect failed");
}

void wifi_connect_scan()
{
    int ret;
    int count;
    WiFiAccessPoint *aps;
    const int net_len = sizeof(networks)/sizeof(networks[0]);
    bool net_stat[net_len - 1];

    memset(net_stat, 0, sizeof(net_stat));

    ret = get_wifi()->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT_MESSAGE(ret == 0, "Connect failed");

    count = get_wifi()->scan(NULL, 0);
    TEST_ASSERT_MESSAGE(count >= 0, "WiFi interface returned error");
    TEST_ASSERT_MESSAGE(count > 0, "Scan result empty");

    aps = new WiFiAccessPoint[count];
    count = get_wifi()->scan(aps, count);
    for(int i = 0; i < count; i++) {
        check_wifi(aps[i].get_ssid(), net_stat);
    }

    delete[] aps;

    ret = get_wifi()->disconnect();
    TEST_ASSERT_MESSAGE(ret == 0, "Disconnect failed");

    for (unsigned i = 0; i < sizeof(net_stat); i++) {
        TEST_ASSERT_MESSAGE(net_stat[i] == true, "Not all required WiFi network detected");
    }
}

void wifi_http()
{
    TCPSocket socket;
    int ret;

    ret = get_wifi()->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT_MESSAGE(ret == 0, "Connect failed");

    // Open a socket on the network interface, and create a TCP connection to www.arm.com
    ret = socket.open(get_wifi());
    TEST_ASSERT_MESSAGE(ret == 0, "Socket open failed");
    ret = socket.connect("www.arm.com", 80);
    TEST_ASSERT_MESSAGE(ret == 0, "Socket connect failed");

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: www.arm.com\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    TEST_ASSERT_MESSAGE(scount >= 0, "Socket send failed");

    // Recieve a simple http response and check if it's not empty
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    TEST_ASSERT_MESSAGE(rcount >= 0, "Socket recv error");
    TEST_ASSERT_MESSAGE(rcount > 0, "No data received");

    ret = socket.close();
    TEST_ASSERT_MESSAGE(ret == 0, "Socket close failed");

    ret = get_wifi()->disconnect();
    TEST_ASSERT_MESSAGE(ret == 0, "Disconnect failed");
}

status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Scan test", wifi_scan, greentea_failure_handler),
    Case("Connect test", wifi_connect, greentea_failure_handler),
    Case("Scan while connected test", wifi_connect_scan, greentea_failure_handler),
    Case("HTTP test", wifi_http, greentea_failure_handler),
};

status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(90, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}


int main() {
    Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    Harness::run(specification);
}
