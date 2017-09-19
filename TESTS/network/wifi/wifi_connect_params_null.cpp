#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_connect_params_null(void)
{
    WiFiInterface *wifi = get_interface();
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_PARAMETER, wifi->connect(NULL, NULL));
}
