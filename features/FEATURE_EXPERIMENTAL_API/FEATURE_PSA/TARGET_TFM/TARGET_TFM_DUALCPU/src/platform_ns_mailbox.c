/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
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
#include "cmsis_os2.h"

static uint8_t saved_irq_state = 1;

/* -------------------------------------- HAL API ------------------------------------ */

static void mailbox_ipc_init(void)
{
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                                0, IPC_RX_INT_MASK);
}

static void mailbox_ipc_config(void)
{
    NVIC_SetPriority(PSA_CLIENT_REPLY_NVIC_IRQn, PSA_CLIENT_REPLY_IRQ_PRIORITY);

    NVIC_EnableIRQ(PSA_CLIENT_REPLY_NVIC_IRQn);
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
    /* semaphore data */
    static uint32_t tfm_sema __attribute__((section("TFM_SHARED_DATA")));

    if (Cy_IPC_Sema_Init(PLATFORM_MAILBOX_IPC_CHAN_SEMA,
                         sizeof(tfm_sema) * CHAR_BIT,
                         &tfm_sema) != CY_IPC_SEMA_SUCCESS) {
        return PLATFORM_MAILBOX_INIT_ERROR;
    }
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

    mailbox_ipc_config();

    return MAILBOX_SUCCESS;
}

const void *tfm_ns_mailbox_get_task_handle(void)
{
#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
    return osThreadGetId();
#else
    return NULL;
#endif
}

void tfm_ns_mailbox_hal_wait_reply(mailbox_msg_handle_t handle)
{
    osThreadFlagsWait(handle, osFlagsWaitAll, osWaitForever);
}

static cy_en_ipcsema_status_t mailbox_raw_spin_lock(uint32_t ipc_channel,
                                                    uint32_t sema_num)
{
    uint32_t semaIndex;
    uint32_t semaMask;
    cy_stc_ipc_sema_t *semaStruct;
    cy_en_ipcdrv_status_t acqStatus;
    cy_en_ipcsema_status_t ret = CY_IPC_SEMA_BAD_PARAM;
    bool is_lock = false;
    IPC_STRUCT_Type *semaIpcStruct;

    /* Get IPC register structure */
    semaIpcStruct = Cy_IPC_Drv_GetIpcBaseAddress(ipc_channel);
    /* Get pointer to structure */
    semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(semaIpcStruct);

    if (sema_num < semaStruct->maxSema) {
        semaIndex = sema_num / CY_IPC_SEMA_PER_WORD;
        semaMask = (uint32_t)(1ul << (sema_num - \
                                      (semaIndex * CY_IPC_SEMA_PER_WORD)));

        while (!is_lock) {
            /* Check to make sure the IPC channel is released
               If so, check if specific channel can be locked. */
            do {
                acqStatus = Cy_IPC_Drv_LockAcquire(semaIpcStruct);
            } while (acqStatus != CY_IPC_DRV_SUCCESS);

            if ((semaStruct->arrayPtr[semaIndex] & semaMask) == 0ul) {
                semaStruct->arrayPtr[semaIndex] |= semaMask;
                is_lock = true;
            }

            /* Release, but do not trigger a release event */
            (void)Cy_IPC_Drv_LockRelease(semaIpcStruct,
                                         CY_IPC_NO_NOTIFICATION);

            if (!is_lock) {
                /*
                 * The secure core is occupying this lock. Insert a small delay
                 * to give the secure core a chance to acquire the IPC channel
                 * and release the lock.
                 * Otherwise, the secure core may not be able to release the
                 * lock if non-secure core has higher CPU frequency. It will
                 * generate a deadlock.
                 * This delay won't harm performance too much since non-secure
                 * core has to busy wait here anyway.
                 * Alternatively, non-secure core can wait for release
                 * notification event from secure core. However, it is more
                 * complex and requires more code and more modifications.
                 */
                volatile uint32_t count = 1000;
                while(count > 0) {
                    count--;
                }
                Cy_IPC_Sema_Status(sema_num);
            }
        }

        ret = CY_IPC_SEMA_SUCCESS;
    }

    return ret;
}

