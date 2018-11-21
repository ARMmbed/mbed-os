/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_AUDIT_DEFS_H__
#define __PSA_AUDIT_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_api.h"
#include "limits.h"

/* The return value is shared with the TFM partition status value. The Audit
 * Log return codes shouldn't overlap with predefined TFM status values
 */
#define PSA_AUDIT_ERR_OFFSET (TFM_PARTITION_SPECIFIC_ERROR_MIN)

enum psa_audit_err {
    PSA_AUDIT_ERR_SUCCESS = 0,                    /*!< Operation successful */
    PSA_AUDIT_ERR_FAILURE = PSA_AUDIT_ERR_OFFSET, /*!< Generic failure */
    PSA_AUDIT_ERR_NOT_SUPPORTED,                  /*!< Operation not supported */

    /* Following entry is only to ensure the error code of int size */
    PSA_AUDIT_ERR_FORCE_INT_SIZE = INT_MAX
};

/*!
 * \struct psa_audit_record
 *
 * \brief This structure contains the record that is added to the audit log
 *        by the requesting secure service
 */
struct psa_audit_record {
    uint32_t size;      /*!< Size in bytes of the id and payload fields */
    uint32_t id;        /*!< ID of the record */
    uint8_t  payload[]; /*!< Flexible array member for payload */
};

#ifdef __cplusplus
}
#endif

#endif /* __PSA_AUDIT_DEFS_H__ */
