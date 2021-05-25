/***************************************************************************//**
* \file cy_ipc_bt.c
* \version 1.0
*
* \brief
*  This driver provides the source code for BT IPC.
*
********************************************************************************
* \copyright
* Copyright 2017-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXIPC)

#include "cy_ipc_bt.h"

/* Should not include this. To be removed */
#include <string.h>

/* local functions prototype */
cy_en_btipcdrv_status_t Cy_bt_handle_hpclong_msg(cy_stc_ipc_bt_context_t *btIpcContext, uint32_t * msgPtr);
cy_en_btipcdrv_status_t Cy_bt_handle_buf_add(cy_stc_ipc_bt_context_t *btIpcContext, uint32_t * msgPtr);
cy_en_btipc_buftype_t Cy_bt_get_buf_type(cy_en_btipc_hcipti_t pti);
uint32_t Cy_bt_getPLLegnth(cy_en_btipc_hcipti_t pti, uint8_t* bufAddr);
bool Cy_bt_isOffsetNeeded(cy_en_btipc_hcipti_t pti);




void Cy_BTIPC_IRQ_Handler(cy_stc_ipc_bt_context_t *btIpcContext)
{
    uint32_t shadowIntr;
    IPC_STRUCT_Type *ipcPtr;
    IPC_INTR_STRUCT_Type *ipcIntrPtr;
    uint32_t mesg[2];
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint32_t notify;
    uint32_t release;
    uint32_t channel;
    uint8_t idx;

    if (NULL == contextPtr)
        return;

    ipcIntrPtr = Cy_IPC_Drv_GetIntrBaseAddr(contextPtr->intStuctureSelf);
    shadowIntr = Cy_IPC_Drv_GetInterruptStatusMasked(ipcIntrPtr);

    /* Check to make sure the interrupt was a release interrupt */
    release = Cy_IPC_Drv_ExtractReleaseMask(shadowIntr);

    /* First process the release callback */
    if (0UL != release)  /* Check for a Release interrupt */
    {
        /* Clear the release interrupt  */
        Cy_IPC_Drv_ClearInterrupt(ipcIntrPtr, release, CY_IPC_NO_NOTIFICATION);
        /* release callback can be added here. */
        if (contextPtr->ulReleaseCallbackPtr)
            contextPtr->ulReleaseCallbackPtr();
    }

    /* Check to make sure the interrupt was a notify interrupt */
    notify = Cy_IPC_Drv_ExtractAcquireMask(shadowIntr);

    if (0UL != notify)
    {
        /* Clear the notify interrupt.  */
        Cy_IPC_Drv_ClearInterrupt(ipcIntrPtr, CY_IPC_NO_NOTIFICATION, notify);
        if (notify & (uint32_t)(0x1 << contextPtr->dlChannelHCI))
            channel = contextPtr->dlChannelHCI;
        else
            channel = contextPtr->dlChannelHPC;

        ipcPtr = Cy_IPC_Drv_GetIpcBaseAddress(channel);

        if (!Cy_IPC_Drv_ReadMsgDWord (ipcPtr, mesg))
        {
            if (channel == contextPtr->dlChannelHCI)
            {
                /* Channel will be released in the callback function */
                if (contextPtr->dlNotifyCallbackPtr)
                    contextPtr->dlNotifyCallbackPtr(mesg);
            }
            else
            {
                if (contextPtr->internal_hpc_notify_cb)
                    contextPtr->internal_hpc_notify_cb((void*)contextPtr, mesg);
                for (idx = 0; idx < MAX_BT_IPC_HPC_CB; idx++)
                {
                    if (contextPtr->hpcNotifyCallbackPtr[idx])
                        contextPtr->hpcNotifyCallbackPtr[idx](mesg);
                }
                /* Release channel for HPC message only */
                Cy_BTIPC_HPC_RelChannel(contextPtr, mesg);
            }
        }
    }
}

