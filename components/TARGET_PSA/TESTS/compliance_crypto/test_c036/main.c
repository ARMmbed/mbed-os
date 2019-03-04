#include "val_interfaces.h"
#include "pal_mbed_os_intf.h"
#include "unity/unity.h"

void test_entry_c036(val_api_t *val_api, psa_api_t *psa_api);

int main(void)
{
    test_start(test_entry_c036, COMPLIANCE_TEST_CRYPTO);
}
