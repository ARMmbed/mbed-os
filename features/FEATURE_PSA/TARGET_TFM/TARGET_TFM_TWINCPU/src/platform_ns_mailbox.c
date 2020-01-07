/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 * Copyright (c) 2019, Cypress Semiconductor Corporation. All rights reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* -------------------------------------- Includes ----------------------------------- */
#include <limits.h>
#include <string.h>

#include "cmsis_compiler.h"

#include "cy_ipc_drv.h"
#include "cy_sysint.h"
#include "cy_ipc_sema.h"

#include "ns_ipc_config.h"
#include "tfm_ns_mailbox.h"
#include "platform_multicore.h"

/* -------------------------------------- HAL API ------------------------------------ */

static void mailbox_ipc_init(void)
{
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                                0, IPC_RX_INT_MASK);
}

int32_t tfm_ns_mailbox_hal_notify_peer(void)
{
    cy_en_ipcdrv_status_t status;

    status = Cy_IPC_Drv_SendMsgWord(Cy_IPC_Drv_GetIpcBaseAddress(IPC_TX_CHAN),
                                    IPC_TX_NOTIFY_MASK,
                                    PSA_CLIENT_CALL_REQ_MAGIC);

    if (status == CY_IPC_DRV_SUCCESS) {
        return MAILBOX_SUCCESS;
    } else {
        return MAILBOX_CHAN_BUSY;
    }
}

static int32_t mailbox_sema_init(void)
{
#if defined(CY_IPC_DEFAULT_CFG_DISABLE)
    /* semaphore data */
    static uint32_t tfm_sema;

    if (Cy_IPC_Sema_Init(PLATFORM_MAILBOX_IPC_CHAN_SEMA,
                         sizeof(tfm_sema) * CHAR_BIT,
                         &tfm_sema) != CY_IPC_SEMA_SUCCESS) {
        return PLATFORM_MAILBOX_INIT_ERROR;
    }
#endif
    return PLATFORM_MAILBOX_SUCCESS;
}

int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue)
{
    uint32_t stage;

    if (!queue) {
        return MAILBOX_INVAL_PARAMS;
    }

    /* Init semaphores used for critical sections */
    if (mailbox_sema_init() != PLATFORM_MAILBOX_SUCCESS)
        return MAILBOX_INIT_ERROR;

    /*
     * FIXME
     * Further verification of mailbox queue address may be required according
     * to diverse NSPE implementations.
     */

    mailbox_ipc_init();

    /*
     * Wait until SPE mailbox library is ready to receive NSPE mailbox queue
     * address.
     */
    while (1) {
        platform_mailbox_wait_for_notify();

        platform_mailbox_fetch_msg_data(&stage);
        if  (stage == NS_MAILBOX_INIT_ENABLE) {
            break;
        }
    }

    /* Send out the address */
    platform_mailbox_send_msg_ptr(queue);

    /* Wait until SPE mailbox service is ready */
    while (1) {
        platform_mailbox_wait_for_notify();

        platform_mailbox_fetch_msg_data(&stage);
        if  (stage == S_MAILBOX_READY) {
            break;
        }
    }

    return MAILBOX_SUCCESS;
}

void tfm_ns_mailbox_hal_enter_critical(void)
{
    while (Cy_IPC_Sema_Set(MAILBOX_SEMAPHORE_NUM, false) !=
           CY_IPC_SEMA_SUCCESS) {
    }
}

void tfm_ns_mailbox_hal_exit_critical(void)
{
    while (Cy_IPC_Sema_Clear(MAILBOX_SEMAPHORE_NUM, false) !=
           CY_IPC_SEMA_SUCCESS) {
    }
}
