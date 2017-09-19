#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_get_rssi(void)
{
    WiFiInterface *wifi = get_interface();

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(MBED_CONF_APP_WIFI_UNSECURE_SSID, NULL));

    TEST_ASSERT_EQUAL_INT8(0, wifi->get_rssi());

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());

    TEST_ASSERT_INT8_WITHIN(-35, -65, wifi->get_rssi()); // -30 ... -100
}

