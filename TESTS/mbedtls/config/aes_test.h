#ifndef __AES_TEST_H__
#define __AES_TEST_H__

int mbedtls_aes_ecb_test( int verbose );
int mbedtls_aes_cbc_test( int verbose );
int mbedtls_aes_cfb_test( int verbose );
int mbedtls_aes_ctr_test( int verbose );
int mbedtls_gcm_self_test2( int verbose );
int mbedtls_ccm_self_test( int verbose );

#endif // __AES_TEST_H__