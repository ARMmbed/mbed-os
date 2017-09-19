#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_connect_params_valid_secure(void)
{
    WiFiInterface *wifi = get_interface();
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect(MBED_CONF_APP_WIFI_SECURE_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA2));
}
