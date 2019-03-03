#include "mbed.h"
#include "rtos.h"
#include "val_interfaces.h"
#include "pal_common.h"

#include "psa/crypto.h"
#include "entropy_poll.h"

#include "psa_attest_inject_key.h"

#include "pal_mbed_os_crypto.h"

#include "lifecycle.h"

#define TEST_STACK_SIZE 8192
extern val_api_t val_api;
extern psa_api_t psa_api;
#ifdef PS_TEST
extern "C" psa_status_t psa_ps_reset();
#endif

test_entry_f test_g = NULL;


static void psa_attestation_inject_key_for_test(void)
{
#ifndef PSA_ATTESTATION_DISABLED
    const uint8_t private_key_data[] = {
       0x49, 0xc9, 0xa8, 0xc1, 0x8c, 0x4b, 0x88, 0x56,
       0x38, 0xc4, 0x31, 0xcf, 0x1d, 0xf1, 0xc9, 0x94,
       0x13, 0x16, 0x09, 0xb5, 0x80, 0xd4, 0xfd, 0x43,
       0xa0, 0xca, 0xb1, 0x7d, 0xb2, 0xf1, 0x3e, 0xee
   };

    const uint8_t public_key_data[] = {
       0x04, 0x77, 0x72, 0x65, 0x6f, 0x81, 0x4b, 0x39,
       0x92, 0x79, 0xd5, 0xe1, 0xf1, 0x78, 0x1f, 0xac,
       0x6f, 0x09, 0x9a, 0x3c, 0x5c, 0xa1, 0xb0, 0xe3,
       0x53, 0x51, 0x83, 0x4b, 0x08, 0xb6, 0x5e, 0x0b,
       0x57, 0x25, 0x90, 0xcd, 0xaf, 0x8f, 0x76, 0x93,
       0x61, 0xbc, 0xf3, 0x4a, 0xcf, 0xc1, 0x1e, 0x5e,
       0x07, 0x4e, 0x84, 0x26, 0xbd, 0xde, 0x04, 0xbe,
       0x6e, 0x65, 0x39, 0x45, 0x44, 0x96, 0x17, 0xde,
       0x45
   };
    psa_status_t status = PSA_SUCCESS;
    size_t exported_length;
    uint8_t exported[sizeof(public_key_data)];
    psa_crypto_init();
    psa_attestation_inject_key(private_key_data,
                                sizeof(private_key_data),
                                PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP256R1),
                                exported,
                                sizeof(exported),
                                &exported_length);
#endif
}


#if defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)

#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

static void inject_entropy()
{
    uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = { 0 };
    for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
        seed[i] = i;
    }
    mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
}
#endif // defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)


void main_wrapper(void)
{
#ifdef ITS_TEST
    mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
#elif PS_TEST
    psa_ps_reset();
#endif

#if defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC)
        inject_entropy();
#endif /* defined(MBEDTLS_ENTROPY_NV_SEED) || defined(COMPONENT_PSA_SRV_IPC) */
        psa_attestation_inject_key_for_test(); // inject key incase needed
        test_g(&val_api, &psa_api);
        
#ifdef ITS_TEST
    mbed_psa_reboot_and_request_new_security_state(PSA_LIFECYCLE_ASSEMBLY_AND_TEST);
#elif PS_TEST
    psa_ps_reset();
#endif
}

int test_start(test_entry_f test_f)
{
    test_g = test_f;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE, NULL);
    thread.start(main_wrapper);
    thread.join();
    return 0;
}
