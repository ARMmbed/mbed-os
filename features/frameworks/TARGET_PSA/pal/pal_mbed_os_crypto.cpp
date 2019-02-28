#include "mbed.h"
#include "rtos.h"
#include "val_interfaces.h"
#include "pal_common.h"

#include "psa/crypto.h"
#include "entropy_poll.h"

#include "pal_mbed_os_crypto.h"

#include "lifecycle.h"

#define TEST_STACK_SIZE 8192
extern val_api_t val_api;
extern psa_api_t psa_api;
#ifdef PS_TEST
extern "C" psa_status_t psa_ps_reset();
#endif

test_entry_f test_g = NULL;

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