cy_en_btipcdrv_status_t Cy_BTIPC_HCI_RelChannel(cy_stc_ipc_bt_context_t *btIpcContext, void * buf)
{
    IPC_STRUCT_Type *ipcPtr;
    cy_en_ipcdrv_status_t status;
    uint32_t rel_mask;
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;

    if ((NULL == contextPtr) || (NULL == buf))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    ipcPtr = Cy_IPC_Drv_GetIpcBaseAddress (contextPtr->dlChannelHCI);

    rel_mask = (uint32_t)(1 << contextPtr->intStucturePeer);

    status = Cy_IPC_Drv_LockRelease (ipcPtr, rel_mask);

    if (status) {
        return CY_BT_IPC_DRV_ERROR_LOCK_REL;
    }
    return CY_BT_IPC_DRV_SUCCESS;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HPC_RelChannel(cy_stc_ipc_bt_context_t *btIpcContext, void * buf)
{
    IPC_STRUCT_Type *ipcPtr;
    cy_en_ipcdrv_status_t status;
    uint32_t rel_mask;
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;

    if ((NULL == contextPtr) || (NULL == buf))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    ipcPtr = Cy_IPC_Drv_GetIpcBaseAddress (contextPtr->dlChannelHPC);

    rel_mask = (uint32_t)(1 << contextPtr->intStucturePeer);
    status = Cy_IPC_Drv_LockRelease (ipcPtr, rel_mask);
    if (status) {
        return CY_BT_IPC_DRV_ERROR_LOCK_REL;
    }
    return CY_BT_IPC_DRV_SUCCESS;
}

void Cy_BTIPC_HPC_Notify(void *btIpcContext, uint32_t * msgPtr)
{
    cy_en_btipc_hpcpti_t pti;
    cy_stc_ipc_bt_context_t *contextPtr = (cy_stc_ipc_bt_context_t*) btIpcContext;
    if ((NULL == contextPtr) || (NULL == msgPtr))
        return;

    pti = (cy_en_btipc_hpcpti_t)((0xFF) & msgPtr[0]);

    switch (pti)
    {
        case CY_BT_IPC_HPC_LONG:
            Cy_bt_handle_hpclong_msg(contextPtr, msgPtr);
            break;
        case CY_BT_IPC_HPC_BUF_AVAIL:
            Cy_bt_handle_buf_add(contextPtr, msgPtr);
            break;
        case CY_BT_IPC_HPC_BUF_FREE:
            Cy_bt_handle_buf_add(contextPtr, msgPtr);
            break;
        default:
            /* default invalid pti */
            break;
    }
}

cy_en_btipcdrv_status_t Cy_BTIPC_Init(cy_stc_ipc_bt_context_t *btIpcContext, cy_stc_ipc_bt_config_t * btIpcConfig)
{
    cy_en_sysint_status_t intrStatus;
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;

    if ((NULL == contextPtr) || (NULL == btIpcConfig))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    contextPtr->dlChannelHCI = btIpcConfig->dlChannelHCI;
    contextPtr->ulChannelHCI = btIpcConfig->ulChannelHCI;

    contextPtr->dlChannelHPC = btIpcConfig->dlChannelHPC;
    contextPtr->ulChannelHPC = btIpcConfig->ulChannelHPC;

    contextPtr->intStuctureSelf = btIpcConfig->intStuctureSelf;
    contextPtr->intStucturePeer = btIpcConfig->intStucturePeer;
    contextPtr->intPeerMask = (uint32_t) (0x1 << btIpcConfig->intStucturePeer);

    contextPtr->dlNotifyCallbackPtr = btIpcConfig->dlNotifyCallbackPtr;
    contextPtr->ulReleaseCallbackPtr = btIpcConfig->ulReleaseCallbackPtr;

    contextPtr->irqHandlerPtr = btIpcConfig->irqHandlerPtr;
    contextPtr->ipcIntConfig.intrSrc = btIpcConfig->ipcIntConfig.intrSrc;
    contextPtr->ipcIntConfig.intrPriority = btIpcConfig->ipcIntConfig.intrPriority;

    contextPtr->internal_hpc_notify_cb = btIpcConfig->internal_hpc_notify_cb;

    contextPtr->dlNotifyMask = (uint32_t)((uint32_t)(0x1 << btIpcConfig->dlChannelHCI) | (uint32_t)(0x1 << btIpcConfig->dlChannelHPC));
    contextPtr->ulReleaseMask = (uint32_t)((uint32_t)(0x1 << btIpcConfig->ulChannelHCI) | (uint32_t)(0x1 << btIpcConfig->ulChannelHPC));

    for (idx = 0; idx < MAX_BT_IPC_HPC_CB; idx++)
        contextPtr->hpcNotifyCallbackPtr[idx] = NULL;

    for (idx = 0; idx < MAX_BUF_COUNT; idx++)
    {
        contextPtr->buffPool[idx].bufPtr = NULL;
        contextPtr->buffPool[idx].bufType = CY_BT_IPC_HCI_INVALID_BUF;
    }

    intrStatus = Cy_SysInt_Init(&contextPtr->ipcIntConfig, contextPtr->irqHandlerPtr);
    if (intrStatus)
    {
        return CY_BT_IPC_DRV_ERROR;
    }

    /* enable interrupt */
    NVIC_EnableIRQ(contextPtr->ipcIntConfig.intrSrc);

    /* Set IPC Interrupt mask */
    /* Allow only notify on DL channel  and release on UL channel interrupts */
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(contextPtr->intStuctureSelf), contextPtr->ulReleaseMask, contextPtr->dlNotifyMask);
    return CY_BT_IPC_DRV_SUCCESS;
 }

