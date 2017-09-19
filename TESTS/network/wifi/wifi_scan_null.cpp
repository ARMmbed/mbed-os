#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_scan_null(void)
{
    WiFiInterface *wifi = get_interface();
    TEST_ASSERT(wifi->scan(NULL, 0) >= 2);
}

