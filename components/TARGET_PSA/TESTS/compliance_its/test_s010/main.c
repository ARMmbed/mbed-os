#include "val_interfaces.h"
#include "pal_mbed_os_intf.h"

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] PSA compliance its test cases require RTOS to run
#else

#ifdef ITS_TEST
void test_entry_s010(val_api_t *val_api, psa_api_t *psa_api);
#elif PS_TEST
void test_entry_p010(val_api_t *val_api, psa_api_t *psa_api);
#endif

int main(void)
{
#ifdef ITS_TEST
    test_start(test_entry_s010, COMPLIANCE_TEST_STORAGE);
#elif PS_TEST
    test_start(test_entry_p010, COMPLIANCE_TEST_STORAGE);
#endif
}
#endif // !defined(MBED_CONF_RTOS_PRESENT)
