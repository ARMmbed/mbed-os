/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* API definitions in NSPE mailbox library */

#ifndef TFM_NS_MAILBOX_H
#define TFM_NS_MAILBOX_H

#include <stdbool.h>
#include "tfm_mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Prepare and send PSA client request to SPE via mailbox.
 *
 * \param[in] call_type         PSA client call type
 * \param[in] params            Parmaters used for PSA client call
 * \param[in] client_id         Optional client ID of non-secure caller.
 *                              It is required to identify the non-secure caller
 *                              when NSPE OS enforces non-secure task isolation.
 *
 * \retval >= 0                 The handle to the mailbox message assigned.
 * \retval < 0                  Operation failed with an error code.
 */
mailbox_msg_handle_t tfm_ns_mailbox_tx_client_req(uint32_t call_type,
                                       const struct psa_client_params_t *params,
                                       int32_t client_id);

/**
 * \brief Fetch PSA client return result.
 *
 * \param[in] handle            The handle to the mailbox message
 * \param[out] reply            The address to be written with return result.
 *
 * \retval MAILBOX_SUCCESS      Successfully get PSA client call return result.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_rx_client_reply(mailbox_msg_handle_t handle,
                                       int32_t *reply);

/**
 * \brief Check whether a specific mailbox message has been replied.
 *
 * \param[in] handle            The handle to the mailbox message
 *
 * \retval true                 The PSA client call return value is replied.
 * \retval false                The PSA client call return value is not
 *                              replied yet.
 */
bool tfm_ns_mailbox_is_msg_replied(mailbox_msg_handle_t handle);

/**
 * \brief NSPE mailbox initialization
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Platform specific NSPE mailbox initialization.
 *        Invoked by \ref tfm_ns_mailbox_init().
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Notify SPE to deal with the PSA client call sent via mailbox
 *
 * \note The implementation depends on platform specific hardware and use case.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_notify_peer(void);

/**
 * \brief Enter critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical(void);

/**
 * \brief Exit critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical(void);

#ifdef __cplusplus
}
#endif

#endif
