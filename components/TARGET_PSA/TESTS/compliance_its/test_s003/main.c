#include "val_interfaces.h"
#include "pal_mbed_os_crypto.h"
#include "lifecycle.h"

#ifdef ITS_TEST
void test_entry_s003(val_api_t *val_api, psa_api_t *psa_api);
#elif PS_TEST
#error [NOT_SUPPORTED] Test is too long for CI, thus always fails on timeout.
void test_entry_p003(val_api_t *val_api, psa_api_t *psa_api);
#endif

int main(void)
{
#ifdef ITS_TEST
    test_start(test_entry_s003);
#elif PS_TEST
    test_start(test_entry_p003);
#endif

}
