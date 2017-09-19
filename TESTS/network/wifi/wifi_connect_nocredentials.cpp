#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_connect_nocredentials(void)
{
    WiFiInterface *wifi = get_interface();
    nsapi_error_t error;
    error = wifi->connect();
    wifi->disconnect();
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
}
