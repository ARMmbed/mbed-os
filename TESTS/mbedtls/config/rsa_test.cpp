#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbedtls/rsa.h"
#include "rsa_test.h"

#if defined(MBEDTLS_RSA_C)
int mbedtls_config_rsa_test_mbedos( int verbose ) {
    // This test just checks that the rsa functions are available to be called.
    // Testing the functionality of the rsa functions is beyond the scope
    // This test does not generate asserts. Instead, a failure will cause a compile error
    mbedtls_rsa_context context;
    mbedtls_rsa_init(&context, 0, 0 );
    return 0;
}
#endif // defined(MBEDTLS_RSA_C)