cy_en_btipcdrv_status_t Cy_BTIPC_Deinit(cy_stc_ipc_bt_context_t *btIpcContext)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;

    if (NULL == contextPtr)
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    //Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(contextPtr->intStuctureSelf), ~contextPtr->ulReleaseMask, ~contextPtr->dlNotifyMask);

    /* enable interrupt */
    NVIC_DisableIRQ(contextPtr->ipcIntConfig.intrSrc);

    contextPtr->irqHandlerPtr = NULL;
    contextPtr->internal_hpc_notify_cb = NULL;

    for (idx = 0; idx < MAX_BT_IPC_HPC_CB; idx++)
        contextPtr->hpcNotifyCallbackPtr[idx] = NULL;

    for (idx = 0; idx < MAX_BUF_COUNT; idx++)
    {
        contextPtr->buffPool[idx].bufPtr = NULL;
        contextPtr->buffPool[idx].bufType = CY_BT_IPC_HCI_INVALID_BUF;
    }

    return CY_BT_IPC_DRV_SUCCESS;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HCI_getPTI (cy_en_btipc_hcipti_t *pti, uint32_t *p_length, uint32_t *msgPtr)
{
    cy_stc_ipc_msg_buff_t *ipcMsgBuf;
    uint8_t *bufAddr;
    cy_en_btipc_hcipti_t mesgPti;

    if ((NULL == pti) || (NULL == msgPtr) || (NULL == p_length))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    /* Incase of long messages the PTI needs to be read from DATA0[bit 8-15] */
    mesgPti = (cy_en_btipc_hcipti_t)((0xFF) & msgPtr[0]);
    if (mesgPti == CY_BT_IPC_HCI_LONG)
    {
        ipcMsgBuf = (cy_stc_ipc_msg_buff_t*)msgPtr;
        *pti = (cy_en_btipc_hcipti_t)ipcMsgBuf->actualPti;
        bufAddr = ipcMsgBuf->bufAddr;
        if (Cy_bt_isOffsetNeeded(*pti))
            bufAddr++;
    }
    else
    {
        *pti = mesgPti;
        bufAddr = (uint8_t*)msgPtr;
        /* Skip the PTI byte read from the DATA0 register */
        bufAddr++;
    }
    *p_length = Cy_bt_getPLLegnth(*pti, bufAddr);
    return CY_BT_IPC_DRV_SUCCESS;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HCI_Read (cy_stc_ipc_bt_context_t *btIpcContext, cy_en_btipc_hcipti_t pti, void *data, size_t* pLength)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    cy_stc_ipc_msg_alloc_t ipcMsgAlloc;
    cy_stc_ipc_msg_buff_t *ipcMsgBuf;
    cy_en_btipcdrv_status_t status;
    cy_stc_ipc_msg_short_t *shortMesg;
    cy_en_btipc_hcipti_t actualPti;
    cy_en_btipc_hcipti_t mesgPti;
    uint32_t mesg[2];
    uint8_t *srcPtr;

    if ((NULL == contextPtr) || (NULL == data))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    Cy_IPC_Drv_ReadDDataValue (Cy_IPC_Drv_GetIpcBaseAddress(contextPtr->dlChannelHCI), mesg);

    mesgPti = (cy_en_btipc_hcipti_t)((0xFF) & mesg[0]);

    if (mesgPti == CY_BT_IPC_HCI_LONG)
    {
        ipcMsgBuf = (cy_stc_ipc_msg_buff_t*)&mesg;
        actualPti = (cy_en_btipc_hcipti_t)ipcMsgBuf->actualPti;
        srcPtr = ipcMsgBuf->bufAddr;
        if (Cy_bt_isOffsetNeeded(actualPti))
            srcPtr++;
        *pLength = Cy_bt_getPLLegnth(actualPti, srcPtr);
    }
    else
    {
        shortMesg = (cy_stc_ipc_msg_short_t*)&mesg;
        srcPtr = &(shortMesg->db0);
        *pLength = Cy_bt_getPLLegnth(mesgPti, srcPtr);
    }

    /* To be done: memcopy needs to be removed */
    /* Need to look for better option */
    /* May be a loop or DMA */
    memcpy((uint8_t*)data, srcPtr, *pLength);

    status = Cy_BTIPC_HCI_RelChannel(contextPtr, (void *) &mesg);

    /* Send buffer back to BLE  */
    /* To be done: Should this assert If Channel release fails? */
    if ((mesgPti == CY_BT_IPC_HCI_LONG) && (!status))
    {
        ipcMsgAlloc.pti = (uint8_t)CY_BT_IPC_HPC_BUF_FREE;
        ipcMsgAlloc.bufType = Cy_bt_get_buf_type(actualPti);
        ipcMsgAlloc.bufSize = *pLength;
        ipcMsgAlloc.bufAddr = ipcMsgBuf->bufAddr;

        status = Cy_BTIPC_HPC_Write(contextPtr, &ipcMsgAlloc, (size_t) 2);
    }

    return status;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HCI_Write(cy_stc_ipc_bt_context_t *btIpcContext, cy_en_btipc_hcipti_t pti, void *data, size_t length)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    cy_en_btipcdrv_status_t status;
    uint8_t *destBuf;
    cy_stc_ipc_msg_buff_t ipcPacket;
    cy_en_btipc_buftype_t bufType;
    cy_stc_ipc_msg_short_t ipcShort;
    uint32_t *msgPtr;
    uint8_t *bPtr;

    if ((NULL == contextPtr) || (NULL == data))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    /* Check if it is short message or buffer needed */
    if (length > MAX_SHORT_MESG_LENGTH) /* Long messge */
    {
        /* Get the buffer type based on the payload type indeicator */
        bufType = Cy_bt_get_buf_type(pti);
        /* Pick a free buffe from the pool of buffers */
        status = Cy_BTIPC_GetBuffer (contextPtr, (void **)&destBuf, bufType);
        if (status)
            return status;

        /* Copy payload to BLE buffer */
        /* To be done: memcopy needs to be removed */
        /* Need to look for better option */
        /* May be a loop or DMA */
        bPtr = destBuf;
        /* Skip pad byte if needed */
        if (Cy_bt_isOffsetNeeded(pti))
            bPtr++;
        memcpy(bPtr, data, length);
        ipcPacket.pti = (uint8_t) CY_BT_IPC_HCI_LONG;
        ipcPacket.actualPti = (uint8_t)pti;
        ipcPacket.bufAddr = destBuf;
        /* end of buffer preperation */
        msgPtr = (uint32_t*)&ipcPacket;
    }
    else /* Short Message */
    {
        ipcShort.pti = pti;
        /* To be done: memcopy needs to be removed */
        /* Need to look for better option */
        /* Loop is fine as max 8 bytes need to be copied */
        memcpy (&(ipcShort.db0), data, length);
        msgPtr = (uint32_t*)&ipcShort;
    }
    if (!Cy_IPC_Drv_SendMsgDWord (Cy_IPC_Drv_GetIpcBaseAddress(contextPtr->ulChannelHCI),
                                contextPtr->intPeerMask, msgPtr))
        return CY_BT_IPC_DRV_SUCCESS;
    else
        return CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HPC_Write(cy_stc_ipc_bt_context_t *btIpcContext, void *data, size_t length)
{
    uint32_t *dataPtr = (uint32_t*) data;
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;

    if ((NULL == contextPtr) || (NULL == data))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    if (!Cy_IPC_Drv_SendMsgDWord (Cy_IPC_Drv_GetIpcBaseAddress(contextPtr->ulChannelHPC),
                                contextPtr->intPeerMask, (uint32_t*) dataPtr))
        return CY_BT_IPC_DRV_SUCCESS;
    else
        return CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE;

}

cy_en_btipcdrv_status_t Cy_BTIPC_HPC_RegisterCb(cy_stc_ipc_bt_context_t *btIpcContext,     cy_ipc_bt_callback_ptr_t hpcNotifyCallbackPtr)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;
    uint8_t placed;

    if ((NULL == contextPtr) || (NULL == hpcNotifyCallbackPtr))
    {
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;
    }

    placed = 0;
    for (idx = 0; idx < MAX_BT_IPC_HPC_CB; idx++)
    {
        if (contextPtr->hpcNotifyCallbackPtr[idx] == NULL)
        {
            contextPtr->hpcNotifyCallbackPtr[idx] = hpcNotifyCallbackPtr;
            placed = 1;
            break;
        }
    }

    if (placed)
        return CY_BT_IPC_DRV_SUCCESS;
    else
        return CY_BT_IPC_DRV_ERROR;
}

