/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
    MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != WIFI
#error [NOT_SUPPORTED] No network configuration found for this target.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

// Test for parameters
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID) &&    \
   (!defined(MBED_CONF_APP_AP_MAC_SECURE)      || \
    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
    !defined(MBED_CONF_APP_WIFI_CH_SECURE)     || \
    !defined(MBED_CONF_APP_WIFI_PASSWORD)      || \
    !defined(MBED_CONF_APP_WIFI_SECURE_SSID)   || \
    !defined(MBED_CONF_APP_WIFI_SECURE_PROTOCOL))
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json (for secure connections)
#else

#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID) &&  \
   (!defined(MBED_CONF_APP_AP_MAC_UNSECURE)    || \
    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
    !defined(MBED_CONF_APP_WIFI_CH_UNSECURE)   || \
    !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID))
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json (for unsecure connections)
#else


using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_WIFI_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_WIFI_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(360, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("WIFI-CONSTRUCTOR", wifi_constructor),
    Case("WIFI-CONNECT-NOCREDENTIALS", wifi_connect_nocredentials),
    Case("WIFI-SET-CREDENTIAL", wifi_set_credential),
    Case("WIFI-SET-CHANNEL", wifi_set_channel),
    Case("WIFI-CONNECT-PARAMS-NULL", wifi_connect_params_null),
    Case("WIFI-SCAN-NULL", wifi_scan_null),
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID) || defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-SCAN", wifi_scan),
#endif
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID)
    Case("WIFI-GET-RSSI", wifi_get_rssi),
    Case("WIFI-CONNECT-DISCONNECT-REPEAT", wifi_connect_disconnect_repeat),
    Case("WIFI-CONNECT-PARAMS-VALID-SECURE", wifi_connect_params_valid_secure),
    Case("WIFI-CONNECT-PARAMS-CHANNEL", wifi_connect_params_channel),
    Case("WIFI-CONNECT-PARAMS-CHANNEL-FAIL", wifi_connect_params_channel_fail),
    Case("WIFI-CONNECT-SECURE", wifi_connect_secure),
    Case("WIFI-CONNECT-SECURE-FAIL", wifi_connect_secure_fail),
#endif
#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-CONNECT", wifi_connect),
    //Most boards are not passing this test, but they should if they support non-blocking API.
    //Case("WIFI_CONNECT_DISCONNECT_NONBLOCK", wifi_connect_disconnect_nonblock),
#endif
};

Specification specification(test_setup, cases, greentea_continue_handlers);

// Entry point into the tests
int main()
{
    return !Harness::run(specification);
}
#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID) && !defined(MBED_CONF_APP_*

#endif // defined(MBED_CONF_APP_WIFI_SECURE_SSID) && (!defined(MBED_CONF_APP_*

#endif //!defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != WIFI
