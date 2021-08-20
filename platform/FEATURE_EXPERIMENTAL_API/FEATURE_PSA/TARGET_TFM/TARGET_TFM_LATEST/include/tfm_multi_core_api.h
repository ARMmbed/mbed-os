/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_MULTI_CORE_API__
#define __TFM_MULTI_CORE_API__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief Called on the non-secure CPU.
 *        Flags that the non-secure side has completed its initialization.
 *        Waits, if necessary, for the secure CPU to flag that it has completed
 *        its initialization.
 *
 * \return Return 0 if succeeds.
 * \return Otherwise, return specific error code.
 */
int32_t tfm_ns_wait_for_s_cpu_ready(void);

/**
 * \brief Synchronisation with secure CPU, platform-specific implementation.
 *        Flags that the non-secure side has completed its initialization.
 *        Waits, if necessary, for the secure CPU to flag that it has completed
 *        its initialization.
 *
 * \retval Return 0 if succeeds.
 * \retval Otherwise, return specific error code.
 */
int32_t tfm_platform_ns_wait_for_s_cpu_ready(void);

/**
 * \brief Acquire the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the use scenario.
 *
 * \return \ref TFM_SUCCESS on success
 * \return \ref TFM_ERROR_GENERIC on error
 */
uint32_t tfm_ns_multi_core_lock_acquire(void);

/**
 * \brief Release the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the use scenario.
 *
 * \return \ref TFM_SUCCESS on success
 * \return \ref TFM_ERROR_GENERIC on error
 */
uint32_t tfm_ns_multi_core_lock_release(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_MULTI_CORE_API__ */