cy_en_btipcdrv_status_t Cy_BTIPC_HPC_UnregisterCb(cy_stc_ipc_bt_context_t *btIpcContext,     cy_ipc_bt_callback_ptr_t hpcNotifyCallbackPtr)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;
    uint8_t found;

    if ((NULL == contextPtr) || (NULL == hpcNotifyCallbackPtr))
    {
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;
    }

    found = 0;
    for (idx = 0; idx < MAX_BT_IPC_HPC_CB; idx++)
    {
        if (contextPtr->hpcNotifyCallbackPtr[idx] == hpcNotifyCallbackPtr)
        {
            contextPtr->hpcNotifyCallbackPtr[idx] = NULL;
            found = 1;
            break;
        }
    }

    if (found)
        return CY_BT_IPC_DRV_SUCCESS;
    else
        return CY_BT_IPC_DRV_ERROR;
}

cy_en_btipcdrv_status_t Cy_BTIPC_GetBuffer (cy_stc_ipc_bt_context_t *btIpcContext, void **bufPtr, cy_en_btipc_buftype_t bufType)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;
    uint8_t found;
    uint32_t interruptState;

    if ((NULL == contextPtr) || (NULL == bufPtr))
    {
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;
    }

    interruptState = Cy_SysLib_EnterCriticalSection();

    found = 0;
    for (idx = 0; idx < MAX_BUF_COUNT; idx++)
    {
        if ((contextPtr->buffPool[idx].bufType == bufType) && (contextPtr->buffPool[idx].bufPtr != NULL))
        {
            *bufPtr = (void*)contextPtr->buffPool[idx].bufPtr;
            contextPtr->buffPool[idx].bufPtr = NULL;
            contextPtr->buffPool[idx].bufType = CY_BT_IPC_HCI_INVALID_BUF;
            found = 1;
            break;
        }
    }

    Cy_SysLib_ExitCriticalSection(interruptState);

    if (found)
        return CY_BT_IPC_DRV_SUCCESS;
    else
    {
        *bufPtr = NULL;
        return CY_BT_IPC_DRV_ERROR_BUF_GET;
    }
}