static cy_en_ipcsema_status_t mailbox_raw_spin_unlock(uint32_t ipc_channel,
                                                      uint32_t sema_num)
{
    uint32_t semaIndex;
    uint32_t semaMask;
    cy_stc_ipc_sema_t *semaStruct;
    cy_en_ipcdrv_status_t acqStatus;
    cy_en_ipcsema_status_t ret = CY_IPC_SEMA_BAD_PARAM;
    bool is_unlock = false;
    IPC_STRUCT_Type *semaIpcStruct;

    /* Get IPC register structure */
    semaIpcStruct = Cy_IPC_Drv_GetIpcBaseAddress(ipc_channel);
    /* Get pointer to structure */
    semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(semaIpcStruct);

    if (sema_num < semaStruct->maxSema) {
        semaIndex = sema_num / CY_IPC_SEMA_PER_WORD;
        semaMask = (uint32_t)(1ul << (sema_num - \
                                      (semaIndex * CY_IPC_SEMA_PER_WORD)));

        while (!is_unlock) {
            /* Check to make sure the IPC channel is released
               If so, check if specific channel can be locked. */
            do {
                acqStatus = Cy_IPC_Drv_LockAcquire(semaIpcStruct);
            } while (acqStatus != CY_IPC_DRV_SUCCESS);

            if ((semaStruct->arrayPtr[semaIndex] & semaMask) != 0ul) {
                semaStruct->arrayPtr[semaIndex] &= ~semaMask;
                is_unlock = true;
            }

            /* Release, but do not trigger a release event */
            (void)Cy_IPC_Drv_LockRelease(semaIpcStruct,
                                         CY_IPC_NO_NOTIFICATION);
        }

        ret = CY_IPC_SEMA_SUCCESS;
    }

    return ret;
}

void tfm_ns_mailbox_hal_enter_critical(void)
{
    saved_irq_state = Cy_SysLib_EnterCriticalSection();

    mailbox_raw_spin_lock(CY_IPC_CHAN_SEMA, MAILBOX_SEMAPHORE_NUM);
}

void tfm_ns_mailbox_hal_exit_critical(void)
{
    mailbox_raw_spin_unlock(CY_IPC_CHAN_SEMA, MAILBOX_SEMAPHORE_NUM);

    Cy_SysLib_ExitCriticalSection(saved_irq_state);
}

void tfm_ns_mailbox_hal_enter_critical_isr(void)
{
    mailbox_raw_spin_lock(CY_IPC_CHAN_SEMA, MAILBOX_SEMAPHORE_NUM);
}

void tfm_ns_mailbox_hal_exit_critical_isr(void)
{
    mailbox_raw_spin_unlock(CY_IPC_CHAN_SEMA, MAILBOX_SEMAPHORE_NUM);
}

static bool mailbox_clear_intr(void)
{
    uint32_t status;

    status = Cy_IPC_Drv_GetInterruptStatusMasked(
                            Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT));
    status >>= CY_IPC_NOTIFY_SHIFT;
    if ((status & IPC_RX_INT_MASK) == 0) {
        return false;
    }

    Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                              0, IPC_RX_INT_MASK);
    return true;
}

void cpuss_interrupts_ipc_5_IRQHandler(void)
{
    uint32_t magic;
    mailbox_msg_handle_t handle;
    osThreadId_t task_handle;

    if (!mailbox_clear_intr())
        return;

    platform_mailbox_fetch_msg_data(&magic);
    if (magic == PSA_CLIENT_CALL_REPLY_MAGIC) {
        /* Handle all the pending replies */
        while (1) {
            handle = tfm_ns_mailbox_fetch_reply_msg_isr();
            if (handle == MAILBOX_MSG_NULL_HANDLE) {
                break;
            }

            task_handle = (osThreadId_t)tfm_ns_mailbox_get_msg_owner(handle);
            if (task_handle) {
                /* According to the description of CMSIS-RTOS v2 Thread Flags,
                 * osThreadFlagsSet() can be called inside Interrupt Service
                 * Routine. */
                osThreadFlagsSet(task_handle, handle);
            }
        }
    }
}
