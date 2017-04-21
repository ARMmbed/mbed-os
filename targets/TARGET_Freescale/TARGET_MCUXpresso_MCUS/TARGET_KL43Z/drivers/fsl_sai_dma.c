/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_sai_dma.h"

/*******************************************************************************
* Definitions
******************************************************************************/
/*<! Structure definition for sai_dma_private_handle_t. The structure is private. */
typedef struct _sai_dma_private_handle
{
    I2S_Type *base;
    sai_dma_handle_t *handle;
} sai_dma_private_handle_t;

enum _sai_dma_states
{
    kSAI_Idle = 0x0U,
    kSAI_Busy = 0x1U,
};

/*<! Private handle only used for internally. */
static sai_dma_private_handle_t s_dmaPrivateHandle[FSL_FEATURE_SOC_I2S_COUNT][2];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the instance number for SAI.
 *
 * @param base SAI base pointer.
 */
extern uint32_t SAI_GetInstance(I2S_Type *base);

/*!
 * @brief SAI EDMA callback for send.
 *
 * @param handle pointer to sai_dma_handle_t structure which stores the transfer state.
 * @param userData Parameter for user callback.
 */
static void SAI_TxDMACallback(dma_handle_t *handle, void *userData);

/*!
 * @brief SAI EDMA callback for receive.
 *
 * @param handle pointer to sai_dma_handle_t structure which stores the transfer state.
 * @param userData Parameter for user callback.
 */
static void SAI_RxDMACallback(dma_handle_t *handle, void *userData);