cy_en_btipcdrv_status_t Cy_BTIPC_PutBuffer(cy_stc_ipc_bt_context_t *btIpcContext, cy_stc_ipc_bt_buf_t *bufDecriptor)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    uint8_t idx;
    uint8_t done;
    uint32_t interruptState;

    if ((NULL == contextPtr) || (NULL == bufDecriptor) || (bufDecriptor->bufPtr == NULL))
    {
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;
    }

    interruptState = Cy_SysLib_EnterCriticalSection();

    done = 0;
    for (idx = 0; idx < MAX_BUF_COUNT; idx++)
    {
        if (contextPtr->buffPool[idx].bufPtr == NULL)
        {
            contextPtr->buffPool[idx] = *bufDecriptor;
            done = 1;
            break;
        }
    }

    Cy_SysLib_ExitCriticalSection(interruptState);

    if (done)
        return CY_BT_IPC_DRV_SUCCESS;
    else
        return CY_BT_IPC_DRV_ERROR_BUF_FULL;
}


/* Local function implmentation */
cy_en_btipcdrv_status_t Cy_bt_handle_hpclong_msg(cy_stc_ipc_bt_context_t *btIpcContext, uint32_t * msgPtr)
{
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;
    cy_stc_ipc_msg_init_t *ptr;
    cy_stc_ipc_bt_buf_t bufDescriptor;
    cy_en_btipcdrv_status_t status;
    uint8_t i;
    uint8_t bufCount;

    uint8_t *bPtr;
    uint16_t *sPtr;
    uint32_t *uiPtr;

    if ((NULL == contextPtr) || (NULL == msgPtr))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    ptr = (cy_stc_ipc_msg_init_t*)(* (msgPtr+1));

    if (NULL == ptr)
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;

    bufCount = ptr->payLoadLen/BUFFER_DESCRIPTION_LEN;
    /* Add code to extract boot type from initi structure */
    //msgId
    //bootType
    /* point to the start of buffer pool */
    ptr++;
    bPtr = (uint8_t*)ptr;

    for (i = 0; i < bufCount; i++)
    {
        bufDescriptor.bufType = (cy_en_btipc_buftype_t) (*bPtr++);
        sPtr = (uint16_t*)bPtr;
        bufDescriptor.bufLen = *(sPtr++);
        uiPtr = (uint32_t*)sPtr;
        bufDescriptor.bufPtr = (uint8_t*)(*uiPtr++);
        status = Cy_BTIPC_PutBuffer(contextPtr, &bufDescriptor);
        if (status)
        {
            /* Need to check if some cleaning needed for error condition */
            return status;
        }
        bPtr = (uint8_t*)uiPtr;
    }
    return CY_BT_IPC_DRV_SUCCESS;
}

