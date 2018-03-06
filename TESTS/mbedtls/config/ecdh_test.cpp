#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbedtls/ecdh.h"
#include "ecdh_test.h"

#if defined (MBEDTLS_ECDH_C)
int mbedtls_ecdh_test_mbedos( int verbose ) {
    // This is just a smoke test. It doesn't test the functionality
    // of the DH algorithm. It just makes sure the function is present to be 
    // called (it tests the build configuration).
    // It never generates asserts. Instead, failures should show up at compile
    // time
    mbedtls_ecdh_make_params( NULL, NULL, NULL, 0, NULL, NULL );

    return 0;
}
#endif // MBEDTLS_ECDH_C