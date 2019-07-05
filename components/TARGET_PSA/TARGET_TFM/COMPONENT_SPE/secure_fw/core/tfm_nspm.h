/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSPM_H__
#define __TFM_NSPM_H__

#include <stdint.h>

/**
 * \brief initialise the NS context database
 */
void tfm_nspm_configure_clients(void);

/**
 * \brief Get the client ID of the current NS client
 *
 * \return The client id of the current NS client. 0 (invalid client id) is
 *         returned in case of error.
 */
int32_t tfm_nspm_get_current_client_id(void);

#ifdef TFM_PSA_API
/**
 * \brief NSPM thread main entry function
 *
 * \return  PSA_SUCCESS indicates failed.
 *
 * Note: This function should not return back.
 */
psa_status_t tfm_nspm_thread_entry(void);
#endif

#endif /* __TFM_NSPM_H__ */
