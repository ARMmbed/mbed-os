/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Data types and API definitions in NSPE mailbox library */

#ifndef __TFM_NS_MAILBOX_H__
#define __TFM_NS_MAILBOX_H__

#include <stdbool.h>
#include <stdint.h>
#include "tfm_mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_MULTI_CORE_TEST
/**
 * \brief The structure to hold the statistics result of NSPE mailbox
 */
struct ns_mailbox_stats_res_t {
    uint8_t avg_nr_slots;               /* The value before the decimal point
                                         * in the average number of NSPE
                                         * mailbox slots in use.
                                         */
    uint8_t avg_nr_slots_tenths;        /* The first digit value after the
                                         * decimal point in the average
                                         * number of NSPE mailbox slots in use.
                                         */
};
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

#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
/**
 * \brief Get the handle of the current non-secure task executing mailbox
 *        functionalities
 *
 * \note This function should be implemented according to platform, NS OS
 *       and actual use scenario.
 *       This function can be ignored or return NULL if sleep/wake-up mechanism
 *       is not required in PSA Client API implementation.
 *
 * \return Return the handle of task.
 */
const void *tfm_ns_mailbox_get_task_handle(void);
#else
static inline const void *tfm_ns_mailbox_get_task_handle(void)
{
    return NULL;
}
#endif

/**
 * \brief Fetch the handle to the first replied mailbox message in the NSPE
 *        mailbox queue.
 *        This function is intended to be called inside platform specific
 *        notification IRQ handler.
 *
 * \note The replied status of the fetched mailbox message will be cleaned after
 *       the message is fetched. When this function is called again, it fetches
 *       the next replied mailbox message from the NSPE mailbox queue.
 *
 * \return Return the handle to the first replied mailbox message in the
 *         queue.
 *         Return \ref MAILBOX_MSG_NULL_HANDLE if no mailbox message is replied.
 */
mailbox_msg_handle_t tfm_ns_mailbox_fetch_reply_msg_isr(void);

/**
 * \brief Return the handle of owner task of a mailbox message according to the
 *        \ref mailbox_msg_handle_t
 *
 * \param[in] handle            The handle of mailbox message.
 *
 * \return Return the handle value of the owner task.
 */
const void *tfm_ns_mailbox_get_msg_owner(mailbox_msg_handle_t handle);

#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
/**
 * \brief Wait for the reply returned from SPE to the mailbox message specified
 *        by handle
 *
 * \param[in] handle            The handle of mailbox message.
 *
 * \retval MAILBOX_SUCCESS      Return from waiting successfully.
 * \retval Other return code    Failed to wait with an error code.
 */
int32_t tfm_ns_mailbox_wait_reply(mailbox_msg_handle_t handle);
#endif

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

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical_isr(void);

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical_isr(void);

#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
/**
 * \brief Performs platform and NS OS specific waiting mechanism to wait for
 *        the reply of the specified mailbox message to be returned from SPE.
 *
 * \note This function is implemented by platform and NS OS specific waiting
 *       mechanism accroding to use scenario.
 *
 * \param[in] handle            The handle of mailbox message.
 */
void tfm_ns_mailbox_hal_wait_reply(mailbox_msg_handle_t handle);
#endif

#ifdef TFM_MULTI_CORE_TEST
/**
 * \brief Initialize the statistics module in TF-M NSPE mailbox.
 *
 * \note This function is only available when multi-core tests are enabled.
 */
void tfm_ns_mailbox_tx_stats_init(void);

/**
 * \brief Calculate the average number of used NS mailbox queue slots each time
 *        NS task requires a queue slot to submit mailbox message, which is
 *        recorded in NS mailbox statisitics module.
 *
 * \note This function is only available when multi-core tests are enabled.
 *
 * \param[in] stats_res         The buffer to be written with
 *                              \ref ns_mailbox_stats_res_t.
 *
 * \return Return the calculation result.
 */
void tfm_ns_mailbox_stats_avg_slot(struct ns_mailbox_stats_res_t *stats_res);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_MAILBOX_H__ */
