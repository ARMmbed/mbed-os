/*
 *  PSA crypto layer on top of Mbed TLS crypto
 */
/*  Copyright (C) 2018, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include <stdlib.h>
#include <string.h>
#include "psa_psa_f_ifs.h"

#include "spm/psa_defs.h"
#include "spm/spm_client.h"

#include "crypto.h"
#include "crypto_platform_spe.h"

#if defined(MBEDTLS_FS_IO)
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#endif

#define MINOR_VER 1

psa_status_t psa_crypto_init(void)
{
    psa_error_t err_call;
    psa_handle_t handle = PSA_NULL_HANDLE;

    handle = psa_connect( PSA_CRYPTO_INIT_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, NULL, 0, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}

/****************************************************************/
/* PSA_MAC */
/****************************************************************/

psa_status_t psa_mac_abort( psa_mac_operation_t *operation )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    if( operation->handle <= 0 )
        return( PSA_SUCCESS );

    psa_crypto_ipc.func = PSA_MAC_ABORT;

    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    err_call = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}

static psa_status_t psa_mac_setup( psa_mac_operation_t *operation,
                                   psa_key_slot_t key,
                                   psa_algorithm_t alg,
                                   psa_sec_function_t func )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = func;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;

    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    operation->handle = psa_connect( PSA_MAC_ID, MINOR_VER );
    if( operation->handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    if( err < 0 )
        err = PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err );
}

psa_status_t psa_mac_sign_setup( psa_mac_operation_t *operation,
                                 psa_key_slot_t key,
                                 psa_algorithm_t alg )
{
    psa_status_t status = PSA_SUCCESS;

    status = psa_mac_setup( operation, key, alg, PSA_MAC_SIGN_SETUP );
    return status;
}

psa_status_t psa_mac_verify_setup( psa_mac_operation_t *operation,
                                   psa_key_slot_t key,
                                   psa_algorithm_t alg )
{
    psa_status_t status = PSA_SUCCESS;

    status = psa_mac_setup( operation, key, alg, PSA_MAC_VERIFY_SETUP );
    return status;
}

psa_status_t psa_mac_update( psa_mac_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_UPDATE;

    psa_invec_t in_vec[2] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                            { input, input_length } };

    err = psa_call( operation->handle, in_vec, 2, NULL, 0 );
    if( err < 0 )
        err = PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err );
}

psa_status_t psa_mac_sign_finish( psa_mac_operation_t *operation,
                                  uint8_t *mac,
                                  size_t mac_size,
                                  size_t *mac_length )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_SIGN_FINISH;

    psa_invec_t in_vec[2] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                            { &mac_size, sizeof( mac_size ) } };
    psa_outvec_t out_vec[2] = { { mac, mac_size }, { mac_length, sizeof( *mac_length ) } };

    err_call = psa_call( operation->handle, in_vec, 2, out_vec, 2 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}

psa_status_t psa_mac_verify_finish( psa_mac_operation_t *operation,
                                    const uint8_t *mac,
                                    size_t mac_length )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_MAC_VERIFY_FINISH;

    psa_invec_t in_vec[3] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                            { &mac_length, sizeof( mac_length ) },
                            { mac, mac_length } };

    err_call = psa_call( operation->handle, in_vec, 3, NULL , 0 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}


/****************************************************************/
/* PSA_HASH */
/****************************************************************/

psa_status_t psa_hash_abort( psa_hash_operation_t *operation )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    if( operation->handle <= 0 )
        return( PSA_SUCCESS );

    psa_crypto_ipc.func = PSA_HASH_ABORT;

    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    err_call = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}

psa_status_t psa_hash_setup( psa_hash_operation_t *operation,
                             psa_algorithm_t alg )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_SETUP;
    psa_crypto_ipc.alg = alg;

    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    operation->handle = psa_connect( PSA_HASH_ID, MINOR_VER );
    if( operation->handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    if( err < 0 )
        err = PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err );
}

