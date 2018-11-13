#include "spm/psa_defs.h"

/**
 * \file psa/crypto_struct.h
 *
 * \brief PSA cryptography module: Mbed TLS structured type implementations
 */
/*
 *  Copyright (C) 2018, ARM Limited, All Rights Reserved
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

#ifndef PSA_CRYPTO_STRUCT_H
#define PSA_CRYPTO_STRUCT_H

struct psa_hash_operation_s
{
    psa_handle_t handle;
};

struct psa_mac_operation_s
{
    psa_handle_t handle;
};

struct psa_cipher_operation_s
{
    psa_handle_t handle;
};

struct psa_aead_operation_s
{
    psa_handle_t handle;
};

struct psa_crypto_generator_s
{
    psa_handle_t handle;
};

static inline struct psa_crypto_generator_s psa_crypto_generator_init( void )
{
    const struct psa_crypto_generator_s v = { 0 };
    return( v );
}

struct psa_key_policy_s
{
    psa_key_usage_t usage;
    psa_algorithm_t alg;
};

#define PSA_CRYPTO_GENERATOR_INIT { PSA_NULL_HANDLE }

#endif /* PSA_CRYPTO_STRUCT_H */