cy_en_btipcdrv_status_t Cy_bt_handle_buf_add(cy_stc_ipc_bt_context_t *btIpcContext, uint32_t * msgPtr)
{
    cy_stc_ipc_bt_buf_t bufDescriptor;
    cy_stc_ipc_msg_alloc_t allocBuf;
    cy_stc_ipc_bt_context_t *contextPtr = btIpcContext;

    if ((NULL == contextPtr) || (NULL == msgPtr))
        return CY_BT_IPC_DRV_ERROR_BAD_HANDLE;
    allocBuf = *(cy_stc_ipc_msg_alloc_t*)(msgPtr);
    bufDescriptor.bufType = (cy_en_btipc_buftype_t) allocBuf.bufType;
    bufDescriptor.bufLen = allocBuf.bufSize;
    bufDescriptor.bufPtr = (uint8_t*) allocBuf.bufAddr;

    return Cy_BTIPC_PutBuffer(contextPtr, &bufDescriptor);
}

cy_en_btipc_buftype_t Cy_bt_get_buf_type(cy_en_btipc_hcipti_t pti)
{
    cy_en_btipc_buftype_t bufType;
    /* To be done: Currently retruning Control buffer for all PTIs. Need to change it once we have clarity on it */
    switch (pti)
    {
        case CY_BT_IPC_HCI_CMD:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_ACL:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_SCO:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_EVT:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_ISO:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_DIAG:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_MPAF:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        case CY_BT_IPC_HCI_SLIPH5:
            bufType = CY_BT_IPC_CTRL_BUF;
            break;
        default:
            bufType = CY_BT_IPC_HCI_INVALID_BUF;
            break;
    }
    return bufType;
}

