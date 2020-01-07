/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_MUTEX_H__
#define __OS_WRAPPER_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * \brief Creates a mutex for mutual exclusion of resources
 *
 * \return The handle of the created mutex on success or NULL on error
 */
void *os_wrapper_mutex_create(void);

/**
 * \brief Acquires a mutex that is created by \ref os_wrapper_mutex_create()
 *
 * \param[in] handle   The handle of the mutex to acquire. Should be one of the
 *                     handles returned by \ref os_wrapper_mutex_create()
 * \param[in] timeout  The maximum amount of time(in tick periods) for the
 *                     thread to wait for the mutex to be available.
 *                     If timeout is zero, the function will return immediately.
 *                     Setting timeout to \ref OS_WRAPPER_WAIT_FOREVER will
 *                     cause the thread to wait indefinitely
 *
 * \return \ref OS_WRAPPER_SUCCESS on success or \ref OS_WRAPPER_ERROR on error
 */
uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout);

/**
 * \brief Releases the mutex acquired previously
 *

 * \param[in] handle The handle of the mutex that has been acquired
 *
 * \return \ref OS_WRAPPER_SUCCESS on success or \ref OS_WRAPPER_ERROR on error
 */
uint32_t os_wrapper_mutex_release(void *handle);

/**
 * \brief Deletes a mutex that is created by \ref os_wrapper_mutex_create()
 *
 * \param[in] handle The handle of the mutex to be deleted
 *
 * \return \ref OS_WRAPPER_SUCCESS on success or \ref OS_WRAPPER_ERROR on error
 */
uint32_t os_wrapper_mutex_delete(void *handle);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_MUTEX_H__ */
