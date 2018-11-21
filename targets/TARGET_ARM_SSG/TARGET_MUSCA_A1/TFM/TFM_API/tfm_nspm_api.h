/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSPM_API_H__
#define __TFM_NSPM_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Reports the client ID of this task to TF-M
 *
 * \return Returns 1 if the client ID was successfully reported 0 otherwise
 */
uint32_t tfm_nspm_register_client_id(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NSPM_API_H__ */