psa_status_t psa_hash_update( psa_hash_operation_t *operation,
                              const uint8_t *input,
                              size_t input_length )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_UPDATE;

    psa_invec_t in_vec[2] = { {&psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                            { input, input_length } };

    err = psa_call( operation->handle, in_vec, 2, NULL, 0 );
    if( err < 0 )
        err = PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err );
}

psa_status_t psa_hash_finish( psa_hash_operation_t *operation,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_FINISH;

    psa_invec_t in_vec[2] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                            { &hash_size, sizeof( hash_size ) } };
    psa_outvec_t out_vec[2] = { { hash, hash_size },
                              { hash_length, sizeof( *hash_length ) } };

    err_call = psa_call( operation->handle, in_vec, 2, out_vec, 2 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}

psa_status_t psa_hash_verify( psa_hash_operation_t *operation,
                             const uint8_t *hash,
                             size_t hash_length )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };

    psa_crypto_ipc.func = PSA_HASH_VERIFY;

    psa_invec_t in_vec[3] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc_t ) },
                            { &hash_length, sizeof( hash_length ) },
                            { hash, hash_length } };

    err_call = psa_call( operation->handle, in_vec, 3, NULL, 0 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}


/****************************************************************/
/* AEAD */
/****************************************************************/
psa_status_t psa_aead_encrypt( psa_key_slot_t key,
                               psa_algorithm_t alg,
                               const uint8_t *nonce,
                               size_t nonce_length,
                               const uint8_t *additional_data,
                               size_t additional_data_length,
                               const uint8_t *plaintext,
                               size_t plaintext_length,
                               uint8_t *ciphertext,
                               size_t ciphertext_size,
                               size_t *ciphertext_length )
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t call_error = PSA_SUCCESS;
    psa_crypto_ipc_aead_t psa_crypto_ipc = { 0, 0, 0, 0, 0, 0, { 0 } };
    psa_invec_t in_vec[2];
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_AEAD_ENCRYPT;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.additional_data_length = additional_data_length;
    psa_crypto_ipc.input_length = plaintext_length;

    if( nonce_length > PSA_MAX_NONCE_SIZE )
        return( PSA_ERROR_INVALID_ARGUMENT );

    psa_crypto_ipc.nonce_size = nonce_length;
    memcpy( psa_crypto_ipc.nonce, nonce, nonce_length );

    uint8_t *buffer = calloc( 1, additional_data_length +
                                                plaintext_length );
    if( buffer == NULL )
        return( PSA_ERROR_INSUFFICIENT_MEMORY );

    memcpy( buffer, additional_data, additional_data_length );
    memcpy( buffer + additional_data_length, plaintext, plaintext_length );


    in_vec[0] = ( psa_invec_t ) { &psa_crypto_ipc,
                                             sizeof( psa_crypto_ipc_aead_t ) };

    in_vec[1] = ( psa_invec_t ) { buffer, additional_data_length +
                                                    plaintext_length };
    out_vec[0] = ( psa_outvec_t ) { ciphertext, ciphertext_size };
    out_vec[1] = ( psa_outvec_t ) { ciphertext_length, sizeof( *ciphertext_length ) };

    handle = psa_connect( PSA_AEAD_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    call_error = psa_call( handle, in_vec, 2, out_vec, 2 );

    psa_close( handle );

    if( call_error < 0 )
        call_error = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t )call_error );
}


