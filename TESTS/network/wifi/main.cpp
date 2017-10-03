#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

// Test for parameters
#if !defined(MBED_CONF_APP_AP_MAC_SECURE)      || \
    !defined(MBED_CONF_APP_AP_MAC_UNSECURE)    || \
    !defined(MBED_CONF_APP_MAX_SCAN_SIZE)      || \
    !defined(MBED_CONF_APP_WIFI_CH_SECURE)     || \
    !defined(MBED_CONF_APP_WIFI_CH_UNSECURE)   || \
    !defined(MBED_CONF_APP_WIFI_DRIVER)        || \
    !defined(MBED_CONF_APP_WIFI_PASSWORD)      || \
    !defined(MBED_CONF_APP_WIFI_RX)            || \
    !defined(MBED_CONF_APP_WIFI_SECURE_SSID)   || \
    !defined(MBED_CONF_APP_WIFI_TX)            || \
    !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#endif

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(240, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("WIFI-CONSTRUCTOR", wifi_constructor),
    Case("WIFI-SET-CREDENTIAL", wifi_set_credential),
    Case("WIFI-SET-CHANNEL", wifi_set_channel),
    Case("WIFI-GET-RSSI", wifi_get_rssi),
    Case("WIFI-CONNECT-PARAMS-NULL", wifi_connect_params_null),
    Case("WIFI-CONNECT-PARAMS-VALID-UNSECURE", wifi_connect_params_valid_unsecure),
    Case("WIFI-CONNECT-PARAMS-VALID-SECURE", wifi_connect_params_valid_secure),
    Case("WIFI-CONNECT-PARAMS-CHANNEL", wifi_connect_params_channel),
    Case("WIFI-CONNECT-PARAMS-CHANNEL-FAIL", wifi_connect_params_channel_fail),
    Case("WIFI-CONNECT-NOCREDENTIALS", wifi_connect_nocredentials),
    Case("WIFI-CONNECT", wifi_connect),
    Case("WIFI-CONNECT-SECURE", wifi_connect_secure),
    Case("WIFI-CONNECT-SECURE-FAIL", wifi_connect_secure_fail),
    Case("WIFI-CONNECT-DISCONNECT-REPEAT", wifi_connect_disconnect_repeat),
    Case("WIFI-SCAN-NULL", wifi_scan_null),
    Case("WIFI-SCAN", wifi_scan),
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
    return !Harness::run(specification);
}
