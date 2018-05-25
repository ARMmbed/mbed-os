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

#ifndef MBED_CONF_APP_OBJECT_CONSTRUCTION
    #error [NOT_SUPPORTED] No network interface found for this target.
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

// Test for parameters
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID)
#if !defined(MBED_CONF_APP_AP_MAC_SECURE)      || \
    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
    !defined(MBED_CONF_APP_WIFI_CH_SECURE)     || \
    !defined(MBED_CONF_APP_WIFI_PASSWORD)      || \
    !defined(MBED_CONF_APP_WIFI_SECURE_SSID)   || \
    !defined MBED_CONF_APP_WIFI_SECURE_PROTOCOL
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json (for secure connections)
#endif
#endif // defined(MBED_CONF_APP_WIFI_SECURE_SSID)

#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#if !defined(MBED_CONF_APP_AP_MAC_UNSECURE)    || \
    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
    !defined(MBED_CONF_APP_WIFI_CH_UNSECURE)   || \
    !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json (for unsecure connections)
#endif
#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(240, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("WIFI-CONSTRUCTOR", wifi_constructor),
    Case("WIFI-CONNECT-NOCREDENTIALS", wifi_connect_nocredentials),
    Case("WIFI-SET-CREDENTIAL", wifi_set_credential),
    Case("WIFI-SET-CHANNEL", wifi_set_channel),
#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-GET-RSSI", wifi_get_rssi),
#endif
    Case("WIFI-CONNECT-PARAMS-NULL", wifi_connect_params_null),
#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-CONNECT-PARAMS-VALID-UNSECURE", wifi_connect_params_valid_unsecure),
#endif
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID)
    Case("WIFI-CONNECT-PARAMS-VALID-SECURE", wifi_connect_params_valid_secure),
    Case("WIFI-CONNECT-PARAMS-CHANNEL", wifi_connect_params_channel),
    Case("WIFI-CONNECT-PARAMS-CHANNEL-FAIL", wifi_connect_params_channel_fail),
#endif
#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-CONNECT", wifi_connect),
#endif
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID)
    Case("WIFI-CONNECT-SECURE", wifi_connect_secure),
    Case("WIFI-CONNECT-SECURE-FAIL", wifi_connect_secure_fail),
#endif
#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-CONNECT-DISCONNECT-REPEAT", wifi_connect_disconnect_repeat),
#endif
    Case("WIFI-SCAN-NULL", wifi_scan_null),
#if defined(MBED_CONF_APP_WIFI_SECURE_SSID) && defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    Case("WIFI-SCAN", wifi_scan),
#endif
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
    return !Harness::run(specification);
}