psa_status_t psa_aead_decrypt( psa_key_slot_t key,
                               psa_algorithm_t alg,
                               const uint8_t *nonce,
                               size_t nonce_length,
                               const uint8_t *additional_data,
                               size_t additional_data_length,
                               const uint8_t *ciphertext,
                               size_t ciphertext_length,
                               uint8_t *plaintext,
                               size_t plaintext_size,
                               size_t *plaintext_length )
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t call_error = PSA_SUCCESS;
    psa_crypto_ipc_aead_t psa_crypto_ipc = { 0, 0, 0, 0, 0, 0, { 0 } };
    psa_invec_t in_vec[2];
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_AEAD_DECRYPT;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.additional_data_length = additional_data_length;
    psa_crypto_ipc.input_length = ciphertext_length;

    if( nonce_length > PSA_MAX_NONCE_SIZE )
        return( PSA_ERROR_INVALID_ARGUMENT );

    psa_crypto_ipc.nonce_size = nonce_length;
    memcpy( psa_crypto_ipc.nonce, nonce, nonce_length );

    uint8_t *buffer = calloc( 1, additional_data_length +
                                                    ciphertext_length );
    if( buffer == NULL )
        return( PSA_ERROR_INSUFFICIENT_MEMORY );

    memcpy( buffer, additional_data, additional_data_length );
    memcpy( buffer + additional_data_length, ciphertext, ciphertext_length );

    in_vec[0] = ( psa_invec_t ) { &psa_crypto_ipc,
                                  sizeof( psa_crypto_ipc_aead_t ) };
    in_vec[1] = ( psa_invec_t ) { buffer, additional_data_length +
                                                ciphertext_length };

    out_vec[0] = ( psa_outvec_t ) { plaintext, plaintext_size };
    out_vec[1] = ( psa_outvec_t ) { plaintext_length, sizeof( *plaintext_length ) };

    handle = psa_connect( PSA_AEAD_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    call_error = psa_call( handle, in_vec, 2, out_vec, 2 );

    psa_close( handle );

    if( call_error < 0 )
        call_error = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t )call_error );
}

/****************************************************************/
/* PSA_ASYMMETRIC */
/****************************************************************/

psa_status_t psa_asymmetric_sign(psa_key_slot_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *hash,
                                 size_t hash_length,
                                 uint8_t *signature,
                                 size_t signature_size,
                                 size_t *signature_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t call_error = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec_t in_vec[2];
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_ASYMMETRIC_SIGN;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;

    in_vec[0] = ( psa_invec_t ) { &psa_crypto_ipc,
                                  sizeof( psa_crypto_ipc_asymmetric_t ) };
    in_vec[1] = ( psa_invec_t ) { hash, hash_length };
    out_vec[0] = ( psa_outvec_t ) { signature, signature_size };
    out_vec[1] = ( psa_outvec_t ) { signature_length, sizeof( *signature_length ) };

    handle = psa_connect( PSA_ASYMMETRIC_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    call_error = psa_call( handle, in_vec, 2, out_vec, 2 );

    psa_close( handle );

    if( call_error < 0 )
        call_error = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t )call_error );
}

psa_status_t psa_asymmetric_verify(psa_key_slot_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *hash,
                                   size_t hash_length,
                                   const uint8_t *signature,
                                   size_t signature_size)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t call_error = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec_t in_vec[3];

    psa_crypto_ipc.func = PSA_ASYMMETRIC_VERIFY;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;

    in_vec[0] = ( psa_invec_t ) { &psa_crypto_ipc,
                                  sizeof( psa_crypto_ipc_asymmetric_t ) };
    in_vec[1] = ( psa_invec_t ) { signature, signature_size };
    in_vec[2] = ( psa_invec_t ) { hash, hash_length };
    handle = psa_connect( PSA_ASYMMETRIC_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );
    call_error = psa_call( handle, in_vec, 3, NULL, 0 );
    psa_close( handle );

    if( call_error < 0 )
        call_error = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t )call_error );
}

