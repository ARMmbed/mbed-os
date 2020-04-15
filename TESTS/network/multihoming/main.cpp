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
#else
#ifndef MBED_CONF_APP_ECHO_SERVER_ADDR
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#else

#if !defined(DEVICE_EMAC) || \
    (!defined(MBED_CONF_APP_WIFI_SECURE_SSID) && !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID))
#error [NOT_SUPPORTED] Both Wifi and Ethernet devices are required for multihoming tests.
#else

#define STRING_VERIFY(str) (str != NULL ? str : "not supported")


#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "multihoming_tests.h"
#include "LWIPStack.h"

using namespace utest::v1;

namespace {
EthInterface *eth;
WiFiInterface *wifi;
}

char interface_name[MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM][INTERFACE_NAME_LEN];
int  interface_num = 0;

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
mbed_stats_socket_t udp_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
#endif

#if defined(MBED_CONF_APP_WIFI_SECURE_SSID) || defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#define SSID_MAX_LEN 32
#define PWD_MAX_LEN 64

#endif

NetworkInterface *get_interface(int interface_index)
{
    if (interface_index == ETH_INTERFACE) {
        return eth;
    } else if (interface_index == WIFI_INTERFACE) {
        return wifi;
    }
    return NULL;
}

static void _ifup()
{
    eth = EthInterface::get_default_instance();
    nsapi_error_t err = eth->connect();
    eth->get_interface_name(interface_name[interface_num]);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    SocketAddress eth_ip_address;
    eth->get_ip_address(&eth_ip_address);
    printf("MBED: IP address is '%s' interface name %s\n", eth_ip_address.get_ip_address(), interface_name[interface_num]);
    SocketAddress eth_ip_address_if;
    LWIP::get_instance().get_ip_address_if(&eth_ip_address_if, interface_name[interface_num]);
    printf("IP_if: %s\n", eth_ip_address.get_ip_address());
    TEST_ASSERT(eth_ip_address_if == eth_ip_address);
    interface_num++;

    wifi = WiFiInterface::get_default_instance();

    if (wifi) {
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID)
        char ssid[SSID_MAX_LEN + 1] = MBED_CONF_APP_WIFI_SECURE_SSID;
        char pwd[PWD_MAX_LEN + 1] = MBED_CONF_APP_WIFI_PASSWORD;
        nsapi_security_t security = NSAPI_SECURITY_WPA_WPA2;

#elif defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
        char ssid[SSID_MAX_LEN + 1] = MBED_CONF_APP_WIFI_UNSECURE_SSID;
        char pwd[PWD_MAX_LEN + 1] = NULL;
        nsapi_security_t security = NSAPI_SECURITY_NONE;
#endif

        printf("\nConnecting to %s...\n", ssid);
        int ret = wifi->connect(ssid, pwd, security);
        if (ret != 0) {
            TEST_FAIL_MESSAGE("Wifi connection error!");
            return;
        }
        printf("Wifi interface name: %s\n\n", STRING_VERIFY(wifi->get_interface_name(interface_name[interface_num])));
        printf("MAC: %s\n", STRING_VERIFY(wifi->get_mac_address()));
        SocketAddress wifi_ip_address;
        wifi->get_ip_address(&wifi_ip_address);
        printf("IP: %s\n", STRING_VERIFY(wifi_ip_address.get_ip_address()));
        SocketAddress wifi_ip_address_if;
        LWIP::get_instance().get_ip_address_if(&wifi_ip_address_if, interface_name[interface_num]);
        printf("IP_if: %s\n", STRING_VERIFY(wifi_ip_address_if.get_ip_address()));
        TEST_ASSERT(wifi_ip_address_if == wifi_ip_address);
        SocketAddress wifi_netmask;
        wifi->get_netmask(&wifi_netmask);
        printf("Netmask: %s\n", STRING_VERIFY(wifi_netmask.get_ip_address()));
        SocketAddress wifi_gateway;
        wifi->get_gateway(&wifi_gateway);
        printf("Gateway: %s\n", STRING_VERIFY(wifi_gateway.get_ip_address()));
        printf("RSSI: %d\n\n", wifi->get_rssi());
        interface_num++;
    } else {
        TEST_FAIL_MESSAGE("ERROR: No WiFiInterface found!");
    }
}

static void _ifdown()
{
    interface_num = 0;
    if (eth != NULL) {
        eth->disconnect();
    }
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID) || defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    if (wifi != NULL) {
        wifi->disconnect();
    }
#endif
    printf("MBED: ifdown\n");
}

void fill_tx_buffer_ascii(char *buff, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
int fetch_stats()
{
    return SocketStats::mbed_stats_socket_get_each(&udp_stats[0], MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT);
}
#endif

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(480, "default_auto");
    _ifup();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
    Case("MULTIHOMING_SYNCHRONOUS_DNS", MULTIHOMING_SYNCHRONOUS_DNS),
    Case("MULTIHOMING_ASYNCHRONOUS_DNS", MULTIHOMING_ASYNCHRONOUS_DNS),
    Case("MULTIHOMING_UDPSOCKET_ECHOTEST", MULTIHOMING_UDPSOCKET_ECHOTEST),
    Case("MULTIHOMING_UDPSOCKET_ECHOTEST_NONBLOCK", MULTIHOMING_UDPSOCKET_ECHOTEST_NONBLOCK),
};

Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(DEVICE_EMAC) || (!defined(MBED_CONF_APP_WIFI_SECURE_SSID) && !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID))
#endif // MBED_CONF_APP_ECHO_SERVER_ADDR
#endif // !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
