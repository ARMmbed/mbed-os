#ifndef PAL_MBED_OS_CRYPTO_H_
#define PAL_MBED_OS_CRYPTO_H_

#include "val_interfaces.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*test_entry_f)(val_api_t *val_api, psa_api_t *psa_api);
int test_start(test_entry_f test_f);

#ifdef __cplusplus
}
#endif

#endif /* PAL_MBED_OS_CRYPTO_H_ */
