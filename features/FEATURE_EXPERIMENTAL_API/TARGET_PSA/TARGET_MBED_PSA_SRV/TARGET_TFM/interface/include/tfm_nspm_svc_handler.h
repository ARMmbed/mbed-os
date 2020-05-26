/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSPM_SVC_HANDLER_H__
#define __TFM_NSPM_SVC_HANDLER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Reports the client ID of this task to TF-M (SVC function)
 *
 * \param [in] client_id Client ID to register.
 *
 * \return Returns 1 if the client ID was successfully reported 0 otherwise
 */
uint32_t tfm_nspm_svc_register_client_id(uint32_t client_id);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NSPM_SVC_HANDLER_H__ */
