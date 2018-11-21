/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __AUDIT_WRAPPERS_H__
#define __AUDIT_WRAPPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct audit_core_retrieve_input
 *
 * \brief Input structure for the audit_core_retrieve_record_wrapper function
 *
 */
struct audit_core_retrieve_input {
    const uint32_t record_index; /*!< Index of the record to retrieve */
    const uint32_t buffer_size;  /*!< Size in bytes of the provided buffer */
    const uint8_t *token;        /*!< Must be set to NULL. Token used as a
                                  *   challenge for encryption, to protect
                                  *   against rollback attacks
                                  */
    const uint32_t token_size;   /*!< Must be set to 0. Size in bytes of the
                                  *   token used as challenge
                                  */
};

/*!
 * \struct audit_core_retrieve_output
 *
 * \brief Output structure for the audit_core_retrieve_record_wrapper function
 *
 */
struct audit_core_retrieve_output {
    uint8_t *buffer;       /*!< Buffer used to store the retrieved record */
    uint32_t *record_size; /*!< Size in bytes of the retrieved record */
};

/*!
 * \brief This function is a TF-M compatible wrapper for the
 *        \ref audit_core_retrieve_record implemented in the Audit log
 *        core functions
 *
 * \param[in]  input_s  Pointer to the structure containing input parameters
 *                      associated with \ref psa_audit_retrieve_record
 * \param[out] output_s Pointer to the structure containing output parameters
 *                      associated with \ref psa_audit_retrieve_record
 *
 */
enum psa_audit_err audit_core_retrieve_record_wrapper(
                                const struct audit_core_retrieve_input *input_s,
                                   struct audit_core_retrieve_output *output_s);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIT_WRAPPERS_H__ */
