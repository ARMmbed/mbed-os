#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "wifi_tests.h"

using namespace utest::v1;

void wifi_set_credential(void)
{
    WiFiInterface *iface = get_interface();
    nsapi_error_t error;

    error = iface->set_credentials(NULL, NULL, NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_WEP);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "kUjd0PHHeAqaDoyfcDDEOvbyiVbYMpUHDukGoR6EJZnO5iLzWsfwiM9JQqOngni82", NSAPI_SECURITY_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
}