/*******************************************************************************
* Code
******************************************************************************/
static void SAI_TxDMACallback(dma_handle_t *handle, void *userData)
{
    sai_dma_private_handle_t *privHandle = (sai_dma_private_handle_t *)userData;
    sai_dma_handle_t *saiHandle = privHandle->handle;

    /* Update queue counter */
    memset(&saiHandle->saiQueue[saiHandle->queueDriver], 0, sizeof(sai_transfer_t));
    saiHandle->queueDriver = (saiHandle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

    /* Call callback function */
    if (saiHandle->callback)
    {
        (saiHandle->callback)(privHandle->base, saiHandle, kStatus_SAI_TxIdle, saiHandle->userData);
    }

    /* If all data finished, just stop the transfer */
    if (saiHandle->saiQueue[saiHandle->queueDriver].data == NULL)
    {
        SAI_TransferAbortSendDMA(privHandle->base, saiHandle);
    }
}

static void SAI_RxDMACallback(dma_handle_t *handle, void *userData)
{
    sai_dma_private_handle_t *privHandle = (sai_dma_private_handle_t *)userData;
    sai_dma_handle_t *saiHandle = privHandle->handle;

    /* Update queue counter */
    memset(&saiHandle->saiQueue[saiHandle->queueDriver], 0, sizeof(sai_transfer_t));
    saiHandle->queueDriver = (saiHandle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

    /* Call callback function */
    if (saiHandle->callback)
    {
        (saiHandle->callback)(privHandle->base, saiHandle, kStatus_SAI_RxIdle, saiHandle->userData);
    }

    /* If all data finished, just stop the transfer */
    if (saiHandle->saiQueue[saiHandle->queueDriver].data == NULL)
    {
        SAI_TransferAbortReceiveDMA(privHandle->base, saiHandle);
    }
}

void SAI_TransferTxCreateHandleDMA(
    I2S_Type *base, sai_dma_handle_t *handle, sai_dma_callback_t callback, void *userData, dma_handle_t *dmaHandle)
{
    assert(handle && dmaHandle);

    uint32_t instance = SAI_GetInstance(base);

    /* Set sai base to handle */
    handle->dmaHandle = dmaHandle;
    handle->callback = callback;
    handle->userData = userData;

    /* Set SAI state to idle */
    handle->state = kSAI_Idle;

    s_dmaPrivateHandle[instance][0].base = base;
    s_dmaPrivateHandle[instance][0].handle = handle;

/* Use FIFO error continue nstead of using interrupt to handle error */
#if defined(FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR) && (FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR)
    base->TCR4 |= I2S_TCR4_FCONT_MASK;
#endif

    /* Install callback for Tx dma channel */
    DMA_SetCallback(dmaHandle, SAI_TxDMACallback, &s_dmaPrivateHandle[instance][0]);
}

void SAI_TransferRxCreateHandleDMA(
    I2S_Type *base, sai_dma_handle_t *handle, sai_dma_callback_t callback, void *userData, dma_handle_t *dmaHandle)
{
    assert(handle && dmaHandle);

    uint32_t instance = SAI_GetInstance(base);

    /* Set sai base to handle */
    handle->dmaHandle = dmaHandle;
    handle->callback = callback;
    handle->userData = userData;

    /* Set SAI state to idle */
    handle->state = kSAI_Idle;

    s_dmaPrivateHandle[instance][1].base = base;
    s_dmaPrivateHandle[instance][1].handle = handle;

/* Use FIFO error continue nstead of using interrupt to handle error */
#if defined(FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR) && (FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR)
    base->RCR4 |= I2S_RCR4_FCONT_MASK;
#endif

    /* Install callback for Tx dma channel */
    DMA_SetCallback(dmaHandle, SAI_RxDMACallback, &s_dmaPrivateHandle[instance][1]);
}

void SAI_TransferTxSetFormatDMA(I2S_Type *base,
                                sai_dma_handle_t *handle,
                                sai_transfer_format_t *format,
                                uint32_t mclkSourceClockHz,
                                uint32_t bclkSourceClockHz)
{
    assert(handle && format);

    dma_transfer_config_t config = {0};

    /* Configure the audio format to SAI registers */
    SAI_TxSetFormat(base, format, mclkSourceClockHz, bclkSourceClockHz);

    /* Update the information in handle */
    handle->channel = format->channel;

    /* Configure the data format into DMA register */
    config.destAddr = SAI_TxGetDataRegisterAddress(base, format->channel);
    config.enableDestIncrement = false;
    config.enableSrcIncrement = true;
    switch (format->bitWidth)
    {
        case 8:
            config.srcSize = kDMA_Transfersize8bits;
            config.destSize = kDMA_Transfersize8bits;
            handle->bytesPerFrame = 1U;
            break;
        case 16:
            config.srcSize = kDMA_Transfersize16bits;
            config.destSize = kDMA_Transfersize16bits;
            handle->bytesPerFrame = 2U;
            break;
        default:
            config.srcSize = kDMA_Transfersize32bits;
            config.destSize = kDMA_Transfersize32bits;
            handle->bytesPerFrame = 4U;
            break;
    }

    /* Configure DMA channel */
    DMA_SubmitTransfer(handle->dmaHandle, &config, true);
}

void SAI_TransferRxSetFormatDMA(I2S_Type *base,
                                sai_dma_handle_t *handle,
                                sai_transfer_format_t *format,
                                uint32_t mclkSourceClockHz,
                                uint32_t bclkSourceClockHz)
{
    assert(handle && format);

    dma_transfer_config_t config = {0};

    /* Configure the audio format to SAI registers */
    SAI_RxSetFormat(base, format, mclkSourceClockHz, bclkSourceClockHz);
    handle->channel = format->channel;

    /* Configure the data format into DMA register */
    config.srcAddr = SAI_RxGetDataRegisterAddress(base, format->channel);
    config.enableDestIncrement = true;
    config.enableSrcIncrement = false;
    switch (format->bitWidth)
    {
        case 8:
            config.srcSize = kDMA_Transfersize8bits;
            config.destSize = kDMA_Transfersize8bits;
            handle->bytesPerFrame = 1U;
            break;
        case 16:
            config.srcSize = kDMA_Transfersize16bits;
            config.destSize = kDMA_Transfersize16bits;
            handle->bytesPerFrame = 2U;
            break;
        default:
            config.srcSize = kDMA_Transfersize32bits;
            config.destSize = kDMA_Transfersize32bits;
            handle->bytesPerFrame = 4U;
            break;
    }

    /* Configure DMA channel */
    DMA_SubmitTransfer(handle->dmaHandle, &config, true);
}

status_t SAI_TransferSendDMA(I2S_Type *base, sai_dma_handle_t *handle, sai_transfer_t *xfer)
{
    assert(handle && xfer);

    /* Check if input parameter invalid */
    if ((xfer->data == NULL) || (xfer->dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    if (handle->saiQueue[handle->queueUser].data)
    {
        return kStatus_SAI_QueueFull;
    }

    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the source address */
    DMA_SetSourceAddress(handle->dmaHandle->base, handle->dmaHandle->channel, (uint32_t)(xfer->data));

    /* Set the transfer size */
    DMA_SetTransferSize(handle->dmaHandle->base, handle->dmaHandle->channel, xfer->dataSize);

    /* Change the state of handle */
    handle->state = kSAI_Busy;

    /* Start DMA transfer */
    DMA_StartTransfer(handle->dmaHandle);

/* Enable DMA request and start SAI */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    SAI_TxEnableDMA(base, kSAI_FIFORequestDMAEnable, true);
#else
    SAI_TxEnableDMA(base, kSAI_FIFOWarningDMAEnable, true);
#endif
    SAI_TxEnable(base, true);

    return kStatus_Success;
}

status_t SAI_TransferReceiveDMA(I2S_Type *base, sai_dma_handle_t *handle, sai_transfer_t *xfer)
{
    assert(handle && xfer);

    /* Check if input parameter invalid */
    if ((xfer->data == NULL) || (xfer->dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    if (handle->saiQueue[handle->queueUser].data)
    {
        return kStatus_SAI_QueueFull;
    }

    /* Add into queue */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the source address */
    DMA_SetDestinationAddress(handle->dmaHandle->base, handle->dmaHandle->channel, (uint32_t)(xfer->data));

    /* Set the transfer size */
    DMA_SetTransferSize(handle->dmaHandle->base, handle->dmaHandle->channel, xfer->dataSize);

    /* Change the state of handle */
    handle->state = kSAI_Busy;

    /* Start DMA transfer */
    DMA_StartTransfer(handle->dmaHandle);

/* Enable DMA request and start SAI */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    SAI_RxEnableDMA(base, kSAI_FIFORequestDMAEnable, true);
#else
    SAI_RxEnableDMA(base, kSAI_FIFOWarningDMAEnable, true);
#endif
    SAI_RxEnable(base, true);

    return kStatus_Success;
}

void SAI_TransferAbortSendDMA(I2S_Type *base, sai_dma_handle_t *handle)
{
    assert(handle);

    /* Disable dma */
    DMA_AbortTransfer(handle->dmaHandle);

/* Disable DMA enable bit */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    SAI_TxEnableDMA(base, kSAI_FIFORequestDMAEnable, false);
#else
    SAI_TxEnableDMA(base, kSAI_FIFOWarningDMAEnable, false);
#endif

    /* Set the handle state */
    handle->state = kSAI_Idle;

    /* Clear the queue */
    memset(handle->saiQueue, 0, sizeof(sai_transfer_t) * SAI_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

void SAI_TransferAbortReceiveDMA(I2S_Type *base, sai_dma_handle_t *handle)
{
    assert(handle);

    /* Disable dma */
    DMA_AbortTransfer(handle->dmaHandle);

/* Disable DMA enable bit */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    SAI_RxEnableDMA(base, kSAI_FIFORequestDMAEnable, false);
#else
    SAI_RxEnableDMA(base, kSAI_FIFOWarningDMAEnable, false);
#endif
    /* Set the handle state */
    handle->state = kSAI_Idle;

    /* Clear the queue */
    memset(handle->saiQueue, 0, sizeof(sai_transfer_t) * SAI_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

status_t SAI_TransferGetSendCountDMA(I2S_Type *base, sai_dma_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = handle->transferSize[handle->queueDriver] -
                 DMA_GetRemainingBytes(handle->dmaHandle->base, handle->dmaHandle->channel);
    }

    return status;
}

status_t SAI_TransferGetReceiveCountDMA(I2S_Type *base, sai_dma_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = handle->transferSize[handle->queueDriver] -
                 DMA_GetRemainingBytes(handle->dmaHandle->base, handle->dmaHandle->channel);
    }

    return status;
}
