#ifndef PAL_MBED_OS_CRYPTO_H_
#define PAL_MBED_OS_CRYPTO_H_

#include "val_interfaces.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _compliance_test_type {
    COMPLIANCE_TEST_UNDEFINED,
    COMPLIANCE_TEST_START = COMPLIANCE_TEST_UNDEFINED,
    COMPLIANCE_TEST_CRYPTO,
    COMPLIANCE_TEST_ATTESTATION,
    COMPLIANCE_TEST_STORAGE,
    COMPLIANCE_TEST_END
} compliance_test_type;

typedef void (*test_entry_f)(val_api_t *val_api, psa_api_t *psa_api);
int test_start(test_entry_f test_f, compliance_test_type type);

#ifdef __cplusplus
}
#endif

#endif /* PAL_MBED_OS_CRYPTO_H_ */
