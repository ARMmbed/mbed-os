/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "attestation.h"
#include "attestation_key.h"

/* Implementation of mandatory functions that used by TFM attestation code */
enum psa_attest_err_t
attest_get_and_register_initial_attestation_key(void) {
    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t
attest_check_memory_access(void *addr,
                           uint32_t size,
                           enum attest_memory_access_t access) {
    return PSA_ATTEST_ERR_SUCCESS;
}