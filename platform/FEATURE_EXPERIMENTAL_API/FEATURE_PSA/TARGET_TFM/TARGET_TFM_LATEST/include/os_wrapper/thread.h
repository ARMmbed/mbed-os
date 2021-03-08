/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_THREAD_H__
#define __OS_WRAPPER_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/* prototype for the thread entry function */
typedef void (*os_wrapper_thread_func) (void *argument);

/**
 * \brief Creates a new thread
 *
 * \param[in] name        Name of the thread
 * \param[in] stack_size  Size of stack to be allocated for this thread. It can
 *                        be \ref OS_WRAPPER_DEFAULT_STACK_SIZE to use the
 *                        default value provided by the underlying RTOS
 * \param[in] func        Pointer to the function invoked by thread
 * \param[in] arg         Argument to pass to the function invoked by thread
 * \param[in] priority    Initial thread priority
 *
 * \return Returns the thread handle created, or NULL in case of error
 */
void *os_wrapper_thread_new(const char *name, int32_t stack_size,
                            os_wrapper_thread_func func, void *arg,
                            uint32_t priority);
/**
 * \brief Gets current thread handle
 *
 * \return Returns the thread handle, or NULL in case of error
 */
void *os_wrapper_thread_get_handle(void);

/**
 * \brief Gets thread priority
 *
 * \param[in]  handle   Thread handle
 * \param[out] priority The priority of the thread
 *
 * \return Returns \ref OS_WRAPPER_SUCCESS on success, or \ref OS_WRAPPER_ERROR
 *                 in case of error
 */
uint32_t os_wrapper_thread_get_priority(void *handle, uint32_t *priority);

/**
 * \brief Exits the calling thread
 */
void os_wrapper_thread_exit(void);

/**
 * \brief Set the event flags for synchronizing a thread specified by handle.
 *
 * \note This function may not be allowed to be called from Interrupt Service
 *       Routines.
 *
 * \param[in] handle    Thread handle to be notified
 * \param[in] flags     Event flags value
 *
 * \return Returns \ref OS_WRAPPER_SUCCESS on success, or \ref OS_WRAPPER_ERROR
 *         in case of error
 */
uint32_t os_wrapper_thread_set_flag(void *handle, uint32_t flags);

/**
 * \brief Set the event flags in an interrupt handler for synchronizing a thread
 *        specified by handle.
 *
 * \param[in] handle    Thread handle to be notified
 * \param[in] flags     Event flags value
 *
 * \return Returns \ref OS_WRAPPER_SUCCESS on success, or \ref OS_WRAPPER_ERROR
 *         in case of error
 */
uint32_t os_wrapper_thread_set_flag_isr(void *handle, uint32_t flags);

/**
 * \brief Wait for the event flags for synchronizing threads.
 *
 * \note This function may not be allowed to be called from Interrupt Service
 *       Routines.
 *
 * \param[in] flags     Specify the flags to wait for
 * \param[in] timeout   Timeout value
 *
 * \return Returns \ref OS_WRAPPER_SUCCESS on success, or \ref OS_WRAPPER_ERROR
 *         in case of error
 */
uint32_t os_wrapper_thread_wait_flag(uint32_t flags, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_THREAD_H__ */
