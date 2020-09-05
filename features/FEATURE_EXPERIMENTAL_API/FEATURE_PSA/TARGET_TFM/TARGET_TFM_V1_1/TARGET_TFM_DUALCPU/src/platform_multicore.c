/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
 * Copyright (c) 2019 Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "cmsis_compiler.h"

#include "platform_multicore.h"
#include "tfm_multi_core_api.h"
#include "tfm_ns_mailbox.h"

#include "cy_ipc_drv.h"
#include "cy_sysint.h"
#if CY_SYSTEM_CPU_CM0P
#include "spe_ipc_config.h"
#else
#include "ns_ipc_config.h"
#endif

int platform_mailbox_fetch_msg_ptr(void **msg_ptr)
{
    cy_en_ipcdrv_status_t status;

    if (!msg_ptr) {
        return PLATFORM_MAILBOX_INVAL_PARAMS;
    }

    status = Cy_IPC_Drv_ReadMsgPtr(Cy_IPC_Drv_GetIpcBaseAddress(IPC_RX_CHAN),
                                   msg_ptr);
    if (status != CY_IPC_DRV_SUCCESS) {
        return PLATFORM_MAILBOX_RX_ERROR;
    }

    Cy_IPC_Drv_ReleaseNotify(Cy_IPC_Drv_GetIpcBaseAddress(IPC_RX_CHAN),
                             IPC_RX_RELEASE_MASK);
    return PLATFORM_MAILBOX_SUCCESS;
}

int platform_mailbox_fetch_msg_data(uint32_t *data_ptr)
{
    cy_en_ipcdrv_status_t status;

    if (!data_ptr) {
        return PLATFORM_MAILBOX_INVAL_PARAMS;
    }

    status = Cy_IPC_Drv_ReadMsgWord(Cy_IPC_Drv_GetIpcBaseAddress(IPC_RX_CHAN),
                                    data_ptr);
    if (status != CY_IPC_DRV_SUCCESS) {
        return PLATFORM_MAILBOX_RX_ERROR;
    }

    Cy_IPC_Drv_ReleaseNotify(Cy_IPC_Drv_GetIpcBaseAddress(IPC_RX_CHAN),
                             IPC_RX_RELEASE_MASK);
    return PLATFORM_MAILBOX_SUCCESS;
}

int platform_mailbox_send_msg_ptr(const void *msg_ptr)
{
    cy_en_ipcdrv_status_t status;

    if (!msg_ptr)
        return PLATFORM_MAILBOX_INVAL_PARAMS;

    status = Cy_IPC_Drv_SendMsgPtr(Cy_IPC_Drv_GetIpcBaseAddress(IPC_TX_CHAN),
                                   IPC_TX_NOTIFY_MASK, msg_ptr);
    if (status != CY_IPC_DRV_SUCCESS) {
        return PLATFORM_MAILBOX_TX_ERROR;
    }

    return PLATFORM_MAILBOX_SUCCESS;
}

int platform_mailbox_send_msg_data(uint32_t data)
{
    cy_en_ipcdrv_status_t status;

    status = Cy_IPC_Drv_SendMsgWord(Cy_IPC_Drv_GetIpcBaseAddress(IPC_TX_CHAN),
                                    IPC_TX_NOTIFY_MASK, data);
    if (status != CY_IPC_DRV_SUCCESS) {
        return PLATFORM_MAILBOX_TX_ERROR;
    }

    return PLATFORM_MAILBOX_SUCCESS;
}

void platform_mailbox_wait_for_notify(void)
{
    uint32_t status;

    while (1) {
        status = Cy_IPC_Drv_GetInterruptStatusMasked(
                            Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT));
        status >>= CY_IPC_NOTIFY_SHIFT;
        if (status & IPC_RX_INT_MASK) {
            break;
        }
    }

    Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                              0, IPC_RX_INT_MASK);
}

int platform_ns_ipc_init(void)
{
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                                0, IPC_RX_INT_MASK);
    return PLATFORM_MAILBOX_SUCCESS;
}

int32_t tfm_platform_ns_wait_for_s_cpu_ready(void)
{
    uint32_t data = 0;

    if (platform_ns_ipc_init() != PLATFORM_MAILBOX_SUCCESS) {
        return PLATFORM_MAILBOX_INVAL_PARAMS;
    }
    while(data != IPC_SYNC_MAGIC)
    {
        platform_mailbox_wait_for_notify();
        platform_mailbox_fetch_msg_data(&data);
    }

    if (platform_mailbox_send_msg_data(~IPC_SYNC_MAGIC) !=
        PLATFORM_MAILBOX_SUCCESS) {
        return PLATFORM_MAILBOX_RX_ERROR;
    }
    return PLATFORM_MAILBOX_SUCCESS;
}
