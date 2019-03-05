/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPM_SERVICES_API_H__
#define __TFM_SPM_SERVICES_API_H__

enum tfm_spm_request_type_t {
    TFM_SPM_REQUEST_RESET_VOTE,
};

/**
 * \brief Request a vote from SPM on a system reset
 *
 * \return Returns 0 if request is accepted, any other value means reject
 */
int32_t tfm_spm_request_reset_vote(void);

#endif /* __TFM_SPM_SERVICES_API_H__ */
