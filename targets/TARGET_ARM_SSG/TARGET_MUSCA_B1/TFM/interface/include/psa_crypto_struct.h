/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/**
 * \file psa_crypto_struct.h
 *
 * \brief PSA cryptography module: structured type implementations
 *
 * \note This file may not be included directly. Applications must
 * include psa_crypto.h.
 *
 * This file contains the definitions of some data structures with
 * implementation-specific definitions.
 *
 * In implementations with isolation between the application and the
 * cryptography module, it is expected that the front-end and the back-end
 * would have different versions of this file.
 */

#ifndef PSA_CRYPTO_STRUCT_H
#define PSA_CRYPTO_STRUCT_H

struct psa_hash_operation_s
{
    uint32_t handle;
};

struct psa_mac_operation_s
{
    uint32_t handle;
};

struct psa_cipher_operation_s
{
    uint32_t handle;
};

struct psa_key_policy_s
{
    psa_key_usage_t usage;
    psa_algorithm_t alg;
};

#endif /* PSA_CRYPTO_STRUCT_H */