uint32_t Cy_bt_getPLLegnth(cy_en_btipc_hcipti_t pti, uint8_t* bufAddr)
{
    uint32_t length;
    switch (pti)
    {
        case CY_BT_IPC_HCI_CMD:
            length = (uint32_t)(((BTHCI_CMD_HDR_t*)bufAddr)->params_len);
            length += sizeof(BTHCI_CMD_HDR_t);
            break;
        case CY_BT_IPC_HCI_ACL:
            length = (uint32_t)(((BTHCI_ACL_HDR_t*)bufAddr)->data_len);
            length += sizeof(BTHCI_ACL_HDR_t);
            break;
        case CY_BT_IPC_HCI_SCO:
            length = (uint32_t)(((BTHCI_SCO_HDR_t*)bufAddr)->data_len);
            length += sizeof(BTHCI_SCO_HDR_t);
            break;
        case CY_BT_IPC_HCI_EVT:
            length = (uint32_t)(((BTHCI_EVENT_HDR_t*)bufAddr)->params_len);
            length += sizeof(BTHCI_EVENT_HDR_t);
            break;
        case CY_BT_IPC_HCI_ISO:
            length = (uint32_t)(((BTHCI_ISO_HDR_t*)bufAddr)->data_len);
            length += sizeof(BTHCI_ISO_HDR_t);
            break;
        /* To be done: Header length for the remaining PTI needs to be updated */
        case CY_BT_IPC_HCI_DIAG:
            length = 0;
            break;
        case CY_BT_IPC_HCI_MPAF:
            length = 0;
            break;
        case CY_BT_IPC_HCI_SLIPH5:
            length = 0;
            break;
        default:
            length = 0;
            break;
    }
    return length;
}

bool Cy_bt_isOffsetNeeded(cy_en_btipc_hcipti_t pti)
{
    switch (pti)
    {
        case CY_BT_IPC_HCI_CMD:
        case CY_BT_IPC_HCI_SCO:
            return true;
        case CY_BT_IPC_HCI_ACL:
        case CY_BT_IPC_HCI_EVT:
        case CY_BT_IPC_HCI_ISO:
        case CY_BT_IPC_HCI_DIAG:
        case CY_BT_IPC_HCI_MPAF:
        case CY_BT_IPC_HCI_SLIPH5:
        default:
            return false;
    }
}

#endif

