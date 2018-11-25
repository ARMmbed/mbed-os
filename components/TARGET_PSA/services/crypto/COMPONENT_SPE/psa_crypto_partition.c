// ---------------------------------- Includes ---------------------------------
#include "spm_panic.h"
#include "spm_server.h"
#include "spm/psa_defs.h"
#include "spm/spm_client.h"

#define PSA_CRYPTO_SECURE 1
#include "crypto_spe.h"
#include "crypto_platform_spe.h"
#include "psa_psa_f_partition.h"
#include "mbedtls/entropy.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#define mbedtls_calloc calloc
#define mbedtls_free   free
#endif

// ------------------------- Partition's Main Thread ---------------------------
static void psa_crypto_init_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    psa_get( PSA_CRYPTO_INIT, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT:
        {
            break;
        }

        case PSA_IPC_CALL:
        {
            status = psa_crypto_init();
            break;
        }

        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }
    psa_reply( msg.handle, ( psa_error_t ) status );
}

static void psa_crypto_free_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    psa_get( PSA_CRYPTO_FREE, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT:
        {
            break;
        }

        case PSA_IPC_CALL:
        {
            mbedtls_psa_crypto_free();
            break;
        }

        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }
    psa_reply( msg.handle, ( psa_error_t ) status );
}

static void psa_mac_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    psa_get( PSA_MAC, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        {
            psa_mac_operation_t *psa_operation = mbedtls_calloc( 1, sizeof( psa_mac_operation_t ) );
            if( psa_operation == NULL )
            {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            psa_set_rhandle( msg.handle, psa_operation );
            break;
        }

        case PSA_IPC_CALL:
        {
            uint32_t bytes_read;
            psa_crypto_ipc_t psa_crypto = { 0 };

            if( msg.in_size[0] != sizeof( psa_crypto_ipc_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read( msg.handle, 0, &psa_crypto, msg.in_size[0] );

            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch( psa_crypto.func )
            {
                case PSA_MAC_SIGN_SETUP:
                {
                    status = psa_mac_sign_setup( msg.rhandle,
                                                 psa_crypto.key,
                                                 psa_crypto.alg );
                    break;
                }

                case PSA_MAC_VERIFY_SETUP:
                {
                    status = psa_mac_verify_setup( msg.rhandle,
                                                   psa_crypto.key,
                                                   psa_crypto.alg );
                    break;
                }

                case PSA_MAC_UPDATE:
                {
                    uint8_t *input_ptr = mbedtls_calloc( 1, msg.in_size[1] );
                    if( input_ptr == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1, input_ptr,
                                           msg.in_size[1] );

                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_mac_update( msg.rhandle,
                                            input_ptr,
                                            msg.in_size[1] );

                    mbedtls_free( input_ptr );
                    break;
                }

                case PSA_MAC_SIGN_FINISH:
                {
                    size_t mac_size = 0;
                    bytes_read = psa_read( msg.handle, 1, &mac_size,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    size_t mac_length = 0;
                    uint8_t *mac = mbedtls_calloc( 1, mac_size );
                    if( mac == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    status = psa_mac_sign_finish( msg.rhandle, mac, mac_size,
                                                  &mac_length );

                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, mac, mac_length );
                        psa_write( msg.handle, 1, &mac_length,
                                   sizeof( mac_length ) );
                    }
                    mbedtls_free( mac );
                    break;
                }
                case PSA_MAC_VERIFY_FINISH:
                {
                    size_t mac_length = 0;
                    bytes_read = psa_read( msg.handle, 1, &mac_length,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] ||
                        mac_length != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    uint8_t *mac = mbedtls_calloc( 1, mac_length );
                    if( mac == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 2, mac, msg.in_size[2] );
                    if( bytes_read != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    status = psa_mac_verify_finish( msg.rhandle, mac, mac_length );
                    mbedtls_free( mac );
                    break;
                }
                case PSA_MAC_ABORT:
                {
                    status = psa_mac_abort( msg.rhandle );
                    break;
                }
                default:
                {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
                break;
            }
            break;
        }
        case PSA_IPC_DISCONNECT:
        {
            psa_mac_abort( msg.rhandle );
            if( msg.rhandle != NULL )
                mbedtls_free( msg.rhandle );
            break;
        }
        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }
    psa_reply( msg.handle, ( psa_error_t ) status );
}


static void psa_hash_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    psa_get( PSA_HASH, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        {
            psa_hash_operation_t *psa_operation = mbedtls_calloc( 1, sizeof( psa_hash_operation_t ) );
            if( psa_operation == NULL )
            {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            psa_set_rhandle( msg.handle, psa_operation );
            break;
        }

        case PSA_IPC_CALL:
        {
            uint32_t bytes_read = 0;
            psa_crypto_ipc_t psa_crypto = {0};

            if( msg.in_size[0] != sizeof( psa_crypto_ipc_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            bytes_read = psa_read( msg.handle, 0, &psa_crypto, msg.in_size[0] );

            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch( psa_crypto.func )
            {
                case PSA_HASH_SETUP:
                {
                    status = psa_hash_setup( msg.rhandle,
                                            psa_crypto.alg );
                    break;
                }

                case PSA_HASH_UPDATE:
                {
                    uint8_t *input_ptr = mbedtls_calloc( 1, msg.in_size[1] );
                    if( input_ptr == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1, input_ptr,
                                           msg.in_size[1] );

                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_hash_update( msg.rhandle,
                                            input_ptr,
                                            msg.in_size[1] );
                    mbedtls_free( input_ptr );
                    break;
                }

                case PSA_HASH_FINISH:
                {
                    size_t hash_size = 0;
                    bytes_read = psa_read( msg.handle, 1, &hash_size,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    size_t hash_length = 0;
                    uint8_t *hash = mbedtls_calloc( 1, hash_size );
                    if( hash == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    status = psa_hash_finish( msg.rhandle, hash, hash_size,
                                              &hash_length );

                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, hash, hash_length );
                        psa_write( msg.handle, 1, &hash_length,
                                   sizeof( hash_length ) );
                    }

                    mbedtls_free( hash );
                    break;
                }
                case PSA_HASH_VERIFY:
                {
                    size_t hash_length = 0;
                    bytes_read = psa_read( msg.handle, 1, &hash_length,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] ||
                        hash_length != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    uint8_t *hash = mbedtls_calloc( 1, hash_length );
                    if( hash == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 2, hash, msg.in_size[2] );
                    if( bytes_read != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    status = psa_hash_verify( msg.rhandle, hash, hash_length );
                    mbedtls_free( hash );
                    break;
                }
                case PSA_HASH_ABORT:
                {
                    status = psa_hash_abort( msg.rhandle );
                    break;
                }
                default:
                {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }
            break;
        }
        case PSA_IPC_DISCONNECT:
        {
            psa_hash_abort( msg.rhandle );
            if( msg.rhandle != NULL )
                mbedtls_free( msg.rhandle );
            break;
        }
        default:
        {
            status = PSA_ERROR_INSUFFICIENT_MEMORY;
            break;
        }
    }

    psa_reply( msg.handle, status );
}

static void psa_asymmetric_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    psa_get( PSA_ASYMMETRIC, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
            break;

        case PSA_IPC_CALL:
        {
            if( msg.in_size[0] != sizeof( psa_crypto_ipc_asymmetric_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            uint32_t bytes_read = 0;
            psa_crypto_ipc_asymmetric_t psa_crypto = {0};

            bytes_read = psa_read( msg.handle, 0, &psa_crypto, msg.in_size[0] );

            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch( psa_crypto.func )
            {
                case PSA_ASYMMETRIC_SIGN:
                {
                    uint8_t *signature;
                    uint8_t *hash;
                    size_t signature_length = 0;

                    signature = mbedtls_calloc( 1, msg.out_size[0] );
                    if( signature == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    hash = mbedtls_calloc( 1, msg.in_size[1] );
                    if( hash == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        mbedtls_free( signature );
                        break;
                    }

                    bytes_read = psa_read( msg.handle, 1,
                                           hash, msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    status = psa_asymmetric_sign( psa_crypto.key,
                                                  psa_crypto.alg,
                                                  hash,
                                                  msg.in_size[1],
                                                  signature,
                                                  msg.out_size[0],
                                                  &signature_length );

                    if( status == PSA_SUCCESS )
                        psa_write( msg.handle, 0, signature, signature_length );

                    psa_write( msg.handle, 1,
                               &signature_length, sizeof( signature_length ) );
                    mbedtls_free( signature );
                    break;
                }

                case PSA_ASYMMETRIC_VERIFY:
                {
                    uint8_t *signature;
                    uint8_t *hash;
                    signature = mbedtls_calloc( 1, msg.in_size[1] );
                    if( signature == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1,
                                           signature, msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    hash = mbedtls_calloc( 1, msg.in_size[2] );
                    if( hash == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    bytes_read = psa_read( msg.handle, 2,
                                           hash, msg.in_size[2] );
                    if( bytes_read != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_asymmetric_verify( psa_crypto.key,
                                                    psa_crypto.alg,
                                                    hash,
                                                    msg.in_size[2],
                                                    signature,
                                                    msg.in_size[1] );
                    mbedtls_free( signature );
                    mbedtls_free( hash );
                    break;
                }

                case PSA_ASYMMETRIC_ENCRYPT:
                case PSA_ASYMMETRIC_DECRYPT:
                {
                    uint8_t *input;
                    uint8_t *salt;
                    uint8_t *output;
                    size_t output_length = 0;

                    uint8_t *buffer = mbedtls_calloc( 1, msg.in_size[1] );
                    if( buffer == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1, buffer,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    input = buffer;
                    salt = buffer + psa_crypto.input_length;

                    output = mbedtls_calloc( 1, msg.out_size[0] );
                    if( output == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        mbedtls_free( buffer );
                        break;
                    }

                    if( psa_crypto.func == PSA_ASYMMETRIC_ENCRYPT )
                        status = psa_asymmetric_encrypt( psa_crypto.key,
                                                         psa_crypto.alg,
                                                         input,
                                                         psa_crypto.input_length,
                                                         salt,
                                                         psa_crypto.salt_length,
                                                         output,
                                                         msg.out_size[0],
                                                         &output_length );
                    else
                        status = psa_asymmetric_decrypt( psa_crypto.key,
                                                         psa_crypto.alg,
                                                         input,
                                                         psa_crypto.input_length,
                                                         salt,
                                                         psa_crypto.salt_length,
                                                         output,
                                                         msg.out_size[0],
                                                         &output_length );

                    if( status == PSA_SUCCESS )
                        psa_write( msg.handle, 0, output, output_length );
                    psa_write( msg.handle, 1,
                               &output_length, sizeof( output_length ) );

                    mbedtls_free( output );
                    mbedtls_free( buffer );
                    break;
                }
                break;

                default:
                status = PSA_ERROR_NOT_SUPPORTED;
            }
        }
        case PSA_IPC_DISCONNECT:
            break;

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
    }
    psa_reply( msg.handle, ( psa_error_t ) status );
}

static void psa_aead_operation()
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;
    psa_get( PSA_AEAD, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
            break;

        case PSA_IPC_CALL:
        {
            if( msg.in_size[0] != sizeof( psa_crypto_ipc_aead_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }

            uint32_t bytes_read = 0;
            psa_crypto_ipc_aead_t psa_crypto = {0};

            bytes_read = psa_read( msg.handle, 0, &psa_crypto, msg.in_size[0] );

            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch( psa_crypto.func )
            {
                case PSA_AEAD_ENCRYPT:
                case PSA_AEAD_DECRYPT:
                {
                    uint8_t *input;
                    uint8_t *additional_data;
                    uint8_t *output;
                    size_t output_length = 0;

                    uint8_t *buffer = mbedtls_calloc( 1, msg.in_size[1] );
                    if( buffer == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1, buffer,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    additional_data = buffer;
                    input = buffer + psa_crypto.additional_data_length;

                    output = mbedtls_calloc( 1, msg.out_size[0] );
                    if( output == NULL )
                    {
                        mbedtls_free( buffer );
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    if( psa_crypto.func == PSA_AEAD_ENCRYPT )
                        status = psa_aead_encrypt( psa_crypto.key,
                                                   psa_crypto.alg,
                                                   psa_crypto.nonce,
                                                   ( size_t )psa_crypto.nonce_size,
                                                   additional_data,
                                                   psa_crypto.additional_data_length,
                                                   input,
                                                   psa_crypto.input_length,
                                                   output,
                                                   msg.out_size[0],
                                                   &output_length );
                    else
                        status = psa_aead_decrypt( psa_crypto.key,
                                                   psa_crypto.alg,
                                                   psa_crypto.nonce,
                                                   ( size_t )psa_crypto.nonce_size,
                                                   additional_data,
                                                   psa_crypto.additional_data_length,
                                                   input,
                                                   psa_crypto.input_length,
                                                   output,
                                                   msg.out_size[0],
                                                   &output_length );

                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, output, output_length );
                        psa_write( msg.handle, 1,
                                   &output_length, sizeof( output_length ) );
                    }
                    mbedtls_free( buffer );
                    mbedtls_free( output );
                    break;
                }
            default:
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
            }
            break;
        }
        case PSA_IPC_DISCONNECT:
            break;

        default:
            status = PSA_ERROR_NOT_SUPPORTED;
    }
    psa_reply( msg.handle, ( psa_error_t ) status );
}

static void psa_symmetric_operation( void )
{
    psa_status_t status = PSA_SUCCESS;
    psa_msg_t msg = { 0 };

    psa_get( PSA_SYMMETRIC, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        {
            psa_cipher_operation_t *psa_operation =
                mbedtls_calloc( 1, sizeof( psa_cipher_operation_t ) );
            if( psa_operation == NULL )
            {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            psa_set_rhandle( msg.handle, psa_operation );
            break;
        }
        case PSA_IPC_CALL:
        {
            uint32_t bytes_read;
            psa_crypto_ipc_t psa_crypto_ipc = { 0 };

            if( msg.in_size[0] != sizeof( psa_crypto_ipc_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }
            bytes_read = psa_read( msg.handle, 0, &psa_crypto_ipc,
                                   msg.in_size[0] );
            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch ( psa_crypto_ipc.func )
            {
                case PSA_CIPHER_ENCRYPT_SETUP:
                {
                    status = psa_cipher_encrypt_setup( msg.rhandle,
                                                       psa_crypto_ipc.key,
                                                       psa_crypto_ipc.alg );
                    break;
                }
                case PSA_CIPHER_DECRYPT_SETUP:
                {
                    status = psa_cipher_decrypt_setup( msg.rhandle,
                                                       psa_crypto_ipc.key,
                                                       psa_crypto_ipc.alg );
                    break;
                }
                case PSA_CIPHER_GENERATE_IV:
                {
                    size_t iv_length = 0;
                    size_t iv_size = msg.out_size[0];
                    unsigned char iv[PSA_MAX_NONCE_SIZE] = { 0 };

                    status = psa_cipher_generate_iv( msg.rhandle, iv,
                                                     iv_size, &iv_length );
                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, iv, iv_length );
                        psa_write( msg.handle, 1, &iv_length,
                            sizeof( iv_length ) );
                    }
                    break;
                }
                case PSA_CIPHER_SET_IV:
                {
                    size_t iv_length = msg.in_size[1];
                    unsigned char iv[PSA_MAX_NONCE_SIZE] = { 0 };

                    bytes_read = psa_read( msg.handle, 1, iv, iv_length );
                    if( bytes_read != iv_length )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }
                    status = psa_cipher_set_iv( msg.rhandle, iv, iv_length );

                    break;
                }
                case PSA_CIPHER_UPDATE:
                {
                    size_t input_length = msg.in_size[1];
                    size_t output_size = msg.out_size[0];
                    size_t output_length = 0;
                    uint8_t *input;
                    unsigned char *output;

                    input = mbedtls_calloc( 1, input_length );
                    if( input == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }
                    bytes_read = psa_read( msg.handle, 1, input,
                        input_length );
                    if( bytes_read != input_length )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    output = mbedtls_calloc( 1, output_size );
                    if( output == NULL )
                    {
                        mbedtls_free( input );
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    status = psa_cipher_update( msg.rhandle, input,
                        input_length, output, output_size, &output_length );
                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, output, output_length );
                        psa_write( msg.handle, 1,
                                   &output_length, sizeof( output_length ) );
                    }
                    mbedtls_free( input );
                    mbedtls_free( output );
                    break;
                }
                case PSA_CIPHER_FINISH:
                {
                    uint8_t *output;
                    size_t output_size = msg.out_size[0];
                    size_t output_length = 0;

                    output = mbedtls_calloc( 1, output_size );
                    if( output == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    status = psa_cipher_finish( msg.rhandle, output,
                        output_size, &output_length );
                    if( status == PSA_SUCCESS )
                    {
                        psa_write( msg.handle, 0, output, output_length );
                        psa_write( msg.handle, 1,
                                   &output_length, sizeof( output_length ) );
                    }
                    mbedtls_free( output );
                    break;
                }
                case PSA_CIPHER_ABORT:
                {
                    status = psa_cipher_abort( msg.rhandle );
                    break;
                }
                default:
                {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }
            break;
        }
        case PSA_IPC_DISCONNECT:
        {
            psa_cipher_abort( msg.rhandle );
            if( msg.rhandle != NULL )
                mbedtls_free( msg.rhandle );
            break;
        }
        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }

    psa_reply( msg.handle, ( psa_error_t ) status );
}


static void psa_key_management_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;
    psa_get( PSA_KEY_MNG, &msg );

    switch ( msg.type )
    {
            case PSA_IPC_CONNECT:
            {
                break; /* do nothing*/
            }

            case PSA_IPC_CALL:
            {
                if( msg.in_size[0] != sizeof( psa_key_mng_ipc_t ) )
                {
                    status = PSA_ERROR_COMMUNICATION_FAILURE;
                    break;
                }

                uint32_t bytes_read = 0;
                psa_key_mng_ipc_t psa_key_mng = {0};

                bytes_read = psa_read( msg.handle, 0,
                                       &psa_key_mng, msg.in_size[0] );

                if( bytes_read != msg.in_size[0] )
                {
                    SPM_PANIC("SPM read length mismatch");
                }

                switch( psa_key_mng.func )
                {
                    case PSA_GET_KEY_LIFETIME:
                    {
                        size_t lifetime_length = msg.out_size[0];
                        psa_key_lifetime_t lifetime;

                        status = psa_get_key_lifetime( psa_key_mng.key,
                                                       &lifetime );
                        if( status == PSA_SUCCESS )
                        {
                            psa_write( msg.handle, 0,
                                       &lifetime, lifetime_length );
                        }
                        break;
                    }
                    case PSA_SET_KEY_LIFETIME:
                    {
                        size_t lifetime_length = msg.in_size[1];
                        psa_key_lifetime_t lifetime;

                        bytes_read = psa_read( msg.handle, 1,
                                               &lifetime, lifetime_length );
                        if( bytes_read != lifetime_length )
                        {
                            SPM_PANIC("SPM read length mismatch");
                        }
                        status = psa_set_key_lifetime( psa_key_mng.key, lifetime );
                        break;
                    }
                    case PSA_SET_KEY_POLICY:
                    {
                        size_t policy_length = msg.in_size[1];
                        psa_key_policy_t policy;

                        bytes_read = psa_read( msg.handle, 1,
                                               &policy, policy_length );
                        if( bytes_read != policy_length )
                        {
                            SPM_PANIC("SPM read length mismatch");
                        }
                        status = psa_set_key_policy( psa_key_mng.key, &policy);
                        break;
                    }
                    case PSA_GET_KEY_POLICY:
                    {
                        size_t policy_size = msg.out_size[0];
                        psa_key_policy_t policy;

                        status = psa_get_key_policy( psa_key_mng.key, &policy );
                        if( status == PSA_SUCCESS )
                        {
                            psa_write( msg.handle, 0, &policy, policy_size );
                        }
                        break;
                    }
                    case PSA_IMPORT_KEY:
                    {
                        size_t key_length = msg.in_size[1];
                        uint8_t *key = mbedtls_calloc( 1, key_length );
                        if( key == NULL )
                        {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }

                        bytes_read = psa_read( msg.handle, 1, key, key_length );
                        if( bytes_read != key_length )
                        {
                            SPM_PANIC("SPM read length mismatch");
                        }
                        status = psa_import_key( psa_key_mng.key,
                                                 psa_key_mng.type,
                                                 key, key_length );
                        mbedtls_free( key );
                        break;
                    }
                    case PSA_DESTROY_KEY:
                    {
                        status  = psa_destroy_key( psa_key_mng.key );
                        break;
                    }
                    case PSA_GET_KEY_INFORMATION:
                    {
                        psa_key_type_t type;
                        size_t bits;
                        status = psa_get_key_information( psa_key_mng.key,
                                                          &type, &bits );
                        if( status == PSA_SUCCESS )
                        {
                            if( msg.out_size[0] >= sizeof( psa_key_type_t ) )
                                psa_write( msg.handle, 0,
                                           &type, sizeof( psa_key_type_t ) );
                            if( msg.out_size[1] >= sizeof( size_t ) )
                                psa_write( msg.handle, 1, &bits, sizeof( size_t ) );
                        }
                        break;
                    }
                    case PSA_EXPORT_KEY:
                    {
                        size_t key_length = msg.out_size[0];
                        size_t data_length;
                        uint8_t *key = mbedtls_calloc( 1, key_length );
                        if ( key == NULL )
                        {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }

                        status = psa_export_key( psa_key_mng.key, key,
                                                     key_length, &data_length );
                        if( status == PSA_SUCCESS )
                            psa_write( msg.handle, 0, key, data_length );

                        psa_write( msg.handle, 1,
                                   &data_length, sizeof( size_t ) );
                        mbedtls_free( key );
                        break;
                    }
                    case PSA_EXPORT_PUBLIC_KEY:
                    {
                        size_t key_length = msg.out_size[0];
                        size_t data_length;
                        uint8_t *key = mbedtls_calloc( 1, key_length );
                        if ( key == NULL )
                        {
                            status = PSA_ERROR_INSUFFICIENT_MEMORY;
                            break;
                        }

                        status = psa_export_public_key( psa_key_mng.key, key,
                                                     key_length, &data_length );
                        if( status == PSA_SUCCESS )
                            psa_write( msg.handle, 0, key, data_length );

                        psa_write( msg.handle, 1,
                                   &data_length, sizeof( size_t ) );
                        mbedtls_free( key );
                        break;
                    }
                    case PSA_GENERATE_KEY:
                    {
                        size_t bits;
                        size_t bits_size = msg.in_size[1];
                        size_t parameter_size = msg.in_size[2];
                        uint8_t *parameter = NULL;

                        bytes_read = psa_read( msg.handle, 1, &bits, bits_size );
                        if( bytes_read != bits_size )
                        {
                            SPM_PANIC("SPM read length mismatch");
                        }
                        if ( PSA_KEY_TYPE_RSA_KEYPAIR == psa_key_mng.type &&
                             parameter_size != 0 )
                        {
                            parameter = mbedtls_calloc( 1, parameter_size );
                            if( parameter == NULL )
                            {
                                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                                break;
                            }


                            bytes_read = psa_read( msg.handle, 2,
                                                    parameter, parameter_size );

                            if( bytes_read != parameter_size )
                            {
                                SPM_PANIC("SPM read length mismatch");
                            }
                        }
                        status = psa_generate_key( psa_key_mng.key,
                                                   psa_key_mng.type,
                                                   bits,
                                                   parameter, parameter_size );
                        mbedtls_free( parameter );

                        break;
                    }
                    default:
                    {
                        status = PSA_ERROR_NOT_SUPPORTED;
                        break;
                    }
                }
                break;
            }
            case PSA_IPC_DISCONNECT:
            {
                break; /* do nothing*/
            }
            default:
            {
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
            }
        }
    psa_reply( msg.handle, status );
}

static void psa_entropy_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;
    psa_get( PSA_ENTROPY_INJECT, &msg );

#if ( defined(MBEDTLS_ENTROPY_NV_SEED) && defined(MBEDTLS_PSA_HAS_ITS_IO) )
    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        {
            break; /* do nothing */
        }
        case PSA_IPC_CALL:
        {
            uint32_t bytes_read;
            size_t seed_size = msg.in_size[0];
            if( MBEDTLS_ENTROPY_MAX_SEED_SIZE < seed_size )
            {
                status = PSA_ERROR_INVALID_ARGUMENT;
                break;
            }
            unsigned char *seed = mbedtls_calloc( 1, seed_size );
            if( seed == NULL )
            {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            bytes_read = psa_read( msg.handle, 0, seed, seed_size );
            if( bytes_read != seed_size )
            {
                SPM_PANIC("SPM read length mismatch");
            }
            status = mbedtls_psa_inject_entropy( seed, seed_size );
            mbedtls_free( seed );
            break;
        }
        case PSA_IPC_DISCONNECT:
        {
            break; /* do nothing */
        }
        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }
#else
    status = PSA_ERROR_NOT_SUPPORTED;
#endif /* MBEDTLS_ENTROPY_NV_SEED && MBEDTLS_PSA_HAS_ITS_IO*/
    psa_reply( msg.handle, status );
}


static void psa_rng_operation( void )
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;
    psa_get( PSA_RNG, &msg );

    switch ( msg.type )
    {
            case PSA_IPC_CONNECT:
            {
                break; /* do nothing*/
            }

            case PSA_IPC_CALL:
            {
                size_t random_size = msg.out_size[0];
                unsigned char *random = mbedtls_calloc( 1, random_size );
                if( random == NULL )
                {
                    status = PSA_ERROR_INSUFFICIENT_MEMORY;
                    break;
                }
                status = psa_generate_random( random, random_size );
                if( status == PSA_SUCCESS )
                {
                    psa_write( msg.handle, 0, random, random_size );
                }
                mbedtls_free( random );
                break;
            }
            case PSA_IPC_DISCONNECT:
            {
                break; /* do nothing*/
            }
            default:
            {
                status = PSA_ERROR_NOT_SUPPORTED;
                break;
            }
        }
    psa_reply( msg.handle, status );
}


void psa_crypto_generator_operations( void )
{
    psa_status_t status = PSA_SUCCESS;
    psa_msg_t msg = { 0 };

    psa_get( PSA_GENERATOR, &msg );

    switch ( msg.type )
    {
        case PSA_IPC_CONNECT:
        {
            psa_crypto_generator_t *psa_operation =
                mbedtls_calloc( 1, sizeof( psa_crypto_generator_t ) );
            if( psa_operation == NULL )
            {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            psa_set_rhandle( msg.handle, psa_operation );
            break;
        }
        case PSA_IPC_CALL:
        {
            uint32_t bytes_read;
            psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0 };
            if( msg.in_size[0] != sizeof( psa_crypto_derivation_ipc_t ) )
            {
                status = PSA_ERROR_COMMUNICATION_FAILURE;
                break;
            }
            bytes_read = psa_read( msg.handle, 0, &psa_crypto_ipc,
                                   msg.in_size[0] );
            if( bytes_read != msg.in_size[0] )
            {
                SPM_PANIC("SPM read length mismatch");
            }

            switch ( psa_crypto_ipc.func )
            {
                case PSA_GET_GENERATOR_CAPACITY:
                {
                    size_t capacity = 0;

                    status = psa_get_generator_capacity( msg.rhandle,
                                                         &capacity );
                    if( status == PSA_SUCCESS )
                        psa_write( msg.handle, 0, &capacity,
                                   sizeof( capacity ) );

                    break;
                }
                case PSA_GENERATOR_READ:
                {
                    uint8_t *output = NULL;
                    size_t output_length = msg.out_size[0];

                    output = mbedtls_calloc( 1, output_length );
                    if( output == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    status = psa_generator_read( msg.rhandle,
                                                 output, output_length);
                    if( status == PSA_SUCCESS )
                        psa_write( msg.handle, 0, output, output_length );

                    mbedtls_free( output );
                    break;

                }
                case PSA_GENERATOR_IMPORT_KEY:
                {
                    psa_key_type_t type;
                    size_t bits;

                    bytes_read = psa_read( msg.handle, 1,
                                           &type, msg.in_size[1] );
                    if( bytes_read != sizeof( type ) )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    bytes_read = psa_read( msg.handle, 2,
                                           &bits, msg.in_size[2] );
                    if( bytes_read != sizeof( bits ) )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_generator_import_key( psa_crypto_ipc.key, type,
                                                       bits, msg.rhandle );
                    break;
                }
                case PSA_GENERATOR_ABORT:
                {
                    status = psa_generator_abort( msg.rhandle );
                    break;
                }
                case PSA_KEY_DERIVATION:
                {

                    uint8_t *salt = mbedtls_calloc( 1, msg.in_size[1] );
                    if ( salt == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        break;
                    }

                    bytes_read = psa_read( msg.handle, 1, salt,
                                           msg.in_size[1] );
                    if( bytes_read != msg.in_size[1] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    uint8_t *label = mbedtls_calloc( 1, msg.in_size[2] );
                    if ( label == NULL )
                    {
                        status = PSA_ERROR_INSUFFICIENT_MEMORY;
                        mbedtls_free( salt );
                        break;
                    }

                    bytes_read = psa_read( msg.handle, 2, label,
                                           msg.in_size[2] );
                    if( bytes_read != msg.in_size[2] )
                    {
                        SPM_PANIC("SPM read length mismatch");
                    }

                    status = psa_key_derivation( msg.rhandle, psa_crypto_ipc.key,
                                                 psa_crypto_ipc.alg,
                                                 salt,
                                                 msg.in_size[1],//salt length
                                                 label,
                                                 msg.in_size[2],//label length
                                                 psa_crypto_ipc.capacity );

                    break;
                }
                default:
                {
                    status = PSA_ERROR_NOT_SUPPORTED;
                    break;
                }
            }
            break;
        }
        case PSA_IPC_DISCONNECT:
        {
            psa_generator_abort( msg.rhandle );
            if( msg.rhandle != NULL )
                mbedtls_free( msg.rhandle );
            break;
        }
        default:
        {
            status = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }

    psa_reply( msg.handle, ( psa_error_t ) status );
}


void part_main(void *ptr)
{
    uint32_t signals = 0;
    psa_status_t status = PSA_SUCCESS;

    while (1) {
        signals = psa_wait_any( PSA_BLOCK );
        if( signals & PSA_CRYPTO_INIT )
        {
            psa_crypto_init_operation();
        }
        if( signals & PSA_MAC )
        {
            psa_mac_operation( );
        }
        if( signals & PSA_HASH )
        {
            psa_hash_operation( );
        }
        if( signals & PSA_SYMMETRIC )
        {
            psa_symmetric_operation();
        }
        if( signals & PSA_ASYMMETRIC )
        {
            psa_asymmetric_operation( );
        }
        if( signals & PSA_AEAD )
        {
            psa_aead_operation( );
        }
        if( signals & PSA_KEY_MNG )
        {
            psa_key_management_operation( );
        }
        if( signals & PSA_RNG )
        {
            psa_rng_operation( );
        }
        if( signals & PSA_CRYPTO_FREE )
        {
            psa_crypto_free_operation( );
        }
        if( signals & PSA_GENERATOR )
        {
            psa_crypto_generator_operations( );
        }
        if( signals & PSA_ENTROPY_INJECT )
        {
            psa_entropy_operation( );
        }
    }
}
