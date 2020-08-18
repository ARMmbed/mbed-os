/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 * Copyright (c) 2019, Cypress Semiconductor Corporation. All rights reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _TFM_PLATFORM_MULTICORE_
#define _TFM_PLATFORM_MULTICORE_

#include <stdint.h>
#include "cy_device_headers.h"

#define IPC_PSA_CLIENT_CALL_CHAN         (8)
#define IPC_PSA_CLIENT_CALL_INTR_STRUCT  (6)
#define IPC_PSA_CLIENT_CALL_INTR_MASK    (1 << IPC_PSA_CLIENT_CALL_CHAN)
#define IPC_PSA_CLIENT_CALL_NOTIFY_MASK  (1 << IPC_PSA_CLIENT_CALL_INTR_STRUCT)
#define IPC_PSA_CLIENT_CALL_IPC_INTR     cpuss_interrupts_ipc_6_IRQn

#define IPC_PSA_CLIENT_REPLY_CHAN        (9)
#define IPC_PSA_CLIENT_REPLY_INTR_STRUCT (8)
#define IPC_PSA_CLIENT_REPLY_INTR_MASK   (1 << IPC_PSA_CLIENT_REPLY_CHAN)
#define IPC_PSA_CLIENT_REPLY_NOTIFY_MASK (1 << IPC_PSA_CLIENT_REPLY_INTR_STRUCT)
#define IPC_PSA_CLIENT_REPLY_IPC_INTR    cpuss_interrupts_ipc_8_IRQn

#define IPC_PSA_MAILBOX_LOCK_CHAN        (10)

#define IPC_RX_RELEASE_MASK              (0)

#define CY_IPC_NOTIFY_SHIFT              (16)

#define PSA_CLIENT_CALL_REQ_MAGIC        (0xA5CF50C6)
#define PSA_CLIENT_CALL_REPLY_MAGIC      (0xC605FC5A)

#define NS_MAILBOX_INIT_ENABLE           (0xAE)
#define S_MAILBOX_READY                  (0xC3)

#define PLATFORM_MAILBOX_SUCCESS         (0x0)
#define PLATFORM_MAILBOX_INVAL_PARAMS    (INT32_MIN + 1)
#define PLATFORM_MAILBOX_TX_ERROR        (INT32_MIN + 2)
#define PLATFORM_MAILBOX_RX_ERROR        (INT32_MIN + 3)
#define PLATFORM_MAILBOX_INIT_ERROR      (INT32_MIN + 4)

/* Inter-Processor Communication (IPC) data channel for the Semaphores */
#define PLATFORM_MAILBOX_IPC_CHAN_SEMA   CY_IPC_CHAN_SEMA
#define MAILBOX_SEMAPHORE_NUM            (16)

#define IPC_SYNC_MAGIC                   0x7DADE011

/**
 * \brief Fetch a pointer from mailbox message
 *
 * \param[out] msg_ptr     The address to write the pointer value to.
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_fetch_msg_ptr(void **msg_ptr);

/**
 * \brief Fetch a data value from mailbox message
 *
 * \param[out] data_ptr    The address to write the pointer value to.
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_fetch_msg_data(uint32_t *data_ptr);

/**
 * \brief Send a pointer via mailbox message
 *
 * \param[in] msg_ptr      The pointer value to be sent.
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_send_msg_ptr(const void *msg_ptr);

/**
 * \brief Send a data value via mailbox message
 *
 * \param[in] data         The data value to be sent
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_send_msg_data(uint32_t data);

/**
 * \brief Wait for a mailbox notify event.
 */
void platform_mailbox_wait_for_notify(void);

/**
 * \brief IPC initialization
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_ns_ipc_init(void);

#endif
