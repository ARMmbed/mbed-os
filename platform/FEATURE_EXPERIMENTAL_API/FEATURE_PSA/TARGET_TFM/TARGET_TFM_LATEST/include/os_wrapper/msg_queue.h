/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_MSG_QUEUE_H__
#define __OS_WRAPPER_MSG_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "common.h"

/**
 * \brief Create and initialize a message queue
 *
 * \param[in] msg_size        The maximum message size in bytes
 * \param[in] msg_count       The maximum number of messages in queue
 *
 * \return Returns handle of the message queue created, or NULL in case of error
 */
void *os_wrapper_msg_queue_create(size_t msg_size, uint8_t msg_count);

/**
 * \brief Send a message via message queue
 *
 * \param[in] mq_handle       The handle of message queue
 * \param[in] msg_ptr         The pointer to the message to be sent
 *
 * \return \ref OS_WRAPPER_SUCCESS if the message is successfully sent, or
 *         \ref OS_WRAPPER_ERROR in case of error
 *
 * \note The message size must be the same as the value set in
 *       \ref os_wrapper_msg_queue_create.
 *
 * \note Time out value is not specified here. Whether the function is blocked
 *       or returns instantly depends on the actual implementation and usage
 *       scenario.
 */
int32_t os_wrapper_msg_queue_send(void *mq_handle,
                                  const void *msg_ptr);

/**
 * \brief Receive a message from message queue
 *
 * \param[in] mq_handle       The handle of message queue
 * \param[in] msg_ptr         The pointer to buffer for message to be received
 *
 * \return \ref OS_WRAPPER_SUCCESS if the message is successfully received, or
 *         \ref OS_WRAPPER_ERROR in case of error
 *
 * \note The message size is the same as the value set in
 *       \ref os_wrapper_msg_queue_create.
 *
 * \note The function should be blocked until a message is received from message
 *       queue, unless an error occurs.
 */
int32_t os_wrapper_msg_queue_receive(void *mq_handle,
                                     void *msg_ptr);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_MSG_QUEUE_H__ */
