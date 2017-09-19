#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_connect_disconnect_repeat(void)
{
    WiFiInterface *wifi = get_interface();
    nsapi_error_t error;

    error = wifi->set_credentials(MBED_CONF_APP_WIFI_UNSECURE_SSID, NULL);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    for(int i=0; i<10; i++) {
        error = wifi->connect();
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->disconnect();
        TEST_ASSERT(error == NSAPI_ERROR_OK);
    }
}
