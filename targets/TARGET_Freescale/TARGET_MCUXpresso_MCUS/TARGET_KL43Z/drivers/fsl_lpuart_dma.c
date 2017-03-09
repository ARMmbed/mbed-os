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
#include "fsl_lpuart_dma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! Structure definition for lpuart_dma_handle_t. The structure is private. */
typedef struct _lpuart_dma_private_handle
{
    LPUART_Type *base;
    lpuart_dma_handle_t *handle;
} lpuart_dma_private_handle_t;

/* LPUART DMA transfer handle. */
enum _uart_dma_tansfer_states
{
    kLPUART_TxIdle, /* TX idle. */
    kLPUART_TxBusy, /* TX busy. */
    kLPUART_RxIdle, /* RX idle. */
    kLPUART_RxBusy  /* RX busy. */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*<! Private handle only used for internally. */
static lpuart_dma_private_handle_t s_dmaPrivateHandle[FSL_FEATURE_SOC_LPUART_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief LPUART DMA send finished callback function.
 *
 * This function is called when LPUART DMA send finished. It disables the LPUART
 * TX DMA request and sends @ref kStatus_LPUART_TxIdle to LPUART callback.
 *
 * @param handle The DMA handle.
 * @param param Callback function parameter.
 */
static void LPUART_TransferSendDMACallback(dma_handle_t *handle, void *param);

/*!
 * @brief LPUART DMA receive finished callback function.
 *
 * This function is called when LPUART DMA receive finished. It disables the LPUART
 * RX DMA request and sends @ref kStatus_LPUART_RxIdle to LPUART callback.
 *
 * @param handle The DMA handle.
 * @param param Callback function parameter.
 */
static void LPUART_TransferReceiveDMACallback(dma_handle_t *handle, void *param);

/*!
 * @brief Get the LPUART instance from peripheral base address.
 *
 * @param base LPUART peripheral base address.
 * @return LPUART instance.
 */
extern uint32_t LPUART_GetInstance(LPUART_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void LPUART_TransferSendDMACallback(dma_handle_t *handle, void *param)
{
    lpuart_dma_private_handle_t *lpuartPrivateHandle = (lpuart_dma_private_handle_t *)param;

    /* Disable LPUART TX DMA. */
    LPUART_EnableTxDMA(lpuartPrivateHandle->base, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(lpuartPrivateHandle->handle->txDmaHandle->base,
                          lpuartPrivateHandle->handle->txDmaHandle->channel);

    lpuartPrivateHandle->handle->txState = kLPUART_TxIdle;

    if (lpuartPrivateHandle->handle->callback)
    {
        lpuartPrivateHandle->handle->callback(lpuartPrivateHandle->base, lpuartPrivateHandle->handle,
                                              kStatus_LPUART_TxIdle, lpuartPrivateHandle->handle->userData);
    }
}

static void LPUART_TransferReceiveDMACallback(dma_handle_t *handle, void *param)
{
    lpuart_dma_private_handle_t *lpuartPrivateHandle = (lpuart_dma_private_handle_t *)param;

    /* Disable LPUART RX DMA. */
    LPUART_EnableRxDMA(lpuartPrivateHandle->base, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(lpuartPrivateHandle->handle->rxDmaHandle->base,
                          lpuartPrivateHandle->handle->rxDmaHandle->channel);

    lpuartPrivateHandle->handle->rxState = kLPUART_RxIdle;

    if (lpuartPrivateHandle->handle->callback)
    {
        lpuartPrivateHandle->handle->callback(lpuartPrivateHandle->base, lpuartPrivateHandle->handle,
                                              kStatus_LPUART_RxIdle, lpuartPrivateHandle->handle->userData);
    }
}

void LPUART_TransferCreateHandleDMA(LPUART_Type *base,
                            lpuart_dma_handle_t *handle,
                            lpuart_dma_transfer_callback_t callback,
                            void *userData,
                            dma_handle_t *txDmaHandle,
                            dma_handle_t *rxDmaHandle)
{
    assert(handle);

    uint32_t instance = LPUART_GetInstance(base);

    memset(handle, 0, sizeof(lpuart_dma_handle_t));

    s_dmaPrivateHandle[instance].base = base;
    s_dmaPrivateHandle[instance].handle = handle;

    handle->rxState = kLPUART_RxIdle;
    handle->txState = kLPUART_TxIdle;

    handle->callback = callback;
    handle->userData = userData;

#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    /* Note:
       Take care of the RX FIFO, DMA request only assert when received bytes
       equal or more than RX water mark, there is potential issue if RX water
       mark larger than 1.
       For example, if RX FIFO water mark is 2, upper layer needs 5 bytes and
       5 bytes are received. the last byte will be saved in FIFO but not trigger
       DMA transfer because the water mark is 2.
     */
    if (rxDmaHandle)
    {
        base->WATER &= (~LPUART_WATER_RXWATER_MASK);
    }
#endif

    handle->rxDmaHandle = rxDmaHandle;
    handle->txDmaHandle = txDmaHandle;

    /* Configure TX. */
    if (txDmaHandle)
    {
        DMA_SetCallback(txDmaHandle, LPUART_TransferSendDMACallback, &s_dmaPrivateHandle[instance]);
    }

    /* Configure RX. */
    if (rxDmaHandle)
    {
        DMA_SetCallback(rxDmaHandle, LPUART_TransferReceiveDMACallback, &s_dmaPrivateHandle[instance]);
    }
}

status_t LPUART_TransferSendDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, lpuart_transfer_t *xfer)
{
    assert(handle->txDmaHandle);

    status_t status;
    dma_transfer_config_t xferConfig;

    /* Return error if xfer invalid. */
    if ((0U == xfer->dataSize) || (NULL == xfer->data))
    {
        return kStatus_InvalidArgument;
    }

    /* If previous TX not finished. */
    if (kLPUART_TxBusy == handle->txState)
    {
        status = kStatus_LPUART_TxBusy;
    }
    else
    {
        handle->txState = kLPUART_TxBusy;
        handle->txDataSizeAll = xfer->dataSize;

        /* Prepare transfer. */
        DMA_PrepareTransfer(&xferConfig, xfer->data, sizeof(uint8_t), (void *)LPUART_GetDataRegisterAddress(base),
                            sizeof(uint8_t), xfer->dataSize, kDMA_MemoryToPeripheral);

        /* Submit transfer. */
        DMA_SubmitTransfer(handle->txDmaHandle, &xferConfig, kDMA_EnableInterrupt);
        DMA_StartTransfer(handle->txDmaHandle);

        /* Enable LPUART TX DMA. */
        LPUART_EnableTxDMA(base, true);

        status = kStatus_Success;
    }

    return status;
}

status_t LPUART_TransferReceiveDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, lpuart_transfer_t *xfer)
{
    assert(handle->rxDmaHandle);

    status_t status;
    dma_transfer_config_t xferConfig;

    /* Return error if xfer invalid. */
    if ((0U == xfer->dataSize) || (NULL == xfer->data))
    {
        return kStatus_InvalidArgument;
    }

    /* If previous RX not finished. */
    if (kLPUART_RxBusy == handle->rxState)
    {
        status = kStatus_LPUART_RxBusy;
    }
    else
    {
        handle->rxState = kLPUART_RxBusy;
        handle->rxDataSizeAll = xfer->dataSize;

        /* Prepare transfer. */
        DMA_PrepareTransfer(&xferConfig, (void *)LPUART_GetDataRegisterAddress(base), sizeof(uint8_t), xfer->data,
                            sizeof(uint8_t), xfer->dataSize, kDMA_PeripheralToMemory);

        /* Submit transfer. */
        DMA_SubmitTransfer(handle->rxDmaHandle, &xferConfig, kDMA_EnableInterrupt);
        DMA_StartTransfer(handle->rxDmaHandle);

        /* Enable LPUART RX DMA. */
        LPUART_EnableRxDMA(base, true);

        status = kStatus_Success;
    }

    return status;
}

void LPUART_TransferAbortSendDMA(LPUART_Type *base, lpuart_dma_handle_t *handle)
{
    assert(handle->txDmaHandle);

    /* Disable LPUART TX DMA. */
    LPUART_EnableTxDMA(base, false);

    /* Stop transfer. */
    DMA_AbortTransfer(handle->txDmaHandle);

    /* Write DMA->DSR[DONE] to abort transfer and clear status. */
    DMA_ClearChannelStatusFlags(handle->txDmaHandle->base, handle->txDmaHandle->channel, kDMA_TransactionsDoneFlag);

    handle->txState = kLPUART_TxIdle;
}

void LPUART_TransferAbortReceiveDMA(LPUART_Type *base, lpuart_dma_handle_t *handle)
{
    assert(handle->rxDmaHandle);

    /* Disable LPUART RX DMA. */
    LPUART_EnableRxDMA(base, false);

    /* Stop transfer. */
    DMA_AbortTransfer(handle->rxDmaHandle);

    /* Write DMA->DSR[DONE] to abort transfer and clear status. */
    DMA_ClearChannelStatusFlags(handle->rxDmaHandle->base, handle->rxDmaHandle->channel, kDMA_TransactionsDoneFlag);

    handle->rxState = kLPUART_RxIdle;
}

status_t LPUART_TransferGetSendCountDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, uint32_t *count)
{
    assert(handle->txDmaHandle);

    if (kLPUART_TxIdle == handle->txState)
    {
        return kStatus_NoTransferInProgress;
    }

    if (!count)
    {
        return kStatus_InvalidArgument;
    }

    *count = handle->txDataSizeAll - DMA_GetRemainingBytes(handle->txDmaHandle->base, handle->txDmaHandle->channel);

    return kStatus_Success;
}

status_t LPUART_TransferGetReceiveCountDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, uint32_t *count)
{
    assert(handle->rxDmaHandle);

    if (kLPUART_RxIdle == handle->rxState)
    {
        return kStatus_NoTransferInProgress;
    }

    if (!count)
    {
        return kStatus_InvalidArgument;
    }

    *count = handle->rxDataSizeAll - DMA_GetRemainingBytes(handle->rxDmaHandle->base, handle->rxDmaHandle->channel);

    return kStatus_Success;
}
