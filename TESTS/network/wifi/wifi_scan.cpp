#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"
#include <stdbool.h>

using namespace utest::v1;

void wifi_scan(void)
{
    WiFiInterface *wifi = get_interface();

    WiFiAccessPoint ap[MBED_CONF_APP_MAX_SCAN_SIZE];

    int size = wifi->scan(ap, MBED_CONF_APP_MAX_SCAN_SIZE);
    TEST_ASSERT(size >= 2);

    bool secure_found = false;
    bool unsecure_found = false;

    char secure_bssid[6];
    char unsecure_bssid[6];
    const char *coversion_string = "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx";
    TEST_ASSERT_EQUAL_INT_MESSAGE(6, sscanf(MBED_CONF_APP_AP_MAC_SECURE, coversion_string, &secure_bssid[0], &secure_bssid[1], &secure_bssid[2], &secure_bssid[3], &secure_bssid[4], &secure_bssid[5]), "Failed to convert ap-mac-secure from mbed_app.json");
    TEST_ASSERT_EQUAL_INT_MESSAGE(6, sscanf(MBED_CONF_APP_AP_MAC_UNSECURE, coversion_string, &unsecure_bssid[0], &unsecure_bssid[1], &unsecure_bssid[2], &unsecure_bssid[3], &unsecure_bssid[4], &unsecure_bssid[5]), "Failed to convert ap-mac-unsecure from mbed_app.json");

    for (int i=0; i<size; i++) {
        const char *ssid = ap[i].get_ssid();
        const uint8_t *bssid = ap[i].get_bssid();
        nsapi_security_t security = ap[i].get_security();
        int8_t rssi = ap[i].get_rssi();
        uint8_t ch = ap[i].get_channel();
        TEST_ASSERT_NOT_EQUAL(0, strlen(ssid));
        TEST_ASSERT_INT8_WITHIN(-35, -65, rssi);
        if (strcmp(MBED_CONF_APP_WIFI_SECURE_SSID, ssid) == 0) {
            secure_found = true;
            TEST_ASSERT_EQUAL_INT(NSAPI_SECURITY_WPA2, security);
            TEST_ASSERT_EQUAL_INT(MBED_CONF_APP_WIFI_CH_SECURE, ch);
        }
        if (strcmp(MBED_CONF_APP_WIFI_UNSECURE_SSID, ssid) == 0) {
            unsecure_found = true;
            TEST_ASSERT_EQUAL_INT(NSAPI_SECURITY_NONE, security);
            TEST_ASSERT_EQUAL_INT(MBED_CONF_APP_WIFI_CH_UNSECURE, ch);
        }
    }
    TEST_ASSERT_TRUE(secure_found);
    TEST_ASSERT_TRUE(unsecure_found);
}

