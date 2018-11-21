/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_audit_veneers.h"
#include "psa_audit_api.h"
#include "tfm_ns_lock.h"
#include "audit_wrappers.h"

enum psa_audit_err psa_audit_retrieve_record(const uint32_t record_index,
                                             const uint32_t buffer_size,
                                             const uint8_t *token,
                                             const uint32_t token_size,
                                             uint8_t *buffer,
                                             uint32_t *record_size)
{
    const struct audit_core_retrieve_input input_s =
                                       {.record_index = record_index,
                                        .buffer_size = buffer_size,
                                        .token = token,
                                        .token_size = token_size};

    struct audit_core_retrieve_output output_s = {.buffer = buffer,
                                                  .record_size = record_size};

    return tfm_ns_lock_dispatch((veneer_fn)tfm_audit_veneer_retrieve_record,
                                (uint32_t)&input_s,
                                (uint32_t)&output_s,
                                0,
                                0);
}

enum psa_audit_err psa_audit_get_info(uint32_t *num_records,
                                      uint32_t *size)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_audit_veneer_get_info,
                                (uint32_t)num_records,
                                (uint32_t)size,
                                0,
                                0);
}

enum psa_audit_err psa_audit_get_record_info(const uint32_t record_index,
                                             uint32_t *size)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_audit_veneer_get_record_info,
                                (uint32_t)record_index,
                                (uint32_t)size,
                                0,
                                0);
}

enum psa_audit_err psa_audit_delete_record(const uint32_t record_index,
                                           const uint8_t *token,
                                           const uint32_t token_size)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_audit_veneer_delete_record,
                                (uint32_t)record_index,
                                (uint32_t)token,
                                (uint32_t)token_size,
                                0);
}

enum psa_audit_err psa_audit_add_record(const struct psa_audit_record *record)
{
    /* This API supports only Secure world calls. As this is the implementation
     * of the Non-Secure interface, always directly return an error without
     * routing the call to TF-M in the Secure world
     */
    return PSA_AUDIT_ERR_NOT_SUPPORTED;
}
