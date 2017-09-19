#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"
#include <stdbool.h>

using namespace utest::v1;

void wifi_set_channel(void)
{
    bool is_2Ghz = false;
    bool is_5Ghz = false;

    WiFiInterface *wifi = get_interface();

    if (wifi->set_channel(1) == NSAPI_ERROR_UNSUPPORTED && wifi->set_channel(36) == NSAPI_ERROR_UNSUPPORTED) {
        TEST_IGNORE_MESSAGE("set_channel() not supported");
        return;
    }

    nsapi_error_t error;
    error = wifi->set_channel(1);
    if (error == NSAPI_ERROR_OK) {
        is_2Ghz = true;
    }

    error = wifi->set_channel(30);
    if (error == NSAPI_ERROR_OK) {
        is_5Ghz = true;
    }

    TEST_ASSERT(is_2Ghz || is_5Ghz);

    if (is_2Ghz) {
        error = wifi->set_channel(0);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(1);
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->set_channel(13);
        TEST_ASSERT(error == NSAPI_ERROR_OK || error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(15);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
    }

    if (is_5Ghz) {
        error = wifi->set_channel(30);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(36);
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->set_channel(169);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
    }

}