static psa_status_t psa_asymmetric_operation(psa_sec_function_t func,
                                             psa_key_slot_t key,
                                             psa_algorithm_t alg,
                                             const uint8_t *input,
                                             size_t input_length,
                                             const uint8_t *salt,
                                             size_t salt_length,
                                             uint8_t *output,
                                             size_t output_size,
                                             size_t *output_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_error_t call_error = PSA_SUCCESS;
    psa_crypto_ipc_asymmetric_t psa_crypto_ipc = { 0, 0, 0, 0, 0 };
    psa_invec_t in_vec[2];
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = func;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.input_length = input_length;
    psa_crypto_ipc.salt_length = salt_length;

    uint8_t *buffer = calloc( 1, input_length + salt_length );
    if( buffer == NULL )
        return( PSA_ERROR_INSUFFICIENT_MEMORY );

    memcpy(buffer, input, input_length);
    memcpy(buffer + input_length, salt, salt_length);

    in_vec[0] = ( psa_invec_t ) { &psa_crypto_ipc,
                                  sizeof( psa_crypto_ipc_asymmetric_t ) };
    in_vec[1] = ( psa_invec_t ) { buffer, input_length + salt_length };

    out_vec[0] = ( psa_outvec_t ) { output, output_size };
    out_vec[1] = ( psa_outvec_t ) { output_length, sizeof( *output_length ) };

    handle = psa_connect( PSA_ASYMMETRIC_ID, MINOR_VER );
    if( handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    call_error = psa_call( handle, in_vec, 2, out_vec, 2 );

    psa_close( handle );

    if( call_error < 0 )
        call_error = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t )call_error );
}

psa_status_t psa_asymmetric_encrypt(psa_key_slot_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status = psa_asymmetric_operation(PSA_ASYMMETRIC_ENCRYPT, key,
                                 alg, input, input_length,
                                 salt, salt_length, output,
                                 output_size, output_length );
    return status;
}

psa_status_t psa_asymmetric_decrypt(psa_key_slot_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length)
{
    psa_status_t status = psa_asymmetric_operation(PSA_ASYMMETRIC_DECRYPT, key,
                                 alg, input, input_length,
                                 salt, salt_length, output,
                                 output_size, output_length );
    return status;
}

/****************************************************************/
/* PSA_KEY_MANAGMENT */
/****************************************************************/

