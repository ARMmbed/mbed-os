#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbedtls/ecdsa.h"
#include "ecdsa_test.h"

#if defined(MBEDTLS_ECDSA_C)
int mbedtls_ecdsa_test_mbedos( int verbose ) {
    // This test just checks that the ecdsa functions are available to be called.
    // Testing the functionality of the ecdsa functions is beyond the scope
    // This test does not generate asserts. Instead, a failure will cause a compile error
    mbedtls_ecp_group group;
    group.N.p = NULL; // will cause the ecdsa function to exit early
    mbedtls_ecdsa_sign( &group, NULL, NULL, NULL, NULL, 0, NULL, NULL );

    return 0;
}
#endif // MBEDTLS_ECDSA_C