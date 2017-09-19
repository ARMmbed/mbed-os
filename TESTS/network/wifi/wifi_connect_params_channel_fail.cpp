#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_connect_params_channel_fail(void)
{
    WiFiInterface *wifi = get_interface();

    if (wifi->set_channel(1) == NSAPI_ERROR_UNSUPPORTED && wifi->set_channel(36) == NSAPI_ERROR_UNSUPPORTED) {
        TEST_IGNORE_MESSAGE("set_channel() not supported");
        return;
    }

    nsapi_error_t error = wifi->connect(MBED_CONF_APP_WIFI_SECURE_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA2, MBED_CONF_APP_WIFI_CH_UNSECURE);
    TEST_ASSERT(error==NSAPI_ERROR_CONNECTION_TIMEOUT || error==NSAPI_ERROR_NO_CONNECTION);
}