psa_status_t psa_get_key_lifetime( psa_key_slot_t key,
                                   psa_key_lifetime_t *lifetime )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_GET_KEY_LIFETIME;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[1] = {
         { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) } };
    psa_outvec_t out_vec[1] = { { lifetime, (lifetime == NULL ? 0 :
                                sizeof( *lifetime ) ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 1, out_vec, 1 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_set_key_lifetime( psa_key_slot_t key,
                                   psa_key_lifetime_t lifetime )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_SET_KEY_LIFETIME;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[2] = {
        { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) },
        { &lifetime, sizeof( psa_key_lifetime_t ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 2, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

/* the psa_key_policy_init psa_key_policy_get_usage,
psa_key_policy_get_algorithm and psa_key_policy_set_usage
accessor functions are implemented directly in the non-secure partition */
void psa_key_policy_init( psa_key_policy_t *policy )
{
    memset( policy, 0, sizeof( psa_key_policy_t ) );
}

psa_key_usage_t psa_key_policy_get_usage( const psa_key_policy_t *policy )
{
    return( policy->usage );
}

psa_algorithm_t psa_key_policy_get_algorithm( const psa_key_policy_t *policy )
{
    return( policy->alg );
}

void psa_key_policy_set_usage( psa_key_policy_t *policy,
                               psa_key_usage_t usage,
                               psa_algorithm_t alg )
{
    policy->usage = usage;
    policy->alg = alg;
}

psa_status_t psa_set_key_policy( psa_key_slot_t key,
                                const psa_key_policy_t *policy )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_SET_KEY_POLICY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[2] = {
        { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) },
        { policy, ( policy == NULL ? 0 : sizeof( *policy ) ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 2, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_get_key_policy( psa_key_slot_t key,
                                psa_key_policy_t *policy )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_GET_KEY_POLICY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[1] = { { &psa_key_mng_ipc,
                                sizeof( psa_key_mng_ipc_t ) } };
    psa_outvec_t out_vec[1] = { { policy, ( policy == NULL ? 0 :
                                 sizeof( *policy ) ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 1, out_vec, 1 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_import_key( psa_key_slot_t key,
                             psa_key_type_t type,
                             const uint8_t *data,
                             size_t data_length )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = type;
    psa_key_mng_ipc.func = PSA_IMPORT_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[2] = {
        { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) },
        { data, data_length  } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 2, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_destroy_key( psa_key_slot_t key )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_DESTROY_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[1] = {
        { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 1, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_get_key_information( psa_key_slot_t key,
                                      psa_key_type_t *type,
                                      size_t *bits )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = PSA_GET_KEY_INFORMATION;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[1] = { { &psa_key_mng_ipc,
                                sizeof( psa_key_mng_ipc_t ) } };
    psa_outvec_t out_vec[2] = { { type, ( type == NULL ? 0 :
                                 sizeof( psa_key_type_t ) ) } ,
                                { bits, ( bits == NULL ? 0 :
                                  sizeof( size_t ) ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 1, out_vec, 2 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

static psa_status_t psa_export_key_common( psa_key_slot_t key,
                                        uint8_t *data,
                                        size_t data_size,
                                        size_t *data_length,
                                        psa_sec_function_t func )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = 0;
    psa_key_mng_ipc.func = func;

    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[1] = { { &psa_key_mng_ipc, sizeof( psa_key_mng_ipc_t ) } };
    psa_outvec_t out_vec[2] = { { data, data_size } ,
                                { data_length, sizeof( *data_length ) } };

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 1, out_vec, 2 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_export_key( psa_key_slot_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length )
{
   psa_status_t status = psa_export_key_common( key, data, data_size,
                                                data_length , PSA_EXPORT_KEY );
   return status;
}

psa_status_t psa_export_public_key( psa_key_slot_t key,
                                    uint8_t *data,
                                    size_t data_size,
                                    size_t *data_length )
{
   psa_status_t status = psa_export_key_common( key, data, data_size,
                                                data_length,
                                                PSA_EXPORT_PUBLIC_KEY );
   return status;
}

psa_status_t psa_generate_key( psa_key_slot_t key,
                               psa_key_type_t type,
                               size_t bits,
                               const void *parameters,
                               size_t parameters_size )
{
    psa_error_t err_call;
    psa_key_mng_ipc_t psa_key_mng_ipc = { 0, 0, 0 };
    psa_key_mng_ipc.key = key;
    psa_key_mng_ipc.type = type;
    psa_key_mng_ipc.func = PSA_GENERATE_KEY;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec[3];
    in_vec[0].base = &psa_key_mng_ipc;
    in_vec[0].len = sizeof ( psa_key_mng_ipc_t );
    in_vec[1].base = &bits;
    in_vec[1].len = sizeof ( size_t );

    /* currently the parameter argument is used only for RSA keypair
       and ignored in other cases. support for other algorithms
       may be added later*/
    if ( PSA_KEY_TYPE_RSA_KEYPAIR == type )
    {
        in_vec[2].base = parameters;
        /* size of parameter is unsigned integer as defined in header */
        in_vec[2].len = parameters_size;
    }
    else // currenty ignored for non RSA case
    {
        in_vec[2].base = NULL;
        in_vec[2].len = 0;
    }

    handle = psa_connect( PSA_KEY_MNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, in_vec, 3, NULL, 0 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

/****************************************************************/
/* PSA_RNG */
/****************************************************************/

psa_status_t psa_generate_random( uint8_t *output,
                                  size_t output_size )
{
    psa_error_t err_call;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_outvec_t out_vec[1] = { { output, output_size } };

    handle = psa_connect( PSA_RNG_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, NULL, 0, out_vec, 1 );
    psa_close( handle );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

#if defined(MBEDTLS_ENTROPY_NV_SEED)
/****************************************************************/
/* PSA_ENTROPY_INJECT */
/****************************************************************/

psa_status_t mbedtls_psa_inject_entropy( const unsigned char *seed,
                                         size_t seed_size )
{
    psa_error_t err_call;
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_invec_t in_vec = { seed, seed_size };

    handle = psa_connect( PSA_ENTROPY_ID, MINOR_VER );
    if( handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( handle, &in_vec, 1, NULL, 0 );
    psa_close( handle );
    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );
}
#endif
/****************************************************************/
/* PSA Generator */
/****************************************************************/
psa_status_t psa_get_generator_capacity( const psa_crypto_generator_t *generator,
                                         size_t *capacity )
{
    psa_error_t err_call;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GET_GENERATOR_CAPACITY;
    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };
    psa_outvec_t out_vec = { capacity, sizeof( *capacity ) };

    err_call = psa_call( generator->handle, &in_vec, 1, &out_vec, 1 );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_generator_read( psa_crypto_generator_t *generator,
                                 uint8_t *output,
                                 size_t output_length )
{
    psa_error_t err_call;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GENERATOR_READ;
    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };
    psa_outvec_t out_vec = { output, output_length };
    err_call = psa_call( generator->handle, &in_vec, 1, &out_vec, 1 );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_generator_import_key( psa_key_slot_t key,
                                       psa_key_type_t type,
                                       size_t bits,
                                       psa_crypto_generator_t *generator )
{
    psa_error_t err_call;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.func = PSA_GENERATOR_IMPORT_KEY;
    psa_invec_t in_vec[3] = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                              { &type, sizeof( type ) },
                              { &bits, sizeof( bits ) } };

    err_call = psa_call( generator->handle, in_vec, 3, NULL, 0 );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_key_derivation( psa_crypto_generator_t *generator,
                                 psa_key_slot_t key,
                                 psa_algorithm_t alg,
                                 const uint8_t *salt,
                                 size_t salt_length,
                                 const uint8_t *label,
                                 size_t label_length,
                                 size_t capacity )
{
    psa_error_t err_call;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;
    psa_crypto_ipc.func = PSA_KEY_DERIVATION;
    psa_crypto_ipc.capacity = capacity;

    psa_invec_t in_vec[3]  = { { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) },
                               { salt, salt_length },
                               { label, label_length } };

    generator->handle = psa_connect( PSA_GENERATOR_ID, MINOR_VER );
    if( generator->handle <= 0 )
        return( PSA_ERROR_COMMUNICATION_FAILURE );

    err_call = psa_call( generator->handle, in_vec, 3, NULL, 0 );

    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );

}

psa_status_t psa_generator_abort( psa_crypto_generator_t *generator )
{
    psa_error_t err_call = PSA_SUCCESS;
    psa_crypto_derivation_ipc_t psa_crypto_ipc = { 0, 0, 0, 0 };
    psa_crypto_ipc.func = PSA_GENERATOR_ABORT;
    psa_invec_t in_vec = { &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };
    if( generator->handle != PSA_NULL_HANDLE )
    {
        err_call = psa_call( generator->handle, &in_vec, 1, NULL, 0 );
        psa_close( generator->handle );
        generator->handle = PSA_NULL_HANDLE;
    }
    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;

    return( ( psa_status_t ) err_call );

}

/****************************************************************/
/* PSA_SYMMETRIC */
/****************************************************************/

psa_status_t psa_cipher_encrypt_setup( psa_cipher_operation_t *operation,
                                       psa_key_slot_t key,
                                       psa_algorithm_t alg )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec;

    psa_crypto_ipc.func = PSA_CIPHER_ENCRYPT_SETUP;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;

    in_vec.base = &psa_crypto_ipc;
    in_vec.len = sizeof( psa_crypto_ipc );

    operation->handle = psa_connect( PSA_SYMMETRIC_ID, MINOR_VER );
    if( operation->handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    if( err < 0 )
        err = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err );
}

psa_status_t psa_cipher_decrypt_setup( psa_cipher_operation_t *operation,
                                       psa_key_slot_t key,
                                       psa_algorithm_t alg )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec;

    psa_crypto_ipc.func = PSA_CIPHER_DECRYPT_SETUP;
    psa_crypto_ipc.key = key;
    psa_crypto_ipc.alg = alg;

    in_vec = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    operation->handle = psa_connect( PSA_SYMMETRIC_ID, MINOR_VER );
    if( operation->handle <= 0 )
        return ( PSA_ERROR_COMMUNICATION_FAILURE );

    err = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    if( err < 0 )
        err = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err );
}

psa_status_t psa_cipher_generate_iv( psa_cipher_operation_t *operation,
                                     unsigned char *iv,
                                     size_t iv_size,
                                     size_t *iv_length )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec;
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_GENERATE_IV;

    in_vec = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    out_vec[0] = ( psa_outvec_t ){ iv, iv_size };
    out_vec[1] = ( psa_outvec_t ){ iv_length, sizeof( *iv_length ) };

    err = psa_call( operation->handle, &in_vec, 1, out_vec, 2 );
    if( err < 0 )
        err = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err );
}

psa_status_t psa_cipher_set_iv( psa_cipher_operation_t *operation,
                                const unsigned char *iv,
                                size_t iv_length )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_SET_IV;

    in_vec[0] = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };
    in_vec[1] = ( psa_invec_t ){ iv, iv_length };

    err = psa_call( operation->handle, in_vec, 2, NULL, 0 );
    if( err < 0 )
        err = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err );
}

psa_status_t psa_cipher_update( psa_cipher_operation_t *operation,
                                const uint8_t *input,
                                size_t input_length,
                                unsigned char *output,
                                size_t output_size,
                                size_t *output_length )
{
    psa_error_t err;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec[2];
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_UPDATE;

    in_vec[0] = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };
    in_vec[1] = ( psa_invec_t ){ input, input_length };

    out_vec[0] = ( psa_outvec_t ){ output, output_size };
    out_vec[1] = ( psa_outvec_t ){ output_length, ( output_length == NULL ? 0 :
                                  sizeof( *output_length ) ) };

    err = psa_call( operation->handle, in_vec, 2, out_vec, 2 );
    if( err < 0 )
        err = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err );
}

psa_status_t psa_cipher_finish( psa_cipher_operation_t *operation,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec;
    psa_outvec_t out_vec[2];

    psa_crypto_ipc.func = PSA_CIPHER_FINISH;

    in_vec = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    out_vec[0] = ( psa_outvec_t ){ output, output_size };
    out_vec[1] = ( psa_outvec_t ){ output_length, ( output_length == NULL ? 0 :
                                  sizeof( *output_length ) ) };

    err_call = psa_call( operation->handle, &in_vec, 1, out_vec, 2 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;
    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

psa_status_t psa_cipher_abort( psa_cipher_operation_t *operation )
{
    psa_error_t err_call;
    psa_crypto_ipc_t psa_crypto_ipc = { 0, 0, 0 };
    psa_invec_t in_vec;

    if( operation->handle <= 0 )
        return( PSA_SUCCESS );

    psa_crypto_ipc.func = PSA_CIPHER_ABORT;

    in_vec = ( psa_invec_t ){ &psa_crypto_ipc, sizeof( psa_crypto_ipc ) };

    err_call = psa_call( operation->handle, &in_vec, 1, NULL, 0 );
    psa_close( operation->handle );
    operation->handle = PSA_NULL_HANDLE;
    if( err_call < 0 )
        err_call = ( psa_error_t ) PSA_ERROR_COMMUNICATION_FAILURE;
    return( ( psa_status_t ) err_call );
}

void mbedtls_psa_crypto_free( void )
{
    psa_handle_t handle = PSA_NULL_HANDLE;

    //TODO: add retry mechanism to make sure resourecs were deallocated.
    handle = psa_connect( PSA_CRYPTO_FREE_ID, MINOR_VER );
    if( handle <= 0 )
        return;
    psa_call( handle, NULL, 0, NULL, 0 );
    psa_close( handle );
}
