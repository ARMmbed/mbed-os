
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#include "mbedtls/cmac.h"
#include "mbedtls/platform_util.h"
#include <string.h>


int mbedtls_cipher_cmac_starts( mbedtls_cipher_context_t *ctx,
                                const unsigned char *key, size_t keybits )
{
    return 0;
}

int mbedtls_cipher_cmac_update( mbedtls_cipher_context_t *ctx,
                                const unsigned char *input, size_t ilen )
{
    return( 0 );
}

int mbedtls_cipher_cmac_finish( mbedtls_cipher_context_t *ctx,
                                unsigned char *output )
{
    return( 0 );
}

int mbedtls_cipher_cmac_reset( mbedtls_cipher_context_t *ctx )
{
    return( 0 );
}

int mbedtls_cipher_cmac( const mbedtls_cipher_info_t *cipher_info,
                         const unsigned char *key, size_t keylen,
                         const unsigned char *input, size_t ilen,
                         unsigned char *output )
{
    return( 0 );
}

int mbedtls_aes_cmac_prf_128( const unsigned char *key, size_t key_length,
                              const unsigned char *input, size_t in_len,
                              unsigned char *output )
{
    return( 0 );
}

