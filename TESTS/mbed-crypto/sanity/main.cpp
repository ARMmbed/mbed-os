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

void test_crypto_asymmetric_encrypt_decrypt(void)
{
    psa_key_slot_t slot = 1;
    psa_key_type_t key_type = PSA_KEY_TYPE_RSA_KEYPAIR;
    psa_algorithm_t alg = PSA_ALG_RSA_PKCS1V15_CRYPT;
    size_t key_bits = 512, got_bits = 0, output_length;
    psa_key_policy_t policy;
    static const unsigned char input[] = "encrypt me!";
    unsigned char encrypted[64];
    unsigned char decrypted[sizeof(input)];

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_crypto_init());

    psa_key_policy_init(&policy);
    psa_key_policy_set_usage(&policy, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, alg);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_set_key_policy(slot, &policy));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_generate_key(slot, key_type, key_bits, NULL, 0));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_get_key_information(slot, NULL, &got_bits));
    TEST_ASSERT_EQUAL(key_bits, got_bits);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_asymmetric_encrypt(slot, alg, input, sizeof(input), NULL, 0,
                                                          encrypted, sizeof(encrypted), &output_length));
    TEST_ASSERT_EQUAL(sizeof(encrypted), output_length);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_asymmetric_decrypt(slot, alg, encrypted, sizeof(encrypted), NULL, 0,
                                                          decrypted, sizeof(decrypted), &output_length));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(slot));
    TEST_ASSERT_EQUAL(sizeof(input), output_length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(input, decrypted, output_length);

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
    Case("mbed-crypto asymmetric encrypt/decrypt", test_crypto_asymmetric_encrypt_decrypt, case_failure_handler),
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
