#ifndef __AES_TEST_H__
#define __AES_TEST_H__

int mbedtls_aes_ecb_test_mbedos( int verbose );
int mbedtls_aes_cbc_test_mbedos( int verbose );
int mbedtls_aes_cfb_test_mbedos( int verbose );
int mbedtls_aes_ctr_test_mbedos( int verbose );

#endif // __AES_TEST_H__