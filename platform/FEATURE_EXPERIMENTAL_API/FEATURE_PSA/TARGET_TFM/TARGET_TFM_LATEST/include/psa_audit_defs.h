/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
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
