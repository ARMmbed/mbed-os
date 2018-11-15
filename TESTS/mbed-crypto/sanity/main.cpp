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

void test_crypto_hash_verify(void)
{
    psa_algorithm_t alg = PSA_ALG_SHA_256;
    psa_hash_operation_t operation;
    /* SHA-256 hash of an empty string */
    static const unsigned char hash[] = {
        0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8,
        0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
        0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
    };

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_crypto_init());

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_setup(&operation, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_verify(&operation, hash, sizeof(hash)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_hash_abort(&operation));

    mbedtls_psa_crypto_free();
}

void test_crypto_symmetric_cipher_encrypt_decrypt(void)
{
    psa_key_slot_t slot = 1;
    psa_key_type_t key_type = PSA_KEY_TYPE_AES;
    psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;
    psa_cipher_operation_t operation;
    psa_key_policy_t policy;
    size_t output_len;
    static const unsigned char key[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    static const unsigned char input[] = {
        0xb0, 0x28, 0x9f, 0x04, 0xdc, 0x7f, 0xe2, 0x25,
        0xa2, 0xce, 0xe9, 0xd3, 0xb9, 0xbc, 0xc7, 0x2f
    };
    static const unsigned char expected_encryption[] = {
        0x28, 0x8d, 0x76, 0xc0, 0xa7, 0x09, 0x50, 0x3f,
        0x87, 0x96, 0x1e, 0x96, 0x05, 0xcb, 0xb9, 0x6d
    };
    unsigned char encrypted[sizeof(input)], decrypted[sizeof(input)], iv[16];

    memset(iv, 0x2a, sizeof(iv));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_crypto_init());

    psa_key_policy_init(&policy);
    psa_key_policy_set_usage(&policy, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, alg);
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_set_key_policy(slot, &policy));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_import_key(slot, key_type, key, sizeof(key)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_encrypt_setup(&operation, slot, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_set_iv(&operation, iv, sizeof(iv)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_update(&operation, input, sizeof(input),
                                                     encrypted, sizeof(encrypted), &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_finish(&operation, encrypted + output_len,
                                                     sizeof(encrypted) - output_len, &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_abort(&operation));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_encryption, encrypted, sizeof(expected_encryption));

    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_decrypt_setup(&operation, slot, alg));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_set_iv(&operation, iv, sizeof(iv)));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_update(&operation, encrypted, sizeof(encrypted),
                                                     decrypted, sizeof(decrypted), &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_finish(&operation, decrypted + output_len,
                                                     sizeof(decrypted) - output_len, &output_len));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_cipher_abort(&operation));
    TEST_ASSERT_EQUAL(PSA_SUCCESS, psa_destroy_key(slot));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(input, decrypted, sizeof(input));

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
    Case("mbed-crypto hash verify", test_crypto_hash_verify, case_failure_handler),
    Case("mbed-crypto symmetric cipher encrypt/decrypt", test_crypto_symmetric_cipher_encrypt_decrypt, case_failure_handler),
};

Specification specification(test_setup, cases);

int main(void)
{
    return !Harness::run(specification);
}
