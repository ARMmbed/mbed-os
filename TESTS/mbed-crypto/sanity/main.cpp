#include <stdio.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "psa/crypto.h"

using namespace utest::v1;

void test_crypto_random(void)
{
    static const unsigned char trail[] = "don't overwrite me";
    unsigned char changed[256] = { 0 };
    unsigned char output[sizeof(changed) + sizeof(trail)];
    size_t i, bytes = sizeof(changed);
    unsigned int run;

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_crypto_init());

    memcpy(output + bytes, trail, sizeof(trail));
    /* Run several times, to ensure that every output byte will be
     * nonzero at least once with overwhelming probability
     * (2^(-8*number_of_runs)). */
    for (run = 0; run < 10; run++) {
        memset(output, 0, bytes);
        TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_random(output, bytes));
        /* Check that no more than 'bytes' have been overwritten */
        TEST_ASSERT_EQUAL_UINT8_ARRAY(trail, output + bytes, sizeof(trail));

        for (i = 0; i < bytes; i++) {
            if (0 != output[i]) {
                ++changed[i];
            }
        }
    }

    /* Check that every byte was changed to nonzero at least once. This
     * validates that psa_generate_random is overwriting every byte of
     * the output buffer. */
    for (i = 0; i < bytes; i++) {
        TEST_ASSERT_NOT_EQUAL(0, changed[i]);
    }

    mbedtls_psa_crypto_free();
}

utest::v1::status_t case_failure_handler(const Case *const source, const failure_t reason)
{
    mbedtls_psa_crypto_free();
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("mbed-crypto random", test_crypto_random, case_failure_handler),
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
