/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "psa_attest_inject_key.h"
#include "psa_inject_attestation_key_impl.h"

psa_status_t
psa_attestation_inject_key(const uint8_t *key_data,
                           size_t key_data_length,
                           psa_key_type_t type,
                           uint8_t *public_key_data,
                           size_t public_key_data_size,
                           size_t *public_key_data_length)
{
    psa_status_t status = PSA_SUCCESS;
    status = psa_attestation_inject_key_impl(key_data,
                                             key_data_length,
                                             type,
                                             public_key_data,
                                             public_key_data_size,
                                             public_key_data_length);
    return (status);
}
