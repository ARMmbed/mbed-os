/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_SEMAPHORE_H__
#define __OS_WRAPPER_SEMAPHORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * \brief Creates a new semaphore
 *
 * \param[in] max_count       Highest count of the semaphore
 * \param[in] initial_count   Starting count of the available semaphore
 * \param[in] name            Name of the semaphore
 *
 * \return Returns handle of the semaphore created, or NULL in case of error
 */
void *os_wrapper_semaphore_create(uint32_t max_count, uint32_t initial_count,
                                  const char *name);

/**
 * \brief Acquires the semaphore
 *
 * \param[in] hanlde  Semaphore handle
 * \param[in] timeout Timeout value
 *
 * \return \ref OS_WRAPPER_SUCCESS in case of successful acquision, or
 *         \ref OS_WRAPPER_ERROR in case of error
 */
uint32_t os_wrapper_semaphore_acquire(void *handle, uint32_t timeout);

/**
 * \brief Releases the semaphore
 *
 * \param[in] hanlde Semaphore handle
 *
 * \return \ref OS_WRAPPER_SUCCESS in case of successful release, or
 *         \ref OS_WRAPPER_ERROR in case of error
 */
uint32_t os_wrapper_semaphore_release(void *handle);

/**
 * \brief Deletes the semaphore
 *
 * \param[in] handle Semaphore handle
 *
 * \return \ref OS_WRAPPER_SUCCESS in case of successful release, or
 *         \ref OS_WRAPPER_ERROR in case of error
 */
uint32_t os_wrapper_semaphore_delete(void *handle);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_SEMAPHORE_H__ */
