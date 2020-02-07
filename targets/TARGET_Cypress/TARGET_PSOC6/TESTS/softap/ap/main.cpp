/*
 * Copyright (c) 2017-2019, ARM Limited, All Rights Reserved
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

#if !defined(MBED_CONF_APP_SOFTAP_SSID)       || \
    !defined(MBED_CONF_APP_SOFTAP_CHANNEL)    || \
    !defined(MBED_CONF_APP_SOFTAP_PASSWORD)

#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#endif

#define SOFTAP 1

#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != SOFTAP

#error [NOT_SUPPORTED] SoftAP testing need to be enabled
#endif

#if !defined(TARGET_PSOC6)
#error [NOT_SUPPORTED] Wifi tests are not valid for the target
#endif

#define SOFTAP_IP_ADDRESS     "192.168.10.1"
#define NETMASK               "255.255.255.0"
#define GATEWAY               "192.168.10.1"

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "WhdSoftAPInterface.h"

using namespace utest::v1;

static char _key[256] = { 0 };
static char _value[128] = { 0 };

void host_os_type_verification() {
    greentea_send_kv("get_os_type", "host");
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("Linux", _value);
}

void softap_test() {
    WhdSoftAPInterface *softap = WhdSoftAPInterface::get_default_instance();
    TEST_ASSERT_TRUE(softap != NULL);

    softap->set_network(SOFTAP_IP_ADDRESS, NETMASK, GATEWAY);
    int ret = softap->start(MBED_CONF_APP_SOFTAP_SSID, MBED_CONF_APP_SOFTAP_PASSWORD, NSAPI_SECURITY_WPA_WPA2,
              MBED_CONF_APP_SOFTAP_CHANNEL);
    TEST_ASSERT_EQUAL_INT(0, ret);
    greentea_send_kv("softap", "up");
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);
}

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
   // here, we specify the timeout (150) and the host runner (the name of our Python file)
   GREENTEA_SETUP(150, "softap_basic");
   return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
   Case("Host OS Type Verification", host_os_type_verification),
   Case("SoftAP test", softap_test)
};

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
   return Harness::run(specification);
}
