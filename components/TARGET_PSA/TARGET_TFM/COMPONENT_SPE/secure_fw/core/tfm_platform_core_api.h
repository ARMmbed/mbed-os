/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_CORE_API_H__
#define __TFM_PLATFORM_CORE_API_H__

/**
 * \brief Should be called in case of access violation.
 *
 * There might be platform specific means, by which it is possible on a
 * subsystem to detect access violation. For example a platform can have a
 * Peripheral Protection Controller, to detect unauthorised accesses to
 * peripheral registers. Setting up the protection, and handling the violation
 * is implemented in platform specific code. However TF-M should be able to
 * decide how to proceed if a violation happens. So to notify TF-M, platform
 * code have to call this function, if a violation happens.
 */
void tfm_access_violation_handler(void);

#endif /* __TFM_PLATFORM_CORE_API_H__ */
