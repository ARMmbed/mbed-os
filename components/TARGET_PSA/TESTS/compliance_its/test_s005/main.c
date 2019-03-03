#include "val_interfaces.h"
#include "pal_mbed_os_crypto.h"

#ifdef ITS_TEST
void test_entry_s005(val_api_t *val_api, psa_api_t *psa_api);
#elif PS_TEST
void test_entry_p005(val_api_t *val_api, psa_api_t *psa_api);
#endif

int main(void)
{
#ifdef ITS_TEST
    test_start(test_entry_s005);
#elif PS_TEST
    test_start(test_entry_p005);
#endif
}
